#ifndef __SHAPE_BASE__
#define __SHAPE_BASE__

#include <splashkit.h>

class OneSideShapeBase
{
    private:
        double scale;
        double scaledWidth;
        double scaledLength;
        vector_2d headPos;
        vector_2d feetPos;

        void set_scaled_length(double newLength)
        {
            this->scaledLength = newLength * this->scale;
        }

        void set_scaled_width(double newWidth)
        {
            this->scaledWidth = newWidth * this->scale;
        }

        void update_feet_pos(vector_2d& direction)
        {
            this->feetPos = vector_add(this->headPos, vector_from_angle(vector_angle(direction), this->scaledLength));
        }

    public:
        OneSideShapeBase(double initScale, 
                         vector_2d initHeadPos, 
                         double width, 
                         double length, 
                         vector_2d direction={1.0, 0.0}) 
        {
            this->scale = initScale;
            this->headPos = initHeadPos;

            this->set_scaled_width(width);
            this->set_scaled_length(length);
            this->update_feet_pos(direction);
        }

        double get_scale() const
        {
            return this->scale;
        }

        double get_scaled_length() const
        {
            return this->scaledLength;
        }

        double get_scaled_width() const
        {
            return this->scaledWidth;
        }

        const vector_2d& get_head_pos() const
        {
            return this->headPos;
        }

        const vector_2d& get_feet_pos() const
        {
            return this->feetPos;
        }

        /// Returns a 2D vector of length `offset` along the head -> feet axis,
        /// rotated by `degree`.
        vector_2d get_body_axis_offset(double offset, double degree=0.0) const
        {
            vector_2d direction = vector_subtract(this->get_feet_pos(), this->get_head_pos());
            double angle = vector_angle(direction);
            return vector_from_angle(angle+degree, offset);
        }

        void set_scale(double newScale)
        {
            double initLength = this->get_scaled_length() / this->scale;
            double initWidth = this->get_scaled_width() / this->scale;

            this->scale = newScale;

            this->set_scaled_length(initLength);
            this->set_scaled_width(initWidth);
        }

        void move(double x, double y)
        {
            vector_2d direction = vector_subtract({x,y}, this->headPos);
            this->headPos = matrix_multiply(translation_matrix(direction), this->headPos);
            this->feetPos = matrix_multiply(translation_matrix(direction), this->feetPos);
        }

        void rotate(double degree)
        {
            double angle = std::fmod(degree, 360.0);
            angle += (angle < 0.0 ? 360.0 : 0);

            vector_2d direction = vector_subtract(this->feetPos, this->headPos);
            double currentAngle = vector_angle(direction);
            direction = matrix_multiply(rotation_matrix(angle - currentAngle), direction);
            this->feetPos = vector_add(this->headPos, direction);
        }
};

#endif