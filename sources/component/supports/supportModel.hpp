#ifndef __SUPPORT__
#define __SUPPORT__

#include <splashkit.h>
#include "../../mathHelpers/mathHelpers.hpp"
#include "../shapeBase.hpp"

inline const double DEFAULT_PIN_GIRTH = 4.0;

struct SupportData
{
    double verticalForce = 0;
    double horizontalForce = 0;

    bool hasVertical = false;
    bool hasHorizontal = false;

    double pinGirth = DEFAULT_PIN_GIRTH; 

    SupportData(bool hasVertical, bool hasHorizontal)
    {
        this->hasVertical = hasVertical;
        this->hasHorizontal = hasHorizontal;
    }
};

class SupportModel
{
    public:
        SupportModel() = default;
        virtual ~SupportModel() = default; 
        SupportModel(const SupportModel&) = default;
        SupportModel& operator=(const SupportModel&) = default;
        virtual SupportModel* clone() const = 0;

        virtual int get_id() const = 0;
        virtual SupportData& get_properties() = 0;
        virtual vector_2d get_mid_pos() const = 0;

        virtual void set_scale(double newScale) = 0;

        virtual void move(double x, double y) = 0;
        virtual void rotate(double degree) = 0;

        virtual bool is_intersect(double x, double y) const = 0;
        virtual bool is_intersect(double x, double y, double radius) const = 0;

        virtual void draw(color memberColor = color_black()) = 0;
};

// class PinBaseModel : public SupportModel
// {
//     protected:
//         int id;
//         double scale;

//         SupportData properties;

//         double scaledGirth;
//         double scaledHeight;
//         vector_2d midPos;
//         vector_2d feetPos;
    
//     public:
//         PinBaseModel(double initScale, double height, double girth, bool hasVerticalReaction, bool hasHorizontalReaction)
//             : properties(hasVerticalReaction, hasHorizontalReaction)
//         {
//             this->id = mh_random(0,MAX_ID);
//             this->scale = initScale;
//             this->midPos = { 0.0, 0.0 };

//             this->scaledGirth = girth * this->scale;
//             this->scaledHeight = height * this->scale;
//             this->feetPos = { 0.0, this->scaledHeight };
//         }

//         virtual SupportModel* clone() const = 0;

//         int get_id() const override
//         {
//             return this->id;
//         }

//         double get_scaled_height() const
//         {
//             return this->scaledHeight;
//         }

//         double get_scaled_girth() const
//         {
//             return this->scaledGirth;
//         }

//         SupportData& get_properties()
//         {
//             return this->properties;
//         }

//         vector_2d get_mid_pos() const override
//         {
//             return this->midPos;
//         }

//         vector_2d get_left_pos() const
//         {
//             vector_2d firstDirection = vector_subtract(this->feetPos, this->midPos);
//             double angle = vector_angle(firstDirection);
//             vector_2d secondDirection = vector_from_angle(angle+90, this->scaledGirth/2);
//             return vector_add(this->feetPos, secondDirection);
//         }

//         vector_2d get_right_pos() const
//         {
//             vector_2d firstDirection = vector_subtract(this->feetPos, this->midPos);
//             double angle = vector_angle(firstDirection);
//             vector_2d secondDirection = vector_from_angle(angle-90, this->scaledGirth/2);
//             return vector_add(this->feetPos, secondDirection);
//         }

//         void set_scale(double newScale) override
//         {

//         }

//         void move(double x, double y) override
//         {
//             vector_2d direction = vector_subtract({x,y}, this->midPos);
//             this->midPos = matrix_multiply(translation_matrix(direction), this->midPos);
//             this->feetPos = matrix_multiply(translation_matrix(direction), this->feetPos);
//         }

//         void rotate(double degree) override
//         {
//             double angle = std::fmod(degree, 360.0);
//             angle += (angle < 0.0 ? 360.0 : 0);

//             vector_2d direction = vector_subtract(this->feetPos, this->midPos);
//             double currentAngle = vector_angle(direction);
//             direction = matrix_multiply(rotation_matrix(angle - currentAngle), direction);
//             this->feetPos = vector_add(this->midPos, direction);
//         }

//         bool is_intersect(double x, double y) const override
//         {
//             return point_in_triangle({x,y}, {
//                                         this->midPos.x, this->midPos.y,
//                                         this->get_left_pos().x, this->get_left_pos().y,
//                                         this->get_right_pos().x, this->get_right_pos().y
//                                     }) ||
//                    point_in_circle({x,y}, {
//                                         this->midPos.x, this->midPos.y,
//                                         this->scaledGirth/2
//                                     });
//         }

//         bool is_intersect(double x, double y, double radius) const override
//         {
//             return circles_intersect(this->midPos.x, this->midPos.y, this->scaledGirth/2,
//                                      x, y, radius);
//         }

//         void draw(color supportColor = color_black()) override
//         {
//             draw_circle(supportColor, this->midPos.x, this->midPos.y, this->scaledGirth/2);
//             draw_triangle(supportColor, this->midPos.x, this->midPos.y,
//                                         this->get_left_pos().x, this->get_left_pos().y,
//                                         this->get_right_pos().x, this->get_right_pos().y);
//         }
// };

class PinBaseModel : public SupportModel
{
    protected:
        int id;
        SupportData properties;
        OneSideShapeBase baseShape;

        double get_pin_angle() const
        {
            vector_2d direction = vector_subtract(this->baseShape.get_feet_pos(), this->baseShape.get_head_pos());
            return vector_angle(direction);
        }
    
    public:
        PinBaseModel(double initScale, double height, double girth, bool hasVerticalReaction, bool hasHorizontalReaction)
            : properties(hasVerticalReaction, hasHorizontalReaction),
              baseShape(initScale, {0.0, 0.0}, girth, height, {0.0, 1.0})
        {
            this->id = mh_random(0,MAX_ID);
        }

        virtual SupportModel* clone() const = 0;

        int get_id() const override
        {
            return this->id;
        }

        double get_scaled_height() const
        {
            return this->baseShape.get_scaled_length();
        }

        double get_scaled_girth() const
        {
            return this->baseShape.get_scaled_width();
        }

        SupportData& get_properties()
        {
            return this->properties;
        }

        vector_2d get_mid_pos() const override
        {
            return this->baseShape.get_head_pos();
        }

        vector_2d get_left_pos() const
        {
            vector_2d secondDirection = vector_from_angle(this->get_pin_angle()+90, this->get_scaled_girth()/2);
            return vector_add(this->baseShape.get_feet_pos(), secondDirection);
        }

        vector_2d get_right_pos() const
        {
            vector_2d secondDirection = vector_from_angle(this->get_pin_angle()-90, this->get_scaled_girth()/2);
            return vector_add(this->baseShape.get_feet_pos(), secondDirection);
        }

        void set_scale(double newScale) override
        {
            this->baseShape.set_scale(newScale);
        }

        void move(double x, double y) override
        {
            this->baseShape.move(x,y);
        }

        void rotate(double degree) override
        {
            this->baseShape.rotate(degree);
        }

        bool is_intersect(double x, double y) const override
        {
            return point_in_triangle({x,y}, {
                                        this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y,
                                        this->get_left_pos().x, this->get_left_pos().y,
                                        this->get_right_pos().x, this->get_right_pos().y
                                    }) ||
                   point_in_circle({x,y}, {
                                        this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y,
                                        this->get_scaled_girth()/2
                                    });
        }

        bool is_intersect(double x, double y, double radius) const override
        {
            return circles_intersect(this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y, this->get_scaled_girth()/2,
                                     x, y, radius);
        }

        void draw(color supportColor = color_black()) override
        {
            draw_circle(supportColor, this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y, this->get_scaled_girth()/2);
            draw_triangle(supportColor, this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y,
                                        this->get_left_pos().x, this->get_left_pos().y,
                                        this->get_right_pos().x, this->get_right_pos().y);
        }
};

class PinJointModel : public PinBaseModel
{
    public:
        PinJointModel(double initScale=5.0) 
            : PinBaseModel(initScale, DEFAULT_PIN_GIRTH*1.1, DEFAULT_PIN_GIRTH, true, true) {}

        SupportModel* clone() const override
        {
            return new PinJointModel(*this);
        }
};

class Roller : public PinBaseModel
{
    private:
        double scaledWheelRadius;
        vector_2d midWheelPos;
        vector_2d leftWheelPos;
        vector_2d rightWheelPos;

        void update_mid_wheel_pos()
        {
            vector_2d offset = vector_from_angle(this->get_pin_angle(), this->scaledWheelRadius);
            this->midWheelPos = vector_add(offset, this->baseShape.get_feet_pos());
        }

        void update_left_wheel_pos()
        {
            vector_2d offset = vector_from_angle(this->get_pin_angle()+90, this->scaledWheelRadius*2);
            this->leftWheelPos = vector_add(this->midWheelPos, offset);
        }

        void update_right_wheel_pos()
        {
            vector_2d offset = vector_from_angle(this->get_pin_angle()-90, this->scaledWheelRadius*2);
            this->rightWheelPos = vector_add(this->midWheelPos, offset);
        }

        void update()
        {
            this->update_mid_wheel_pos();
            this->update_left_wheel_pos();
            this->update_right_wheel_pos();
        }

    public:
        Roller(double initScale=5.0) 
            : PinBaseModel(initScale, DEFAULT_PIN_GIRTH*23.0/30.0, DEFAULT_PIN_GIRTH, true, false)
        {
            this->scaledWheelRadius = (DEFAULT_PIN_GIRTH * initScale / 3.0) / 2.0;
            this->update();
        }

        SupportModel* clone() const override
        {
            return new Roller(*this);
        }

        void move(double x, double y)
        {
            PinBaseModel::move(x,y);
            this->update();
        }

        void rotate(double degree)
        {
            PinBaseModel::rotate(degree);
            this->update();
        }

        bool is_intersect(double x, double y)
        {
            // vector_2d direction = vector_subtract(this->feetPos, this->midPos);
            // direction = vector_from_angle(vector_angle(direction), this->scaledWheelRadius*2);

            // quad q;
            // q.points[0] = { this->get_left_pos().x, this->get_left_pos().y };
            // q.points[1] = { this->get_right_pos().x, this->get_right_pos().y };
            // q.points[2] = { this->get_left_pos().x+direction.x, this->get_left_pos().y+direction.y};
            // q.points[3] = { this->get_right_pos().x+direction.x, this->get_right_pos().y+direction.y };

            // return PinBaseModel::is_intersect(x,y) || point_in_quad({x,y},q);
            return PinBaseModel::is_intersect(x,y);
        }

        void draw(color supportColor = color_black())
        {
            PinBaseModel::draw(supportColor);
            draw_circle(supportColor, this->midWheelPos.x, this->midWheelPos.y, this->scaledWheelRadius);
            draw_circle(supportColor, this->leftWheelPos.x, this->leftWheelPos.y, this->scaledWheelRadius);
            draw_circle(supportColor, this->rightWheelPos.x, this->rightWheelPos.y, this->scaledWheelRadius);
        }
};


#endif