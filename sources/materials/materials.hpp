#ifndef __MATERIALS__
#define __MATERIALS__

#include <string>
#include <vector>

class Materials
{
    private:
        double ts; // MPa
        double E; // MPa
        double cs; // MPa
        std::string name;

    public:
        Materials(double tensileStrength, double youngModulus, double compressionStrength, std::string name)
            : ts(tensileStrength), E(youngModulus), cs(compressionStrength), name(name)
        {}

        double get_tensile_strength() const
        {
            return this->ts;
        }

        double get_young_modulus() const
        {
            return this->E;
        }

        double get_compressive_strength() const
        {
            return this->cs;
        }

        const std::string& get_name() const
        {
            return this->name;
        }
};

inline Materials acrylic = Materials(66, 3600, 98.2, "Acrylic");
inline Materials pla45 = Materials(38, 2200, 38, "PLA 45° infill");

inline std::vector<Materials*> materialsOption = {
    &acrylic,
    &pla45
};

#endif // __MATERIALS__