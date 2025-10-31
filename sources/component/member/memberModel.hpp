#ifndef __MEMBERS__
#define __MEMBERS__

#include <cmath>

#include "../../materials/materials.hpp"
#include "../../materials/cross_section.hpp"
#include "../../mathHelpers/mathHelpers.hpp"

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

        int jointIDAtStart;
        int jointIDAtEnd;

    public:
        MemberData(double length=20,
                   cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                   Materials* material=new Acrylic());

        MemberData(const MemberData& obj);
        MemberData& operator=(MemberData obj);
        ~MemberData();

        void swap(MemberData& obj1, MemberData& obj2);

        unsigned int get_id() const;
        double get_length() const;
        const Materials* get_material() const;
        int get_joint_id_at_start() const;
        int get_joint_id_at_end() const;
        double get_axial_force() const;
        // Get view-only cross section
        const CrossSection* get_cross_section() const;
        
        // Get the editable cross section
        CrossSection* cross_section();
        
        // Avoid calling this beside inside MemberInterface class.
        // Unless you know what are you doing.
        // True if sucessfully change the length, otherwise false.
        // length cannot be less than girth.
        bool set_length(double newLength);
        void set_axial_force(double axialForce);
        void set_joint_id_at_start(int id);
        void set_joint_id_at_end(int id);

        bool is_yielded();
        bool is_buckled();
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
        MemberModel(double length=20,
                    double scale=20,
                    cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                    Materials* material=new Acrylic());

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

        void draw(color memberColor = color_black()) const;
};

#endif
