
#include <lhmodelutil_test/ExampleModelB.h>

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 2 > ModelMembersMeta< TestLHModelNS::ExampleModelB >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< TestLHModelNS::ExampleModelB >::memberIndex{ {
    { "member1", 0 },
    { "member2", 1 } } };

    constexpr ModelMeta ModelModelMeta< TestLHModelNS::ExampleModelB >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

