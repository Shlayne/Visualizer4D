#pragma once

#include "Engine/Core/Core.h"

namespace eng
{
	struct CommandLineArgs;

	class RendererAPI
	{
	public:
		// TODO: api functions.
	public:
		enum class API : uint8 { None, OpenGL, Vulkan };
		static inline API GetAPI() { return s_API; }
		static bool SupportsAPI(API api);
	private:
		friend int Main(CommandLineArgs args);
		static bool SetAPI(API api);
		static void LoadAPI();
		static void UnloadAPI();
	private:
		friend class Renderer;
		static Scope<RendererAPI> CreateScope();
	private:
		static API s_API;
	};
}

#define UNKNOWN_RENDERER_API(api, ...) \
	default: \
		CORE_ASSERT(false, "Unknown or Unsupported Renderer API ({0})!", static_cast<uint8>(api)); \
		return __VA_ARGS__ // Despite being va args, this is only for one return value.
