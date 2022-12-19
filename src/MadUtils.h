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
#include <wx/event.h>
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

extern void	SetHtmlColors(void);
extern wxString	GetExecutablePath(void);
extern wxString	g_MadEditRegkeyPath;

class MadUniqueIDReserver : private boost::noncopyable
{
public:
	long RecentFindTextID1(void) { return fid1; }
	long RecentFindTextID20(void) { return fid20; }
	long RecentReplaceTextID1(void) { return rid1; }
	long RecentReplaceTextID20(void){ return rid20; }

	static MadUniqueIDReserver& Instance(void)
	{
		static MadUniqueIDReserver inst;
		return inst;
	}
private:
	MadUniqueIDReserver(void);
	long fid1;
	long fid20;
	long rid1;
	long rid20;
};

void EditUndo(void);
void EditRedo(void);
void EditCut(void);
void EditCopy(void);
void EditPaste(void);
void EditDelete(void);
void EditCutLine(void);
void EditDeleteLine(void);
void EditSelectAll(void);
void EditStartEndSelction(void);
void EditInsertTabChar(void);
void EditInsertDateTime(void);
void EditSortAscending(void);
void EditSortDescending(void);
void EditSortAscendingCase(void);
void EditSortDescendingCase(void);
void EditSortByOptions(void);
void EditSortOptions(wxWindow* parent);
void EditCopyAsHexString(void);
void EditCopyAsHexStringWithSpace(void);
void EditCopyRevertHex(void);
void EditIncIndent(void);
void EditDecIndent(void);
void EditComment(void);
void EditUncomment(void);
void EditWordWrapToNewLine(void);
void EditNewLineToWordWrap(void);
void EditToUpperCase(void);
void EditToLowerCase(void);
void EditInvertCase(void);
void EditCapitalize(void);
void EditToHalfWidth(void);
void EditToHalfWidthByOptions(wxWindow* parent);
void EditToFullWidth(void);
void EditToFullWidthByOptions(wxWindow* parent);
void EditTabToSpace(void);
void EditSpaceToTab(void);
void EditTrimTrailingSpaces(void);
void EditTrimLeadingSpaces(void);
void EditDeleteEmptyLines(void);
void EditDeleteEmptyLinesWithSpaces(void);
void EditJoinLines(void);
void EditInsertNumbers(wxWindow* parent);
void EditColumnAlignLeft(void);
void EditColumnAlignRight(void);
void EditSpellCheck(wxCommandEvent& event);
void ToolsMadScriptList(wxCommandEvent& event);

#endif
