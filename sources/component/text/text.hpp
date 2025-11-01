#ifndef __TEXT__
#define __TEXT__

#include <string>
#include <splashkit.h>

inline void drawInfo(std::string resourcesPath, 
                     double value, 
                     std::string unit, 
                     color colorText, 
                     double x, double y, double radius,
                     int fontSize = 15)
{
    // Setup font
    font fnt = load_font("VarelaRound", resourcesPath + "fonts/VarelaRound-Regular.ttf");
    set_font_style(fnt, NORMAL_FONT);

    std::string text = std::to_string(value);

    // Format the text
    char buf[text.size()];
    std::snprintf(buf, sizeof(buf), "%.3f", value);
    std::string formattedText = buf + unit;   

    // Get the text's width and height
    double width = text_width(formattedText, fnt, fontSize);
    double height = text_height(formattedText, fnt, fontSize);

    fill_circle(color_white(), x, y, radius);
    draw_circle(colorText, x, y, radius);
    fill_rectangle(color_white(), {x, y, width, height});

    draw_text(formattedText, colorText, fnt, fontSize, x, y);
}

#endif