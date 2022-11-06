#ifndef __LHMODEL_MEMBERMETA_H__
#define __LHMODEL_MEMBERMETA_H__

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace LHModelNS
{
    // Member metadata
    class MemberMeta
    {
    public:
        constexpr MemberMeta( const MemberMeta& other )
            : name( other.name )
            , type( other.type )
            , format( other.format )
        {
        }

        constexpr MemberMeta( const char* _name, const char* _type, const char* _format )
            : name( _name )
            , type( _type )
            , format( _format )
        {
        }

        friend std::ostream& operator<<( std::ostream& os, const MemberMeta& memberMeta );

        const char* name;
        const char* type;
        const char* format;

    private:
        constexpr MemberMeta()
            : name( nullptr )
            , type( nullptr )
            , format( nullptr )
        {
        }
    };

    std::ostream& operator<<( std::ostream& os, const MemberMeta& memberMeta );

    // Mapping from typename to array of member metadata
    template< typename T >
    class ModelMembersMeta
    {
    public:
        // undefined, will fail to link
        static constexpr std::array< MemberMeta, 0 > membersMeta{ {} };
    };

    // Lookup member metadata for a type at the specified member index
    template< typename T, size_t i  >
    constexpr const MemberMeta& GetMemberMeta()
    {
        static_assert( i < ModelMembersMeta< T >::membersMeta.size(),
            "memberIndex exceeds number of members for type" );

        return ModelMembersMeta< T >::membersMeta[ i ];
    }

    template< typename T >
    size_t GetMemberIndex( const std::string& memberName )
    {
        return ModelMembersMeta< T >::memberIndex.at( memberName );
    }

    // Tags for three types of members
    typedef std::integral_constant< int, 1 > ModelMemberType;
    typedef std::integral_constant< int, 2 > PrimitiveMemberType;
    typedef std::integral_constant< int, 3 > ArrayMemberType;
    typedef std::integral_constant< int, 4 > MapMemberType;

    // Mapping from type to what type of MemberType it is
    template< typename T >
    class MemberType
    {
    public:
        typedef PrimitiveMemberType valueType;
    };

    // Mapping from any type of vector to ArrayMemberType
    template< typename T, typename A >
    class MemberType< std::vector< T, A > >
    {
    public:
        typedef ArrayMemberType valueType;
    };

    template< typename K, typename T, typename H, typename C, typename A >
    class MemberType< std::unordered_map< K, T, H, C, A > >
    {
    public:
        typedef MapMemberType valueType;
    };
}

#endif
