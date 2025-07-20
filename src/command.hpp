#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>


class Buffer;
class Editor;

namespace Command
{
    void execute(Editor* bite, Buffer* buf, const std::string& cmd);
};






#endif
