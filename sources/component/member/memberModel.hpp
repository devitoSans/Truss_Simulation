#ifndef __MEMBERS__
#define __MEMBERS__

#include <cmath>

#include "../../materials/materials.hpp"
#include "../../materials/cross_section.hpp"
#include "../../mathHelpers/mathHelpers.hpp"
#include "../text/text.hpp"
#include "../../sidebar/sidebar.hpp"

class MemberData
{
    private:
        // Note: If you want to add another field, remember 
        //       to add it on:
        //       1. swap method
        //       2. copy constructor
        unsigned int id;

        double length; // mm
        CrossSection* crossSection;
        Materials* material;

        double axialForce; // N (negative -> compression, positive -> tension)

    public:
        MemberData(double length=20,
                   cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                   Materials* material=&acrylic);

        MemberData(const MemberData& obj);
        MemberData& operator=(MemberData obj);
        ~MemberData();

        void swap(MemberData& obj1, MemberData& obj2);

        unsigned int get_id() const;
        double get_length() const;
        Materials* get_material();
        int get_joint_id_at_start() const;
        int get_joint_id_at_end() const;
        double get_axial_force() const;
        // Get view-only cross section
        const CrossSection* read_cross_section() const;
        
        // Get the editable cross section
        CrossSection* cross_section();
        
        void set_cross_section(CrossSection* crossSection);
        void set_material(Materials* material);
        // Avoid calling this beside inside MemberInterface class.
        // Unless you know what are you doing.
        // True if sucessfully change the length, otherwise false.
        // length cannot be less than girth.
        bool set_length(double newLength);
        void set_axial_force(double axialForce);

        bool is_yielded() const;
        bool is_buckled() const;
};

class MemberModel
{
    private:
        MemberData properties;

        // coordinates at the left of the member (if angle = 0)
        vector_2d startPos;
        // coordinates at the right of the member (if angle = 0)
        vector_2d endPos;
        // coordinates at the middle of the member
        vector_2d midPos;

        // angle at the start of the member (if angle = 0)
        double angle; // degree
        // angle at the end of the member (if angle = 0)
        double counter_angle; // degree

        // How big the 1mm is on the screen
        double scale;

        double scaledLength;
        
        void set_angle(double newAngle);
        void set_counter_angle(double newAngle);
        bool set_scaled_length(double newLength);
        void update_mid_pos();
        
    public:
        bool showLengthInfo;
        bool showAngleInfo;
        bool showCounterAngleInfo;
        std::string resourcesPath;

        MemberModel(double length=20,
                    double scale=20,
                    cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                    Materials* material=&acrylic,
                    std::string resourcesPath="");

        double get_angle() const;
        double get_counter_angle() const;
        vector_2d get_start_pos() const;
        vector_2d get_mid_pos() const;
        vector_2d get_end_pos() const;
        MemberData& get_properties();
        double get_scale() const;
        int get_id() const;
        double get_scaled_girth() const;

        void set_scale(double newScale);

        const MemberData& read_properties() const;

        void modify_length(double newLength);
        void modify_length_on_start(double newLength); // moves end only
        void modify_length_on_end(double newLength);   // moves start only

        void modify_start_pos(double x, double y);
        void modify_end_pos(double x, double y);
        void modify_mid_pos(double x, double y);

        void modify_angle(double angle);
        void modify_counter_angle(double angle);

        bool is_intersect_start(double x, double y) const;
        bool is_intersect_end(double x, double y) const;
        bool is_intersect_start(double x, double y, double radius) const; // circle vs start
        bool is_intersect_end(double x, double y, double radius) const;   // circle vs end
        bool is_intersect_body(double x, double y) const;

        void draw(bool showInfo = false, color memberColor = color_black()) const;
};

#include <iostream>
class MemberContent : InfoContent
{
    private:
        MemberModel* data;

        void show_materials()
        {
            if(start_treenode("Materials"))
            {
                Materials* currentMaterial = this->data->get_properties().get_material();
                label_element("Currently Using: " + 
                    currentMaterial->get_name()
                );
                for(auto material : materialsOption)
                {
                    if(start_treenode(material->get_name()))
                    {
                        label_element("Tensile Strength (MPa): " + std::to_string(material->get_tensile_strength()));
                        label_element("Young Modulus (MPa): " + std::to_string(material->get_young_modulus()));
                        label_element("Compression Strength (MPa): " + std::to_string(material->get_compressive_strength()));

                        std::string label = "    ";
                        if(currentMaterial == material)
                        {
                            label = "Used";
                        }
                        if(button(label, "use " + material->get_name()))
                        {
                            this->data->get_properties().set_material(material);
                        }

                        end_treenode(material->get_name());
                    }
                }
                end_treenode("Materials");
            }
        }

        void show_cross_sectional()
        {
            if(start_treenode("Cross Sectional Area"))
            {                
                CrossSection* currentCrossSectional = this->data->get_properties().cross_section();
                label_element("Currently Using: " + 
                    currentCrossSectional->get_name()
                );
                int index = 4;
                for(auto crossSection : crossSectionOpts)
                {
                    if(start_treenode(crossSection->get_name()))
                    {
                        std::string label = "    ";
                        if(currentCrossSectional->get_name() == crossSection->get_name())
                        {
                            label = "Used";
                        }

                        crossSection->draw(this->data->resourcesPath);
                        std::vector<std::string> properties = (label == "Used" ? currentCrossSectional : crossSection)->get_properties_name();
                        std::vector<double> values = (label == "Used" ? currentCrossSectional : crossSection)->get_properties_values();
                        // printf("get: %f\n", crossSection->get_girth());
                        bool change = false;
                        for(int i = 0; i < properties.size(); i++)
                        {
                            if(draw_properties(properties[i] + "            ", values[i], values[i], index))
                            {
                                crossSection->change_property(properties[i], values[i]);
                                if(label == "Used")
                                {
                                    change = true;
                                }
                            }
                            index++;
                        }

                        if(button(label, "use " + crossSection->get_name()) || change)
                        {
                            this->data->get_properties().set_cross_section(crossSection->clone());
                        }
                        end_treenode(crossSection->get_name());
                    }
                }
                
                end_treenode("Cross Sectional Area");
            }
        }
    
        public:
        MemberContent() : data(nullptr) {}

        void set_member_data(MemberModel* data)
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
            label_element("Component Type: Member");
            label_element("");
            
            // Length
            double newLength = this->data->read_properties().get_length();
            if(this->draw_properties("Length", newLength, newLength, 1))
            {
                this->data->modify_length(newLength);
            }
            bool& showLength = this->data->showLengthInfo;
            this->show_properties("Show length?", showLength, showLength);
            label_element("");

            // Angle
            double newAngle = ANGLE(-this->data->get_angle());
            if(this->draw_properties("Angle(b)", newAngle, newAngle, 2))
            {
                this->data->modify_angle(ANGLE(-newAngle));
            }
            bool& showAngle = this->data->showAngleInfo;
            this->show_properties("Show angle(b)?", showAngle, showAngle);
            label_element("");

            // Counter angle
            newAngle = ANGLE(-this->data->get_counter_angle());
            if(this->draw_properties("Angle(g)", newAngle, newAngle, 3))
            {
                this->data->modify_counter_angle(ANGLE(-newAngle));
            }
            bool& showCounterAngle = this->data->showCounterAngleInfo;
            this->show_properties("Show angle(g)?", showCounterAngle, showCounterAngle);
            label_element("");
            
            this->show_materials();
            this->show_cross_sectional();

            label_element("");
            label_element("");
        }
};
inline MemberContent memberContent = MemberContent();

#endif
