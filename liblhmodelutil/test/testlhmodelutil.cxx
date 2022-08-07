#include <rapidjson/document.h>

#include <gtest/gtest.h>

#include <lhmodelutil/lhmodel_rapidjson.h>
#include <lhmodelutil/lhmodel_multimap_visitor.h>
#include <lhmodelutil_test/ExampleModelD.h>

// For ExampleModelD TestNestedMaps
namespace LHModelNS
{
    template< typename K, typename T, typename C, typename A >
    class MemberType< std::map< K, T, C, A > >
    {
    public:
        typedef MapMemberType valueType;
    };
}

namespace TestLHModelUtilNS
{
    TEST( TestLHModelExamples, TestSerializeWithRapidJson )
    {
        TestLHModelNS::ExampleModelD exD;
        std::string jsonSerialization;
        std::string expectedJsonSerialization(
            "{"
            "\"member1\":123456789,"
            "\"member2\":"
            "["
            "["
            "{"
            "\"map1\":"
            "{"
            "\"a1\":{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]},"
            "\"a2\":{\"member1\":45.6,\"member2\":[]},"
            "\"a3\":{\"member1\":789.0,\"member2\":[\"a\",\"b\",\"c\"]}"
            "}"
            "},"
            "{"
            "},"
            "{"
            "\"map2\":"
            "{"
            "}"
            "}"
            "]"
            "],"
            "\"member3\":"
            "{"
            "\"emptyMap\":{}"
            "}"
            "}" );

        rapidjson::StringBuffer s;
        rapidjson::Writer< rapidjson::StringBuffer > writer( s );

        // member1
        exD.member1 = 123456789;

        // member2
        exD.member2.emplace_back(); // std::vector of maps
        exD.member2.back().emplace_back(); // std::map from string to maps
        TestLHModelNS::ExampleModelA exA1;
        exA1.member1 = 1.23;
        exA1.member2 = { "hello", "world" };
        exD.member2.back().back().emplace( "map1", std::map< std::string, TestLHModelNS::ExampleModelA >() );
        exD.member2.back().back()[ "map1" ].emplace( "a1", exA1 );
        TestLHModelNS::ExampleModelA exA2;
        exA2.member1 = 45.6;
        exD.member2.back().back()[ "map1" ].emplace( "a2", exA2 );
        TestLHModelNS::ExampleModelA exA3;
        exA3.member1 = 789;
        exA3.member2 = { "a", "b", "c" };
        exD.member2.back().back()[ "map1" ].emplace( "a3", exA3 );

        exD.member2.back().emplace_back(); // std::map

        exD.member2.back().emplace_back(); // std::map
        exD.member2.back().back().emplace( "map2", std::map< std::string, TestLHModelNS::ExampleModelA >() );

        // member3
        exD.member3.emplace( "emptyMap", std::map< std::string, std::string >() );

        LHModelUtilNS::SerializeModel( writer, exD );

        jsonSerialization = s.GetString();

        ASSERT_EQ( expectedJsonSerialization, jsonSerialization );
    }

    TEST( TestLHModelExamples, TestDeserializeWithRapidJson )
    {
        TestLHModelNS::ExampleModelD exD;
        std::string exDJson(
            "{"
            "\"member1\":123456789,"
            "\"member2\":"
            "["
            "["
            "{"
            "\"map1\":"
            "{"
            "\"a1\":{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]},"
            "\"a2\":{\"member1\":45.6,\"member2\":[]},"
            "\"a3\":{\"member1\":789.0,\"member2\":[\"a\",\"b\",\"c\"]}"
            "}"
            "},"
            "{"
            "},"
            "{"
            "\"map2\":"
            "{"
            "}"
            "}"
            "]"
            "],"
            "\"member3\":"
            "{"
            "\"emptyMap\":{}"
            "}"
            "}" );

        rapidjson::Document exDDoc;

        exDDoc.Parse( exDJson.c_str() );

        std::ostringstream oss;
        ASSERT_TRUE( LHModelUtilNS::DeserializeValue( exD, exDDoc, &oss, false ) )
            << "failed with[" << oss.str() << "]";

        // member1
        EXPECT_EQ( 123456789, exD.member1 );

        // member2
        ASSERT_EQ( 1, exD.member2.size() );

        const auto& member2Maps = exD.member2.back();
        ASSERT_EQ( 3, member2Maps.size() );

        const auto& firstMap = member2Maps[ 0 ];
        ASSERT_TRUE( firstMap.find( "map1" ) != firstMap.end() );

        const auto& map1 = firstMap.at( "map1" );
        ASSERT_TRUE( map1.find( "a1" ) != map1.end() );
        ASSERT_TRUE( map1.find( "a2" ) != map1.end() );
        ASSERT_TRUE( map1.find( "a3" ) != map1.end() );

        const auto& a1 = map1.at( "a1" );
        EXPECT_EQ( 1.23, a1.member1 );
        ASSERT_EQ( 2, a1.member2.size() );
        EXPECT_STREQ( "hello", a1.member2[ 0 ].c_str() );
        EXPECT_STREQ( "world", a1.member2[ 1 ].c_str() );

        const auto& a2 = map1.at( "a2" );
        EXPECT_EQ( 45.6, a2.member1 );
        EXPECT_EQ( 0, a2.member2.size() );

        const auto& a3 = map1.at( "a3" );
        EXPECT_EQ( 789.0, a3.member1 );
        ASSERT_EQ( 3, a3.member2.size() );
        EXPECT_STREQ( "a", a3.member2[ 0 ].c_str() );
        EXPECT_STREQ( "b", a3.member2[ 1 ].c_str() );
        EXPECT_STREQ( "c", a3.member2[ 2 ].c_str() );

        const auto& secondMap = member2Maps[ 1 ];
        EXPECT_EQ( 0, secondMap.size() );

        const auto& thirdMap = member2Maps[ 2 ];
        ASSERT_EQ( 1, thirdMap.size() );

        const auto& map2 = thirdMap.at( "map2" );
        EXPECT_EQ( 0, map2.size() );

        // member3
        ASSERT_EQ( 1, exD.member3.size() );
        ASSERT_TRUE( exD.member3.find( "emptyMap" ) != exD.member3.end() );
        EXPECT_EQ( 0, exD.member3.at( "emptyMap" ).size() );
    }

    TEST( TestLHModelExamples, TestMultiMapDeserialize )
    {
        std::multimap< std::string, std::string > mm;
        std::unordered_multimap< std::string, std::string > umm;
        TestLHModelNS::ExampleModelA aToDeserializeWith;
        TestLHModelNS::ExampleModelA aToSerializeWith;

        aToSerializeWith.member1 = 1.23;
        aToSerializeWith.member2 = { "hello", "world", "kasdha" };

        LHModelUtilNS::SerializeToMultiMap( mm, aToSerializeWith );
        ASSERT_EQ( 1, mm.count( "member1" ) );
        ASSERT_EQ( 3, mm.count( "member2" ) );

        LHModelUtilNS::DeserializeFromMultiMap( aToDeserializeWith, mm );
        ASSERT_DOUBLE_EQ( aToSerializeWith.member1, aToDeserializeWith.member1 );
        ASSERT_EQ( 3, aToDeserializeWith.member2.size() );
        ASSERT_STREQ( "hello", aToDeserializeWith.member2[ 0 ].c_str() );
        ASSERT_STREQ( "world", aToDeserializeWith.member2[ 1 ].c_str() );
        ASSERT_STREQ( "kasdha", aToDeserializeWith.member2[ 2 ].c_str() );

        LHModelUtilNS::SerializeToMultiMap( umm, aToDeserializeWith );
        ASSERT_EQ( 1, umm.count( "member1" ) );
        ASSERT_EQ( 3, umm.count( "member2" ) );
    }
}
