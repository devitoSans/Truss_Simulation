#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__

#include <vector>
#include <splashkit.h>
#include "../component/component.hpp"

// Check if user requesting to create a new member (clicking m key)
inline bool create_member_condition()
{
    static int isReady = true;
    if(isReady && key_down(M_KEY))
    {
        isReady = false;
        return true;
    }
    if(!isReady && !key_down(M_KEY))
    {
        isReady = true;
    }
    return false;
}

inline bool hold_component_conditions(cmp::component& comp, cmp::componentStatus status, bool condition, bool userInput=mouse_down(LEFT_BUTTON))
{
    if(!userInput && comp.status == status)
    {
        comp.status = cmp::NONE;
        return false;
    }

    if(condition && userInput || comp.status == status)
    {
        comp.status = status;
        return true;
    }
    return false;
}

inline bool move_member_condition(cmp::component& comp)
{
    if(comp.ct != cmp::MEMBER)
    {
        return false;
    }
    
    MemberInterface* member = std::get_if<MemberInterface>(&comp.d);
    point_2d mousePos = mouse_position();

    return hold_component_conditions(comp, cmp::MEMBER_MOVE, member->is_intersect_body(mousePos.x, mousePos.y));
}

inline bool rotate_start_member_condition(cmp::component& comp)
{
    if(comp.ct != cmp::MEMBER)
    {
        return false;
    }
    
    MemberInterface* member = std::get_if<MemberInterface>(&comp.d);
    point_2d mousePos = mouse_position();

    return hold_component_conditions(comp, cmp::MEMBER_ROTATE_START,
                                 member->is_intersect_start(mousePos.x, mousePos.y),
                                 (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)) ));
}

inline bool rotate_end_member_condition(cmp::component& comp)
{
    if(comp.ct != cmp::MEMBER)
    {
        return false;
    }
    
    MemberInterface* member = std::get_if<MemberInterface>(&comp.d);
    point_2d mousePos = mouse_position();

    return hold_component_conditions(comp, cmp::MEMBER_ROTATE_END,
                                 member->is_intersect_end(mousePos.x, mousePos.y),
                                 (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)) ));
}

inline bool change_start_member_condition(cmp::component& comp)
{
    if(comp.ct != cmp::MEMBER)
    {
        return false;
    }
    
    MemberInterface* member = std::get_if<MemberInterface>(&comp.d);
    point_2d mousePos = mouse_position();

    return hold_component_conditions(comp, cmp::MEMBER_CHANGE_START,
                                 member->is_intersect_start(mousePos.x, mousePos.y),
                                 (mouse_down(LEFT_BUTTON)));
}

inline bool change_end_member_condition(cmp::component& comp)
{
    if(comp.ct != cmp::MEMBER)
    {
        return false;
    }
    
    MemberInterface* member = std::get_if<MemberInterface>(&comp.d);
    point_2d mousePos = mouse_position();

    return hold_component_conditions(comp, cmp::MEMBER_CHANGE_END,
                                 member->is_intersect_end(mousePos.x, mousePos.y),
                                 (mouse_down(LEFT_BUTTON)));
}

#endif