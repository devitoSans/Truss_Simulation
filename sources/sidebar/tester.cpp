// #include <catch_amalgamated.hpp>
// #include <splashkit.h>

// TEST_CASE("TS")
// {
//   open_window("Demo", 800, 600);

//   bool show_tools = true;
//   int  tools_gen  = 0;  // increments each time we reopen

//   int width = 250;
//     // rectangle fda = rectangle_from(0, -20, width, 180);
//   while (!quit_requested()) 
//   {
//     process_events();
//     clear_screen(color_white());

//     // Button/key to request showing the panel again
//     if (button("Reopen Tools") || key_down(T_KEY)) {
//       show_tools = true;
//       tools_gen++; // force a new internal state
//       width++;
//     }

//     if (show_tools) {
//       // Different name each "generation" so SplashKit treats it as new
//       string name = "Tools #" + std::to_string(tools_gen);
//       rectangle tools_rect = rectangle_from(0, -20, width, 180);

//       if (start_panel(name, tools_rect)) {
//         label_element("This is a panel");
//         if (button("Close (or click X)")) {
//           // Let closing happen; start_panel will return false next frame
//         }
//         end_panel(name);
//       } else {
//         // It was closed this or a prior frame; stop asking for it
//         show_tools = false;
//       }

//     }   

//     // interface_style_panel(fda);

//     // fill_rectangle(color_red(), { 0, 0, 100, 50 }, option_to_world());

//     // move_camera_to(-50, -50);

//     draw_interface();
//     refresh_screen();
//   }
// }

// void asdf()
// {
//         open_window("World rectangle + camera move", 1024, 720);

//     // A rectangle anchored in WORLD space
//     rectangle world_rect = rectangle_from(100, 100, 300, 200);

//     // Camera pan speed (pixels per frame)
//     const double cam_speed = 8.0;

//     while (!quit_requested())
//     {
//         process_events();

//         // --- Camera movement (WASD or arrows) ---
//         double dx = 0, dy = 0;
//         if (key_down(A_KEY) || key_down(LEFT_KEY))  dx -= cam_speed;
//         if (key_down(D_KEY) || key_down(RIGHT_KEY)) dx += cam_speed;
//         if (key_down(W_KEY) || key_down(UP_KEY))    dy -= cam_speed;
//         if (key_down(S_KEY) || key_down(DOWN_KEY))  dy += cam_speed;
//         if (dx != 0 || dy != 0) move_camera_by(dx, dy);

//         clear_screen(COLOR_WHITE);

//         // Tell drawing calls to use WORLD coordinates (affected by camera)
//         drawing_options world = option_defaults();
//         world.camera = DRAW_TO_WORLD;

//         // --- (Optional) simple world grid so movement is obvious ---
//         for (int x = -2000; x <= 2000; x += 100)
//             draw_line(COLOR_LIGHT_GRAY, x, -2000, x, 2000, world);
//         for (int y = -2000; y <= 2000; y += 100)
//             draw_line(COLOR_LIGHT_GRAY, -2000, y, 2000, y, world);

//         // Axis lines
//         draw_line(COLOR_BLACK, -2000, 0, 2000, 0, world);
//         draw_line(COLOR_BLACK, 0, -2000, 0, 2000, world);

//         // --- The world-anchored rectangle ---
//         fill_rectangle(COLOR_ORANGE_RED, world_rect, world);
//         draw_rectangle(COLOR_BLACK, world_rect, world);

//         // --- HUD (screen space; NOT affected by camera) ---
//         drawing_options camera = option_defaults();
//         camera.camera = DRAW_TO_SCREEN;
//         draw_text("W/A/S/D or Arrow Keys to move the camera", COLOR_BLACK, 16, 16, camera);
//         draw_text("Orange rectangle is at world (100,100)-(400,300)", COLOR_BLACK, 16, 40, camera);

//         refresh_screen(60);
//     }

// }

// TEST_CASE("TEST")
// {
//     // asdf();
// }

// void a()
// {
    
// }

// #include "splashkit.h"
// #include <cmath>

// // --- Camera state (top-left in WORLD units) + zoom ---
// double cam_x = 0, cam_y = 0;           // current
// double cam_tx = 0, cam_ty = 0;         // targets
// double zoom_ = 1.0, zoom_t = 1.0;      // current & target zoom
// const double Z_MIN = 0.25, Z_MAX = 8.0;

// // Smooth towards target (fixed-step easing; independent of any timer API)
// inline double ease(double current, double target, double alpha)
// {
//     return current + (target - current) * alpha; // 0<alpha<=1
// }

// // Convert WORLD -> SCREEN using our camera + zoom
// inline point_2d world_to_screen(double wx, double wy)
// {
//     return point_at((wx - cam_x) * zoom_, (wy - cam_y) * zoom_);
// }

// inline rectangle world_rect_to_screen(rectangle r)
// {
//     point_2d tl = world_to_screen(r.x, r.y);
//     return rectangle_from(tl.x, tl.y, r.width * zoom_, r.height * zoom_);
// }

// // Draw a simple grid in the visible world bounds
// void draw_grid()
// {
//     // Visible world rectangle
//     double left   = cam_x;
//     double top    = cam_y;
//     double right  = cam_x + screen_width()  / zoom_;
//     double bottom = cam_y + screen_height() / zoom_;

//     const int G = 100;
//     int x0 = (int)floor(left / G) * G, x1 = (int)ceil(right / G) * G;
//     int y0 = (int)floor(top  / G) * G, y1 = (int)ceil(bottom/ G) * G;

//     for (int x = x0; x <= x1; x += G)
//     {
//         point_2d a = world_to_screen(x, top);
//         point_2d b = world_to_screen(x, bottom);
//         draw_line(COLOR_GAINSBORO, a.x, a.y, b.x, b.y);
//     }
//     for (int y = y0; y <= y1; y += G)
//     {
//         point_2d a = world_to_screen(left, y);
//         point_2d b = world_to_screen(right, y);
//         draw_line(COLOR_GAINSBORO, a.x, a.y, b.x, b.y);
//     }

//     // Axes
//     point_2d x1s = world_to_screen(left, 0), x2s = world_to_screen(right, 0);
//     point_2d y1s = world_to_screen(0, top),  y2s = world_to_screen(0, bottom);
//     draw_line(COLOR_BLACK, x1s.x, x1s.y, x2s.x, x2s.y);
//     draw_line(COLOR_BLACK, y1s.x, y1s.y, y2s.x, y2s.y);
// }

// // Zoom toward a screen pivot (e.g., mouse). Keeps the point under the cursor fixed.
// void zoom_toward(double factor, double pivot_sx, double pivot_sy)
// {
//     double new_zt = zoom_t * factor;
//     if (new_zt < Z_MIN) new_zt = Z_MIN;
//     if (new_zt > Z_MAX) new_zt = Z_MAX;
//     if (fabs(new_zt - zoom_t) < 1e-9) return;

//     // World position under pivot before changing zoom_t
//     double world_px = cam_x + pivot_sx / zoom_;
//     double world_py = cam_y + pivot_sy / zoom_;

//     zoom_t = new_zt;

//     // Choose camera targets so that world_px/world_py remains under the same screen pixel
//     cam_tx = world_px - pivot_sx / zoom_t;
//     cam_ty = world_py - pivot_sy / zoom_t;
// }

// int main()
// {
//     open_window("Smooth pan + zoom (no wheel, no timers)", 1024, 720);

//     // Some world content
//     rectangle world_rect = rectangle_from(100, 100, 300, 200);

//     while (!quit_requested())
//     {
//         process_events();

//         // --- Input: pan (keeps screen-speed roughly constant across zooms)
//         double step = 12.0 / zoom_; // tweak to taste
//         if (key_down(A_KEY) || key_down(LEFT_KEY))  cam_tx -= step;
//         if (key_down(D_KEY) || key_down(RIGHT_KEY)) cam_tx += step;
//         if (key_down(W_KEY) || key_down(UP_KEY))    cam_ty -= step;
//         if (key_down(S_KEY) || key_down(DOWN_KEY))  cam_ty += step;

//         // --- Input: zoom (Q/E) – zooms toward the mouse cursor
//         if (key_down(Q_KEY)) zoom_toward(1.0 / 2.0, mouse_x(), mouse_y());
//         if (key_down(E_KEY)) zoom_toward(2.0,        mouse_x(), mouse_y());

//         // Reset
//         if (key_typed(R_KEY)) { cam_x = cam_y = cam_tx = cam_ty = 0; zoom_ = zoom_t = 1.0; }

//         // --- Smooth toward targets (alpha controls “butteriness”)
//         cam_x = ease(cam_x, cam_tx, 0.20);   // higher = snappier
//         cam_y = ease(cam_y, cam_ty, 0.20);
//         zoom_ = ease(zoom_, zoom_t, 0.25);

//         clear_screen(COLOR_WHITE);

//         // Grid + world rectangle (drawn using transformed screen coords)
//         draw_grid();
//         rectangle sr = world_rect_to_screen(world_rect);
//         fill_rectangle(COLOR_ORANGE_RED, sr);
//         draw_rectangle(COLOR_BLACK, sr);

//         // HUD (screen space)
//         draw_text("Pan: WASD/Arrows  |  Zoom: Q/E (toward cursor)  |  Reset: R",
//                   COLOR_BLACK, 16, 16);
//         draw_text("If you need mouse-wheel zoom, tell me your SplashKit version.",
//                   COLOR_GRAY, 16, 40);

//         refresh_screen(60);
//     }
//     return 0;
// }

#include "splashkit.h"
#include <string>

int main()
{
    open_window("Popup: safe pattern", 800, 500);

    const std::string POP = "Tools";   // single source of truth for the name

    while (!quit_requested())
    {
        process_events();
        clear_screen(COLOR_WHITE);

        // Triggers to open the popup (opens at mouse position)
        if (button("Open Tools (or press P)")) open_popup(POP);
        if (key_typed(P_KEY))                  open_popup(POP);

        // ✅ Only call end_popup if start_popup returned true, and with the SAME name
        if (start_popup(POP))
        {
            label_element("Hello from a popup!");
            if (button("Do something")) { /* ... */ }
            label_element("Click outside or press ESC to close.");
            end_popup(POP);
        }

        draw_interface();   // required each frame after building UI
        refresh_screen(60);
    }
    return 0;
}

