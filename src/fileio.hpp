#ifndef BITE_FILEIO_HPP
#define BITE_FILEIO_HPP


class Buffer;

namespace FileIO {

    void save_buffer(Buffer* buf);
    void read_to_buffer(Buffer* buf, const char* filename);



};




#endif
