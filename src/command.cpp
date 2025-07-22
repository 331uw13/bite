#include <lua.hpp>

#include "command.hpp"
#include "editor.hpp"
#include "log.hpp"




void Command::execute(Editor* bite, Buffer* buf, const std::string& cmd) {    
    if(cmd.empty()) { return; }
    log_print(INFO, "'%s'", buf->cmd.c_str());

    if(luaL_dostring(bite->lua, cmd.c_str())) {
        log_print(ERROR, "%s", lua_tostring(bite->lua, -1));
        buf->set_msg(lua_tostring(bite->lua, -1));
        buf->set_msg_title("Lua error");
        buf->set_msg_color(bite->settn.style.msg_error_color);
    }
    else {
        buf->set_msg_title(cmd);
        buf->set_msg_color(bite->settn.style.msg_info_color);
    }

}
