#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

namespace LHModelNS
{
    std::ostream& operator<<( std::ostream& os, const MemberMeta& memberMeta )
    {
        os << memberMeta.name << "|" << memberMeta.type << "|" << memberMeta.format;
        return os;
    }

    std::ostream& operator<<( std::ostream& os, const ModelMeta& modelMeta )
    {
        os << modelMeta.name;
        return os;
    }
}
