
#include "editor.hpp"
#include "editor_draw.hpp"
#include "util.hpp" 
   

void EDraw__window_infostr(Editor* bite, int x, int y, const std::string& str, int color) {

    bite->set_color(color);
    mvaddstr(y, x+1, str.c_str());
    
    bite->set_color(bite->settn.style.win_infostr_affix_color);
    mvaddch(y, x,              bite->settn.style.win_infostr_prefix_ch);
    mvaddch(y, x+str.size()+1, bite->settn.style.win_infostr_suffix_ch);
}


void EDraw::window(Editor* bite,
            int x, int y,
            int w, int h,
            const std::string& name,
            std::initializer_list<std::pair<std::string, ColorT>> info
){

    bite->set_color(bite->settn.style.win_border_color);

    // Top left corner.
    mvadd_wch(y, x, &bite->settn.style.TL_corner_ch);

    // Top right corner.
    mvadd_wch(y, x+w+1, &bite->settn.style.TR_corner_ch);
    
    // Bottom left corner.
    mvadd_wch(y+h+1, x, &bite->settn.style.BL_corner_ch);
    
    // Bottom right corner.
    mvadd_wch(y+h+1, x+w+1, &bite->settn.style.BR_corner_ch);
  

    // Top line.
    mvhline_set(
            y,
            x + 1,
            &bite->settn.style.T_line_ch,
            w
            );

    // Bottom line.
    mvhline_set(
            y + h + 1,
            x + 1,
            &bite->settn.style.B_line_ch,
            w
            );
   
    // Left line.
    mvvline_set(
            y + 1,
            x,
            &bite->settn.style.L_line_ch,
            h
            );

    // Right line.
    mvvline_set(
            y + 1,
            x + w + 1,
            &bite->settn.style.L_line_ch,
            h
            );


    int info_offX = x + w - name.size() - 1;
    EDraw__window_infostr(bite, info_offX, y, name, bite->settn.style.win_name_color);


    for(std::pair<std::string, ColorT> part : info) {
        info_offX -= part.first.size()+3;       
        EDraw__window_infostr(bite, info_offX, y, part.first, part.second);

    }
    
}

void EDraw::cmdline(Editor* bite, Buffer* buf) {
    int X = buf->pos_x + 3;
    int Y = buf->last_draw_base_y + buf->height + CMDLINE_HEIGHT - 1;
    
    U::clear_part(X, Y, CMDLINE_MAX+1);

    // Command line data.
    bite->set_color(bite->settn.style.cmdline_color);
    mvaddstr(Y, X, buf->cmd.c_str());
    
    // Command line cursor.
    char cursor_char = buf->cmd[buf->cmd_cur_x];
    if(cursor_char == 0) {
        cursor_char = 0x20;
    }
    bite->set_color(Color::CMD_CURSOR);
    mvaddch(Y, X + buf->cmd_cur_x, cursor_char);

    // Command line indicator.
    bite->set_color(bite->settn.style.cmdline_indicator_color);
    mvaddch(Y, X - 2, bite->settn.style.cmdline_indicator_ch);


    // Border.
    bite->set_color(bite->settn.style.win_border_color);

    // Bottom line.
    mvhline_set(
            Y + 1,
            buf->pos_x + 1,
            &bite->settn.style.B_line_ch,
            buf->width
            );
 
    // Left bottom corner.
    mvadd_wch(Y + 1, buf->pos_x, &bite->settn.style.BL_corner_ch);
    
    // Right bottom corner.
    mvadd_wch(Y + 1, buf->pos_x + buf->width + 1, &bite->settn.style.BR_corner_ch);
    
    // Left and right lines.
    mvadd_wch(Y, buf->pos_x,                  &bite->settn.style.L_line_ch);
    mvadd_wch(Y, buf->pos_x + buf->width + 1, &bite->settn.style.L_line_ch);


    // Connecting characters.

    /*
    // Left line.
    mvvline_set(
            y + 1,
            x,
            &bite->settn.style.L_line_ch,
            h
            );
    */
}





