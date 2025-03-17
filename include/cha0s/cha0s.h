#ifndef __CHA0S_H__
#define __CHA0S_H__

#include <stdint.h>

#define CHA0S_INCLUDE_D3D9   0 // 1 if you need D3D9 hook
#define CHA0S_INCLUDE_D3D10  0 // 1 if you need D3D10 hook
#define CHA0S_INCLUDE_D3D11  1 // 1 if you need D3D11 hook
#define CHA0S_INCLUDE_D3D12  0 // 1 if you need D3D12 hook
#define CHA0S_INCLUDE_OPENGL 0 // 1 if you need OpenGL hook
#define CHA0S_INCLUDE_VULKAN 0 // 1 if you need Vulkan hook
#define CHA0S_USE_MINHOOK    1 // 1 if you will use cha0s::bind function

#define CHA0S_ARCH_X64 0
#define CHA0S_ARCH_X86 0

#if defined(_M_X64)	
# undef  CHA0S_ARCH_X64
# define CHA0S_ARCH_X64 1
#else
# undef  CHA0S_ARCH_X86
# define CHA0S_ARCH_X86 1
#endif

#if CHA0S_ARCH_X64
typedef uint64_t uint150_t;
#else
typedef uint32_t uint150_t;
#endif

namespace cha0s
{

	struct statuses
	{
		enum enums
		{
			UnknownError = -1,
			NotSupportedError = -2,
			ModuleNotFoundError = -3,

			AlreadyInitializedError = -4,
			NotInitializedError = -5,

			Success = 0,
		};
	};

	struct rendertypes
	{
		enum enums
		{
			None,

			D3D9,
			D3D10,
			D3D11,
			D3D12,

			OpenGL,
			Vulkan,

			Auto
		};
	};

	statuses::enums init(rendertypes::enums renderType);
	void shutdown();

	statuses::enums bind(uint16_t index, void** original, void* function);
	void unbind(uint16_t index);

	rendertypes::enums getRenderType();
	uint150_t* getMethodsTable();
}

#endif // __CHA0S_H__