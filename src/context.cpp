
/*
 * src/context.cpp
 *
 * Copyright 2014,2015 Andreas Seuss
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * */

#include "context.h"

#define GL_GLEXT_PROTOTYPES 1

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace be {

Context::Context(const char *appTitle)
        : m_Window(nullptr),
          m_GlContext(0),
          m_IsValid(false) {

    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
        return;
    }
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_EGL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);

    int width = mode.w;
    int height = mode.h;

    SDL_Log("Width = %d, Height = %d.\n", width, height);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    // SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);

    m_Window = SDL_CreateWindow(appTitle, 0, 0, 1024, 768, SDL_WINDOW_OPENGL //| SDL_WINDOW_FULLSCREEN
        | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (m_Window == 0) {
        SDL_Log("Failed to create window.");
        SDL_Quit();
        return;
    }

    // create Open GL context
    m_GlContext = SDL_GL_CreateContext(m_Window);
    if (m_GlContext) {
        SDL_Log("ok, got a GL context\n");
        m_IsValid = true;
    } else {
        SDL_Log("fucked up creating a GL context\n");
    }
}

Context::~Context() {
    SDL_GL_DeleteContext(m_GlContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void Context::swap() const {
    SDL_GL_SwapWindow(m_Window); 
}

} // end of namespace be

