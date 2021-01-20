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
#include <string.h>
#include <stdio.h>
#include <unistd.h>


// Create key code translation tables
//
static void createKeyTables(void)
{
    int scancode;

    memset(_glfw.boat.keycodes, -1, sizeof(_glfw.boat.keycodes));
    memset(_glfw.boat.scancodes, -1, sizeof(_glfw.boat.scancodes));

    _glfw.boat.keycodes[BOAT_KEY_GRAVE]      = GLFW_KEY_GRAVE_ACCENT;
    _glfw.boat.keycodes[BOAT_KEY_1]          = GLFW_KEY_1;
    _glfw.boat.keycodes[BOAT_KEY_2]          = GLFW_KEY_2;
    _glfw.boat.keycodes[BOAT_KEY_3]          = GLFW_KEY_3;
    _glfw.boat.keycodes[BOAT_KEY_4]          = GLFW_KEY_4;
    _glfw.boat.keycodes[BOAT_KEY_5]          = GLFW_KEY_5;
    _glfw.boat.keycodes[BOAT_KEY_6]          = GLFW_KEY_6;
    _glfw.boat.keycodes[BOAT_KEY_7]          = GLFW_KEY_7;
    _glfw.boat.keycodes[BOAT_KEY_8]          = GLFW_KEY_8;
    _glfw.boat.keycodes[BOAT_KEY_9]          = GLFW_KEY_9;
    _glfw.boat.keycodes[BOAT_KEY_0]          = GLFW_KEY_0;
    _glfw.boat.keycodes[BOAT_KEY_SPACE]      = GLFW_KEY_SPACE;
    _glfw.boat.keycodes[BOAT_KEY_MINUS]      = GLFW_KEY_MINUS;
    _glfw.boat.keycodes[BOAT_KEY_EQUAL]      = GLFW_KEY_EQUAL;
    _glfw.boat.keycodes[BOAT_KEY_Q]          = GLFW_KEY_Q;
    _glfw.boat.keycodes[BOAT_KEY_W]          = GLFW_KEY_W;
    _glfw.boat.keycodes[BOAT_KEY_E]          = GLFW_KEY_E;
    _glfw.boat.keycodes[BOAT_KEY_R]          = GLFW_KEY_R;
    _glfw.boat.keycodes[BOAT_KEY_T]          = GLFW_KEY_T;
    _glfw.boat.keycodes[BOAT_KEY_Y]          = GLFW_KEY_Y;
    _glfw.boat.keycodes[BOAT_KEY_U]          = GLFW_KEY_U;
    _glfw.boat.keycodes[BOAT_KEY_I]          = GLFW_KEY_I;
    _glfw.boat.keycodes[BOAT_KEY_O]          = GLFW_KEY_O;
    _glfw.boat.keycodes[BOAT_KEY_P]          = GLFW_KEY_P;
    _glfw.boat.keycodes[BOAT_KEY_LEFTBRACE]  = GLFW_KEY_LEFT_BRACKET;
    _glfw.boat.keycodes[BOAT_KEY_RIGHTBRACE] = GLFW_KEY_RIGHT_BRACKET;
    _glfw.boat.keycodes[BOAT_KEY_A]          = GLFW_KEY_A;
    _glfw.boat.keycodes[BOAT_KEY_S]          = GLFW_KEY_S;
    _glfw.boat.keycodes[BOAT_KEY_D]          = GLFW_KEY_D;
    _glfw.boat.keycodes[BOAT_KEY_F]          = GLFW_KEY_F;
    _glfw.boat.keycodes[BOAT_KEY_G]          = GLFW_KEY_G;
    _glfw.boat.keycodes[BOAT_KEY_H]          = GLFW_KEY_H;
    _glfw.boat.keycodes[BOAT_KEY_J]          = GLFW_KEY_J;
    _glfw.boat.keycodes[BOAT_KEY_K]          = GLFW_KEY_K;
    _glfw.boat.keycodes[BOAT_KEY_L]          = GLFW_KEY_L;
    _glfw.boat.keycodes[BOAT_KEY_SEMICOLON]  = GLFW_KEY_SEMICOLON;
    _glfw.boat.keycodes[BOAT_KEY_APOSTROPHE] = GLFW_KEY_APOSTROPHE;
    _glfw.boat.keycodes[BOAT_KEY_Z]          = GLFW_KEY_Z;
    _glfw.boat.keycodes[BOAT_KEY_X]          = GLFW_KEY_X;
    _glfw.boat.keycodes[BOAT_KEY_C]          = GLFW_KEY_C;
    _glfw.boat.keycodes[BOAT_KEY_V]          = GLFW_KEY_V;
    _glfw.boat.keycodes[BOAT_KEY_B]          = GLFW_KEY_B;
    _glfw.boat.keycodes[BOAT_KEY_N]          = GLFW_KEY_N;
    _glfw.boat.keycodes[BOAT_KEY_M]          = GLFW_KEY_M;
    _glfw.boat.keycodes[BOAT_KEY_COMMA]      = GLFW_KEY_COMMA;
    _glfw.boat.keycodes[BOAT_KEY_DOT]        = GLFW_KEY_PERIOD;
    _glfw.boat.keycodes[BOAT_KEY_SLASH]      = GLFW_KEY_SLASH;
    _glfw.boat.keycodes[BOAT_KEY_BACKSLASH]  = GLFW_KEY_BACKSLASH;
    _glfw.boat.keycodes[BOAT_KEY_ESC]        = GLFW_KEY_ESCAPE;
    _glfw.boat.keycodes[BOAT_KEY_TAB]        = GLFW_KEY_TAB;
    _glfw.boat.keycodes[BOAT_KEY_LEFTSHIFT]  = GLFW_KEY_LEFT_SHIFT;
    _glfw.boat.keycodes[BOAT_KEY_RIGHTSHIFT] = GLFW_KEY_RIGHT_SHIFT;
    _glfw.boat.keycodes[BOAT_KEY_LEFTCTRL]   = GLFW_KEY_LEFT_CONTROL;
    _glfw.boat.keycodes[BOAT_KEY_RIGHTCTRL]  = GLFW_KEY_RIGHT_CONTROL;
    _glfw.boat.keycodes[BOAT_KEY_LEFTALT]    = GLFW_KEY_LEFT_ALT;
    _glfw.boat.keycodes[BOAT_KEY_RIGHTALT]   = GLFW_KEY_RIGHT_ALT;
    _glfw.boat.keycodes[BOAT_KEY_LEFTMETA]   = GLFW_KEY_LEFT_SUPER;
    _glfw.boat.keycodes[BOAT_KEY_RIGHTMETA]  = GLFW_KEY_RIGHT_SUPER;
    _glfw.boat.keycodes[BOAT_KEY_MENU]       = GLFW_KEY_MENU;
    _glfw.boat.keycodes[BOAT_KEY_NUMLOCK]    = GLFW_KEY_NUM_LOCK;
    _glfw.boat.keycodes[BOAT_KEY_CAPSLOCK]   = GLFW_KEY_CAPS_LOCK;
    _glfw.boat.keycodes[BOAT_KEY_PRINT]      = GLFW_KEY_PRINT_SCREEN;
    _glfw.boat.keycodes[BOAT_KEY_SCROLLLOCK] = GLFW_KEY_SCROLL_LOCK;
    _glfw.boat.keycodes[BOAT_KEY_PAUSE]      = GLFW_KEY_PAUSE;
    _glfw.boat.keycodes[BOAT_KEY_DELETE]     = GLFW_KEY_DELETE;
    _glfw.boat.keycodes[BOAT_KEY_BACKSPACE]  = GLFW_KEY_BACKSPACE;
    _glfw.boat.keycodes[BOAT_KEY_ENTER]      = GLFW_KEY_ENTER;
    _glfw.boat.keycodes[BOAT_KEY_HOME]       = GLFW_KEY_HOME;
    _glfw.boat.keycodes[BOAT_KEY_END]        = GLFW_KEY_END;
    _glfw.boat.keycodes[BOAT_KEY_PAGEUP]     = GLFW_KEY_PAGE_UP;
    _glfw.boat.keycodes[BOAT_KEY_PAGEDOWN]   = GLFW_KEY_PAGE_DOWN;
    _glfw.boat.keycodes[BOAT_KEY_INSERT]     = GLFW_KEY_INSERT;
    _glfw.boat.keycodes[BOAT_KEY_LEFT]       = GLFW_KEY_LEFT;
    _glfw.boat.keycodes[BOAT_KEY_RIGHT]      = GLFW_KEY_RIGHT;
    _glfw.boat.keycodes[BOAT_KEY_DOWN]       = GLFW_KEY_DOWN;
    _glfw.boat.keycodes[BOAT_KEY_UP]         = GLFW_KEY_UP;
    _glfw.boat.keycodes[BOAT_KEY_F1]         = GLFW_KEY_F1;
    _glfw.boat.keycodes[BOAT_KEY_F2]         = GLFW_KEY_F2;
    _glfw.boat.keycodes[BOAT_KEY_F3]         = GLFW_KEY_F3;
    _glfw.boat.keycodes[BOAT_KEY_F4]         = GLFW_KEY_F4;
    _glfw.boat.keycodes[BOAT_KEY_F5]         = GLFW_KEY_F5;
    _glfw.boat.keycodes[BOAT_KEY_F6]         = GLFW_KEY_F6;
    _glfw.boat.keycodes[BOAT_KEY_F7]         = GLFW_KEY_F7;
    _glfw.boat.keycodes[BOAT_KEY_F8]         = GLFW_KEY_F8;
    _glfw.boat.keycodes[BOAT_KEY_F9]         = GLFW_KEY_F9;
    _glfw.boat.keycodes[BOAT_KEY_F10]        = GLFW_KEY_F10;
    _glfw.boat.keycodes[BOAT_KEY_F11]        = GLFW_KEY_F11;
    _glfw.boat.keycodes[BOAT_KEY_F12]        = GLFW_KEY_F12;
    _glfw.boat.keycodes[BOAT_KEY_F13]        = GLFW_KEY_F13;
    _glfw.boat.keycodes[BOAT_KEY_F14]        = GLFW_KEY_F14;
    _glfw.boat.keycodes[BOAT_KEY_F15]        = GLFW_KEY_F15;
    _glfw.boat.keycodes[BOAT_KEY_F16]        = GLFW_KEY_F16;
    _glfw.boat.keycodes[BOAT_KEY_F17]        = GLFW_KEY_F17;
    _glfw.boat.keycodes[BOAT_KEY_F18]        = GLFW_KEY_F18;
    _glfw.boat.keycodes[BOAT_KEY_F19]        = GLFW_KEY_F19;
    _glfw.boat.keycodes[BOAT_KEY_F20]        = GLFW_KEY_F20;
    _glfw.boat.keycodes[BOAT_KEY_F21]        = GLFW_KEY_F21;
    _glfw.boat.keycodes[BOAT_KEY_F22]        = GLFW_KEY_F22;
    _glfw.boat.keycodes[BOAT_KEY_F23]        = GLFW_KEY_F23;
    _glfw.boat.keycodes[BOAT_KEY_F24]        = GLFW_KEY_F24;
    _glfw.boat.keycodes[BOAT_KEY_KPSLASH]    = GLFW_KEY_KP_DIVIDE;
    _glfw.boat.keycodes[BOAT_KEY_KPDOT]      = GLFW_KEY_KP_MULTIPLY;
    _glfw.boat.keycodes[BOAT_KEY_KPMINUS]    = GLFW_KEY_KP_SUBTRACT;
    _glfw.boat.keycodes[BOAT_KEY_KPPLUS]     = GLFW_KEY_KP_ADD;
    _glfw.boat.keycodes[BOAT_KEY_KP0]        = GLFW_KEY_KP_0;
    _glfw.boat.keycodes[BOAT_KEY_KP1]        = GLFW_KEY_KP_1;
    _glfw.boat.keycodes[BOAT_KEY_KP2]        = GLFW_KEY_KP_2;
    _glfw.boat.keycodes[BOAT_KEY_KP3]        = GLFW_KEY_KP_3;
    _glfw.boat.keycodes[BOAT_KEY_KP4]        = GLFW_KEY_KP_4;
    _glfw.boat.keycodes[BOAT_KEY_KP5]        = GLFW_KEY_KP_5;
    _glfw.boat.keycodes[BOAT_KEY_KP6]        = GLFW_KEY_KP_6;
    _glfw.boat.keycodes[BOAT_KEY_KP7]        = GLFW_KEY_KP_7;
    _glfw.boat.keycodes[BOAT_KEY_KP8]        = GLFW_KEY_KP_8;
    _glfw.boat.keycodes[BOAT_KEY_KP9]        = GLFW_KEY_KP_9;
    _glfw.boat.keycodes[BOAT_KEY_KPCOMMA]    = GLFW_KEY_KP_DECIMAL;
    _glfw.boat.keycodes[BOAT_KEY_KPEQUAL]    = GLFW_KEY_KP_EQUAL;
    _glfw.boat.keycodes[BOAT_KEY_KPENTER]    = GLFW_KEY_KP_ENTER;

    for (scancode = BOAT_MIN_SCANCODE;  scancode <= BOAT_MAX_SCANCODE;  scancode++)
    {
        // Store the reverse translation for faster key name lookup
        if (_glfw.boat.keycodes[scancode] > 0)
            _glfw.boat.scancodes[_glfw.boat.keycodes[scancode]] = scancode;
    }

}

// Retrieve system content scale via folklore heuristics
//
static void getSystemContentScale(float* xscale, float* yscale)
{

    *xscale = 1.f;
    *yscale = 1.f;
}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Reports the specified error, appending information about the last Boat error
//
void _glfwInputErrorBoat(int error, const char* message)
{
    _glfwInputError(error, "%s: %s", message, "Some unknown error occurred.");
}

// Creates a native cursor object from the specified image and hotspot
//
void* _glfwCreateCursorBoat(const GLFWimage* image, int xhot, int yhot)
{
    return NULL;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformInit(void)
{

    _glfw.boat.boatlib.handle = _glfw_dlopen("libboat.so");

    if (!_glfw.boat.boatlib.handle)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "Boat: Failed to load libboat.so");
        return GLFW_FALSE;
    }
    
    _glfw.boat.boatlib.GetNativeWindow = (PFN_BoatGetNativeWindow)
        _glfw_dlsym(_glfw.boat.boatlib.handle, "boatGetNativeWindow");
    _glfw.boat.boatlib.GetNativeDisplay = (PFN_BoatGetNativeDisplay)
        _glfw_dlsym(_glfw.boat.boatlib.handle, "boatGetNativeDisplay");
    _glfw.boat.boatlib.SetCurrentEventProcessor = (PFN_BoatSetCurrentEventProcessor)
        _glfw_dlsym(_glfw.boat.boatlib.handle, "boatSetCurrentEventProcessor");
    _glfw.boat.boatlib.GetCurrentEvent = (PFN_BoatGetCurrentEvent)
        _glfw_dlsym(_glfw.boat.boatlib.handle, "boatGetCurrentEvent");
    _glfw.boat.boatlib.SetCursorMode = (PFN_BoatSetCursorMode)
        _glfw_dlsym(_glfw.boat.boatlib.handle, "boatSetCursorMode");
        
    _glfw.boat.androidlib.handle = _glfw_dlopen("libandroid.so");

    if (!_glfw.boat.androidlib.handle)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "Boat: Failed to load libandroid.so");
        return GLFW_FALSE;
    }
    
    _glfw.boat.androidlib.ANativeWindowGetWidth = (PFN_ANativeWindowGetWidth)
        _glfw_dlsym(_glfw.boat.androidlib.handle, "ANativeWindow_getWidth");
    _glfw.boat.androidlib.ANativeWindowGetHeight = (PFN_ANativeWindowGetHeight)
        _glfw_dlsym(_glfw.boat.androidlib.handle, "ANativeWindow_getHeight");

    // EGL_DEFAULT_DISPLAY = 0.
    _glfw.boat.display = boatGetNativeDisplay();
    
    _glfw.boat.screen = 0;

    getSystemContentScale(&_glfw.boat.contentScaleX, &_glfw.boat.contentScaleY);

    // the keyboard mapping.
    createKeyTables();
    
    _glfwInitTimerPOSIX();

    _glfwPollMonitorsBoat();
    return GLFW_TRUE;
}

void _glfwPlatformTerminate(void)
{
   
    _glfwTerminateEGL();

    if (_glfw.boat.boatlib.handle)
    {
        _glfw_dlclose(_glfw.boat.boatlib.handle);
        _glfw.boat.boatlib.handle = NULL;
    }
}

const char* _glfwPlatformGetVersionString(void)
{
    return _GLFW_VERSION_NUMBER " Android Boat EGL OSMesa"
#if defined(_POSIX_TIMERS) && defined(_POSIX_MONOTONIC_CLOCK)
        " clock_gettime"
#else
        " gettimeofday"
#endif
#if defined(__linux__)
        " evdev"
#endif
#if defined(_GLFW_BUILD_DLL)
        " shared"
#endif
        ;
}

