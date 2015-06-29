#if defined(_WIN32) || defined(_WIN64)
#include "pyconfig_win.h"
#else
#include <bits/wordsize.h>

#if __WORDSIZE == 32
#include "pyconfig-32.h"
#elif __WORDSIZE == 64
#include "pyconfig-64.h"
#else
#error "Unknown word size"
#endif
#endif