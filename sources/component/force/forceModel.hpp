#ifndef __FORCE_MODEL__
#define __FORCE_MODEL__

#include <splashkit.h>
#include "../shapeBase.hpp"
#include "../../mathHelpers/mathHelpers.hpp"
#include "../text/text.hpp"

inline const double FORCE_WIDTH = 4;
inline const double FORCE_LENGTH = 20;
inline const double FORCE_PIN_GIRTH = 4;

inline const double HEAD_RATIO = 1.0 / 3.0;

class ForceModel
{
    private:
        // properties
        double force; // N

        int id;
        OneSideShapeBase baseShape;
        double scaledPinGirth;

        std::string resourcesPath;

        void update_scaled_pin_girth()
        {
            this->scaledPinGirth = FORCE_PIN_GIRTH/2.0 * this->baseShape.get_scale();
        }

    public:
        ForceModel(double initScale=5.0, std::string resourcesPath="")
            : baseShape(initScale, {0.0, 0.0}, FORCE_WIDTH, FORCE_LENGTH, {0.0, -1.0})
        {
            this->force = 1;
            this->id = mh_random(0, MAX_ID);
            this->update_scaled_pin_girth();
            this->resourcesPath = resourcesPath;
        }

        int get_id() const
        {
            return this->id;
        }

        double get_force() const
        {
            return this->force;
        }

        // Get the angle of the force it is acting to (feet, the base, to head--pointy part)
        //
        // In a screen orientation, up is negative and down is positive.
        // So 90 degree vector will points down. To convert it to "normal" orientation,
        // simply negate it with negative, aka. multiplied by -1.
        double get_angle() const
        {
            return ANGLE(vector_angle(this->baseShape.get_body_axis_offset(1,180)));
        }

        vector_2d get_mid_pos() const
        {
            return this->baseShape.get_head_pos();
        }

        // Hit box radius (to connect with other components)
        double get_scaled_pin_girth() const
        {
            return this->scaledPinGirth;
        }

        vector_2d get_arrow_point() const
        {
            return vector_add(
                this->baseShape.get_head_pos(),
                this->baseShape.get_body_axis_offset(this->get_scaled_pin_girth())
            );
        }

        vector_2d get_left_wing_pos() const
        {
            vector_2d offset = vector_add(
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_length() * HEAD_RATIO),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width(), 90)
            );
            return vector_add(this->baseShape.get_head_pos(), offset);
        }

        vector_2d get_right_wing_pos() const
        {
            vector_2d offset = vector_add(
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_length() * HEAD_RATIO),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width(), -90)
            );
            return vector_add(this->baseShape.get_head_pos(), offset);
        }

        vector_2d get_left_feet_pos() const
        {
            return vector_add(
                this->baseShape.get_feet_pos(),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2.0, 90)
            );
        }
        
        vector_2d get_right_feet_pos() const
        {
            return vector_add(
                this->baseShape.get_feet_pos(),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2.0, -90)
            );
        }

        vector_2d get_left_head_pos() const
        {
            vector_2d offset = vector_add(
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_length() * HEAD_RATIO),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2.0, 90)
            );
            return vector_add(this->baseShape.get_head_pos(), offset);
        }

        vector_2d get_right_head_pos() const
        {
            vector_2d offset = vector_add(
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_length() * HEAD_RATIO),
                this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2.0, -90)
            );
            return vector_add(this->baseShape.get_head_pos(), offset);
        }

        void set_force(double force)
        {
            this->force = force;
        }

        void set_scale(double scale)
        {
            this->baseShape.set_scale(scale);
            this->update_scaled_pin_girth();
        }

        void move(double x, double y)
        {
            this->baseShape.move(x,y);
        }

        void rotate(double degree)
        {
            this->baseShape.rotate(degree-180);
        }

        bool is_intersect(double x, double y) const
        {
            quad q;
            q.points[0] = { this->get_left_feet_pos().x, this->get_left_feet_pos().y };
            q.points[1] = { this->get_right_feet_pos().x, this->get_right_feet_pos().y };
            q.points[2] = { this->get_left_head_pos().x, this->get_left_head_pos().y };
            q.points[3] = { this->get_right_head_pos().x, this->get_right_head_pos().y };

            return point_in_quad({x,y}, q) || 
                   point_in_circle(x,y, this->baseShape.get_head_pos().x,
                                        this->baseShape.get_head_pos().y, 
                                        this->get_scaled_pin_girth()) ||
                   point_in_triangle({x,y}, {
                    this->get_left_wing_pos().x, this->get_left_wing_pos().y,
                    this->get_right_wing_pos().x, this->get_right_wing_pos().y,
                    this->get_arrow_point().x, this->get_arrow_point().y
                });
        }

        bool is_intersect(double x, double y, double radius) const
        {
            return circles_intersect(
                circle {x,y,radius},
                circle {
                        this->baseShape.get_head_pos().x,
                        this->baseShape.get_head_pos().y, 
                        this->get_scaled_pin_girth()
                }
            );
        }

        void draw(bool showInfo = false, color forceColor = color_black(), color hitBoxColor = color_dark_gray()) const
        {
            static bool yes = false;
            bool asdf = false;
            bool t = requestAction.is_in_action(ActionType::CALCULATE_FORCE, ActionType::CALCULATE_FORCE);
            if(t)
            {
                yes = true;
            }
            if((requestAction.is_in_action(-1, ActionType::NONE) || t) && yes)
            {
                asdf = true;
            }
            else if(yes)
            {
                yes = false;
            }

            if(showInfo || asdf)
            {
                forceColor = color_red();
            }

            quad q;
            q.points[0] = { this->get_left_feet_pos().x, this->get_left_feet_pos().y };
            q.points[1] = { this->get_right_feet_pos().x, this->get_right_feet_pos().y };
            q.points[2] = { this->get_left_head_pos().x, this->get_left_head_pos().y };
            q.points[3] = { this->get_right_head_pos().x, this->get_right_head_pos().y };

            draw_quad(forceColor, q);
            draw_triangle(forceColor,
                this->get_left_wing_pos().x, this->get_left_wing_pos().y,
                this->get_right_wing_pos().x, this->get_right_wing_pos().y,
                this->get_arrow_point().x, this->get_arrow_point().y
            );

            draw_circle(hitBoxColor, this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y, this->get_scaled_pin_girth());

            if(asdf)
            {
                vector_2d dir = this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2, 180);
                dir = vector_add(dir, this->baseShape.get_feet_pos());
                drawInfo(this->resourcesPath, 
                         this->force, 
                         " N", 
                         forceColor, 
                         dir.x, 
                         dir.y, 
                         this->baseShape.get_scaled_width()/2);
            }
            else if(showInfo)
            {
                drawInfo(this->resourcesPath, 
                         this->force, 
                         " N", 
                         forceColor, 
                         this->baseShape.get_head_pos().x, 
                         this->baseShape.get_head_pos().y, 
                         this->baseShape.get_scaled_width()/2);

                vector_2d dir = this->baseShape.get_body_axis_offset(this->baseShape.get_scaled_width()/2, 180);
                dir = vector_add(dir, this->baseShape.get_feet_pos());
                drawInfo(this->resourcesPath, 
                         ANGLE(-this->get_angle()), 
                         "°", 
                         forceColor, 
                         dir.x, 
                         dir.y, 
                         this->baseShape.get_scaled_width()/2);
            }
        }
};

#endif