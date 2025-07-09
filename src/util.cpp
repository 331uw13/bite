
#include <cstdarg>
#include <cstring>

#include "util.hpp"



int64_t U::iclamp64(int64_t i, int64_t min, int64_t max) {
    return (i < min) ? min : (i > max) ? max : i;
}
    
int U::iclamp32(int i, int min, int max) {
    return (i < min) ? min : (i > max) ? max : i;
}
    
bool U::is_valid_char(char c) {
    return ((c >= 0x20) && (c <= 0x7E));
}
    

void U::draw_info(const char* info_fmt, ...) {
    va_list args;
    va_start(args, info_fmt);

    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);

    char buffer[512] = { 0 };
    vsnprintf(buffer, 512, info_fmt, args);

    mvprintw(rows-1, 0, buffer);

    va_end(args);
}
    
cchar_t U::get_cchar(int u ) {
    return (cchar_t) { .attr = 0, .chars = { u, 0 }, .ext_color = 0 };
}
    
void U::clear_part(int x, int y, int width) {
    mvhline(y, x, ' ', width);
}
    
uint32_t U::count_digits(uint64_t number) {
    if(number == 0) {
        return 1;
    }

    uint32_t i = 0;

    while(number > 0) {
        i++;
        number /= 10;
    }

    return i;
}


uint32_t U::count_whitespace(
        const std::string* str, int64_t index,
        enum IDIRECT direction
){
    uint32_t count = 0;
    if(direction == IDIRECT::LEFT) {
        index--;
    }

    while(true) {
        if(index < 0) {
            break;
        }
        if(index >= (int64_t)str->size()) {
            break;
        }

        if((*str)[index] != 0x20) {
            break;
        }

        count++;
        index += direction;
    }


    return count;
}




