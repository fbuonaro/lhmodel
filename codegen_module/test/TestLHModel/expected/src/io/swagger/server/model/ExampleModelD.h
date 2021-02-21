#ifndef __TestLHModelNS_ExampleModelD_lhmodel_H__
#define __TestLHModelNS_ExampleModelD_lhmodel_H__

#include <cstdint>
#include <lhmodel_test/ExampleModelA.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace TestLHModelNS
{
    class ExampleModelD;
}

namespace LHModelNS
{
    template<>
    class MemberType< TestLHModelNS::ExampleModelD >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< TestLHModelNS::ExampleModelD >
    {
        public:
            static constexpr std::array< MemberMeta, 3 > membersMeta{ {
                MemberMeta( "member1", "std::int64_t", "int64" ),
                MemberMeta( "member2", "std::vector<std::vector<std::unordered_map<std::string, std::unordered_map<std::string, ExampleModelA>>>>", "" ),
                MemberMeta( "member3", "std::unordered_map<std::string, std::unordered_map<std::string, std::string>>", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< TestLHModelNS::ExampleModelD >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ExampleModelD" };
    };

}

namespace TestLHModelNS
{
    class ExampleModelD
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 1 >(), member2 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 2 >(), member3 );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 0 >(), member1 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 1 >(), member2 );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 2 >(), member3 );
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member ) const
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelD >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 1 >(), member2 );
                        break;
                    }
                    case( 2 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 2 >(), member3 );
                        break;
                    }
                }
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member )
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelD >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 0 >(), member1 );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 1 >(), member2 );
                        break;
                    }
                    case( 2 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelD, 2 >(), member3 );
                        break;
                    }
                }
            }

            std::int64_t member1;
            std::vector<std::vector<std::unordered_map<std::string, std::unordered_map<std::string, ExampleModelA>>>> member2;
            std::unordered_map<std::string, std::unordered_map<std::string, std::string>> member3;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
