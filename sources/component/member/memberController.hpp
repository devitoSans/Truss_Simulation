#ifndef __MEMBER_CONTROLLER__
#define __MEMBER_CONTROLLER__

#include <map>
#include <splashkit.h>
#include "memberModel.hpp"
#include "../componentControl.hpp"
#include "../../action/action.hpp"

// User input for member

inline bool MEMBER_CREATE_INPUT() 
{ 
    return key_down(M_KEY); 
} 

inline bool MEMBER_MOVE_INPUT() 
{
    return mouse_down(LEFT_BUTTON);
}
inline bool MEMBER_MOVE_CONDITION(const MemberModel& X)
{
    return X.is_intersect_body(mouse_position().x, mouse_position().y);
}

inline bool MEMBER_ROTATE_ON_START_INPUT()
{
    return (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)));
}
inline bool MEMBER_ROTATE_ON_START_CONDITION(const MemberModel& X)
{
    return X.is_intersect_start(mouse_position().x, mouse_position().y);
}

inline bool MEMBER_ROTATE_ON_END_INPUT()
{
    return (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)));
}
inline bool MEMBER_ROTATE_ON_END_CONDITION(const MemberModel& X)
{
    return X.is_intersect_end(mouse_position().x, mouse_position().y);
}

inline bool MEMBER_MOVE_ON_START_INPUT()
{
    return mouse_down(LEFT_BUTTON);
}
inline bool MEMBER_MOVE_ON_START_CONDITION(const MemberModel& X)
{
    return X.is_intersect_start(mouse_position().x, mouse_position().y);
}

inline bool MEMBER_MOVE_ON_END_INPUT()
{
    return mouse_down(LEFT_BUTTON);
}
inline bool MEMBER_MOVE_ON_END_CONDITION(const MemberModel& X)
{
    return X.is_intersect_end(mouse_position().x, mouse_position().y);
}

typedef std::map<int, MemberModel> MultiMember;

class MultiMemberController : public ComponentController
{
    private:
        MultiMember members;

        void create_member()
        {
            if(requestAction.click(MEMBER_CREATE_INPUT(), ActionType::MEMBER_CREATE, ActionType::MEMBER_CREATE))
            {
                MemberModel tempModel = MemberModel(20, 5.0);
                this->inActionID = tempModel.get_properties().get_id();

                tempModel.modify_mid_pos(mouse_position().x, mouse_position().y);
                this->members[tempModel.get_properties().get_id()] = tempModel;
            }
        }

        void delete_member()
        {
            if(requestAction.click(DELETE_INPUT(this->focusedID), ActionType::MEMBER_DELETE, ActionType::MEMBER_DELETE))
            {
                this->members.erase(this->focusedID);
                this->inActionID= -1;
                this->focusedID = -1;
            }
        }

        void move_member(MemberModel& member)
        {
            // If no one is currently being dragged, this call starts a new drag session.
            bool firstTimeDrag = requestAction.is_in_action(-1, ActionType::NONE);

            const int id = member.get_properties().get_id();
            if(!requestAction.hold(MEMBER_MOVE_INPUT(), ActionType::MEMBER_MOVE, id, MEMBER_MOVE_CONDITION(member)))
            {
                return;
            }
            this->inActionID = id;

            vector_2d mousePos = mouse_position_vector();
            static vector_2d diff = mousePos;
            
            // On the first frame of a drag, capture where on the member the user grabbed it.
            // Store (member_mid - mouse) so the member won’t “snap” its center to the cursor.
            if(firstTimeDrag)
            {
                diff = vector_subtract(member.get_mid_pos(), mousePos);
            }

            vector_2d change = vector_add(diff, mousePos);
            member.modify_mid_pos(change.x, change.y);
        }

        void rotate_start_member(MemberModel& member)
        {
            const int id = member.get_properties().get_id();
            if(!requestAction.hold(MEMBER_ROTATE_ON_START_INPUT(), ActionType::MEMBER_ROTATE_ON_START, id, MEMBER_ROTATE_ON_START_CONDITION(member)))
            {
                return;
            }
            this->inActionID = id;
            
            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract({ mousePos.x, mousePos.y }, member.get_end_pos());
            member.modify_counter_angle(vector_angle(direction));
        }

        void rotate_end_member(MemberModel& member)
        {
            const int id = member.get_properties().get_id();
            if(!requestAction.hold(MEMBER_ROTATE_ON_END_INPUT(), ActionType::MEMBER_ROTATE_ON_END, id, MEMBER_ROTATE_ON_END_CONDITION(member)))
            {
                return;
            }
            this->inActionID = id;
            
            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract({ mousePos.x, mousePos.y }, member.get_start_pos());
            member.modify_angle(vector_angle(direction));
        }

        void move_start_member(MemberModel& member)
        {
            const int id = member.get_properties().get_id();
            if(!requestAction.hold(MEMBER_MOVE_ON_START_INPUT(), ActionType::MEMBER_MOVE_ON_START, id, MEMBER_MOVE_ON_START_CONDITION(member)))
            {
                return;
            }
            this->inActionID = id;

            point_2d mousePos = mouse_position();
            member.modify_start_pos(mousePos.x, mousePos.y);
        }
        
        void move_end_member(MemberModel& member)
        {
            const int id = member.get_properties().get_id();
            if(!requestAction.hold(MEMBER_MOVE_ON_END_INPUT(), ActionType::MEMBER_MOVE_ON_END, id, MEMBER_MOVE_ON_END_CONDITION(member)))
            {
                return;
            }
            this->inActionID = id;

            point_2d mousePos = mouse_position();
            member.modify_end_pos(mousePos.x, mousePos.y);
        }

    public:
        // ..
        MultiMemberController() : members() {}

        MultiMember& get_members()
        {
            return this->members;
        }

        ComponentType::ComponentType get_type() const override
        {
            return ComponentType::MEMBER;
        }

        ComponentType::ComponentType get_type(int id) const override
        {
            return (this->members.find(id) == this->members.end() ? ComponentType::NONE : this->get_type());
        }

        std::vector<Hitbox> get_hit_box(int id) override
        {
            MemberModel idedMember = this->members.at(id);
            return { 
                Hitbox {
                    circle { idedMember.get_start_pos().x, idedMember.get_start_pos().y, idedMember.get_scaled_girth()/2 },
                    Part::MEMBER_START
                },
                Hitbox {
                    circle { idedMember.get_end_pos().x, idedMember.get_end_pos().y, idedMember.get_scaled_girth()/2 },
                    Part::MEMBER_END
                }
            };
        }

        // negative means it is connected to the start, while positive means there's connection at the end.
        std::vector<Connection> get_intersection(double x, double y, double radius) const override
        {
            std::vector<Connection> intersected_id;
            for(auto& [_, member] : this->members)
            {
                if(member.is_intersect_start(x,y,radius))
                {
                    intersected_id.push_back({member.get_id(), Part::MEMBER_START});
                }
                else if(member.is_intersect_end(x,y,radius))
                {
                    intersected_id.push_back({member.get_id(), Part::MEMBER_END});
                }
            }
            return intersected_id;
        }

        // return which member's id that is currently in focus (or selected)
        int update(bool canUpdate) override
        {   
            // reset 
            this->inActionID = -1;
            this->create_member();
            this->delete_member();
              
            if(canUpdate)
            {
                for(auto& [_, member] : this->members)
                {
                    if(this->focusedID != -1)
                    {
                        this->rotate_start_member(this->members.at(this->focusedID));
                        this->rotate_end_member(this->members.at(this->focusedID));
                        this->move_start_member(this->members.at(this->focusedID));
                        this->move_end_member(this->members.at(this->focusedID));
                        this->move_member(this->members.at(this->focusedID));
                    }
                    if(this->focusedID != this->inActionID || this->focusedID == -1)
                    {
                        this->rotate_start_member(member);
                        this->rotate_end_member(member);
                        this->move_start_member(member);
                        this->move_end_member(member);
                        this->move_member(member);
                    }
                }
            }

            this->set_focused_member_id(ComponentType::MEMBER);
            this->clear_focused_member(ComponentType::MEMBER);
            return this->focusedID;
        }
        
        void draw(double scale=5.0) override
        {
            for(auto& [_, member] : this->members)
            {
                member.set_scale(scale);
                const int id = member.get_properties().get_id();
                member.draw((this->focusedID == id ? color_red() : color_black()));
            }
        }
};

#endif