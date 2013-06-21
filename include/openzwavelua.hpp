/*
 *  Open Z-Wave for Lua
 *  Copyright (C) 2013  justin@dynam.ac
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Created on: 15 Feb, 2013
 *      Author: fish
 */

#ifndef laubindings_hpp
#define laubindings_hpp

#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <boost/function.hpp>
#include <Manager.h>


enum logLevel {log_debug, log_info, log_warning, log_critical};

typedef boost::function<void (logLevel x, std::string)> logCallBack;

class OZW4Lua {
	public:
 	  OZW4Lua();
	  OZW4Lua(logCallBack cb);
	  ~OZW4Lua();
	  static OZW4Lua *Self;
	  static lua_State *getLuaState() { /* assert(OZW4Lua::Self); */ return Self->L;};
	  void setLogCB(logCallBack cb);
	  bool InitManager(OpenZWave::Manager *Manager, std::string filename);
	  bool DoNotification(OpenZWave::Notification const *);
	  std::string getVersion();
	protected:
          lua_State* L;
	private:
	void doLog(logLevel, std::string);
	logCallBack logcb;
};


#endif
