#include <ncurses.h>
#include <stdlib.h>
#include <cstring>

#include "buffer.hpp"
#include "editor.hpp"
#include "util.hpp"



static const char* BUFFER_MODES_STR[] = {
    "NULLMODE",
    "INSERT",
    "SELECT"
};



Buffer::Buffer(const char* name) {
    this->cursor = Cursor();
    this->name = name;
    this->scroll = 0;
    this->paddn_x = 2;
    this->paddn_y = 1;
    this->width = -1;
    this->height = -1;
    this->tab_width = 3;

    this->mode = BufferMode::INSERT;

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
        free(m_scrnbuf);
        m_scrnbuf = NULL;
    }

    m_mem_freed = true;
}

void Buffer::m_draw_borders(Editor* bite, int base_x, int base_y) {
    

    bite->set_color(Color::DARK_CYAN_1);
    // Top left corner.
    mvadd_wch(base_y, base_x, &bite->settn.style.TL_corner_ch);
    
    // Top right corner.
    mvadd_wch(base_y, base_x + (this->width)+1, &bite->settn.style.TR_corner_ch);
    
    // Bottom left corner.
    mvadd_wch(base_y + this->height+1, base_x, &bite->settn.style.BL_corner_ch);
    
    // Bottom right corner.
    mvadd_wch(base_y + this->height+1, base_x + (this->width)+1, &bite->settn.style.BR_corner_ch);
  
    // Top.
    mvhline_set(
            base_y,
            base_x + 1,
            &bite->settn.style.T_line_ch,
            this->width
            );

    // Bottom.
    mvhline_set(
            base_y + this->height + 1,
            base_x + 1,
            &bite->settn.style.B_line_ch,
            this->width
            );
   
    // Left.
    mvvline_set(
            base_y+1,
            base_x,
            &bite->settn.style.L_line_ch,
            this->height
            );

    // Right.
    mvvline_set(
            base_y+1,
            base_x + this->width + 1,
            &bite->settn.style.L_line_ch,
            this->height
            );



    // Draw buffer info.

    int info_x = base_x + this->width - this->name.size() - 1;

    m_draw_title_info(bite, info_x, this->pos_y, this->name.c_str(),
            Color::DARK_CYAN_0);

    if(this->mode != BufferMode::NULLMODE) {
        char mode_str[16] = { 0 };
        info_x -= get_mode_str(mode_str, 16) + 3;

        m_draw_title_info(bite, info_x, this->pos_y, mode_str, Color::DARK_PINK_0);

    }
}
        
void Buffer::m_draw_title_info(Editor* bite, int x, int y, const char* info, int color) {

    const size_t info_size = strlen(info);
   
    bite->set_color(Color::DARK_CYAN_1);

    mvaddch(y, x, '[');
    mvaddch(y, x + info_size+1, ']');

    bite->set_color(color);
    mvprintw(y, x+1, info);

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


    m_draw_borders(bite, this->pos_x, this->pos_y);

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
            ln->force_update = 0;
        }
        else
        if(m_scrnbuf[y].prev_length > m_scrnbuf[y].length) {
            U::clear_part(this->pos_x+1, base_y+y, this->width);
        }

        // Line number.
        // Clear small part where the line number is.
        // It will avoid making confusing looking numbers when scrolling up
        // and the number of digits in the line number changes
        U::clear_part(this->pos_x+1, base_y+y, lastln_digits);
        bite->set_color(Color::DARK_WHITE_1);
        uint64_t ln_digits = U::count_digits(idx);
        mvprintw(base_y + y,
                this->pos_x + (lastln_digits - ln_digits) + 1,
                "%li", idx);

        // Line data.
        bite->set_color(Color::WHITE);
        mvprintw(base_y + y, base_x, ln->str.c_str());
        y++;
    }


    if(m_clear_last_row) {
        m_clear_last_row = false;
        U::clear_part(this->pos_x+1, base_y + data.size() - this->scroll, this->width);
    }


    // Draw cursor.
    bite->set_color(Color::CURSOR);
    mvprintw(base_y + this->cursor.y - this->scroll,
             base_x + this->cursor.x,
             "%c", this->cursor.chr);
    bite->reset_color();


    m_prev_lastln_digits = lastln_digits;

    //U::draw_info("Key: %i", bite->last_key_input);

}
        
size_t Buffer::get_mode_str(char* outbuf, size_t outbuf_size) {
    const char* mode_str = BUFFER_MODES_STR[this->mode];
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
        fprintf(stderr, "%s: buffer size is not set.\n",
                __func__);
        return;
    }

    const size_t new_scrnbuf_size = this->height * sizeof *m_scrnbuf;

    if(!m_scrnbuf) {
        m_scrnbuf = (ScreenLine*)malloc(new_scrnbuf_size);
        if(!m_scrnbuf) {
            fprintf(stderr, "%s: Failed to allocate %li bytes for \"scrnbuf\"\n",
                    __func__, new_scrnbuf_size);
            return;
        }
    }
    else // TODO: Maybe handle a case where user would resize from huge array to very small.
         //       to free some memory.
    if(m_scrnbuf_size < new_scrnbuf_size) {
        ScreenLine* tmpptr = (ScreenLine*)realloc(m_scrnbuf, new_scrnbuf_size);
        if(!tmpptr) {
            fprintf(stderr, "%s: Failed to re-allocate %li bytes for \"scrnbuf\"\n",
                    __func__, new_scrnbuf_size);
            return;
        }
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
        ln->force_update = 1;
    }
    if(this->cursor.py != this->cursor.y) {
        getln(this->cursor.py)->force_update = 1;
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
    y = U::iclamp64(y, 0, data.size()-1);
    data.insert(data.begin() + y+1,
    (Line){ 
        .str = str,
        .force_update = 0
    });
}

void Buffer::del_line(int64_t y) {
    m_clear_last_row = true;
    y = U::iclamp64(y, 0, data.size()-1);

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
        add_line(this->cursor.y, ln->str.substr(this->cursor.x, size));
        
        ln = getln(cursor.y); // Update pointer after insert.
        ln->str.resize(this->cursor.x);
        mov_cursor_to(0, this->cursor.y+1);
    }
    else {
        add_line(this->cursor.y, "");
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


