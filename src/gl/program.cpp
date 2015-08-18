
/*
 * src/gl/program.cpp
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

#include "program.h"

#include "log.h"

namespace be {

Program::Program() {
    m_ProgramId = glCreateProgram();
    Log::i("create program: %d\n", m_ProgramId);
}

Program::~Program() {
    glDeleteProgram(m_ProgramId);
}
    
void Program::use() const {
    glUseProgram(m_ProgramId);
}

GLuint Program::link(const Shader &vertex, const Shader &fragment) {
    GLint result = GL_FALSE;

    Log::i("linking program\n");
    glAttachShader(m_ProgramId, vertex.shaderId());
    glAttachShader(m_ProgramId, fragment.shaderId());
    glLinkProgram(m_ProgramId);

    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &result);
    if (!result) {
        GLint infoLogLength;
        glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[sizeof(GLchar) * (infoLogLength + 1)];
        glGetProgramInfoLog(m_ProgramId, infoLogLength, nullptr, infoLog);
        Log::e("%s\n", infoLog);
        delete [] infoLog;
    }

    return m_ProgramId;
}

} // end of namespace be

