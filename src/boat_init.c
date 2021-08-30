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
// This file is derived from x11_init.c
//========================================================================

#include "internal.h"

#include <string.h>
#include <stdio.h>


// Create key code translation tables
//
static void createKeyTables(void)
{
    int scancode, scancodeMin, scancodeMax;

    memset(_glfw.x11.keycodes, -1, sizeof(_glfw.x11.keycodes));
    memset(_glfw.x11.scancodes, -1, sizeof(_glfw.x11.scancodes));

    if (_glfw.x11.xkb.available)
    {
        // Use XKB to determine physical key locations independently of the
        // current keyboard layout

        XkbDescPtr desc = XkbGetMap(_glfw.x11.display, 0, XkbUseCoreKbd);
        XkbGetNames(_glfw.x11.display, XkbKeyNamesMask | XkbKeyAliasesMask, desc);

        scancodeMin = desc->min_key_code;
        scancodeMax = desc->max_key_code;

        const struct
        {
            int key;
            char* name;
        } keymap[] =
        {
            { GLFW_KEY_GRAVE_ACCENT, "TLDE" },
            { GLFW_KEY_1, "AE01" },
            { GLFW_KEY_2, "AE02" },
            { GLFW_KEY_3, "AE03" },
            { GLFW_KEY_4, "AE04" },
            { GLFW_KEY_5, "AE05" },
            { GLFW_KEY_6, "AE06" },
            { GLFW_KEY_7, "AE07" },
            { GLFW_KEY_8, "AE08" },
            { GLFW_KEY_9, "AE09" },
            { GLFW_KEY_0, "AE10" },
            { GLFW_KEY_MINUS, "AE11" },
            { GLFW_KEY_EQUAL, "AE12" },
            { GLFW_KEY_Q, "AD01" },
            { GLFW_KEY_W, "AD02" },
            { GLFW_KEY_E, "AD03" },
            { GLFW_KEY_R, "AD04" },
            { GLFW_KEY_T, "AD05" },
            { GLFW_KEY_Y, "AD06" },
            { GLFW_KEY_U, "AD07" },
            { GLFW_KEY_I, "AD08" },
            { GLFW_KEY_O, "AD09" },
            { GLFW_KEY_P, "AD10" },
            { GLFW_KEY_LEFT_BRACKET, "AD11" },
            { GLFW_KEY_RIGHT_BRACKET, "AD12" },
            { GLFW_KEY_A, "AC01" },
            { GLFW_KEY_S, "AC02" },
            { GLFW_KEY_D, "AC03" },
            { GLFW_KEY_F, "AC04" },
            { GLFW_KEY_G, "AC05" },
            { GLFW_KEY_H, "AC06" },
            { GLFW_KEY_J, "AC07" },
            { GLFW_KEY_K, "AC08" },
            { GLFW_KEY_L, "AC09" },
            { GLFW_KEY_SEMICOLON, "AC10" },
            { GLFW_KEY_APOSTROPHE, "AC11" },
            { GLFW_KEY_Z, "AB01" },
            { GLFW_KEY_X, "AB02" },
            { GLFW_KEY_C, "AB03" },
            { GLFW_KEY_V, "AB04" },
            { GLFW_KEY_B, "AB05" },
            { GLFW_KEY_N, "AB06" },
            { GLFW_KEY_M, "AB07" },
            { GLFW_KEY_COMMA, "AB08" },
            { GLFW_KEY_PERIOD, "AB09" },
            { GLFW_KEY_SLASH, "AB10" },
            { GLFW_KEY_BACKSLASH, "BKSL" },
            { GLFW_KEY_WORLD_1, "LSGT" },
            { GLFW_KEY_SPACE, "SPCE" },
            { GLFW_KEY_ESCAPE, "ESC" },
            { GLFW_KEY_ENTER, "RTRN" },
            { GLFW_KEY_TAB, "TAB" },
            { GLFW_KEY_BACKSPACE, "BKSP" },
            { GLFW_KEY_INSERT, "INS" },
            { GLFW_KEY_DELETE, "DELE" },
            { GLFW_KEY_RIGHT, "RGHT" },
            { GLFW_KEY_LEFT, "LEFT" },
            { GLFW_KEY_DOWN, "DOWN" },
            { GLFW_KEY_UP, "UP" },
            { GLFW_KEY_PAGE_UP, "PGUP" },
            { GLFW_KEY_PAGE_DOWN, "PGDN" },
            { GLFW_KEY_HOME, "HOME" },
            { GLFW_KEY_END, "END" },
            { GLFW_KEY_CAPS_LOCK, "CAPS" },
            { GLFW_KEY_SCROLL_LOCK, "SCLK" },
            { GLFW_KEY_NUM_LOCK, "NMLK" },
            { GLFW_KEY_PRINT_SCREEN, "PRSC" },
            { GLFW_KEY_PAUSE, "PAUS" },
            { GLFW_KEY_F1, "FK01" },
            { GLFW_KEY_F2, "FK02" },
            { GLFW_KEY_F3, "FK03" },
            { GLFW_KEY_F4, "FK04" },
            { GLFW_KEY_F5, "FK05" },
            { GLFW_KEY_F6, "FK06" },
            { GLFW_KEY_F7, "FK07" },
            { GLFW_KEY_F8, "FK08" },
            { GLFW_KEY_F9, "FK09" },
            { GLFW_KEY_F10, "FK10" },
            { GLFW_KEY_F11, "FK11" },
            { GLFW_KEY_F12, "FK12" },
            { GLFW_KEY_F13, "FK13" },
            { GLFW_KEY_F14, "FK14" },
            { GLFW_KEY_F15, "FK15" },
            { GLFW_KEY_F16, "FK16" },
            { GLFW_KEY_F17, "FK17" },
            { GLFW_KEY_F18, "FK18" },
            { GLFW_KEY_F19, "FK19" },
            { GLFW_KEY_F20, "FK20" },
            { GLFW_KEY_F21, "FK21" },
            { GLFW_KEY_F22, "FK22" },
            { GLFW_KEY_F23, "FK23" },
            { GLFW_KEY_F24, "FK24" },
            { GLFW_KEY_F25, "FK25" },
            { GLFW_KEY_KP_0, "KP0" },
            { GLFW_KEY_KP_1, "KP1" },
            { GLFW_KEY_KP_2, "KP2" },
            { GLFW_KEY_KP_3, "KP3" },
            { GLFW_KEY_KP_4, "KP4" },
            { GLFW_KEY_KP_5, "KP5" },
            { GLFW_KEY_KP_6, "KP6" },
            { GLFW_KEY_KP_7, "KP7" },
            { GLFW_KEY_KP_8, "KP8" },
            { GLFW_KEY_KP_9, "KP9" },
            { GLFW_KEY_KP_DECIMAL, "KPDL" },
            { GLFW_KEY_KP_DIVIDE, "KPDV" },
            { GLFW_KEY_KP_MULTIPLY, "KPMU" },
            { GLFW_KEY_KP_SUBTRACT, "KPSU" },
            { GLFW_KEY_KP_ADD, "KPAD" },
            { GLFW_KEY_KP_ENTER, "KPEN" },
            { GLFW_KEY_KP_EQUAL, "KPEQ" },
            { GLFW_KEY_LEFT_SHIFT, "LFSH" },
            { GLFW_KEY_LEFT_CONTROL, "LCTL" },
            { GLFW_KEY_LEFT_ALT, "LALT" },
            { GLFW_KEY_LEFT_SUPER, "LWIN" },
            { GLFW_KEY_RIGHT_SHIFT, "RTSH" },
            { GLFW_KEY_RIGHT_CONTROL, "RCTL" },
            { GLFW_KEY_RIGHT_ALT, "RALT" },
            { GLFW_KEY_RIGHT_ALT, "LVL3" },
            { GLFW_KEY_RIGHT_ALT, "MDSW" },
            { GLFW_KEY_RIGHT_SUPER, "RWIN" },
            { GLFW_KEY_MENU, "MENU" }
        };

        // Find the X11 key code -> GLFW key code mapping
        for (scancode = scancodeMin;  scancode <= scancodeMax;  scancode++)
        {
            int key = GLFW_KEY_UNKNOWN;

            // Map the key name to a GLFW key code. Note: We use the US
            // keyboard layout. Because function keys aren't mapped correctly
            // when using traditional KeySym translations, they are mapped
            // here instead.
            for (int i = 0;  i < sizeof(keymap) / sizeof(keymap[0]);  i++)
            {
                if (strncmp(desc->names->keys[scancode].name,
                            keymap[i].name,
                            XkbKeyNameLength) == 0)
                {
                    key = keymap[i].key;
                    break;
                }
            }

            // Fall back to key aliases in case the key name did not match
            for (int i = 0;  i < desc->names->num_key_aliases;  i++)
            {
                if (key != GLFW_KEY_UNKNOWN)
                    break;

                if (strncmp(desc->names->key_aliases[i].real,
                            desc->names->keys[scancode].name,
                            XkbKeyNameLength) != 0)
                {
                    continue;
                }

                for (int j = 0;  j < sizeof(keymap) / sizeof(keymap[0]);  j++)
                {
                    if (strncmp(desc->names->key_aliases[i].alias,
                                keymap[j].name,
                                XkbKeyNameLength) == 0)
                    {
                        key = keymap[j].key;
                        break;
                    }
                }
            }

            _glfw.x11.keycodes[scancode] = key;
        }

        XkbFreeNames(desc, XkbKeyNamesMask, True);
        XkbFreeKeyboard(desc, 0, True);
    }
    else
        XDisplayKeycodes(_glfw.x11.display, &scancodeMin, &scancodeMax);

    int width;
    KeySym* keysyms = XGetKeyboardMapping(_glfw.x11.display,
                                          scancodeMin,
                                          scancodeMax - scancodeMin + 1,
                                          &width);

    for (scancode = scancodeMin;  scancode <= scancodeMax;  scancode++)
    {
        // Translate the un-translated key codes using traditional X11 KeySym
        // lookups
        if (_glfw.x11.keycodes[scancode] < 0)
        {
            const size_t base = (scancode - scancodeMin) * width;
            _glfw.x11.keycodes[scancode] = translateKeySyms(&keysyms[base], width);
        }

        // Store the reverse translation for faster key name lookup
        if (_glfw.x11.keycodes[scancode] > 0)
            _glfw.x11.scancodes[_glfw.x11.keycodes[scancode]] = scancode;
    }

    XFree(keysyms);
}

// Retrieve system content scale via folklore heuristics
//
static void getSystemContentScale(float* xscale, float* yscale)
{
    // Start by assuming the default X11 DPI
    // NOTE: Some desktop environments (KDE) may remove the Xft.dpi field when it
    //       would be set to 96, so assume that is the case if we cannot find it
    float xdpi = 96.f, ydpi = 96.f;

    // NOTE: Basing the scale on Xft.dpi where available should provide the most
    //       consistent user experience (matches Qt, Gtk, etc), although not
    //       always the most accurate one
    char* rms = XResourceManagerString(_glfw.x11.display);
    if (rms)
    {
        XrmDatabase db = XrmGetStringDatabase(rms);
        if (db)
        {
            XrmValue value;
            char* type = NULL;

            if (XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value))
            {
                if (type && strcmp(type, "String") == 0)
                    xdpi = ydpi = atof(value.addr);
            }

            XrmDestroyDatabase(db);
        }
    }

    *xscale = xdpi / 96.f;
    *yscale = ydpi / 96.f;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformInit(void)
{
    getSystemContentScale(&_glfw.boat.contentScaleX, &_glfw.boat.contentScaleY);
    createKeyTables();

    _glfwInitTimerPOSIX();
    _glfwPollMonitorsBoat();
    return GLFW_TRUE;
}

void _glfwPlatformTerminate(void)
{
    _glfwTerminateEGL();
}

const char* _glfwPlatformGetVersionString(void)
{
    return _GLFW_VERSION_NUMBER " Boat EGL OSMesa"
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

