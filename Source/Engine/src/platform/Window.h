#pragma once

#include "common.h"
#include "util/String.h"
#include "math/Bounds.h"

namespace Quartz
{
	enum WindowMode
	{
		WINDOW_MODE_WINDOWED,
		WINDOW_MODE_BORDERLESS,
		WINDOW_MODE_FULLSCREEN
	};

	struct WindowInfo
	{
		StringW		title;
		Point2i		position;
		Point2u		size;
		Bool8		focused;
		Bool8		shown;
		WindowMode	mode;
	};

	class QUARTZ_API Window
	{
	protected:
		WindowInfo mInfo;

	public:
		Window(const WindowInfo& info);

		virtual void Move(UInt32 x, UInt32 y) = 0;
		virtual void Resize(UInt32 width, UInt32 height) = 0;
		virtual void Focus(Bool8 focus) = 0;
		virtual void Show(Bool8 show) = 0;

		Int32 GetX();
		Int32 GetY();
		UInt32 GetWidth();
		UInt32 GetHeight();
		/*
		Point2i GetPosition();
		Point2i GetBorderedPosition();
		Point2i GetSize();
		Point2i GetBorderedSize();
		Bounds2i GetBounds();
		Bounds2i GetBorderedBounds();
		Bool8 IsFocused();
		Bool8 IsMouseInside();
		Bool8 IsMouseInsideBordered();
		WindowInfo GetInfo();
		*/
	};
}