
#include <ncurses.h>

#include "selectreg_act.hpp"
#include "buffer.hpp"

    

void SelectRegAct::draw (Buffer* buf, const std::string& ln, Int64x2 ln_pos) {

    if(ln.empty()) { return; }

    int y = buf->last_draw_base_y + ln_pos.y - buf->scroll;
    int x = buf->last_draw_base_x + ln_pos.x;
    
    mvaddstr(y, x, ln.c_str());

}


