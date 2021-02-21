#ifndef __TestLHModelNS_ExampleModelA_lhmodel_H__
#define __TestLHModelNS_ExampleModelA_lhmodel_H__

#include <string>
#include <vector>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace TestLHModelNS
{
    class ExampleModelA;
}

namespace LHModelNS
{
    template<>
    class MemberType< TestLHModelNS::ExampleModelA >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< TestLHModelNS::ExampleModelA >
    {
        public:
            static constexpr std::array< MemberMeta, 2 > membersMeta{ {
                MemberMeta( "member1", "double", "double" ),
                MemberMeta( "member2", "std::vector<std::string>", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< TestLHModelNS::ExampleModelA >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ExampleModelA" };
    };

}

namespace TestLHModelNS
{
    class ExampleModelA
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), member2 );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), member2 );
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
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), member2 );
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
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), member2 );
                        break;
                    }
                }
            }

            double member1;
            std::vector<std::string> member2;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
