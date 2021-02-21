
#include <lhmodelutil_test/ExampleModelD.h>

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 3 > ModelMembersMeta< TestLHModelNS::ExampleModelD >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< TestLHModelNS::ExampleModelD >::memberIndex{ {
    { "member1", 0 },
    { "member2", 1 },
    { "member3", 2 } } };

    constexpr ModelMeta ModelModelMeta< TestLHModelNS::ExampleModelD >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

