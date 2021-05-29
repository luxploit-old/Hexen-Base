#pragma once
#include "Invoker.hpp"
#include "ClassPointers.hpp"
#include "return_spoof.hpp"

void* Invoker::GetHandler(std::uint64_t hash) {
	for (auto reg = ClassPointers::cPatterns->pNativeReg[hash & 0xFF]; reg; reg = reg->GetNextRegistration())
	{
		for (std::uint32_t i = 0, end = reg->GetNumEntries(); i < end; ++i)
		{
			if (reg->GetHash(i) == hash)
			{
				return reg->m_Handlers[i];
			}
		}
	}

	return nullptr;
}

void Invoker::BeginCall() {
	m_Context.Reset();
}

void Invoker::EndCall(std::uint64_t hash) {
	if (auto newHash = GetNewHash(hash); newHash.has_value()) {
		if (auto handler = GetHandler(*newHash)) {
			try {
				spoof_call(ClassPointers::cPatterns->pRegisterSpoof, 
					static_cast<void(*)(rage::scrNativeCallContext*)>(handler), 
					reinterpret_cast<rage::scrNativeCallContext*>(&m_Context));
				ClassPointers::cPatterns->pFixVectors(&m_Context);
			}
			catch (...) {
				printfnl("Failed to Invoke Native %p", hash);
			}
		}
		else
			printfnl("Failed to find Handler for Native %p", hash);
	}
	else
		printfnl("Failed to find current hash for Native %p", hash);
}