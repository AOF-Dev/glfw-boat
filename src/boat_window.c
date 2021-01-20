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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

// Additional mouse button names for XButtonEvent
#define Button6            6
#define Button7            7



// Translates an Boat event modifier state mask
//
/*
static int translateState(int state)
{
    int mods = 0;

    if (state & ShiftMask)
        mods |= GLFW_MOD_SHIFT;
    if (state & ControlMask)
        mods |= GLFW_MOD_CONTROL;
    if (state & Mod1Mask)
        mods |= GLFW_MOD_ALT;
    if (state & Mod4Mask)
        mods |= GLFW_MOD_SUPER;
    if (state & LockMask)
        mods |= GLFW_MOD_CAPS_LOCK;
    if (state & Mod2Mask)
        mods |= GLFW_MOD_NUM_LOCK;

    return mods;
}
*/
// Translates an Boat key code to a GLFW key token
//
static int translateKey(int scancode)
{   
    // Use the pre-filled LUT (see createKeyTables() in boat_init.c)
    if (scancode < 0 || scancode > 255)
        return GLFW_KEY_UNKNOWN;

    return _glfw.boat.keycodes[scancode];
}

// Updates the normal hints according to the window settings
//
static void updateNormalHints(_GLFWwindow* window, int width, int height)
{
    
}

// Updates the full screen status of the window
//
static void updateWindowMode(_GLFWwindow* window)
{
    
}

// Updates the cursor image according to its cursor mode
//
static void updateCursorImage(_GLFWwindow* window)
{
    
}

// Apply disabled cursor mode to a focused window
//
static void disableCursor(_GLFWwindow* window)
{
    
    _glfw.boat.disabledCursorWindow = window;
    _glfwPlatformGetCursorPos(window,
                              &_glfw.boat.restoreCursorPosX,
                              &_glfw.boat.restoreCursorPosY);
    //updateCursorImage(window);
    _glfwCenterCursorInContentArea(window);
    boatSetCursorMode(CursorDisabled);
    
}

// Exit disabled cursor mode for the specified window
//
static void enableCursor(_GLFWwindow* window)
{

    _glfw.boat.disabledCursorWindow = NULL;
    boatSetCursorMode(CursorEnabled);
    _glfwPlatformSetCursorPos(window,
                              _glfw.boat.restoreCursorPosX,
                              _glfw.boat.restoreCursorPosY);
    //updateCursorImage(window);
}

// Process the specified Boat event
//
static _GLFWwindow* current_window = NULL;
static BoatInputEvent current_event;
static void processEvent()
{
    int keycode = 0;
    BoatInputEvent *event = 0; 
    
    _GLFWwindow* window = current_window;
    
    if (!window){
        return;
    }
    
    event = &current_event;
    boatGetCurrentEvent(event);
    
    // HACK: Save scancode
    if (event->type == KeyPress || event->type == KeyRelease)
        keycode = event->keycode;

    switch (event->type)
    {
        
        case KeyPress:
        {
            const int key = translateKey(keycode);
            const int mods = 0;//translateState(event->xkey.state);
            const int plain = !(mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT));
            
            if (event->keychar != 0)
            {
                    if (keycode)
                        _glfwInputKey(window, key, keycode, GLFW_PRESS, mods);
                    
                    _glfwInputChar(window, event->keychar, mods, plain);
            }
            else {
                _glfwInputKey(window, key, keycode, GLFW_PRESS, mods);
            }
            return;
        }

        case KeyRelease:
        {
            const int key = translateKey(keycode);
            const int mods = 0;//translateState(event->xkey.state);

            _glfwInputKey(window, key, keycode, GLFW_RELEASE, mods);
            return;
        }

        case ButtonPress:
        {
            const int mods = 0;//translateState(event->xbutton.state);

            if (event->mouse_button == Button1)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, mods);
            else if (event->mouse_button == Button2)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, mods);
            else if (event->mouse_button == Button3)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, mods);

            // Modern X provides scroll events as mouse button presses
            else if (event->mouse_button == Button4)
                _glfwInputScroll(window, 0.0, 1.0);
            else if (event->mouse_button == Button5)
                _glfwInputScroll(window, 0.0, -1.0);
            else if (event->mouse_button == Button6)
                _glfwInputScroll(window, 1.0, 0.0);
            else if (event->mouse_button == Button7)
                _glfwInputScroll(window, -1.0, 0.0);

            else
            {
                // Additional buttons after 7 are treated as regular buttons
                // We subtract 4 to fill the gap left by scroll input above
                _glfwInputMouseClick(window,
                                     event->mouse_button - Button1 - 4,
                                     GLFW_PRESS,
                                     mods);
            }

            return;
        }

        case ButtonRelease:
        {
            const int mods = 0;//translateState(event->xbutton.state);

            if (event->mouse_button == Button1)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_LEFT,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->mouse_button == Button2)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_MIDDLE,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->mouse_button == Button3)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_RIGHT,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->mouse_button > Button7)
            {
                // Additional buttons after 7 are treated as regular buttons
                // We subtract 4 to fill the gap left by scroll input above
                _glfwInputMouseClick(window,
                                     event->mouse_button - Button1 - 4,
                                     GLFW_RELEASE,
                                     mods);
            }

            return;
        }

        case MotionNotify:
        {
            const int x = event->x;
            const int y = event->y;

            if (x != window->boat.warpCursorPosX ||
                y != window->boat.warpCursorPosY)
            {
                // The cursor was moved by something other than GLFW

                if (window->cursorMode == GLFW_CURSOR_DISABLED)
                {
                    if (_glfw.boat.disabledCursorWindow != window)
                        return;
                    if (window->rawMouseMotion)
                        return;

                    const int dx = x - window->boat.lastCursorPosX;
                    const int dy = y - window->boat.lastCursorPosY;

                    _glfwInputCursorPos(window,
                                        window->virtualCursorPosX + dx,
                                        window->virtualCursorPosY + dy);
                }
                else
                    _glfwInputCursorPos(window, x, y);
            }

            window->boat.lastCursorPosX = x;
            window->boat.lastCursorPosY = y;
            return;
        }

    }
}

// Create the Boat window
//
static GLFWbool createNativeWindow(_GLFWwindow* window,
                                   const _GLFWwndconfig* wndconfig,
                                   int visualID, int depth)
{
    int width = wndconfig->width;
    int height = wndconfig->height;

    if (wndconfig->scaleToMonitor)
    {
        width *= _glfw.boat.contentScaleX;
        height *= _glfw.boat.contentScaleY;
    }
    
    window->boat.transparent = GLFW_FALSE;

    window->boat.handle = boatGetNativeWindow();
    

    if (!window->boat.handle)
    {
        _glfwInputErrorBoat(GLFW_PLATFORM_ERROR,
                           "Boat: Failed to get window");
        return GLFW_FALSE;
    }

    if (!wndconfig->decorated)
        _glfwPlatformSetWindowDecorated(window, GLFW_FALSE);

    window->boat.maximized = GLFW_TRUE;

    //updateNormalHints(window, width, height);

    _glfwPlatformSetWindowTitle(window, wndconfig->title);
    _glfwPlatformGetWindowPos(window, &window->boat.xpos, &window->boat.ypos);
    _glfwPlatformGetWindowSize(window, &window->boat.width, &window->boat.height);
    
    current_window = window;
    boatSetCurrentEventProcessor((BoatEventProcessor)(void*)processEvent);

    return GLFW_TRUE;
}



// Make the specified window and its video mode active on its monitor
//
static void acquireMonitor(_GLFWwindow* window)
{
    
    //_glfwSetVideoModeX11(window->monitor, &window->videoMode);

    
    int xpos, ypos;
    GLFWvidmode mode;

    // Manually position the window over its monitor
    _glfwPlatformGetMonitorPos(window->monitor, &xpos, &ypos);
    _glfwPlatformGetVideoMode(window->monitor, &mode);

    _glfwInputMonitorWindow(window->monitor, window);
}

// Remove the window and restore the original video mode
//
static void releaseMonitor(_GLFWwindow* window)
{
    if (window->monitor->window != window)
        return;

    _glfwInputMonitorWindow(window->monitor, NULL);
    //_glfwRestoreVideoModeX11(window->monitor);

    
}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformCreateWindow(_GLFWwindow* window,
                              const _GLFWwndconfig* wndconfig,
                              const _GLFWctxconfig* ctxconfig,
                              const _GLFWfbconfig* fbconfig)
{
    int visualID = NULL;
    int depth;

    if (ctxconfig->client != GLFW_NO_API)
    {
        if (!_glfwInitEGL())
            return GLFW_FALSE;
        if (!_glfwChooseVisualEGL(wndconfig, ctxconfig, fbconfig, &visualID, &depth))
            return GLFW_FALSE;
    }

    if (ctxconfig->client == GLFW_NO_API ||
        ctxconfig->source == GLFW_OSMESA_CONTEXT_API)
    {
        visualID = 0;
        depth = 16;
    }

    if (!createNativeWindow(window, wndconfig, visualID, depth))
        return GLFW_FALSE;

    if (ctxconfig->client != GLFW_NO_API)
    {
        
        if (!_glfwCreateContextEGL(window, ctxconfig, fbconfig))
            return GLFW_FALSE;
    }

    if (window->monitor)
    {
        _glfwPlatformShowWindow(window);
        updateWindowMode(window);
        //acquireMonitor(window);
    }

    return GLFW_TRUE;
}

void _glfwPlatformDestroyWindow(_GLFWwindow* window)
{
    if (_glfw.boat.disabledCursorWindow == window)
        _glfw.boat.disabledCursorWindow = NULL;

    if (window->monitor)
        releaseMonitor(window);

    if (window->boat.handle)
    {
        window->boat.handle = (ANativeWindow*) 0;
    }

}

void _glfwPlatformSetWindowTitle(_GLFWwindow* window, const char* title)
{
    
}

void _glfwPlatformSetWindowIcon(_GLFWwindow* window,
                                int count, const GLFWimage* images)
{
    
}

void _glfwPlatformGetWindowPos(_GLFWwindow* window, int* xpos, int* ypos)
{
    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
}

void _glfwPlatformSetWindowPos(_GLFWwindow* window, int xpos, int ypos)
{
    
}

void _glfwPlatformGetWindowSize(_GLFWwindow* window, int* width, int* height)
{
    if (width)
        *width = ANativeWindow_getWidth(window->boat.handle);
    if (height)
        *height = ANativeWindow_getHeight(window->boat.handle);
}

void _glfwPlatformSetWindowSize(_GLFWwindow* window, int width, int height)
{
    
}

void _glfwPlatformSetWindowSizeLimits(_GLFWwindow* window,
                                      int minwidth, int minheight,
                                      int maxwidth, int maxheight)
{
    
}

void _glfwPlatformSetWindowAspectRatio(_GLFWwindow* window, int numer, int denom)
{
    
}

void _glfwPlatformGetFramebufferSize(_GLFWwindow* window, int* width, int* height)
{
    _glfwPlatformGetWindowSize(window, width, height);
}

void _glfwPlatformGetWindowFrameSize(_GLFWwindow* window,
                                     int* left, int* top,
                                     int* right, int* bottom)
{
    
    if (left)
        *left = 0;
    if (top)
        *top = 0;
    if (right)
        *right = 0;
    if (bottom)
        *bottom = 0;
    
}

void _glfwPlatformGetWindowContentScale(_GLFWwindow* window,
                                        float* xscale, float* yscale)
{
    if (xscale)
        *xscale = _glfw.boat.contentScaleX;
    if (yscale)
        *yscale = _glfw.boat.contentScaleY;
}

void _glfwPlatformIconifyWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformRestoreWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformMaximizeWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformShowWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformHideWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformRequestWindowAttention(_GLFWwindow* window)
{
    
}

void _glfwPlatformFocusWindow(_GLFWwindow* window)
{
    
}

void _glfwPlatformSetWindowMonitor(_GLFWwindow* window,
                                   _GLFWmonitor* monitor,
                                   int xpos, int ypos,
                                   int width, int height,
                                   int refreshRate)
{
    

}

int _glfwPlatformWindowFocused(_GLFWwindow* window)
{
    return GLFW_TRUE;
}

int _glfwPlatformWindowIconified(_GLFWwindow* window)
{
    return GLFW_FALSE;
}

int _glfwPlatformWindowVisible(_GLFWwindow* window)
{
    return GLFW_TRUE;
}

int _glfwPlatformWindowMaximized(_GLFWwindow* window)
{
    
    return GLFW_TRUE;
}

int _glfwPlatformWindowHovered(_GLFWwindow* window)
{
    return GLFW_FALSE;
}

int _glfwPlatformFramebufferTransparent(_GLFWwindow* window)
{
    if (!window->boat.transparent)
        return GLFW_FALSE;

    return GLFW_FALSE;
}

void _glfwPlatformSetWindowResizable(_GLFWwindow* window, GLFWbool enabled)
{
    int width, height;
    _glfwPlatformGetWindowSize(window, &width, &height);
    updateNormalHints(window, width, height);
}

void _glfwPlatformSetWindowDecorated(_GLFWwindow* window, GLFWbool enabled)
{
    
}

void _glfwPlatformSetWindowFloating(_GLFWwindow* window, GLFWbool enabled)
{
    
}

void _glfwPlatformSetWindowMousePassthrough(_GLFWwindow* window, GLFWbool enabled)
{
    
}

float _glfwPlatformGetWindowOpacity(_GLFWwindow* window)
{
    float opacity = 1.f;
    return opacity;
}

void _glfwPlatformSetWindowOpacity(_GLFWwindow* window, float opacity)
{
    
}

void _glfwPlatformSetRawMouseMotion(_GLFWwindow *window, GLFWbool enabled)
{
    
}

GLFWbool _glfwPlatformRawMouseMotionSupported(void)
{
    return GLFW_FALSE;
}

void _glfwPlatformPollEvents(void)
{
    _GLFWwindow* window;

    window = _glfw.boat.disabledCursorWindow;
    if (window)
    {
        int width, height;
        _glfwPlatformGetWindowSize(window, &width, &height);

        // NOTE: Re-center the cursor only if it has moved since the last call,
        //       to avoid breaking glfwWaitEvents with MotionNotify
        if (window->boat.lastCursorPosX != width / 2 ||
            window->boat.lastCursorPosY != height / 2)
        {
            _glfwPlatformSetCursorPos(window, width / 2, height / 2);
        }
    }

}

void _glfwPlatformWaitEvents(void)
{
    /*
    while (!XPending(_glfw.x11.display))
        waitForEvent(NULL);
    */

    _glfwPlatformPollEvents();
}

void _glfwPlatformWaitEventsTimeout(double timeout)
{
    /*
    while (!XPending(_glfw.x11.display))
    {
        if (!waitForEvent(&timeout))
            break;
    }
    */

    _glfwPlatformPollEvents();
}

void _glfwPlatformPostEmptyEvent(void)
{
    
}

void _glfwPlatformGetCursorPos(_GLFWwindow* window, double* xpos, double* ypos)
{

    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
}

void _glfwPlatformSetCursorPos(_GLFWwindow* window, double x, double y)
{
    // Store the new position so it can be recognized later
    window->boat.warpCursorPosX = (int) x;
    window->boat.warpCursorPosY = (int) y;
    
    // FIXME : should set cursor pos in Android view
}

void _glfwPlatformSetCursorMode(_GLFWwindow* window, int mode)
{
    if (mode == GLFW_CURSOR_DISABLED)
    {
        if (_glfwPlatformWindowFocused(window))
            disableCursor(window);
    }
    else if (_glfw.boat.disabledCursorWindow == window) {
        enableCursor(window);
    }
    else {
        //updateCursorImage(window);
    }

}

const char* _glfwPlatformGetScancodeName(int scancode)
{
    if (scancode < 0 || scancode > 0xff ||
        _glfw.boat.keycodes[scancode] == GLFW_KEY_UNKNOWN)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid scancode");
        return NULL;
    }
    
    const int key = _glfw.boat.keycodes[scancode];
    if (_glfw.boat.keynames[key][0] != 0) {
        return _glfw.boat.keynames[key];
    }
    return NULL;
}

int _glfwPlatformGetKeyScancode(int key)
{
    return _glfw.boat.scancodes[key];
}

int _glfwPlatformCreateCursor(_GLFWcursor* cursor,
                              const GLFWimage* image,
                              int xhot, int yhot)
{

    return GLFW_TRUE;
}

int _glfwPlatformCreateStandardCursor(_GLFWcursor* cursor, int shape)
{
    
    return GLFW_TRUE;
}

void _glfwPlatformDestroyCursor(_GLFWcursor* cursor)
{
    
}

void _glfwPlatformSetCursor(_GLFWwindow* window, _GLFWcursor* cursor)
{
    if (window->cursorMode == GLFW_CURSOR_NORMAL)
    {
        // update image
    }
}

void _glfwPlatformSetClipboardString(const char* string)
{
    
}

const char* _glfwPlatformGetClipboardString(void)
{
    return "";//getSelectionString(_glfw.x11.CLIPBOARD);
}

EGLenum _glfwPlatformGetEGLPlatform(EGLint** attribs)
{
    return 0;
}

EGLNativeDisplayType _glfwPlatformGetEGLNativeDisplay(void)
{
    return _glfw.boat.display;
}

EGLNativeWindowType _glfwPlatformGetEGLNativeWindow(_GLFWwindow* window)
{
    if (_glfw.egl.platform)
        return &window->boat.handle;
    else
        return (EGLNativeWindowType) window->boat.handle;
}

void _glfwPlatformGetRequiredInstanceExtensions(char** extensions)
{
    
}

int _glfwPlatformGetPhysicalDevicePresentationSupport(VkInstance instance,
                                                      VkPhysicalDevice device,
                                                      uint32_t queuefamily)
{
    return GLFW_FALSE;
}

VkResult _glfwPlatformCreateWindowSurface(VkInstance instance,
                                          _GLFWwindow* window,
                                          const VkAllocationCallbacks* allocator,
                                          VkSurfaceKHR* surface)
{
    // This seems like the most appropriate error to return here
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI void* glfwGetBoatDisplay(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return _glfw.boat.display;
}

GLFWAPI ANativeWindow* glfwGetBoatWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return window->boat.handle;
}

