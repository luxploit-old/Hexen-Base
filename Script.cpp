#pragma once
#include "ClassPointers.hpp"
#include "script.hpp"

void script::script_exception_handler(PEXCEPTION_POINTERS exp) {
	HMODULE mod{};
	DWORD64 offset{};
	char buffer[MAX_PATH]{};
	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCWSTR>(exp->ExceptionRecord->ExceptionAddress), &mod) == TRUE && mod != nullptr) {
		offset = (reinterpret_cast<DWORD64>(exp->ExceptionRecord->ExceptionAddress) - reinterpret_cast<DWORD64>(mod));
		GetModuleFileNameA(mod, buffer, MAX_PATH - 1);
	}
	printfnl("EX-Handler Triggered!");
	printfnl("Script Status Code: [503 -> Bad Gateway]");
	printfnl("EXC: %x | EXO: %x | FNM: %s", exp->ExceptionRecord->ExceptionCode, offset, buffer);

	printfnl("ASM Registers Dump:");
	printfnl("RAX: %u | RCX: %u | RDX: %u | RBX: %u", exp->ContextRecord->Rax, exp->ContextRecord->Rcx, exp->ContextRecord->Rdx, exp->ContextRecord->Rbx);
	printfnl("RSP: %u | RBP: %u | RSI: %u | RDI: %u", exp->ContextRecord->Rsp, exp->ContextRecord->Rbp, exp->ContextRecord->Rsi, exp->ContextRecord->Rdi);
	printfnl("R8: %u  | R9: %u  | R10: %u | R11: %u", exp->ContextRecord->R8, exp->ContextRecord->R9, exp->ContextRecord->R10, exp->ContextRecord->R11);
	printfnl("R12: %u | R13: %u | R14: %u | R15: %u", exp->ContextRecord->R12, exp->ContextRecord->R13, exp->ContextRecord->R14, exp->ContextRecord->R15);
}

script::script(func_t func, std::optional<std::size_t> stack_size) :
	m_func(func),
	m_script_fiber(nullptr),
	m_main_fiber(nullptr) {
	m_script_fiber = CreateFiber(stack_size.has_value() ? stack_size.value() : 0, [](void* param) {
		static_cast<script*>(param)->fiber_func();
	}, this);
}

script::~script() {
	if (m_script_fiber)
		DeleteFiber(m_script_fiber);
}

void script::tick() {
	m_main_fiber = GetCurrentFiber();
	if (!m_wake_time.has_value() || m_wake_time.value() <= std::chrono::high_resolution_clock::now())
		SwitchToFiber(m_script_fiber);
}

void script::yield(std::optional<std::chrono::high_resolution_clock::duration> time) {
	if (time.has_value())
		m_wake_time = std::chrono::high_resolution_clock::now() + time.value();
	else
		m_wake_time = std::nullopt;

	SwitchToFiber(m_main_fiber);
}

script* script::get_current() {
	return static_cast<script*>(GetFiberData());
}

void script::fiber_func() {
	TRY_CLAUSE {
		m_func();
	}
	EXCEPT_CLAUSE
	[]() {
		printfnl("Script Status Code: [409 -> Conflict]");
	}();

	while (true) {
		yield();
	}
}