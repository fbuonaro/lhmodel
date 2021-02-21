#ifndef __TestLHModelNS_ExampleModelC_lhmodel_H__
#define __TestLHModelNS_ExampleModelC_lhmodel_H__

#include <lhmodelutil_test/ExampleModelB.h>
#include <vector>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace TestLHModelNS
{
    class ExampleModelC;
}

namespace LHModelNS
{
    template<>
    class MemberType< TestLHModelNS::ExampleModelC >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< TestLHModelNS::ExampleModelC >
    {
        public:
            static constexpr std::array< MemberMeta, 1 > membersMeta{ {
                MemberMeta( "member1", "std::vector<std::vector<ExampleModelB>>", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< TestLHModelNS::ExampleModelC >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ExampleModelC" };
    };

}

namespace TestLHModelNS
{
    class ExampleModelC
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelC, 0 >(), member1 );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelC, 0 >(), member1 );
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member ) const
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelA >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelC, 0 >(), member1 );
                        break;
                    }
                }
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member )
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelA >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelC, 0 >(), member1 );
                        break;
                    }
                }
            }

            std::vector<std::vector<ExampleModelB>> member1;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
