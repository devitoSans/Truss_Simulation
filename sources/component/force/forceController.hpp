#ifndef __FORCE_CONTFORCE__
#define __FORCE_CONTFORCE__

#include <map>
#include <vector>
#include "forceModel.hpp"
#include "../componentControl.hpp"
#include "../../action/action.hpp"

// User input for force

inline bool FORCE_CREATE_INPUT()
{
    return key_down(F_KEY);
}
inline bool FORCE_MOVE_INPUT() 
{
    return mouse_down(LEFT_BUTTON);
}
inline bool FORCE_ROTATE_INPUT()
{
    return (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)));
}
inline bool FORCE_TRANSFORM_CONDITION(const ForceModel& X)
{
    return X.is_intersect(mouse_position().x, mouse_position().y);
}

typedef std::map<int, ForceModel> MultiForce;

class MultiForceController : ComponentControl
{
    private:
        MultiForce forces;

        void create_force()
        {
            if(requestAction.click(FORCE_CREATE_INPUT(), ActionType::FORCE_CREATE, ActionType::FORCE_CREATE))
            {
                ForceModel temp = ForceModel();
                temp.move(mouse_position().x, mouse_position().y);
                this->forces.emplace(temp.get_id(), temp);
                this->inActionID = temp.get_id();
            }
        }

        void delete_force()
        {
            if(requestAction.click(DELETE_INPUT(this->focusedID), ActionType::FORCE_DELETE, ActionType::FORCE_DELETE))
            {
                this->forces.erase(this->focusedID);
                this->inActionID = -1;
                this->focusedID = -1;
            }
        }
        
        void move_force(ForceModel& force)
        {
            // see member controller.
            bool firstTimeDrag = requestAction.is_in_action(-1, ActionType::NONE);

            if(!requestAction.hold(FORCE_MOVE_INPUT(), ActionType::FORCE_MOVE, force.get_id(), FORCE_TRANSFORM_CONDITION(force)))
            {
                return;
            }
            this->inActionID = force.get_id();

            vector_2d mousePos = mouse_position_vector();
            static vector_2d diff = mousePos;
            
            if(firstTimeDrag)
            {
                diff = vector_subtract(force.get_mid_pos(), mousePos);
            }

            vector_2d change = vector_add(diff, mousePos);
            force.move(change.x, change.y);
        }

        void rotate_force(ForceModel& force)
        {
            bool firstTimeDrag = requestAction.is_in_action(-1, ActionType::NONE);

            if(!requestAction.hold(FORCE_ROTATE_INPUT(), 
                                   ActionType::FORCE_ROTATE, 
                                   force.get_id(), 
                                   FORCE_TRANSFORM_CONDITION(force)))
            {
                return;
            }
            this->inActionID = force.get_id();

            double angle = force.get_angle();
            // angle = std::fmod(angle, 360.0);

            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract(force.get_mid_pos(), { mousePos.x, mousePos.y });

            double newAngle = vector_angle(direction);

            static double diff = angle;
            if(firstTimeDrag)
            {
                diff = newAngle - angle;
            }

            force.rotate(-diff + newAngle);
        }

    public:
        MultiForceController() : forces() {}

        ComponentType::ComponentType get_type() const override
        {
            return ComponentType::FORCE;
        }

        ComponentType::ComponentType get_type(int id) const override
        {
            return (this->forces.find(id) == this->forces.end() ? ComponentType::NONE : this->get_type());
        }

        int update() override
        {
            this->inActionID = -1;
            this->create_force();
            this->delete_force();

            for(auto& [_, force] : this->forces)
            {
                if(this->focusedID != -1)
                {
                    this->rotate_force(this->forces[this->focusedID]);
                    this->move_force(this->forces[this->focusedID]);
                }
                if(this->focusedID != this->inActionID || this->focusedID == -1)
                {
                    this->rotate_force(force);
                    this->move_force(force);
                }
            }

            this->set_focused_member_id();
            this->clear_focused_member();
            return this->focusedID;
        }

        std::vector<int> is_intersect(double x, double y, double radius) const override
        {
            std::vector<int> intersected_id;
            for(auto& [_, force] : this->forces)
            {
                if(force.is_intersect(x,y,radius))
                {
                    intersected_id.push_back(force.get_id());
                }
            }
            return intersected_id;
        }

        void draw(double scale=5.0) override
        {
            for(auto& [_, force] : this->forces)
            {
                force.set_scale(scale);
                force.draw((this->focusedID == force.get_id() ? color_red() : color_black()),
                            this->focusedID == force.get_id() ? color_dark_red() : color_dark_gray());
            }
        }
};

#endif