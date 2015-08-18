
/*
 * src/image/astc.cpp
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

#include "astc.h"

#include "log.h"

namespace be {

struct ASTCHeader {
    unsigned char magic[4];
    unsigned char blockDimX;
    unsigned char blockDimY;
    unsigned char blockDimZ;
    unsigned char sizeX[3];
    unsigned char sizeY[3];
    unsigned char sizeZ[3];
};

static const size_t kFileCodeSize = 4;
static const uint8_t kASTCFileCode[] = { 0x13, 0xAB, 0xA1, 0x5C };

ASTC::ASTC() : 
    m_Format(Format::ASTC_UNKNOWN),
    m_Width(0),
    m_Height(0),
    m_Depth(0),
    m_Size(0),
    m_Data(nullptr) {}

int ASTC::isASTCFile(std::ifstream &stream) {
    char fileCode[kFileCodeSize];
    int hasASTCSignature = 0;

    if (!stream) {
        Log::e("inavlid stream\n");
        return -1;
    }
    stream.seekg(0, stream.beg);
    stream.read(fileCode, kFileCodeSize);
    hasASTCSignature = (memcmp(kASTCFileCode, fileCode, kFileCodeSize) == 0);

    return hasASTCSignature;
}

void ASTC::load(const char *buffer, size_t offset) {
    // map ASTC header onto buffer
    ASTCHeader *header = (ASTCHeader*)(buffer + offset);

    // reconstruct size values
    m_Width = header->sizeX[0] + (header->sizeX[1] << 8) + (header->sizeX[2] << 16);
    m_Height = header->sizeY[0] + (header->sizeY[1] << 8) + (header->sizeY[2] << 16);
    m_Depth = header->sizeZ[0] + (header->sizeZ[1] << 8) + (header->sizeZ[2] << 16);

    // compute number of blocks
    int blockX = (m_Width + header->blockDimX - 1) / header->blockDimX;
    int blockY = (m_Height + header->blockDimY - 1) / header->blockDimY;
    int blockZ = (m_Depth + header->blockDimZ - 1) / header->blockDimZ;

    Log::i("block x: %d  block y: %d  block z: %d\n", blockX, blockY, blockZ);
    Log::i("x: %f  y: %f\n", ceilf((float) m_Width / blockX), ceilf((float) m_Height / blockY));
    Log::i("block dim x: %d  block dim y: %d\n", header->blockDimX, header->blockDimY);

    m_Format = determineFormat(header->blockDimX, header->blockDimY);

    // each block has 16 encoded bytes
    m_Size = blockX * blockY * blockZ << 4;

    Log::i("%d x %d size: %d\n", m_Width, m_Height, m_Size);

    // leave out header data
    m_Data = (char*)&header[1];
}

ASTC::Format ASTC::determineFormat(uint8_t blockDimX, uint8_t blockDimY) {
    Format astcFormat;

    // we ignore block dimension z for now
    switch ((blockDimX << 8) | blockDimY) {
        case static_cast<uint16_t>(Format::ASTC_4x4):
            astcFormat = Format::ASTC_4x4;
            break;
        case static_cast<uint16_t>(Format::ASTC_5x4):
            astcFormat = Format::ASTC_5x4;
            break;
        case static_cast<uint16_t>(Format::ASTC_5x5):
            astcFormat = Format::ASTC_5x5;
            break;
        case static_cast<uint16_t>(Format::ASTC_6x5):
            astcFormat = Format::ASTC_6x5;
            break;
        case static_cast<uint16_t>(Format::ASTC_6x6):
            astcFormat = Format::ASTC_6x6;
            break;
        case static_cast<uint16_t>(Format::ASTC_8x5):
            astcFormat = Format::ASTC_8x5;
            break;
        case static_cast<uint16_t>(Format::ASTC_8x6):
            astcFormat = Format::ASTC_8x6;
            break;
        case static_cast<uint16_t>(Format::ASTC_8x8):
            astcFormat = Format::ASTC_8x8;
            break;
        case static_cast<uint16_t>(Format::ASTC_10x5):
            astcFormat = Format::ASTC_10x5;
            break;
        case static_cast<uint16_t>(Format::ASTC_10x6):
            astcFormat = Format::ASTC_10x6;
            break;
        case static_cast<uint16_t>(Format::ASTC_10x8):
            astcFormat = Format::ASTC_10x8;
            break;
        case static_cast<uint16_t>(Format::ASTC_10x10):
            astcFormat = Format::ASTC_10x10;
            break;
        case static_cast<uint16_t>(Format::ASTC_12x10):
            astcFormat = Format::ASTC_12x10;
            break;
        case static_cast<uint16_t>(Format::ASTC_12x12):
            astcFormat = Format::ASTC_12x12;
            break;
        default:
            astcFormat = Format::ASTC_UNKNOWN;
    };
    return astcFormat;
}

} // end of namespace be

