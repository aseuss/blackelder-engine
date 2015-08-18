
/*
 * src/camera2d.cpp
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

#include "camera2d.h"
#include "log.h"

#include <glm/gtc/matrix_transform.hpp>

namespace be {

Camera2D::Camera2D(float frustumWidth, float frustumHeight)
    : m_FrustumWidth(frustumWidth),
      m_FrustumHeight(frustumHeight),
      m_Zoom(1.0f) {

    position = glm::vec2(frustumWidth / 2, frustumHeight / 2);
    const float kLeft = position.x - m_FrustumWidth * m_Zoom / 2;
    const float kRight = position.x + m_FrustumWidth * m_Zoom / 2;
    const float kBottom = position.y - m_FrustumHeight * m_Zoom / 2;
    const float kTop = position.y + m_FrustumHeight * m_Zoom / 2;
    projectionMatrix = glm::ortho(kLeft, kRight, kBottom, kTop, 1.0f, -1.0f);
}

} // end of namespace be

