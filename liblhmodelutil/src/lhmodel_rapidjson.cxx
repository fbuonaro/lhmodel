#include <lhmodelutil/lhmodel_rapidjson.h>

namespace LHModelUtilNS
{
    DeseriailzationFailure::DeseriailzationFailure( const std::string& failure )
    :   std::runtime_error( failure )
    {
    }

    DeseriailzationFailure::~DeseriailzationFailure()
    {
    }
}
