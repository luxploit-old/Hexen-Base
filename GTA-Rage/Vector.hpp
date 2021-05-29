#pragma once
#include <cstdint>

namespace rage {
	struct Vector2 {
		float x{};
		float y{};
	};

	struct Vector3 {
		float x{};
		float y{};
		float z{};
	};

	struct Vector4 {
		float x;
		float y;
		float z;
		float w;
	};

	struct NativeVector3 {
	public:
		float x{};
	private:
		char paddingX[4];
	public:
		float y{};
	private:
		char paddingY[4];
	public:
		float z{};
	private:
		char paddingZ[4];
	};

	struct NativeVector4 {
		alignas(uintptr_t) float x;
		alignas(uintptr_t) float y;
		alignas(uintptr_t) float z;
		alignas(uintptr_t) float w;
	};
}