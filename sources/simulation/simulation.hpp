#ifndef __SIMULATION__
#define __SIMULATION__

#include <map>
#include "../component/component.hpp"
#include "../inputHandler/inputHandler.hpp"

class Simulation
{
    private:
        std::map<int, cmp::component> components;
        std::map<int, std::vector<int>> joints;
        bool isPerformingAction;

    public:
        Simulation()
        {
            this->isPerformingAction = false;
        }

        void createMember()
        {
            if(create_member_condition())
            {
                cmp::component comp = cmp::convert(MemberInterface());
                cmp::member(comp)->set_scale(5);
                cmp::member(comp)->modify_mid_pos(mouse_position().x, mouse_position().y);
                this->components[comp.id] = comp;
            }
        }

        void update()
        {
            this->isPerformingAction = this->is_component_performing_action();

            for(auto& [_, comp] : this->components)
            {
                // Member
                this->rotate_start_member(comp);
                this->rotate_end_member(comp);
                this->change_start_member(comp);
                this->change_end_member(comp);
                this->move_member(comp);
            }
        }

        bool is_component_performing_action()
        {
            bool performedAction = false;
            for(auto& [_, comp] : this->components)
            {
                if(comp.status != cmp::NONE)
                {
                    performedAction = true;
                    break;
                }
            }
            return performedAction;
        }

        bool action_condition(cmp::component& comp, cmp::componentStatus status, bool (*userConditions)(cmp::component&))
        {
            if(!(comp.status == status || !this->isPerformingAction))
            {
                return false;
            }
            if(userConditions(comp))
            {
                this->isPerformingAction = true;
                return true;
            }
            return false;
        }

        void move_member(cmp::component& comp)
        {
            bool firstTime = !this->isPerformingAction;

            if(!action_condition(comp, cmp::MEMBER_MOVE, move_member_condition))
            {
                return;
            }

            vector_2d mousePos = mouse_position_vector();
            static vector_2d diff = mousePos;
            
            if(firstTime)
            {
                diff = vector_subtract(cmp::member(comp)->get_mid_pos(), mousePos);
            }

            vector_2d change = vector_add(diff, mousePos);
            cmp::member(comp)->modify_mid_pos(change.x, change.y);
        }
        
        void rotate_start_member(cmp::component& comp)
        {
            if(!action_condition(comp, cmp::MEMBER_ROTATE_START, rotate_start_member_condition))
            {
                return;
            }
            
            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract({ mousePos.x, mousePos.y }, cmp::member(comp)->get_end_pos());
            cmp::member(comp)->modify_counter_angle(vector_angle(direction));
        }

        void rotate_end_member(cmp::component& comp)
        {
            if(!action_condition(comp, cmp::MEMBER_ROTATE_END, rotate_end_member_condition))
            {
                return;
            }
            
            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract({ mousePos.x, mousePos.y }, cmp::member(comp)->get_start_pos());
            cmp::member(comp)->modify_angle(vector_angle(direction));
        }

        void change_start_member(cmp::component& comp)
        {
            if(!action_condition(comp, cmp::MEMBER_CHANGE_START, change_start_member_condition))
            {
                return;
            }

            point_2d mousePos = mouse_position();
            MemberInterface* member = cmp::member(comp);
            member->modify_start_pos(mousePos.x, mousePos.y);
        }
        
        void change_end_member(cmp::component& comp)
        {
            if(!action_condition(comp, cmp::MEMBER_CHANGE_END, change_end_member_condition))
            {
                return;
            }

            point_2d mousePos = mouse_position();
            MemberInterface* member = cmp::member(comp);
            member->modify_end_pos(mousePos.x, mousePos.y);
        }

        void drawAllMember()
        {
            for(auto& [_, comp] : this->components)
            {
                cmp::member(comp)->draw();
            }
        }
};

#endif