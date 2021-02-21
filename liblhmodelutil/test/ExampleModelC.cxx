
#include <lhmodelutil_test/ExampleModelC.h>

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 1 > ModelMembersMeta< TestLHModelNS::ExampleModelC >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< TestLHModelNS::ExampleModelC >::memberIndex{ {
    { "member1", 0 } } };

    constexpr ModelMeta ModelModelMeta< TestLHModelNS::ExampleModelC >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

