#ifndef __DEFINITION__
#define __DEFINITION__

#include <utility>
#include <splashkit.h>

namespace ComponentType
{
    enum ComponentType
    {
        NONE,
        MEMBER,
        SUPPORT,
        FORCE
    };
}

namespace Part
{
    enum PartType
    {
        NONE,

        // Member's parts
        MEMBER_START,
        MEMBER_END,

        // Support's parts
        SUPPORT_HEAD,

        // Force's parts
        FORCE_DOWN
    };
}

namespace ForceType
{
    enum type
    {
        LOAD,

        VERTICAL_REACTION,
        HORIZONTAL_REACTION,

        AXIAL
    };

    struct value
    {
        double _val;
        ForceType::type _typ;
    };
}

struct ForceAngle
{
    double angle;
    ForceType::type type;
};

// first: component's id. 
// second: the part that is connected
typedef std::pair<int,Part::PartType> Connection;
// first: part's circle. 
// second: the part that is considered hitbox
struct Hitbox
{
    circle shape;
    Part::PartType type;
};

#endif