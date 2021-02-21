#ifndef __LHMODELUTIL_VISITOR_HELPER_H__
#define __LHMODELUTIL_VISITOR_HELPER_H__

#include <lhmodel/visitor.h>

namespace LHModelUtilNS
{
    using namespace LHModelNS;

    class FlatInspector
    {
        protected:
            template< typename T >
            bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& modelMember )
            {
                return false;
            }

            template< typename T >
            void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, const T& model )
            {
                return;
            }

            template< typename T >
            bool EnterArrayMember( const MemberMeta& memberMeta, const T& arrayMember )
            {
                return false;
            }

            bool EnterArrayValue( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveArrayValue( const MemberMeta& memberMeta )
            {
                return;
            }

            template< typename T >
            void LeaveArrayMember( const MemberMeta& memberMeta, const T& vectorMember )
            {
                return;
            }

            template< typename T >
            bool EnterMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                return false;
            }

            bool EnterMapEntryKey( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveMapEntryKey( const MemberMeta& memberMeta )
            {
                return;
            }

            bool EnterMapEntryValue( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveMapEntryValue( const MemberMeta& memberMeta )
            {
                return;
            }

            template< typename T >
            void LeaveMapMember( const MemberMeta& memberMeta, const T& mapMember )
            {
                return;
            }
    };

    class FlatPopulator
    {
        protected:
            template< typename T >
            bool EnterModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& modelMember )
            {
                return false;
            }

            template< typename T >
            void LeaveModelMember( const ModelMeta& modelMeta, const MemberMeta& memberMeta, T& model )
            {
                return;
            }

            template< typename T >
            bool EnterArrayMember( const MemberMeta& memberMeta, T& arrayMember )
            {
                return false;
            }

            bool HasMoreArrayValues( const MemberMeta& memberMeta )
            {
                return false;
            }

            bool EnterArrayValue( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveArrayValue( const MemberMeta& memberMeta )
            {
                return;
            }

            template< typename T >
            void LeaveArrayMember( const MemberMeta& memberMeta, T& vectorMember )
            {
                return;
            }

            template< typename T >
            bool EnterMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                return false;
            }

            bool HasMoreMapEntries( const MemberMeta& memberMeta )
            {
                return false;
            }

            bool EnterMapEntryKey( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveMapEntryKey( const MemberMeta& memberMeta )
            {
                return;
            }

            bool EnterMapEntryValue( const MemberMeta& memberMeta )
            {
                return false;
            }

            void LeaveMapEntryValue( const MemberMeta& memberMeta )
            {
                return;
            }

            template< typename T >
            void LeaveMapMember( const MemberMeta& memberMeta, T& mapMember )
            {
                return;
            }
    };
}

#endif
