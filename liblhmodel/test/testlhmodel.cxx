#include <gtest/gtest.h>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <lhmodel_test/ExampleModelA.h>
#include <lhmodel_test/ExampleModelB.h>
#include <lhmodel_test/ExampleModelC.h>
#include <lhmodel_test/ExampleModelD.h>

#include <array>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <stdexcept>

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

namespace TestLHModelNS
{
    using namespace LHModelNS;

    class ExampleVisitorImpl
    {
    public:
        ExampleVisitorImpl( std::unique_ptr< std::ostringstream > _os )
            : os( std::move( _os ) )
        {
        }

        template< typename T >
        bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
        {
            (void)memberMeta;
            (void)model;
            *os << "EnterModel[" << modelMeta.name << "]\n";
            return true;
        }

        template< typename T >
        void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
        {
            (void)memberMeta;
            (void)model;
            *os << "LeaveModel[" << modelMeta.name << "]\n";
        }

        template< template< typename, typename > class V, typename T, typename A >
        bool EnterArrayMember( const MemberMeta& memberMeta, const V< T, A >& vectorMember )
        {
            (void)vectorMember;
            *os << "BeginArray[" << memberMeta.name << "]\n";
            return true;
        }

        bool EnterArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return;
        }

        template< template< typename, typename > class V, typename T, typename A >
        void LeaveArrayMember( const MemberMeta& memberMeta, const V< T, A >& vectorMember )
        {
            (void)vectorMember;
            *os << "EndArray[" << memberMeta.name << "]\n";
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
        {
            *os << "Primitive Member[" << memberMeta.name << "|" << primitiveMember << "]\n";
        }

        std::string Serialization()
        {
            return os->str();
        }

    private:
        std::unique_ptr< std::ostringstream > os;
    };

    enum class JsonState
    {
        None,
        InArray,
        InModel,
        InMap
    };

    template< typename T >
    void serializePrimitive( std::ostringstream& oss, const T& value )
    {
        oss << value;
    }

    template<>
    void serializePrimitive< std::string >(
        std::ostringstream& oss,
        const std::string& value )
    {
        oss << "\"" << value << "\"";
    }

    class DumbJsonVisitor
    {
    public:
        DumbJsonVisitor()
            : oss()
            , jsonState()
            , prevMember( false )
        {
            oss << "{";
            jsonState.push_back( JsonState::InModel );
        }

        template< typename T >
        bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& modelMember )
        {
            (void)modelMeta;
            (void)modelMember;
            if ( prevMember )
                oss << ",";

            if ( inModel() )
                oss << "\"" << memberMeta.name << "\"" << ":";

            oss << "{";

            jsonState.push_back( JsonState::InModel );

            prevMember = false;

            return true;
        }

        template< typename T >
        void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
        {
            (void)modelMeta;
            (void)memberMeta;
            (void)model;
            oss << "}";
            jsonState.pop_back();
            prevMember = true;
        }

        template< typename T, typename A >
        bool EnterArrayMember( const MemberMeta& memberMeta, const std::vector< T, A >& vectorMember )
        {
            (void)vectorMember;
            if ( prevMember )
                oss << ",";

            if ( inModel() )
                oss << "\"" << memberMeta.name << "\"" << ":";

            oss << "[";

            jsonState.push_back( JsonState::InArray );
            prevMember = false;

            return true;
        }

        template< typename T, typename A >
        void LeaveArrayMember( const MemberMeta& memberMeta, const std::vector< T, A >& vectorMember )
        {
            (void)memberMeta;
            (void)vectorMember;
            oss << "]";
            jsonState.pop_back();
            prevMember = true;
        }

        bool EnterArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return;
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, const T& primitiveMember )
        {
            if ( prevMember )
                oss << ",";

            if ( inModel() )
                oss << "\"" << memberMeta.name << "\"" << ":";

            serializePrimitive( oss, primitiveMember );

            prevMember = true;
        }

        std::string Serialization()
        {
            std::string serialization;

            oss << "}";

            serialization = oss.str();
            oss.str( "" );

            return serialization;
        }

        template< typename K, typename T >
        bool EnterMapMember( const MemberMeta& memberMeta, const std::map< K, T >& mapMember )
        {
            (void)mapMember;
            if ( prevMember )
                oss << ",";

            if ( inModel() )
                oss << "\"" << memberMeta.name << "\"" << ":";

            oss << "{";

            jsonState.push_back( JsonState::InMap );
            prevMember = false;

            return true;
        }

        template< typename K, typename T >
        void LeaveMapMember( const MemberMeta& memberMeta, const std::map< K, T >& mapMember )
        {
            (void)memberMeta;
            (void)mapMember;
            oss << "}";
            jsonState.pop_back();
            prevMember = true;
        }

        bool EnterMapEntryKey( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveMapEntryKey( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            oss << ":";
            prevMember = false;
        }

        bool EnterMapEntryValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveMapEntryValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return;
        }

    private:

        bool inArray() const
        {
            return ( !jsonState.empty() ) && ( jsonState.back() == JsonState::InArray );
        }

        bool inModel() const
        {
            return ( !jsonState.empty() ) && ( jsonState.back() == JsonState::InModel );
        }

        bool inMap() const
        {
            return ( !jsonState.empty() ) && ( jsonState.back() == JsonState::InMap );
        }

        std::ostringstream oss;
        std::deque< JsonState > jsonState;
        bool prevMember;
    };

    class ResetToDefaultVisitor
    {
    public:
        ResetToDefaultVisitor()
        {
        }

        template< typename T >
        bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
        {
            return true;
        }

        template< typename T >
        void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
        {
            return;
        }

        template< typename T, typename A >
        bool EnterArrayMember( const MemberMeta& memberMeta, std::vector< T, A >& vectorMember )
        {
            (void)memberMeta;
            vectorMember.resize( 0 );
            return true;
        }

        template< typename T, typename A >
        void LeaveArrayMember( const MemberMeta& memberMeta, const std::vector< T, A >& vectorMember )
        {
            (void)memberMeta;
            (void)vectorMember;
            return;
        }

        bool EnterArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return;
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, T& primitiveMember )
        {
            (void)memberMeta;
            primitiveMember = T();
        }
    };


    TEST( TestLHModel, TestTypes )
    {
        // #myfavoritesyntax - parantheses around the GetMemberMeta call because otherwise the macro sees
        //                     3 parameters
        ASSERT_STREQ( "member1", ( GetMemberMeta< ExampleModelA, 0 >().name ) );
        ASSERT_STREQ( "double", ( GetMemberMeta< ExampleModelA, 0 >().type ) );
        ASSERT_STREQ( "double", ( GetMemberMeta< ExampleModelA, 0 >().format ) );
        ASSERT_STREQ( "member2", ( GetMemberMeta< ExampleModelA, 1 >().name ) );
        ASSERT_STREQ( "std::vector<std::string>", ( GetMemberMeta< ExampleModelA, 1 >().type ) );
        ASSERT_STREQ( "ExampleModelA", ( GetModelMeta< ExampleModelA >().name ) );

        ASSERT_EQ( ModelMemberType::value, MemberType< ExampleModelA >::valueType::value );
        ASSERT_EQ( PrimitiveMemberType::value, MemberType< int >::valueType::value );
        ASSERT_EQ( PrimitiveMemberType::value, MemberType< std::string >::valueType::value );
        ASSERT_EQ( ArrayMemberType::value, MemberType< std::vector< std::vector< int > > >::valueType::value );
        ASSERT_EQ( ArrayMemberType::value, MemberType< std::vector< std::string > >::valueType::value );
    }

    TEST( TestLHModel, TestDoesAnythingWork )
    {
        ExampleModelA exA;
        std::unique_ptr< std::ostringstream > os( new std::ostringstream() );
        Inspector< ExampleVisitorImpl > exVisitor( std::move( os ) );
        std::string serialization;
        std::string expectedSerialization( // "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1.23]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|hello]\n"
            "Primitive Member[member2|world]\n"
            "EndArray[member2]\n" );
        // "LeaveModel[ExampleModelA]\n" );

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };

        exA.Accept< decltype( exVisitor ) >( exVisitor );

        serialization = exVisitor.Serialization();

        ASSERT_EQ( expectedSerialization, serialization ) << "Q: Does anything work A: No";

    }

    TEST( TestLHModel, TestJsonLmao )
    {
        ExampleModelA exA;
        Inspector< DumbJsonVisitor > jsonVisitor;
        std::string serialization;
        std::string expectedSerialization( "{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]}" );

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };

        exA.Accept< decltype( jsonVisitor ) >( jsonVisitor );

        serialization = jsonVisitor.Serialization();

        ASSERT_EQ( expectedSerialization, serialization );
    }

    TEST( TestLHModel, TestNestedModel )
    {
        ExampleModelB exB;
        ExampleModelA& exA( exB.member1 );
        std::unique_ptr< std::ostringstream > os( new std::ostringstream() );
        Inspector< ExampleVisitorImpl > exVisitor( std::move( os ) );
        std::string serialization;
        std::string expectedSerialization(
            // "EnterModel[ExampleModelB]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1.23]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|hello]\n"
            "Primitive Member[member2|world]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "BeginArray[member2]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1230.1]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|bonjour]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|0.123]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|comment]\n"
            "Primitive Member[member2|vousappellez]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EndArray[member2]\n"
            // "LeaveModel[ExampleModelB]\n"
        );

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };

        exB.member2.emplace_back();
        exB.member2.back().member1 = 1230.1;
        exB.member2.back().member2.emplace_back( "bonjour" );

        exB.member2.emplace_back();
        exB.member2.back().member1 = .123;
        exB.member2.back().member2.emplace_back( "comment" );
        exB.member2.back().member2.emplace_back( "vousappellez" );

        exB.Accept< decltype( exVisitor ) >( exVisitor );

        serialization = exVisitor.Serialization();

        ASSERT_EQ( expectedSerialization, serialization ) << "Q: Does anything work A: No";

    }

    TEST( TestLHModel, TestMultiVecNestedModel )
    {
        ExampleModelC exC;
        ExampleModelC exCi;
        ExampleModelB exB;
        ExampleModelA& exA( exB.member1 );
        std::unique_ptr< std::ostringstream > os( new std::ostringstream() );
        Inspector< ExampleVisitorImpl > exVisitor( std::move( os ) );
        Inspector< DumbJsonVisitor > jsonVisitor;
        std::string serialization;
        std::string jsonSerialization;
        std::string expectedSerialization(
            // "EnterModel[ExampleModelC]\n"
            "BeginArray[member1]\n"

            "BeginArray[member1]\n"
            "EndArray[member1]\n"

            "BeginArray[member1]\n"

            "EnterModel[ExampleModelB]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1.23]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|hello]\n"
            "Primitive Member[member2|world]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "BeginArray[member2]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1230.1]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|bonjour]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|0.123]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|comment]\n"
            "Primitive Member[member2|vousappellez]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelB]\n"

            "EnterModel[ExampleModelB]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1.23]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|hello]\n"
            "Primitive Member[member2|world]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "BeginArray[member2]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1230.1]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|bonjour]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|0.123]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|comment]\n"
            "Primitive Member[member2|vousappellez]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelB]\n"

            "EndArray[member1]\n"

            "BeginArray[member1]\n"

            "EnterModel[ExampleModelB]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1.23]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|hello]\n"
            "Primitive Member[member2|world]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "BeginArray[member2]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|1230.1]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|bonjour]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EnterModel[ExampleModelA]\n"
            "Primitive Member[member1|0.123]\n"
            "BeginArray[member2]\n"
            "Primitive Member[member2|comment]\n"
            "Primitive Member[member2|vousappellez]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelA]\n"
            "EndArray[member2]\n"
            "LeaveModel[ExampleModelB]\n"

            "EndArray[member1]\n"

            "EndArray[member1]\n"
            // "LeaveModel[ExampleModelC]\n"
        );

        std::string expectedJsonSerialization(
            "{\"member1\":[[],[{\"member1\":{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]},\"member2\":[{\"member1\":1230.1,\"member2\":[\"bonjour\"]},{\"member1\":0.123,\"member2\":[\"comment\",\"vousappellez\"]}]},{\"member1\":{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]},\"member2\":[{\"member1\":1230.1,\"member2\":[\"bonjour\"]},{\"member1\":0.123,\"member2\":[\"comment\",\"vousappellez\"]}]}],[{\"member1\":{\"member1\":1.23,\"member2\":[\"hello\",\"world\"]},\"member2\":[{\"member1\":1230.1,\"member2\":[\"bonjour\"]},{\"member1\":0.123,\"member2\":[\"comment\",\"vousappellez\"]}]}]]}" );

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };

        exB.member2.emplace_back();
        exB.member2.back().member1 = 1230.1;
        exB.member2.back().member2.emplace_back( "bonjour" );

        exB.member2.emplace_back();
        exB.member2.back().member1 = .123;
        exB.member2.back().member2.emplace_back( "comment" );
        exB.member2.back().member2.emplace_back( "vousappellez" );

        exC.member1.emplace_back();
        exC.member1.emplace_back( 2, exB );
        exC.member1.emplace_back( 1, exB );

        exC.Accept< decltype( exVisitor ) >( exVisitor );

        const_cast<const ExampleModelC&>( exC ).Accept< decltype( jsonVisitor ) >( jsonVisitor );

        serialization = exVisitor.Serialization();
        jsonSerialization = jsonVisitor.Serialization();

        ASSERT_EQ( expectedSerialization, serialization ) << "Q: Does anything work A: No";
        ASSERT_EQ( expectedJsonSerialization, jsonSerialization ) << "Q: Does anything work A: No";
    }

    TEST( TestLHModel, TestNonConstStuff )
    {
        ExampleModelA exA;
        Inspector< ResetToDefaultVisitor > resetVisitor;

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };

        exA.Accept< decltype( resetVisitor ) >( resetVisitor );

        EXPECT_EQ( 0.0, exA.member1 );
        EXPECT_EQ( 0, exA.member2.size() );
    }

    TEST( TestLHModel, TestMemberAccept )
    {
        ExampleModelA exA;
        Inspector< ResetToDefaultVisitor > resetVisitor;

        exA.member1 = 1.23;
        exA.member2 = { "hello", "world" };
        exA.Accept< decltype( resetVisitor ) >( resetVisitor, "member1" );

        EXPECT_EQ( 0.0, exA.member1 );
        EXPECT_EQ( 2, exA.member2.size() );

        exA.member1 = 1.23;
        exA.Accept< decltype( resetVisitor ) >( resetVisitor, "member2" );

        EXPECT_EQ( 1.23, exA.member1 );
        EXPECT_EQ( 0, exA.member2.size() );

        EXPECT_THROW( exA.Accept< decltype( resetVisitor ) >( resetVisitor, "member3" ), std::out_of_range );
        EXPECT_THROW( exA.Accept< decltype( resetVisitor ) >( resetVisitor, "a" ), std::out_of_range );
        EXPECT_THROW( exA.Accept< decltype( resetVisitor ) >( resetVisitor, "" ), std::out_of_range );
    }

    TEST( TestLHModel, TestNestedMaps )
    {
        ExampleModelD exD;
        Inspector< DumbJsonVisitor > jsonVisitor;
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
            "\"a3\":{\"member1\":789,\"member2\":[\"a\",\"b\",\"c\"]}"
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

        // member1
        exD.member1 = 123456789;

        // member2
        exD.member2.emplace_back(); // std::vector of maps
        exD.member2.back().emplace_back(); // std::map from string to maps
        ExampleModelA exA1;
        exA1.member1 = 1.23;
        exA1.member2 = { "hello", "world" };
        exD.member2.back().back().emplace( "map1", std::map< std::string, ExampleModelA >() );
        exD.member2.back().back()[ "map1" ].emplace( "a1", exA1 );
        ExampleModelA exA2;
        exA2.member1 = 45.6;
        exD.member2.back().back()[ "map1" ].emplace( "a2", exA2 );
        ExampleModelA exA3;
        exA3.member1 = 789;
        exA3.member2 = { "a", "b", "c" };
        exD.member2.back().back()[ "map1" ].emplace( "a3", exA3 );

        exD.member2.back().emplace_back(); // std::map

        exD.member2.back().emplace_back(); // std::map
        exD.member2.back().back().emplace( "map2", std::map< std::string, ExampleModelA >() );

        // member3
        exD.member3.emplace( "emptyMap", std::map< std::string, std::string >() );

        exD.Accept< decltype( jsonVisitor ) >( jsonVisitor );
        jsonSerialization = jsonVisitor.Serialization();

        ASSERT_EQ( expectedJsonSerialization, jsonSerialization );
    }

    class RandomPopulator
    {
    public:
        RandomPopulator()
        {
        }

        template< typename T >
        bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& modelMember )
        {
            (void)modelMeta;
            (void)memberMeta;
            (void)modelMember;
            return true;
        }

        template< typename T >
        void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& model )
        {
            (void)modelMeta;
            (void)memberMeta;
            (void)model;
            return;
        }

        template< template< typename, typename... > class A, typename T, typename... Rest >
        bool EnterArrayMember( const MemberMeta& memberMeta, A< T, Rest... >& arrayMember )
        {
            (void)memberMeta;
            (void)arrayMember;
            int numArrayValues = ( rand() % 3 ) + 1;
            containerCount.push_back( numArrayValues );
            return true;
        }

        bool HasMoreArrayValues( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return containerCount.back() > 0;
        }

        template< template< typename, typename... > class A, typename T, typename... Rest >
        void LeaveArrayMember( const MemberMeta& memberMeta, A< T >& arrayMember )
        {
            (void)memberMeta;
            (void)arrayMember;
            containerCount.pop_back();
        }

        bool EnterArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveArrayValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            containerCount.back() = containerCount.back() - 1;
        }

        template< typename T >
        void VisitPrimitiveMember( const MemberMeta& memberMeta, T& primitiveMember )
        {
            (void)memberMeta;

            if ( containerCount.empty() )
            {
                primitiveMember = T();
            }
            else
            {
                std::stringstream ss;
                ss << containerCount.back();
                ss >> primitiveMember;
            }
        }

        template< template< typename, typename, typename... > class M,
            typename K, typename T, typename... Rest >
        bool EnterMapMember( const MemberMeta& memberMeta, M< K, T, Rest... >& mapMember )
        {
            (void)memberMeta;
            (void)mapMember;
            int numMapEntries = ( rand() % 3 ) + 1;
            containerCount.push_back( numMapEntries );
            return true;
        }

        template< template< typename, typename, typename... > class M,
            typename K, typename T, typename... Rest >
        void LeaveMapMember( const MemberMeta& memberMeta, M< K, T, Rest... >& mapMember )
        {
            (void)memberMeta;
            (void)mapMember;
            containerCount.pop_back();
        }

        bool HasMoreMapEntries( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return containerCount.back() > 0;
        }

        bool EnterMapEntryKey( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveMapEntryKey( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return;
        }

        bool EnterMapEntryValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            return true;
        }

        void LeaveMapEntryValue( const MemberMeta& memberMeta )
        {
            (void)memberMeta;
            containerCount.back() = containerCount.back() - 1;
        }

    private:
        std::deque< int > containerCount;
    };

    TEST( TestLHModel, TestFillingRandomThings )
    {
        ExampleModelD exD;
        Populator< RandomPopulator > randomPopulator;
        Inspector< DumbJsonVisitor > jsonVisitor;
        std::string jsonSerialization;

        exD.Accept< decltype( randomPopulator ) >( randomPopulator );
        exD.Accept< decltype( jsonVisitor ) >( jsonVisitor );

        jsonSerialization = jsonVisitor.Serialization();

        ASSERT_TRUE( jsonSerialization.size() );
    }
}
