#include <catch_amalgamated.hpp>
#include "supportModel.hpp"
#include "supportController.hpp"

using Catch::Approx;

TEST_CASE("SupportData: Sanity Check")
{
    SupportData supportData = SupportData(true, true);
    
    REQUIRE(supportData.hasHorizontal == true);
    REQUIRE(supportData.hasVertical == true);

    REQUIRE(supportData.horizontalForce == Approx(0.0));
    REQUIRE(supportData.verticalForce == Approx(0.0));

    REQUIRE(supportData.pinGirth == Approx(4.0));
}

TEST_CASE("PinJointModel: Initialisation")
{
    PinJointModel pinJointModel = PinJointModel();
    // printf("%d\n", pinJointModel.get_id());

    REQUIRE(pinJointModel.get_scaled_girth() == Approx(20.0));
    REQUIRE(pinJointModel.get_scaled_height() == Approx(22.0));

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(0.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-10.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(22.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(22.0));
}

TEST_CASE("PinJointModel: Move")
{
    PinJointModel pinJointModel = PinJointModel();

    pinJointModel.move(10, 10);

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(10.0));

    // REQUIRE(pinJointModel.get_left_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(32.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(20.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(32.0));

    pinJointModel.move(2, 30);
    REQUIRE(pinJointModel.get_mid_pos().x == Approx(2.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(30.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-8.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(52.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(12.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(52.0));
}

TEST_CASE("PinJointModel: Rotate")
{
    PinJointModel pinJointModel = PinJointModel();

    pinJointModel.rotate(90);

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(0.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-10.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(22.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(22.0));

    pinJointModel.rotate(180);

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-22.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(-10.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(-22.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(10.0));

    pinJointModel.rotate(-90);

    REQUIRE(pinJointModel.get_left_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(-22.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(-10.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(-22.0));
}

TEST_CASE("PinJointModel: Move & Rotate")
{
    PinJointModel pinJointModel = PinJointModel();

    pinJointModel.rotate(90);

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(0.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-10.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(22.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(22.0));

    pinJointModel.move(10, 2);

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(2.0));

    // REQUIRE(pinJointModel.get_left_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(24.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(20.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(24.0));

    pinJointModel.move(0, -11);
    // -10, -13

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(-11.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(-10.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(11.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(10.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(11.0));

    pinJointModel.rotate(0);

    REQUIRE(pinJointModel.get_mid_pos().x == Approx(0.0));
    REQUIRE(pinJointModel.get_mid_pos().y == Approx(-11.0));

    REQUIRE(pinJointModel.get_left_pos().x == Approx(22.0));
    REQUIRE(pinJointModel.get_left_pos().y == Approx(-1.0));

    REQUIRE(pinJointModel.get_right_pos().x == Approx(22.0));
    REQUIRE(pinJointModel.get_right_pos().y == Approx(-21.0));
}

TEST_CASE("PinJointModel: point intersection")
{
    PinJointModel pinJointModel = PinJointModel();

    REQUIRE(pinJointModel.is_intersect(0, 0) == true);
    REQUIRE(pinJointModel.is_intersect(0, -10) == true);
    REQUIRE(pinJointModel.is_intersect(0, 22) == true);
    REQUIRE(pinJointModel.is_intersect(0, 23) == false);
    REQUIRE(pinJointModel.is_intersect(3, 20) == true);
}

TEST_CASE("PinJointModel: circle intersection")
{
    PinJointModel pinJointModel = PinJointModel();

    REQUIRE(pinJointModel.is_intersect(0, -20.0, 10) == false);
    REQUIRE(pinJointModel.is_intersect(0, -19.0, 10) == true);
    REQUIRE(pinJointModel.is_intersect(0, 21, 10) == false);
}

void TEST1()
{
    open_window("Test", 640, 480);
    PinJointModel pinJointModel = PinJointModel();
    pinJointModel.move(300, 200);

    double angle = 0;
    while(!quit_requested())
    {
        process_events();

        if(key_down(UP_KEY))
        {
            angle += 10;
            pinJointModel.rotate(angle);
        }
        if(key_down(DOWN_KEY))
        {
            angle -= 10;
            pinJointModel.rotate(angle);
        }

        pinJointModel.move(mouse_position().x, mouse_position().y);

        clear_screen(color_white());
        pinJointModel.draw(true);

        refresh_screen(60);
    }
}

TEST_CASE("Pin Joint (Manual Testing): Drawing, move, and rotate (Toggelable)")
{
    // TEST1();
}

void TEST2()
{
    open_window("Test", 640, 480);
    Roller roller = Roller();
    roller.move(300, 200);
    
    while(!quit_requested())
    {
        process_events();

        clear_screen(color_white());
        roller.draw();

        refresh_screen(60);
    }
}

TEST_CASE("Roller (Manual Testing): Drawing  (Toggelable)")
{
    // TEST2();
}

void TEST3()
{
    open_window("Test", 640, 480);
    Roller roller = Roller();
    roller.move(300, 200);

    double angle = 0;
    while(!quit_requested())
    {
        process_events();

        if(key_down(UP_KEY))
        {
            angle += 10;
            roller.rotate(angle);
        }
        if(key_down(DOWN_KEY))
        {
            angle -= 10;
            roller.rotate(angle);
        }

        roller.move(mouse_position().x, mouse_position().y);

        clear_screen(color_white());
        roller.draw(true, color_black());

        refresh_screen(60);
    }
}

TEST_CASE("Roller (Manual Testing): Move & Rotate  (Toggelable)")
{
    // TEST3();
}

// TEST_CASE("Roller: Point intersection")
// {
//     Roller roller = Roller();

//     REQUIRE(roller.is_intersect(0, 22) == true);
// }

void TEST4()
{
    MultiSupportController msc = MultiSupportController("../../../resources/");

    const int WIDTH = 640, HEIGHT = 480; 

    open_window("Test", WIDTH, HEIGHT);

    int id = -1;
    while(!quit_requested())
    {
        process_events();
        id = msc.update(true);

        clear_screen(color_white());
        
        Connection c = {id, Part::SUPPORT_HEAD};
        for(auto& i : msc.get_part_angles(c))
        {
            printf("angle: %f, ", i);
        }
        printf("\n");

        msc.draw();

        refresh_screen(60);
    }
}

TEST_CASE("MultiSupportController (Manual Testing): Controlling  (Toggelable)")
{
    TEST4();
}