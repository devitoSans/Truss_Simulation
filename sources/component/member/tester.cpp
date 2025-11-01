#include <catch_amalgamated.hpp>
#include "memberModel.hpp"
#include "memberController.hpp"

const double ELIPSON = 1e-3;

double round_to(double a)
{
    return std::round(a * 1/ELIPSON) * ELIPSON;
}

bool compare_double(double a, double b)
{
    return abs(round_to(a)-round_to(b)) < ELIPSON;
}

using Catch::Approx;

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

    // printf("id: %d", member2.get_id());
}

TEST_CASE("MemberData: Yielding and buckling check")
{
    MemberData member = MemberData(120, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());
    member.set_axial_force(-42.46);
    
    REQUIRE(member.is_yielded() == false);
    REQUIRE(member.is_buckled() == false);
    
    member.set_axial_force(-43);

    REQUIRE(member.is_yielded() == false);
    REQUIRE(member.is_buckled() == true);

    member.set_axial_force(589);

    REQUIRE(member.is_yielded() == false);
    REQUIRE(member.is_buckled() == false);

    member.set_axial_force(590);

    REQUIRE(member.is_yielded() == true);
    REQUIRE(member.is_buckled() == false);
}

TEST_CASE("Member Model: Initialising")
{
    MemberModel Imember = MemberModel(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    REQUIRE(Imember.get_mid_pos().x == 0.0);
    REQUIRE(Imember.get_mid_pos().y == 0.0);

    REQUIRE(Imember.get_start_pos().x == -10.0);
    REQUIRE(Imember.get_start_pos().y == 0.0);

    REQUIRE(Imember.get_end_pos().x == 10.0);
    REQUIRE(Imember.get_end_pos().y == 0.0);
}

TEST_CASE("Member Model: Angle and Counter angle")
{
    MemberModel Imember = MemberModel();
    REQUIRE(Imember.get_counter_angle() == 180);
}

TEST_CASE("Member Model: Modify angle and counter angle")
{
    MemberModel Imember = MemberModel(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

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

TEST_CASE("Member Model: Modify length")
{
    MemberModel Imember = MemberModel(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());
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

TEST_CASE("Member Model: Modify start, mid, and end position")
{
    MemberModel Imember = MemberModel(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

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

TEST_CASE("Member Model: detection for start's, body's, and end's intersections")
{
    MemberModel Imember = MemberModel(20, 1.0, cs_type::RECTANGLE_WITH_CIRCLE_OUT, new Acrylic());

    REQUIRE(Imember.is_intersect_start(0.0, 0.0) == false);
    REQUIRE(Imember.is_intersect_start(-12.0, 0.0) == true);
    REQUIRE(Imember.is_intersect_start(-12.01, 0.0) == false);

    REQUIRE(Imember.is_intersect_end(0.0, 0.0) == false);
    REQUIRE(Imember.is_intersect_end(12.0, 0.0) == true);
    REQUIRE(Imember.is_intersect_end(12.01, 0.0) == false);

    REQUIRE(Imember.is_intersect_start(-13, 0, 2) == true);
    REQUIRE(Imember.is_intersect_start(-13.9, 0, 2) == true);
    REQUIRE(Imember.is_intersect_start(-14, 0, 2) == false);
    
    REQUIRE(Imember.is_intersect_end(13, 0, 2) == true);
    REQUIRE(Imember.is_intersect_end(13.9, 0, 2) == true);
    REQUIRE(Imember.is_intersect_end(14, 0, 2) == false);

    Imember.modify_length(0);
    REQUIRE(Imember.is_intersect_start(
        Imember.get_end_pos().x,
        Imember.get_end_pos().y,
        Imember.get_properties().cross_section()->get_girth()/2
    ) == false);

    Imember.modify_length(20);

    // mid
    REQUIRE(Imember.is_intersect_body(0.0, 0.0) == true);

    // up
    REQUIRE(Imember.is_intersect_body(0.0, 2.01) == false);
    REQUIRE(Imember.is_intersect_body(0.0, 2.0) == true);

    // bottom
    REQUIRE(Imember.is_intersect_body(0.0, -2.01) == false);
    REQUIRE(Imember.is_intersect_body(0.0, -2.0) == true);

    // left-up
    REQUIRE(Imember.is_intersect_body(-10.0, 2.0) == true);
    REQUIRE(Imember.is_intersect_body(-10.01, 0.0) == false);

    // right-up
    REQUIRE(Imember.is_intersect_body(10.0, 2.0) == true);
    REQUIRE(Imember.is_intersect_body(10.01, 0.0) == false);

    // left-bottom
    REQUIRE(Imember.is_intersect_body(-10.0, -2.0) == true);
    REQUIRE(Imember.is_intersect_body(-10.01, 0.0) == false);

    // right-bottom
    REQUIRE(Imember.is_intersect_body(10.0, -2.0) == true);
    REQUIRE(Imember.is_intersect_body(10.01, 0.0) == false);
}

void TEST1()
{
    MemberModel Imember = MemberModel(20.0, 5.0);

    const int WIDTH = 640, HEIGHT = 480; 

    open_window("Test", WIDTH, HEIGHT);
    Imember.modify_mid_pos(WIDTH/2, HEIGHT/2);
    // Imember.modify_angle(-90);
    
    double length = Imember.get_properties().get_length();
    while(!quit_requested())
    {
        process_events();
        
        if(key_down(UP_KEY))
        {
            length++;
        }
        if(key_down(DOWN_KEY))
        {
            length--;
        }
        if(key_down(W_KEY))
        {
            Imember.modify_mid_pos(Imember.get_mid_pos().x-1, Imember.get_mid_pos().y);
        }
        if(key_down(S_KEY))
        {
            Imember.modify_mid_pos(Imember.get_mid_pos().x+1, Imember.get_mid_pos().y);
        }
        if(key_down(Q_KEY))
        {
            Imember.modify_angle(Imember.get_angle()-1);
        }
        if(key_down(A_KEY))
        {
            Imember.modify_angle(Imember.get_angle()+1);
        }
        if(key_down(E_KEY))
        {
            Imember.modify_counter_angle(Imember.get_counter_angle()+1);
        }
        if(key_down(D_KEY))
        {
            Imember.modify_counter_angle(Imember.get_counter_angle()-1);
        }
        length = std::max(0.0,length);
        // Imember.modify_length(length);
        // Imember.get_properties().set_axial_force(Imember.read_properties().get_length());
        Imember.modify_start_pos(mouse_position().x, mouse_position().y);

        clear_screen(color_white());
        Imember.draw(true);
        
        refresh_screen(60);
    } 
}

TEST_CASE("Member Model (Manual Testing): Drawing (Toggleable)")
{
    // TEST1();
}

void TEST2()
{
    MultiMemberController membersController = MultiMemberController("../../../resources/");

    const int WIDTH = 640, HEIGHT = 480; 

    open_window("Test", WIDTH, HEIGHT);

    int a = -1;
    while(!quit_requested())
    {
        process_events();
        a = membersController.update(true);

        Connection c = { a, Part::MEMBER_START };
        for(auto& [i, _] : membersController.get_part_angles(c))
        {
            printf("angle: %f, ", i);
        }
        c.second = Part::MEMBER_END;
        for(auto& [i, _] : membersController.get_part_angles(c))
        {
            printf("%f\n", i);
        }

        clear_screen(color_white());

        membersController.draw();

        refresh_screen(60);
    }
}

TEST_CASE("MultiMemberController (Manual Testing): Controlling Members (Toggleable)")
{
    TEST2();
}