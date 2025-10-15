#ifndef __MATERIALS__
#define __MATERIALS__

// TODO: Might need to do something on this. Since this whole thing is a constant

class Materials
{
    public: // C++ default's scope is private
        // required for deleting in polymorphism
        virtual ~Materials() = default; 
        Materials& operator=(const Materials&) = default;
        Materials(const Materials&) = default;

        Materials() = default;

        virtual Materials* clone() const = 0;

        virtual double get_tensile_strength() const = 0;
        virtual double get_young_modulus() const = 0;
        virtual double get_compressive_strength() const = 0;
};

class Acrylic : public Materials
{
    private:
        double ts;
        double E;
        double cs;

    public:
        Acrylic()
        {
            this->ts = 66; // Limit for tension force (UTS) (MPa)
            this->E = 3600; // Young's Modulus (MPa)
            this->cs = 98.2; // Limit for compression force (MPa)
        }

        Acrylic* clone() const override
        {
            return new Acrylic(*this);
        }

        double get_tensile_strength() const override
        {
            return this->ts;
        }

        double get_young_modulus() const override
        {
            return this->E;
        }

        double get_compressive_strength() const override
        {
            return this->cs;
        }
};

#endif // __MATERIALS__