ACLOCAL_AMFLAGS = -I autotools
bin_PROGRAMS = OZW4Lua
OZW4Lua_SOURCES = Main.cpp
OZW4Lua_CXXFLAGS = -I$(top_srcdir)/include/ -pthread
OZW4Lua_LDADD = $(top_builddir)/src/libozw4lua.la $(LUA_LIB) $(OZW_LIB) $(LUABIND_LIB) $(TXML_LIB) $(UDEV_LIB)
exampledir = $(datarootdir)/doc/@PACKAGE@-@OZW4LUA_MAJOR_VERSION@.@OZW4LUA_MINOR_VERSION@
example_DATA = example.lua
EXTRA_DIST = example.lua