#pragma once

#ifdef _WIN32
    #ifdef ARNOLDENGINE_BUILD_DLL
        #define ARNOLD_API __declspec(dllexport)
    #else
        #define ARNOLD_API __declspec(dllimport)
    #endif
#elif defined(__APPLE__) || defined(__linux__)
    #ifdef ARNOLDENGINE_BUILD_DLL
        #define ARNOLD_API __attribute__((visibility("default")))
    #else
        #define ARNOLD_API
    #endif
#else
    #define ARNOLD_API
#endif

#define AE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// #ifdef AE_ENABLE_ASSERTS
//     #define AE_ASSERT(x, ...) { if(!(x)) { AE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//     #define AE_CORE_ASSERT(x, ...) { if(!(x)) { AE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
// #else
//     #define AE_ASSERT(x, ...)
//     #define AE_CORE_ASSERT(x, ...)
// #endif


#define BIT(x) (1 << x)

