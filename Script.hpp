#pragma once
#include <Windows.h>
#include <vector>

using ScriptFunc = void(*)();
class Script
{
public:
	inline Script(ScriptFunc function, const char* n)
	{
		scriptFiber = nullptr;
		callbackFunction = function;
		name = n;
		wakeAt = GetTickCount64();
	}
	inline ~Script()
	{
		if (scriptFiber) DeleteFiber(scriptFiber);
	}
	inline ScriptFunc GetCallbackFunction()
	{
		return callbackFunction;
	}
	inline const char* GetName()
	{
		return name;
	}
	void Tick();
	void Wait(DWORD time);
private:
	void Run();
	HANDLE scriptFiber{};
	DWORD wakeAt{};
	ScriptFunc callbackFunction{};
	const char* name{};
};