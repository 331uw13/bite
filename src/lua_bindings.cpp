
#include "lua_bindings.hpp"
#include "editor.hpp"
#include "log.hpp"
#include "util.hpp"

    
int LuaBind::print(lua_State* L) {
    Editor& bite = Editor::Instance();
    const char* data = luaL_checkstring(L, 1);

    bite.buf->msg = data;
    log_print(INFO, "%s", data);

    return 1;
}

int LuaBind::gotoln(lua_State* L) {
    Editor& bite = Editor::Instance();
    int line = luaL_checkinteger(L, 1);

    if((line < 0) || ((size_t)line > bite.buf->data.size())) {
        U::clear_screen();
        line = bite.buf->data.size();
        bite.buf->scroll -= bite.buf->height / 2;
    }

    bite.buf->mov_cursor_to(bite.buf->cursor.x, line);


    return 1;
}

    
int LuaBind::buf_size(lua_State* L) {
    Editor& bite = Editor::Instance();

    lua_pushinteger(L, bite.buf->data.size());

    return 1;
}


