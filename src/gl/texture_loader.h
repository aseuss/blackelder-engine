
/*
 * src/gl/texture_loader.h
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

#ifndef BLACKELDER_TEXTURE_LOADER_H
#define BLACKELDER_TEXTURE_LOADER_H

#include "texture.h"

#include <fstream>

namespace be {

class TextureLoader {
public:
    static Texture* loadTexture(const char *fileName);
private:
    static size_t fileSize(std::ifstream &stream);
    static void loadASTC(Texture *texture, char *buffer);
    static void loadDDS(Texture *texture, char *buffer);
    static void loadPNG(Texture *texture, char *buffer, size_t length);
};

} // end of namespace be

#endif // BLACKELDER_TEXTURE_LOADER_H

