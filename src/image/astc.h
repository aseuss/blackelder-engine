
/*
 * src/astc.h
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

#ifndef BLACKELDER_ASTC_H
#define BLACKELDER_ASTC_H

#include <stddef.h>
#include <fstream>

namespace be {

class ASTC {
public:
    ASTC();

    enum class Format : uint16_t {
        ASTC_4x4   =  (4 << 8) |  (4),
        ASTC_5x4   =  (5 << 8) |  (4),
        ASTC_5x5   =  (5 << 8) |  (5),
        ASTC_6x5   =  (6 << 8) |  (5),
        ASTC_6x6   =  (6 << 8) |  (6),
        ASTC_8x5   =  (8 << 8) |  (5),
        ASTC_8x6   =  (8 << 8) |  (6),
        ASTC_8x8   =  (8 << 8) |  (8),
        ASTC_10x5  = (10 << 8) |  (5),
        ASTC_10x6  = (10 << 8) |  (6),
        ASTC_10x8  = (10 << 8) |  (8),
        ASTC_10x10 = (10 << 8) | (10),
        ASTC_12x10 = (12 << 8) | (10),
        ASTC_12x12 = (12 << 8) | (12),
        ASTC_UNKNOWN = 0
    };

    Format format() const { return m_Format; }
    size_t width() const { return m_Width; }
    size_t height() const { return m_Height; }
    size_t depth() const { return m_Depth; }
    size_t size() const { return m_Size; }
    char* data() const { return m_Data; }

    void load(const char *buffer, size_t offset);
    static int isASTCFile(std::ifstream &stream);

private:
    static ASTC::Format determineFormat(uint8_t blockDimX, uint8_t blockDimY);
    Format m_Format;
    size_t m_Width;
    size_t m_Height;
    size_t m_Depth;
    size_t m_Size;
    char *m_Data;
};

} // end of namespace be

#endif // end of BLACKELDER_ASTC_H
