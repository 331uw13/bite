#ifndef SELECTREG_ACTION_HPP
#define SELECTREG_ACTION_HPP

#include <cstdint>
#include <string>
#include <string_view>

class Buffer;

struct Int64x2 {
    int64_t x;
    int64_t y;
};


namespace SelectRegAct
{
    // buf:     buffer which called this function.
    // ln:      data being processed.
    // ln_pos:  data position in the buffer.
    
    // NOTE: These functions can only be called through: Buffer::selectreg_act()
  

    void draw   (Buffer* buf, const std::string& ln, Int64x2 ln_pos);


};




#endif
