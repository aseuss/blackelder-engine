
/*
 * src/context.h
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

#ifndef BLACKELDER_CONTEXT_H
#define BLACKELDER_CONTEXT_H

#include <SDL2/SDL.h>

namespace be {

class Context {
public:
    Context(const char *appTitle);
    ~Context();
    bool isValid() const { return m_IsValid; }
    void swap() const;
private:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    bool m_IsValid;
};

} // end of namespace be


#endif // BLACKELDER_CONTEXT_H
