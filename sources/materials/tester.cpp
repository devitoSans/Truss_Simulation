#include "catch_amalgamated.hpp"
#include "cross_section.hpp"
#include "materials.hpp"
#include <splashkit.h>

const double ELIPSON = 1e-6;

double round_to(double a)
{
    return std::round(a * 1/ELIPSON) * ELIPSON;
}

bool compare_double(double a, double b)
{
    return abs(round_to(a)-round_to(b)) < ELIPSON;
}

TEST_CASE("Materials: Polymorphism on Acrylic")
{
    Materials* material = new Acrylic();

    REQUIRE(material->get_tensile_strength() == 66);
    REQUIRE(material->get_young_modulus() == 3600);
    REQUIRE(material->get_compressive_strength() == 98.2);

    delete material;
}

TEST_CASE("Cross Sectionn: Polymorphism on Rectangle with circle cut out")
{
    CrossSection* section = new RectangleWithCircleCut(3.650, 3.650, 0.5);

    // write_line(section->get_area());
    REQUIRE(compare_double(section->get_area(), 7.807041));

    delete section;
}

TEST_CASE("Cross Sectionn: Validity of Set width, height, and thickness")
{
    CrossSection* section = new RectangleWithCircleCut(4, 4, 1);

    if(section->get_type() == cs_type::RECTANGLE_WITH_CIRCLE_OUT)
    {
        RectangleWithCircleCut* rcc = (RectangleWithCircleCut*)section;

        REQUIRE(rcc->set_width(3) == true);
        REQUIRE(rcc->set_width(2) == true);
        REQUIRE(rcc->set_width(4) == true);
    
        REQUIRE(rcc->set_thickness(2) == true);
        REQUIRE(rcc->set_thickness(2.1) == false);
        REQUIRE(rcc->set_thickness(1) == true);
    
        REQUIRE(rcc->set_height(2.5) == true);
        REQUIRE(rcc->set_height(2) == false);
        REQUIRE(rcc->set_height(4) == true);
    }
}

TEST_CASE("Cross Sectionn: Second moment of area (I)")
{
    CrossSection* section = new RectangleWithCircleCut(3.650, 3.650, 0.5);
    
    REQUIRE(compare_double(section->get_I(),12.36998126));

    if(section->get_type() == cs_type::RECTANGLE_WITH_CIRCLE_OUT)
    {
        RectangleWithCircleCut* rcc = (RectangleWithCircleCut*)section;
        rcc->set_width(5); // automatically updates for section
    }

    REQUIRE(compare_double(section->get_I(),7.694931));
    // REQUIRE(section.get_I() <= 7.694931 + ELIPSON);
}

// TEST_CASE("Cross Sectionn: Drawwing (Toggleable)")
// {
//     CrossSection* section = new RectangleWithCircleCut(3.650, 3.650, 0.5);

//     open_window("Test", 640, 480);

//     double scale = 100;
//     while(!quit_requested())
//     {
//         process_events();

//         if(key_down(UP_KEY))
//         {
//             scale++;
//         }
//         if(key_down(DOWN_KEY))
//         {
//             scale--;
//         }
//         scale = std::max(0.0,scale);
        
//         clear_screen(color_white());
//         section->draw(300, 200, scale);
        
//         refresh_screen(60);
//     }
// }