
/*
 * src/log.h
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

#ifndef BLACKELDER_LOG_H
#define BLACKELDER_LOG_H

#include <SDL2/SDL.h>

// TODO: implement log levels, tags and timestamps. See C++11 parameter_packs

namespace be {

/* for android we need something like this:
 * #include <android/log.h>
 * #define LOG_TAG "cranberry"
 * #define LOG_DEBUG(...) ((void) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
 * #define LOG_INFO(...) ((void) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
 * #define LOG_VERBOSE(...) ((void) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
 * #define LOG_WARN(...) ((void) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
 * #define LOG_ERROR(...) ((void) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
 */

#define LOG_DEBUG(format, ...) (SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, format, __VA_ARGS__))
#define LOG_INFO(format, ...) (SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, __VA_ARGS__))
#define LOG_VERBOSE(format, ...) (SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, format, __VA_ARGS__))
#define LOG_WARN(format, ...) (SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, format, __VA_ARGS__))
#define LOG_ERROR(format, ...) (SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, format, __VA_ARGS__))

class Log {
public:
    template<class ... T> inline static void d(const char *format, T ... args) {
        LOG_DEBUG(format, args...);
    }

    template<class ... T> inline static void i(const char *format, T ... args) {
        LOG_INFO(format, args...);
    }

    template<class ... T> inline static void v(const char *format, T ... args) {
        LOG_VERBOSE(format, args...);
    }

    template<class ... T> inline static void w(const char *format, T ... args) {
        LOG_WARN(format, args...);
    }

    template<class ... T> inline static void e(const char *format, T ... args) {
        LOG_ERROR(format, args...);
    }
};

} // end of namespace be

#endif // BLACKELDER_LOG_H
