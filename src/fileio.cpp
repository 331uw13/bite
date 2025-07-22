
#include <iostream>
#include <fstream>
#include <string>

#include "fileio.hpp"
#include "buffer.hpp"
#include "log.hpp"


void FileIO::save_buffer(Buffer* buf) {
}


void FileIO::read_to_buffer(Buffer* buf, const char* filename, READ_OPT opt) {
    if(buf == NULL) {
        log_print(ERROR, "Buffer is NULL.");
        return;
    }

    if(opt == READ_OPT::TRUNC_BUFFER) {
        buf->data.clear();
    }

    buf->name = filename;    
    std::fstream file(filename, std::ios_base::in);
    if(!file.is_open()) {
        if(opt == READ_OPT::TRUNC_BUFFER) {
            buf->add_line(0, "");
        }
        return;
    }

    std::string line;
    
    while(std::getline(file, line)) {
        buf->add_line(buf->data.size(), line);
    }

    file.close();
}




