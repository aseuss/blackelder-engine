
/*
 * src/image/png.h
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

#ifndef BLACKELDER_PNG_H
#define BLACKELDER_PNG_H

#include <fstream>

#include <stdio.h>
#include <png.h>

namespace be {

class PNG {
public:
    ~PNG();

    static int isPNGFile(std::ifstream &stream);
    void load(const char *buffer, size_t offset, size_t length);

    unsigned int width() const { return m_Width; }
    unsigned int height() const { return m_Height; }
    unsigned int size() const { return m_Size; }
    unsigned int colorType() { return m_ColorType; }
    unsigned char *data() const { return m_Data; }

private:
    static bool hasPngSignature(const char *buffer, size_t offset);
    static void readPngDataCallback(png_structp pngPtr, png_byte* rawData, png_size_t length);
    void readPng(const png_structp pngPtr, const png_infop infoPtr);
    void readAndUpdateInfo(const png_structp pngPtr, const png_infop infoPtr);

    unsigned int m_Width;
    unsigned int m_Height;
    unsigned int m_Size;
    unsigned int m_ColorType;
    unsigned char *m_Data;
};

} // end of namespace be

#endif // BLACKELDER_PNG_H
