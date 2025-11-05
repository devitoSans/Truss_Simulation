#ifndef __CROSS_SECTION__
#define __CROSS_SECTION__

#include <cmath>
#include <stdexcept>
#include <string>
#include <splashkit.h>
#include <vector>
#include "../mathHelpers/mathHelpers.hpp"

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
        // required for deleting in polymorphism
        virtual ~CrossSection() = default; 
        CrossSection& operator=(const CrossSection&) = default;
        CrossSection(const CrossSection&) = default;

        CrossSection() = default;

        virtual CrossSection* clone() const = 0;

        virtual double get_area() = 0;

        // 
        virtual double get_girth() const = 0;

        // Get I_minimum, that is Ixx if it is smaller than Iyy or vice versa.
        virtual double get_I() = 0;

        virtual cs_type::cross_section_type get_type() const = 0;

        virtual std::string get_name() const = 0;

        virtual void change_property(const std::string& name_property, double value) = 0;

        virtual std::vector<std::string> get_properties_name() const = 0;
        virtual std::vector<double> get_properties_values() const = 0;

        virtual void draw(const std::string& resourcesPath) const = 0;
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

        RectangleWithCircleCut* clone() const override
        {
            return new RectangleWithCircleCut(*this);
        }

        double get_area() override 
        {
            return this->width*this->height - PI * pow(this->get_circle_cut_diameter()/2, 2);
        }

        double get_girth() const override
        {
            return this->height;
        }

        double get_I() override 
        {
            double Ixx = (this->width*pow(this->height,3))/12 - (PI * pow(this->get_circle_cut_diameter()/2, 4))/4;
            double Iyy = (this->height*pow(this->width,3))/12 - (PI * pow(this->get_circle_cut_diameter()/2, 4))/4;
            return std::min(Ixx, Iyy);
        }

        cs_type::cross_section_type get_type() const override
        {
            return cs_type::RECTANGLE_WITH_CIRCLE_OUT;
        }

        std::string get_name() const override
        {
            return "Rectangular shape with circle cut out";
        }

        void change_property(const std::string& name_property, double value) override
        {
            if(name_property == "a")
            {
                this->set_width(value);
            }
            else if(name_property == "t")
            {
                this->set_thickness(value);
            }
            else if(name_property == "b")
            {
                this->set_height(value);
            }
        }

        std::vector<std::string> get_properties_name() const override
        {
            return { "a", "b", "t" };
        }

        std::vector<double> get_properties_values() const override
        {
            return { this->width, this->height, this->thickness };
        }

        void draw(const std::string& resourcesPath) const override
        {
            start_inset("img-area", 200);
            bitmap bmp = load_bitmap(this->get_name(), resourcesPath + "cross_section/"+this->get_name()+".png");
            disable_interface();
            set_layout_height(200);
            (void)bitmap_button(bmp);
            enable_interface();
            end_inset("img-area");
            set_layout_height(0);
        }
};

inline RectangleWithCircleCut rectangleWithCircleCut = RectangleWithCircleCut();

inline std::vector<CrossSection*> crossSectionOpts = {
    &rectangleWithCircleCut
};

#endif // _CROSS_SECTION__ 