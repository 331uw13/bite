
#include <ncurses.h>

#include "selectreg_act.hpp"
#include "buffer.hpp"

    

void SelectRegAct::draw (Buffer* buf, const std::string& ln, Int64x2 ln_pos) {

    if(ln.empty()) { return; }

    int y = buf->last_draw_base_y + ln_pos.y - buf->scroll;
    int x = buf->last_draw_base_x + ln_pos.x;
   
    if(y < buf->pos_y+1) { return; }
    else
    if(y > buf->pos_y + buf->height) { return; }

    mvaddnstr(y, x, ln.c_str(), buf->width - buf->last_draw_lndigits - 2); 

}


