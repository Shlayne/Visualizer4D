#pragma once

#include "Engine/Core/Core.h"

namespace eng
{
	struct CommandLineArgs;

	using RendererPrimitive = uint8;
	enum : RendererPrimitive
	{
		// OpenGL, Vulkan, Direct3D, and Metal support these
		RendererPrimitive_Points,
		RendererPrimitive_Lines,
		RendererPrimitive_LineStrip,
		RendererPrimitive_Triangles,
		RendererPrimitive_TriangleStrip,

		// OpenGL, Vulkan, and Direct3D support these
		RendererPrimitive_LinesAdjacency,
		RendererPrimitive_LineStripAdjacency,
		RendererPrimitive_TrianglesAdjacency,
		RendererPrimitive_TriangleStripAdjacency,

		// OpenGL and Vulkan support these
		RendererPrimitive_LineLoop,
		RendererPrimitive_TriangleFan,

		RendererPrimitive_Count
	};

	class RendererAPI
	{
	public:
		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void EnableBlending() = 0;
		virtual void DisableBlending() = 0;
		virtual void EnableCulling() = 0;
		virtual void DisableCulling() = 0;

		virtual void SetViewport(const glm::s32vec2& position, const glm::s32vec2& size) = 0;

		virtual void Clear() = 0;
		virtual void ClearDepth() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		//virtual void DrawIndexed() = 0; // TODO
	public: // Renderer Capabilities.
		virtual sint32 GetMaxTextureSlots() = 0;
		virtual sint32 GetMaxTextureSize() = 0;
		virtual sint32 GetMaxFramebufferWidth() = 0;
		virtual sint32 GetMaxFramebufferHeight() = 0;
		virtual sint32 GetMaxFramebufferColorAttachments() = 0;
	public:
		using API = uint8;
		enum : API { API_None, API_Vulkan };
		static API GetAPI();
		static bool SupportsAPI(API api);
	private:
		friend int Main(CommandLineArgs args);
		static bool SetAPI(API api);
		static void LoadAPI();
		static void UnloadAPI();
	private:
		friend class Renderer;
		static Scope<RendererAPI> CreateScope();
	public:
		// Public for Scope.
		virtual ~RendererAPI() = default;
	};
}

#define UNKNOWN_RENDERER_API(api, ...) \
	default: \
		CORE_ASSERT(false, "Unknown or unsupported Renderer API ({0})!", (api)); \
		return __VA_ARGS__	// Despite being va args, this is only for one return value.
							// It's for there being a return value or not.
