#include <catch_amalgamated.hpp>
#include "memberData.hpp"
#include "memberInterface.hpp"

const double ELIPSON = 1e-3;

double round_to(double a)
{
    return std::round(a * 1/ELIPSON) * ELIPSON;
}

bool compare_double(double a, double b)
{
    return abs(round_to(a)-round_to(b)) < ELIPSON;
}

TEST_CASE("MemberData: Initialising a member")
{
    MemberData member = MemberData(120, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    REQUIRE(member.get_length() == 120);

    REQUIRE(member.cross_section()->get_type() == cs_type::RECTANGLE_WITH_CIRCLE_OUT);

    REQUIRE(member.get_material()->get_compressive_strength() == 98.2);
    REQUIRE(member.get_material()->get_tensile_strength() == 66);
    REQUIRE(member.get_material()->get_young_modulus() == 3600);

    MemberData member2(member);

    REQUIRE(member2.get_length() == 120);

    REQUIRE(member2.cross_section()->get_type() == cs_type::RECTANGLE_WITH_CIRCLE_OUT);

    REQUIRE(member2.get_material()->get_compressive_strength() == 98.2);
    REQUIRE(member2.get_material()->get_tensile_strength() == 66);
    REQUIRE(member2.get_material()->get_young_modulus() == 3600);
}

TEST_CASE("Member Interface: Initialising")
{
    MemberInterface Imember = MemberInterface(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    REQUIRE(Imember.get_mid_pos().x == 0.0);
    REQUIRE(Imember.get_mid_pos().y == 0.0);

    REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == 0.0);

    REQUIRE(Imember.get_end_pos().x == 10.0);
    REQUIRE(Imember.get_end_pos().y == 0.0);
}

TEST_CASE("Member Interface: Angle and Counter angle")
{
    MemberInterface Imember = MemberInterface();
    REQUIRE(Imember.get_counter_angle() == 180);
}

TEST_CASE("Member Interface: Modify angle and counter angle")
{
    MemberInterface Imember = MemberInterface(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    Imember.modify_angle(90);

    // REQUIRE(compare_double(Imember.get_start_pos().x, -10.0));
    REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == 0.0);

    REQUIRE(compare_double(Imember.get_end_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_end_pos().y, 20.0));

    REQUIRE(compare_double(Imember.get_mid_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_mid_pos().y, 10.0));
    
    Imember.modify_counter_angle(0);
    
    REQUIRE(compare_double(Imember.get_start_pos().x, 10.0));
    REQUIRE(compare_double(Imember.get_start_pos().y, 20.0));

    REQUIRE(compare_double(Imember.get_mid_pos().x, 0));
    REQUIRE(compare_double(Imember.get_mid_pos().y, 20.0));

    Imember.modify_angle(180 * 5);

    REQUIRE(compare_double(Imember.get_start_pos().x, 10.0));
    REQUIRE(compare_double(Imember.get_start_pos().y, 20.0));

    REQUIRE(compare_double(Imember.get_mid_pos().x, 0));
    REQUIRE(compare_double(Imember.get_mid_pos().y, 20.0));
    
    // REQUIRE(Imember.get_end_pos().x == -10.0);
    // REQUIRE(Imember.get_end_pos().y == 10.0);
    // REQUIRE(Imember.get_start_pos().x == 10.0);
}

TEST_CASE("Member Interface: Modify length")
{
    MemberInterface Imember = MemberInterface(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());
    Imember.modify_angle(90);

    Imember.modify_length(10);

    REQUIRE(compare_double(Imember.get_start_pos().x, -10.0));
    REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == 5.0);

    REQUIRE(compare_double(Imember.get_end_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_end_pos().y, 15.0));

    REQUIRE(compare_double(Imember.get_mid_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_mid_pos().y, 10.0));

    Imember.modify_length(30);

    REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == -5.0);

    REQUIRE(compare_double(Imember.get_end_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_end_pos().y, 25.0));

    REQUIRE(compare_double(Imember.get_mid_pos().x, -10.0));
    REQUIRE(compare_double(Imember.get_mid_pos().y, 10.0));
}

TEST_CASE("Member Interface: Modify start, mid, and end position")
{
    MemberInterface Imember = MemberInterface(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    Imember.modify_mid_pos(0.0, 10);

    // REQUIRE(compare_double(Imember.get_start_pos().x, -10.0));
    // REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == 10.0);

    REQUIRE(Imember.get_end_pos().x == 10.0);
    REQUIRE(Imember.get_end_pos().y == 10.0);

    REQUIRE(Imember.get_mid_pos().x == 0.0);
    REQUIRE(Imember.get_mid_pos().y == 10.0);

    REQUIRE(Imember.get_angle() == 0.0);

    Imember.modify_start_pos(10.0, -20);

    REQUIRE(Imember.get_angle() == 90);
    REQUIRE(Imember.get_counter_angle() == 270);
    REQUIRE(Imember.get_properties().get_length() == 30);
    REQUIRE(Imember.get_mid_pos().x == 10.0);
    REQUIRE(Imember.get_mid_pos().y == -5.0);

    Imember.modify_end_pos(5, -20);

    REQUIRE(Imember.get_angle() == 180.0);
    REQUIRE(Imember.get_counter_angle() == 0.0);
    REQUIRE(Imember.get_properties().get_length() == 5.0);
    REQUIRE(Imember.get_mid_pos().x == 7.5);
    REQUIRE(Imember.get_mid_pos().y == -20.0);
}

// TEST_CASE("Member Interface: Drawing (Toggleable)")
// {
//     MemberInterface Imember = MemberInterface();

//     const int WIDTH = 1920, HEIGHT = 1080; 

//     open_window("Test", WIDTH, HEIGHT);
//     Imember.modify_mid_pos(WIDTH/2, HEIGHT/2);
//     Imember.modify_angle(-90);

//     double length = Imember.get_properties().get_length();
//     while(!quit_requested())
//     {
//         process_events();
        
//         if(key_down(UP_KEY))
//         {
//             length++;
//         }
//         if(key_down(DOWN_KEY))
//         {
//             length--;
//         }
//         if(key_down(W_KEY))
//         {
//             Imember.modify_mid_pos(Imember.get_mid_pos().x-1, Imember.get_mid_pos().y);
//         }
//         if(key_down(S_KEY))
//         {
//             Imember.modify_mid_pos(Imember.get_mid_pos().x+1, Imember.get_mid_pos().y);
//         }
//         if(key_down(Q_KEY))
//         {
//             Imember.modify_angle(Imember.get_angle()-1);
//         }
//         if(key_down(A_KEY))
//         {
//             Imember.modify_angle(Imember.get_angle()+1);
//         }
//         if(key_down(E_KEY))
//         {
//             Imember.modify_counter_angle(Imember.get_counter_angle()+1);
//         }
//         if(key_down(D_KEY))
//         {
//             Imember.modify_counter_angle(Imember.get_counter_angle()-1);
//         }
//         length = std::max(0.0,length);
//         // Imember.modify_length(length);
//         Imember.modify_end_pos(mouse_position().x, mouse_position().y);

//         clear_screen(color_white());
//         Imember.draw();
        
//         refresh_screen(60);
//     }
// }