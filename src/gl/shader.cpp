
/*
 * src/gl/shader.cpp
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

#include "shader.h"
#include "log.h"

#include <fstream>

namespace be {

Shader::Shader(Type type) {
    GLenum shaderType;
    switch (type) {
        case Type::VERTEX:
            shaderType = GL_VERTEX_SHADER;
            break;
        case Type::FRAGMENT:
            shaderType = GL_FRAGMENT_SHADER;
            break;
    }
    m_ShaderId = glCreateShader(shaderType);
    Log::i("create shader: %d\n", m_ShaderId);
}

Shader::~Shader() {
    glDeleteShader(m_ShaderId);
}

void Shader::compileShader(GLuint shaderId, const GLchar *source) {
    Log::i("use shader id: %d\n", shaderId);
    if (source != nullptr) {
        GLint result = GL_FALSE;

        glShaderSource(shaderId, 1, &source, NULL);
        glCompileShader(shaderId);

        // check whether shader compiled fine
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
        if (!result) {
            GLint infoLogLength;

            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[sizeof(GLchar) * (infoLogLength + 1)];
            glGetShaderInfoLog(shaderId, infoLogLength, nullptr, infoLog);
            Log::e("%s\n", infoLog);
            delete [] infoLog;
        }
    }
}

GLchar* Shader::readSourceFromFile(const char *fileName) {
    GLchar *buffer = nullptr;

    std::ifstream stream(fileName, std::ifstream::binary);
    if (stream) {
        // figure out file length
        stream.seekg(0, stream.end);
        const int length = stream.tellg();
        stream.seekg(0, stream.beg);

        buffer = new GLchar[length + 1];
        Log::i("reading shader source...\n");
        stream.read(buffer, length);
        buffer[length] = 0;

        // TODO: improve error handling
        stream.close();
    }
    return buffer;
}

GLuint Shader::load(const char *fileName) {
    const GLchar *shaderSource = readSourceFromFile(fileName);
    compileShader(m_ShaderId, shaderSource);
    delete [] shaderSource;

    return m_ShaderId;
}

} // end of namespace be
