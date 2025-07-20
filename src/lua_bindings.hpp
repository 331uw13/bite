#ifndef LUA_BINDINGS_HPP
#define LUA_BINDINGS_HPP

#include <lua.hpp>


namespace LuaBind
{
    int gotoln(lua_State* L);
    int buf_size(lua_State* L);
};




#endif
