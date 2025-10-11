#ifndef __MEMBER_INTERFACE__
#define __MEMBER_INTERFACE__

#include "memberData.hpp"

class MemberInterface
{
    private:
        MemberData properties;

        // coordinates at the left of the member (if angle = 0)
        vector_2d startPos = { 0.0, 0.0 };
        // coordinates at the right of the member (if angle = 0)
        vector_2d endPos = { 0.0, 0.0 };
        // coordinates at the middle of the member
        vector_2d midPos = { 0.0, 0.0 };

        // angle at the start of the member (if angle = 0)
        double angle; // degree
        // angle at the end of the member (if angle = 0)
        double counter_angle; // degree

        // How big the 1mm is on the screen
        double scale;

        void set_angle(double newAngle)
        {
            this->angle = std::fmod(newAngle, 360.0);
            this->angle += (this->angle < 0.0 ? 360.0 : 0);
            
            this->counter_angle = std::fmod(180 + this->angle, 360.0);
        }
        
        void set_counter_angle(double newAngle)
        {
            this->counter_angle = std::fmod(newAngle, 360.0);
            this->counter_angle += (this->counter_angle < 0.0 ? 360.0 : 0);

            this->angle = std::fmod(180 + this->counter_angle, 360.0);
        }

        void update_mid_pos()
        {
            vector_2d middle = vector_subtract(this->endPos, this->startPos);
            middle = { middle.x/2, middle.y/2 };
            this->midPos = vector_add(this->startPos, middle);
        }

    public:
        MemberInterface(double length=20,
                        double angle=0,
                        cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                        Materials* material=new Acrylic(),
                        double scale=20)
        {
            this->properties = MemberData(0, type, material);
            this->scale = scale;

            this->modify_length(length);
            this->modify_angle(angle);
        }

        double get_angle() const
        {
            return this->angle;
        }

        double get_counter_angle() const
        {
            return this->counter_angle;
        }

        vector_2d get_start_pos() const
        {
            return this->startPos;
        }

        vector_2d get_mid_pos() const
        {
            return this->midPos;
        }

        vector_2d get_end_pos() const
        {
            return this->endPos;
        }

        MemberData& get_properties()
        {
            return this->properties;
        }

        void modify_length(double newLength)
        {
            this->properties.set_length(newLength);
            double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));

            double deltaLength = newLength-oldLength;
            vector_2d direction = vector_from_angle(this->angle, 1);

            // change start's position
            vector_2d toEndDirection = { -direction.x*deltaLength/2, -direction.y*deltaLength/2 };
            this->startPos = vector_add(this->startPos, toEndDirection);
            
            // change end's position
            vector_2d toStartDirection = { direction.x*deltaLength/2, direction.y*deltaLength/2 };
            this->endPos = vector_add(this->endPos, toStartDirection);
        }

        void modify_start_pos(double x, double y)
        {
            this->startPos = vector_2d { x,y };
            this->update_mid_pos();
            
            vector_2d distance = vector_subtract(this->startPos, this->endPos);
            
            this->set_counter_angle(vector_angle(distance)); 
            this->properties.set_length(vector_magnitude(distance));
        }
        
        void modify_end_pos(double x, double y)
        {
            this->endPos = vector_2d { x,y };
            this->update_mid_pos();

            vector_2d distance = vector_subtract(this->endPos, this->startPos);

            this->set_angle(vector_angle(distance)); 
            this->properties.set_length(vector_magnitude(distance));
        }

        void modify_mid_pos(double x, double y)
        {
            vector_2d destination = vector_2d { x,y };
            vector_2d direction = vector_subtract(destination, this->midPos);

            this->startPos = vector_add(direction, this->startPos);
            this->midPos = vector_add(direction, this->midPos);
            this->endPos = vector_add(direction, this->endPos);
        }

        void modify_angle(double angle)
        {
            this->set_angle(angle);

            vector_2d direction = vector_subtract(this->endPos, this->startPos);
            double currentAngle = vector_angle(direction);
            vector_2d newDirection = matrix_multiply(rotation_matrix(this->angle-currentAngle), direction);

            this->endPos = vector_add(newDirection, this->startPos);
            this->update_mid_pos();
        }
        
        void modify_counter_angle(double angle)
        {
            this->set_counter_angle(angle);

            vector_2d direction = vector_subtract(this->startPos, this->endPos);
            double currentAngle = vector_angle(direction);
            vector_2d newDirection = matrix_multiply(rotation_matrix(this->counter_angle-currentAngle), direction);

            this->startPos = vector_add(newDirection, this->endPos);
            this->update_mid_pos();
        }

        void draw()
        {

        }
};

#endif