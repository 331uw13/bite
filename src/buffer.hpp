#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <span>

#include "selectreg_act.hpp"
#include "color.hpp"

static inline constexpr auto CMDLINE_HEIGHT = 2;
static inline constexpr auto CMDLINE_MAX = 64;
static inline constexpr auto MAX_MSG_ROWS = 16;


struct Cursor {
    int64_t x;
    int64_t y;
    int64_t px; // Previous X
    int64_t py; // Previous Y
    char chr;   // Current character
    
    Cursor() {
        x = 0;
        y = 0;
        px = 0;
        py = 0;
        chr = 0x20;
    }

    Cursor(int64_t X, int64_t Y) {
        x = X;
        y = Y;
        px = X;
        py = Y;
        chr = 0x20;
    }
};


struct SelectReg {
    int64_t startX;
    int64_t startY;
    int64_t endX;
    int64_t endY;

    SelectReg() {
        endX = startX = 0;
        endY = startY = 0;
    }

    SelectReg(const Cursor& cur) {
        endX = startX = cur.x;
        endY = startY = cur.y;
    }

    SelectReg(int64_t sX, int64_t sY,
              int64_t eX, int64_t eY
    ){
        startX = sX;
        startY = sY;
        endX = eX;
        endY = eY;
    }
};


// TODO: Rename to BufferLine ? 
struct Line {
    std::string  str;
    bool         force_update; // Clear the whole line before drawing.
};

struct ScreenLine {
    uint64_t      length;
    uint64_t prev_length;
};


enum BufferMode : int {
    NULLMODE = 0,
    INSERT,
    SELECT,
    COMMAND_INPUT,
    
    SHARED, // Special for key input handling.

    NUM_MODES
};


class Editor;

class Buffer {
    public:

        Buffer(const char* name);
        ~Buffer();
        void free_memory();

        void draw(Editor* bite);
        

        uint64_t index;   // Index in Editor::buffers array.
        uint16_t paddn_x; // Number of empty space at start of X and Y
        uint16_t paddn_y; //
        int pos_x;
        int pos_y;
        int width;
        int height;
        bool msg_visible;

        int last_draw_base_x;
        int last_draw_base_y;
        int last_draw_lndigits;

        uint8_t tab_width;

        std::vector<Line> data;
        std::string name;
        std::string cmd;
        int16_t     cmd_cur_x;

        // See 'selectreg_act.hpp' for more info about this:
        void selectreg_action(void(*act_callback)(Buffer* buf, const std::string&, Int64x2));

        void set_msg       (const std::string& new_msg);
        void set_msg_title (const std::string& msg_title);
        void set_msg_color (ColorT color);

        void checkup_scrnbuf();
        void clamp_cursor_xy();


        Line*        getln(int64_t y);
        std::string* getlnstr(int64_t y);


        void mov_cursor_to(int64_t x, int64_t y);
        void mov_cursor(int xoff, int yoff);
        
        // Warning: these functions trust that cursor x and y are not out of bounds.
        bool add_char(const Cursor& cur, char c);
        bool del_char(const Cursor& cur);
        void add_line(int64_t y, const std::string& str); // str can be empty.
        void del_line(int64_t y);
        bool add_tabs(const Cursor& cur, int num);
       
        // Returns the mode string size.
        size_t get_mode_str(char* buf, size_t buf_size);


        void handle_enter();
        void handle_backspace();

        Cursor  cursor;
        int64_t scroll;

        void              set_mode(BufferMode new_mode);
        void              set_previous_mode();
        inline BufferMode get_mode() { return m_mode; }


    private:

        SelectReg  m_select;
        BufferMode m_mode;
        BufferMode m_mode_prev;

        // Message.
        struct m__msg_t {
            int         num_rows;
            std::string data[MAX_MSG_ROWS];
            std::string title;
            ColorT      color;
        } m_msg;

        bool m_mem_freed;
        bool m_clear_last_row;

        inline int m_max_row();

        //void m_draw_command_line(Editor* bite);
        void m_draw_message(Editor* bite);

        // The screen buffer (scrnbuf) is responsible to
        // update lines which width is bigger than in previous update.
        ScreenLine* m_scrnbuf;
        size_t      m_scrnbuf_size;
        
        uint32_t     m_prev_lastln_digits;
};





#endif
