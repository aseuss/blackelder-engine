
/*
 * src/image/png.cpp
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

#include "png.h"
#include <png.h>

#include "log.h"

namespace be {

static const png_size_t kFileCodeSize = 8;

struct DataHandle {
    const png_byte *data;
    const png_size_t size;
};

struct ReadDataHandle {
    const DataHandle data;
    png_size_t offset;
};

PNG::~PNG() {
    delete [] m_Data;
}

int PNG::isPNGFile(std::ifstream &stream) {
    char fileCode[kFileCodeSize];

    stream.seekg(0, stream.beg);
    stream.read(fileCode, kFileCodeSize);

    //return (png_sig_cmp((png_bytep) fileCode, 0, kFileCodeSize) == 0);
    return hasPngSignature(fileCode, 0);
}

bool PNG::hasPngSignature(const char *buffer, size_t offset) {
    return (png_sig_cmp((png_bytep) buffer, offset, kFileCodeSize) == 0);
}
    
void PNG::readPngDataCallback(png_structp pngPtr, png_byte* rawData, png_size_t length) {
    ReadDataHandle *handle = static_cast<ReadDataHandle*>(png_get_io_ptr(pngPtr));
    const png_byte *pngSrc = handle->data.data + handle->offset;

    memcpy(rawData, pngSrc, length);
    handle->offset += length;
}

void PNG::readAndUpdateInfo(const png_structp pngPtr, const png_infop infoPtr) {
    png_read_info(pngPtr, infoPtr);
    
    m_Width = png_get_image_width(pngPtr, infoPtr);
    m_Height = png_get_image_height(pngPtr, infoPtr);
    m_ColorType = png_get_color_type(pngPtr, infoPtr);

    unsigned int bitdepth = png_get_bit_depth(pngPtr, infoPtr);

    // convert transparency to full alpha
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
    }

    // convert grayscale if needed
    if (m_ColorType == PNG_COLOR_TYPE_GRAY && bitdepth < 8) {
        png_set_expand_gray_1_2_4_to_8(pngPtr);
    }

    // convert paletted images if needed
    if (m_ColorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(pngPtr);
    }

    // add alpha channel if there is none
    // GL_RGBA is supposed to be faster than RGB on many GPUs
    if (m_ColorType == PNG_COLOR_TYPE_PALETTE || m_ColorType == PNG_COLOR_TYPE_RGB) {
        png_set_add_alpha(pngPtr, 0xff, PNG_FILLER_AFTER);
    }

    // make sure packing is 8-bit
    if (bitdepth < 8) {
        png_set_packing(pngPtr);
    } else if (bitdepth == 16) {
        // TODO: fix this
        //png_set_scale_16(pngPtr);
    }

    png_read_update_info(pngPtr, infoPtr);

    // get new color type after update
    m_ColorType = png_get_color_type(pngPtr, infoPtr);
}

void PNG::readPng(const png_structp pngPtr, const png_infop infoPtr) {
    png_size_t rowSize = png_get_rowbytes(pngPtr, infoPtr);
    m_Size = rowSize * m_Height;

    m_Data = new unsigned char[m_Size];

    png_byte *rowPtrs[m_Height];

    for (unsigned int i = 0; i < m_Height; ++i) {
        // when reading a PNG from file it's upside down. For GL
        // we need to invert it to generate the texture correctly.
        rowPtrs[m_Height - i] = m_Data + i * rowSize;
    }

    png_read_image(pngPtr, &rowPtrs[0]);
}

void PNG::load(const char *buffer, size_t offset, size_t length) {
    png_structp pngPtr;
    png_infop infoPtr;

    if (buffer == nullptr) {
        Log::e("invalid buffer\n");
        return;
    }

    if (!hasPngSignature(buffer, 0)) {
        Log::e("not a png\n");
        return;
    }

    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) {
        Log::e("could not allocate png read struct.\n");
        return;
    }

    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) {
        png_destroy_read_struct(&pngPtr, NULL, NULL);
        Log::e("could not create png info struct.\n");
        return;
    }

    const png_byte* dataPtr = reinterpret_cast<const png_byte*>(buffer + offset);
    ReadDataHandle pngDataHandle = (ReadDataHandle) {{dataPtr, length}, 0};
    png_set_read_fn(pngPtr, &pngDataHandle, readPngDataCallback);

    if (setjmp(png_jmpbuf(pngPtr))) {
        //png_destroy_read_struct(&pngPtr, &infoPtr, &endInfoPtr);
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
        return;
    }

    readAndUpdateInfo(pngPtr, infoPtr);
    readPng(pngPtr, infoPtr);

    // finally free resources
    png_read_end(pngPtr, infoPtr);
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
}

} // end of namespace be

