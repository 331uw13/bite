#include "command.hpp"
#include "buffer.hpp"
#include "log.hpp"


void Command::execute(Buffer* buf, const std::string& cmd) {    

    log_print(INFO, "'%s'", buf->cmd.c_str());


}
