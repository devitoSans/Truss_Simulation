#include <catch_amalgamated.hpp>
#include "forceModel.hpp"
#include "forceController.hpp"

void TEST1()
{
    ForceModel fm = ForceModel();
    open_window("Test", 640, 480);

    fm.move(300, 200);

    while(!quit_requested())
    {
        process_events();

        clear_screen(color_white());

        fm.draw();

        refresh_screen(60);
    }
}

TEST_CASE("ForceModel (Manual Testing): Drawing (Toggleable)")
{
    // TEST1();
}

void TEST2()
{
    ForceModel fm = ForceModel();
    open_window("Test", 640, 480);

    fm.move(300, 200);

    bool moving = true;
    while(!quit_requested())
    {
        process_events();

        moving = true;
        if(key_down(SPACE_KEY))
        {
            moving = false;
        }

        if(moving)
        {
            fm.move(mouse_position().x, mouse_position().y);
        }
        else 
        {
            vector_2d direction = vector_subtract(fm.get_mid_pos(), mouse_position_vector());
            fm.rotate(vector_angle(direction));
        }

        clear_screen(color_white());

        fm.draw();

        refresh_screen(60);
    }
}

TEST_CASE("ForceModel (Manual Testing): Moving and rotating (Toggleable)")
{
    // TEST2();
}

void TEST3()
{
    MultiForceController mfc = MultiForceController();

    const int WIDTH = 640, HEIGHT = 480; 

    open_window("Test", WIDTH, HEIGHT);

    while(!quit_requested())
    {
        process_events();
        mfc.update();

        clear_screen(color_white());

        mfc.draw();

        refresh_screen(60);
    }
}

TEST_CASE("MultiForceController (Manual Testing): Controlling (Toggleable)")
{
    // TEST3();
}