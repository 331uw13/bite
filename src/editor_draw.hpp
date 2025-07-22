#ifndef EDITOR_DRAW_HPP
#define EDITOR_DRAW_HPP

#include <initializer_list>
#include <string>
#include <utility>

#include <ncurses.h>

#include "color.hpp"


class Editor;
class Buffer;

namespace EDraw
{
    
    void window(Editor* bite,
            int x, int y,
            int w, int h,
            const std::string& name,
            std::initializer_list<std::pair<std::string, ColorT>> info);

    void cmdline(Editor* bite, Buffer* buf);




};





#endif
