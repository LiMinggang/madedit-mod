#if defined(_WIN32) || defined(_WIN64)
    #include "pyconfig_win.h"
#else
    #if defined(__unix__)
        #if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
            #if __SIZEOF_LONG__ == 8 || defined(__x86_64__)
                #include "pyconfig-bsd64.h"
            #elif __SIZEOF_LONG__ == 4 || defined(__i386__)
                #include "pyconfig-bsd32.h"
            #else
                #error "Unknown LONG size"
            #endif
        #else
            #if defined(__aarch64__) || defined(__arm__)
                #if __SIZEOF_LONG__ == 8
                    #include "pyconfig-arm64.h"
                #elif __SIZEOF_LONG__ == 4 || defined(__i386__)
                    #include "pyconfig-arm32.h"
                #else
                    #error "Unknown LONG size"
                #endif
            #elif __SIZEOF_LONG__ == 8 || defined(__x86_64__)
                #include "pyconfig-64.h"
            #elif __SIZEOF_LONG__ == 4 || defined(__i386__)
                #include "pyconfig-32.h"
            #else
                #error "Unknown LONG size"
            #endif
        #endif
        #ifdef HAVE_CONFIG_H
            #include "../../../config.h"
        #endif
    #else
        #error "Unknown OS"
    #endif
#endif
