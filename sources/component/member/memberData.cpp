#include "MemberData.hpp"

MemberData::MemberData(double length,
                       cs_type::cross_section_type type, 
                       Materials* material)
{
    switch(type)
    {
        case cs_type::RECTANGLE_WITH_CIRCLE_OUT:
        this->crossSection = new RectangleWithCircleCut();
        break;
        
        default:
        log(WARNING, "Undefined cross section area type. Defaulting to rectangle with a circle cut out.");
        this->crossSection = new RectangleWithCircleCut();
    }
    
    this->material = material;
    this->id = (unsigned)mh_random(0, 2147483630);
    this->set_length(length);
}

MemberData::MemberData(const MemberData& obj)
{
    this->id = obj.id;

    this->length = obj.length;
    this->crossSection = obj.crossSection->clone();
    this->material = obj.material->clone();

    this->axialForce = obj.axialForce;
}

MemberData& MemberData::operator=(MemberData obj)
{
    this->swap(*this, obj);
    return *this;
}

MemberData::~MemberData()
{
    delete this->crossSection;
    delete this->material;
}

void MemberData::swap(MemberData& obj1, MemberData& obj2)
{
    std::swap(obj1.id, obj2.id);

    std::swap(obj1.length, obj2.length);
    std::swap(obj1.crossSection, obj2.crossSection);
    std::swap(obj1.material, obj2.material);

    std::swap(obj1.axialForce, obj2.axialForce);
    // std::swap(obj1.start, obj2.start);
    // std::swap(obj1.end, obj2.end);
}

unsigned int MemberData::get_id() const
{
    return this->id;
}

double MemberData::get_length() const
{
    return this->length;
}

CrossSection* MemberData::cross_section()
{
    return this->crossSection;
}

const Materials* MemberData::get_material() const
{
    return this->material;
}

bool MemberData::set_length(double newLength)
{
    if(newLength < this->crossSection->get_girth())
    {
        this->length = this->crossSection->get_girth()+0.01;
        return false;
    }

    this->length = newLength;
    return true;
}

void MemberData::set_axial_force(double axialForce)
{
    this->axialForce = axialForce;
}

bool MemberData::is_yielded()
{
    double maxForce = this->crossSection->get_area();
    if(this->axialForce > 0)
    {
        maxForce *= this->material->get_tensile_strength();
    }
    else 
    {
        maxForce *= this->material->get_compressive_strength();
    }
    return maxForce < abs(this->axialForce);
}

bool MemberData::is_buckled()
{
    if(this->axialForce < 0)
    {
        double I = this->crossSection->get_I();
        double Leff = this->length;
        double E = this->material->get_young_modulus();

        double maxForce = (pow(PI,2)*E*I) / pow(Leff,2);
        
        return maxForce < -this->axialForce;
    }
    return false;
}
