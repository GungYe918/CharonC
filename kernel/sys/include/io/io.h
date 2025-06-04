#pragma once
#ifdef __x86_64__
    #include "../../sys/arch/x86_64/io/io.h"
#elif defined(__aarch64__)
    #define NOT_IMPLEMENTED 1
#endif