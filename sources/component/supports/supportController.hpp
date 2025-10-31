#ifndef __SUPPORT_CONTROLLER__
#define __SUPPORT_CONTROLLER__

#include <map>
#include <vector>
#include <splashkit.h>
#include "../componentControl.hpp"
#include "../../action/action.hpp"
#include "supportModel.hpp"

// User input for support

inline bool PIN_CREATE_INPUT()
{
    return key_down(P_KEY);
}
inline bool ROLLER_CREATE_INPUT()
{
    return key_down(R_KEY);
}

inline bool SUPPORT_MOVE_INPUT() 
{
    return mouse_down(LEFT_BUTTON);
}
inline bool SUPPORT_ROTATE_INPUT()
{
    return (mouse_down(LEFT_BUTTON) && (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY)));
}
inline bool SUPPORT_TRANSFORM_CONDITION(const SupportModel* X)
{
    return X->is_intersect(mouse_position().x, mouse_position().y);
}

typedef std::map<int, SupportModel*> MultiSupport;

class MultiSupportController : public ComponentController
{
    private:
        MultiSupport supports;

        void erase_suport(int id)
        {
            delete this->supports[id];
            this->supports.erase(id);
        }

        void create_pin()
        {
            if(requestAction.click(PIN_CREATE_INPUT(), ActionType::SUPPORT_PIN_CREATE, ActionType::SUPPORT_PIN_CREATE))
            {
                auto* ptr = new PinJointModel();
                ptr->move(mouse_position().x, mouse_position().y);
                this->supports.emplace(ptr->get_id(), ptr);
                this->inActionID = ptr->get_id();
            }
        }
        
        void create_roller()
        {
            if(requestAction.click(ROLLER_CREATE_INPUT(), ActionType::SUPPORT_ROLLER_CREATE, ActionType::SUPPORT_ROLLER_CREATE))
            {
                auto* ptr = new Roller();
                ptr->move(mouse_position().x, mouse_position().y);
                this->supports.emplace(ptr->get_id(), ptr);
                this->inActionID = ptr->get_id();
            }
        }

        void delete_support()
        {
            if(requestAction.click(DELETE_INPUT(this->focusedID), ActionType::SUPPORT_DELETE, ActionType::SUPPORT_DELETE))
            {
                erase_suport(this->focusedID);
                this->inActionID = -1;
                this->focusedID = -1;
            }
        }
        
        void move_support(SupportModel* support)
        {
            // see member controller.
            bool firstTimeDrag = requestAction.is_in_action(-1, ActionType::NONE);

            if(!requestAction.hold(SUPPORT_MOVE_INPUT(), ActionType::SUPPORT_MOVE, support->get_id(), SUPPORT_TRANSFORM_CONDITION(support)))
            {
                return;
            }
            this->inActionID = support->get_id();

            vector_2d mousePos = mouse_position_vector();
            static vector_2d diff = mousePos;
            
            if(firstTimeDrag)
            {
                diff = vector_subtract(support->get_mid_pos(), mousePos);
            }

            vector_2d change = vector_add(diff, mousePos);
            support->move(change.x, change.y);
        }

        void rotate_support(SupportModel* support)
        {
            bool firstTimeDrag = requestAction.is_in_action(-1, ActionType::NONE);

            if(!requestAction.hold(SUPPORT_ROTATE_INPUT(), 
                                   ActionType::SUPPORT_ROTATE, 
                                   support->get_id(), 
                                   SUPPORT_TRANSFORM_CONDITION(support)))
            {
                return;
            }
            this->inActionID = support->get_id();

            double angle = support->get_angle();
            
            point_2d mousePos = mouse_position();
            vector_2d direction = vector_subtract({ mousePos.x, mousePos.y }, support->get_mid_pos());
            double newAngle = vector_angle(direction);

            static double diff = angle;
            if(firstTimeDrag)
            {
                diff = newAngle - angle;
            }

            support->rotate(newAngle - diff);
        }

    public:
        MultiSupportController() : supports() {}
        ~MultiSupportController()
        {
            for(auto& [_, support] : this->supports)
            {
                delete support;
            }
        }
        MultiSupportController(const MultiSupportController& obj) 
        {
            for (const auto& [id, support] : obj.supports)
            {
                supports.emplace(id, support ? support->clone() : nullptr);
            }
        }
        MultiSupportController& operator=(MultiSupportController obj) 
        {
            this->swap(*this, obj);
            return *this;
        }

        void swap(MultiSupportController& a, MultiSupportController& b)
        {
            std::swap(a.supports, b.supports);
        }

        ComponentType::ComponentType get_type() const override
        {
            return ComponentType::SUPPORT;
        }

        ComponentType::ComponentType get_type(int id) const override
        {
            return (this->supports.find(id) == this->supports.end() ? ComponentType::NONE : this->get_type());
        }

        std::vector<Hitbox> get_hit_box(int id) override
        {
            const SupportModel* idedSupport = this->supports.at(id);
            return { 
                Hitbox {
                    circle {
                        idedSupport->get_mid_pos().x,
                        idedSupport->get_mid_pos().y,
                        idedSupport->get_scaled_girth()/2
                    }, 
                    Part::SUPPORT_HEAD
                } 
            };
        }

        std::vector<Connection> get_intersection(double x, double y, double radius) const override
        {
            std::vector<Connection> intersected_id;
            for(auto& [_, support] : this->supports)
            {
                if(support->is_intersect(x,y,radius))
                {
                    intersected_id.push_back({support->get_id(), Part::SUPPORT_HEAD});
                }
            }
            return intersected_id;
        }

        std::vector<double> get_part_angles(const Connection& connection) const override
        {
            if(this->get_type(connection.first) != ComponentType::SUPPORT)
            {
                fprintf(stderr, "\n\nWarning: Attempting to get an unidentified support's angles, aka. ID is not found.\n");
                return {};
            }
            const SupportModel* support = this->supports.at(connection.first);
            std::vector<double> angles;

            if(support->read_properties().hasVertical)
            {
                angles.push_back(ANGLE(-support->get_angle()+180));
            }
            if(support->read_properties().hasHorizontal)
            {
                angles.push_back(ANGLE(-support->get_angle()+90));
            }
            return angles;
        }

        std::vector<double> get_forces(const Connection& connection) const override
        {
            if(this->get_type(connection.first) != ComponentType::SUPPORT)
            {
                fprintf(stderr, "\n\nWarning: Attempting to get an unidentified support's forces, aka. ID is not found.\n");
                return {};
            }
            return { 
                this->supports.at(connection.first)->read_properties().verticalForce,
                this->supports.at(connection.first)->read_properties().horizontalForce
            };
        }

        int update(bool canUpdate) override
        {
            this->inActionID = -1;
            this->create_pin();
            this->create_roller();
            this->delete_support();

            if(canUpdate)
            {
                for(auto& [_, support] : this->supports)
                {
                    if(this->focusedID != -1)
                    {
                        this->rotate_support(this->supports[this->focusedID]);
                        this->move_support(this->supports[this->focusedID]);
                    }
                    if(this->focusedID != this->inActionID || this->focusedID == -1)
                    {
                        this->rotate_support(support);
                        this->move_support(support);
                    }
                }
            }

            this->set_focused_member_id(ComponentType::SUPPORT);
            this->clear_focused_member(ComponentType::SUPPORT);
            return this->focusedID;
        }

        void draw(double scale=5.0) override
        {
            for (auto it = this->supports.rbegin(); it != this->supports.rend(); it++) 
            {
                auto& support = it->second;
                support->set_scale(scale);
                support->draw((this->focusedID == support->get_id() ? color_red() : color_black()));
            }
        }
};

#endif