
#include "lua_bindings.hpp"
#include "editor.hpp"
#include "log.hpp"
#include "util.hpp"


int LuaBind::test(lua_State* L) {
    Editor& bite = Editor::Instance();

    return 1;
}


    
int LuaBind::print(lua_State* L) {
    Editor& bite = Editor::Instance();
    const char* data = luaL_checkstring(L, 1);

    bite.buf->set_msg(data);
    log_print(INFO, "msg = '%s'", data);

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
    bite.buf->set_previous_mode();
    return 1;
}

    
int LuaBind::buf_size(lua_State* L) {
    Editor& bite = Editor::Instance();

    lua_pushinteger(L, bite.buf->data.size());

    return 1;
}

    
int LuaBind::hide_msg(lua_State* L) {
    Editor& bite = Editor::Instance();
    bite.buf->msg_visible = false;    
    U::clear_screen();
    return 1;
}

int LuaBind::show_msg(lua_State* L) {
    Editor& bite = Editor::Instance();
    bite.buf->msg_visible = true;
    return 1;
}
    
int LuaBind::open_file(lua_State* L) { 
    Editor& bite = Editor::Instance();
    const char* filename = luaL_checkstring(L, 1);

    log_print(INFO, "'%s'", filename);
    FileIO::read_to_buffer(bite.buf, filename, FileIO::TRUNC_BUFFER);
    U::clear_screen();

    bite.buf->set_previous_mode();
    return 1;
}

int LuaBind::get_file(lua_State* L) { 
    Editor& bite = Editor::Instance();
    const char* filename = luaL_checkstring(L, 1);

    log_print(INFO, "'%s'", filename);
    FileIO::read_to_buffer(bite.buf, filename, FileIO::DONT_TRUNC_BUFFER);

    bite.buf->set_previous_mode();
    return 1;
}















