#ifndef __SUPPORT__
#define __SUPPORT__

#include <splashkit.h>
#include "../../mathHelpers/mathHelpers.hpp"
#include "../shapeBase.hpp"
#include "../text/text.hpp"
#include "../../sidebar/sidebar.hpp"

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
        virtual const SupportData& read_properties() const = 0;
        virtual SupportData& get_properties() = 0;
        virtual vector_2d get_mid_pos() const = 0;
        virtual double get_angle() const = 0;
        virtual double get_scaled_girth() const = 0;
        virtual bool get_show_info() const =0;

        virtual void set_scale(double newScale) = 0;
        virtual void set_show_info(bool info) = 0;

        virtual void move(double x, double y) = 0;
        virtual void rotate(double degree) = 0;

        virtual bool is_intersect(double x, double y) const = 0;
        virtual bool is_intersect(double x, double y, double radius) const = 0;

        virtual void draw(bool showInfo = false, color memberColor = color_black()) = 0;
};

class SupportContent : public InfoContent
{
    private:
        SupportModel* data; 

    public:
        SupportContent() : data(nullptr) {}

        void set_support_data(SupportModel* data)
        {
            this->data = data;
        }

        void update()
        {
            if(this->data == nullptr) ingfokan->set_content(nullptr);
            else ingfokan->set_content(this);
        }

        void draw() override
        {
            if(data == nullptr) return;

            label_element("");
            label_element("Component Type: Support");
            label_element("");

            // Angle
            double newAngle = ANGLE(-this->data->get_angle());
            if(this->draw_properties("Angle", newAngle, newAngle, 1))
            {
                this->data->rotate(ANGLE(-newAngle));
            }
            bool showAngle = this->data->get_show_info();
            this->show_properties("Show angle?", showAngle, showAngle);
            this->data->set_show_info(showAngle);
            label_element("");
        }
};
inline SupportContent supportContent = SupportContent();


class PinBaseModel : public SupportModel
{
    protected:
        int id;
        SupportData properties;
        OneSideShapeBase baseShape;
        std::string resourcesPath;
        bool showAngleInfo;
    
    public:
        PinBaseModel(double initScale, double height, double girth, bool hasVerticalReaction, bool hasHorizontalReaction, std::string resourcesPath)
            : properties(hasVerticalReaction, hasHorizontalReaction),
              baseShape(initScale, {0.0, 0.0}, girth, height, {0.0, 1.0})
        {
            this->id = mh_random(0,MAX_ID);
            this->resourcesPath = resourcesPath;
            this->showAngleInfo = true;
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

        double get_scaled_girth() const override
        {
            return this->baseShape.get_scaled_width();
        }

        // Get the angle of its body orientation, which is head (the circle pin) to feet (the base of a support)
        // 
        // The returned angle will be in a screen orientation: up is negative and down is positive.
        // So 90 degree vector will points down. To convert it to "normal" orientation,
        // simply negate it with negative, aka. multiplied by -1.
        double get_angle() const override
        {
            vector_2d dir = this->baseShape.get_body_axis_offset(1);
            return ANGLE(vector_angle(dir));
        }

        const SupportData& read_properties() const override
        {
            return this->properties;
        }

        SupportData& get_properties() override
        {
            return this->properties;
        }

        vector_2d get_mid_pos() const override
        {
            return this->baseShape.get_head_pos();
        }

        vector_2d get_left_pos() const
        {
            return vector_add(this->baseShape.get_feet_pos(), 
                              this->baseShape.get_body_axis_offset(this->get_scaled_girth()/2, 90));
        }

        vector_2d get_right_pos() const
        {
            return vector_add(this->baseShape.get_feet_pos(), 
                              this->baseShape.get_body_axis_offset(this->get_scaled_girth()/2, -90));
        }

        bool get_show_info() const override
        {
            return this->showAngleInfo;
        }

        void set_scale(double newScale) override
        {
            this->baseShape.set_scale(newScale);
        }
        void set_show_info(bool info) override
        {
            this->showAngleInfo = info;
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

        void draw(bool showInfo = false, color supportColor = color_black()) override
        {
            bool showForce = (this->properties.verticalForce != 0.0) || this->properties.horizontalForce != 0.0;

            if(showInfo || showForce)
            {
                supportColor = color_red();
            }

            point_2d headPos = { this->baseShape.get_head_pos().x, this->baseShape.get_head_pos().y };
            double radius = this->get_scaled_girth()/2.0;

            draw_circle(supportColor, headPos.x, headPos.y, radius);
            draw_triangle(supportColor, headPos.x, headPos.y,
                                        this->get_left_pos().x, this->get_left_pos().y,
                                        this->get_right_pos().x, this->get_right_pos().y);

            if(strengthPage->get_do_check())
            {
                return;
            }

            if(this->properties.verticalForce != 0.0)
            {
                drawInfo(this->resourcesPath, this->read_properties().verticalForce, " N", supportColor, headPos.x, headPos.y, radius, 15, "y: ");
            }
            if(this->properties.horizontalForce != 0.0)
            {
                drawInfo(this->resourcesPath, this->read_properties().horizontalForce, " N", supportColor, this->get_left_pos().x, this->get_left_pos().y, radius, 15, "x: ");
            }
            if((showInfo && this->showAngleInfo) && !showForce)
            {
                drawInfo(this->resourcesPath, ANGLE(-this->get_angle()), "°", supportColor, headPos.x, headPos.y, radius);
            }
        }
};

class PinJointModel : public PinBaseModel
{
    public:
        PinJointModel(double initScale=5.0, std::string resourcesPath="") 
            : PinBaseModel(initScale, DEFAULT_PIN_GIRTH*1.1, DEFAULT_PIN_GIRTH, true, true, resourcesPath) {}

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
            this->midWheelPos = vector_add(this->baseShape.get_feet_pos(),
                                           this->baseShape.get_body_axis_offset(this->scaledWheelRadius));
        }

        void update_left_wheel_pos()
        {
            vector_2d offset = vector_add(this->baseShape.get_body_axis_offset(this->scaledWheelRadius),
                                        this->baseShape.get_body_axis_offset(this->scaledWheelRadius*2,90));
            this->leftWheelPos = vector_add(this->baseShape.get_feet_pos(), offset);
        }

        void update_right_wheel_pos()
        {
            vector_2d offset = vector_add(this->baseShape.get_body_axis_offset(this->scaledWheelRadius),
                                       this->baseShape.get_body_axis_offset(this->scaledWheelRadius*2,-90));
            this->rightWheelPos = vector_add(this->baseShape.get_feet_pos(), offset);
        }

        void update()
        {
            this->update_mid_wheel_pos();
            this->update_left_wheel_pos();
            this->update_right_wheel_pos();
        }

    public:
        Roller(double initScale=5.0, std::string resourcesPath="") 
            : PinBaseModel(initScale, DEFAULT_PIN_GIRTH*23.0/30.0, DEFAULT_PIN_GIRTH, true, false, resourcesPath)
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

        void draw(bool showInfo = false, color supportColor = color_black())
        {
            PinBaseModel::draw(showInfo, supportColor);
            if(showInfo) { supportColor = color_red(); }
            draw_circle(supportColor, this->midWheelPos.x, this->midWheelPos.y, this->scaledWheelRadius);
            draw_circle(supportColor, this->leftWheelPos.x, this->leftWheelPos.y, this->scaledWheelRadius);
            draw_circle(supportColor, this->rightWheelPos.x, this->rightWheelPos.y, this->scaledWheelRadius);
        }
};


#endif