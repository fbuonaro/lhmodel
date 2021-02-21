#ifndef __LHMODELUTIL_URIQUERYPARAM_VISITOR_H__
#define __LHMODELUTIL_URIQUERYPARAM_VISITOR_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <sstream>
#include <string>

namespace LHModelUtilNS
{
    template< typename T >
    void defaultSerializeToUriQueryParamStr( std::string& str, const T& value )
    {
        std::stringstream ss;
        ss << value;
        str = ss.str();
    }

    template< typename T, typename = void >
    class PrimitiveUriQueryParamSerializer
    {
        public:
            static void serializeToUriQueryParamStr( std::string& str, const T& value )
            {
                defaultSerializeToUriQueryParamStr( str, value );
            }
    };

    template< template< typename, typename, typename... > class MM, typename... Rest >
    class UriQueryParamSeriailzer : public FlatInspector
    {
        public:
            UriQueryParamSeriailzer( std::ostringstream& _oss )
            :   oss( _oss )
            ,   first( true )
            {
            }

        protected:
            template< typename T >
            void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
            {
                std::string str;

                PrimitiveUriQueryParamSerializer< T >::serializeToUriQueryParamStr( str, primitiveMember );

                if( str.size() )
                {
                    if( !first )
                    {
                        oss << "&";
                        first = false;
                    }

                    // TODO - url encode str

                    oss << memberMeta.name << "=" << str;
                }
            }

        private:
            std::ostringstream& oss;
            bool first;
    };
}

#endif
