#ifndef AI_LUAHEADERS_HDR
#define AI_LUAHEADERS_HDR

#if (AI_LUA_LIB_EXT == 1)
#include <lua5.1/lua.hpp>
#else
// include Spring's own Lua-library headers
#include "lib/lua/include/lua.h"
#include "lib/lua/include/lualib.h"
#include "lib/lua/include/lauxlib.h"
#endif

#endif
