#ifndef __COMPONENT_LIST__
#define __COMPONENT_LIST__

#include <vector>

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


class ComponentControl
{
    public:
        virtual void draw(double scale=5.0) = 0;
        virtual std::vector<int> is_intersect(double x, double y, double radius) const = 0;
        virtual ComponentType::ComponentType get_type(int id) const = 0;
        virtual ComponentType::ComponentType get_type() const = 0;
        virtual int update() = 0;
};

#endif