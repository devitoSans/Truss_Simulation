#include <catch_amalgamated.hpp>

#include <splashkit.h>
#include "../../definition.hpp"

TEST_CASE("TES")
{
    const int WIDTH = 640, HEIGHT = 480; 

    open_window("Test", WIDTH, HEIGHT);

    point_2d loc = {0.0, 0.0};

    int width = 50;
    while(!quit_requested())
    {
        process_events();
        // loc = mouse_position();
        loc = {300, 200};
        
        clear_screen(color_white());

        if(key_down(UP_KEY)) width++;
        if(key_down(DOWN_KEY)) width--;

        font fnt = load_font("VarelaRound", "fonts/VarelaRound-Regular.ttf");
        set_font_style(fnt, NORMAL_FONT);
        struct Text
        {
            std::string val;
            font fnt;
            int width;
            int height;
        };
        std::string s = "123.49000989/080914";
        // std::string s = "1";
        Text text = { s, fnt, s.size()+10, 10 };
        int fontSize = 20;

        draw_text(text.val, color_black(), text.fnt, fontSize, (mouse_position().x - text_width(text.val, text.fnt, fontSize) / 2), mouse_position().y - text_height(text.val, text.fnt, fontSize)/2.0);
        
        // printf("%d\n", width);
        // // draw_circle(color_black(), loc.x, loc.y, 10);
        // bitmap bmp = create_bitmap("test", 3000, 10);
        // draw_bitmap("test", loc.x, loc.y);
        // // clear_bitmap("test", color_red());
        // fill_rectangle(color_white(), {loc.x, loc.y, 100, 10});
        // // draw_text_on_bitmap(bmp, "Test", color_black(), 0.0, 0.0);
        
        // // if(bitmap_point_collision(bmp, mouse_position(), ))
        // draw_text("1234.34132 N", color_red(), loc.x, loc.y);
        
        refresh_screen(60);
        // free_bitmap(bmp);
    }
}