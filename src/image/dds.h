
/*
 * src/image/dds.h
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

#ifndef BLACKELDER_DDS_H
#define BLACKELDER_DDS_H

#include <stddef.h>
#include <stdio.h>

#include <fstream>

namespace be {

class DDS {
public:
    enum class Type : unsigned int {
        INVALID = 0,
        DXT1 = 0x31545844,
        DXT3 = 0x33545844,
        DXT5 = 0x35545844,
    };

    DDS();
    ~DDS();

    static int isDDSFile(std::ifstream &stream);
    int read(std::ifstream &stream);
    void load(char *buffer, size_t offset);

    unsigned int width() const { return m_Width; }
    unsigned int height() const { return m_Height; }
    unsigned int mipMapCount() const { return m_MipMapCount; }
    unsigned int blockSize() const { return m_BlockSize; }
    Type fourCC() const { return m_FourCC; }
    char *data() const { return m_Data; }
private:

    unsigned int m_Width;
    unsigned int m_Height;
    unsigned int m_LinearSize;
    unsigned int m_MipMapCount;
    Type m_FourCC;
    unsigned int m_BlockSize;
    char *m_Data;
    size_t m_Size;
};

} // end of namespace be

#endif // BLACKELDER_DDS_H
