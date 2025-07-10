#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>
#include <cstdint>



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


struct Select {
    Cursor start;
    Cursor end;

    Select(const Cursor& cur) {
        start = cur;
        end = cur;
    }
};


// TODO: Rename to BufferLine
struct Line {
    std::string  str;
    uint8_t      force_update; // Clear the whole line before drawing.
};


struct ScreenLine {
    uint64_t      length;
    uint64_t prev_length;
};

enum BufferMode : int {
    NULLMODE = 0,
    INSERT,
    SELECT,
    
    SHARED, // Special for key input handling.

    NUM_MODES
};


class Editor;

class Buffer {
    public:

        Buffer(const char* name);
        ~Buffer();
        void free_memory();

        BufferMode mode;

        uint64_t index; // Index in Editor::buffers array.

        uint16_t paddn_x;  // Number of empty space at start of X and Y
        uint16_t paddn_y;  //
    
        int pos_x;
        int pos_y;
        int width;
        int height;

        uint8_t tab_width;

        std::vector<Line> data;
        std::string name;
        
        void draw(Editor* bite);
        void checkup_scrnbuf();

        // ---- General Utilities ----

        Line*        getln(int64_t y);
        std::string* getlnstr(int64_t y);

        void clamp_cursor_xy();


        // ---- Data Control ----
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

        // ---- Input Related ----

        void handle_enter();
        void handle_backspace();

        Cursor  cursor;
        int64_t scroll;
           

    private:
        
        bool m_mem_freed;
        void m_draw_borders(Editor* bite, int base_x, int base_y);
        bool m_clear_last_row;

        inline int m_max_row();

        void m_draw_title_info(Editor* bite, int x, int y, const char* info, int color);


        // The screen buffer (scrnbuf) is responsible to
        // update lines which width is smaller than in previous update.
        // it will keep track of the widths and update them accordingly.
        ScreenLine* m_scrnbuf;
        size_t      m_scrnbuf_size;
        
        uint32_t     m_prev_lastln_digits;
};





#endif
