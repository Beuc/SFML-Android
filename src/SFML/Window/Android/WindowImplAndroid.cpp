////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
// Copyright (C) 2012  Sylvain Beucler
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <SFML/Window/Android/WindowImplAndroid.hpp>
#include <SFML/Window/WindowStyle.hpp>
//#include <EGL/egl.h>
#include <GLES/gl.h>
#include "SFML/Main/android_native_app_glue/android_native_app_glue.h"
#include <android/log.h>
#include <android/native_window.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SFML", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SFML", __VA_ARGS__))

extern "C" void app_dummy();


#define EVENT_HANDLED 1
#define EVENT_NOT_HANDLED 0

/// Only one window in Android for now
static sf::WindowHandle unique_window = NULL;

/**
 * Process the next input event.
 */
int32_t handle_input(struct android_app* app, AInputEvent* event) {
  //SFG_Window* window = fgStructure.CurrentWindow;

  /* FIXME: in Android, when key is repeated, down and up events
     happen most often at the exact same time.  This makes it
     impossible to animate based on key press time. */
  /* e.g. down/up/wait/down/up rather than down/wait/down/wait/up */
  
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
    /* LOGI("action: %d", AKeyEvent_getAction(event)); */
    /* LOGI("keycode: %d", code); */
    int32_t code = AKeyEvent_getKeyCode(event);

    if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN) {
      // int32_t keypress = 0;
      // unsigned char ascii = 0;
      // if ((keypress = key_a2fg[code]) && FETCH_WCB(*window, Special)) {
      // 	INVOKE_WCB(*window, Special, (keypress, window->State.MouseX, window->State.MouseY));
      // 	return EVENT_HANDLED;
      // } else if ((ascii = key_ascii(app, event)) && FETCH_WCB(*window, Keyboard)) {
      // 	INVOKE_WCB(*window, Keyboard, (ascii, window->State.MouseX, window->State.MouseY));
      // 	return EVENT_HANDLED;
      // }
    }
    else if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_UP) {
      // int32_t keypress = 0;
      // unsigned char ascii = 0;
      // if ((keypress = key_a2fg[code]) && FETCH_WCB(*window, Special)) {
      // 	INVOKE_WCB(*window, SpecialUp, (keypress, window->State.MouseX, window->State.MouseY));
      // 	return EVENT_HANDLED;
      // } else if ((ascii = key_ascii(app, event)) && FETCH_WCB(*window, Keyboard)) {
      // 	INVOKE_WCB(*window, KeyboardUp, (ascii, window->State.MouseX, window->State.MouseY));
      // 	return EVENT_HANDLED;
      // }
    }
  }

  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    // int32_t action = AMotionEvent_getAction(event);
    // float x = AMotionEvent_getX(event, 0);
    // float y = AMotionEvent_getY(event, 0);
    // LOGI("motion %.01f,%.01f action=%d", x, y, AMotionEvent_getAction(event));
    
    // /* Virtual arrows PAD */
    // // Don't interfere with existing mouse move event
    // if (!touchscreen.in_mmotion) {
    //   struct vpad_state prev_vpad = touchscreen.vpad;
    //   touchscreen.vpad.left = touchscreen.vpad.right
    // 	= touchscreen.vpad.up = touchscreen.vpad.down = false;

    //   int32_t width = ANativeWindow_getWidth(window->Window.Handle);
    //   int32_t height = ANativeWindow_getHeight(window->Window.Handle);
    //   if (action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_MOVE) {
    // 	if ((x > 0 && x < 100) && (y > (height - 100) && y < height))
    // 	  touchscreen.vpad.left = true;
    // 	if ((x > 200 && x < 300) && (y > (height - 100) && y < height))
    // 	  touchscreen.vpad.right = true;
    // 	if ((x > 100 && x < 200) && (y > (height - 100) && y < height))
    // 	  touchscreen.vpad.down = true;
    // 	if ((x > 100 && x < 200) && (y > (height - 200) && y < (height - 100)))
    // 	  touchscreen.vpad.up = true;
    //   }
    //   if (action == AMOTION_EVENT_ACTION_DOWN && 
    // 	  (touchscreen.vpad.left || touchscreen.vpad.right || touchscreen.vpad.down || touchscreen.vpad.up))
    // 	touchscreen.vpad.on = true;
    //   if (action == AMOTION_EVENT_ACTION_UP)
    // 	touchscreen.vpad.on = false;
    //   if (prev_vpad.left != touchscreen.vpad.left
    // 	  || prev_vpad.right != touchscreen.vpad.right
    // 	  || prev_vpad.up != touchscreen.vpad.up
    // 	  || prev_vpad.down != touchscreen.vpad.down
    // 	  || prev_vpad.on != touchscreen.vpad.on) {
    // 	if (FETCH_WCB(*window, Special)) {
    // 	  if (prev_vpad.left == false && touchscreen.vpad.left == true)
    // 	    INVOKE_WCB(*window, Special, (GLUT_KEY_LEFT, x, y));
    // 	  else if (prev_vpad.right == false && touchscreen.vpad.right == true)
    // 	    INVOKE_WCB(*window, Special, (GLUT_KEY_RIGHT, x, y));
    // 	  else if (prev_vpad.up == false && touchscreen.vpad.up == true)
    // 	    INVOKE_WCB(*window, Special, (GLUT_KEY_UP, x, y));
    // 	  else if (prev_vpad.down == false && touchscreen.vpad.down == true)
    // 	    INVOKE_WCB(*window, Special, (GLUT_KEY_DOWN, x, y));
    // 	}
    // 	if (FETCH_WCB(*window, SpecialUp)) {
    // 	  if (prev_vpad.left == true && touchscreen.vpad.left == false)
    // 	    INVOKE_WCB(*window, SpecialUp, (GLUT_KEY_LEFT, x, y));
    // 	  if (prev_vpad.right == true && touchscreen.vpad.right == false)
    // 	    INVOKE_WCB(*window, SpecialUp, (GLUT_KEY_RIGHT, x, y));
    // 	  if (prev_vpad.up == true && touchscreen.vpad.up == false)
    // 	    INVOKE_WCB(*window, SpecialUp, (GLUT_KEY_UP, x, y));
    // 	  if (prev_vpad.down == true && touchscreen.vpad.down == false)
    // 	    INVOKE_WCB(*window, SpecialUp, (GLUT_KEY_DOWN, x, y));
    // 	}
    // 	return EVENT_HANDLED;
    //   }
    // }
    
    // /* Normal mouse events */
    // if (!touchscreen.vpad.on) {
    //   window->State.MouseX = x;
    //   window->State.MouseY = y;
    //   LOGI("Changed mouse position: %d,%d", x, y);
    //   if (action == AMOTION_EVENT_ACTION_DOWN && FETCH_WCB(*window, Mouse)) {
    //   	touchscreen.in_mmotion = true;
    //   	INVOKE_WCB(*window, Mouse, (GLUT_LEFT_BUTTON, GLUT_DOWN, x, y));
    //   } else if (action == AMOTION_EVENT_ACTION_UP && FETCH_WCB(*window, Mouse)) {
    //   	touchscreen.in_mmotion = false;
    //   	INVOKE_WCB(*window, Mouse, (GLUT_LEFT_BUTTON, GLUT_UP, x, y));
    //   } else if (action == AMOTION_EVENT_ACTION_MOVE && FETCH_WCB(*window, Motion)) {
    //   	INVOKE_WCB(*window, Motion, (x, y));
    //   }
    // }
    
    return EVENT_HANDLED;
  }

  /* Let Android handle other events (e.g. Back and Menu buttons) */
  return EVENT_NOT_HANDLED;
}

/**
 * Process the next main command.
 */
void handle_cmd(struct android_app* app, int32_t cmd) {
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    /* The system has asked us to save our current state.  Do so. */
    LOGI("handle_cmd: APP_CMD_SAVE_STATE");
    break;
  case APP_CMD_INIT_WINDOW:
    /* The window is being shown, get it ready. */
    LOGI("handle_cmd: APP_CMD_INIT_WINDOW");
    unique_window = app->window;
    /* glPlatformOpenWindow was waiting for Handle to be defined and
       will now return from fgPlatformProcessSingleEvent() */
    break;
  case APP_CMD_TERM_WINDOW:
    /* The window is being hidden or closed, clean it up. */
    LOGI("handle_cmd: APP_CMD_TERM_WINDOW");
    // fgDestroyWindow(fgDisplay.pDisplay.single_window);
    break;
  case APP_CMD_DESTROY:
    /* Not reached because GLUT exit()s when last window is closed */
    LOGI("handle_cmd: APP_CMD_DESTROY");
    break;
  case APP_CMD_GAINED_FOCUS:
    LOGI("handle_cmd: APP_CMD_GAINED_FOCUS");
    break;
  case APP_CMD_LOST_FOCUS:
    LOGI("handle_cmd: APP_CMD_LOST_FOCUS");
    break;
  case APP_CMD_CONFIG_CHANGED:
    /* Handle rotation / orientation change */
    LOGI("handle_cmd: APP_CMD_CONFIG_CHANGED");
    break;
  case APP_CMD_WINDOW_RESIZED:
    LOGI("handle_cmd: APP_CMD_WINDOW_RESIZED");
    // if (fgDisplay.pDisplay.single_window->Window.pContext.egl.Surface != EGL_NO_SURFACE)
    //   /* Make ProcessSingleEvent detect the new size, only available
    // 	 after the next SwapBuffer */
    //   // glutPostRedisplay();
    break;
  default:
    LOGI("handle_cmd: unhandled cmd=%d", cmd);
  }
}

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid()
:
myHandle(NULL)
{
    // Create a dummy window (with the fewest attributes -- it's just to have a valid support for an OpenGL context)

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and DO NOT make it active

    /* TODO: only one full-screen window possible? */
    if (unique_window != NULL)
	return;

    OpenDisplay();

    if (!CreateContext())
	return;

    /* Wait until window is available and OpenGL context is created */
    /* Normally events are processed through glutMainLoop(), but the
       user didn't call it yet, and the Android may not have initialized
       the View yet.  So we need to wait for that to happen. */
    /* We can't return from this function before the OpenGL context is
       properly made current with a valid surface. So we wait for the
       surface. */
    while (unique_window == NULL) {
	/* APP_CMD_INIT_WINDOW will do the job */
	ProcessEvents();
    }

  /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
   * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
   * As soon as we picked a EGLConfig, we can safely reconfigure the
   * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
  EGLint vid;
  eglGetConfigAttrib(ourDisplay, ourConfig,
		     EGL_NATIVE_VISUAL_ID, &vid);
  ANativeWindow_setBuffersGeometry(unique_window, 0, 0, vid);

  CreateEGLSurface(unique_window);
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(WindowHandle Handle, WindowSettings& Params)
{
    // Make sure we'll be able to catch all the events of the given window

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and make it active

    std::cerr << "WindowImplAndroid::WindowImplAndroid: STUB" << std::endl;
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params)
{
    // Create a new window with given size, title and style

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and make it active

    std::cerr << "WindowImplAndroid::WindowImplAndroid: STUB" << std::endl;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplAndroid::~WindowImplAndroid()
{
    // Destroy the OpenGL context, the window and every resource allocated by this class

    // Make sure Android glue isn't stripped.
    // JNI entry points need to be bundled even when linking statically
    // Placed in the destructor because it's always used
    ::app_dummy();
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplAndroid::IsContextActive()
{
    // Should return whether xxxGetCurrentContext() is NULL or not;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplAndroid::ProcessEvents()
{
  // Process every event for this window
  
  // Generate a sf::Event and call SendEvent(Evt) for each event

  static int32_t last_width = -1;
  static int32_t last_height = -1;

  /* When the screen is resized, the window handle still points to the
     old window until the next SwapBuffer, while it's crucial to set
     the size (onShape) correctly before the next onDisplay callback.
     Plus we don't know if the next SwapBuffer already occurred at the
     time we process the event (e.g. during onDisplay). */
  /* So we do the check each time rather than on event. */
  /* Interestingly, on a Samsung Galaxy S/PowerVR SGX540 GPU/Android
     2.3, that next SwapBuffer is fake (but still necessary to get the
     new size). */
  // SFG_Window* window = fgDisplay.pDisplay.single_window;
  // if (window != NULL && window->Window.Handle != NULL) {
  //   int32_t width = ANativeWindow_getWidth(window->Window.Handle);
  //   int32_t height = ANativeWindow_getHeight(window->Window.Handle);
  //   if (width != last_width || height != last_height) {
  //     last_width = width;
  //     last_height = height;
  //     LOGI("width=%d, height=%d", width, height);
  //     if( FETCH_WCB( *window, Reshape ) )
  // 	INVOKE_WCB( *window, Reshape, ( width, height ) );
  //     else
  // 	glViewport( 0, 0, width, height );
  //     glutPostRedisplay();
  //   }
  // }

  /* Read pending event. */
  int ident;
  int events;
  struct android_poll_source* source;
  /* This is called "ProcessSingleEvent" but this means we'd only
     process ~60 (screen Hz) mouse events per second, plus other ports
     are processing all events already.  So let's process all pending
     events. */
  /* if ((ident=ALooper_pollOnce(0, NULL, &events, (void**)&source)) >= 0) { */
  while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
    /* Process this event. */
    if (source != NULL) {
      source->process(source->app, source);
    }
  }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplAndroid::UseVerticalSync(bool Enabled)
{
    // Activate / deactivate vertical synchronization
    // usually using an OpenGL extension (should be a call to xxxSwapInterval)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplAndroid::ShowMouseCursor(bool Show)
{
    // Show or hide the system cursor in this window
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplAndroid::SetCursorPosition(unsigned int Left, unsigned int Top)
{
    // Change the cursor position (Left and Top are relative to this window)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplAndroid::SetPosition(int Left, int Top)
{
    // Change the window position
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
////////////////////////////////////////////////////////////
void WindowImplAndroid::SetSize(unsigned int Width, unsigned int Height)
{
    // Change the window size
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplAndroid::Show(bool State)
{
    // Show or hide the window
}

////////////////////////////////////////////////////////////
/// /see WindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplAndroid::EnableKeyRepeat(bool Enabled)
{
    // Enable or disable automatic key-repeat for keydown events
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplAndroid::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
    // Change all the necessary icons of the window (titlebar, task bar, ...) with the
    // provided array of 32 bits RGBA pixels
}


/*===========================================================
            STRATEGY FOR OPENGL CONTEXT CREATION

- If the requested level of anti-aliasing is not supported and is greater than 2, try with 2
  --> if level 2 fails, disable anti-aliasing
  --> it's important not to generate an error if anti-aliasing is not supported

- Use a matching pixel mode, or the best of all available pixel modes if no perfect match ;
  You should use the function EvaluateConfig to get a score for a given configuration

- Don't forget to fill Params (see constructors) back with the actual parameters we got from the chosen pixel format

- IMPORTANT : all OpenGL contexts must be shared (usually a call to xxxShareLists)

===========================================================*/


/*===========================================================
               STRATEGY FOR EVENT HANDLING

- Process any event matching with the ones in sf::Event::EventType
  --> Create a sf::Event, fill the members corresponding to the event type
  --> No need to handle joystick events, they are handled by WindowImpl::ProcessJoystickEvents
  --> Event::TextEntered must provide UTF-16 characters
      (see http://www.unicode.org/Public/PROGRAMS/CVTUTF/ for unicode conversions)
  --> Don't forget to process any destroy-like event (ie. when the window is destroyed externally)

- Use SendEvent function from base class to propagate the created events

===========================================================*/


} // namespace priv

} // namespace sf
