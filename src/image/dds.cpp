
/*
 * src/image/dds.cpp
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

#include "dds.h"

#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace be {

//const unsigned int kFourCCDXT1 = 0x31545844;
//const unsigned int kFourCCDXT3 = 0x33545844;
//const unsigned int kFourCCDXT5 = 0x35545844;

static const size_t kHeaderSize = 124;
static const size_t kFileCodeSize = 4;
static const char* kDDSFileCode = "DDS ";

/*
int isDDSFile(FILE *filePtr) {
    char fileCode[kFileCodeSize];
    int hasDDSSignature = 0;

    if (filePtr == NULL) {
        printf("invalid file pointer.\n");
        return -1;
    }

    // verify that we have a DDS file. Need to reset file ptr in case it does
    // not point to the beginngin of the file.
    fseek(filePtr, 0, SEEK_SET);
    fread(fileCode, 1, kFileCodeSize, filePtr);
    hasDDSSignature = (strncmp(fileCode, kDDSFileCode, strlen(kDDSFileCode)) == 0);

    return hasDDSSignature;
}
*/

DDS::DDS() : m_BlockSize(16), m_Data(nullptr), m_Size(0) {}

DDS::~DDS() {
    // we do not own the data
    //delete [] m_Data;
    Log::i("d'tor DDS\n");
}

int DDS::isDDSFile(std::ifstream &stream) {
    char fileCode[kFileCodeSize];
    int hasDDSSignature = 0;

    if (!stream) {
        Log::e("invalid input stream.\n");
        return -1;
    }

    // verify that we have a DDS file. Need to reset stream in case it does
    // not point to the beginning of the file.
    stream.seekg(0, stream.beg);
    stream.read(fileCode, kFileCodeSize);

    hasDDSSignature = (strncmp(fileCode, kDDSFileCode, strlen(kDDSFileCode)) == 0);

    return hasDDSSignature;
}

struct DDSPixelFormat {
    unsigned int size;
    unsigned int flags;
    unsigned int fourCC;
    unsigned int rgbBitCount;
    unsigned int redBitMask;
    unsigned int greenBitMask;
    unsigned int blueBitMask;
    unsigned int alphaBitMask;
};

struct DDSHeader {
    unsigned int headerSize;
    unsigned int flags;
    unsigned int height;
    unsigned int width;
    unsigned int linearSize;
    unsigned int depth;
    unsigned int mipMapCount;
    unsigned int reserved1[11];
    struct DDSPixelFormat pixelFormat;
    unsigned int caps;
    unsigned int caps2;
    unsigned int caps3;
    unsigned int caps4;
    unsigned int reserved2;
};

void DDS::load(char *buffer, size_t offset) {
    DDSHeader *header = (DDSHeader*)(buffer + offset);

    m_Width = header->width;
    m_Height = header->height;
    m_LinearSize = header->linearSize;
    m_MipMapCount = header->mipMapCount;
    const unsigned int fourCC = header->pixelFormat.fourCC;

    Log::i("header size: %d  fourCC: %X\n", header->headerSize, header->pixelFormat.fourCC);

    if (fourCC == static_cast<unsigned int>(Type::DXT1)) {
        //format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        m_BlockSize = 8;
        m_FourCC = Type::DXT1;
    //} else if (m_FourCC == kFourCCDXT3) {
    } else if (fourCC == static_cast<unsigned int>(Type::DXT3)) {
        m_FourCC = Type::DXT3;
        //format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    //} else if (fourCC == kFourCCDXT5) {
    } else if (fourCC == static_cast<unsigned int>(Type::DXT5)) {
        m_FourCC = Type::DXT5;
        //format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    } else {
        m_FourCC = Type::INVALID;
        Log::w("unknown fourCC 0x%x\n", m_FourCC);
        return;
    }

    Log::i("%u x %u size: %u mipmaps: %u fourCC: 0x%x\n",
        m_Width, m_Height, m_LinearSize, m_MipMapCount, m_FourCC);

    int hoffset = (offset + kHeaderSize);
    Log::i("data offset: %d 0x%X 0x%X\n", hoffset, buffer, (buffer + hoffset));
    // TODO: add calculation of data pointer and set it
    m_Data = (buffer + offset + kHeaderSize);
}

int DDS::read(std::ifstream &stream) {
    char header[kHeaderSize];
    int hasDDSSignature = 0;

    if (!stream) {
        Log::e("invalid file stream\n");
        return -1;
    }

    // verify that we have a DDS file
    hasDDSSignature = DDS::isDDSFile(stream);
    if (hasDDSSignature != 1) {
        Log::e("not a DDS file.\n");
        return -1;
    }

    stream.read(header, kHeaderSize);

    m_Height = *(unsigned int*)&(header[8]);
    m_Width = *(unsigned int*)&(header[12]);
    m_LinearSize = *(unsigned int*)&(header[16]);
    m_MipMapCount = *(unsigned int*)&(header[24]);
    //m_FourCC = *(unsigned int*)&(header[80]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);
    if (fourCC == static_cast<unsigned int>(Type::DXT1)) {
        //format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        m_BlockSize = 8;
        m_FourCC = Type::DXT1;
    //} else if (m_FourCC == kFourCCDXT3) {
    } else if (fourCC == static_cast<unsigned int>(Type::DXT3)) {
        m_FourCC = Type::DXT5;
        //format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    //} else if (fourCC == kFourCCDXT5) {
    } else if (fourCC == static_cast<unsigned int>(Type::DXT5)) {
        m_FourCC = Type::DXT5;
        //format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    } else {
        m_FourCC = Type::INVALID;
        Log::w("unknown fourCC 0x%x\n", m_FourCC);
        return false;
    }

    Log::i("%u x %u size: %u mipmaps: %u fourCC: 0x%x\n",
        m_Width, m_Height, m_LinearSize, m_MipMapCount, m_FourCC);

    // check size of image including mip maps
    if (m_MipMapCount > 1) {
        m_Size = m_LinearSize * 2;
    } else {
        m_Size = m_LinearSize;
    }

    m_Data = new char[m_Size * sizeof(char)];
    if (m_Data == nullptr) {
        Log::e("could not allocate memory for dds image data.\n");
        //free(*image);
        //*image = NULL;
    }
    stream.read(m_Data, m_Size);

    return 0;
}

/*
int readDDS(FILE *filePtr, dds_t **image) {
    unsigned char header[kHeaderSize];
    int hasDDSSignature = 0;

    if (filePtr == NULL) {
        printf("invalid file pointer.\n");
        return -1;
    }

    // verify that we have a DDS file
    hasDDSSignature = isDDSFile(filePtr);
    if (hasDDSSignature != 1) {
        printf("not a DDS file.\n");
        return -1;
    }

    *image = (dds_t*) malloc(sizeof(dds_t));
    if (*image) {
        (*image)->data = NULL;
    } else {
        printf("could not allocate memory.\n");
        return -1;
    }

    // get the surface description
    fread(&header, kHeaderSize, 1, filePtr);

    (*image)->height = *(unsigned int*)&(header[8]);
    (*image)->width = *(unsigned int*)&(header[12]);
    (*image)->linearSize = *(unsigned int*)&(header[16]);
    (*image)->mipMapCount = *(unsigned int*)&(header[24]);
    (*image)->fourCC = *(unsigned int*)&(header[80]);

    printf("%u x %u size: %u mipmaps: %u fourCC: 0x%x\n",
            (*image)->width, (*image)->height, (*image)->linearSize, (*image)->mipMapCount, (*image)->fourCC);

    // check size of image including mip maps
    if ((*image)->mipMapCount > 1) {
       (*image)->size = (*image)->linearSize * 2;
    } else {
       (*image)->size = (*image)->linearSize;
    }

    (*image)->data = (unsigned char*) malloc((*image)->size * sizeof(unsigned char));
    if ((*image)->data == NULL) {
        printf("could not allocate memory for dds image data.\n");
        free(*image);
        *image = NULL;
    }
    fread((*image)->data, 1, (*image)->size, filePtr);

    return 0;
}
*/

} // end of namespace be

