
/*
 * src/gl/texture.h
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

#ifndef BLACKELDER_TEXTURE_H
#define BLACKELDER_TEXTURE_H

#include "gl.h"

namespace be {

class Texture {
public:
    Texture();
    ~Texture();

    inline GLuint textureId() const { return m_TextureId; }

    inline void bind() const {
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }

    inline void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void setMinFilter(GLint filter) const;
    void setMagFilter(GLint filter) const;
private:
    GLuint m_TextureId;
};

} // end of namespace be

#endif // BLACKELDER_TEXTURE_H
