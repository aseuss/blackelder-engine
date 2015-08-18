
/*
 * src/memory_map.cpp
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

#include "memory_map.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace be {

MemoryMap::MemoryMap(const char *fileName) {
    int32_t file = open(fileName, O_RDONLY);
    struct stat fileStatus;
    fstat(file, &fileStatus);
    m_DataLength = fileStatus.st_size;
    m_Data = mmap(0, m_DataLength, PROT_READ, MAP_PRIVATE, file, 0);
    close(file);
}

MemoryMap::~MemoryMap() {
    munmap(m_Data, m_DataLength);
}

} // end of namespace be

