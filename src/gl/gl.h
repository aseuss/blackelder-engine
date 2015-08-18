
/*
 * src/gl/gl.h
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

#ifndef BLACKELDER_GL_H
#define BLACKELDER_GL_H

// TODO: this is required to compile, use glew instead?
#define GL_GLEXT_PROTOTYPES 1

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#endif // BLACKELDER_GL_H
