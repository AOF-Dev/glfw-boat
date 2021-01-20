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

#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <dlfcn.h>

#include <boat.h>


typedef ANativeWindow* (* PFN_BoatGetNativeWindow)();
typedef void* (* PFN_BoatGetNativeDisplay)();
typedef void (* PFN_BoatSetCurrentEventProcessor)(BoatEventProcessor);
typedef void (* PFN_BoatGetCurrentEvent)(BoatInputEvent*);
typedef void (* PFN_BoatSetCursorMode)(int);
#define boatGetNativeWindow _glfw.boat.boatlib.GetNativeWindow
#define boatGetNativeDisplay _glfw.boat.boatlib.GetNativeDisplay
#define boatSetCurrentEventProcessor _glfw.boat.boatlib.SetCurrentEventProcessor
#define boatGetCurrentEvent _glfw.boat.boatlib.GetCurrentEvent
#define boatSetCursorMode _glfw.boat.boatlib.SetCursorMode

typedef int (* PFN_ANativeWindowGetWidth)(ANativeWindow*);
typedef int (* PFN_ANativeWindowGetHeight)(ANativeWindow*);
#define ANativeWindow_getWidth _glfw.boat.androidlib.ANativeWindowGetWidth
#define ANativeWindow_getHeight _glfw.boat.androidlib.ANativeWindowGetHeight

#include "posix_thread.h"
#include "posix_time.h"
#include "null_joystick.h"

#define _glfw_dlopen(name) dlopen(name, RTLD_LAZY | RTLD_LOCAL)
#define _glfw_dlclose(handle) dlclose(handle)
#define _glfw_dlsym(handle, name) dlsym(handle, name)

#define _GLFW_PLATFORM_WINDOW_STATE         _GLFWwindowBoat  boat
#define _GLFW_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryBoat boat
#define _GLFW_PLATFORM_MONITOR_STATE        _GLFWmonitorBoat boat
#define _GLFW_PLATFORM_CURSOR_STATE         _GLFWcursorBoat  boat


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

    // The time of the last KeyPress event per keycode, for discarding
    // duplicate key events generated for some keys by ibus
    long long            keyPressTimes[256];

} _GLFWwindowBoat;

// Boat-specific global data
//
typedef struct _GLFWlibraryBoat
{
    void*        display;
    
    // System content scale
    float           contentScaleX, contentScaleY;
    // Most recent error code received by Boat error handler
    int             errorCode;
    // Primary selection string (while the primary selection is owned)
    char*           primarySelectionString;
    // Clipboard string (while the selection is owned)
    char*           clipboardString;
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

    struct {
        void*       handle;
        PFN_BoatGetNativeDisplay GetNativeDisplay;
        PFN_BoatGetNativeWindow GetNativeWindow;
        PFN_BoatSetCurrentEventProcessor SetCurrentEventProcessor;
        PFN_BoatGetCurrentEvent GetCurrentEvent;
        PFN_BoatSetCursorMode SetCursorMode;
    } boatlib;
    
    struct {
        void*       handle;
        PFN_ANativeWindowGetWidth ANativeWindowGetWidth;
        PFN_ANativeWindowGetHeight ANativeWindowGetHeight;
    } androidlib;


} _GLFWlibraryBoat;

// Boat-specific per-monitor data
//
typedef struct _GLFWmonitorBoat
{

    // Index of screen. Useless
    int             index;

} _GLFWmonitorBoat;

// Boat-specific per-cursor data
//
typedef struct _GLFWcursorBoat
{
    void* handle;

} _GLFWcursorBoat;

void _glfwPollMonitorsBoat(void);
void _glfwInputErrorBoat(int error, const char* message);
