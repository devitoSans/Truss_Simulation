#ifndef __CROSS_SECTION__
#define __CROSS_SECTION__

#include <cmath>
#include <stdexcept>
#include <string>
#include <splashkit.h>

#define PI 3.14159265358979311600

namespace cs_type
{
    enum cross_section_type
    {
        RECTANGLE_WITH_CIRCLE_OUT
    };
};

class CrossSection
{
    public:
        virtual double get_area() = 0;

        // Get I_minimum, that is Ixx if it is smaller than Iyy or vice versa.
        virtual double get_I() = 0;

        // scale will define how long/short 1 mm is on the screen.
        // mid_x and mid_y will located at the centre of the cross section.
        virtual void draw(double mid_x, double mid_y, double scale) = 0;

        virtual cs_type::cross_section_type get_type() const = 0;
};

class RectangleWithCircleCut : public CrossSection
{
    private:
        double height; // mm
        double width; // mm
        double thickness; // mm
        color section_color;
        color background_color;

    public:
        RectangleWithCircleCut(double height=4, double width=4, double thickness=0.5, 
                               color section=color_black(), color background=color_white())
        {
            this->width = width;

            if(!this->set_thickness(thickness))
            {
                throw std::invalid_argument("Thickness (" + std::to_string(thickness) + ") cannot be greater than width (" + std::to_string(this->width) + ")");
            }
            
            if(!this->set_height(height))
            {
                throw std::invalid_argument("Height (" + std::to_string(thickness) + ") cannot be less than or equal to the circle cut out's diameter (" + std::to_string(this->get_circle_cut_diameter()) + ")");
            }

            this->section_color = section;
            this->background_color = background;
        }

        double get_circle_cut_diameter()
        {
            return this->width - 2*this->thickness;
        }

        bool set_width(double newWidth)
        {
            if(this->get_circle_cut_diameter() >= newWidth)
            {
                return false;
            }
            this->width = newWidth;
            return true;
        }

        bool set_thickness(double newThickness)
        {
            double newCircleCutDiameter = this->width - 2*newThickness;
            if(newCircleCutDiameter < 0)
            {
                return false;
            }
            this->thickness = newThickness;
            return true;
        }

        bool set_height(double newHeight)
        {
            if(this->get_circle_cut_diameter() >= newHeight)
            {
                return false;
            }
            this->height = newHeight;
            return true;
        }

        double get_area() override 
        {
            return this->width*this->height - PI * pow(this->get_circle_cut_diameter()/2, 2);
        }

        double get_I() override 
        {
            double Ixx = (this->width*pow(this->height,3))/12 - (PI * pow(this->get_circle_cut_diameter()/2, 4))/4;
            double Iyy = (this->height*pow(this->width,3))/12 - (PI * pow(this->get_circle_cut_diameter()/2, 4))/4;
            return std::min(Ixx, Iyy);
        }

        void draw(double mid_x, double mid_y, double scale) override
        {
            double scaled_width = this->width*scale;
            double scaled_height = this->height*scale;
            double scaled_radius = this->get_circle_cut_diameter()/2 * scale;

            double x = (2*mid_x - scaled_width)/2;
            double y = (2*mid_y - scaled_height)/2;

            
            fill_rectangle(this->section_color, x,y, scaled_width, scaled_height);
            // double centre_x = x + scaled_width/2;
            // double centre_y = y + scaled_height/2;
            // fill_circle(this->background_color, centre_x,centre_y, scaled_radius);
            fill_circle(this->background_color, mid_x,mid_y, scaled_radius);
        }

        cs_type::cross_section_type get_type() const override
        {
            return cs_type::RECTANGLE_WITH_CIRCLE_OUT;
        }
};



#endif // _CROSS_SECTION__ 