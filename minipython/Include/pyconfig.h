#if defined(_WIN32) || defined(_WIN64)
    #include "pyconfig_win.h"
#else
    #if defined(__unix__)
        #if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
            #if __SIZEOF_LONG__ == 8 || defined(__x86_64__)
                #include "pyconfig-bsd64.h"
            #elif __SIZEOF_LONG__ == 4
                #include "pyconfig-bsd32.h"
            #else
                #error "Unknown LONG size"
            #endif
        #else
            #if __SIZEOF_LONG__ == 8 || defined(__x86_64__)
                #include "pyconfig-64.h"
            #elif __SIZEOF_LONG__ == 4
                #include "pyconfig-32.h"
            #else
                #error "Unknown LONG size"
            #endif
        #endif
    #else
        #error "Unknown OS"
    #endif
#endif
