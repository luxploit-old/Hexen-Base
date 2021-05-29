#pragma once
#include <cstdint>
#include <minwindef.h>
namespace rage {
#pragma warning(push)
#pragma warning(disable : 4324)
	struct scrNativeCallContext {
	public:
		void* m_Return;
		uint32_t m_ArgCount;
		char padding[4];
		void* m_Args;
		uint32_t m_DataCount;
		char padding1[4];
		alignas(uintptr_t) uint8_t m_VectorSpace[192];
	};

	struct scrNativeRegistration
	{
		std::uint64_t m_NextRegistration1;
		std::uint64_t m_NextRegistration2;
		void* m_Handlers[7];
		std::uint32_t m_NumEntries1;
		std::uint32_t m_NumEntries2;
		std::uint64_t m_Hashes;

		inline scrNativeRegistration* GetNextRegistration()
		{
			std::uintptr_t result;
			auto v5 = std::uintptr_t(&m_NextRegistration1);
			auto v12 = 2;
			auto v13 = v5 ^ m_NextRegistration2;
			auto v14 = (char*)&result - v5;
			do
			{
				*(DWORD*)&v14[v5] = static_cast<DWORD>(v13) ^ *(DWORD*)v5;
				v5 += 4;
				--v12;
			} while (v12);

			return reinterpret_cast<scrNativeRegistration*>(result);
		}

		inline std::uint32_t GetNumEntries()
		{
			return static_cast<std::uint32_t>(((std::uintptr_t)&m_NumEntries1) ^ m_NumEntries1 ^ m_NumEntries2);
		}

		inline std::uint64_t GetHash(std::uint32_t index)
		{
			auto naddr = 16 * index + std::uintptr_t(&m_NextRegistration1) + 0x54;
			auto v8 = 2;
			std::uint64_t nResult;
			auto v11 = (char*)&nResult - naddr;
			auto v10 = naddr ^ *(DWORD*)(naddr + 8);
			do
			{
				*(DWORD*)&v11[naddr] = static_cast<DWORD>(v10 ^ *(DWORD*)(naddr));
				naddr += 4;
				--v8;
			} while (v8);

			return nResult;
		}
	};
}