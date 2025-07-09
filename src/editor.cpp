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
    int k = 0;
    char char_key = 0;
    
    size_t ctrl_pos = str.find("<CTRL>");
    bool has_ctrl = (ctrl_pos != std::string::npos && ctrl_pos == 0);

    size_t start_index = 0;
    if(has_ctrl) {
        start_index = 6; // length of "<CTRL>"
    }

    for(size_t i = start_index; i < str.size(); i++) {
        char c = str[i];
        if(c == 0x20) { continue; }
        if(U::is_valid_char(c)) {
            char_key = c;
            break;
        }
    }


    // terminals would set first 2 bits to 0
    // when control key is held down with input key
    // so that means 1100001 = 'a'
    // and when control key is down 'a' becomes 0000001

    k = (int)char_key;
    if(has_ctrl) {
        k &= 0x1F; // 0011111
    }

    return k;
}


void Editor::add_keymap(BufferMode mode, const KeyCommand& kc, const std::string& str) {
    this->keymap[mode][m_key_fromstr(str)] = kc;
}

void Editor::map_input_keys() {

    add_keymap(BufferMode::INSERT, (KeyCommand){ EditorKey::K_LEFT }, "<CTRL> L");

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

    this->buf->pos_x = 32;
    this->buf->pos_y = 3;
    this->buf->width = this->term_width / 3;
    this->buf->height = 32;

    this->buf->checkup_scrnbuf();
}





