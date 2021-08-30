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
// This file is derived from x11_platform.h
//========================================================================

#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <dlfcn.h>

#include <boat.h>

typedef VkFlags VkAndroidSurfaceCreateFlagsKHR;

typedef struct VkAndroidSurfaceCreateInfoKHR
{
    VkStructureType                   sType;
    const void*                       pNext;
    VkAndroidSurfaceCreateFlagsKHR    flags;
    struct ANativeWindow*             window;
} VkAndroidSurfaceCreateInfoKHR;

typedef VkResult (APIENTRY *PFN_vkCreateAndroidSurfaceKHR)(VkInstance, const VkAndroidSurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

#include "posix_thread.h"
#include "posix_time.h"
#include "egl_context.h"
#include "osmesa_context.h"
#include "null_joystick.h"

#define _glfw_dlopen(name) dlopen(name, RTLD_LAZY | RTLD_LOCAL)
#define _glfw_dlclose(handle) dlclose(handle)
#define _glfw_dlsym(handle, name) dlsym(handle, name)

#define _GLFW_EGL_NATIVE_WINDOW  ((EGLNativeWindowType) window->boat.handle)
#define _GLFW_EGL_NATIVE_DISPLAY EGL_DEFAULT_DISPLAY

#define _GLFW_PLATFORM_WINDOW_STATE         _GLFWwindowBoat  boat
#define _GLFW_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryBoat boat
#define _GLFW_PLATFORM_MONITOR_STATE        _GLFWmonitorBoat boat
#define _GLFW_PLATFORM_CURSOR_STATE         _GLFWcursorBoat  boat

#define _GLFW_PLATFORM_CONTEXT_STATE         struct { int dummyContext; }
#define _GLFW_PLATFORM_LIBRARY_CONTEXT_STATE struct { int dummyLibraryContext; }

// Boat-specific per-window data
//
typedef struct _GLFWwindowBoat
{
    ANativeWindow*  handle;

    GLFWbool        overrideRedirect;
    GLFWbool        iconified;
    GLFWbool        maximized;

    // Whether the visual supports framebuffer transparency
    GLFWbool        transparent;

    // Cached position and size used to filter out duplicate events
    int             width, height;
    int             xpos, ypos;

    // The last received cursor position, regardless of source
    int             lastCursorPosX, lastCursorPosY;
    // The last position the cursor was warped to by GLFW
    int             warpCursorPosX, warpCursorPosY;

} _GLFWwindowBoat;

// Boat-specific global data
//
typedef struct _GLFWlibraryBoat
{
    // System content scale
    float           contentScaleX, contentScaleY;
    // Key name string
    char            keynames[GLFW_KEY_LAST + 1][5];
    // Boat keycode to GLFW key LUT
    short int       keycodes[256];
    // GLFW key to Boat keycode LUT
    short int       scancodes[GLFW_KEY_LAST + 1];
    // Where to place the cursor when re-enabled
    double          restoreCursorPosX, restoreCursorPosY;
    // The window whose disabled cursor mode is active
    _GLFWwindow*    disabledCursorWindow;
    // The window receiving and processing events
    _GLFWwindow*    eventCurrent;

} _GLFWlibraryBoat;

// Boat-specific per-monitor data
//
typedef struct _GLFWmonitorBoat
{
    // Current monitor mode index
    int             currentMode;

} _GLFWmonitorBoat;

// Boat-specific per-cursor data
//
typedef struct _GLFWcursorBoat
{
    // Useless struct filler
    void* handle;

} _GLFWcursorBoat;


void _glfwPollMonitorsBoat(void);

