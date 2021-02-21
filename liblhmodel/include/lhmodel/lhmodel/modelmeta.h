#ifndef __LHMODEL_MODELMETA_H__
#define __LHMODEL_MODELMETA_H__

#include <iostream>
#include <string>

namespace LHModelNS
{
    // Model metadata
    class ModelMeta
    {
        public:
            constexpr ModelMeta( const char* _name )
            :   name( _name )
            {
            }

            const char* name;

        private:
            constexpr ModelMeta()
            :   name( nullptr )
            {
            }
    };

    std::ostream& operator<<( std::ostream& os, const ModelMeta& modelMeta );

    // Mapping from for a type to its model metadata
    // By default, a type has no model metadata (nullptr)
    template< typename T >
    class ModelModelMeta
    {
        public:
            // undefined, will fail to link
            static constexpr ModelMeta modelMeta{ nullptr };
    };

    // Get the model metadata for a type
    template< typename T >
    constexpr const ModelMeta& GetModelMeta()
    {
        static_assert( ModelModelMeta< T >::modelMeta.name != nullptr, "modelMeta is null for type" );

        return ModelModelMeta< T >::modelMeta;
    }
}

#endif
