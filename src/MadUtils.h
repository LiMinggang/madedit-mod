///////////////////////////////////////////////////////////////////////////////
// Name:		MadUtils.h
// Description:	utility	functions. collects	from internet
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADUTILS_H_
#define	_MADUTILS_H_
#ifndef _MSC_VER 
#if __cplusplus <= 199711L
#define CPLUSEPLUSE98 1
#ifndef emplace_back
#define emplace_back push_back
#endif
#ifndef nullptr
#define nullptr (0)
#endif
#endif
#if (__cplusplus - 0) >= 201703L
  #define __REGISTER
#else
  #define __REGISTER                              register
#endif
#else
#if _MSC_VER < 1600
#define CPLUSEPLUSE98 1
#ifndef emplace_back
#define emplace_back push_back
#endif
#ifndef nullptr
#define nullptr (0)
#endif
#endif
#if ((_MSC_VER >= 1910) && (_MSC_VER <= 2000))
  #define __REGISTER
#else
  #define __REGISTER                              register
#endif
#endif

#include <wx/defs.h>
#include <boost/noncopyable.hpp>

struct HtmlColor
{
	const wxChar *name;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
extern HtmlColor HtmlColorTable[];
extern const int HtmlColorTableCount;

extern void	SetHtmlColors();
extern wxString	GetExecutablePath();
extern wxString	g_MadEditRegkeyPath;

class MadUniqueIDReserver : private boost::noncopyable
{
public:
	long RecentFindTextID1() { return fid1; }
	long RecentFindTextID20() { return fid20; }
	long RecentReplaceTextID1() { return rid1; }
	long RecentReplaceTextID20(){ return rid20; }

	static MadUniqueIDReserver& Instance()
	{
		static MadUniqueIDReserver inst;
		return inst;
	}
private:
	MadUniqueIDReserver();
	long fid1;
	long fid20;
	long rid1;
	long rid20;
};

#endif
