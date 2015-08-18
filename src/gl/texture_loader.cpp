
/*
 * src/gl/texture_loader.cpp
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

#include "texture_loader.h"

#include "log.h"
#include "image/astc.h"
#include "image/dds.h"
#include "image/png.h"

namespace be {

Texture* TextureLoader::loadTexture(const char *fileName) {
    Texture *texture = nullptr;
    // open stream
    std::ifstream stream(fileName, std::ifstream::binary);
    // check stream
    if (stream) {
        // load file into buffer
        const size_t length = fileSize(stream);
        char *buffer = new char[length];
        Log::i("reading image %s length: %d\n", fileName, length);
        stream.read(buffer, length);
        // create texture
        texture = new Texture();
        // check which format to load
        if (ASTC::isASTCFile(stream)) {
            Log::i("detected astc: %s\n", fileName);
            loadASTC(texture, buffer);
        } else if (DDS::isDDSFile(stream)) {
            Log::i("detected DDS: %s\n", fileName);
            loadDDS(texture, buffer);
        } else if (PNG::isPNGFile(stream)) {
            Log::i("detected PNG: %s\n", fileName);
            loadPNG(texture, buffer, length);
        } else {
            Log::w("unknown format");
        }
        delete [] buffer;
        stream.close();
    } else {
        Log::e("could not open file: %s\n", fileName);
    }
    return texture;
}

void TextureLoader::loadASTC(Texture *texture, char *buffer) {
        ASTC astc;
        // call respective image loading method
        astc.load(buffer, 0);
        GLenum glFormat;
        switch (astc.format()) {
            case ASTC::Format::ASTC_4x4:
//                glFormat = COMPRESSED_RGBA_ASTC_4x4_KHR;
                break;
            default:
                glFormat = 0;
        };

        if (!glFormat) {
            // bail out, we could not determine the format
            Log::w("unknwon format on ASTC texture");
            return;
        }

        texture->bind();


        glCompressedTexImage2D(GL_TEXTURE_2D,
                0,
                glFormat,
                astc.width(),
                astc.height(),
                0,
                astc.size(),
                static_cast<GLvoid*>(astc.data()));
        texture->unbind();
}

void TextureLoader::loadDDS(Texture *texture, char *buffer) {
    // DDS textures need to be flipped before being loaded into GL so that they
    // can be mapped to the UV origin which is 0, 0 in GL. Best is to have some
    // preprocessing with a tool like 'convert' in some kind of asset conditioning
    // pipeline
    DDS dds;
    const int kMagicNumberOffset = 4;
    dds.load(buffer, kMagicNumberOffset);
    GLint format;
    switch (dds.fourCC()) {
        case DDS::Type::DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            Log::i("dxt1");
            break;
        case DDS::Type::DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            Log::i("dxt3");
            break;
        case DDS::Type::DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            Log::i("dxt5\n");
            break;
        default:
            Log::w("unknown DXT format\n");
            return;
    }

    texture->bind();

    unsigned int width = dds.width();
    unsigned int height = dds.height();
    unsigned int blockSize = dds.blockSize();
    unsigned int offset = 0;

    Log::i("blocksize: %d\n", blockSize);

    // load mip maps
    
    for (unsigned int level = 0; level <= dds.mipMapCount() && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        Log::i("%u x %u, size: %u  offset: %d  0x%X\n", width, height, size, offset, (dds.data() + offset));

        if (dds.mipMapCount() <= 1) {
            // per default mipmap is set for filtering. Maybe do it the other way around?
            // anyhow, if mipmap filtering is activated and there are no mipmaps, GL will
            // turn off texturing
            texture->setMinFilter(GL_LINEAR);
        }

        glCompressedTexImage2D(GL_TEXTURE_2D, level, format,
                width, height, 0, size, (dds.data() + offset));

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            Log::e("GL error: %d 0x%x\n", err, err);
        }

        offset += size;
        // divide the texture size by 2
        width >>= 1;
        height >>= 1;

        // handle non-power of 2 textures
        if (width < 1) {
            width = 1;
        }
        if (height < 1) {
            height = 1;
        }

    } 

    texture->unbind();
}

void TextureLoader::loadPNG(Texture *texture, char *buffer, size_t length) {
    if (texture && buffer)
        Log::i("load png");
    PNG png;
    png.load(buffer, 0, length);

    GLint format;

    switch (png.colorType()) {
        case PNG_COLOR_TYPE_GRAY:
            format = GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            format = GL_LUMINANCE_ALPHA;
            break;
        default:
            Log::w("unknown format: %d\n", png.colorType());
            return;
    }

    texture->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, format, png.width(), png.height(), 0,
            format, GL_UNSIGNED_BYTE, png.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        // TODO: this needs improvement
        Log::e("GL error %u\n", err);
    }

    texture->unbind();
}

size_t TextureLoader::fileSize(std::ifstream &stream) {
    // determine file length
    stream.seekg(0, stream.end);
    const size_t fileLength = stream.tellg();
    stream.seekg(0, stream.beg);
    return fileLength;
}

} // end of namespace be

