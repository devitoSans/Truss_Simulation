#ifndef __MEMBER_INTERFACE__
#define __MEMBER_INTERFACE__

#include "memberData.hpp"

class MemberInterface
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

        bool set_scaled_length(double newLength)
        {
            bool status = this->properties.set_length(newLength);
            this->scaledLength = this->properties.get_length() * this->scale;
            return status;
        }

        void update_mid_pos()
        {
            vector_2d middle = vector_subtract(this->endPos, this->startPos);
            middle = { middle.x/2, middle.y/2 };
            this->midPos = vector_add(this->startPos, middle);
        }

    public:
        MemberInterface(double length=20,
                        double scale=20,
                        cs_type::cross_section_type type=cs_type::RECTANGLE_WITH_CIRCLE_OUT, 
                        Materials* material=new Acrylic())
        {
            this->properties = MemberData(length, type, material);
            this->scale = scale;

            this->startPos = { 0.0, 0.0 };
            this->midPos = { 0.0, 0.0 };
            this->endPos = { 0.0, 0.0 };;

            this->set_angle(0);
            this->modify_length(length);
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

        double get_scale() const
        {
            return this->scale;
        }

        void set_scale(double newScale)
        {
            this->scale = newScale;
            this->modify_length(this->properties.get_length());
        }

        // Modifying length that changes both start's and end's pos.
        void modify_length(double newLength)
        {
            this->set_scaled_length(newLength);

            double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
            double deltaLength = this->scaledLength-oldLength;

            vector_2d direction = vector_from_angle(this->angle, 1);

            // change start's position
            vector_2d toEndDirection = { -direction.x*deltaLength/2, -direction.y*deltaLength/2 };
            this->startPos = vector_add(this->startPos, toEndDirection);
            
            // change end's position
            vector_2d toStartDirection = { direction.x*deltaLength/2, direction.y*deltaLength/2 };
            this->endPos = vector_add(this->endPos, toStartDirection);
        }

        // Modifying length that only changes end point. Start's position remains the same
        void modify_length_on_start(double newLength)
        {
            this->set_scaled_length(newLength);
            
            double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
            double deltaLength = this->scaledLength-oldLength;
            
            vector_2d direction = vector_from_angle(this->angle, 1);
            direction = { direction.x*deltaLength, direction.y*deltaLength };
            this->endPos = vector_add(this->endPos, direction);
        }
        
        // Modifying length that only changes start point. End's position remains the same
        void modify_length_on_end(double newLength)
        {
            this->set_scaled_length(newLength);

            double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
            double deltaLength = this->scaledLength-oldLength;

            vector_2d direction = vector_from_angle(this->angle, 1);
            direction = { -direction.x*deltaLength, -direction.y*deltaLength };
            this->startPos = vector_add(this->startPos, direction);
        }

        void modify_start_pos(double x, double y)
        {
            this->startPos = { x,y };
            vector_2d distance = vector_subtract(this->startPos, this->endPos);

            // change on the end side
            this->set_counter_angle(vector_angle(distance)); 
            this->set_scaled_length(vector_magnitude(distance) / this->scale);
            this->modify_length_on_end(this->properties.get_length());

            this->update_mid_pos();
        }
        
        void modify_end_pos(double x, double y)
        {
            this->endPos = { x,y };
            vector_2d distance = vector_subtract(this->endPos, this->startPos);
            
            // change on the start side
            this->set_angle(vector_angle(distance)); 
            this->set_scaled_length(vector_magnitude(distance) / this->scale);
            this->modify_length_on_start(this->properties.get_length());

            this->update_mid_pos();
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

        // detect point on start
        bool is_intersect_start(double x, double y)
        {
            point_2d point = { x,y };

            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;
            double radius = scaledGirth/2;
            circle startCircle = { this->startPos.x, this->startPos.y, radius };

            return point_in_circle(point, startCircle);
        }
        
        // detect point on end
        bool is_intersect_end(double x, double y)
        {
            point_2d point = { x,y };

            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;
            double radius = scaledGirth/2;
            circle endCircle = { this->endPos.x, this->endPos.y, radius };

            return point_in_circle(point, endCircle);
        }
        
        // detect circle on start
        bool is_intersect_start(double x, double y, double radius)
        {
            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;
            return circles_intersect(
                x,y,
                radius,

                this->startPos.x,this->startPos.y,
                scaledGirth/2
            );
        }
        
        // detect circle on end
        bool is_intersect_end(double x, double y, double radius)
        {
            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;
            return circles_intersect(
                x,y,
                radius,

                this->endPos.x,this->endPos.y,
                scaledGirth/2
            );
        }

        // Add intersection detection on body
        bool is_intersect_body(double x, double y)
        {
            // TODO: might need to somehow rewrite this to speed it up. i.e. save this on some variables or something
            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;
            vector_2d line = vector_subtract(this->endPos, this->startPos);
            vector_2d direction = { line.x/this->scaledLength, line.y/this->scaledLength };

            vector_2d topDirection = matrix_multiply(rotation_matrix(-90), direction);
            topDirection = { topDirection.x * scaledGirth/2, topDirection.y * scaledGirth/2 };

            vector_2d left_corner_top = vector_add(topDirection, this->startPos);
            vector_2d left_corner_bot = vector_add({-topDirection.x, -topDirection.y}, this->startPos);

            vector_2d right_corner_top = vector_add(left_corner_top, line);
            vector_2d right_corner_bot = vector_add(left_corner_bot, line);

            quad q;
            q.points[0] = {left_corner_top.x, left_corner_top.y};
            q.points[1] = {right_corner_top.x, right_corner_top.y};
            q.points[2] = {left_corner_bot.x, left_corner_bot.y};
            q.points[3] = {right_corner_bot.x, right_corner_bot.y};

            return point_in_quad({x,y}, q);
        }

        void draw(color memberColor=color_black())
        {
            double scaledGirth = this->properties.cross_section()->get_girth() * this->scale;

            // Start's circle
            draw_circle(memberColor, this->startPos.x, this->startPos.y, scaledGirth/2);
            
            // End's circle
            draw_circle(memberColor, this->endPos.x, this->endPos.y, scaledGirth/2);

            vector_2d line = vector_subtract(this->endPos, this->startPos);
            vector_2d direction = { line.x/this->scaledLength, line.y/this->scaledLength };

            // printf("start.x: %f, start.y: %f\n", this->startPos.x, this->startPos.y);
            // printf("end.x: %f, end.y: %f\n", this->endPos.x, this->endPos.y);

            // printf("line.x: %f, line.y: %f\n", line.x, line.y);
            // printf("line: %f\n", vector_angle(line));

            // printf("direction: %f\n", vector_angle(direction));

            // -90 since the positive y screen is going down
            vector_2d topDirection = matrix_multiply(rotation_matrix(-90), direction);
            topDirection = { topDirection.x * scaledGirth/2, topDirection.y * scaledGirth/2 };

            vector_2d botDirection = matrix_multiply(rotation_matrix(90), direction);
            botDirection = { botDirection.x * scaledGirth/2, botDirection.y * scaledGirth/2 };

            // Top's line
            vector_2d topStart = vector_add(topDirection, this->startPos);
            vector_2d topEnd = vector_add(topStart, line);
            draw_line(memberColor, topStart.x, topStart.y, topEnd.x, topEnd.y);

            // Bottom's line
            vector_2d botStart = vector_add(botDirection, this->startPos);
            vector_2d botEnd = vector_add(botStart, line);
            draw_line(memberColor, botStart.x, botStart.y, botEnd.x, botEnd.y);
        }
};

#endif