#include <stdio.h>
#include <cstring>


#include "editor.hpp"
#include "util.hpp"



void Editor::init() {
    start_color();
    m_init_all_colors();
  
    
    m_color_pair_num = 0;

    this->last_key_input = 0;
    this->running = true;
    this->buf = add_buffer("Test Buffer");

}

void Editor::init_style() {

    this->settn.style.TL_corner_ch = U::get_cchar(0x250C);
    this->settn.style.TR_corner_ch = U::get_cchar(0x2510);
    this->settn.style.BL_corner_ch = U::get_cchar(0x2514);
    this->settn.style.BR_corner_ch = U::get_cchar(0x2518);
    this->settn.style.T_line_ch = U::get_cchar(0x2500);
    this->settn.style.R_line_ch = U::get_cchar(0x2502);
    this->settn.style.L_line_ch = U::get_cchar(0x2502);
    this->settn.style.B_line_ch = U::get_cchar(0x2500);

}


int Editor::m_key_fromstr(const std::string& str) {
    
    size_t ctrl_pos = str.find("<CTRL>");
    bool has_ctrl = (ctrl_pos != std::string::npos && ctrl_pos == 0);

    size_t start_index = 0;
    if(has_ctrl) {
        start_index = 6; // length of "<CTRL>"
    }

    // Read the input key it may be special one like arrow keys or backspace etc..
    char buffer[32+1] = { 0 };
    int buf_i = 0; 

    for(size_t i = start_index; i < str.size(); i++) {
        char c = str[i];
        if(c == 0x20) {
            continue;
        }

        if(!U::is_valid_char(c)) { // Allow only printable characters.
            continue;
        }

        buffer[buf_i] = c;
        buf_i++;
        if(buf_i >= 32) {
            log_print(ERROR, "The input 'str' is too large. Probably caused by invalid input to add_keymap function");
            return 0;
        }
    }

    int result = 0;

    if(buf_i == 1) { 
        /* Single character input */
        result = (int)buffer[0];
    }
    else {
        if(strcmp(buffer, "<LEFT>") == 0) {
            result = KEY_LEFT;
        }
        else
        if(strcmp(buffer, "<RIGHT>") == 0) {
            result = KEY_RIGHT;
        }
        else
        if(strcmp(buffer, "<UP>") == 0) {
            result = KEY_UP;
        }
        else    
        if(strcmp(buffer, "<DOWN>") == 0) {
            result = KEY_DOWN;
        }
        else    
        if(strcmp(buffer, "<BACKSPACE>") == 0) {
            result = KEY_BACKSPACE;
        }
        else    
        if(strcmp(buffer, "<ENTER>") == 0) {
            result = '\n';
        }
        else    
        if(strcmp(buffer, "<TAB>") == 0) {
            result = '\t';
        }
    }


    // terminals would set first 2 bits to 0
    // when control key is held down with input key
    // so that means 1100001 = 'a'
    // and when control key is down 'a' becomes 0000001

    if(has_ctrl) {
        result &= 0x1F; // 0011111
    }

    return result;
}


void Editor::add_keymap(BufferMode mode, const KeyCommand& kc, const std::string& str) {
    this->keymap[mode][m_key_fromstr(str)] = kc;
}

void Editor::map_input_keys() {
    
    add_keymap(BufferMode::INSERT, (KeyCommand){ EditorKey::K_ENTER },     "<ENTER>");
    add_keymap(BufferMode::INSERT, (KeyCommand){ EditorKey::K_BACKSPACE }, "<BACKSPACE>");
    add_keymap(BufferMode::INSERT, (KeyCommand){ EditorKey::K_TAB },       "<TAB>");

    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_LEFT },      "<LEFT>");
    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_RIGHT },     "<RIGHT>");
    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_UP },        "<UP>");
    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_DOWN },      "<DOWN>");

    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_NULLMODE },  "<CTRL> x");
    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_INSERTMODE },"<CTRL> l");
    add_keymap(BufferMode::SHARED, (KeyCommand){ EditorKey::K_SELECTMODE },"<CTRL> s");

    add_keymap(BufferMode::NULLMODE, (KeyCommand){ EditorKey::K_UPDATE_SCRN }, "U");

    add_keymap(BufferMode::NULLMODE, (KeyCommand){ EditorKey::K_LEFT },   "j");
    add_keymap(BufferMode::NULLMODE, (KeyCommand){ EditorKey::K_RIGHT },  "l");
    add_keymap(BufferMode::NULLMODE, (KeyCommand){ EditorKey::K_UP },     "i");
    add_keymap(BufferMode::NULLMODE, (KeyCommand){ EditorKey::K_DOWN },   "k");

}


void Editor::m_init_color(int color_num, int r, int g, int b, int bg_color_num) {
    init_color(color_num, r << 2, g << 2, b << 2);
    init_pair(color_num, color_num, bg_color_num);

    // Few next are the darker version of r, g and b.
    
    const float factor = 0.5;
    for(int i = 0; i < Color::NUM_DARK_SHADES; i++) {
        r = (int)((float)r * factor);
        g = (int)((float)g * factor);
        b = (int)((float)b * factor);
        
        color_num++;

        init_color(color_num, r << 2, g << 2, b << 2);
        init_pair(color_num, color_num, bg_color_num);
    }

}
        
void Editor::m_init_all_colors() {
    
    init_color(Color::BACKGROUND, 47, 43, 40);
    
    m_init_color(Color::WHITE, 200, 180, 150);
    m_init_color(Color::BEIGE, 200, 160, 120);
    m_init_color(Color::PINK, 230, 130, 180);
    m_init_color(Color::RED, 200, 40, 40);
    m_init_color(Color::ORANGE, 240, 80, 45);
    m_init_color(Color::YELLOW, 240, 180, 60);
    m_init_color(Color::LIME, 180, 230, 32);
    m_init_color(Color::GREEN, 50, 200, 50);
    m_init_color(Color::CYAN, 56, 200, 180);
    m_init_color(Color::BLUE, 40, 160, 230);
    m_init_color(Color::PURPLE, 180, 90, 240);
    m_init_color(Color::MAGENTA, 240, 60, 190);

    m_init_color(Color::CURSOR, 40, 200, 40, Color::DARK_GREEN_1);
}

Buffer* Editor::add_buffer(const char* name) {

    Buffer buffer = Buffer(name);
    buffer.index = buffers.size();
    this->buffers.push_back(buffer);

    return &this->buffers[buffers.size()-1];
}

void Editor::del_buffer(Buffer* buf) {
    printf("%s NOT IMPLEMENTED YET.\n", __func__);
}
        
void Editor::set_color(int color) {
    attron(COLOR_PAIR(color));
    m_color_pair_num = color;
}

void Editor::reset_color() {
    attroff(COLOR_PAIR(m_color_pair_num));
}

        
void Editor::update_buffer_areas() {

    // TODO: Add support for multiple buffers.

    this->buf->pos_x = 0;
    this->buf->pos_y = 0;
    this->buf->width = this->term_width / 2;
    this->buf->height = this->term_height - 2;

    this->buf->checkup_scrnbuf();
}





