#ifndef EDITOR_COLOR_HPP
#define EDITOR_COLOR_HPP


typedef int ColorT;


namespace Color {
    static constexpr int NUM_DARK_SHADES = 2;
    enum EditorColor : ColorT {
        BACKGROUND = 0,
        WHITE,   DARK_WHITE_0,    DARK_WHITE_1,
        BEIGE,   DARK_BEIGE_0,    DARK_BEIGE_1,
        PINK,    DARK_PINK_0,     DARK_PINK_1,
        RED,     DARK_RED_0,      DARK_RED_1,
        ORANGE,  DARK_ORANGE_0,   DARK_ORANGE_1,
        YELLOW,  DARK_YELLOW_0,   DARK_YELLOW_1,
        LIME,    DARK_LIME_0,     DARK_LIME_1,
        GREEN,   DARK_GREEN_0,    DARK_GREEN_1,
        CYAN,    DARK_CYAN_0,     DARK_CYAN_1,
        BLUE,    DARK_BLUE_0,     DARK_BLUE_1,
        PURPLE,  DARK_PURPLE_0,   DARK_PURPLE_1,
        MAGENTA, DARK_MAGENTA_0,  DARK_MAGENTA_1,
        CURSOR,
        CMD_CURSOR,
        NUM_COLORS
    };
}



#endif
