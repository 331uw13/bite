#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>
#include <ncurses.h>
#include <string>

namespace U {

    int64_t iclamp64(int64_t i, int64_t min, int64_t max);
    int     iclamp32(int i, int min, int max);
    bool is_valid_char(char c); // <- TODO: maybe rename this?

    void draw_info(const char* info_fmt, ...);
    void clear_part(int x, int y, int width);

    cchar_t get_cchar(int u);
    uint32_t count_digits(uint64_t number);


    // --- String utilities ---

    // Iteration direction for functions.
    enum IDIRECT : int {
        LEFT = -1,
        RIGHT = 1
    };


    // Count whitespace from index until non-whitespace character
    // is found or index goes out of bounds for str.
    // NOTE: when direction is to the left, index is decremented by 1
    //       this ensures that only changing the direction will result in correct results.
    uint32_t count_whitespace(
            const std::string* str, int64_t index,
            enum IDIRECT direction);

    bool str_contains(const std::string& str, const char* part);



};

#endif
