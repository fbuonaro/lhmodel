#ifndef __LHMODELUTIL_MULTIMAP_VISITOR_H__
#define __LHMODELUTIL_MULTIMAP_VISITOR_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <sstream>
#include <string>

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

    template< template< typename, typename, typename... > class MM, typename... Rest >
    class MultiMapSerializer : public FlatInspector
    {
        public:
            MultiMapSerializer( MM< std::string, std::string, Rest...>& _mm )
            :   mm( _mm )
            {
            }

        protected:
            template< typename T >
            void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
            {
                std::string str;
                PrimitiveStrSerializer< T >::serializeToStr( str, primitiveMember );

                // skip empty members
                if( str.size() )
                    mm.emplace( memberMeta.name, std::move( str ) );
            }

        private:
            MM< std::string, std::string, Rest...>& mm;
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

    template< template< typename, typename, typename... > class MM, typename... Rest >
    class MultiMapDeserializer : public FlatPopulator
    {
        public:
            MultiMapDeserializer( const MM< std::string, std::string, Rest...>& _mm )
            :   mm( _mm )
            {
            }

        protected:
            template< typename T >
            void VisitPrimitiveMember( const MemberMeta& memberMeta, T& primitiveMember )
            {
                auto its = mm.equal_range( memberMeta.name ); 
                if( its.first != its.second && its.first->second.size() ) // skip if does not exist or empty
                {
                    PrimitiveStrDeserializer< T >::deserializeFromStr( primitiveMember, its.first->second );
                }
            }

        private:
            const MM< std::string, std::string, Rest...>& mm;
    };
}

#endif
