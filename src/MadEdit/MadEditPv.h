///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditPv.h
// Description: main Edit component of MadEdit
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
// PATCH_ used for bug fix
// EXTRA_ used for extra functinal
#ifndef _MADEDITPV_H_
#define _MADEDITPV_H_

#define USE_MADEDITPV 1

#if USE_MADEDITPV == 1
#define PATCH_RPLACEALL_HEX_MODE 1
#define PATCH_RPLACE_REGEXP 1
#define PATCH_MAXLINELENGTH 1

#else

#endif //USE_MADEDITPV
#endif //_MADEDITPV_H_
