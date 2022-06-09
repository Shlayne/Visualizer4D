#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Rendering/Context.h"
#include <glm/glm.hpp>
#include <string_view>

namespace eng
{
	struct WindowSpecifications
	{
		sint32 width = 1280; // Framebuffer width.
		sint32 height = 720; // Framebuffer height.
		std::string_view title = "Untitled Window";

		bool vsync : 1 = true;
		bool resizable : 1 = true;
		bool mouseCaptured : 1 = false;
		bool decorated : 1 = true;

		bool fullscreenOnShow : 1 = false;
		bool focusOnShow : 1 = true;
		bool maximizeOnShow : 1 = false;
	};

	class Window
	{
	public:
		static Scope<Window> CreateScope(const WindowSpecifications& crSpecs, const Scope<Window>& crShareContextWindow);
		virtual ~Window() = default;
	public:
		virtual sint32 GetWidth() const = 0;
		virtual sint32 GetHeight() const = 0;
		virtual glm::s32vec2 GetSize() const = 0;

		virtual sint32 GetFramebufferWidth() const = 0;
		virtual sint32 GetFramebufferHeight() const = 0;
		virtual glm::s32vec2 GetFramebufferSize() const = 0;
	public:
		virtual bool IsFocused() const = 0;
		virtual bool ContainsMouse() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(std::string_view title) = 0;

		//virtual void SetIcon(const Ref<LocalTexture2D>& icon) = 0;
	public:
		virtual bool IsVsyncEnabled() const = 0;
		virtual void SetVsync(bool vsync) = 0;
		virtual void ToggleVSync() = 0;

		virtual bool IsResizable() const = 0;
		virtual void SetResizable(bool resizable) = 0;
		virtual void ToggleResizable() = 0;

		virtual bool IsDecorated() const = 0;
		virtual void SetDecorated(bool decorated) = 0;
		virtual void ToggleDecorated() = 0;

		virtual bool IsFullscreen() const = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual void ToggleFullscreen() = 0;

		virtual bool IsMouseCaptured() const = 0;
		virtual void SetMouseCapture(bool mouseCaptured) = 0;
		virtual void ToggleMouseCapture() = 0;
	public:
		virtual Context& GetContext() = 0;
		virtual const Context& GetContext() const = 0;
		virtual void* GetNativeWindow() = 0;
		virtual const void* GetNativeWindow() const = 0;
	public:
		virtual bool ShouldClose() const = 0;
		virtual void Close() = 0;
	protected:
		static void OnEvent(Event& rEvent);
	};
}
