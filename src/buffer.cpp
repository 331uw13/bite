#include <ncurses.h>
#include <cstdlib>
#include <cstring>

#include "buffer.hpp"
#include "editor.hpp"
#include "util.hpp"


static const char* BUFFER_MODES_STR[] = {
    "NULLMODE",
    "INSERT",
    "SELECT",
    "---"
};



Buffer::Buffer(const char* name) {
    this->cursor = Cursor();
    this->name = name;
    this->cmd_cur_x = 0;
    this->scroll = 0;
    this->paddn_x = 2;
    this->paddn_y = 1;
    this->width = -1;
    this->height = -1;
    this->tab_width = 3;

    this->set_mode(BufferMode::INSERT);
    this->cmd.reserve(CMDLINE_MAX);

    m_scrnbuf = NULL;
    m_mem_freed = false;
    m_prev_lastln_digits = 1;
    
    add_line(0, "");
}

Buffer::~Buffer() {
    if(!m_mem_freed) {
        this->free_memory();
    }
}

void Buffer::free_memory() {
    if(m_scrnbuf) {
        delete[] m_scrnbuf;
        m_scrnbuf = NULL;
    }

    m_mem_freed = true;
}

/*        
void Buffer::m_draw_command_line(Editor* bite) {
   
    int Y = this->last_draw_base_y + this->height + CMDLINE_HEIGHT - 1;
    int X = this->pos_x + 3;

    U::clear_part(X, Y, CMDLINE_MAX+1);

    bite->set_color(Color::GREEN);
    mvaddstr(Y, X, this->cmd.c_str());


    bite->set_color(Color::DARK_GREEN_0);
    mvaddch(Y, X-2, '>');


    // Draw cursor for command line.

    char cur_char = this->cmd[this->cmd_cur_x];
    if(cur_char == 0) {
        cur_char = 0x20;
    }

    bite->set_color(Color::CMD_CURSOR);
    mvaddch(Y, X + this->cmd_cur_x, cur_char);

}*/

void Buffer::m_draw_message(Editor* bite) {
    int Y = this->height;
    int X = this->pos_x + 2;
   
    bite->set_color(Color::ORANGE);
    mvaddstr(Y, X, this->msg.c_str());

}

void Buffer::draw(Editor* bite) {
    
    if(!m_scrnbuf) {
        fprintf(stderr, "%s: Buffer is not initialized properly.\n",
                __func__);
        return;
    }

    
    const uint32_t lastln_digits = U::count_digits(data.size());
    
    const int base_x = this->pos_x + this->paddn_x + lastln_digits + 1;
    const int base_y = this->pos_y + this->paddn_y;

    this->last_draw_base_x = base_x;
    this->last_draw_base_y = base_y;


    // Buffer window.
    //
    char mode_str_buf[16] = { 0 };
    this->get_mode_str(mode_str_buf, 16);
    EDraw::window(bite, this->pos_x, this->pos_y, this->width, this->height, 
            this->name, {{ mode_str_buf, bite->settn.style.buf_modestr_color }});


    bool update_all = (lastln_digits != m_prev_lastln_digits);
   

    // Draw Data.

    int y = 0;
    size_t draw_start = this->scroll;
    size_t draw_end   = this->scroll + this->height; 

    for(size_t idx = draw_start; idx < draw_end; idx++) {
        if(idx >= data.size()) {
            break;
        }

        Line* ln = &data[idx];
        
        m_scrnbuf[y].prev_length = m_scrnbuf[y].length;
        m_scrnbuf[y].length = ln->str.size();

        if(ln->force_update || update_all) {
            U::clear_part(this->pos_x+1, base_y+y, this->width);
            ln->force_update = false;
        }
        else
        if(m_scrnbuf[y].prev_length > m_scrnbuf[y].length) {
            U::clear_part(this->pos_x+1, base_y+y, this->width);
        }
        

        // Line number.
        U::clear_part(this->pos_x+1, base_y+y, lastln_digits);
        bite->set_color(Color::DARK_WHITE_1);
        uint64_t ln_digits = U::count_digits(idx);
        mvprintw(base_y + y,
                this->pos_x + (lastln_digits - ln_digits) + 1,
                "%li", idx);


        // Line data.
        bite->set_color(Color::DARK_WHITE_0);
        mvaddnstr(base_y + y, base_x, ln->str.c_str(), this->width - lastln_digits - 2); 
        y++;
    }


    if(m_clear_last_row) {
        m_clear_last_row = false;
        U::clear_part(this->pos_x+1, base_y + data.size() - this->scroll, this->width);
    }


    // Draw selected region.
    if(m_mode == BufferMode::SELECT) {
        bite->set_color(Color::RED);
        this->selectreg_action(SelectRegAct::draw);
    }
    else 
    if(m_mode == BufferMode::COMMAND_INPUT) {
        EDraw::cmdline(bite, this);
    }

    if(!this->msg.empty()) {
        m_draw_message(bite);
    }

    // Draw cursor.
    if(m_mode != BufferMode::COMMAND_INPUT) {
        bite->set_color(Color::CURSOR);
        mvprintw(base_y + this->cursor.y - this->scroll,
                 base_x + this->cursor.x,
                 "%c", this->cursor.chr);
    }


    m_prev_lastln_digits = lastln_digits;
}
        
size_t Buffer::get_mode_str(char* outbuf, size_t outbuf_size) {
    const char* mode_str = BUFFER_MODES_STR[m_mode];
    const size_t mode_str_size = strlen(mode_str);
    
    if(outbuf_size < mode_str_size) {
        log_print(ERROR, 
                "'outbuf' size is too small for \"%s\","
                " 16 bytes should always be enough.", mode_str);
        return 0;
    }

    memmove(outbuf, mode_str, mode_str_size);

    return mode_str_size;
}

void Buffer::checkup_scrnbuf() {
    if((this->width < 0) || (this->height < 0)) {
        log_print(ERROR, "buffer size is not set.\n");
        return;
    }

    const size_t new_scrnbuf_size = this->height + 16;

    if(!m_scrnbuf) {
        m_scrnbuf = new ScreenLine[new_scrnbuf_size];
        if(!m_scrnbuf) {
            log_print(FATAL, "Failed to allocate %li elements for \"scrnbuf\"",
                    new_scrnbuf_size);
            return;
        }
    }
    else // TODO: Maybe handle a case where user would resize from huge array to very small.
         //       to free some memory.
    if(m_scrnbuf_size < new_scrnbuf_size) {
        ScreenLine* tmpptr = new ScreenLine[new_scrnbuf_size];
        if(!tmpptr) {
            log_print(FATAL, "Failed to re-allocate %li elements for \"scrnbuf\"",
                    new_scrnbuf_size);
            return;
        }

        delete[] m_scrnbuf;
        m_scrnbuf = tmpptr;
    }

    m_scrnbuf_size = new_scrnbuf_size;
}

Line* Buffer::getln(int64_t y) {
    y = U::iclamp64(y, 0, data.size()-1);
    return &data[y];
}

std::string* Buffer::getlnstr(int64_t y) {
    y = U::iclamp64(y, 0, data.size()-1);
    return &data[y].str;
}

void Buffer::clamp_cursor_xy() {
    this->cursor.y = U::iclamp64(this->cursor.y, 0, data.size()-1);
    this->cursor.x = U::iclamp64(this->cursor.x, 0, getlnstr(this->cursor.y)->size());
}


void Buffer::mov_cursor_to(int64_t x, int64_t y) {
    this->cursor.px = this->cursor.x;
    this->cursor.py = this->cursor.y;
    this->cursor.x = x;
    this->cursor.y = y;

    clamp_cursor_xy();

    this->cursor.y = U::iclamp64(this->cursor.y, 0, data.size()-1);
    Line* ln = getln(this->cursor.y);
    
    this->cursor.chr = ln->str[this->cursor.x];

    if(!U::is_valid_char(this->cursor.chr)) {
        this->cursor.chr = 0x20;
    }

    if(this->cursor.px >= (int64_t)ln->str.size()) {
        ln->force_update = true;
    }
    if(this->cursor.py != this->cursor.y) {
        getln(this->cursor.py)->force_update = true;
    }

    
    // Update buffer scroll.
    if(
    (this->cursor.y >= m_max_row()) /* Scroll Down */
    ||
    (this->cursor.y < this->scroll)) /* Scroll Up */
    {
        this->scroll += (this->cursor.y - this->cursor.py);
        this->scroll = (this->scroll < 0) ? 0 : this->scroll;
    }
   
    // Update buffer selected region if mode is active.
    // Only end position needs to be updated because
    // start position is set when buffer mode is changed to BufferMode::SELECT.
    if(this->get_mode() == BufferMode::SELECT) {
        m_select.endX = cursor.x;
        m_select.endY = cursor.y;
    }
}

void Buffer::mov_cursor(int xoff, int yoff) {
    mov_cursor_to(this->cursor.x + xoff, this->cursor.y + yoff);
}

inline int Buffer::m_max_row() {
    return this->height + this->scroll;
}

bool Buffer::add_char(const Cursor& cur, char c) {
    if(!U::is_valid_char(c)) {
        return false; // Allow only printable ASCII characters.
    }

    std::string* current = getlnstr(cur.y);
    const int64_t x = U::iclamp64(cur.x, 0, current->size());

    current->insert(x, 1, c);
    return true;
}

bool Buffer::del_char(const Cursor& cur) {
    Line* ln = getln(cur.y);
    if(ln->str.empty()) {
        return false;
    }

    const int64_t x = U::iclamp64(cur.x, 0, ln->str.size());
    if(x-1 < 0) {
        return false;
    }
    ln->str.erase(x-1, 1);

    return true;
}

void Buffer::add_line(int64_t y, const std::string& str) {
    data.insert(data.begin() + y,
    (Line){ 
        .str = str,
        .force_update = false
    });
}

void Buffer::del_line(int64_t y) {
    if(data.empty()) {
        return;
    }
    m_clear_last_row = true;
    data.erase(data.begin() + y);
}

bool Buffer::add_tabs(const Cursor& cur, int num) {
    bool result = false;
    for(int i = 0; i < num * this->tab_width; i++) {
        result = add_char(cur, 0x20);
        if(!result) {
            break;
        }
    }
    return result;
}

void Buffer::handle_enter() {
    Line* ln = getln(this->cursor.y);

    if(this->cursor.x < (int64_t)ln->str.size()) {
        /* Split line and move it down */
        
        const size_t size = ln->str.size() - this->cursor.x;
        add_line(this->cursor.y+1, ln->str.substr(this->cursor.x, size));
        
        ln = getln(cursor.y); // Update pointer after insert.
        ln->str.resize(this->cursor.x);
        mov_cursor_to(0, this->cursor.y+1);
    }
    else {
        add_line(this->cursor.y+1, "");
        mov_cursor(0, 1);
    }
}

void Buffer::handle_backspace() {
    clamp_cursor_xy();
    if(this->cursor.x > 0) {
        /* Normal character delete */
        
        uint32_t num_whitespace =
            U::count_whitespace(getlnstr(this->cursor.y), 
                    this->cursor.x,
                    U::IDIRECT::LEFT);

        if(num_whitespace == 0 || (num_whitespace % this->tab_width) != 0) {
            /* Remove normal character */
            if(del_char(this->cursor)) {
                mov_cursor(-1, 0);
            }
        }
        else {
            /* Remove "tab" */
            for(int i = 0; i < this->tab_width; i++) {
                if(del_char(this->cursor)) {
                    mov_cursor(-1, 0);
                }
                else {
                    break;
                }
            }
        }
    }
    else
    if(this->cursor.x == 0 && this->cursor.y > 0) {
        /* Move current line to one above */
        std::string* above   = getlnstr(this->cursor.y - 1);
        std::string* current = getlnstr(this->cursor.y);
        int64_t ncur_x = above->size();
        *above += *current;
        del_line(this->cursor.y);
        mov_cursor_to(ncur_x, this->cursor.y-1);
    }
}

void Buffer::set_mode(BufferMode new_mode) {

    if(new_mode == BufferMode::SELECT) {
        m_select = SelectReg(cursor);
    }
    else
    if((m_mode != new_mode)
    && (new_mode == BufferMode::COMMAND_INPUT)) {
        this->height -= CMDLINE_HEIGHT;
        U::clear_screen();
    }

    if((m_mode == BufferMode::COMMAND_INPUT)
    && (new_mode != BufferMode::COMMAND_INPUT)) {
        this->height += CMDLINE_HEIGHT;
        U::clear_screen();
    }

    m_mode = new_mode;
}
   
void Buffer::selectreg_action(void(*act_callback)(Buffer*, const std::string&, Int64x2)) {
    if(this->get_mode() != BufferMode::SELECT) {
        log_print(WARNING, "Buffer mode is not SELECT but this function was called.");
        return;
    }


    SelectReg reg = m_select;

    if(reg.startY > reg.endY) {
        reg.startX += 1;
        U::iswap64(&reg.startY, &reg.endY);
        U::iswap64(&reg.startX, &reg.endX);
    }
    else
    if(reg.startY == reg.endY
    && reg.startX > reg.endX) {
        reg.startX += 1;
        U::iswap64(&reg.startX, &reg.endX);
    }
    
    std::string tmp;
    tmp.reserve(1024);


    if(reg.startY == reg.endY) {    
        std::string* ln = getlnstr(reg.startY);

        act_callback(this,
                ln->substr(reg.startX, reg.endX - reg.startX),
                (Int64x2){ reg.startX, reg.startY }
                );

        return;
    }

    for(int64_t y = reg.startY; y <= reg.endY; y++) {
        std::string* ln = getlnstr(y);
        tmp = *ln;

        size_t v_index = 0;
        size_t v_size = tmp.size();
        int64_t x = 0;

        if(y == reg.startY) {
            x = reg.startX;
            v_index = x;
            v_size = ln->size() - x;
        }
        else
        if(y == reg.endY) {
            v_index = 0;
            v_size = reg.endX;
            x = 0;
        }

        Int64x2 ln_pos = (Int64x2){ x, y };
        act_callback(this, tmp.substr(v_index, v_size), ln_pos);
    }


}



