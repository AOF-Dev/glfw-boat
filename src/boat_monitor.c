//========================================================================
// GLFW 3.3 Boat - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
// Copyright (c) 2019-2021 Cosine Math <boat_jvm@qq.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================
//
//========================================================================
// This file is derived from x11_monitor.c
//========================================================================

#include "internal.h"

#include <stdlib.h>
#include <string.h>


static void modeChangeHandle(int width, int height, void* data)
{
    GLFWvidmode mode;
    _GLFWmonitor* monitor = data;

    mode.width = width;
    mode.height = height;
    mode.redBits = 8;
    mode.greenBits = 8;
    mode.blueBits = 8;
    mode.refreshRate = 0;

    monitor->modeCount++;
    monitor->modes =
        realloc(monitor->modes, monitor->modeCount * sizeof(GLFWvidmode));
    monitor->modes[monitor->modeCount - 1] = mode;

    monitor->boat.currentMode = monitor->modeCount - 1;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

void _glfwPollMonitorsBoat(void)
{
    ANativeWindow* window = boatGetNativeWindow();
    const float dpi = 141.f;
    _GLFWmonitor* monitor = _glfwAllocMonitor("Boat Monitor 0",
                                              (int) (ANativeWindow_getWidth(window) * 25.4f / dpi),
                                              (int) (ANativeWindow_getHeight(window) * 25.4f / dpi));
    _glfwInputMonitor(monitor, GLFW_CONNECTED, _GLFW_INSERT_FIRST);
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

void _glfwPlatformFreeMonitor(_GLFWmonitor* monitor)
{
}

void _glfwPlatformGetMonitorPos(_GLFWmonitor* monitor, int* xpos, int* ypos)
{
    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
}

void _glfwPlatformGetMonitorContentScale(_GLFWmonitor* monitor,
                                         float* xscale, float* yscale)
{
    if (xscale)
        *xscale = _glfw.boat.contentScaleX;
    if (yscale)
        *yscale = _glfw.boat.contentScaleY;
}

void _glfwPlatformGetMonitorWorkarea(_GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height)
{
    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
    if (width)
        *width = monitor->modes[monitor->boat.currentMode].width;
    if (height)
        *height = monitor->modes[monitor->boat.currentMode].height;
}

GLFWvidmode* _glfwPlatformGetVideoModes(_GLFWmonitor* monitor, int* count)
{
    if (monitor->modes == NULL || monitor->modeCount == 0) {
        ANativeWindow* window = boatGetNativeWindow();
        modeChangeHandle(ANativeWindow_getWidth(window),
                         ANativeWindow_getHeight(window),
                         monitor);
    }
    *count = monitor->modeCount;
    return monitor->modes;
}

void _glfwPlatformGetVideoMode(_GLFWmonitor* monitor, GLFWvidmode* mode)
{
    if (monitor->modes == NULL || monitor->modeCount == 0) {
        ANativeWindow* window = boatGetNativeWindow();
        modeChangeHandle(ANativeWindow_getWidth(window),
                         ANativeWindow_getHeight(window),
                         monitor);
    }
    *mode = monitor->modes[monitor->boat.currentMode];
}

GLFWbool _glfwPlatformGetGammaRamp(_GLFWmonitor* monitor, GLFWgammaramp* ramp)
{
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "Boat: Gamma ramp access not supported");
    return GLFW_FALSE;
}

void _glfwPlatformSetGammaRamp(_GLFWmonitor* monitor, const GLFWgammaramp* ramp)
{
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "Boat: Gamma ramp access not supported");
}

