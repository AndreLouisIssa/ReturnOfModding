#pragma once
#include "log.hpp"
#include "lua/lua_module.hpp"

namespace lua::log
{
	// Lua API: Table
	// Name: log
	// Table containing functions for printing to console / log file.

	// Lua API: Function
	// Table: log
	// Name: info
	// Param: data: string
	// Logs an informational message.
	static void info(const std::string& data, sol::this_environment env)
	{
		LOG(INFO) << big::lua_module::guid_from(env) << ": " << data;
	}

	// Lua API: Function
	// Table: log
	// Name: warning
	// Param: data: string
	// Logs a warning message.
	static void warning(const std::string& data, sol::this_environment env)
	{
		LOG(WARNING) << big::lua_module::guid_from(env) << ": " << data;
	}

	// Lua API: Function
	// Table: log
	// Name: debug
	// Param: data: string
	// Logs a debug message.
	static void debug(const std::string& data, sol::this_environment state)
	{
		sol::environment& env = state;
		LOG(VERBOSE) << big::lua_module::guid_from(env) << ": " << data;
	}

	void bind(sol::state& state)
	{
		auto ns       = state["log"].get_or_create<sol::table>();
		ns["info"]    = info;
		ns["warning"] = warning;
		ns["debug"]   = debug;
	}
}