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


// Translate the Boat key code for a key to a GLFW key code
// NOTE: It is layout-dependent and will fail partially on most non-US layouts
//
static int translateKeyCode(int keycode)
{
    switch (keycode)
    {
        case BOAT_KEYBOARD_KP_0:           return GLFW_KEY_KP_0;
        case BOAT_KEYBOARD_KP_1:           return GLFW_KEY_KP_1;
        case BOAT_KEYBOARD_KP_2:           return GLFW_KEY_KP_2;
        case BOAT_KEYBOARD_KP_3:           return GLFW_KEY_KP_3;
        case BOAT_KEYBOARD_KP_4:           return GLFW_KEY_KP_4;
        case BOAT_KEYBOARD_KP_5:           return GLFW_KEY_KP_5;
        case BOAT_KEYBOARD_KP_6:           return GLFW_KEY_KP_6;
        case BOAT_KEYBOARD_KP_7:           return GLFW_KEY_KP_7;
        case BOAT_KEYBOARD_KP_8:           return GLFW_KEY_KP_8;
        case BOAT_KEYBOARD_KP_9:           return GLFW_KEY_KP_9;
        case BOAT_KEYBOARD_KP_Separator:
        case BOAT_KEYBOARD_KP_Decimal:     return GLFW_KEY_KP_DECIMAL;
        
        case BOAT_KEYBOARD_Escape:         return GLFW_KEY_ESCAPE;
        case BOAT_KEYBOARD_Tab:            return GLFW_KEY_TAB;
        case BOAT_KEYBOARD_Shift_L:        return GLFW_KEY_LEFT_SHIFT;
        case BOAT_KEYBOARD_Shift_R:        return GLFW_KEY_RIGHT_SHIFT;
        case BOAT_KEYBOARD_Control_L:      return GLFW_KEY_LEFT_CONTROL;
        case BOAT_KEYBOARD_Control_R:      return GLFW_KEY_RIGHT_CONTROL;
        case BOAT_KEYBOARD_Meta_L:
        case BOAT_KEYBOARD_Alt_L:          return GLFW_KEY_LEFT_ALT;
        case BOAT_KEYBOARD_Mode_switch: // Mapped to Alt_R on many keyboards
        case BOAT_KEYBOARD_ISO_Level3_Shift: // AltGr on at least some machines
        case BOAT_KEYBOARD_Meta_R:
        case BOAT_KEYBOARD_Alt_R:          return GLFW_KEY_RIGHT_ALT;
        case BOAT_KEYBOARD_Super_L:        return GLFW_KEY_LEFT_SUPER;
        case BOAT_KEYBOARD_Super_R:        return GLFW_KEY_RIGHT_SUPER;
        case BOAT_KEYBOARD_Menu:           return GLFW_KEY_MENU;
        case BOAT_KEYBOARD_Num_Lock:       return GLFW_KEY_NUM_LOCK;
        case BOAT_KEYBOARD_Caps_Lock:      return GLFW_KEY_CAPS_LOCK;
        case BOAT_KEYBOARD_Print:          return GLFW_KEY_PRINT_SCREEN;
        case BOAT_KEYBOARD_Scroll_Lock:    return GLFW_KEY_SCROLL_LOCK;
        case BOAT_KEYBOARD_Pause:          return GLFW_KEY_PAUSE;
        case BOAT_KEYBOARD_Delete:         return GLFW_KEY_DELETE;
        case BOAT_KEYBOARD_BackSpace:      return GLFW_KEY_BACKSPACE;
        case BOAT_KEYBOARD_Return:         return GLFW_KEY_ENTER;
        case BOAT_KEYBOARD_Home:           return GLFW_KEY_HOME;
        case BOAT_KEYBOARD_End:            return GLFW_KEY_END;
        case BOAT_KEYBOARD_Page_Up:        return GLFW_KEY_PAGE_UP;
        case BOAT_KEYBOARD_Page_Down:      return GLFW_KEY_PAGE_DOWN;
        case BOAT_KEYBOARD_Insert:         return GLFW_KEY_INSERT;
        case BOAT_KEYBOARD_Left:           return GLFW_KEY_LEFT;
        case BOAT_KEYBOARD_Right:          return GLFW_KEY_RIGHT;
        case BOAT_KEYBOARD_Down:           return GLFW_KEY_DOWN;
        case BOAT_KEYBOARD_Up:             return GLFW_KEY_UP;
        case BOAT_KEYBOARD_F1:             return GLFW_KEY_F1;
        case BOAT_KEYBOARD_F2:             return GLFW_KEY_F2;
        case BOAT_KEYBOARD_F3:             return GLFW_KEY_F3;
        case BOAT_KEYBOARD_F4:             return GLFW_KEY_F4;
        case BOAT_KEYBOARD_F5:             return GLFW_KEY_F5;
        case BOAT_KEYBOARD_F6:             return GLFW_KEY_F6;
        case BOAT_KEYBOARD_F7:             return GLFW_KEY_F7;
        case BOAT_KEYBOARD_F8:             return GLFW_KEY_F8;
        case BOAT_KEYBOARD_F9:             return GLFW_KEY_F9;
        case BOAT_KEYBOARD_F10:            return GLFW_KEY_F10;
        case BOAT_KEYBOARD_F11:            return GLFW_KEY_F11;
        case BOAT_KEYBOARD_F12:            return GLFW_KEY_F12;
        case BOAT_KEYBOARD_F13:            return GLFW_KEY_F13;
        case BOAT_KEYBOARD_F14:            return GLFW_KEY_F14;
        case BOAT_KEYBOARD_F15:            return GLFW_KEY_F15;
        case BOAT_KEYBOARD_F16:            return GLFW_KEY_F16;
        case BOAT_KEYBOARD_F17:            return GLFW_KEY_F17;
        case BOAT_KEYBOARD_F18:            return GLFW_KEY_F18;
        case BOAT_KEYBOARD_F19:            return GLFW_KEY_F19;
        case BOAT_KEYBOARD_F20:            return GLFW_KEY_F20;
        case BOAT_KEYBOARD_F21:            return GLFW_KEY_F21;
        case BOAT_KEYBOARD_F22:            return GLFW_KEY_F22;
        case BOAT_KEYBOARD_F23:            return GLFW_KEY_F23;
        case BOAT_KEYBOARD_F24:            return GLFW_KEY_F24;
        case BOAT_KEYBOARD_F25:            return GLFW_KEY_F25;

        // Numeric keypad
        case BOAT_KEYBOARD_KP_Divide:      return GLFW_KEY_KP_DIVIDE;
        case BOAT_KEYBOARD_KP_Multiply:    return GLFW_KEY_KP_MULTIPLY;
        case BOAT_KEYBOARD_KP_Subtract:    return GLFW_KEY_KP_SUBTRACT;
        case BOAT_KEYBOARD_KP_Add:         return GLFW_KEY_KP_ADD;

        // These should have been detected in secondary keysym test above!
        case BOAT_KEYBOARD_KP_Insert:      return GLFW_KEY_KP_0;
        case BOAT_KEYBOARD_KP_End:         return GLFW_KEY_KP_1;
        case BOAT_KEYBOARD_KP_Down:        return GLFW_KEY_KP_2;
        case BOAT_KEYBOARD_KP_Page_Down:   return GLFW_KEY_KP_3;
        case BOAT_KEYBOARD_KP_Left:        return GLFW_KEY_KP_4;
        case BOAT_KEYBOARD_KP_Right:       return GLFW_KEY_KP_6;
        case BOAT_KEYBOARD_KP_Home:        return GLFW_KEY_KP_7;
        case BOAT_KEYBOARD_KP_Up:          return GLFW_KEY_KP_8;
        case BOAT_KEYBOARD_KP_Page_Up:     return GLFW_KEY_KP_9;
        case BOAT_KEYBOARD_KP_Delete:      return GLFW_KEY_KP_DECIMAL;
        case BOAT_KEYBOARD_KP_Equal:       return GLFW_KEY_KP_EQUAL;
        case BOAT_KEYBOARD_KP_Enter:       return GLFW_KEY_KP_ENTER;

        // Last resort: Check for printable keys (should not happen if the XKB
        // extension is available). This will give a layout dependent mapping
        // (which is wrong, and we may miss some keys, especially on non-US
        // keyboards), but it's better than nothing...
        case BOAT_KEYBOARD_a:              return GLFW_KEY_A;
        case BOAT_KEYBOARD_b:              return GLFW_KEY_B;
        case BOAT_KEYBOARD_c:              return GLFW_KEY_C;
        case BOAT_KEYBOARD_d:              return GLFW_KEY_D;
        case BOAT_KEYBOARD_e:              return GLFW_KEY_E;
        case BOAT_KEYBOARD_f:              return GLFW_KEY_F;
        case BOAT_KEYBOARD_g:              return GLFW_KEY_G;
        case BOAT_KEYBOARD_h:              return GLFW_KEY_H;
        case BOAT_KEYBOARD_i:              return GLFW_KEY_I;
        case BOAT_KEYBOARD_j:              return GLFW_KEY_J;
        case BOAT_KEYBOARD_k:              return GLFW_KEY_K;
        case BOAT_KEYBOARD_l:              return GLFW_KEY_L;
        case BOAT_KEYBOARD_m:              return GLFW_KEY_M;
        case BOAT_KEYBOARD_n:              return GLFW_KEY_N;
        case BOAT_KEYBOARD_o:              return GLFW_KEY_O;
        case BOAT_KEYBOARD_p:              return GLFW_KEY_P;
        case BOAT_KEYBOARD_q:              return GLFW_KEY_Q;
        case BOAT_KEYBOARD_r:              return GLFW_KEY_R;
        case BOAT_KEYBOARD_s:              return GLFW_KEY_S;
        case BOAT_KEYBOARD_t:              return GLFW_KEY_T;
        case BOAT_KEYBOARD_u:              return GLFW_KEY_U;
        case BOAT_KEYBOARD_v:              return GLFW_KEY_V;
        case BOAT_KEYBOARD_w:              return GLFW_KEY_W;
        case BOAT_KEYBOARD_x:              return GLFW_KEY_X;
        case BOAT_KEYBOARD_y:              return GLFW_KEY_Y;
        case BOAT_KEYBOARD_z:              return GLFW_KEY_Z;
        case BOAT_KEYBOARD_1:              return GLFW_KEY_1;
        case BOAT_KEYBOARD_2:              return GLFW_KEY_2;
        case BOAT_KEYBOARD_3:              return GLFW_KEY_3;
        case BOAT_KEYBOARD_4:              return GLFW_KEY_4;
        case BOAT_KEYBOARD_5:              return GLFW_KEY_5;
        case BOAT_KEYBOARD_6:              return GLFW_KEY_6;
        case BOAT_KEYBOARD_7:              return GLFW_KEY_7;
        case BOAT_KEYBOARD_8:              return GLFW_KEY_8;
        case BOAT_KEYBOARD_9:              return GLFW_KEY_9;
        case BOAT_KEYBOARD_0:              return GLFW_KEY_0;
        case BOAT_KEYBOARD_space:          return GLFW_KEY_SPACE;
        case BOAT_KEYBOARD_minus:          return GLFW_KEY_MINUS;
        case BOAT_KEYBOARD_equal:          return GLFW_KEY_EQUAL;
        case BOAT_KEYBOARD_bracketleft:    return GLFW_KEY_LEFT_BRACKET;
        case BOAT_KEYBOARD_bracketright:   return GLFW_KEY_RIGHT_BRACKET;
        case BOAT_KEYBOARD_backslash:      return GLFW_KEY_BACKSLASH;
        case BOAT_KEYBOARD_semicolon:      return GLFW_KEY_SEMICOLON;
        case BOAT_KEYBOARD_apostrophe:     return GLFW_KEY_APOSTROPHE;
        case BOAT_KEYBOARD_grave:          return GLFW_KEY_GRAVE_ACCENT;
        case BOAT_KEYBOARD_comma:          return GLFW_KEY_COMMA;
        case BOAT_KEYBOARD_period:         return GLFW_KEY_PERIOD;
        case BOAT_KEYBOARD_slash:          return GLFW_KEY_SLASH;
        case BOAT_KEYBOARD_less:           return GLFW_KEY_WORLD_1; // At least in some layouts...
        default:                break;
    }

    // No matching translation was found
    return GLFW_KEY_UNKNOWN;
}

// Create key code translation tables
//
static void createKeyTables(void)
{
    int scancode, scancodeMin, scancodeMax;

    memset(_glfw.boat.keycodes, -1, sizeof(_glfw.boat.keycodes));
    memset(_glfw.boat.scancodes, -1, sizeof(_glfw.boat.scancodes));

    scancodeMin = 0; 
    scancodeMax = 255;
    for (scancode = scancodeMin;  scancode <= scancodeMax;  scancode++)
    {
        // Translate the un-translated key codes using traditional Boat KeySym
        // lookups
        if (_glfw.boat.keycodes[scancode] < 0)
        {
            _glfw.boat.keycodes[scancode] = translateKeyCode(scancode);
        }

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
    
    //_glfw.boat.screen = 0;

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

