#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>


class Buffer;


namespace Command
{
    void execute(Buffer* buf, const std::string& cmd);
};






#endif
