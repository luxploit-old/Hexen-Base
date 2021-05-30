#pragma once
#include <memory>
#include "Hooks.hpp"
#include "Patterns.hpp"
#include "Invoker.hpp"
#include "Helpers.hpp"
#include "Renderer.hpp"
#include "script_mgr.hpp"
#include "fiber_pool.hpp"
#include "TimerManager.hpp"

/*This is a helper function to call printf with a new line*/
/*Yes i am that lazy, but this is a UC base, not like i care*/
extern void printfnl(const char* msg, ...);

/*Bool that dictates if menu should stop Running*/
inline bool bUnInject = false;

/*Location for all pointers, avoiding Race-Conditions during Init*/
namespace ClassPointers {
	extern std::unique_ptr<Hooks> cHooks;
	extern std::unique_ptr<PatternScan> cPatterns;
	extern std::unique_ptr<Invoker> cInvoker;
	extern std::unique_ptr<Helpers> cHelpers;
	extern std::unique_ptr<DirectRender> cDX;
	extern std::unique_ptr<script_mgr> cScripts;
	extern std::unique_ptr<fiber_pool> cPool;
	extern std::unique_ptr<TimerManager> cTimerMgr;

	void ResetPointers();
}