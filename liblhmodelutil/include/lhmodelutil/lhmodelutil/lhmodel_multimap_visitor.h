#ifndef __LHMODELUTIL_MULTIMAP_VISITOR_H__
#define __LHMODELUTIL_MULTIMAP_VISITOR_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <sstream>
#include <string>
#include <utility>

namespace LHModelUtilNS
{
    template< typename T >
    void defaultSerializeToStr( std::string& str, const T& value )
    {
        std::stringstream ss;
        ss << value;
        str = ss.str();
    }

    template< typename T, typename = void >
    class PrimitiveStrSerializer
    {
    public:
        static void serializeToStr( std::string& str, const T& value )
        {
            defaultSerializeToStr( str, value );
        }
    };

    // supports arrays
    template< typename MM >
    class MultiMapSerializer : public FlatInspector
    {
    public:
        MultiMapSerializer( MM& _mm )
            : mm( _mm )
            , inArray( false )
        {
        }

    protected:
        template< typename T >
        bool EnterArrayMember( const MemberMeta& memberMeta, const T& arrayMember )
        {
            // no nested arrays

            if ( inArray )
            {
                return false;
            }

            inArray = true;

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
            inArray = false;
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
        {
            std::string str;
            PrimitiveStrSerializer< T >::serializeToStr( str, primitiveMember );

            mm.emplace( memberMeta.name, std::move( str ) );
        }

    private:
        MM& mm;
        bool inArray;
    };

    template< typename T >
    void defaultDeserializeFromStr( T& value, const std::string& str )
    {
        std::stringstream ss( str );
        ss >> value;
    }

    template< typename T, typename = void >
    class PrimitiveStrDeserializer
    {
    public:
        static void deserializeFromStr( T& value, const std::string& str )
        {
            defaultDeserializeFromStr( value, str );
        }
    };

    template< typename MM >
    class MultiMapDeserializer : public FlatPopulator
    {
    public:
        MultiMapDeserializer( const MM& _mm )
            : mm( _mm )
            , itRange()
            , currIt()
        {
        }

    protected:
        template< typename T >
        bool EnterArrayMember( const MemberMeta& memberMeta, T& arrayMember )
        {
            // no nested arrays

            if ( inArray )
            {
                return false;
            }


            inArray = true;
            itRange = mm.equal_range( memberMeta.name );
            currIt = itRange.first;

            return true;
        }

        bool HasMoreArrayValues( const MemberMeta& memberMeta )
        {
            return currIt != itRange.second;
        }

        bool EnterArrayValue( const MemberMeta& memberMeta )
        {
            return true;
        }

        void LeaveArrayValue( const MemberMeta& memberMeta )
        {
            ++currIt;
            return;
        }

        template< typename T >
        void LeaveArrayMember( const MemberMeta& memberMeta, T& vectorMember )
        {
            inArray = false;
            return;
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, T& primitiveMember )
        {
            if ( inArray )
            {
                PrimitiveStrDeserializer< T >::deserializeFromStr( primitiveMember, currIt->second );
            }
            else
            {
                auto its = mm.equal_range( memberMeta.name );
                if ( its.first != its.second ) // skip if does not exist
                {
                    PrimitiveStrDeserializer< T >::deserializeFromStr( primitiveMember, its.first->second );
                }
            }
        }

    private:
        const MM& mm;
        bool inArray;
        std::pair< typename MM::const_iterator, typename MM::const_iterator > itRange;
        typename MM::const_iterator currIt;
    };

    template< typename T, typename MM >
    void DeserializeFromMultiMap( T& model, const MM& mm )
    {
        LHModelNS::Populator< MultiMapDeserializer< MM > > mmDeserializer( mm );
        model.template Accept< decltype( mmDeserializer ) >( mmDeserializer );
    }

    template< typename T, typename MM >
    void SerializeToMultiMap( MM& mm, const T& model )
    {
        LHModelNS::Inspector< MultiMapSerializer< MM > > mmSerializer( mm );
        model.template Accept< decltype( mmSerializer ) >( mmSerializer );
    }
}

#endif
