#ifndef __COMPONENT_LIST__
#define __COMPONENT_LIST__

#include <utility>
#include <vector>
#include "../action/action.hpp"
#include "../definition.hpp"

// General user input 
inline bool DELETE_INPUT(int id)
{
    if(id == -1) 
    {
        return false;
    }
    return key_down(BACKSPACE_KEY);
}
inline bool CLEAR_FOCUS_INPUT()
{
    return mouse_down(LEFT_BUTTON);
}

class ComponentController
{
    protected:
        int focusedID;
        int inActionID;
        inline static ComponentType::ComponentType focusedComponent = ComponentType::NONE;

        // Set the focused member which is the last member in action.
        void set_focused_member_id(ComponentType::ComponentType type)
        {
            if(this->inActionID != -1)
            {
                this->focusedID = this->inActionID;
                focusedComponent = type;
            }
        }

        void clear_focused_member(ComponentType::ComponentType type)
        {
            if(requestAction.click(CLEAR_FOCUS_INPUT(), ActionType::NONE, -1) || focusedComponent != type)
            {
                this->focusedID = -1;
            }
        }

    public:
        ComponentController() : focusedID(-1), inActionID(-1) {}

        virtual std::vector<Connection> get_intersection(double x, double y, double radius) const = 0;
        virtual std::vector<Hitbox> get_hit_box(int id) = 0;
        virtual ComponentType::ComponentType get_type() const = 0;
        virtual ComponentType::ComponentType get_type(int id) const = 0;
        virtual std::vector<double> get_part_angles(const Connection& connection) const = 0;
        virtual std::vector<double> get_forces(const Connection& connection) const = 0;
        virtual int update(bool canUpdate) = 0;
        virtual void draw(double scale=5.0) = 0;
};

#endif