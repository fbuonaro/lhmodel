#ifndef __LHMODEL_VISITOR_H__
#define __LHMODEL_VISITOR_H__

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>

namespace LHModelNS
{
    template< typename M >
    class Visitor : public M
    {
        public:
            template< typename ... Args >
            Visitor( Args&&... args )
            :   M( std::forward< Args >( args )... )
            {
            }

            // CONST VERSIONS

            template< typename T >
            bool _EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
            {
                return M::template EnterModelMember( modelMeta, memberMeta, model );
            }

            template< typename T >
            void _LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
            {
                M::template LeaveModelMember( modelMeta, memberMeta, model );
            }

            template< typename T >
            bool _EnterArrayMember( const MemberMeta& memberMeta, const T& arrayMember )
            {
                return M::template EnterArrayMember( memberMeta, arrayMember );
            }

            bool _EnterArrayValue( const MemberMeta& memberMeta )
            {
                return M::EnterArrayValue( memberMeta );
            }

            void _LeaveArrayValue( const MemberMeta& memberMeta )
            {
                M::LeaveArrayValue( memberMeta );
            }

            template< typename T >
            void _LeaveArrayMember( const MemberMeta& memberMeta, const T& arrayMember )
            {
                M::template LeaveArrayMember( memberMeta, arrayMember );
            }

            template< typename T >
            bool _EnterMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                return M::template EnterMapMember( memberMeta, mapMember );
            }

            bool _EnterMapEntryKey( const MemberMeta& memberMeta )
            {
                return M::EnterMapEntryKey( memberMeta );
            }

            void _LeaveMapEntryKey( const MemberMeta& memberMeta )
            {
                M::LeaveMapEntryKey( memberMeta );
            }

            bool _EnterMapEntryValue( const MemberMeta& memberMeta )
            {
                return M::EnterMapEntryValue( memberMeta );
            }

            void _LeaveMapEntryValue( const MemberMeta& memberMeta )
            {
                M::LeaveMapEntryValue( memberMeta );
            }

            template< typename T >
            void _LeaveMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                M::template LeaveMapMember( memberMeta, mapMember );
            }

            // NON-CONST VERSIONS
            
            template< typename T >
            bool _EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& model )
            {
                return M::template EnterModelMember( modelMeta, memberMeta, model );
            }

            template< typename T >
            void _LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& model )
            {
                M::template LeaveModelMember( modelMeta, memberMeta, model );
            }

            template< typename T >
            bool _EnterArrayMember( const MemberMeta& memberMeta, T& arrayMember )
            {
                return M::template EnterArrayMember( memberMeta, arrayMember );
            }

            template< typename T >
            void _LeaveArrayMember( const MemberMeta& memberMeta, T& arrayMember )
            {
                M::template LeaveArrayMember( memberMeta, arrayMember );
            }

            template< typename T >
            bool _EnterMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                return M::template EnterMapMember( memberMeta, mapMember );
            }

            template< typename T >
            void _LeaveMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                M::template LeaveMapMember( memberMeta, mapMember );
            }
    };

    template< typename M >
    class Inspector : public Visitor< M >
    {
        public:
            template< typename ... Args >
            Inspector( Args&&... args )
            :   Visitor< M >( std::forward< Args >( args )... )
            {
            }

            // CONST VERSIONS

            template< typename T >
            void _VisitMember( const MemberMeta& memberMeta, const T& member )
            {
                typename MemberType< T >::valueType valueTag;
                _VisitMemberType( memberMeta, member, valueTag );
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, const T& modelMember, ModelMemberType )
            {
                if( Visitor< M >::template _EnterModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember ) )
                {
                    modelMember.template Accept< decltype( *this ) >( *this );

                    Visitor< M >::template _LeaveModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, const T& arrayMember, ArrayMemberType )
            {
                if( Visitor<M>::template _EnterArrayMember( memberMeta, arrayMember ) )
                {
                    for( auto it = arrayMember.cbegin(); it != arrayMember.cend(); ++it )
                    {
                        if( Visitor<M>::_EnterArrayValue( memberMeta ) )
                        {
                            typename MemberType< typename T::value_type >::valueType valueTag;
                            _VisitMemberType( memberMeta, *it, valueTag );
                            Visitor<M>::_LeaveArrayValue( memberMeta );
                        }
                    }

                    Visitor<M>::template _LeaveArrayMember( memberMeta, arrayMember );
                }
            }

            template< typename T >
            void _VisitMemberType(
                const MemberMeta& memberMeta,
                const T& mapMember,
                MapMemberType )
            {
                if( Visitor<M>::template _EnterMapMember( memberMeta, mapMember ) )
                {
                    for( auto it = mapMember.cbegin(); it != mapMember.cend(); ++it )
                    {
                        if( Visitor<M>::_EnterMapEntryKey( memberMeta ) )
                        {
                            typename MemberType< typename T::key_type >::valueType keyTag;
                            _VisitMemberType( memberMeta, it->first, keyTag );
                            Visitor<M>::_LeaveMapEntryKey( memberMeta );

                            if( Visitor<M>::_EnterMapEntryValue( memberMeta ) )
                            {
                                typename MemberType< typename T::mapped_type >::valueType valueTag;
                                _VisitMemberType( memberMeta, it->second, valueTag );
                                Visitor<M>::_LeaveMapEntryValue( memberMeta );
                            }
                        }
                    }

                    Visitor<M>::template _LeaveMapMember( memberMeta, mapMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, const T& primitiveMember, PrimitiveMemberType )
            {
                M::template VisitPrimitiveMember( memberMeta, primitiveMember );
            }

            // NON-CONST VERSIONS

            template< typename T >
            void _VisitMember( const MemberMeta& memberMeta, T& member )
            {
                typename MemberType< T >::valueType valueTag;
                _VisitMemberType( memberMeta, member, valueTag );
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& modelMember, ModelMemberType )
            {
                if( Visitor< M >::template _EnterModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember ) )
                {
                    modelMember.template Accept< decltype( *this ) >( *this );

                    Visitor< M >::template _LeaveModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& arrayMember, ArrayMemberType )
            {
                if( Visitor<M>::template _EnterArrayMember( memberMeta, arrayMember ) )
                {
                    for( auto it = arrayMember.begin(); it != arrayMember.end(); ++it )
                    {
                        if( Visitor<M>::_EnterArrayValue( memberMeta ) )
                        {
                            typename MemberType< typename T::value_type >::valueType valueTag;
                            _VisitMemberType( memberMeta, *it, valueTag );
                            Visitor<M>::_LeaveArrayValue( memberMeta );
                        }
                    }

                    Visitor<M>::template _LeaveArrayMember( memberMeta, arrayMember );
                }
            }

            template< typename T >
            void _VisitMemberType(
                const MemberMeta& memberMeta,
                T& mapMember,
                MapMemberType )
            {
                if( Visitor<M>::template _EnterMapMember( memberMeta, mapMember ) )
                {
                    for( auto it = mapMember.begin(); it != mapMember.end(); ++it )
                    {
                        if( Visitor<M>::_EnterMapEntryKey( memberMeta ) )
                        {
                            typename MemberType< typename T::key_type >::valueType keyTag;
                            _VisitMemberType( memberMeta, it->first, keyTag );
                            Visitor<M>::_LeaveMapEntryKey( memberMeta );

                            if( Visitor<M>::_EnterMapEntryValue( memberMeta ) )
                            {
                                typename MemberType< typename T::mapped_type >::valueType valueTag;
                                _VisitMemberType( memberMeta, it->second, valueTag );
                                Visitor<M>::_LeaveMapEntryValue( memberMeta );
                            }
                        }
                    }

                    Visitor<M>::template _LeaveMapMember( memberMeta, mapMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& primitiveMember, PrimitiveMemberType )
            {
                M::template VisitPrimitiveMember( memberMeta, primitiveMember );
            }
    };

    template< typename M >
    class Populator : public Visitor< M >
    {
        public:
            template< typename ... Args >
            Populator( Args&&... args )
            :   Visitor< M >( std::forward< Args >( args )... )
            {
            }

            template< typename T >
            void _VisitMember( const MemberMeta& memberMeta, T& member )
            {
                typename MemberType< T >::valueType valueTag;
                _VisitMemberType( memberMeta, member, valueTag );
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& modelMember, ModelMemberType )
            {
                if( Visitor< M >::template _EnterModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember ) )
                {
                    modelMember.template Accept< decltype( *this ) >( *this );

                    Visitor< M >::template _LeaveModelMember(
                        LHModelNS::GetModelMeta< T >(),
                        memberMeta,
                        modelMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& arrayMember, ArrayMemberType )
            {
                if( Visitor<M>::template _EnterArrayMember( memberMeta, arrayMember ) )
                {
                    while( M::HasMoreArrayValues( memberMeta ) ) 
                    {
                        if( Visitor<M>::_EnterArrayValue( memberMeta ) )
                        {
                            typename MemberType< typename T::value_type >::valueType valueTag;
                            typename T::value_type newArrayValue;
                            _VisitMemberType( memberMeta, newArrayValue, valueTag );
                            Visitor<M>::_LeaveArrayValue( memberMeta );

                            arrayMember.emplace_back( std::move( newArrayValue ) );
                        }
                    }

                    Visitor<M>::template _LeaveArrayMember( memberMeta, arrayMember );
                }
            }

            template< typename T >
            void _VisitMemberType(
                const MemberMeta& memberMeta,
                T& mapMember,
                MapMemberType )
            {
                if( Visitor<M>::template _EnterMapMember( memberMeta, mapMember ) )
                {
                    while( M::HasMoreMapEntries( memberMeta ) ) 
                    {
                        if( Visitor<M>::_EnterMapEntryKey( memberMeta ) )
                        {
                            typename MemberType< typename T::key_type >::valueType keyTag;
                            typename T::key_type newEntryKey;
                            _VisitMemberType( memberMeta, newEntryKey, keyTag );
                            Visitor<M>::_LeaveMapEntryKey( memberMeta );

                            if( Visitor<M>::_EnterMapEntryValue( memberMeta ) )
                            {
                                typename MemberType< typename T::mapped_type >::valueType valueTag;
                                typename T::mapped_type newEntryValue;
                                _VisitMemberType( memberMeta, newEntryValue, valueTag );
                                Visitor<M>::_LeaveMapEntryValue( memberMeta );

                                mapMember.emplace( std::move( newEntryKey ), std::move( newEntryValue ) );
                            }
                        }
                    }

                    Visitor<M>::template _LeaveMapMember( memberMeta, mapMember );
                }
            }

            template< typename T >
            void _VisitMemberType( const MemberMeta& memberMeta, T& primitiveMember, PrimitiveMemberType )
            {
                M::template VisitPrimitiveMember( memberMeta, primitiveMember );
            }
    };
}

#endif
