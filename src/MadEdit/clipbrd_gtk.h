#ifdef __WXGTK__
#include <wx/version.h>
#if __cplusplus <= 199711L
#ifndef nullptr
	#define nullptr (0)
#endif
#endif

#if wxMAJOR_VERSION >= 3
#include "clipbrd_gtk_wx3.h"
#else
#include "clipbrd_gtk_wx2.h"
#endif
#endif // __WXGTK__
