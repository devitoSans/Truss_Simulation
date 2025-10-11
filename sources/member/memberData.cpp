#include "MemberData.hpp"

MemberData::MemberData(double length,
                       cs_type::cross_section_type type, 
                       Materials* material)
{
    this->set_length(length);

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
}

MemberData::MemberData(const MemberData& obj)
{
    this->length = obj.length;
    this->crossSection = obj.crossSection->clone();
    this->material = obj.material->clone();
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
    std::swap(obj1.crossSection, obj2.crossSection);
    std::swap(obj1.material, obj2.material);
    std::swap(obj1.length, obj2.length);
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

void MemberData::set_length(double newLength)
{
    this->length = newLength;
}

// void MemberData::draw(double midX, double midY, double scale, color MemberDataColor)
// {
//     double scaledLength = this->length * scale;
//     double scaledGirth = this->crossSection->get_girth() * scale;

//     double rad_angle = mh::to_rad(this->angle);
//     double rad_counter_angle = mh::to_rad(this->counter_angle);

//     // coordinates for circle on the left (if angle = 0)
//     double xc1 = midX - scaledLength/2 * std::cos(rad_angle);
//     double yc1 = midY + scaledLength/2 * std::sin(rad_angle);
    
//     // coordinates for circle on the right (if angle = 0)
//     double xc2 = midX - scaledLength/2 * std::cos(rad_counter_angle);
//     double yc2 = midY + scaledLength/2 * std::sin(rad_counter_angle);

//     // coordinates for top line on the left point (if angle = 0)
//     double xltop_pl = xc1 + scaledGirth/2 * std::cos(rad_angle+mh::to_rad(90));
//     double yltop_pl = yc1 - scaledGirth/2 * std::sin(rad_angle+mh::to_rad(90));

//     // coordinates for top line on the right point (if angle = 0)
//     double xltop_pr = xc2 + scaledGirth/2 * std::cos(rad_counter_angle+mh::to_rad(90));
//     double yltop_pr = yc2 + scaledGirth/2 * std::sin(rad_counter_angle+mh::to_rad(90));

//     // coordinates for bottom line on the left point (if angle = 0)
//     double xlbot_pl = xc1 + scaledGirth/2 * std::cos(rad_angle-mh::to_rad(90));
//     double ylbot_pl = yc1 - scaledGirth/2 * std::sin(rad_angle-mh::to_rad(90));

//     // coordinates for bottom line on the right point (if angle = 0)
//     double xlbot_pr = xc2 + scaledGirth/2 * std::cos(rad_counter_angle-mh::to_rad(90));
//     double ylbot_pr = yc2 + scaledGirth/2 * std::sin(rad_counter_angle-mh::to_rad(90));

//     draw_circle(MemberDataColor, xc1,yc1, scaledGirth/2);
//     draw_circle(MemberDataColor, xc2,yc2, scaledGirth/2);

//     fill_circle(color_red(), midX, midY, 5);

//     // Top line
//     draw_line(MemberDataColor, xltop_pl, yltop_pl, 
//                            xltop_pr, yltop_pr);

//     // Bottom line
//     draw_line(MemberDataColor, xlbot_pl, ylbot_pl, 
//                            xlbot_pr, ylbot_pr);
// }