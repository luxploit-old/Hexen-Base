#pragma once
#include "Crossmap.hpp"
#include <optional>
#include "GTA-Rage/Vector.hpp"
#include "Patterns.hpp"
#include "GTA-Rage/ScrNative.hpp"
#include "return_spoof.hpp" 

struct NativeContext : public rage::scrNativeCallContext
{
	NativeContext()
	{
		Reset();
	}

	void Reset()
	{
		m_Return = &m_ReturnData[0];
		m_ArgCount = 0;
		m_Args = &m_ArgumentData[0];
		m_DataCount = 0;
		std::fill(std::begin(m_VectorSpace), std::end(m_VectorSpace), '\0');

		std::fill(std::begin(m_ReturnData), std::end(m_ReturnData), 0);
		std::fill(std::begin(m_ArgumentData), std::end(m_ArgumentData), 0);
	}

	template <typename T>
	void Push(T value)
	{
		static_assert(sizeof(T) <= 8);

		std::uintptr_t data{};
		*(T*)(&data) = value;
		m_ArgumentData[m_ArgCount++] = data;
	}

	template <>
	void Push<rage::NativeVector3>(rage::NativeVector3 value)
	{
		Push(value.x);
		Push(value.y);
		Push(value.z);
	}

	template <typename T>
	T GetReturnValue()
	{
		return *static_cast<T*>(m_Return);
	}

	template <>
	void GetReturnValue<void>()
	{
	}
private:
	std::uintptr_t m_ReturnData[10] = {};
	std::uintptr_t m_ArgumentData[100] = {};
};

class Invoker {
public:
	void* GetHandler(std::uint64_t hash);

	void BeginCall();

	template <typename T>
	void Push(T&& value) {
		m_Context.Push(std::forward<T>(value));
	}

	template <typename T>
	T GetReturn() {
		return m_Context.GetReturnValue<T>();
	}

	std::optional<std::uint64_t> GetNewHash(std::uint64_t oldHash)
	{
		for (auto it = std::begin(g_Crossmap); it != std::end(g_Crossmap); ++it)
			if (it->m_Old == oldHash)
				return it->m_New;

		return std::nullopt;
	}

	void EndCall(std::uint64_t hash);
private:
	NativeContext m_Context;
};

