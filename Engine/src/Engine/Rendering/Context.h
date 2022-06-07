#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Timestep.h"

namespace eng
{
	class Context
	{
	public:
		// TODO: for non-Windows systems and non-OpenGL renderers,
		// is passing the native window what they do for rendering contexts?
		static Scope<Context> CreateScope(void* pNativeWindow);
		virtual ~Context() = default;
	public:
		virtual void SwapBuffers() = 0;
	public:
		virtual const char* GetVendor() const = 0;
		virtual const char* GetRenderer() const = 0;
		virtual const char* GetVersion() const = 0;
	};
}
