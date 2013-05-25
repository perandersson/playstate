#pragma once

// Definition for the playstate version
#define playstate_VERSION 1

// Platform recognition
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
  #define playstate_WINRT 1
#elif defined(WIN32) || defined(_WIN32)
  #define playstate_WIN32 1
#else
  #define playstate_LINUX 1
#endif

#if defined(_MSC_VER)
  #if defined(playstate_STATICLIB)
    #define playstate_API
  #else
    #if defined(playstate_SOURCE)
      #define playstate_API __declspec(dllexport)
    #else
      #define playstate_API __declspec(dllimport)
    #endif
  #endif

#elif defined(__GNUC__)
  #if defined(playstate_STATICLIB)
    #define playstate_API
  #else
    #if defined(playstate_SOURCE)
      #define playstate_API __attribute__ ((visibility ("default")))
    #else
      // If you use -fvisibility=hidden in GCC, exception handling and RTTI
      // would break if visibility wasn't set during export _and_ import
      // because GCC would immediately forget all type infos encountered.
      // See http://gcc.gnu.org/wiki/Visibility
      #define playstate_API __attribute__ ((visibility ("default")))
    #endif
  #endif

#else

  #error Unknown compiler, please implement shared library macros

#endif