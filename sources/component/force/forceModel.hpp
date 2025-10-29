#ifndef __FORCE_MODEL__
#define __FORCE_MODEL__

#include <splashkit.h>
#include "../shapeBase.hpp"
#include "../../mathHelpers/mathHelpers.hpp"

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

        void update_scaled_pin_girth()
        {
            this->scaledPinGirth = FORCE_PIN_GIRTH/2.0 * this->baseShape.get_scale();
        }

    public:
        ForceModel(double initScale=5.0)
            : baseShape(initScale, {0.0, 0.0}, FORCE_WIDTH, FORCE_LENGTH, {0.0, -1.0})
        {
            this->id = mh_random(0, MAX_ID);
            this->update_scaled_pin_girth();
        }

        int get_id() const
        {
            return this->id;
        }

        double get_force() const
        {
            return this->force;
        }

        double get_angle() const
        {
            return vector_angle(this->baseShape.get_body_axis_offset(1,180));
        }

        const vector_2d& get_mid_pos() const
        {
            return this->baseShape.get_head_pos();
        }

        // Hit box radius (to connect with other components)
        double get_scaled_pin_girth() const
        {
            return this->scaledPinGirth;
        }

        vector_2d get_hit_box_pos() const
        {
            return vector_add(
                this->baseShape.get_head_pos(),
                this->baseShape.get_body_axis_offset(this->get_scaled_pin_girth(), 180)
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
                   point_in_circle(x,y, this->get_hit_box_pos().x,
                                        this->get_hit_box_pos().y, 
                                        this->get_scaled_pin_girth()) ||
                   point_in_triangle({x,y}, {
                    this->get_left_wing_pos().x, this->get_left_wing_pos().y,
                    this->get_right_wing_pos().x, this->get_right_wing_pos().y,
                    this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y,
                });
        }

        bool is_intersect(double x, double y, double radius) const
        {
            return circles_intersect(
                circle {x,y,radius},
                circle {
                        this->get_hit_box_pos().x,
                        this->get_hit_box_pos().y, 
                        this->get_scaled_pin_girth()
                }
            );
        }

        void draw(color forceColor = color_black(), color hitBoxColor = color_dark_gray()) const
        {
            quad q;
            q.points[0] = { this->get_left_feet_pos().x, this->get_left_feet_pos().y };
            q.points[1] = { this->get_right_feet_pos().x, this->get_right_feet_pos().y };
            q.points[2] = { this->get_left_head_pos().x, this->get_left_head_pos().y };
            q.points[3] = { this->get_right_head_pos().x, this->get_right_head_pos().y };

            draw_quad(forceColor, q);
            draw_triangle(forceColor,
                this->get_left_wing_pos().x, this->get_left_wing_pos().y,
                this->get_right_wing_pos().x, this->get_right_wing_pos().y,
                this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y
            );

            draw_circle(hitBoxColor, this->get_hit_box_pos().x, this->get_hit_box_pos().y, this->get_scaled_pin_girth());
        }
};

#endif