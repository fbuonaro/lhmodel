#ifndef __LHMODELUTIL_RAPIDJSON_H__
#define __LHMODELUTIL_RAPIDJSON_H__

#include <deque>
#include <stdexcept>
#include <sstream>
#include <string>
#include <tuple>

#include <lhmodel/visitor.h>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace LHModelUtilNS
{
    using namespace LHModelNS;

    template< typename W, typename T >
    void SerializeModel( W& writer, const T& model );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, const std::string& value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, bool value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, int value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, unsigned value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, int64_t value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, uint64_t value );

    template< typename W >
    void defaultSerialize( W& w, bool inKey, double value );

    template< typename W, typename T, typename = void >
    class PrimitiveSeriailzer
    {
        public:
            static void serialize( W& w, bool inKey, const T& value )
            {
                defaultSerialize( w, inKey, value );
            }
    };

    enum class SerializationContext
    {
        InKey,
        InMap,
        InModel,
        InValue
    };

    template< typename W >
    class JsonSerializerSAX
    {
        public:
            template< typename U, typename T >
            friend void SerializeModel( U& writer, const T& model );

        protected:
            typedef W WriterType;

            // NOTE: stores w as a reference
            //       expects caller to call StartObject before and EndObject after passing this to Accept
            JsonSerializerSAX( WriterType& _w )
            :   w( _w )
            ,   contextStack()
            {
                contextStack.push_back( SerializationContext::InModel );
            }

            template< typename T >
            bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& modelMember )
            {
                if( contextStack.back() == SerializationContext::InModel )
                    w.Key( memberMeta.name );

                w.StartObject();

                contextStack.push_back( SerializationContext::InModel );

                return true;
            }

            template< typename T >
            void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
            {
                contextStack.pop_back();

                w.EndObject();
            }

            template< typename T >
            bool EnterArrayMember( const MemberMeta& memberMeta, const T& arrayMember )
            {
                if( contextStack.back() == SerializationContext::InModel )
                    w.Key( memberMeta.name );

                w.StartArray();

                contextStack.push_back( SerializationContext::InValue );

                return true;
            }

            bool EnterArrayValue( const MemberMeta& memberMeta )
            {
                return true;
            }

            void LeaveArrayValue( const MemberMeta& memberMeta )
            {
                return;
            }

            template< typename T >
            void LeaveArrayMember( const MemberMeta& memberMeta, const T& vectorMember )
            {
                contextStack.pop_back();

                w.EndArray();
            }

            template< typename T >
            void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
            {
                if( contextStack.back() == SerializationContext::InModel )
                    w.Key( memberMeta.name );

                PrimitiveSeriailzer< W, T >::serialize( w,
                                                        contextStack.back() == SerializationContext::InKey,
                                                        primitiveMember );
            }

            template< typename T >
            bool EnterMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                if( contextStack.back() == SerializationContext::InModel )
                    w.Key( memberMeta.name );

                w.StartObject();

                contextStack.push_back( SerializationContext::InMap );

                return true;
            }

            bool EnterMapEntryKey( const MemberMeta& memberMeta )
            {
                contextStack.push_back( SerializationContext::InKey );

                return true;
            }

            void LeaveMapEntryKey( const MemberMeta& memberMeta )
            {
                contextStack.pop_back();
            }

            bool EnterMapEntryValue( const MemberMeta& memberMeta )
            {
                contextStack.push_back( SerializationContext::InValue );

                return true;
            }

            void LeaveMapEntryValue( const MemberMeta& memberMeta )
            {
                contextStack.pop_back();
            }

            template< typename T >
            void LeaveMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                contextStack.pop_back();

                w.EndObject();
            }

        private:
            WriterType& w;
            std::deque< SerializationContext > contextStack;
    };

    template< typename T, typename V >
    void DeserializeValue( T& model, const V& value );

    template< typename T, typename V >
    bool DeserializeValue( T& model, const V& value, std::ostream* trace, bool failFast );

    template< typename T, typename V >
    bool defaultDeserialize( T& member, const V& value, std::string& failureReason );

    template< typename V >
    bool defaultDeserialize( std::string& member, const V& value, std::string& failureReason );

    template< typename T, typename V, typename = void >
    class PrimitiveDeserializer
    {
        public:
            static bool deserialize( T& member, const V& value, std::string& failureReason )
            {
                return defaultDeserialize( member, value, failureReason );
            }
    };

    enum class ContainerType
    {
        Array,
        Map,
        Model
    };

    class DeseriailzationFailure : public std::runtime_error
    {
        public:
            DeseriailzationFailure( const std::string& failure );
            virtual ~DeseriailzationFailure();
    };

    template< typename V >
    class JsonDeserializerDOM
    {
        public:
            template< typename T, typename U >
            friend void DeserializeValue( T& model, const U& value );

            template< typename T, typename U >
            friend bool DeserializeValue( T& model, const U& value, std::ostream* trace, bool failFast );

        protected:
            typedef V ValueType;

            enum class FailureHandling
            {
                Throw,
                FailFast,
                None
            };

            // NOTE: stores v as a reference
            JsonDeserializerDOM( const ValueType& _v, FailureHandling _failureHandling, std::ostream* _trace )
            :   v( _v )
            ,   containerStack()
            ,   mapContextStack()
            ,   arrayContextStack()
            ,   failureHandling( _failureHandling )
            ,   trace( _trace )
            ,   firstTrace( true )
            ,   anyFailures( false )
            ,   valid( true )
            {
                if( !v.IsObject() )
                {
                    std::ostringstream oss;

                    oss << "expected object, received " << v.GetType();

                    handleFailure( oss.str(), false );
                }
                else
                {
                    containerStack.push_back( std::make_tuple( &v, ContainerType::Model ) );
                }
            }

            JsonDeserializerDOM( const ValueType& _v )
            :   JsonDeserializerDOM( _v, FailureHandling::Throw, nullptr )
            {
            }

            JsonDeserializerDOM( const ValueType& _v, std::ostream* _trace, bool failFast )
            :   JsonDeserializerDOM( _v, ( failFast ? FailureHandling::FailFast : FailureHandling::None ), _trace )
            {
            }

            template< typename T >
            bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& modelMember )
            {
                if( !valid )
                    return false;

                const ValueType* nextValue = getNextValue( memberMeta );

                if( !nextValue )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] not found";

                    handleFailure( oss.str(), true );

                    return false;
                }

                if( !nextValue->IsObject() )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] expected object, encountered " << nextValue->GetType();

                    handleFailure( oss.str(), true );

                    return false;
                }


                containerStack.push_back( std::make_tuple( nextValue, ContainerType::Model ) );

                return true;
            }

            template< typename T >
            void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& model )
            {
                containerStack.pop_back();
            }

            template< typename T >
            bool EnterArrayMember( const MemberMeta& memberMeta, T& arrayMember )
            {
                if( !valid )
                    return false;

                const ValueType* nextValue = getNextValue( memberMeta );

                if( !nextValue )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] not found";

                    handleFailure( oss.str(), true );

                    return false;
                }

                if( !nextValue->IsArray() )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] expected array, encountered " << nextValue->GetType();

                    handleFailure( oss.str(), true );

                    return false;
                }


                containerStack.push_back( std::make_tuple( nextValue, ContainerType::Array ) );
                arrayContextStack.push_back( std::make_tuple( nextValue->Begin(), nextValue->End() ) );

                return true;
            }

            bool HasMoreArrayValues( const MemberMeta& memberMeta )
            {
                assert( arrayContextStack.size() );

                return ( std::get< 0 >( arrayContextStack.back() ) < std::get< 1 >( arrayContextStack.back() ) );
            }

            bool EnterArrayValue( const MemberMeta& memberMeta )
            {
                return valid;
            }

            void LeaveArrayValue( const MemberMeta& memberMeta )
            {
                assert( mapContextStack.size() );

                // go to next array value
                ++( std::get< 0 >( arrayContextStack.back() ) );
            }

            template< typename T >
            void LeaveArrayMember( const MemberMeta& memberMeta, T& vectorMember )
            {
                containerStack.pop_back();
                arrayContextStack.pop_back();
            }

            template< typename T >
            void VisitPrimitiveMember( const MemberMeta& memberMeta, T& primitiveMember )
            {
                if( !valid )
                    return;

                const ValueType* nextValue = getNextValue( memberMeta );

                if( !nextValue )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] not found";

                    handleFailure( oss.str(), true );

                    return;
                }

                std::string failureReason; 
                if( !( PrimitiveDeserializer< T, V >::deserialize( primitiveMember,
                                                                   *nextValue,
                                                                   failureReason ) ) )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] failed to be deserialized[" << failureReason << "]";

                    handleFailure( oss.str(), true );
                }
            }

            template< typename T >
            bool EnterMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                if( !valid )
                    return false;

                const ValueType* nextValue = getNextValue( memberMeta );

                if( !nextValue )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] not found";

                    handleFailure( oss.str(), true );

                    return false;
                }

                if( !nextValue->IsObject() )
                {
                    std::ostringstream oss;

                    oss << "member[" << memberMeta << "] expected object, encountered " << nextValue->GetType();

                    handleFailure( oss.str(), true );

                    return false;
                }

                containerStack.push_back( std::make_tuple( nextValue, ContainerType::Map ) );
                mapContextStack.push_back(
                    std::make_tuple(
                        nextValue->MemberBegin(),
                        nextValue->MemberEnd(),
                        nullptr ) );

                return true;
            }

            bool HasMoreMapEntries( const MemberMeta& memberMeta )
            {
                assert( mapContextStack.size() );

                return ( std::get< 0 >( mapContextStack.back() ) < std::get< 1 >( mapContextStack.back() ) );
            }

            bool EnterMapEntryKey( const MemberMeta& memberMeta )
            {
                if( !valid )
                    return false;

                assert( mapContextStack.size() );
                assert( std::get< 0 >( mapContextStack.back() ) !=
                        std::get< 1 >( mapContextStack.back() ) );

                std::get< 2 >( mapContextStack.back() ) = &std::get< 0 >( mapContextStack.back() )->name;

                return true;
            }

            void LeaveMapEntryKey( const MemberMeta& memberMeta )
            {
                assert( mapContextStack.size() );
                std::get< 2 >( mapContextStack.back() ) = nullptr;
            }

            bool EnterMapEntryValue( const MemberMeta& memberMeta )
            {
                if( !valid )
                    return false;

                assert( mapContextStack.size() );
                std::get< 2 >( mapContextStack.back() ) = &std::get< 0 >( mapContextStack.back() )->value;

                return true;
            }

            void LeaveMapEntryValue( const MemberMeta& memberMeta )
            {
                assert( mapContextStack.size() );
                // go to next map entry
                std::get< 2 >( mapContextStack.back() ) = nullptr;
                ++( std::get< 0 >( mapContextStack.back() ) );
            }

            template< typename T >
            void LeaveMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                containerStack.pop_back();
                mapContextStack.pop_back();
            }

            bool AnyFailures() const
            {
                return anyFailures || !valid;
            }

        private:
            typedef std::tuple< const ValueType*, ContainerType > ContainerContext;
            typedef typename ValueType::ConstMemberIterator MembIt;
            typedef typename ValueType::ConstValueIterator ValueIt;

            // start|end|next values in map/array
            typedef std::tuple< MembIt, MembIt, const ValueType* > MapContext;
            typedef std::tuple< ValueIt, ValueIt > ArrayContext;

            const ValueType* getNextValue( const MemberMeta& memberMeta )
            {
                assert( containerStack.size() );
                const ContainerContext& currentContainer = containerStack.back();

                switch( std::get< 1 >( currentContainer ) )
                {
                    case( ContainerType::Array ):
                    {
                        assert( arrayContextStack.size() );
                        assert( std::get< 0 >( arrayContextStack.back() ) !=
                                std::get< 1 >( arrayContextStack.back() ) );

                        return std::get< 0 >( arrayContextStack.back() );
                    }
                    case( ContainerType::Map ):
                    {
                        assert( mapContextStack.size() );
                        assert( std::get< 2 >( mapContextStack.back() ) );

                        return std::get< 2 >( mapContextStack.back() );
                    }
                    case( ContainerType::Model ):
                    default:
                    {
                        const ValueType& currentValue = *( std::get< 0 >( currentContainer ) );

                        MembIt membIt = currentValue.FindMember( memberMeta.name );

                        if( membIt == currentValue.MemberEnd() )
                            return nullptr;
                        else
                            return &(membIt->value);
                    }
                }
            }

            void handleFailure( const std::string& failureReason, bool canContinue )
            {
                anyFailures = true;

                if( failureHandling == FailureHandling::Throw )
                {
                    valid = false;
                    throw DeseriailzationFailure( failureReason );
                }
                else
                {
                    if( trace )
                    {
                        if( !firstTrace )
                            *trace << "\n";

                        *trace << failureReason;
                        firstTrace = false;
                    }

                    if( !canContinue || ( failureHandling == FailureHandling::FailFast ) )
                    {
                        valid = false;
                    }
                }
            }

            const ValueType& v;
            std::deque< ContainerContext > containerStack;
            std::deque< MapContext > mapContextStack;
            std::deque< ArrayContext > arrayContextStack;
            FailureHandling failureHandling;
            std::ostream* trace;
            bool firstTrace;
            bool anyFailures;
            bool valid;
    };

    template< typename W, typename T >
    void SerializeModel( W& writer, const T& model )
    {
        LHModelNS::Inspector< JsonSerializerSAX< W > > saxSerializer( writer );

        writer.StartObject();
        model.template Accept< decltype( saxSerializer ) >( saxSerializer );
        writer.EndObject();
    }

    // add other overrides to customize behavior
    template< typename W >
    void defaultSerialize( W& w, bool inKey, const std::string& value )
    {
        if( inKey )
            w.Key( value.c_str(), value.size() );
        else
            w.String( value.c_str(), value.size() );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, bool value )
    {
        w.Bool( value );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, int value )
    {
        w.Int( value );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, unsigned value )
    {
        w.Uint( value );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, int64_t value )
    {
        w.Int64( value );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, uint64_t value )
    {
        w.Uint64( value );
    }

    template< typename W >
    void defaultSerialize( W& w, bool inKey, double value )
    {
        w.Double( value );
    }


    template< typename T, typename V >
    void DeserializeValue( T& model, const V& value )
    {
        LHModelNS::Populator< JsonDeserializerDOM< typename V::ValueType > > domDeserializer( value );
        model.template Accept< decltype( domDeserializer ) >( domDeserializer );
    }

    template< typename T, typename V >
    bool DeserializeValue( T& model, const V& value, std::ostream* trace, bool failFast )
    {
        LHModelNS::Populator< JsonDeserializerDOM< typename V::ValueType > > domDeserializer( value, trace, failFast );
        model.template Accept< decltype( domDeserializer ) >( domDeserializer );
        return !( domDeserializer.AnyFailures() );
    }

    template< typename T, typename V >
    bool defaultDeserialize( T& member, const V& value, std::string& failureReason )
    {
        if( !value.template Is<T>() )
        {
            std::ostringstream oss;

            oss << "member[" << member << "] encountered incorrect type " << value.GetType();

            failureReason = oss.str();

            return false;
        }

        member = value.template Get<T>();

        return true;
    }

    template< typename V >
    bool defaultDeserialize( std::string& member, const V& value, std::string& failureReason )
    {
        if( !value.IsString() )
        {
            std::ostringstream oss;

            oss << "member[" << member << "] expected string, encountered " << value.GetType();

            failureReason = oss.str();

            return false;
        }

        member = value.GetString();

        return true;
    }
}

#endif
