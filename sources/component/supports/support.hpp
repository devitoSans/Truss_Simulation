#ifndef __SUPPORT__
#define __SUPPORT__

class Support
{
    virtual double get_vertical_angle() const = 0;
    virtual void set_vertical_force(double) = 0;

    virtual double get_horizontal_angle() const = 0;
    virtual void set_horizontal_force(double) = 0;
};

class PinJoint : public Support
{
    private:
        double verticalForce; // N (upwards is positive)
        double horizontalForce; // N (upwards is positive)

        double verticalAngle; // degree
        double horizontalAngle; // degree

    public:
        PinJoint()
        {
            this->verticalForce = 0;
            this->horizontalForce = 0;
            this->verticalAngle = 90;
            this->horizontalAngle = 0;
        }

        double get_vertical_angle() const override
        {
            return this->verticalAngle;
        }

        double get_horizontal_angle() const override
        {
            // return this->ver
        }

        void set_vertical_force(double newForce) override
        {
            this->verticalForce = newForce;
        }

        void set_horizontal_force(double newForce) override
        {
            this->horizontalForce = newForce;
        }


};

#endif