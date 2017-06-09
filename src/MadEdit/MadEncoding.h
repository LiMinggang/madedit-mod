///////////////////////////////////////////////////////////////////////////////
// Name:		MadEdit/MadEncoding.h
// Description:	define the Encodings which are supported by	MadEdit
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADENCODING_H_
#define	_MADENCODING_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma	hdrstop
#endif

#ifndef	WX_PRECOMP
// Include your	minimal	set	of headers here, or	wx.h
#include <wx/wx.h>
#endif
#if __cplusplus <= 199711L
#ifndef nullptr
#define nullptr (0)
#endif
#endif
#include <wx/fontmap.h>
#include <wx/string.h>
#include "ucs4_t.h"
#include <vector>
#include <map>

class MadCSConv;

enum MadEncodingType
{ etSingleByte,	etDoubleByte, etUTF8, etUTF16LE, etUTF16BE,	etUTF32LE, etUTF32BE, etGB18030 };

enum MadEncodingGrp
{
	ENCG_WESTERNEUROPE = 0,
	ENCG_CENTRALEUROPE,
	ENCG_SOUTHEUROPE,
	ENCG_NORTHEUROPE,
	ENCG_CYRILLIC,
	ENCG_ARABIC,
	ENCG_GREEK,
	ENCG_HEBREW,
	ENCG_TURKISH,
	ENCG_BALTIC,
	ENCG_EASTASIA,
	ENCG_SOUTHEASTASIA,
	ENCG_UNICODE,
	ENCG_ISO8859,
	ENCG_WINDOWS,
	ENCG_OEM,
	ENCG_MACINTOSH,
	ENCG_DEFAULT,
	ENCG_MAX
};

struct MadEncodingInfo
{
	int         	m_Encoding;
	wxString		m_Name;
	wxString		m_Description;
	wxString		m_FontName;
	MadEncodingType	m_Type;
	MadCSConv	   *m_CSConv;
	ucs2_t		   *m_MBtoWC_Table;		 //	MultiByte To WideChar table
	wxWord		   *m_WCtoMB_Table;		 //	WideChar To	MultiByte table
	wxByte		   *m_LeadByte_Table;	 //	DBCS Lead-Byte table
	std::vector<int>  m_GrpIdVec; //Filter Ids

	MadEncodingInfo()
		:m_Encoding(-1), m_Type(etUTF8),
		m_CSConv(nullptr), m_MBtoWC_Table(nullptr), m_WCtoMB_Table(nullptr), m_LeadByte_Table(nullptr)
	{
	}

	MadEncodingInfo(int e, const	wxString &n, const wxString	&de, MadEncodingType t,	const wxString &fn,	std::vector<int>& grp)
		:m_Encoding(e),	m_Name(n), m_Description(de), m_FontName(fn), m_Type(t),
		m_CSConv(nullptr),	m_MBtoWC_Table(nullptr), m_WCtoMB_Table(nullptr),	m_LeadByte_Table(nullptr),	m_GrpIdVec(grp)
	{
	}
};


class MadEncoding
{
private:
	static size_t ms_SystemEncodingIndex;
	static MadEncoding *ms_SystemEncoding;
	static std::map<int, wxString>MadEncodingGrpName;

public:
	static void		InitEncodings(); //	must call this before use MadEncoding
	static void		FreeEncodings(); //

	static size_t	GetEncodingsCount();
	static wxString	GetEncodingName(size_t idx);
	static wxString	GetEncodingDescription(size_t idx);
	static wxString	GetEncodingFontName(size_t idx);
	static const std::vector<int>& GetEncodingGrps(size_t idx);
	static wxString	EncodingToName(int enc);
	static int NameToEncoding(const wxString &name);
	static MadEncoding *GetSystemEncoding();
	static wxString	GetGroupNameById(int id)
	{
		if(id<ENCG_WESTERNEUROPE ||	id>=ENCG_MAX) return wxEmptyString;
		return MadEncodingGrpName[id];
	}

private:
	MadEncodingInfo	*m_Info;
	MadCSConv		*m_CSConv;
	ucs2_t			*m_MBtoWC_Table;	// MultiByte To	WideChar table
	wxWord			*m_WCtoMB_Table;	// WideChar	To MultiByte table
	wxByte			*m_LeadByte_Table;	// DBCS	Lead-Byte table, 0:unset, 1:IsLeadByte,	0xFF:NotLeadByte

	void Create(size_t idx);

public:
	explicit MadEncoding(size_t idx);
	explicit MadEncoding(int enc);
	explicit MadEncoding(const wxString &name);
	~MadEncoding();

	// return the converted	length of buf
	size_t UCS4toMB		(ucs4_t	ucs4, wxByte *buf);
	size_t UCS4toUTF8	(ucs4_t	ucs4, wxByte *buf);
	static size_t UCS4toUTF16LE_U10000(ucs4_t ucs4,	wxByte *buf); // special case
	size_t UCS4toUTF16LE(ucs4_t	ucs4, wxByte *buf);
	size_t UCS4toUTF16BE(ucs4_t	ucs4, wxByte *buf);
	size_t UCS4toUTF32LE(ucs4_t	ucs4, wxByte *buf);
	size_t UCS4toUTF32BE(ucs4_t	ucs4, wxByte *buf);
	size_t UCS4toGB18030(ucs4_t	ucs4, wxByte *buf);

	ucs4_t SBtoUCS4(wxByte b1);    // Single-Byte to UCS4
	ucs4_t DBtoUCS4(wxByte *buf);	// Double-Byte to UCS4
    ucs4_t GB18030toUCS4(wxByte *buf, size_t &len);

	bool IsLeadByte(wxByte byte);	// for DBCS	only

	typedef	size_t (MadEncoding::*UCS4toMultiByteFuncPtr)(ucs4_t ucs4, wxByte *buf);
	UCS4toMultiByteFuncPtr UCS4toMultiByte;

	wxString GetName() { return	m_Info->m_Name;	}
	wxString GetDescription() {	return m_Info->m_Description; }
	MadEncodingType	GetType() {	return m_Info->m_Type; }
	wxString GetFontName() { return	m_Info->m_FontName;	}
	int GetEncoding() { return m_Info->m_Encoding; }
};


bool IsTextUTF32LE(wxByte *text, int size);
bool IsTextUTF32BE(wxByte *text, int size);
bool IsTextUTF16LE(wxByte *text, int size);
bool IsTextUTF16BE(wxByte *text, int size);
bool IsTextUTF8(wxByte *text, int size);
bool IsBinaryData(wxByte *data,	int	size);
void DetectChineseEncoding(const wxByte	*text, int count, int &enc);
void DetectJapaneseEncoding(const wxByte *text,	int	count, int &enc);
void DetectEncoding(const wxByte *text,	int	count, int &enc);

#endif
