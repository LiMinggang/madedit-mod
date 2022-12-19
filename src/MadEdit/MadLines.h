///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadLines.h
// Description: the Text/Data Buffer of MadEdit
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADLINES_H_
#define _MADLINES_H_
#include "../MadUtils.h"

#if CPLUSEPLUSE98
	#include <boost/shared_ptr.hpp>
#else
	#include <memory>
#endif

#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
	#include <wx/wx.h>
	#include <wx/log.h>
#endif

#include <wx/file.h>

#include <vector>
#include <list>
#include <deque>
#include <utility>

using std::vector;
using std::list;
using std::deque;
using std::pair;

#include "ucs4_t.h"
#include "MadDeque.hpp"

//===========================================================================
// MadFileNameIsUTF8, MadDirExists, MadConvFileName_WC2MB_UseLibc
// for Testing/Converting of FileName Encoding under Linux
//===========================================================================

// return 0: filename is not found, 1:yes, -1:no
int MadFileNameIsUTF8(const wxString& filename);

// return 0: name is not a dir or not found, 1:yes(utf8), -1:yes(libc)
int MadDirExists(const wxString& name);

extern bool g_WC2MB_2_utf8;
extern bool g_MB2WC_is_utf8;
extern bool g_MB2WC_check_dir_filename;//check dir, filename separately

class MadConvFileName_WC2MB_UseLibc
{
public:
	explicit MadConvFileName_WC2MB_UseLibc(bool uselibc) {
		g_WC2MB_2_utf8 = !uselibc;
	}
	~MadConvFileName_WC2MB_UseLibc() {
		g_WC2MB_2_utf8 = true;
	}
};

#ifndef __WXMSW__
class MadConvFileName : public wxMBConv
{
public:
	virtual size_t MB2WC(wchar_t *outputBuf, const char *psz, size_t outputSize) const;
	virtual size_t WC2MB(char *outputBuf, const wchar_t *psz, size_t outputSize) const;
	virtual wxMBConv *Clone() const { return new MadConvFileName; }
};
#endif

//==================================================
class MadInData
{
protected:
	wxFileOffset m_Size;
public:
	virtual ~MadInData() {}
	virtual wxByte Get(const wxFileOffset &pos) = 0;
	virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size) = 0;
	wxFileOffset GetSize() { return m_Size; }
};

class MadOutData
{
public:
	virtual ~MadOutData() {}
	virtual wxFileOffset Put(wxByte *buffer, size_t size) = 0;
};


class MadMemData : public MadInData, public MadOutData
{
private:
	friend class MadLines;
	friend class MadEdit;

	vector <wxByte*> m_Buffers;
	void Reset() { m_Size = 0; }

public:
	MadMemData();
	virtual ~MadMemData();

	virtual wxByte Get(const wxFileOffset &pos);
	virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size);
	virtual wxFileOffset Put(wxByte *buffer, size_t size);
};

class MadFileData : public MadInData, public MadOutData
{
private:
	friend class MadLines;

	wxFile m_File;
	bool m_OpenSuccess;
	wxString m_Name;
	bool m_ReadOnly;

	wxByte *m_Buffer1, *m_Buffer2;
	wxFileOffset m_Buf1Pos, m_Buf2Pos;

	wxFileOffset m_SavePos; // for Put()

	bool OpenFile();
	bool Rename(const wxString &name);

public:
	explicit MadFileData(const wxString &name);
	virtual ~MadFileData();
	virtual wxByte Get(const wxFileOffset &pos);
	virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size);

	virtual wxFileOffset Put(wxByte *buffer, size_t size);
	void Read(const wxFileOffset &pos, wxByte *buffer, size_t size) {
		m_File.Seek(pos);
		m_File.Read(buffer, size);
	}

	bool OpenSuccess() { return m_OpenSuccess; }
	bool IsReadOnly() { return m_ReadOnly; }
};

//==================================================

struct MadBlock//:public Loki::SmallObject <>
{
	MadInData   *m_Data;
	wxFileOffset m_Pos;
	wxFileOffset m_Size;

	MadBlock(): m_Data(nullptr), m_Pos(0), m_Size(0) {
	}
	MadBlock(MadInData *data, const wxFileOffset &pos, const wxFileOffset &size)
		: m_Data(data), m_Pos(pos), m_Size(size) {
	}
	void Reset() {
		m_Data = nullptr;
		m_Pos = 0;
		m_Size = 0;
	}
	wxByte Get(const wxFileOffset &pos) {
		wxASSERT(pos >= 0 && pos < m_Size);
		return m_Data->Get(m_Pos + pos);
	}
	void Get(const wxFileOffset &pos, wxByte *buffer, size_t size) {
		wxASSERT(pos >= 0 && pos < m_Size);
		m_Data->Get(m_Pos + pos, buffer, size);
	}
};

struct MadRowIndex    // wrapped-line index
{
	wxFileOffset m_Start;                   // start position
	int m_Width;                            // pixel-width of this row

	MadRowIndex(): m_Start(0), m_Width(0) {
	}
	MadRowIndex(const wxFileOffset &start, int width): m_Start(start), m_Width(width) {
	}
	void Reset() {
		m_Start = 0;
		m_Width = 0;
	}
};

struct MadLineState
{
	wxByte RangeId;     // 0: Global Range, 1..255: User Defined Range
	wxByte CommentId;   // 0: not in blockcomment, 1..255: User Defined BlockComment
	wxByte CommentOff;  // contains a BlockCommentOff string in this line, for speed-up of parsing
	wxByte StringId;    // 0: not in string, 1..255: in string
	wxByte LineComment; // 0: not in linecomment, 1..255: in linecomment
	wxByte Directive;   // 0: not in directive, 1..255: in directive

	MadLineState(): RangeId(0), CommentId(0), CommentOff(0), StringId(0), LineComment(0), Directive(0) {
	}
	bool operator==(const MadLineState &ls) const {
		return RangeId == ls.RangeId && CommentId == ls.CommentId &&
			   CommentOff == ls.CommentOff && StringId == ls.StringId &&
			   LineComment == ls.LineComment && Directive == ls.Directive;
	}
};

struct BracePairIndex
{
	wxFileOffset LinePos;    // linepos
	int          XPos;       // pixel-width pos
	wxUint16     Width;
	wxUint16     Length;     // byte-length
	wxByte       LeftPair;   // non-zero: leftpair, zero: rightpair
	char         BraceIndex; //

	BracePairIndex(): LinePos(0), XPos(0), Width(0), Length(0), LeftPair(0), BraceIndex(0)
	{}

	BracePairIndex(int xpos, wxUint16 wid, wxFileOffset lpos, wxUint16 len, wxByte lp, char bi)
		: LinePos(lpos), XPos(xpos), Width(wid), Length(len), LeftPair(lp), BraceIndex(bi)
	{}
};

typedef vector < MadBlock > MadBlockVector;
typedef MadBlockVector::iterator MadBlockIterator;
typedef vector < MadRowIndex >::iterator MadRowIndexIterator;

class MadEncoding;
#ifndef __WXDEBUG__
	#define  __WXDEBUG__
#endif
struct MadLine
{
	wxFileOffset            m_Size;         // data size, include m_NewLineSize
	MadLineState            m_State;
	wxByte                  m_NewLineSize;  // ANSI: "0D,0A" , UNICODE: "0D,00,0A,00"
	MadBlockVector          m_Blocks;
	vector <MadRowIndex>    m_RowIndices;
	vector <BracePairIndex> m_BracePairIndices;

	MadLine(): m_Size(0), m_NewLineSize(0) {
	}
	void Reset();
	void Empty();
	bool IsEmpty() { return m_Size == 0; }

	// if false return 0 ; else return 0x0D or 0x0A
	ucs4_t LastUCharIsNewLine(MadEncoding *encoding);
	bool FirstUCharIs0x0A(MadEncoding *encoding);

	wxByte Get(wxFileOffset pos) {                        // get 1 byte
		wxASSERT(pos >= 0 && pos < m_Size);
		MadBlockIterator biter = m_Blocks.begin();

		if (pos >= biter->m_Size)
			do {
				pos -= biter->m_Size;
				++biter;
			}
			while (pos >= biter->m_Size);

		return biter->Get(pos);
	}

	void Get(wxFileOffset pos, wxByte *buf, size_t size) { // get n bytes
		wxASSERT(pos >= 0 && ((wxFileOffset)(pos + size) <= m_Size));

		if (size == 0) { return; }

		MadBlockIterator biter = m_Blocks.begin();

		if (pos >= biter->m_Size)
			do {
				pos -= biter->m_Size;
			}
			while (pos >= (++biter)->m_Size);

		size_t cnt;

		while (true) {
			cnt = size;

			//wxLogDebug(wxT("Before:m_size[%d], m_data[%p]"), biter->m_Size, biter->m_Data);
			if (((wxFileOffset)(pos + cnt)) > biter->m_Size) { cnt = (size_t)(biter->m_Size - pos); }

			biter->Get(pos, buf, cnt);

			//wxLogDebug(wxT("After:m_size[%d], m_data[%p]"), biter->m_Size, biter->m_Data);
			if ((size -= cnt) == 0) { break; }

			pos = 0;
			buf += cnt;
			++biter;
		}
	}

	size_t RowCount() {
		return m_RowIndices.size() - 1;
	}
};

//==================================================

typedef list<MadLine>::iterator         MadLineIterator;
typedef pair<ucs4_t, int>               MadUCPair;  // ucs4char, uc_len
//typedef deque<MadUCPair>             MadUCQueue;
//typedef deque<MadUCPair>::iterator   MadUCQueueIterator;
typedef MadDeque<MadUCPair>             MadUCQueue;
typedef MadDeque<MadUCPair>::iterator   MadUCQueueIterator;
typedef vector<wxString>::iterator      MadStringIterator;


// add: gogo, 21.09.2009 (bookmarks support)
//
class MadLineList : public list <MadLine>
{
	list<MadLineIterator> m_BookmarkList;

public:
	MadLineList();

	void SetBookmark(MadLineIterator position, bool toggle = true);     // toggle or remove bookmark from given position
	void RemoveBookmark(MadLineIterator position);     // remove bookmark from given position
	int  GetNextBookmark(MadLineIterator position); // return line number, or -1 if no bookmars
	int  GetPreviousBookmark(MadLineIterator position); // return line number from the end to the beginning, or -1
	bool IsBookmarked(MadLineIterator position);
	void ClearAllBookmarks();
	bool HasBookMark() {return !(m_BookmarkList.empty());}
	list<MadLineIterator> & GetBookmarkedLines() {return m_BookmarkList;}

	MadLineIterator erase(MadLineIterator position);

private:
	// it is private because bookmarks removal is not implemented yet for it
	// in this way if someone try to use it, he will get compiler error
	MadLineIterator erase(MadLineIterator first, MadLineIterator last)
	{ return list<MadLine>::erase(first, last); }
};
typedef list<MadLineIterator>::iterator  MadBookmarkIterator;


class MadEdit;
class MadSyntax;

class MadLines
{
private:
	typedef int (MadLines::*FindStringPtr)(MadUCQueue &ucqueue,
			MadStringIterator begin, const MadStringIterator &end, size_t &len);
	friend class MadEdit;
	friend class MadSyntax;

	MadEdit       *m_MadEdit;
	MadSyntax     *m_Syntax;
	MadEncoding   *m_Encoding;

	// changed: gogo, 21.09.2009 (bookmarks support)
	//list <MadLine> m_LineList;
	MadLineList m_LineList;

	size_t m_LineCount, m_RowCount;         // line counts
	wxFileOffset m_Size;                    // total size

	MadLineState m_EndState;

	MadFileData *m_FileData;
	MadFileData *m_TmpFileData;

	MadMemData *m_MemData;
	FindStringPtr FindString;

	wxByte          *m_WriteBuffer;
	wxByte          *m_NextUChar_Buffer;
	size_t          m_NextUChar_BufferStart;
	size_t          m_NextUChar_BufferSize;
	wxFileOffset    m_NextUChar_BufferNextPos;
	wxFileOffset    m_NextUChar_LineSize;
	wxFileOffset    m_NextUChar_Pos;
	vector<wxByte>  m_WriteBufferVector;
	MadLineIterator m_NextUChar_LineIter;
	wxString    m_Name;
	bool        m_ReadOnly;
	bool        m_NextUChar_BufferLoadNew;
	int         m_MaxLineWidth;             //max pixel width of line/row


private:

	void Empty(bool freeAll);
	void Clear(bool freeAll);

	size_t ReformatCount;
	// reformat single line, return the state of line-end
	void Reformat(/*IN*/MadLineIterator &iter,/*IN*/int maxwrapwidth, /*IN*/long orgtabwidth, /*OUT*/MadLineState &state);
	// reformat lines in [first,last]
	size_t Reformat(MadLineIterator first, MadLineIterator last);
	// Recount all lines' width
	void RecountLineWidth(void);

	// append lit2 after lit1
	void Append(const MadLineIterator &lit1, const MadLineIterator &lit2);

	// write to fd or file if which one isn't nullptr
	void WriteBlockToData(MadOutData *fd, const MadBlockIterator &bit);
	void WriteToFile(wxFile &file, MadFileData *oldfd, MadFileData *newfd);

	wxFileOffset GetMaxTempSize(const wxString &filename);

public:
	explicit MadLines(MadEdit *madedit);
	~MadLines();

	bool LoadFromFile(const wxString &filename, const wxString &encoding = wxEmptyString);
	bool SaveToFile(const wxString &filename, const wxString &tempdir);
	wxFileOffset GetSize() { return m_Size; }

private:  // NextUChar()
	void LoadNewBuffer();

	bool NextUChar_SBCS(MadUCQueue &ucqueue);
	bool NextUChar_DBCS(MadUCQueue &ucqueue);
	bool NextUChar_UTF8(MadUCQueue &ucqueue);
	bool NextUChar_UTF16LE(MadUCQueue &ucqueue);
	bool NextUChar_UTF16BE(MadUCQueue &ucqueue);
	bool NextUChar_UTF32LE(MadUCQueue &ucqueue);
	bool NextUChar_UTF32BE(MadUCQueue &ucqueue);
    bool NextUChar_GB18030(MadUCQueue &ucqueue);

	bool NextUCharIs0x0A(void);

	int FindStringCase(MadUCQueue &ucqueue, MadStringIterator begin,
						const MadStringIterator &end, size_t &len);

	// the [begin,end) iter must be lower case
	int FindStringNoCase(MadUCQueue &ucqueue, MadStringIterator begin,
						  const MadStringIterator &end, size_t &len);


public:
	void SetEncoding(MadEncoding *encoding);

	// if no newline return 0 ; else return 0x0D or 0x0A or 0x0D+0x0A (=0x17)
	ucs4_t GetNewLine(const MadLineIterator &iter);

	void InitNextUChar(const MadLineIterator &iter, const wxFileOffset pos);

	typedef bool (MadLines::*NextUCharFuncPtr)(MadUCQueue & ucqueue);
	NextUCharFuncPtr NextUChar;

	// should not frequently use this, it's slowly
	// if no, return MadUCPair(0, 0)
	MadUCPair PreviousUChar(/*IN_OUT*/MadLineIterator &lit, /*IN_OUT*/wxFileOffset &linepos);
	static const MadLine madNewEmptyLine;
	static const MadRowIndex madNewRowIdx;
};

#endif
