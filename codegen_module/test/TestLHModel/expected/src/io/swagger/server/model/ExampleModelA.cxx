
#include <lhmodel_test/ExampleModelA.h>

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 3 > ModelMembersMeta< TestLHModelNS::ExampleModelA >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< TestLHModelNS::ExampleModelA >::memberIndex{ {
    { "member1", 0 },
    { "member2", 1 },
    { "member3", 2 } } };

    constexpr ModelMeta ModelModelMeta< TestLHModelNS::ExampleModelA >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

