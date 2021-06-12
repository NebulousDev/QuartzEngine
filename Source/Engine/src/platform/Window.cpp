#include "Window.h"

namespace Quartz
{
    Window::Window(const WindowInfo& info)
        : mInfo(info)
    {
        // Nothing
    }

    Int32 Window::GetX()
    {
        return mInfo.position.x;
    }

    Int32 Window::GetY()
    {
        return mInfo.position.y;
    }

    UInt32 Window::GetWidth()
    {
        return mInfo.size.x;
    }

    UInt32 Window::GetHeight()
    {
        return mInfo.size.y;
    }
}

