#ifndef __TestLHModelNS_ExampleModelB_lhmodel_H__
#define __TestLHModelNS_ExampleModelB_lhmodel_H__

#include <lhmodelutil_test/ExampleModelA.h>
#include <vector>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace TestLHModelNS
{
    class ExampleModelB;
}

namespace LHModelNS
{
    template<>
    class MemberType< TestLHModelNS::ExampleModelB >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< TestLHModelNS::ExampleModelB >
    {
        public:
            static constexpr std::array< MemberMeta, 2 > membersMeta{ {
                MemberMeta( "member1", "ExampleModelA", "" ),
                MemberMeta( "member2", "std::vector<ExampleModelA>", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< TestLHModelNS::ExampleModelB >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ExampleModelB" };
    };

}

namespace TestLHModelNS
{
    class ExampleModelB
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 1 >(), member2 );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 1 >(), member2 );
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
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 1 >(), member2 );
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
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelB, 1 >(), member2 );
                        break;
                    }
                }
            }

            ExampleModelA member1;
            std::vector<ExampleModelA> member2;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
