#pragma once

// @formatter:off

#define ARNOLD_API

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
#endif

#define AE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)
