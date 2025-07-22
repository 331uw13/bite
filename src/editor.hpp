#ifndef EDITOR_HPP
#define EDITOR_HPP


#include <vector>
#include <cstdint>
#include <map>


#include "input.hpp"
#include "buffer.hpp"
#include "log.hpp"
#include "color.hpp"
#include "fileio.hpp"
#include "editor_draw.hpp"



struct StyleSettings {     // Buffer border style unicode.
    cchar_t TL_corner_ch;  // Top Left
    cchar_t TR_corner_ch;  // Top Right
    cchar_t BL_corner_ch;  // Bottom Left
    cchar_t BR_corner_ch;  // Bottom Right
    cchar_t T_line_ch;
    cchar_t R_line_ch;
    cchar_t L_line_ch;
    cchar_t B_line_ch;
    cchar_t UDR_connect_ch; // Up-Down Connect to Right
    cchar_t UDL_connect_ch; // Up-Down Connect to Left

    char win_infostr_prefix_ch;
    char win_infostr_suffix_ch;
    char cmdline_indicator_ch;

    ColorT win_border_color;
    ColorT win_infostr_affix_color; // Prefix and Suffix.
    ColorT win_name_color;
    ColorT buf_modestr_color;
    ColorT cmdline_color;
    ColorT cmdline_indicator_color;
    ColorT msg_info_color;
    ColorT msg_error_color;
};



struct Settings {
    StyleSettings style;

    // << Syntax and color settings go here >> 

};


struct lua_State;


class Editor {

    public:

        static Editor& Instance() {
            static Editor i;
            return i;
        }

        // Avoid accidental copies.
        Editor(Editor const&) = delete;
        void operator=(Editor const&) = delete;

        bool running;

        void init();
        void init_style();
        void map_input_keys();

        lua_State* lua;

        // NOTE: add_keymap function expects that the <CTRL> modifier starts at first index.
        void add_keymap(BufferMode mode, const KeyCommand& kc, const std::string& str);
        std::map<int, KeyCommand> keymap [BufferMode::NUM_MODES];

        void add_lua_bindings();


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

        Editor() {}

        int m_color_pair_num;
        void m_init_all_colors();
        void m_pair_colors(int color_num, int r, int g, int b, int bg_color_num);
        void m_init_color(int color_num, int r, int g, int b, int bg_color_num = Color::BACKGROUND);
        int  m_key_fromstr(const std::string& str);


};




#endif
