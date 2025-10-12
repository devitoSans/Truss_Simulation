#ifndef __MEMBERS__
#define __MEMBERS__

#include <cmath>

#include <splashkit.h>
#include "../materials/materials.hpp"
#include "../materials/cross_section.hpp"
#include "../mathHelpers/mathHelpers.hpp"

class MemberData
{
    private:
        // Note: If you want to add another field, remember 
        //       to add it on:
        //       1. swap method
        //       2. copy constructor
        double length; // mm
        CrossSection* crossSection;
        Materials* material;

    public:
        MemberData(double length=20,
                   cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                   Materials* material=new Acrylic());

        MemberData(const MemberData& obj);
        MemberData& operator=(MemberData obj);
        ~MemberData();

        void swap(MemberData& obj1, MemberData& obj2);

        double get_length() const;
        const Materials* get_material() const;

        CrossSection* cross_section();
        
        // Avoid calling this beside inside MemberInterface class.
        // Unless you know what are you doing.
        void set_length(double newLength);

        // Check Yield Strength
        // Check Buckling Strength
        // Axial force
};

#endif
