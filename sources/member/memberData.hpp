#ifndef __MEMBERS__
#define __MEMBERS__

#include <cmath>

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
        unsigned int id;

        double length; // mm
        CrossSection* crossSection;
        Materials* material;

        double axialForce; // N (negative -> compression, positive -> tension)

        // std::vector<MemberData*> start;
        // std::vector<MemberData*> end;

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

        CrossSection* cross_section();
        
        // Avoid calling this beside inside MemberInterface class.
        // Unless you know what are you doing.
        // True if sucessfully change the length, otherwise false.
        // length cannot be less than girth.
        bool set_length(double newLength);
        void set_axial_force(double axialForce);

        bool is_yielded();
        bool is_buckled();

        // // Join start
        // void join_on_start(MemberData* member)
        // {
        //     this->start.push_back(member);
        // }
        // // Join end
        // void join_on_end(MemberData* member)
        // {
        //     this->end.push_back(member);
        // }

        // // Detach on start
        // void detach_from_start()
        // {
        //     for(auto member : this->start)
        //     {
        //         member->remove_member_from_start(member)
        //     }
        // }
        // // Detach on end


        // bool remove_member_from_start(unsigned int id)
        // {
        //     int offset = -1;
        //     for(int i = 0; i < this->start.size(); i++)
        //     {
        //         if(this->start[i]->id == id)
        //         {
        //             offset = i;
        //         }
        //     }
        //     if(offset == -1) 
        //     {
        //         return false;
        //     }
        //     this->start.erase(this->start.begin()+offset);
        //     return true;
        // }

        // bool remove_member_from_end(unsigned int id)
        // {
        //     int offset = -1;
        //     for(int i = 0; i < this->end.size(); i++)
        //     {
        //         if(this->end[i]->id == id)
        //         {
        //             offset = i;
        //         }
        //     }
        //     if(offset == -1) 
        //     {
        //         return false;
        //     }
        //     this->end.erase(this->end.begin()+offset);
        //     return true;
        // }
};

#endif
