//
//  laubindings.hpp
//  xBeeHome
//
//  Created by Justin Hammond on 23/12/11.
//  Copyright (c) 2011 DynamX. All rights reserved.
//

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
	protected:
          lua_State* L;
	private:
	void doLog(logLevel, std::string);
	logCallBack logcb;
};


#endif
