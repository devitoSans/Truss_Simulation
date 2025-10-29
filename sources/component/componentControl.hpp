#ifndef __COMPONENT_LIST__
#define __COMPONENT_LIST__

#include <vector>
#include "../action/action.hpp"

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

        // Set the focused member which is the last member in action.
        void set_focused_member_id()
        {
            if(this->inActionID != -1)
            {
                this->focusedID = this->inActionID;
            }
        }

        void clear_focused_member()
        {
            if(requestAction.click(CLEAR_FOCUS_INPUT(), ActionType::NONE, -1))
            {
                this->focusedID = -1;
            }
        }

    public:
        ComponentController() : focusedID(-1), inActionID(-1) {}

        virtual void draw(double scale=5.0) = 0;
        virtual std::vector<int> is_intersect(double x, double y, double radius) const = 0;
        virtual ComponentType::ComponentType get_type() const = 0;
        virtual ComponentType::ComponentType get_type(int id) const = 0;
        virtual int update() = 0;
};

#endif