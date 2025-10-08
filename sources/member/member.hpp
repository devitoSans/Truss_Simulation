#ifndef __MEMBERS__
#define __MEMBERS__

#include <splashkit.h>
#include "../materials/materials.hpp"
#include "../materials/cross_section.hpp"

class Member
{
    private:
        double length;
        double angle;
        CrossSection* crossSection;
        Materials* material;

    public:
        Member();
        ~Member()
        {
            delete this->crossSection;
            delete this->material;
        }
};

#endif
