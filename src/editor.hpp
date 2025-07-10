#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <ncurses.h>

#include <vector>
#include <cstdint>
#include <map>


#include "input.hpp"
#include "buffer.hpp"
#include "log.hpp"
#include "color.hpp"

struct StyleSettings {
    cchar_t TL_corner_ch;  // Top Left
    cchar_t TR_corner_ch;  // Top Right
    cchar_t BL_corner_ch;  // Bottom Left
    cchar_t BR_corner_ch;  // Bottom Right
    cchar_t T_line_ch;
    cchar_t R_line_ch;
    cchar_t L_line_ch;
    cchar_t B_line_ch;
};


struct Settings {
    StyleSettings style;

    // << Syntax and color settings go here >> 

};





class Editor {

    public:
        bool running;

        void init();
        void init_style();
        void map_input_keys();
       

        // NOTE: add_keymap function expects that the <CTRL> modifier starts at first index.
        void add_keymap(BufferMode mode, const KeyCommand& kc, const std::string& str);
        std::map<int, KeyCommand> keymap [BufferMode::NUM_MODES];
        


        void set_color(int color);
        void reset_color();

        // Update size and position of all open buffers.
        void update_buffer_areas();
        
        int term_width;
        int term_height;

        Settings settn;
        
        /*REMOVE*/int last_key_input;

    // ---- Buffer Control ----------------------

        Buffer*  add_buffer (const char* name);
        void     del_buffer (Buffer* buf);

        std::vector<Buffer> buffers;
        Buffer*             buf; // Current buffer pointer to specific index in 'buffers' array

        
    
    private:

        int m_color_pair_num;
        void m_init_all_colors();
        void m_init_color(int color_num, int r, int g, int b, int bg_color_num = Color::BACKGROUND);
        int  m_key_fromstr(const std::string& str);


};




#endif
