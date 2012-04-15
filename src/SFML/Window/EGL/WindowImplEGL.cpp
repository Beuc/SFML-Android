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
#include <SFML/Window/EGL/WindowImplEGL.hpp>
#include <SFML/Window.hpp>  // for OpenGL headers

namespace sf
{
namespace priv
{

EGLDisplay     WindowImplEGL::ourDisplay          = EGL_NO_DISPLAY;
EGLConfig      WindowImplEGL::ourConfig           = NULL;
unsigned int   WindowImplEGL::ourWindowsCount     = 0;
  
////////////////////////////////////////////////////////////
/// Open the display (if not already done)
///
/// \param AddWindow : Tell whether or not we must increase the windows count
///
/// \return True if the display is properly opened
///
////////////////////////////////////////////////////////////
bool WindowImplEGL::OpenDisplay(bool AddWindow)
{
    std::cerr << "WindowImplEGL::OpenDisplay" << std::endl;


    // If no display has been opened yet, open it
    if (ourDisplay == EGL_NO_DISPLAY)
    {
	/* Using EGL_DEFAULT_DISPLAY, or a specific native display */
	EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
	ourDisplay = eglGetDisplay(nativeDisplay);

        if (ourDisplay != EGL_NO_DISPLAY)
        {
	    if (!eglInitialize(ourDisplay, NULL, NULL))
	    {
# ifdef GL_VERSION_1_1  /* or later */
		eglBindAPI(EGL_OPENGL_API);
# else
		eglBindAPI(EGL_OPENGL_ES_API);
# endif
	    }
	    else
	    {
		std::cerr << "eglInitialize: error " << std::hex << eglGetError() << std::endl;
	    }
        }
        else
        {
            std::cerr << "Failed to open an EGL connection" << std::endl;
        }
    }

    // Increase the number of windows
    if (AddWindow)
        ourWindowsCount++;

    return ourDisplay != EGL_NO_DISPLAY;
}

////////////////////////////////////////////////////////////
/// Close the display
///
////////////////////////////////////////////////////////////
void WindowImplEGL::CloseDisplay()
{
    // Decrease the number of windows
    ourWindowsCount--;

    // If all windows have been destroyed, then we can close the display
    if (ourWindowsCount == 0)
    {
	if (ourDisplay != EGL_NO_DISPLAY) {
	    eglTerminate(ourDisplay);
	    ourDisplay = EGL_NO_DISPLAY;
	}
    }
}


////////////////////////////////////////////////////////////
/// Create EGL Surface from a native window handle
////////////////////////////////////////////////////////////
void WindowImplEGL::CreateEGLSurface(WindowHandle Handle)
{
    std::cerr << "WindowImplEGL::CreateEGLSurface" << std::endl;

    // - Make sure we'll be able to catch all the events of the given window
    // - Initialize myWidth and myHeight members from base class with the window size
    // - Create an OpenGL context in this window and make it active
    mySurface = eglCreateWindowSurface(ourDisplay, ourConfig, Handle, NULL);
    if (mySurface == EGL_NO_SURFACE)
	std::cerr << "Cannot create EGL window surface, err=" << std::hex << eglGetError() << std::endl;
    // Done by Window::Initialize():
    // SetActive(true);
    
    // TODO: test if we get the right size
    EGLint w, h;
    eglQuerySurface(ourDisplay, mySurface, EGL_WIDTH, &w);
    eglQuerySurface(ourDisplay, mySurface, EGL_HEIGHT, &h);
    myWidth = w;
    myHeight = h;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplEGL::~WindowImplEGL()
{
    // Destroy the OpenGL context, the window and every resource allocated by this class
    SetActive(false);

    if (myGLContext != EGL_NO_CONTEXT) {
	eglDestroyContext(ourDisplay, myGLContext);
	myGLContext = EGL_NO_CONTEXT;
    }
    
    if (mySurface != EGL_NO_SURFACE) {
	eglDestroySurface(ourDisplay, mySurface);
	mySurface = EGL_NO_SURFACE;
    }
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplEGL::IsContextActive()
{
    // Should return whether xxxGetCurrentContext() is NULL or not;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplEGL::Display()
{
    // Swap OpenGL buffers (should be a call to xxxSwapBuffers)
    if (!eglSwapBuffers(ourDisplay, mySurface))
        std::cerr << "eglSwapBuffers: error " << std::hex << eglGetError() << std::endl;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetActive
////////////////////////////////////////////////////////////
void WindowImplEGL::SetActive(bool Active) const
{
    if (ourDisplay != EGL_NO_DISPLAY)
    {
	// Bind / unbind OpenGL context (should be a call to xxxMakeCurrent)
	if (Active)
        {
	    if (!eglMakeCurrent(ourDisplay, mySurface, mySurface, myGLContext))
		std::cerr << "eglMakeCurrent: error " << std::hex << eglGetError() << std::endl;
	}
	else
	{
	    eglMakeCurrent(ourDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	}
    }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplEGL::UseVerticalSync(bool Enabled)
{
    // Activate / deactivate vertical synchronization
    // usually using an OpenGL extension (should be a call to xxxSwapInterval)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplEGL::ShowMouseCursor(bool Show)
{
    // Show or hide the system cursor in this window
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplEGL::SetCursorPosition(unsigned int Left, unsigned int Top)
{
    // Change the cursor position (Left and Top are relative to this window)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplEGL::SetPosition(int Left, int Top)
{
    // Change the window position
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
////////////////////////////////////////////////////////////
void WindowImplEGL::SetSize(unsigned int Width, unsigned int Height)
{
    // Change the window size
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplEGL::Show(bool State)
{
    // Show or hide the window
}

////////////////////////////////////////////////////////////
/// /see WindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplEGL::EnableKeyRepeat(bool Enabled)
{
    // Enable or disable automatic key-repeat for keydown events
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplEGL::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
    // Change all the necessary icons of the window (titlebar, task bar, ...) with the
    // provided array of 32 bits RGBA pixels
}

////////////////////////////////////////////////////////////
/// Define context config
////////////////////////////////////////////////////////////
void WindowImplEGL::ChooseConfig()
{
    const EGLint attribs[] = {
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
#ifdef GL_ES_VERSION_2_0
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#elif GL_VERSION_ES_CM_1_0 || GL_VERSION_ES_CL_1_0 || GL_VERSION_ES_CM_1_1 || GL_VERSION_ES_CL_1_1
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
#else
	EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
#endif
	EGL_BLUE_SIZE, 1,
	EGL_GREEN_SIZE, 1,
	EGL_RED_SIZE, 1,
	EGL_ALPHA_SIZE, 1,
	EGL_DEPTH_SIZE, 1,
	EGL_STENCIL_SIZE, 0,
	EGL_SAMPLE_BUFFERS, 0,
	EGL_SAMPLES, 0,
	EGL_NONE
    };
    
    EGLint num_config;
    if (!eglChooseConfig(ourDisplay,
			 attribs, &ourConfig, 1, &num_config)) {
	std::cerr << "eglChooseConfig: error " << std::hex << eglGetError() << std::endl;
	return;
    }
    
    // TODO: choose best configuration
    // Evaluate the current configuration
    //int Color = Red + Green + Blue + Alpha;
    //int Score = EvaluateConfig(Mode, Params, Color, Depth, Stencil, MultiSampling ? Samples : 0);
}

////////////////////////////////////////////////////////////
/// Create EGL Context
///
////////////////////////////////////////////////////////////
bool WindowImplEGL::CreateContext()
{
    ChooseConfig();
    
    /* Ensure OpenGLES 2.0 context */
    static const EGLint ctx_attribs[] = {
#ifdef GL_ES_VERSION_2_0
	EGL_CONTEXT_CLIENT_VERSION, 2,
#elif GL_VERSION_ES_CM_1_0 || GL_VERSION_ES_CL_1_0 || GL_VERSION_ES_CM_1_1 || GL_VERSION_ES_CL_1_1
	EGL_CONTEXT_CLIENT_VERSION, 1,
#endif
	EGL_NONE
    };
    myGLContext = eglCreateContext(ourDisplay, ourConfig, EGL_NO_CONTEXT, ctx_attribs);
    if (myGLContext == EGL_NO_CONTEXT) {
	std::cerr << "Cannot initialize EGL context, err=" << std::hex << eglGetError() << std::endl;
	return false;
    }
    EGLint ver = -1;
    eglQueryContext(ourDisplay, myGLContext, EGL_CONTEXT_CLIENT_VERSION, &ver);
    if (ver != 
#ifdef GL_ES_VERSION_2_0
	2
#else
	1
#endif
	) {
	    std::cerr << "Wrong GLES major version: " << ver << std::endl;
	    return false;
	}
    return true;
}

} // namespace priv

} // namespace sf
