#include <lua.hpp>

#include "command.hpp"
#include "editor.hpp"
#include "log.hpp"


void Command::execute(Editor* bite, Buffer* buf, const std::string& cmd) {    
    if(cmd.empty()) { return; }

    log_print(INFO, "'%s'", buf->cmd.c_str());

    if(luaL_dostring(bite->lua, cmd.c_str())) {
        log_print(ERROR, "%s", lua_tostring(bite->lua, -1));
    }

}
