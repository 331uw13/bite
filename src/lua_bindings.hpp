#ifndef LUA_BINDINGS_HPP
#define LUA_BINDINGS_HPP

#include <lua.hpp>


namespace LuaBind
{
    int test(lua_State* L);
    int print(lua_State* L);
    int gotoln(lua_State* L);
    int buf_size(lua_State* L);
    int hide_msg(lua_State* L);
    int show_msg(lua_State* L);
    int open_file(lua_State* L); 
    int get_file(lua_State* L); 
};




#endif
