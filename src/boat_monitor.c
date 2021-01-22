//========================================================================
// GLFW 3.4 Boat - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
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

#include "internal.h"

#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Poll for changes in the set of connected monitors
//
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
    _glfwFreeGammaArrays(&monitor->boat.ramp);
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
    
    ANativeWindow* window = boatGetNativeWindow();
    
    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
    if (width)
        *width = ANativeWindow_getWidth(window);
    if (height)
        *height = ANativeWindow_getHeight(window);
}

GLFWvidmode* _glfwPlatformGetVideoModes(_GLFWmonitor* monitor, int* count)
{
    GLFWvidmode* result;

    *count = 1;
    result = calloc(1, sizeof(GLFWvidmode));
    _glfwPlatformGetVideoMode(monitor, result);
    
    return result;
}

void _glfwPlatformGetVideoMode(_GLFWmonitor* monitor, GLFWvidmode* mode)
{
    ANativeWindow* window = boatGetNativeWindow();
    
    mode->width = ANativeWindow_getWidth(window);
    mode->height = ANativeWindow_getHeight(window);
    mode->redBits = 8;
    mode->greenBits = 8;
    mode->blueBits = 8;
    mode->refreshRate = 60;
}

GLFWbool _glfwPlatformGetGammaRamp(_GLFWmonitor* monitor, GLFWgammaramp* ramp)
{
    if (!monitor->boat.ramp.size)
    {
        _glfwAllocGammaArrays(&monitor->boat.ramp, 256);

        for (unsigned int i = 0;  i < monitor->boat.ramp.size;  i++)
        {
            const float gamma = 2.2f;
            float value;
            value = i / (float) (monitor->boat.ramp.size - 1);
            value = powf(value, 1.f / gamma) * 65535.f + 0.5f;
            value = _glfw_fminf(value, 65535.f);

            monitor->boat.ramp.red[i]   = (unsigned short) value;
            monitor->boat.ramp.green[i] = (unsigned short) value;
            monitor->boat.ramp.blue[i]  = (unsigned short) value;
        }
    }

    _glfwAllocGammaArrays(ramp, monitor->boat.ramp.size);
    memcpy(ramp->red,   monitor->boat.ramp.red,   sizeof(short) * ramp->size);
    memcpy(ramp->green, monitor->boat.ramp.green, sizeof(short) * ramp->size);
    memcpy(ramp->blue,  monitor->boat.ramp.blue,  sizeof(short) * ramp->size);
    return GLFW_TRUE;
}

void _glfwPlatformSetGammaRamp(_GLFWmonitor* monitor, const GLFWgammaramp* ramp)
{
    if (monitor->boat.ramp.size != ramp->size)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "Boat: Gamma ramp size must match current ramp size");
        return;
    }

    memcpy(monitor->boat.ramp.red,   ramp->red,   sizeof(short) * ramp->size);
    memcpy(monitor->boat.ramp.green, ramp->green, sizeof(short) * ramp->size);
    memcpy(monitor->boat.ramp.blue,  ramp->blue,  sizeof(short) * ramp->size);
}
