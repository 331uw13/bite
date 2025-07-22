#ifndef BITE_FILEIO_HPP
#define BITE_FILEIO_HPP


class Buffer;

namespace FileIO {

    enum READ_OPT {
        DONT_TRUNC_BUFFER,
        TRUNC_BUFFER
    };

    void save_buffer(Buffer* buf);
    void read_to_buffer(Buffer* buf, const char* filename, READ_OPT);


};




#endif
