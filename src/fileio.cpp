
#include <iostream>
#include <fstream>
#include <string>

#include "fileio.hpp"
#include "buffer.hpp"
#include "log.hpp"


void FileIO::save_buffer(Buffer* buf) {
}


void FileIO::read_to_buffer(Buffer* buf, const char* filename) {
    if(buf == NULL) {
        log_print(ERROR, "Buffer is NULL.");
        return;
    }

    buf->data.clear();
    
    std::fstream file(filename, std::ios_base::in);
    std::string line;
    
    while(std::getline(file, line)) {
        buf->add_line(buf->data.size(), line);
    }

    file.close();
}




