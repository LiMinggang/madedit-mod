///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEdit.cpp
// Description: main Edit component of MadEdit
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEdit.h"
#include "MadEncoding.h"
#include "MadSyntax.h"
#include "MadUndo.h"

#include "MadEditPv.h"
#include <wx/buffer.h>
#include <wx/fileconf.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/dataobj.h>
#include <wx/tokenzr.h>
#include <wx/menu.h>

//#ifdef __WXGTK__
	//#   include <wx/gtk/clipbrd.h>

//#else
	#   include <wx/clipbrd.h>
//#endif

#include <wx/caret.h>
#if !defined(__WXMSW__)
	#   include "caret_new.h"
#endif
#ifdef __WXMSW__
	#include <io.h>
#endif
#ifdef __WXGTK__
	#include <stdio.h>
	#include <gdk/gdk.h>
	#include <gdk/gdkx.h>
	#include <gdk/gdkprivate.h>
	#include <gtk/gtk.h>
	#if (wxMAJOR_VERSION < 3)
		#include <wx/gtk/win_gtk.h>
		#include <wx/gtk/dcmemory.h>
	#else
		//#include <wx/gtk/private/win_gtk.h>
		#include <wx/dcmemory.h>
	#endif
#endif

#ifndef PYMADEDIT_DLL
	#include "SpellCheckerManager.h"
	#include "HunspellInterface.h"
#endif

#ifdef __WXMSW__
#include "../../images/zerolen.xpm"
#define zerolen_xpm_idx 0
#endif

using std::vector;
using std::list;

#ifndef FIXINVERT
	#ifdef __WXMSW__
		#       define FIXINVERT 0
	#   else
		#ifdef __WXGTK3__
			#           define FIXINVERT 1
		#       else
			#           define FIXINVERT 0
		#       endif
	#   endif
#endif

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

/*wxMAJOR_VERSION wxMINOR_VERSION wxRELEASE_NUMBER wxSUBRELEASE_NUMBER*/
#if wxMAJOR_VERSION < 3
	#define MADK_NONE 0
#else
	#define MADK_NONE WXK_NONE
#endif

static inline int wxChCmp( const wchar_t * wchStr, const wxString & wsStr );
extern bool IsMacroRecording();
extern void RecordAsMadMacro( MadEdit *, const wxString&, bool=false);
extern bool FromCmdToString( wxString &cmdStr, long madCmd );

MadKeyBindings MadEdit::ms_KeyBindings;

const int HexModeMaxColumns = 78;
extern const ucs4_t HexHeader[78] =
{
	' ', 'O', 'f', 'f', 's', 'e', 't', ' ', ' ', ' ', '0', '0', ' ', '0', '1', ' ',
	'0', '2', ' ', '0', '3', ' ', '0', '4', ' ', '0', '5', ' ', '0', '6', ' ', '0',
	'7', ' ', '0', '8', ' ', '0', '9', ' ', '0', 'A', ' ', '0', 'B', ' ', '0', 'C',
	' ', '0', 'D', ' ', '0', 'E', ' ', '0', 'F', ' ', '|', ' ', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '|', ':'
};

static wxCursor ArrowCursor, IBeamCursor, DragCopyCursor, DragMoveCursor, DragNoneCursor, RightArrowCursor;
//==================================================

#define BUF_LEN 512
wxString MadStrLower( const wxString& ws )
{
#ifdef __WXMSW__
	wchar_t buf[BUF_LEN + 1];
	wxString result;
	size_t len = ws.Len(), beg = 0;

	do
	{
		wcscpy( buf, ws.SubString( beg, beg + BUF_LEN - 1 ).wc_str() );
		CharLowerW( buf );
		result << buf;
		beg += BUF_LEN;
	}
	while( len > beg );

	return result;
#else
	return ws.Lower();
#endif
}

class MadDataObject : public wxDataObjectSimple
{
public:
	explicit MadDataObject( const wxChar *fmt ) {
		SetFormat( fmt );
	}

	vector<char> data;

	// get the size of our data
	virtual size_t GetDataSize() const
	{ return data.size() + 4 + sizeof( size_t ); }

	// copy our data to the buffer
	virtual bool GetDataHere( void *buf ) const {
		// prepend 0xFFFFFFFF and the length (fixed crash under win98)
		wxByte *b = ( wxByte * )buf;
		b[0] = b[1] = b[2] = b[3] = 0xFF;
		b += 4;
		*( ( size_t* )b ) = data.size();
		b += sizeof( size_t );
		memcpy( b, &data[0], data.size() );
		return true;
	}

	// copy data from buffer to our data
	virtual bool SetData( size_t len, const void *buf ) {
		wxByte *b = ( wxByte * )buf;
		b += 4;
		len = *( ( size_t* )b );
		b += sizeof( size_t );
		data.resize( len );
		memcpy( &data[0], b, len );
		return true;
	}
};

class MadColumnDataObject : public MadDataObject
{
public:
	MadColumnDataObject() : MadDataObject( wxT( "application/madedit-columndata" ) )
	{}

	bool GetColumnData( wxString &str, int &linecount ) {
		if( data.empty() ) return false;

		char *buf = &data[0];
		linecount = *( ( int* )buf );
		buf += sizeof( int );
		str = ( wxChar* )buf;
		return true;
	}

	bool SetColumnData( const wxString &ws, int linecount ) {
		size_t size = sizeof( int ) + ( ws.Len() + 1 ) * sizeof( wxChar );
		data.resize( size );
		char *buf = &data[0];
		*( ( int* )buf ) = linecount;
		buf += sizeof( int );
		memcpy( buf, ws.wc_str(), size - sizeof( int ) );
		return true;
	}
};

class MadHexDataObject : public MadDataObject
{
public:
	MadHexDataObject() : MadDataObject( wxT( "application/madedit-hexdata" ) )
	{}

	size_t GetHexDataSize() {
		return data.size();
	}

	bool GetHexData( void *buf ) {
		if( data.empty() ) return false;

		memcpy( buf, &data[0], data.size() );
		return true;
	}

	bool SetHexData( size_t len, const void *buf ) {
		data.resize( len );
		memcpy( &data[0], buf, len );
		return true;
	}
};

//==================================================
class MadMouseMotionTimer : public wxTimer
{
private:
	MadEdit *m_madedit;
public:
	explicit MadMouseMotionTimer( MadEdit *madedit ): m_madedit( madedit )
	{}

	virtual void Notify() {
		wxMouseEvent evt;
		wxPoint pt = wxGetMousePosition();
		pt = m_madedit->ScreenToClient( pt );
		evt.m_x = pt.x;
		evt.m_y = pt.y;
		m_madedit->OnMouseMotion( evt );
	}
};

//==================================================
wxDEFINE_EVENT( CHECK_MODIFICATION_TIME, wxCommandEvent );
wxDEFINE_EVENT( UPDATE_HSCROLL_POS, wxCommandEvent );

#if defined(__WXGTK20__) && (wxMAJOR_VERSION < 3)
void GTK2_DrawText( wxMemoryDC *dc, MadEncoding *encoding, const int *widths,
					const wxString &text, wxCoord x, wxCoord y )
{
	wxCHECK_RET( dc->IsOk(), wxT( "invalid window dc" ) );

	if( !dc->m_window ) return;

	if( text.empty() ) return;

	x = dc->LogicalToDeviceX( x ); //XLOG2DEV(x);
	y = dc->LogicalToDeviceY( y ); //YLOG2DEV(y);
	wxCHECK_RET( dc->m_context, wxT( "no Pango context" ) );
	wxCHECK_RET( dc->m_layout, wxT( "no Pango layout" ) );
	wxCHECK_RET( dc->m_fontdesc, wxT( "no Pango font description" ) );
	wxFont font = dc->GetFont();
	bool underlined = font.Ok() && font.GetUnderlined();
	const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( text );
	size_t datalen_utf8 = strlen( data_utf8 );
	pango_layout_set_text( dc->m_layout, ( const char* ) data_utf8, datalen_utf8 );

	if( underlined )
	{
		PangoAttrList *attrs = pango_attr_list_new();
		PangoAttribute *a = pango_attr_underline_new( PANGO_UNDERLINE_SINGLE );
		a->start_index = 0;
		a->end_index = datalen_utf8;
		pango_attr_list_insert( attrs, a );
		pango_layout_set_attributes( dc->m_layout, attrs );
		pango_attr_list_unref( attrs );
	}

	GSList *run;
	PangoGlyphItem *gi;
	PangoGlyphString *gs;
	PangoLayoutLine* ll = pango_layout_get_line( dc->m_layout, 0 );
	run = ll->runs;

	for( ; run != nullptr; run = g_slist_next( run ) )
	{
		gi = ( PangoGlyphItem * )run->data;
		gs = gi->glyphs;
		gint count = gs->num_glyphs;

		for( gint i = 0; i < count; ++i )
		{
			gs->glyphs[i].geometry.width = ( *widths ) * PANGO_SCALE; // set char width
			++widths;
		}
	}

	//gdk_draw_layout( dc->m_window, dc->m_textGC, x, y, dc->m_layout );
	double sx, sy;
	dc->GetUserScale( &sx, &sy );

	if( fabs( sy - 1.0 ) > 0.00001 )
	{
		// If there is a user or actually any scale applied to
		// the device context, scale the font.
		// scale font description
		gint oldSize = pango_font_description_get_size( dc->m_fontdesc );
		double size = oldSize;
		size = size * sy;
		pango_font_description_set_size( dc->m_fontdesc, ( gint )size );
		// actually apply scaled font
		pango_layout_set_font_description( dc->m_layout, dc->m_fontdesc );
		//pango_layout_get_pixel_size( dc->m_layout, &w, &h );
		//if ( m_backgroundMode == wxSOLID )
		//{
		//gdk_gc_set_foreground(m_textGC, m_textBackgroundColour.GetColor());
		//gdk_draw_rectangle(m_window, m_textGC, TRUE, x, y, w, h);
		//gdk_gc_set_foreground(m_textGC, m_textForegroundColour.GetColor());
		//}
		// Draw layout.
		//if (m_owner && m_owner->GetLayoutDirection() == wxLayout_RightToLeft)
		//gdk_draw_layout( m_window, m_textGC, x-w, y, m_layout );
		//else
		gdk_draw_layout( dc->m_window, dc->m_textGC, x, y, dc->m_layout );
		// reset unscaled size
		pango_font_description_set_size( dc->m_fontdesc, oldSize );
		// actually apply unscaled font
		pango_layout_set_font_description( dc->m_layout, dc->m_fontdesc );
	}
	else
	{
		//pango_layout_get_pixel_size( dc->m_layout, &w, &h );
		//if ( m_backgroundMode == wxSOLID )
		//{
		//gdk_gc_set_foreground(m_textGC, m_textBackgroundColour.GetColor());
		//gdk_draw_rectangle(m_window, m_textGC, TRUE, x, y, w, h);
		//gdk_gc_set_foreground(m_textGC, m_textForegroundColour.GetColor());
		//}
		// Draw layout.
		//if (m_owner && m_owner->GetLayoutDirection() == wxLayout_RightToLeft)
		//gdk_draw_layout( m_window, m_textGC, x-w, y, m_layout );
		//else
		gdk_draw_layout( dc->m_window, dc->m_textGC, x, y, dc->m_layout );
	}

	/*
	int w,h;
	w=x1-x;
	int tmpw;
	pango_layout_get_pixel_size( dc->m_layout, &tmpw, &h );
	*/

	if( underlined )
	{
		// undo underline attributes setting:
		pango_layout_set_attributes( dc->m_layout, nullptr );
	}

	/*
	double sx, sy;
	dc->GetUserScale(&sx, &sy);
	w = wxCoord(w / sx);
	h = wxCoord(h / sy);
	dc->CalcBoundingBox (x + w, y + h);
	dc->CalcBoundingBox (x, y);
	*/
}
#endif


wxString FixUTF8ToWCS( const wxString &str )
{
	size_t len = str.Len();
	wxByte *bbuf = new wxByte[len + 1];
	wxByte *ps = bbuf;
	const wxChar *pwcs = str.c_str();

	for( size_t l = len; l > 0; --l, ++ps, ++pwcs )
	{
		if( *pwcs >= 0x100 )
		{
			delete []bbuf;
			return str;
		}

		*ps = *pwcs;
	}

	*ps = 0;
	wxString wcs;

	if( IsTextUTF8( bbuf, ( int )len ) )
	{
		wxChar *wbuf = new wxChar[len + 1];
		wxConvUTF8.MB2WC( wbuf, ( const char* )bbuf, len + 1 );
		wcs = wbuf;
		delete []wbuf;
	}
	else
	{
		wcs = str;
	}

	delete []bbuf;
	return wcs;
}

//==============================================================================
int FontWidthManager::MaxCount = 10;
wxString FontWidthManager::DataDir;
wxString FontWidthManager::FontDataFile[17];
vector<FontWidthManager::FontWidthBuffers> FontWidthManager::FontWidthBuffersVector;
list<FontWidthManager::VerifiedFlag> FontWidthManager::VerifiedFlagList;

template <class T>
void memset_t( void * buf, int value, size_t buf_len )
{
	size_t ord_len = buf_len % sizeof( T );
	T * p1 = ( T * )buf;
	char * p2 = ( char * )buf + buf_len - ord_len;

	while( ( ( char * )p1 ) != p2 )
	{
		*p1++ = value;
	}

	while( p2 != ( ( char * )buf ) + buf_len )
	{
		*p2++ = value;
	}
}

bool FontWidthManager::VerifyFontWidths( wxUint16 *widths, const wxString &fontname, int fontsize, wxWindow *win )
{
	// check the VerifiedFlag
	list<VerifiedFlag>::iterator vfit = VerifiedFlagList.begin();
	list<VerifiedFlag>::iterator vfend = VerifiedFlagList.end();

	while( vfit != vfend )
	{
		if( vfit->first == widths && vfit->second == false )
		{
			vfit->second = true;
			// test the font width is the same or not
			wxFont *pf = wxTheFontList->FindOrCreateFont( //fwbuf.fontsize, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined(), fwbuf.fontname);
							 fontsize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, fontname );
			int w, h;
			wxString text( wxT( 'A' ) );
			wxChar wc;

			for( wc = 0x20; wc <= 0x7E; ++wc )
			{
				text.SetChar( 0, wc );
				win->GetTextExtent( text, &w, &h, nullptr, nullptr, pf );

				if( widths[wc] != 0 )
				{
					if( widths[wc] != w ) // not match
					{
						return false;
					}
				}
				else
				{
					widths[wc] = w;
				}
			}

			wxChar wcs[] = {0x0400, 0x0600, 0x3040, 0x3400, 0x4E00, 0x9F8D, 0xAC00};
			const int count = sizeof( wcs ) / sizeof( wxChar );

			for( int idx = 0; idx < count; ++idx )
			{
				wc = wcs[idx];
				text.SetChar( 0, wc );
				win->GetTextExtent( text, &w, &h, nullptr, nullptr, pf );

				if( widths[wc] != 0 )
				{
					if( widths[wc] != w ) // not match
					{
						return false;
					}
				}
				else
				{
					widths[wc] = w;
				}
			}

			break;
		}

		++vfit;
	}

	return true;
}

void FontWidthManager::ClearBuffer_1_16( const wxString &fontname, int fontsize )
{
	vector<FontWidthBuffers>::iterator vecit = FontWidthBuffersVector.begin();
	vector<FontWidthBuffers>::iterator vecitend = FontWidthBuffersVector.end();

	while( vecit != vecitend )
	{
		list<FontWidthBuffer>::iterator it = vecit->begin();
		list<FontWidthBuffer>::iterator itend = vecit->end();

		while( it != itend )
		{
			if( it->fontsize == fontsize && !fontname.compare( it->fontname )/*(!wxChCmp(it->fontname, fontname))*/ )
			{
				memset_t<long long>( it->widths, 0, sizeof( wxUint16 ) * 65536 );
				break;
			}

			++it;
		}

		++vecit;
	}
}

bool FontWidthManager::LoadFromFile( const wxString &filename, FontWidthBuffers &fwbuffers, bool verify )
{
	if( !wxFileExists( filename ) )
		return true;

	wxFile file( filename.c_str(), wxFile::read );

	if( file.IsOpened() )
	{
		wxFileOffset size = file.SeekEnd();

		if( size_t( size ) % ( sizeof( FontWidthBuffer ) + sizeof( wxUint16 ) * 65536 ) != 0 )
		{
			// fileformat error!!!
			// discard all data
			return false;
		}

		file.Seek( 0 );
		FontWidthBuffer fwbuf;
		int i = 0;

		while( !file.Eof() && i < MaxCount )
		{
			if( file.Read( &fwbuf, sizeof( FontWidthBuffer ) ) == sizeof( FontWidthBuffer ) )
			{
				fwbuf.widths = new wxUint16[65536];

				if( file.Read( fwbuf.widths, sizeof( wxUint16 ) * 65536 ) == sizeof( wxUint16 ) * 65536 )
				{
					fwbuffers.push_back( fwbuf );

					if( verify )
					{
						VerifiedFlagList.push_back( VerifiedFlag( fwbuf.widths, false ) );
					}
				}
				else
				{
					delete []fwbuf.widths;
					// fileformat error!!!
					return false;
				}
			}
			else
			{
				// fileformat error!!
				return false;
			}

			++i;
		}
	}

	return true;
}

void FontWidthManager::Init( const wxString &datadir )
{
	DataDir = datadir;
	FontWidthBuffersVector.resize( 17 ); // 0~16 : U+0000 ~ U+10FFFF
	FontDataFile[0] = DataDir + wxT( "FontWidth" ) + wxT( ".dat" );

	for( int i = 1; i < 17; ++i )
	{
		wxString filename( DataDir + wxT( "FontWidth" ) );
		FontDataFile[i] = filename << i << wxT( ".dat" );
	}
}

static inline int wxChCmp( const wchar_t * wchStr, const wxString & wsStr )
{
	size_t count = wsStr.Length();

	if( !count ) return 1;

	const wchar_t * wchTmpStr = wsStr.wc_str();

	while( * wchStr && * wchTmpStr )
	{
		if( ( * wchStr ) != ( * wchTmpStr ) )
			break;

		++wchStr;
		++wchTmpStr;
	}

	return ( ( int )( * wchStr ) - ( int )( * wchTmpStr ) );
}

wxUint16 *FontWidthManager::GetFontWidths( int index, const wxString &fontname, int fontsize, wxWindow *win )
{
	wxASSERT( index >= 0 && index <= 16 );
	if((size_t)index >= FontWidthBuffersVector.size()) index = 0;
	FontWidthBuffers &fwbuffers = FontWidthBuffersVector[index];

	if( fwbuffers.empty() )
	{
		//wxString fname(wxT("FontWidth"));
		LoadFromFile( FontDataFile[index], fwbuffers, index == 0 );
	}

	list<FontWidthBuffer>::iterator it = fwbuffers.begin();
	list<FontWidthBuffer>::iterator itend = fwbuffers.end();
	wxUint16 *wid;

	while( it != itend )
	{
		if( it->fontsize == fontsize && !wxChCmp( it->fontname, fontname.c_str() ) )
		{
			wid = it->widths;

			if( index == 0 && VerifyFontWidths( wid, fontname, fontsize, win ) == false )
			{
#ifdef __WXGTK__
				std::wcout << fontname.c_str() << wxT( ", " ) << fontsize << wxT( " : FontWidth does not match\n" );
#else
				//wxLogMessage(wxString()<<fontname<<wxT(", ")<<fontsize <<wxT(" : FontWidth does not match"));
#endif
				// reset font widths
				memset_t<long long>( wid, 0, sizeof( wxUint16 ) * 65536 );
				ClearBuffer_1_16( fontname, fontsize );

				// delete 1~16 datafile
				for( int i = 1; i <= 16; ++i )
				{
					if( wxFileExists( FontDataFile[i] ) )
					{
						wxRemoveFile( FontDataFile[i] );
					}
				}
			}

			if( it != fwbuffers.begin() ) // move it to front
			{
				fwbuffers.push_front( *it );
				fwbuffers.erase( it );
			}

			return wid;
		}

		++it;
	}

	int idx = int( fwbuffers.size() );

	if( idx >= MaxCount )
	{
		wid = fwbuffers.back().widths;
		fwbuffers.pop_back();
	}
	else
	{
		wid = new wxUint16[65536];
	}

	memset_t<long long>( wid, 0, sizeof( wxUint16 ) * 65536 );
	fwbuffers.push_front( FontWidthBuffer( fontname, fontsize, wid ) );
	return wid;
}

void FontWidthManager::Save()
{
	if( FontWidthBuffersVector.empty() )
		return;

	for( int idx = 0; idx <= 16; ++idx )
	{
		//wxString filename(DataDir+wxT("FontWidth"));
		//if(idx>0) filename<<idx;
		//filename<<wxT(".dat");
		if( FontWidthBuffersVector[idx].size() > 0 )
		{
			//if(wxFileExists(filename))
			//{
			//wxRemoveFile(filename);
			//}
			wxFile file( FontDataFile[idx].c_str(), wxFile::write );

			if( file.IsOpened() )
			{
				list<FontWidthBuffer>::iterator it = FontWidthBuffersVector[idx].begin();
				list<FontWidthBuffer>::iterator itend = FontWidthBuffersVector[idx].end();
				int i = 0;

				while( it != itend && i < MaxCount )
				{
					file.Write( &( *it ), sizeof( FontWidthBuffer ) );
					file.Write( it->widths, sizeof( wxUint16 ) * 65536 );
					++it;
					++i;
				}
			}
		}
	}
}

void FontWidthManager::FreeMem()
{
	if( FontWidthBuffersVector.empty() )
		return;

	for( int idx = 0; idx <= 16; ++idx )
	{
		list<FontWidthBuffer>::iterator it = FontWidthBuffersVector[idx].begin();
		list<FontWidthBuffer>::iterator itend = FontWidthBuffersVector[idx].end();

		while( it != itend )
		{
			delete []it->widths;
			++it;
		}

		FontWidthBuffersVector[idx].clear();
	}

	FontWidthBuffersVector.clear();
}

//==================================================

int MadEdit::ms_Count = 0;
wxMenu * MadEdit::m_ZeroLenSelIndicator = nullptr; // When to delete?

#include "../../images/dnd_copy.xpm"
#include "../../images/dnd_move.xpm"
//#include "../../images/dnd_none.xpm"

wxString MadEdit::m_FileFilter;

MadEdit::MadEdit( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
	: MadEditSuperClass( parent, id, pos, size, style ) //wxWANTS_CHARS|wxSIMPLE_BORDER)//|style|wxTAB_TRAVERSAL)
{
	++ms_Count;
	m_VScrollBar = new wxScrollBar( this, ID_VSCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	m_HScrollBar = new wxScrollBar( this, ID_HSCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL );
	m_VScrollBar->GetSize( &m_VSBWidth, &m_VSBHeight );
	m_HScrollBar->GetSize( &m_HSBWidth, &m_HSBHeight );
	m_VScrollBar->Bind(wxEVT_RIGHT_UP, &MadEdit::OnVSMouseRightUp, this);
	m_HScrollBar->Bind(wxEVT_RIGHT_UP, &MadEdit::OnHSMouseRightUp, this);

	if( !ArrowCursor.Ok() )
	{
		ArrowCursor = wxCursor( wxCURSOR_ARROW );
		IBeamCursor = wxCursor( wxCURSOR_IBEAM );
		RightArrowCursor = wxCursor( wxCURSOR_RIGHT_ARROW );
		DragCopyCursor = wxCursor( wxImage( dnd_copy_xpm ) );
		DragMoveCursor = wxCursor( wxImage( dnd_move_xpm ) );
		//DragNoneCursor = wxCursor(wxImage(dnd_none_xpm));
		DragNoneCursor = wxCursor( wxCURSOR_NO_ENTRY );
	}

	m_VScrollBar->SetCursor( ArrowCursor );
	m_HScrollBar->SetCursor( ArrowCursor );
	m_ClientWidth = m_ClientHeight = 0;
	m_Config = wxConfigBase::Get( false );
	m_Config->SetPath( wxT( "/MadEdit" ) );
	m_ActiveLMSize = m_Config->ReadLong( wxT( "ActiveLineMarkerSize" ), 1 );
	wxString defaultenc;
	m_Config->Read( wxT( "DefaultEncoding" ), &defaultenc );
	m_Syntax = MadSyntax::GetSyntaxByTitle( wxGetTranslation(MadPlainTextTitle) );
	wxASSERT( m_Syntax != 0 );
	if(m_SpellCheckerPtr)
		m_SpellCheckerPtr->SetSyntaxDictionary(m_Syntax->GetSyntaxDictionary());
	m_Encoding = new MadEncoding( defaultenc );
	m_Lines = new MadLines( this );
	// set default value
	m_Config->ReadLong( wxT( "MaxSizeToLoad" ), 20 * 1000 * 1000 );
	m_Config->ReadLong( wxT( "MaxTextFileSize" ), 10 * 1000 * 1000 );
	m_UndoBuffer = new MadUndoBuffer();
	m_CaretTracker = new MadUndoBuffer();
	m_SavePoint = nullptr;
	m_RecordCaretMovements = m_Config->ReadBool( wxT( "RecordCaretMovements" ), true );
	m_Modified = false;
	m_ModificationTime = 0;
	m_ReadOnly = false;
#ifdef __WXMSW__
	wxCaret *caret = new wxCaret( this, 2, 2 );
#else
	wxCaret *caret = new wxCaretNew( this, 2, 2 );
#endif
	SetCaret( caret );
	//caret->Show();    // first Show() in OnPaint()
	m_MaxLineLength = m_Config->ReadLong( wxT( "MaxLineLength" ), DEFAULT_MAX_LINELEN );
	if(m_MaxLineLength < 80 ) m_MaxLineLength = 80;
	if(m_MaxLineLength > DEFAULT_MAX_LINELEN) m_MaxLineLength = DEFAULT_MAX_LINELEN;
	m_Config->Write( wxT( "MaxLineLength" ), m_MaxLineLength ); // Make sure

	if( m_MaxLineLength < 80 ) m_MaxLineLength = 80;

#if PATCH_MAXLINELENGTH == 1

	if( m_MaxLineLength > DEFAULT_MAX_LINELEN ) m_MaxLineLength = DEFAULT_MAX_LINELEN;

#endif
	m_WordBuffer = new ucs4_t[m_MaxLineLength + 1];
#ifdef __WXMSW__
	m_WCWordBuffer = new wchar_t[( m_MaxLineLength + 1 ) * 2];
	m_WCWidthBuffer = new int[( m_MaxLineLength + 1 ) * 2];
#else
	m_WCWordBuffer = nullptr;
	m_WCWidthBuffer = nullptr;
#endif
	m_WidthBuffer = new int[m_MaxLineLength + 1];
	m_ClientBitmap = m_MarkBitmap = nullptr; // alloc at OnSize()
	m_SingleLineMode = false;
	m_StorePropertiesToGlobalConfig = true;
	m_CaretPos.Reset( m_Lines->m_LineList.begin() );
	m_ValidPos_iter = m_Lines->m_LineList.begin();
	m_ValidPos_lineid = 0;
	m_ValidPos_rowid = 0;
	m_ValidPos_pos = 0;
	m_UpdateValidPos = 0;
	m_Selection = false;
	m_ZeroSelection = false;
	m_SelectionStart = false;
	m_SelectionStartPos = -1;
	m_SelFirstRow = INT_MAX;
	m_SelLastRow = -1;
	//hacking for drag&drop
	m_DragDrop = false;
	m_DragCopyFlag = false;
	m_DndLines = 0;
	m_SelectionBegin = &m_SelectionPos1;
	m_SelectionEnd = &m_SelectionPos2;
	m_RepaintAll = true;
	m_RepaintSelection = false;
	m_FixedWidthMode = m_Config->ReadBool( wxT( "FixedWidthMode" ), true );
	m_EditMode = emTextMode;
	m_DoRecountLineWidth = false;
	m_OldWidth = m_OldHeight = 0;
	m_WordWrapMode = (MadWordWrapMode) m_Config->ReadLong( wxT( "WordWrapMode" ), ( long )wwmNoWrap );
	if(m_WordWrapMode < wwmNoWrap || m_WordWrapMode > wwmWrapByColumn) m_WordWrapMode = wwmNoWrap;
	m_Config->Write( wxT( "WordWrapMode" ), ( long )m_WordWrapMode ); // Make sure	
	m_TopRow = 0;
	m_DrawingXPos = 0;
	m_LineSpacing = m_Config->ReadLong( wxT( "LineSpacing" ), 100 );
	if(m_LineSpacing < 100) m_LineSpacing = 100;
	if(m_LineSpacing > 250) m_LineSpacing = 250;
	m_Config->Write( wxT( "LineSpacing" ), m_LineSpacing ); // Make sure
	m_MaxColumns = m_Config->ReadLong( wxT( "MaxColumns" ), 80 );
	if(m_MaxColumns < 0) m_MaxColumns = 80;
	if(m_MaxColumns > DEFAULT_MAX_LINELEN) m_MaxColumns = DEFAULT_MAX_LINELEN;
	m_Config->Write( wxT( "MaxColumns" ), m_MaxColumns ); // Make sure	
	m_MaxWidth = m_MaxHeight = 0;
	m_NewLineType = m_InsertNewLineType = nltDefault;
	m_HasTab = false;
	m_TabColumns = m_Config->ReadLong( wxT( "TabColumns" ), 4 );
	if(m_TabColumns < 0) m_TabColumns = 8;
	if(m_TabColumns > 1024) m_TabColumns = 1024;	
	m_Config->Write( wxT( "TabColumns" ), m_TabColumns ); // Make sure	
	m_IndentColumns = m_Config->ReadLong( wxT( "IndentColumns" ), 4 );
	if(m_IndentColumns < 0) m_IndentColumns = 8;
	if(m_IndentColumns > 1024) m_IndentColumns = 1024;
	m_Config->Write( wxT( "IndentColumns" ), m_IndentColumns ); // Make sure	
	m_InsertSpacesInsteadOfTab = m_Config->ReadBool( wxT( "InsertSpacesInsteadOfTab" ), false );
	m_WantTab = true;
	m_AutoIndent        = m_Config->ReadBool( wxT( "AutoIndent" ),         true );
	m_DisplayLineNumber = m_Config->ReadBool( wxT( "DisplayLineNumber" ),  true );
	m_DisplayBookmark   = m_Config->ReadBool( wxT( "DisplayBookmark" ),    true );
	m_Display80ColHint  = m_Config->ReadBool( wxT( "Display80ColHint" ),   true );
	m_ShowEndOfLine     = m_Config->ReadBool( wxT( "ShowEndOfLine" ),      true );
	m_ShowSpaceChar     = m_Config->ReadBool( wxT( "ShowSpaceChar" ),      true );
	m_ShowTabChar       = m_Config->ReadBool( wxT( "ShowTabChar" ),        true );
	m_MarkActiveLine    = m_Config->ReadBool( wxT( "MarkActiveLine" ),     true );
	m_MarkBracePair     = m_Config->ReadBool( wxT( "MarkBracePair" ),      true );
	m_AutoCompletePair  = m_Config->ReadBool( wxT( "AutoCompletePair" ),   false );
	m_InsertPairForSelection = (m_AutoCompletePair && m_Config->ReadBool(wxT("InsertPairForSelction"), false));
	m_AutoCompleteRightChar = 0;
	m_AutoCompletePos = 0;
	m_LeftBrace_rowid = m_RightBrace_rowid = -1;
	m_VSMousePos = 0;
	m_SpellCheck = false;
	m_BookmarkInSearch = false;
	m_TypewriterMode = m_Config->ReadBool( wxT( "TypewriterMode" ),   false );
	m_LDClickHighlight = m_Config->ReadBool( wxT( "LDoubleClickHighlight" ), true );
	m_HasBackup = true;
#ifndef PYMADEDIT_DLL
	m_Config->Read( wxT( "SpellCheck" ), &m_SpellCheck, false );

	if( SpellCheckerManager::Instance().GetSelectedDictionaryNumber() == -1 ) m_SpellCheck = false;

	if( m_SpellCheck )
		m_SpellCheckerPtr = SpellCheckerManager::Instance().GetSpellChecker();

#endif
	m_InsertMode = true;
	m_CaretType = ctVerticalLine;
	m_MouseLeftDown = false;
	m_MouseLeftDoubleClick = false;
	m_MouseAtHexTextArea = false;
	m_MouseInWindow = true;
	m_MouseMotionTimer = new MadMouseMotionTimer( this );
	m_MouseSelectToCopy = m_Config->ReadBool( wxT( "MouseSelectToCopy" ), true );
#if defined(__WXGTK__)
	m_MouseSelectToCopyWithCtrlKey = m_Config->ReadBool( wxT( "MouseSelectToCopyWithCtrlKey" ), false );
#else
	m_MouseSelectToCopyWithCtrlKey = m_Config->ReadBool( wxT( "MouseSelectToCopyWithCtrlKey" ), true );
#endif
	m_MiddleMouseToPaste = m_Config->ReadBool( wxT( "MiddleMouseToPaste" ), true );
	m_AutoFillColumnPaste = m_Config->ReadBool( wxT( "AutoFillColumnPaste" ), true );
	m_HexTopRow = -1;
	m_HexRowCount = 0;
	m_HexDigitBitmap = nullptr;
	m_LoadingFile = false;
	m_ModReloaded = true;
	m_OnSelectionChanged = nullptr;
	m_OnStatusChanged = nullptr;
	m_OnFontChanged = nullptr;
	m_OnEncodingChanged = nullptr;
	m_OnSyntaxChanged = nullptr;
	m_OnLineSpaceChanged = nullptr;
	m_OnToggleWindow = nullptr;
	m_OnMouseRightUp = nullptr;
	m_RightClickMenu = nullptr;
	m_OnVSMouseRightUp = nullptr;
	m_OnHSMouseRightUp = nullptr;
	m_OnActivate = nullptr;
	// set fonts
	m_TextFontSpaceWidth = 10;
	memset( m_TextFontWidths, 0, sizeof( m_TextFontWidths ) );
	memset( m_HexFontWidths, 0, sizeof( m_HexFontWidths ) );
	wxString fontname;
	int fontsize;
	m_Config->Read( wxString( wxT( "DefaultTextFont" ) ), &fontname );
	if(fontname.IsEmpty())
	{
		fontname = m_Config->Read( wxString( wxT( "/Fonts/" ) ) + m_Encoding->GetName(), m_Encoding->GetFontName() );
	}
	fontsize = m_Config->ReadLong( wxT( "DefaultTextFontSize" ), DEFAULT_FONT_SIZE );
	if(fontsize < 0) fontsize = DEFAULT_FONT_SIZE;
	if(fontsize > MAX_FONT_SIZE) fontsize = MAX_FONT_SIZE;
	m_Config->Write( wxT( "TextFontSize" ),  fontsize); // Make sure
	SetTextFont( fontname, fontsize, true );
#ifdef __WXMSW__
	fontname = wxT( "Courier New" );
#elif defined(__APPLE__) && defined(__MACH__)
	fontname = wxT( "Monaco" );
#else
	fontname = wxT( "Monospace" );
#endif
	fontname = m_Config->Read( wxString( wxT( "HexFontName" ) ), fontname );
	m_Config->Read( wxT( "HexFontSize" ), &fontsize, DEFAULT_FONT_SIZE );
	if(fontsize < 0) fontsize = DEFAULT_FONT_SIZE;
	if(fontsize > MAX_FONT_SIZE) fontsize = MAX_FONT_SIZE;
	m_Config->Write( wxT( "HexFontSize" ),  fontsize); // Make sure	
	SetHexFont( fontname, fontsize, true );
	m_Printing = 0;
	m_PrintPageCount = 0;
	InvertRect = nullptr;
	m_Painted = false;
	m_LineNumberAreaWidth = GetLineNumberAreaWidth( 0 );

	if( m_DisplayBookmark && m_LineNumberAreaWidth == 0 )
		m_BookmarkWidth = m_RowHeight/2;
	else
		m_BookmarkWidth = 0;

	m_LastPaintBitmap = -1;
	m_UseDefaultSyntax = false;
	m_SearchWholeWord = false;
	m_MaxDisplaySize = m_Config->ReadLong( wxT( "/UIView/MaxDisplaySize" ), 256 );
	if(m_MaxDisplaySize < 128 )	m_MaxDisplaySize = 128;
	if( m_MaxDisplaySize > 1024) m_MaxDisplaySize = 1024;
	m_Config->Write( wxT( "/UIView/MaxDisplaySize" ), m_MaxDisplaySize ); // Make sure	
#if 0
//#ifdef __WXGTK__
	ConnectToFixedKeyPressHandler();
#endif
	m_lastDoubleClick = 0;

	m_PartialLoadMode = true;
	m_PosOffsetBeg = 0;
	m_PosOffsetEnd = 0;
	m_LineidOffset = 0;
	m_LineEndPos.reserve(0xFFFF);

	Bind(CHECK_MODIFICATION_TIME, &MadEdit::OnCheckModificationTime, this );
	Bind(UPDATE_HSCROLL_POS, &MadEdit::OnUpdateHScrollPos, this );

	Bind( wxEVT_CHAR, &MadEdit::OnChar, this );
	Bind( wxEVT_KEY_DOWN, &MadEdit::OnKeyDown, this );
	Bind( wxEVT_KEY_UP, &MadEdit::OnKeyUp, this );

	Bind( wxEVT_LEFT_DOWN, &MadEdit::OnMouseLeftDown, this );
	Bind( wxEVT_LEFT_UP, &MadEdit::OnMouseLeftUp, this );
	Bind( wxEVT_LEFT_DCLICK, &MadEdit::OnMouseLeftDClick, this );
	Bind( wxEVT_MOTION, &MadEdit::OnMouseMotion, this );
	Bind( wxEVT_RIGHT_UP, &MadEdit::OnMouseRightUp, this );
	Bind( wxEVT_MIDDLE_UP, &MadEdit::OnMouseMiddleUp, this );

	Bind( wxEVT_SET_FOCUS, &MadEdit::OnSetFocus, this );
	Bind( wxEVT_KILL_FOCUS, &MadEdit::OnKillFocus, this );

	Bind( wxEVT_SIZE, &MadEdit::OnSize, this );
	Bind( wxEVT_SCROLL_TOP, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_BOTTOM, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_LINEUP, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_LINEDOWN, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_PAGEUP, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_PAGEDOWN, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_THUMBTRACK, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_THUMBRELEASE, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_CHANGED, &MadEdit::OnVScroll, this, ID_VSCROLLBAR);
	Bind( wxEVT_SCROLL_TOP, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_BOTTOM, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_LINEUP, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_LINEDOWN, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_PAGEUP, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_PAGEDOWN, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_THUMBTRACK, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_THUMBRELEASE, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_SCROLL_CHANGED, &MadEdit::OnHScroll, this, ID_HSCROLLBAR);
	Bind( wxEVT_MOUSEWHEEL, &MadEdit::OnMouseWheel, this );
	Bind( wxEVT_ENTER_WINDOW, &MadEdit::OnMouseEnterWindow, this );
	Bind( wxEVT_LEAVE_WINDOW, &MadEdit::OnMouseLeaveWindow, this );
	Bind( wxEVT_MOUSE_CAPTURE_LOST, &MadEdit::OnMouseCaptureLost, this );

	Bind( wxEVT_ERASE_BACKGROUND, &MadEdit::OnEraseBackground, this );
	Bind( wxEVT_PAINT, &MadEdit::OnPaint, this );
}

MadEdit::~MadEdit()
{
	--ms_Count;

	if( ms_Count == 0 )
	{
		wxSetCursor( wxNullCursor ); // reset to default
	}

	delete m_Lines;
	delete m_Encoding;
	delete m_Syntax;
	delete m_UndoBuffer;
	delete m_CaretTracker;
	delete []m_WordBuffer;
#ifdef __WXMSW__
	delete []m_WCWordBuffer;
	delete []m_WCWidthBuffer;
#endif
	delete []m_WidthBuffer;

	if( m_HexDigitBitmap ) delete m_HexDigitBitmap;

	delete m_ClientBitmap;
	delete m_MarkBitmap;
	m_MouseMotionTimer->Stop();
	m_MouseMotionTimer->DeletePendingEvents();
	delete m_MouseMotionTimer;
	DeletePendingEvents();
}

//==================================================

int MadEdit::GetLineByRow( MadLineIterator &lit, wxFileOffset &pos, int &rowid )
{
	int lineid, count, rowid0;

	// begin <= rowid <= validpos
	if( rowid <= m_ValidPos_rowid )
	{
		if( m_ValidPos_rowid - rowid <= rowid ) // rowid <-- validpos
		{
			lit     = m_ValidPos_iter;
			lineid  = m_ValidPos_lineid;
			pos     = m_ValidPos_pos;
			rowid0  = m_ValidPos_rowid;

			if( rowid0 > rowid )
			{
				do
				{
					--lit;
					--lineid;
					rowid0 -= int( lit->RowCount() );
					pos -= lit->m_Size;
				}
				while( rowid0 > rowid );
			}

			rowid = rowid0;
		}
		else                                // begin --> rowid
		{
			lit     = m_Lines->m_LineList.begin();
			lineid  = 0;
			pos     = 0;
			rowid0  = 0;

			if( rowid0 + ( count = int( lit->RowCount() ) ) <= rowid )
			{
				do
				{
					rowid0 += count;
					pos += lit->m_Size;
					++lit;
					++lineid;
				}
				while( rowid0 + ( count = int( lit->RowCount() ) ) <= rowid );
			}

			rowid = rowid0;
		}
	}
	else    // validpos < rowid <= end
	{
		if( rowid - m_ValidPos_rowid < int( m_Lines->m_RowCount ) - rowid ) // validpos --> rowid
		{
			lit     = m_ValidPos_iter;
			lineid  = m_ValidPos_lineid;
			pos     = m_ValidPos_pos;
			rowid0  = m_ValidPos_rowid;

			if( rowid0 + ( count = int( lit->RowCount() ) ) <= rowid )
			{
				do
				{
					rowid0 += count;
					pos += lit->m_Size;
					++lit;
					++lineid;
				}
				while( rowid0 + ( count = int( lit->RowCount() ) ) <= rowid );
			}

			rowid = rowid0;
		}
		else                                                    // rowid <-- end
		{
			lit     = m_Lines->m_LineList.end();
			--lit;
			lineid  = int( m_Lines->m_LineCount ) - 1;
			pos     = m_Lines->m_Size - lit->m_Size;
			rowid0  = int( m_Lines->m_RowCount - lit->RowCount() );

			if( rowid0 > rowid )
			{
				do
				{
					--lit;
					--lineid;
					rowid0 -= int( lit->RowCount() );
					pos -= lit->m_Size;
				}
				while( rowid0 > rowid );
			}

			rowid = rowid0;
		}
	}

	if( m_UpdateValidPos < 0 || ( m_UpdateValidPos > 0 && rowid < m_ValidPos_rowid ) )
	{
		m_ValidPos_iter     = lit;
		m_ValidPos_lineid   = lineid;
		m_ValidPos_rowid    = rowid;
		m_ValidPos_pos      = pos;
	}

	return lineid;
}

int MadEdit::GetLineByPos( MadLineIterator &lit, wxFileOffset &pos, int &rowid )
{
	int lineid;
	wxFileOffset pos0, size;

	// begin <= pos <= validpos
	if( pos <= m_ValidPos_pos )
	{
		if( m_ValidPos_pos - pos <= pos )   // pos <-- validpos
		{
			lit     = m_ValidPos_iter;
			lineid  = m_ValidPos_lineid;
			pos0    = m_ValidPos_pos;
			rowid   = m_ValidPos_rowid;

			if( pos0 > pos )
			{
				do
				{
					--lit;
					--lineid;
					rowid -= int( lit->RowCount() );
					pos0 -= lit->m_Size;
				}
				while( pos0 > pos );
			}

			pos = pos0;
		}
		else                                // begin --> pos
		{
			lit     = m_Lines->m_LineList.begin();
			lineid  = 0;
			pos0    = 0;
			rowid   = 0;

			if( ( size = lit->m_Size ) != 0 && pos0 + size <= pos )
			{
				do
				{
					pos0 += size;
					rowid += int( lit->RowCount() );
					++lit;
					++lineid;
				}
				while( ( size = lit->m_Size ) != 0 && pos0 + size <= pos );
			}

			pos = pos0;
		}
	}
	else    // validpos < pos <= end
	{
		if( pos - m_ValidPos_pos < m_Lines->m_Size - pos ) // validpos --> pos
		{
			lit     = m_ValidPos_iter;
			lineid  = m_ValidPos_lineid;
			pos0    = m_ValidPos_pos;
			rowid   = m_ValidPos_rowid;

			if( ( size = lit->m_Size ) != 0 && pos0 + size <= pos )
			{
				do
				{
					pos0 += size;
					rowid += int( lit->RowCount() );
					++lit;
					++lineid;
				}
				while( ( size = lit->m_Size ) != 0 && pos0 + size <= pos );
			}

			pos = pos0;
		}
		else                                            // pos <-- end
		{
			lit     = m_Lines->m_LineList.end();
			--lit;
			lineid  = int( m_Lines->m_LineCount - 1 );
			pos0    = m_Lines->m_Size - lit->m_Size;
			rowid   = int( m_Lines->m_RowCount - lit->RowCount() );

			if( pos0 > pos )
			{
				do
				{
					--lit;
					--lineid;
					rowid -= int( lit->RowCount() );
					pos0 -= lit->m_Size;
				}
				while( pos0 > pos );
			}

			pos = pos0;
		}
	}

	if( m_UpdateValidPos < 0 || ( m_UpdateValidPos > 0 && pos < m_ValidPos_pos ) )
	{
		m_ValidPos_iter     = lit;
		m_ValidPos_lineid   = lineid;
		m_ValidPos_rowid    = rowid;
		m_ValidPos_pos      = pos;
	}

	return lineid;
}

int MadEdit::GetLineByLine( /*OUT*/ MadLineIterator &lit, /*OUT*/ wxFileOffset &pos, /*IN*/ int lineid )
{
	int rowid, lineid0;

	// begin <= lineid <= validpos
	if( lineid <= m_ValidPos_lineid )
	{
		if( m_ValidPos_lineid - lineid <= lineid )   // lineid <-- validpos
		{
			lit     = m_ValidPos_iter;
			lineid0 = m_ValidPos_lineid;
			pos     = m_ValidPos_pos;
			rowid   = m_ValidPos_rowid;

			if( lineid0 > lineid )
			{
				do
				{
					--lit;
					--lineid0;
					rowid -= int( lit->RowCount() );
					pos -= lit->m_Size;
				}
				while( lineid0 > lineid );
			}
		}
		else                                // begin --> lineid
		{
			lit     = m_Lines->m_LineList.begin();
			lineid0 = 0;
			pos     = 0;
			rowid   = 0;

			if( lineid0 + 1 <= lineid )
			{
				do
				{
					rowid += int( lit->RowCount() );
					pos += lit->m_Size;
					++lit;
					++lineid0;
				}
				while( lineid0 + 1 <= lineid );
			}
		}
	}
	else    // validpos < lineid <= end
	{
		if( lineid - m_ValidPos_lineid < int( m_Lines->m_LineCount ) - lineid ) // validpos --> lineid
		{
			lit     = m_ValidPos_iter;
			lineid0 = m_ValidPos_lineid;
			pos     = m_ValidPos_pos;
			rowid   = m_ValidPos_rowid;

			if( lineid0 + 1 <= lineid )
			{
				do
				{
					rowid += int( lit->RowCount() );
					pos += lit->m_Size;
					++lit;
					++lineid0;
				}
				while( lineid0 + 1 <= lineid );
			}
		}
		else                                            // lineid <-- end
		{
			lit     = m_Lines->m_LineList.end();
			--lit;
			lineid0 = int( m_Lines->m_LineCount - 1 );
			pos     = m_Lines->m_Size - lit->m_Size;
			rowid   = int( m_Lines->m_RowCount - lit->RowCount() );

			if( lineid0 > lineid )
			{
				do
				{
					--lit;
					--lineid0;
					rowid -= int( lit->RowCount() );
					pos   -= lit->m_Size;
				}
				while( lineid0 > lineid );
			}
		}
	}

	if( m_UpdateValidPos < 0 || ( m_UpdateValidPos > 0 && lineid < m_ValidPos_lineid ) )
	{
		m_ValidPos_iter     = lit;
		m_ValidPos_lineid   = lineid;
		m_ValidPos_rowid    = rowid;
		m_ValidPos_pos      = pos;
	}

	return rowid;
}


void MadEdit::UpdateCaret( MadCaretPos &caretPos,
						   MadUCQueue &ucharQueue, vector<int> &widthArray,
						   int &ucharPos )
{
	MadRowIndexIterator riter = caretPos.iter->m_RowIndices.begin();

	// linepos in middle of BOM
	if( caretPos.linepos < riter->m_Start )
	{
		if( m_EditMode == emHexMode )
			return;

		wxASSERT( caretPos.subrowid == 0 );
		int size = riter->m_Start - caretPos.linepos;
		caretPos.linepos = riter->m_Start;
		caretPos.pos += size;
	}

	std::advance( riter, caretPos.subrowid );
	const int rowwidth = riter->m_Width;
	wxFileOffset rowpos = caretPos.linepos - riter->m_Start;
	wxASSERT( rowpos >= 0 );
	//if(rowpos < 0)  {    rowpos += 0;  }
	ucharQueue.clear();
	widthArray.clear();
	ucharPos = 0;
	caretPos.xpos = 0;
	caretPos.extraspaces = 0;
	int xpos = 0;

	if( IsTextFile() ) // must be a text file
	{
		m_Lines->InitNextUChar( caretPos.iter, riter->m_Start );
		const wxFileOffset & row_endpos = ( ++riter )->m_Start;
		MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

		while( m_Lines->m_NextUChar_Pos < row_endpos
				&& ( m_Lines->*NextUChar )( ucharQueue ) )
		{
			const MadUCPair & ucp = ucharQueue.back();
			ucs4_t uc = ucp.first;

			if( uc == 0x0D || uc == 0x0A )
			{
				ucharQueue.pop_back();
				break;
			}

			int width = GetUCharWidth( uc );

			if( uc == 0x09 )
			{
				int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
				width = rowwidth - xpos;
				tabwidth -= ( xpos % tabwidth );

				if( tabwidth < width )
					width = tabwidth;
			}

			widthArray.push_back( width );
			xpos += width;

			if( rowpos >= ucp.second )
			{
				rowpos -= ucp.second;
				caretPos.xpos += width;
				++ucharPos;
			}
			else
				if( rowpos != 0 ) //rowpos in middle of char
				{
					if( m_EditMode != emHexMode )
					{
						caretPos.pos -= rowpos;
						caretPos.linepos -= rowpos;
					}

					rowpos = 0;
				}
		}

		// rowpos in middle of EOL
		if( rowpos != 0 && m_EditMode != emHexMode )
		{
			caretPos.pos -= rowpos;
			caretPos.linepos -= rowpos;
			//((wxFrame*)wxTheApp->GetTopWindow())->SetTitle(wxString::Format(wxT("%s %s:%s"), wxLongLong(caretPos.linepos).ToString(), wxLongLong(caretPos.iter->m_NewLineSize).ToString(), wxLongLong(caretPos.iter->m_Size).ToString()));
			wxASSERT( caretPos.linepos + caretPos.iter->m_NewLineSize == caretPos.iter->m_Size );
		}
	}
}

void MadEdit::UpdateCaretByPos( MadCaretPos &caretPos,
								MadUCQueue &ucharQueue, vector<int> &widthArray,
								int &ucharPos )
{
	wxFileOffset pos = caretPos.pos;
	caretPos.lineid = GetLineByPos( caretPos.iter, pos, caretPos.rowid );
	caretPos.linepos = caretPos.pos - pos;
	caretPos.subrowid = 0;
	MadRowIndexIterator rit = caretPos.iter->m_RowIndices.begin();
	MadRowIndexIterator lastrit = caretPos.iter->m_RowIndices.end();
	--lastrit;
	++rit;

	while( rit != lastrit && caretPos.linepos >= rit->m_Start )
	{
		++caretPos.rowid;
		++caretPos.subrowid;
		++rit;
	}

	UpdateCaret( caretPos, ucharQueue, widthArray, ucharPos );
}

void MadEdit::UpdateCaretByXPos( int xPos, MadCaretPos &caretPos,
								 MadUCQueue &ucharQueue, vector<int> &widthArray,
								 int &ucharPos )
{
	MadRowIndexIterator riter = caretPos.iter->m_RowIndices.begin();

	if( m_EditMode == emColumnMode )
	{
		if( xPos > m_MaxColumnRowWidth ) xPos = m_MaxColumnRowWidth;
	}

	std::advance( riter, caretPos.subrowid );
	const int rowwidth = riter->m_Width;
	m_Lines->InitNextUChar( caretPos.iter, riter->m_Start );
	const wxFileOffset & row_endpos = ( ++riter )->m_Start;
	ucharQueue.clear();
	widthArray.clear();
	ucharPos = 0;
	caretPos.xpos = 0;
	caretPos.extraspaces = 0;
	int xpos = 0, restxpos = xPos;
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

	while( m_Lines->m_NextUChar_Pos < row_endpos
			&& ( m_Lines->*NextUChar )( ucharQueue ) )
	{
		const MadUCPair & ucp = ucharQueue.back();
		ucs4_t uc = ucp.first;

		if( uc == 0x0D || uc == 0x0A )
		{
			ucharQueue.pop_back();
			break;
		}

		int width = GetUCharWidth( uc );

		if( uc == 0x09 )
		{
			int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
			width = rowwidth - xpos;
			tabwidth -= ( xpos % tabwidth );

			if( tabwidth < width )
				width = tabwidth;
		}

		widthArray.push_back( width );
		xpos += width;

		if( restxpos > 0 )
		{
			if( restxpos > ( width >> 1 ) )
			{
				restxpos -= width;
				++ucharPos;
				caretPos.xpos += width;
				caretPos.linepos += ucp.second;
				caretPos.pos += ucp.second;
			}
			else
			{
				restxpos = 0;
			}
		}
	}

	if( restxpos > 0 && m_EditMode == emColumnMode )
	{
		int spacewidth = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
		caretPos.extraspaces = restxpos / spacewidth;
		restxpos -= int( caretPos.extraspaces * spacewidth );

		if( restxpos > ( spacewidth >> 1 ) )
		{
			++caretPos.extraspaces;
		}

		caretPos.xpos += int( caretPos.extraspaces * spacewidth );
	}
}

void MadEdit::UpdateSelectionPos()
{
	MadUCQueue ucharQueue;
	vector<int> widthArray;
	int ucharPos;
	UpdateCaretByPos( m_SelectionPos1, ucharQueue, widthArray, ucharPos );
	UpdateCaretByPos( m_SelectionPos2, ucharQueue, widthArray, ucharPos );

	m_ZeroSelection = false;
	if( m_SelectionPos1.pos == m_SelectionPos2.pos )
	{
		m_Selection = false;
		m_SelFirstRow = INT_MAX;
		m_SelLastRow = -1;
	}
	else
	{
		if( m_SelectionPos1.pos < m_SelectionPos2.pos )
		{
			m_SelectionBegin = &m_SelectionPos1;
			m_SelectionEnd = &m_SelectionPos2;
		}
		else
		{
			m_SelectionBegin = &m_SelectionPos2;
			m_SelectionEnd = &m_SelectionPos1;
		}

		m_SelFirstRow = int( m_SelectionBegin->rowid );
		m_SelLastRow = int( m_SelectionEnd->rowid );

		if( m_EditMode == emColumnMode )
		{
			if( m_SelectionPos1.xpos < m_SelectionPos2.xpos )
			{
				m_SelLeftXPos = m_SelectionPos1.xpos;
				m_SelRightXPos = m_SelectionPos2.xpos;
			}
			else
			{
				m_SelLeftXPos = m_SelectionPos2.xpos;
				m_SelRightXPos = m_SelectionPos1.xpos;
			}
		}
	}

	//RecordAsMadMacro(this, wxString(wxT("UpdateSelectionPos()")));
}

//===========================================================================

void MadEdit::InvertRectNormal( wxDC *dc, int x, int y, int w, int h )
{
	dc->Blit( x, y, w, h, dc, x, y, wxINVERT );
}

void MadEdit::InvertRectManual( wxDC *dc, int x, int y, int w, int h )
{
	//int ox=x, oy=y, ow=w, oh=h;

	// calc actual x,y,w,h
	if( x < 0 )
	{
		w += x;

		if( w <= 0 ) return;

		x = 0;
	}
	else
		if( x >= m_ClientWidth )
		{
			return;
		}

	if( x + w > m_ClientWidth )
	{
		w = m_ClientWidth - x;
	}

	if( w <= 0 ) return;

	if( y < 0 )
	{
		h += y;

		if( h <= 0 ) return;

		y = 0;
	}
	else
		if( y >= m_ClientHeight )
		{
			return;
		}

	if( y + h > m_ClientHeight )
	{
		h = m_ClientHeight - y;
	}

	if( h <= 0 ) return;

	/***
	if(w<=0 || h<=0)
	{
	    char str[256];
	    sprintf(str, "[%d %d] %d %d %d %d\n", m_ClientWidth, m_ClientHeight, ox, oy, ow, oh);
	    std::cout<<str;
	}
	wxASSERT(w>0 && h>0);
	***/
	// get rect data from dc
	wxBitmap *bmp = new wxBitmap( w, h );
	wxMemoryDC memdc;
	memdc.SelectObject( *bmp );
	memdc.Blit( 0, 0, w, h, dc, x, y ); // copy to m_InvertDC
	memdc.SelectObject( wxNullBitmap );
	wxImage img = bmp->ConvertToImage();
	// invert data
	unsigned char* olddata = img.GetData();
	wxASSERT(olddata != nullptr);
	int size = w * h * 3;
	unsigned char* newdata = ( unsigned char* )malloc( size );
	wxASSERT(newdata != nullptr);
	int* pold = ( int* )olddata;
	int* pnew = ( int* )newdata;

	if( size >= int( sizeof( int ) * 8 ) )
	{
		do
		{
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
			( *pnew++ ) = ~( *pold++ );
		}
		while( ( size -= sizeof( int ) * 8 ) >= int( sizeof( int ) * 8 ) );
	}

	if( size >= int( sizeof( int ) ) )
	{
		do
		{
			( *pnew++ ) = ~( *pold++ );
		}
		while( ( size -= sizeof( int ) ) >= int( sizeof( int ) ) );
	}

	if( size > 0 )
	{
		//wxASSERT(size<sizeof(int));
		unsigned char* po = ( unsigned char* )pold;
		unsigned char* pn = ( unsigned char* )pnew;
#if SIZEOF_INT == 4
		( *pn++ ) = ~( *po++ );

		if( --size > 0 )
		{
			( *pn++ ) = ~( *po++ );

			if( --size > 0 )
			{
				( *pn++ ) = ~( *po++ );
			}
		}

#else

		do
		{
			( *pn++ ) = ~( *po++ );
		}
		while( --size > 0 );

#endif
	}

	// draw back to dc
	img.SetData( newdata );
	wxBitmap *newbmp = new wxBitmap( img );
	dc->DrawBitmap( *newbmp, x, y );
	delete bmp;
	delete newbmp;
}


#if     FIXINVERT == 0
	#   define INVERT_RECT(dc,x,y,w,h) dc->Blit(x, y, w, h, dc, x, y, wxINVERT)
#elif   FIXINVERT == 1
	#   define INVERT_RECT(dc,x,y,w,h) (this->*InvertRect)(dc, x, y, w, h)
#elif   FIXINVERT == 2
	#   define INVERT_RECT(dc,x,y,w,h) InvertRectManual(dc, x, y, w, h)
#else
	#   error FIXINVERT not defined
#endif

//===========================================================================


#if 0
void MadEdit::PaintText( wxDC *dc, int x, int y, const ucs4_t *text, const int *width, int count )
{
	wxString text1, text2;
	text1.resize( 1 );
	text2.resize( 2 );
	wchar_t wcbuf2[3] = {0, 0, 0};

	while( count > 0 )
	{
#ifdef __WXMSW__

		if( *text < 0x10000 )
		{
			if( *text != 0x20 )
			{
				text1.SetChar( 0, *text );
				dc->DrawText( text1, x, y );
			}
		}
		else
		{
			m_Encoding->UCS4toUTF16LE_U10000( *text, ( wxByte* )wcbuf2 );
			text2.SetChar( 0, wcbuf2[0] );
			text2.SetChar( 1, wcbuf2[1] );
			dc->DrawText( text2, x, y );
		}

#else

		if( *text != 0x20 )
		{
			text1.SetChar( 0, *text );
			dc->DrawText( text1, x, y );
		}

#endif
		x += *width;
		++text;
		++width;
		--count;
	}
}
#endif

void MadEdit::PaintText( wxDC *dc, int x, int y, const ucs4_t *text, const int *width, int count, int minleft, int maxright, bool spellmark/*=false*/ )
{
	int totalwidth = 0;
#ifdef __WXMSW__
	const ucs4_t  *pu = text;
	const int     *pw = width;
	wchar_t *pwc = m_WCWordBuffer;
	int     *pwcw = m_WCWidthBuffer;
	int wcount = 0, wcstart = 0;
	int nowright = x;
	int nowleft = x;
	bool nowleftend = false;
	wxChar wcbuf2[2];

	for( int i = 0; i < count && nowright < maxright; ++i, ++pwc, ++pwcw, ++wcount )
	{
		ucs4_t uc = *pu++;
		int ucw = *pw++;
		totalwidth += ucw;

		if( uc < 0x10000 )
		{
			*pwc = uc;
			*pwcw = ucw;

			if(( nowleftend == false ) && (( nowleft + ucw ) < minleft ))
			{
				nowleft += ucw ;
				wcstart++;
			}
			else
			{
				nowleftend = true;
			}
		}
		else
		{
			m_Encoding->UCS4toUTF16LE_U10000( uc, ( wxByte* )wcbuf2 );
			*pwc++ = wcbuf2[0];
			*pwc = wcbuf2[1];
			*pwcw++ = ucw;
			*pwcw = 0;
			++wcount;

			if(( nowleftend == false ) && (( nowleft + ucw ) < minleft ))
			{
				nowleft += ucw ;
				wcstart += 2;
			}
			else
			{
				nowleftend = true;
			}
		}

		nowright += ucw;
	}

	UINT len = wcount - wcstart;

	if( len > 0 )
	{
		HDC hdc = ( HDC )dc->GetHDC();
		SetTextColor( hdc, m_Syntax->nw_Color.GetPixel() );
		::ExtTextOut( hdc, nowleft, y, 0, nullptr, m_WCWordBuffer + wcstart, len, m_WCWidthBuffer + wcstart );
	}

#elif defined(__WXGTK20__) && (wxMAJOR_VERSION < 3)
	int nowleft = x;
	if( !InPrinting() )
	{
		const ucs4_t *pu = text;
		const int    *pw = width;
		int wcount = 0, wcstart = 0;
		int nowright = x;
		bool nowleftend = false;
		wxString str;

		for( int i = count; i > 0 && nowright < maxright; --i, ++pu, ++wcount )
		{
			int ucw = *pw++;
			totalwidth += ucw;

			if(( nowleftend == false ) && (( nowleft + ucw ) < minleft ))
			{
				nowleft += ucw ;
				++wcstart;
			}
			else
			{
				nowleftend = true;
				str << wxChar( *pu );
			}

			nowright += ucw;
		}

		if( wcount > wcstart )
		{
			//((wxMemoryDCExt*)dc)->m_Widths=width+wcstart;
			//dc->DrawText(str, nowleft, y);
			GTK2_DrawText( ( wxMemoryDC* )dc, m_Encoding, width + wcstart, str, nowleft, y );
		}
	}
	else
#endif
#ifndef __WXMSW__
	//other platform or printing
	int nowleft = x;
	{
		wxString text1( wxT( '1' ) );
		const ucs4_t  *pu = text;
		const int     *pw = width;

		for( int i = count; i > 0 && nowleft < maxright; --i, nowleft += *pw, ++pu, ++pw )
		{
			if( nowleft + *pw > minleft )
			{
				text1.SetChar( 0, *pu );
				dc->DrawText( text1, nowleft, y );
			}

			totalwidth += *pw;
		}
	}

#endif

	if( spellmark )
	{
		wxColour color( 255, 0, 0 ); /*RED*/
		int delta = 3, yd[2] = {3, 0};
		dc->SetPen( *( wxThePenList->FindOrCreatePen( color, 1, wxPENSTYLE_SOLID/*wxDOT*/ ) ) );
		nowleft = ( x > minleft ? x : minleft );

		for( int i = 0; delta < totalwidth; ++i )
		{
			dc->DrawLine( nowleft + delta - 3, y + m_RowHeight - yd[i % 2] - 1, nowleft + delta, y + m_RowHeight - yd[( i + 1 ) % 2] - 1 );
			delta += 3;
		}
	}
}

template< class T >
bool IsTheSame( vector< T >& First, T * Second, size_t Len )
{
	T *p = Second;
	size_t i = 0;

	wxASSERT(Len <= First.size());
	while( i < Len )
	{
		if( First[i++] != *( p++ ) ) return false;
	}

	return true;
}

int MadEdit::GetIndentCountByPos( wxFileOffset pos )
{
	MadLineIterator lineiter, lit;
	int row = -1, count = 0;
	vector <ucs4_t> spaces;
	int lineid = GetLineByPos( lit, pos, row );
	GetIndentSpaces( lineid, lit, spaces, true, true );
	long totalSpaces = 0;
	for (int i = 0; i < spaces.size(); ++i)
	{
		if (spaces[i] == (ucs4_t)'\t') totalSpaces += GetIndentColumns();
		else totalSpaces += 1;
	}
	count = totalSpaces / GetIndentColumns();
	return count;
}

void MadEdit::SelectWholeLine()
{
	SetSelection( m_SelectionBegin->pos - m_SelectionBegin->linepos, m_SelectionEnd->pos - m_SelectionEnd->linepos + m_SelectionEnd->iter->m_Size );
}

void MadEdit::ReplaceSelection( wxString &ws )
{
	vector < ucs4_t > ucs;
	TranslateText( ws.c_str(), ws.Len(), &ucs, false );

	if( ucs.size() )
		InsertString( &ucs[0], ucs.size(), false, false, false );

	//else
	//    DeleteSelection(false,vector < int > * rpos,bool bColumnEditing)
}

void MadEdit::PaintTextLines( wxDC *dc, const wxRect &rect, int toprow, int rowcount, const wxColor &bgcolor )
{
	MadLineIterator lineiter;
	int subrowid = toprow;
	wxFileOffset notused = -1;
	int lineid = GetLineByRow( lineiter, notused, subrowid ) + 1;
	subrowid = toprow - subrowid;
	bool displaylinenumber = true; // check first row for displayning line-number or not
	int wordwidth = -1, wordlength = -1, lastwordwidth = -1;

	wxASSERT(subrowid >= 0);
	if( m_Syntax->m_SynAttr->m_CheckState )
	{
		m_Syntax->InitNextWord2( lineiter, 0 );

		if( subrowid != 0 )         // ignore rows above toprow
		{
			displaylinenumber = false;
			size_t hiderows = subrowid;

			do
			{
				do
				{
					if(m_Syntax->NextWord( wordwidth ) == 0) break;
				}
				while( m_Syntax->nw_LineWidth != 0 );
			}
			while( --hiderows != 0 );
		}
	}
	else
	{
		m_Syntax->InitNextWord2( lineiter, subrowid );

		if( subrowid != 0 )         // ignore rows above toprow
		{
			displaylinenumber = false;
		}
	}

	m_Syntax->SetAttributes( aeSpace );
	const wxColor bm_BgColor = m_Syntax->nw_BgColor;
	const int minleft = rect.GetLeft() + m_LineNumberAreaWidth + m_BookmarkWidth;
	const int maxright = rect.GetRight();
	const unsigned int ncount = m_LineNumberAreaWidth / m_TextFontMaxDigitWidth;
	const int leftpos = minleft + m_LeftMarginWidth - m_DrawingXPos;
	bool bPaintSelection = m_Selection;

	if( bPaintSelection )
	{
		const int brow = toprow + rowcount - 1;

		if( toprow > m_SelectionEnd->rowid || brow < m_SelectionBegin->rowid )
		{
			bPaintSelection = false;
		}
		else
		{
			if( m_EditMode == emColumnMode )
			{
				const int selleftxpos = leftpos + m_SelLeftXPos;
				const int selrightxpos = leftpos + m_SelRightXPos;
				const int ww = m_TextFontAveCharWidth << 1;

				if( selleftxpos > ( maxright + ww ) || selrightxpos < ( minleft - ww ) )
				{
					bPaintSelection = false;
				}
			}
		}
	}

	int SpacingHeight = m_RowHeight - m_TextFontHeight;
	int left, text_top = rect.GetTop() + ( SpacingHeight >> 1 );
	int row_top = rect.GetTop();
	const int rectright = rect.x + rect.width;
	int SelLeft = -1, SelRight = -1;
	int xpos1 = 0, xpos2 = 0;
	bool reverseLineNum = false;
	const int wspace = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
	wxString lnums;
	wxColor current_bgcolor;
	//wxColour * clr = &g_MadDefBmkColor, *bgclr = &g_MadDefBmkBgColor;
	//wxColour * hwclr = &g_MadDefHWColor, *hwbgclr = &g_MadDefHWBgColor;

	if( m_DisplayLineNumber )
	{
		// paint bg
		m_Syntax->SetAttributes( aeLineNumber );

		if( m_Syntax->nw_BgColor != bgcolor )
		{
			dc->SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_BgColor, 1, wxPENSTYLE_SOLID ) ) );
			dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_BgColor ) ) );
			dc->DrawRectangle( rect.GetLeft(), rect.GetTop(), m_LineNumberAreaWidth, rect.GetHeight() ) ;
		}

		reverseLineNum = ( GetLayoutDirection() == wxLayout_RightToLeft );
	}

	// Begin Paint Lines
	for( ;; )                       // every line
	{
		do                          // every row of line
		{
			left = leftpos;
			current_bgcolor = bgcolor;
			// paint left margin
			int leftmarginwidth = m_LeftMarginWidth - m_DrawingXPos;

			if( leftmarginwidth > 0 )
			{
				wxColor &c = m_Syntax->GetAttributes( aeText )->bgcolor;

				if( c != current_bgcolor )
				{
					current_bgcolor = c;
					dc->SetPen( *( wxThePenList->FindOrCreatePen( c, 1, wxPENSTYLE_SOLID ) ) );
					dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( c ) ) );
					dc->DrawRectangle( minleft, row_top, rectright - minleft, m_RowHeight );
				}
			}

			if( bPaintSelection )
			{
				SelLeft = SelRight = leftpos;
				xpos1 = xpos2 = 0;

				if( IsTextMode() )
				{
					if( toprow == m_SelectionBegin->rowid )
					{
						xpos1 = m_SelectionBegin->xpos;
					}

					if( toprow == m_SelectionEnd->rowid )
					{
						xpos2 = m_SelectionEnd->xpos;
					}
				}
				else                    //ColumnMode
				{
					if( toprow >= m_SelectionBegin->rowid && toprow <= m_SelectionEnd->rowid )
					{
						xpos1 = m_SelLeftXPos;
						xpos2 = m_SelRightXPos;
					}
				}
			}

			do                        // every word of row
			{
				lastwordwidth = wordlength;
				wordlength = m_Syntax->NextWord( wordwidth );

				if( wordlength )
				{
					if( left < maxright && left + wordwidth > minleft )
					{
						if( m_WordBuffer[0] == 0x20 || m_WordBuffer[0] == 0x09 )
						{
							if( m_Syntax->nw_BgColor != current_bgcolor )
							{
								current_bgcolor = m_Syntax->nw_BgColor;
								dc->SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_BgColor, 1, wxPENSTYLE_SOLID ) ) );
								dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_BgColor ) ) );
								dc->DrawRectangle( ( left > minleft ) ? left : minleft, row_top, rectright - left, m_RowHeight );
							}

							dc->SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_Color, 1, wxPENSTYLE_SOLID ) ) );
							int idx = 0;
							int x0 = left;

							do
							{
								if( x0 + m_WidthBuffer[idx] > minleft )
								{
									if( m_WordBuffer[idx] == 0x20 )
									{
										if( m_ShowSpaceChar )
										{
											dc->DrawLines( 4, m_Space_Points, x0, text_top );
										}
									}
									else            // 0x09 Tab
									{
										if( m_ShowTabChar )
										{
											const int t1 = m_TextFontHeight / 7 + 1;
											const int  y = text_top + m_TextFontHeight * 3 / 5 + 1;
											const int tx = ( t1 > ( m_WidthBuffer[idx] >> 1 ) ) ? ( m_WidthBuffer[idx] >> 1 ) : t1;
											const int x = x0 + 1;
											const int dx = m_WidthBuffer[idx] - 2;
											wxPoint pts[8] =
											{
												wxPoint( x, y - t1 * 2 / 3 ),
												wxPoint( x, y + t1 * 2 / 3 ),
												wxPoint( x, y ),
												wxPoint( x + dx, y ),
												wxPoint( x + dx - tx, y - t1 ),
												wxPoint( x + dx, y ),
												wxPoint( x + dx - tx, y + t1 ),
												wxPoint( x + dx, y )
											};
											dc->DrawLines( 8, pts );
										}
									}
								}

								x0 += m_WidthBuffer[idx];
							}
							while( ++idx < wordlength && x0 < maxright );
						}
						else
						{
							wxColour * fclr = 0;
							bool newBg = false;
							int pensize = 1;
							//static ucs4_t data[] = {117, 115, 101 ,114};//"user"
							int nowright = left;
							bool nothighlighted = true;
							if( !m_HighlightWords.empty() )
							{
								vector<ucs4_t> newword;
								newword.reserve(wordlength);
								for( size_t i = 0; i < (size_t)wordlength; ++i )
								{
									newword.push_back( m_WordBuffer[i] );
								}

								std::set< vector<ucs4_t> >::iterator it = m_HighlightWords.find(newword);
								if (it != m_HighlightWords.end())
								{
									//static wxColour hlBgColor( 0x11, 0x3D, 0x6F );
									MadAttributes* attr = GetSyntax()->GetAttributes(aeHighlightWord);
									wxASSERT(attr != 0);
									fclr = &attr->color;
									if(current_bgcolor != attr->bgcolor)
									{
										current_bgcolor = attr->bgcolor;
										newBg = true;
										const int *pw = m_WidthBuffer;
										for( int i = 0; i < wordlength && nowright < maxright; ++i )
										{
											int ucw = *pw++;
											nowright += ucw;
										}
									}
									nothighlighted = false;
								}
							}

							if(nothighlighted)
							{
								if( m_Syntax->nw_BgColor != current_bgcolor )
								{
									current_bgcolor = m_Syntax->nw_BgColor;
									newBg = true;
									fclr = &current_bgcolor;
									nowright = maxright;
								}
							}

							if(newBg)
							{
								wxASSERT(fclr != 0);
								dc->SetPen( *( wxThePenList->FindOrCreatePen( *fclr, pensize, wxPENSTYLE_SOLID ) ) );
								dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( current_bgcolor ) ) );
								dc->DrawRectangle( left, row_top, nowright - left, m_RowHeight );
							}

							dc->SetTextForeground( m_Syntax->nw_Color );
							dc->SetFont( *( m_Syntax->nw_Font ) );
							bool spellMark = false;
#ifndef PYMADEDIT_DLL

							if( !InPrinting() && ( m_SpellCheckerPtr ) )
							{
								wxString str;

								for( int i = 0; i < wordlength; ++i )
								{
									str << wxChar( m_WordBuffer[i] );
								}

								if( !m_SpellCheckerPtr->IsSpellingOk( str ) )
									spellMark = true;
							}

#endif
							PaintText( dc, left, text_top, m_WordBuffer, m_WidthBuffer, wordlength, minleft, maxright, spellMark );
						}
					}

					if( xpos1 > 0 || xpos2 > 0 )
					{
						int idx = 0;
						int *pw = m_WidthBuffer;

						do
						{
							if( xpos1 > ( *pw >> 1 ) )
							{
								SelLeft += *pw;
								xpos1 -= *pw;
							}
							else
								xpos1 = 0;

							if( xpos2 > ( *pw >> 1 ) )
							{
								SelRight += *pw;
								xpos2 -= *pw;
							}
							else
								xpos2 = 0;

							++pw;
						}
						while( ++idx < wordlength && ( xpos1 > 0 || xpos2 > 0 ) );
					}

					left += wordwidth;
				}

				// ignore the text outside the rect
				if( left > maxright )
				{
					if( subrowid + 1 == int( lineiter->RowCount() ) )
					{
						m_Syntax->nw_EndOfLine = true;
						break;
					}

					/***/
					if( m_Syntax->m_SynAttr->m_CheckState == false )
					{
						m_Syntax->InitNextWord2( lineiter, subrowid + 1 );
						break;
					}

					/***/
				}

				if( ( lastwordwidth == wordlength ) && ( wordlength == 0 ) )
				{
					m_Syntax->nw_EndOfLine = true;
					break;
				}
			}
			while( m_Syntax->nw_LineWidth != 0 );

			// show end of line
			if( m_Syntax->nw_EndOfLine && ( ( left + wspace > minleft ) && ( left < maxright ) ) && m_ShowEndOfLine )
			{
				m_Syntax->SetAttributes( aeSpace );

				// clear background
				if( m_Syntax->nw_BgColor != current_bgcolor )
				{
					current_bgcolor = m_Syntax->nw_BgColor;
					dc->SetPen( *(wxThePenList->FindOrCreatePen( m_Syntax->nw_BgColor, 1, wxPENSTYLE_SOLID ) ) );
					dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_BgColor ) ) );
					dc->DrawRectangle( left, row_top, maxright - left, m_RowHeight );
				}

				dc->SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_Color, 1, wxPENSTYLE_SOLID ) ) );
				dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_Color ) ) );

				switch( m_Lines->GetNewLine( lineiter ) )
				{
				case 0:
					dc->DrawLines( 5, m_EOF_Points, left, text_top );
					break;

				case 0x0D:
					dc->DrawLines( m_CR_Points_Count, m_CR_Points, left, text_top );
					//dc->DrawPolygon(m_CR_Points_Count, m_CR_Points, left, text_top );
					break;

				case 0x0A:
					dc->DrawLines( m_LF_Points_Count, m_LF_Points, left, text_top );
					//dc->DrawPolygon(m_LF_Points_Count, m_LF_Points, left, text_top );
					break;

				case 0x0D+0x0A:
					dc->DrawLines( m_CRLF_Points_Count, m_CRLF_Points, left, text_top );
					//dc->DrawLines(m_CR_Points_Count, m_CRLF_Points, left, text_top);
					//dc->DrawLines(m_LF_Points_Count, m_CRLF_Points+m_CR_Points_Count, left, text_top);
					//dc->DrawPolygon(m_CRLF_Points_Count, m_CRLF_Points, left, text_top );
					break;
				}

				left += wspace;
			}

			if( left < maxright )     // paint range color at rest of row
			{
				wxColor &c = m_Syntax->GetAttributes(aeText)->bgcolor;

				if( c != current_bgcolor )
				{
					dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( c ) ) );
					dc->SetPen( *( wxThePenList->FindOrCreatePen( c, 1, wxPENSTYLE_SOLID ) ) );
					dc->DrawRectangle( left, row_top, rectright - left, m_RowHeight );
				}
			}

			if( bPaintSelection )
			{
				if( !m_ShowEndOfLine && m_Syntax->nw_EndOfLine )
					left += wspace;

				if( IsTextMode() )
				{
					if( toprow == m_SelectionBegin->rowid )
					{
						if( toprow == m_SelectionEnd->rowid ) // single row
						{
							INVERT_RECT( dc, SelLeft, text_top, SelRight - SelLeft, m_TextFontHeight );
						}
						else
						{
							INVERT_RECT( dc, SelLeft, text_top, left - SelLeft, m_TextFontHeight );
						}
					}
					else
						if( toprow == m_SelectionEnd->rowid )
						{
							INVERT_RECT( dc, leftpos, text_top, SelRight - leftpos, m_TextFontHeight );
						}
						else
							if( toprow > m_SelectionBegin->rowid
									&& toprow < m_SelectionEnd->rowid )
							{
								INVERT_RECT( dc, leftpos, text_top, left - leftpos, m_TextFontHeight );
							}
				}
				else                    // ColumnMode
				{
					if( toprow >= m_SelectionBegin->rowid && toprow <= m_SelectionEnd->rowid )
					{
						if( xpos1 > 0 )
						{
							int spaces = xpos1 / wspace;
							xpos1 -= spaces * wspace;

							if( xpos1 > ( wspace >> 1 ) )
							{
								++spaces;
							}

							SelLeft += spaces * wspace;
						}

						if( xpos2 > 0 )
						{
							int spaces = xpos2 / wspace;
							xpos2 -= spaces * wspace;

							if( xpos2 > ( wspace >> 1 ) )
							{
								++spaces;
							}

							SelRight += spaces * wspace;
						}

						if( SelRight == SelLeft && toprow != m_CaretPos.rowid )
						{
							SelRight += 2;
						}

						INVERT_RECT( dc, SelLeft, text_top, SelRight - SelLeft, m_TextFontHeight );
					}
				}
			}

			if( !m_SingleLineMode )
			{
				int l = rect.GetLeft();

				if( m_DisplayLineNumber  || (displaylinenumber && m_DisplayBookmark ) )
				{
					// paint bg
					m_Syntax->SetAttributes( aeLineNumber );

					int lwidth = m_LineNumberAreaWidth + m_BookmarkWidth;
					//int offset = 0;

					if( m_DrawingXPos && m_Syntax->nw_BgColor != bgcolor )
					{
						dc->SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_BgColor, 1, wxPENSTYLE_SOLID ) ) );
						dc->SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_BgColor ) ) );
						dc->DrawRectangle( l, row_top, lwidth, m_RowHeight );
					}
#if 0
					if(displaylinenumber)
					{
						lnums = wxT( '%' );
						lnums += wxString::Format( wxT( "%u" ), ncount );
						lnums += wxT( 'd' );
						lnums = wxString::Format( lnums, lineid );
						if (m_DisplayBookmark)
						{
							const wxChar *wcstr = lnums.c_str();
							unsigned int i = 0;
							for (; i < ncount; ++i)
							{
								if (wcstr[i] != 0x20) break;
							}
							offset = lwidth;
							lwidth = (ncount - i) * m_TextFontMaxDigitWidth;
							offset -= lwidth;
						}
					}
#endif

					if( displaylinenumber )
					{
						if( m_DisplayBookmark )
						{
							// add: gogo, 27.09.2009
							if( m_Lines->m_LineList.IsBookmarked( lineiter ) )
							{
								int b = m_RowHeight < 24 ? 1 : m_RowHeight / 24;
								
								MadAttributes* attr = GetSyntax()->GetAttributes(aeBookmark);
								wxASSERT(attr != 0);

								dc->SetPen(*(wxThePenList->FindOrCreatePen(attr->color, b, wxPENSTYLE_SOLID)));
								dc->SetBrush(*(wxTheBrushList->FindOrCreateBrush(attr->bgcolor)));
								
								wxRect rdrect(l /*+ offset*/, row_top + b, lwidth - b, m_RowHeight - b * 3 / 2);
								double r = m_RowHeight < 18 ? 3.0 : m_RowHeight / 6.0;
								dc->DrawRoundedRectangle(rdrect, r);
							}
						}

						lnums = wxT('%');
						lnums += wxString::Format(wxT("%u"), ncount);
						lnums += wxT('d');
						lnums = wxString::Format(lnums, lineid);
						const wxChar *wcstr = lnums.c_str();
						dc->SetTextForeground( m_Syntax->nw_Color );
						dc->SetFont( *( m_Syntax->nw_Font ) );

						if( !InPrinting() && reverseLineNum )
						{
							for( unsigned int i = 0; i < ncount; ++i, l += m_TextFontMaxDigitWidth )
							{
								if( wcstr[i] != 0x20 ) break;
							}

							for(unsigned int i = ncount; i > 0; --i, l += m_TextFontMaxDigitWidth )
							{
								if( wcstr[i - 1] != 0x20 )
								{
									dc->DrawText( wcstr[i - 1], l, text_top );
								}
								else
									break;
							}
						}
						else
						{
							for(unsigned int i = 0; i < ncount; ++i, l += m_TextFontMaxDigitWidth )
							{
								if( wcstr[i] != 0x20 )
								{
									dc->DrawText( wcstr[i], l, text_top );
								}
							}
						}
					}
				}
			}

			++toprow;
			++subrowid;
			displaylinenumber = false;
			text_top += m_RowHeight;
			row_top += m_RowHeight;
		}
		while( --rowcount > 0 && !m_Syntax->nw_EndOfLine );

		if( rowcount == 0 )
		{
			break;
		}

		++lineiter;
		if(lineiter == m_Lines->m_LineList.end()) --lineiter;
		m_Syntax->InitNextWord2( lineiter, 0 );
		subrowid = 0;
		displaylinenumber = true;
		++lineid;
	}

	if( !m_SingleLineMode )
	{
		int x1 = rect.GetLeft(), y = rect.GetTop();

		if( m_DisplayLineNumber || m_DisplayBookmark )
		{
			dc->SetPen( *( wxThePenList->FindOrCreatePen( *wxBLACK, 1, wxPENSTYLE_SOLID ) ) );

			// draw a line between LineNumberArea and Text
			if( m_DisplayLineNumber )
				x1 += m_LineNumberAreaWidth;
			else if( m_DisplayBookmark )
				x1 += m_BookmarkWidth;

			dc->DrawLine( x1, y, x1, y + rect.GetHeight() );
		}

		if( !InPrinting() && m_Display80ColHint && m_FixedWidthMode )
		{
			extern long g_HintColumns;
			int width = g_HintColumns * m_TextFontAveCharWidth;
			x1 += width + m_LeftMarginWidth - m_DrawingXPos;

			if( x1 > minleft )
			{
				if( *wxLIGHT_GREY != bgcolor )
				{
					dc->SetPen( *( wxThePenList->FindOrCreatePen( *wxLIGHT_GREY, 1, wxPENSTYLE_SOLID ) ) );
				}
				else
				{
					dc->SetPen( *( wxThePenList->FindOrCreatePen( wxColour( 128, 128, 128 ), 1, wxPENSTYLE_SOLID ) ) );
				}

				dc->DrawLine( x1, y, x1, y + rect.GetHeight() );
			}
		}
	}
}

void MadEdit::PaintHexDigit( wxDC *dc, int x, int y, const ucs4_t *hexdigit, const int *width, int count )
{
	while( count > 0 )
	{
		if( *hexdigit != 0x20 )
		{
			int xsrc = *hexdigit - '0';

			if( xsrc >= 10 )
			{
				if( xsrc > 22 ) xsrc = 16;
				else xsrc -= 7;
			}

			dc->Blit( x, y, m_HexFontMaxDigitWidth, m_RowHeight, m_HexDigitDC, xsrc * m_HexFontMaxDigitWidth, 0 );
		}

		x += *width;
		++hexdigit;
		++width;
		--count;
	}
}

void MadEdit::PaintHexOffset( wxDC *dc, int x, int y, const ucs4_t *hexdigit, const int *width, int count )
{
	while( count > 0 )
	{
		int xsrc = *hexdigit - '0';

		if( xsrc >= 10 )
		{
			if( xsrc == 10 ) xsrc = 17;
			else         xsrc -= 7;
		}

		dc->Blit( x, y, m_HexFontMaxDigitWidth, m_RowHeight, m_HexDigitDC, xsrc * m_HexFontMaxDigitWidth, m_RowHeight );
		x += *width;
		++hexdigit;
		++width;
		--count;
	}
}

void MadEdit::PaintHexLines( wxDC *dc, wxRect &rect, int toprow, int rowcount, bool painthead )
{
	int left = rect.x;
	int top = rect.y;

	for( int i = 0; i < 76; ++i )
		m_WidthBuffer[i] = m_HexFontMaxDigitWidth;

	m_Syntax->SetAttributes( aeText );
	dc->SetTextForeground( m_Syntax->nw_Color );
	dc->SetFont( *m_HexFont );
	wxColor &markcolor = m_Syntax->GetAttributes( aeActiveLine )->color;
	dc->SetPen( *( wxThePenList->FindOrCreatePen( markcolor, 1, wxPENSTYLE_SOLID ) ) );

	if( painthead )
	{
		dc->Blit( left, top, m_HexFontMaxDigitWidth * 76, m_RowHeight, m_HexDigitDC, 0, m_RowHeight * 2 );
		top += m_RowHeight;
	}

	if( rowcount <= 0 )
		return;

	wxASSERT( toprow >= m_TopRow );
	wxFileOffset pos = m_HexRowIndex[toprow - m_TopRow];
	MadLineIterator lit2;
	wxFileOffset linepos2 = pos;
	int rowno_notused = -1;
	GetLineByPos( lit2, linepos2, rowno_notused );
	linepos2 = pos - linepos2;
	MadLineIterator lit1;
	wxFileOffset linepos1 = -1;
	wxFileOffset hexrowpos = toprow;
	hexrowpos <<= 4;

	if( hexrowpos == pos )
	{
		lit1 = lit2;
		linepos1 = linepos2;
	}
	else
	{
		linepos1 = hexrowpos;
		GetLineByPos( lit1, linepos1, rowno_notused );
		linepos1 = hexrowpos - linepos1;
	}

	MadLineIterator lineend = m_Lines->m_LineList.end();
	m_ActiveRowUChars.clear();
	bool bMarkChar = false;
	int MarkCharLeft = -1, MarkCharRight = -1;
	wxFileOffset MarkPos1 = -1, MarkPos2 = -1;
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
	int SelLeft = 0, SelRight = 0;
	wxFileOffset SelPos1 = -1, SelPos2 = -1;

	if( m_Selection )
	{
		SelPos1 = m_SelectionBegin->pos;
		SelPos2 = m_SelectionEnd->pos;
		SelLeft = SelRight = -1;
	}

	const int minleft = rect.GetLeft() + m_DrawingXPos;
	const int maxright = rect.GetRight() + m_DrawingXPos;

	do                            // for every Hex Row
	{
		left = rect.x;
		// paint offset
		size_t hex = hexrowpos;

		for( int i = 7; i >= 0; --i )
		{
			m_WordBuffer[i] = ToHex( int( hex & 0x0F ) );
			hex >>= 4;
		}

		m_WordBuffer[8] = wxT( ':' );
		PaintHexOffset( dc, left, top, m_WordBuffer, m_WidthBuffer, 9 );
		left += m_HexFontMaxDigitWidth * 10;
		// paint binary data
		int idx = 0;

		for( int i = 0; i < 16; ++i )
		{
			if( linepos1 == lit1->m_Size )     // to next line
			{
				if( ( ++lit1 ) == lineend || lit1->m_Size == 0 )
				{
					break;
				}

				linepos1 = 0;
			}

			hex = lit1->Get( linepos1++ );
			m_WordBuffer[idx++] = ToHex( int( hex >> 4 ) );
			m_WordBuffer[idx++] = ToHex( int( hex & 0x0F ) );
			m_WordBuffer[idx++] = 0x20;
		}

		if( idx < 16 * 3 )
			do
			{
				m_WordBuffer[idx++] = 0x20;
			}
			while( idx < 16 * 3 );

		m_WordBuffer[idx] = wxT( '|' );
		PaintHexDigit( dc, left, top, m_WordBuffer, m_WidthBuffer, 16 * 3 + 1 );
		left += m_HexFontMaxDigitWidth * ( 16 * 3 + 2 );
		// paint text data
		idx = 0;
		int *pw = m_WidthBuffer + 60;

		if( hexrowpos < pos )       // show spaces
		{
			int i = pos - hexrowpos;

			do
			{
				m_WordBuffer[idx] = 0x20;
				++idx;
				*pw++ = m_HexFontMaxDigitWidth;
			}
			while( --i > 0 );
		}

		wxFileOffset oldpos = -1;
		wxFileOffset oldhexrowpos = hexrowpos;
		hexrowpos += 16;

		if( linepos2 < lit2->m_Size )
			m_Lines->InitNextUChar( lit2, linepos2 );

		do
		{
			if( linepos2 == lit2->m_Size )
			{
				if( ( ++lit2 ) == lineend || lit2->m_Size == 0 )
				{
					break;
				}

				linepos2 = 0;
				m_Lines->InitNextUChar( lit2, 0 );
			}

			if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) == false )
				break;

			MadUCPair & ucp = m_ActiveRowUChars.back();

			if( ucp.first == 0xFEFF || ucp.first == 0x9 || ucp.first == 0
#ifdef __WXGTK__
					|| ucp.first == 0x0A || ucp.first == 0x0D
#endif
			  )
				m_WordBuffer[idx++] = ' ';
			else
				m_WordBuffer[idx++] = ucp.first;

			*pw++ = m_HexFontMaxDigitWidth * ucp.second;
			linepos2 += ucp.second;
			oldpos = pos;
			pos += ucp.second;

			if( m_MarkActiveLine && oldpos <= m_CaretPos.pos && pos > m_CaretPos.pos )
			{
				bMarkChar = true;
				MarkCharLeft = left - 1;

				for( int i = 0; i < idx - 1; ++i )
				{
					MarkCharLeft += m_WidthBuffer[60 + i];
				}

				MarkCharRight = MarkCharLeft + GetHexUCharWidth( m_WordBuffer[idx - 1] ) + 1;
				MarkPos1 = oldpos;
				MarkPos2 = pos - 1;
			}

			if( SelLeft < 0 && SelPos1 < hexrowpos && pos > SelPos1 )
			{
				if( oldpos <= SelPos1 )
				{
					if( oldpos == SelPos1 )
					{
						SelLeft = ( ( int ( oldpos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
					}
					else
						if( pos < hexrowpos )
						{
							SelLeft = ( ( int ( pos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
						}
						else//oldpos < SelPos1
						{
							// partial selected
							SelLeft = ( ( int ( oldpos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
						}
				}
				else
					if( SelPos1 >= oldhexrowpos )
					{
						SelLeft = ( ( int ( oldpos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
					}
			}

			if( SelRight < 0 && oldpos <= SelPos2 && pos > SelPos2 )
			{
				/***
				if(pos == SelPos2)
				{
				    SelRight = ((int (pos) & 0xF)+60) * m_HexFontMaxDigitWidth;
				}
				else ***/
				{
					SelRight = ( ( int ( oldpos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
				}
			}
		}
		while( pos < hexrowpos );

		if( SelRight < 0 && SelLeft <= 0 )
		{
			if( SelPos2 == hexrowpos )
			{
				SelRight = ( 60 + int ( oldpos & 0x0F ) ) * m_HexFontMaxDigitWidth +
						   GetHexUCharWidth( m_WordBuffer[idx - 1] );
			}
			else
				if( SelPos2 < hexrowpos && pos == SelPos2 )
				{
					SelRight = ( ( int ( pos ) & 0xF ) + 60 ) * m_HexFontMaxDigitWidth;
				}
		}

		PaintText( dc, left, top, m_WordBuffer, m_WidthBuffer + 60, idx, minleft, maxright );

		// paint selection
		if( m_Selection )
		{
			// paint rect in HexArea
			if( SelPos1 >= oldhexrowpos )
			{
				if( SelPos2 < hexrowpos )
				{
					int x1 = ( 10 + ( int ( SelPos1 ) & 0xF ) * 3 ) * m_HexFontMaxDigitWidth + rect.x;
					int x2 = ( 9 + ( int ( SelPos2 ) & 0xF ) * 3 ) * m_HexFontMaxDigitWidth + rect.x;
					INVERT_RECT( dc, x1, top, x2 - x1, m_RowHeight );
				}
				else
					if( SelPos1 < hexrowpos && hexrowpos <= SelPos2 )
					{
						int x1 = ( 10 + ( int ( SelPos1 ) & 0xF ) * 3 ) * m_HexFontMaxDigitWidth + rect.x;
						INVERT_RECT( dc, x1, top, ( 57 * m_HexFontMaxDigitWidth + rect.x ) - x1, m_RowHeight );
					}
			}
			else                      //oldhexrowpos> SelPos1
			{
				if( hexrowpos <= SelPos2 )
				{
					INVERT_RECT( dc, 10 * m_HexFontMaxDigitWidth + rect.x, top, 47 * m_HexFontMaxDigitWidth, m_RowHeight );
				}
				else
					if( SelPos2 > oldhexrowpos )
					{
						int x1 = 10 * m_HexFontMaxDigitWidth + rect.x;
						int x2 = ( 9 + ( int ( SelPos2 ) & 0xF ) * 3 ) * m_HexFontMaxDigitWidth + rect.x;
						INVERT_RECT( dc, x1, top, x2 - x1, m_RowHeight );
					}
			}

			// paint rect in TextArea
			if( SelLeft > 0 )
			{
				if( SelRight > 0 )
				{
					if( SelLeft < SelRight )
					{
						INVERT_RECT( dc, SelLeft + rect.x, top, SelRight - SelLeft, m_RowHeight );
						SelLeft = 0;
						SelRight = 0;
					}
				}
				else
				{
					int x1 = SelLeft + rect.x;
					int x2 = ( 60 + int ( oldpos & 0x0F ) ) * m_HexFontMaxDigitWidth +
							 GetHexUCharWidth( m_WordBuffer[idx - 1] ) + rect.x;
					INVERT_RECT( dc, x1, top, x2 - x1, m_RowHeight );
					SelLeft = 0;
				}
			}
			else
				if( SelRight > 0 )
				{
					int x1 = 60 * m_HexFontMaxDigitWidth + rect.x;
					INVERT_RECT( dc, x1, top, SelRight + rect.x - x1, m_RowHeight );
					SelRight = 0;
				}
				else
					if( oldhexrowpos > SelPos1 && hexrowpos - 1 < SelPos2 )      // in middle of selection
					{
						int x1 = 60 * m_HexFontMaxDigitWidth + rect.x;
						int x2 = ( 60 + int ( oldpos & 0x0F ) ) * m_HexFontMaxDigitWidth +
								 GetHexUCharWidth( m_WordBuffer[idx - 1] ) + rect.x;
						INVERT_RECT( dc, x1, top, x2 - x1, m_RowHeight );
					}
		}

		// mark hex data
		if( oldhexrowpos <= MarkPos2 )
		{
			int h = m_RowHeight - 1;

			if( oldhexrowpos <= MarkPos1 )
			{
				if( MarkPos2 < hexrowpos )
				{
					int x1 = ( ( ( int )MarkPos1 & 0xF ) * 3 + 10 ) * m_HexFontMaxDigitWidth + rect.x;
					int x2 = x1 + ( ( ( int )( MarkPos2 - MarkPos1 ) ) * 3 + 2 ) * m_HexFontMaxDigitWidth;
					--x1;
					dc->DrawLine( x1, top, x2, top );
					dc->DrawLine( x2, top, x2, top + h );
					dc->DrawLine( x2, top + h, x1, top + h );
					dc->DrawLine( x1, top + h, x1, top );
					MarkPos2 = -1;
				}
				else
				{
					int x1 = ( ( int )MarkPos1 & 0xF );
					int x2 = ( ( 15 - x1 ) * 3 + 2 ) * m_HexFontMaxDigitWidth;
					x1 = ( x1 * 3 + 10 ) * m_HexFontMaxDigitWidth + rect.x;
					x2 += x1;
					--x1;
					dc->DrawLine( x2, top, x1, top );
					dc->DrawLine( x1, top, x1, top + h );
					dc->DrawLine( x1, top + h, x2, top + h );
				}
			}
			else
			{
				int x1 = 10 * m_HexFontMaxDigitWidth + rect.x;
				int x2 = x1 + ( ( ( int )( MarkPos2 ) & 0xF ) * 3 + 2 ) * m_HexFontMaxDigitWidth;
				--x1;
				dc->DrawLine( x1, top, x2, top );
				dc->DrawLine( x2, top, x2, top + h );
				dc->DrawLine( x2, top + h, x1, top + h );
				MarkPos2 = -1;          // reset mark
			}
		}

		// mark char at text area
		if( bMarkChar )
		{
			int h = m_RowHeight - 1;
			dc->DrawLine( MarkCharLeft, top, MarkCharRight, top );
			dc->DrawLine( MarkCharRight, top, MarkCharRight, top + h );
			dc->DrawLine( MarkCharRight, top + h, MarkCharLeft, top + h );
			dc->DrawLine( MarkCharLeft, top + h, MarkCharLeft, top );
			bMarkChar = false;
		}

		top += m_RowHeight;
		++toprow;
	}
	while( --rowcount > 0 );
}

int MadEdit::GetVisibleHexRowCount()
{
	int count = ( ( m_Lines->m_Size ) >> 4 ) + 1;
	count -= m_TopRow;
	int vrows = m_VisibleRowCount - 1;

	if( count > vrows )
		return vrows;

	return count;
}

// this will also validate the caretpos if in TextArea
void MadEdit::PrepareHexRowIndex( int toprow, int count )
{
	m_HexTopRow = toprow;

	if( count == 0 )              // at least one row
		++count;

	m_HexRowCount = count;
	wxFileOffset hexrowpos = toprow;
	hexrowpos <<= 4;
	wxASSERT( hexrowpos <= m_Lines->m_Size );
	MadLineIterator lit;
	int rowid;
	m_UpdateValidPos = -1;
	GetLineByPos( lit, hexrowpos, rowid );
	m_UpdateValidPos = 0;

	if( m_HexRowIndex.size() < size_t( count ) )
	{
		m_HexRowIndex.resize(size_t(count));
	}

	hexrowpos = toprow;
	hexrowpos <<= 4;
	int idx = 0;

	if( m_Encoding->GetType() == etSingleByte )
	{
		while( idx < count )
		{
			m_HexRowIndex[idx++] = hexrowpos;
			hexrowpos += 16;
		}

		if( !m_CaretAtHexArea )
		{
			m_TextAreaXPos = ( ( m_CaretPos.pos & 0x0F ) + 60 ) * m_HexFontMaxDigitWidth;
		}
	}
	else
	{
		m_TextAreaXPos = 0;

		if( !m_CaretAtHexArea )
		{
			--m_TextAreaXPos;      // set flag = -1
		}

		MadLineIterator lineend = m_Lines->m_LineList.end();
		wxFileOffset linepos = hexrowpos - m_ValidPos_pos;      //+ FDecodingOffset;
		wxASSERT( linepos <= lit->m_Size );
		m_Lines->InitNextUChar( lit, linepos );
		int uclen;
		m_ActiveRowUChars.clear();
		wxFileOffset nextpos = hexrowpos;
		MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

		do
		{
			m_HexRowIndex[idx] = hexrowpos;
			nextpos += 16;

			do
			{
				//uclen = 0;
				if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) )
				{
					uclen = m_ActiveRowUChars.back().second;
				}
				else
				{
					// to next line
					if( ++lit == lineend || lit->m_Size == 0 )
						break;

					m_Lines->InitNextUChar( lit, 0 );

					if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) )
					{
						uclen = m_ActiveRowUChars.back().second;
					}
					else
					{
						break;
					}
				}

				wxFileOffset oldpos = hexrowpos;
				hexrowpos += uclen;

				if( m_TextAreaXPos < 0 && oldpos <= m_CaretPos.pos
						&& hexrowpos > m_CaretPos.pos )
				{
					if( m_CaretPos.pos != oldpos )
					{
						m_CaretPos.linepos -= ( m_CaretPos.pos - oldpos );
						m_CaretPos.pos = oldpos;
						DoSelectionChanged();
					}

					m_TextAreaXPos = ( ( m_CaretPos.pos & 0x0F ) + 60 ) * m_HexFontMaxDigitWidth;
				}
			}
			while( hexrowpos < nextpos );
		}
		while( ++idx < count );

		if( m_TextAreaXPos < 0 ) // at end of file
		{
			m_TextAreaXPos = ( ( m_CaretPos.pos & 0x0F ) + 60 ) * m_HexFontMaxDigitWidth;
		}
	}
}

void MadEdit::UpdateTextAreaXPos()
{
	int crow = m_CaretPos.pos >> 4;
	wxASSERT( crow >= m_TopRow && crow < m_TopRow + m_HexRowCount );
	crow -= m_TopRow;
	wxFileOffset pos = m_HexRowIndex[crow];

	if( pos > m_CaretPos.pos )
	{
		wxASSERT( crow > 0 );
		pos = m_HexRowIndex[crow - 1];
	}

	wxFileOffset pos2 = pos;
	MadLineIterator lit;
	int rowid_notused = -1;
	GetLineByPos( lit, pos2, rowid_notused );
	pos2 = pos - pos2;
	m_Lines->InitNextUChar( lit, pos2 );
	MadUCQueue ucarray;
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

	for( ;; )
	{
		if( !( m_Lines->*NextUChar )( ucarray ) )
		{
			if( ++lit != m_Lines->m_LineList.end() && lit->m_Size != 0 )
			{
				m_Lines->InitNextUChar( lit, 0 );
				( m_Lines->*NextUChar )( ucarray );
			}
			else
				break;
		}

		pos2 = pos;
		pos += ucarray.back().second;

		if( pos2 <= m_CaretPos.pos && pos > m_CaretPos.pos )
		{
			if( m_CaretPos.pos != pos2 )
			{
				m_CaretPos.linepos -= ( m_CaretPos.pos - pos2 );
				m_CaretPos.pos = pos2;
				DoSelectionChanged();
			}

			break;
		}
	}

	m_TextAreaXPos = ( int( m_CaretPos.pos & 0x0F ) + 60 ) * m_HexFontMaxDigitWidth;
}

void MadEdit::ToggleHexTextArea()
{
	wxASSERT( m_EditMode == emHexMode );
	m_CaretAtHexArea = !m_CaretAtHexArea;
	m_CaretAtHalfByte = false;
	int crow = ( m_CaretPos.pos >> 4 );

	if( crow < m_TopRow )
	{
		m_RepaintAll = true;
		Refresh( false );
	}
	else
	{
		int rows = m_VisibleRowCount;

		if( rows > 1 )              // at least one row
			--rows;

		if( crow >= ( m_TopRow + rows ) )
		{
			m_RepaintAll = true;
			Refresh( false );
		}
		else
		{
			if( !m_CaretAtHexArea )
			{
				UpdateTextAreaXPos();
			}
		}
	}

	AppearCaret();
	UpdateScrollBarPos();
	m_LastTextAreaXPos = m_TextAreaXPos;
}

// it will change m_TopRow to make Caret shown in screen
void MadEdit::AppearHexRow( wxFileOffset pos )
{
	wxASSERT( m_EditMode == emHexMode );
	int crow = ( pos >> 4 );

	if( crow < m_TopRow )
	{
		if( pos == m_Lines->m_Size && ( ( ( int )pos ) & 0x0F ) == 0 && crow > 0 )
			m_TopRow = crow - 1;
		else
			m_TopRow = crow;

		m_RepaintAll = true;
		Refresh( false );
	}
	else
	{
		int rows = m_CompleteRowCount;

		if( rows > 0 )
			--rows;

		if( crow >= ( m_TopRow + rows ) )
		{
			if( rows == 0 )
			{
				if( m_TopRow != crow )
				{
					m_TopRow = crow;
					m_RepaintAll = true;
					Refresh( false );
				}
			}
			else
			{
				m_TopRow = crow - rows + 1;
				m_RepaintAll = true;
				Refresh( false );
			}
		}
	}

	if( m_RepaintAll )
	{
		int count = GetVisibleHexRowCount();

		if( count == 0 )
			++count;

		PrepareHexRowIndex( m_TopRow, count );
	}
}

void MadEdit::UpdateHexPosByXPos( int row, int xpos )
{
	wxASSERT( m_TopRow == m_HexTopRow );
	wxASSERT( row >= m_TopRow && row < m_TopRow + m_HexRowCount );
	wxFileOffset rowpos = wxFileOffset( row ) << 4;
	wxFileOffset pos = rowpos + 16;
	int hw = ( m_HexFontMaxDigitWidth >> 1 );

	if( m_CaretAtHexArea )
	{
		int minpos = 10;
		int maxpos = 55;
		int bytes;

		if( pos > m_Lines->m_Size )
		{
			bytes = pos - m_Lines->m_Size;
			--maxpos;

			if( bytes > 1 )
			{
				maxpos -= ( bytes - 1 ) * 3;
			}

			maxpos = maxpos * m_HexFontMaxDigitWidth;
		}
		else
		{
			bytes = 0;
			maxpos = maxpos * m_HexFontMaxDigitWidth + hw;
		}

		minpos = minpos * m_HexFontMaxDigitWidth;
		m_CaretAtHalfByte = false;

		if( xpos < minpos )
		{
			pos = rowpos;
		}
		else
			if( xpos >= maxpos )
			{
				if( bytes == 0 )
				{
					pos = rowpos + 15;
					m_CaretAtHalfByte = true;
				}
				else
				{
					pos = rowpos + ( 16 - bytes );
				}
			}
			else
			{
				int dw = m_HexFontMaxDigitWidth << 1;
				pos = rowpos;

				if( minpos + hw < maxpos )
					do
					{
						if( xpos < minpos + hw )
							break;

						m_CaretAtHalfByte = true;
						minpos += dw;

						if( xpos < minpos + hw )
							break;

						m_CaretAtHalfByte = false;
						++pos;
						minpos += m_HexFontMaxDigitWidth;
					}
					while( minpos + hw < maxpos );
			}
	}
	else                          //TextArea
	{
		row -= m_TopRow;
		wxFileOffset endpos = pos;

		if( endpos > m_Lines->m_Size )
			endpos = m_Lines->m_Size;

		wxFileOffset firstpos = m_HexRowIndex[row];
		MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

		if( firstpos < endpos )
		{
			int firstxpos = ( ( int ( firstpos ) & 0x0F ) + 60 ) * m_HexFontMaxDigitWidth;
			m_ActiveRowUChars.clear();
			MadLineIterator lit;
			int rn = -1;
			pos = firstpos;
			GetLineByPos( lit, pos, rn );
			pos = firstpos - pos;
			m_Lines->InitNextUChar( lit, pos );
			pos = firstpos;

			do
			{
				if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) )
				{
					int w = GetHexUCharWidth( m_ActiveRowUChars.back().first );

					if( w < m_HexFontMaxDigitWidth )
						w = m_HexFontMaxDigitWidth;

					if( xpos < firstxpos + w )
					{
						break;
					}

					firstpos += m_ActiveRowUChars.back().second;

					if( firstpos >= endpos )
					{
						if( endpos == m_Lines->m_Size && ( endpos & 0x0F ) != 0 )
						{
							pos = firstpos;
						}

						break;
					}

					pos = firstpos;
					firstxpos += m_ActiveRowUChars.back().second * m_HexFontMaxDigitWidth;
				}
				else
				{
					m_Lines->InitNextUChar( ++lit, 0 );
				}
			}
			while( firstpos < endpos );
		}
		else
		{
			pos = firstpos;
		}
	}

	// now pos is CaretPos, use pos to update m_CaretPos
	wxFileOffset linepos = pos;
	MadLineIterator lit;
	int rowid = -1;
	m_CaretPos.lineid = GetLineByPos( lit, linepos, rowid );
	m_CaretPos.linepos = pos - linepos;
	m_CaretPos.pos = pos;
	m_CaretPos.iter = lit;
	m_CaretPos.rowid = rowid;
	m_CaretPos.subrowid = 0;
	int cnt = int( lit->RowCount() - 1 );

	if( m_CaretPos.subrowid < cnt )
	{
		if( m_CaretPos.linepos >= lit->m_RowIndices[1].m_Start )
			do
			{
				++m_CaretPos.subrowid;
				++m_CaretPos.rowid;
			}
			while( m_CaretPos.subrowid < cnt
					&& m_CaretPos.linepos >= lit->m_RowIndices[m_CaretPos.subrowid + 1].m_Start );
	}
}



void MadEdit::RecountLineWidth( bool bForceRecount )
{
	if( m_LoadingFile )
		return;

	if( bForceRecount == false )
	{
		if( m_WordWrapMode == wwmWrapByWindow  && ( m_Lines->m_RowCount != m_Lines->m_LineCount
				|| m_Lines->m_MaxLineWidth > GetMaxWordWrapWidth() ) )
		{
			bForceRecount = true;
		}
	}

	if( bForceRecount )
	{
		m_Lines->RecountLineWidth();
		// update ValidPos
		m_DrawingXPos = 0;

		if( m_TopRow < 0 ) m_TopRow = 0;
		else
			if( m_TopRow >= int( m_Lines->m_RowCount ) ) m_TopRow = int( m_Lines->m_RowCount - 1 );

		m_ValidPos_iter = m_Lines->m_LineList.begin();
		m_ValidPos_lineid = 0;
		m_ValidPos_rowid = 0;
		m_ValidPos_pos = 0;
		m_UpdateValidPos = -1;
		MadLineIterator lit;
		wxFileOffset pos = -1;
		int toprow = m_TopRow;
		GetLineByRow( lit, pos, toprow );
		m_UpdateValidPos = 0;
		// update caretpos
		UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		m_LastCaretXPos = m_CaretPos.xpos;

		// update selection pos
		if( m_Selection )
		{
			UpdateSelectionPos();
		}

		m_RepaintAll = true;
		DoSelectionChanged();
		FindBracePairUnderCaretPos();
	}
}

void MadEdit::ReformatAll()
{
	m_Lines->m_MaxLineWidth = 0;
	MadLineIterator first = m_Lines->m_LineList.begin();
	MadLineIterator last = m_Lines->m_LineList.end();
	--last;
	m_Lines->Reformat( first, last );
	// reset validpos
	m_ValidPos_iter = m_Lines->m_LineList.begin();
	m_ValidPos_lineid = 0;
	m_ValidPos_rowid = 0;
	m_ValidPos_pos = 0;
	m_UpdateValidPos = -1;
	UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
	m_UpdateValidPos = 0;

	if( !InPrinting() )
	{
		AppearCaret();
		UpdateScrollBarPos();
	}

	if( m_EditMode == emHexMode )
	{
		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
			m_LastTextAreaXPos = m_TextAreaXPos;
		}
	}
	else
	{
		if( m_Selection )
		{
			UpdateSelectionPos();
		}
	}
}

//==================================================

void MadEdit::UpdateAppearance()
{
	if( m_EditMode != emHexMode )
	{
		m_RowHeight = ( m_LineSpacing * m_TextFontHeight ) / 100;

		if( m_DisplayBookmark && m_LineNumberAreaWidth == 0 )
			m_BookmarkWidth = m_RowHeight/2;
		else
			m_BookmarkWidth = 0;
	}
	else
	{
		m_RowHeight = m_HexFontHeight;
	}

	if( m_RowHeight == 0 ) m_RowHeight = 9;

	m_CompleteRowCount = m_ClientHeight / m_RowHeight;

	if( m_CompleteRowCount == 0 )  ++m_CompleteRowCount;

	m_VisibleRowCount = m_CompleteRowCount;

	if( m_VisibleRowCount * m_RowHeight < m_ClientHeight ) ++m_VisibleRowCount;

	m_PageRowCount = m_CompleteRowCount;

	if( m_PageRowCount > 1 )
	{
		--m_PageRowCount;

		if( m_EditMode == emHexMode && m_PageRowCount > 1 ) --m_PageRowCount;
	}
}

void MadEdit::UpdateScrollBarPos()
{
	if( m_SingleLineMode )
	{
		m_TopRow = 0;

		if( m_DrawingXPos < 0 ) m_DrawingXPos = 0;

		return;
	}

	if( m_EditMode != emHexMode ) // Text/Column Mode
	{
		m_MaxColumnRowWidth = m_Lines->m_MaxLineWidth + m_RightMarginWidth;
		int xmax = GetLineNumberAreaWidth( int( m_Lines->m_LineCount ) ) + m_LeftMarginWidth
				   + m_MaxColumnRowWidth + m_BookmarkWidth;

		if( m_WordWrapMode == wwmWrapByWindow )
		{
			xmax -= m_RightMarginWidth;
		}

		int xpos = xmax - m_ClientWidth;

		if( m_DrawingXPos < 0 || xpos <= 0 ) m_DrawingXPos = 0;
		else
			if( xpos > 0 && m_DrawingXPos >= xpos ) m_DrawingXPos = xpos - 1;

		if( xmax > m_ClientWidth )
		{
			m_HScrollBar->Enable();
			m_HScrollBar->SetScrollbar( m_DrawingXPos, m_ClientWidth, xmax, m_ClientWidth, true );
		}
		else
		{
#ifndef __WXMSW__
			m_HScrollBar->SetScrollbar( 0, 1, 1, 1, true );
#endif
			m_HScrollBar->Enable( false );
		}

		if( m_TopRow < 0 ) m_TopRow = 0;
		else
			if( m_TopRow >= int( m_Lines->m_RowCount ) ) m_TopRow = int( m_Lines->m_RowCount - 1 );

		if( m_Lines->m_RowCount > 1 )
		{
			m_VScrollBar->Enable();
			int ymax = int( m_Lines->m_RowCount ) + m_PageRowCount - 1;
			m_VScrollBar->SetScrollbar( m_TopRow, m_PageRowCount, ymax, m_PageRowCount, true );
		}
		else
		{
#ifndef __WXMSW__
			m_VScrollBar->SetScrollbar( 0, 1, 1, 1, true );
#endif
			m_VScrollBar->Enable( false );
		}

		if( m_EditMode == emColumnMode )
		{
			m_MaxColumnRowWidth -= m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
			int w = m_ClientWidth - ( m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth + m_RightMarginWidth );

			if( m_MaxColumnRowWidth < w )
			{
				m_MaxColumnRowWidth = w;
			}
		}
	}
	else                        // HexMode
	{
		int xmax = HexModeMaxColumns * m_HexFontMaxDigitWidth;

		if( xmax <= m_ClientWidth )
		{
			m_DrawingXPos = 0;
		}

		int xpos = xmax - m_ClientWidth;

		if( m_DrawingXPos < 0 || xpos <= 0 ) m_DrawingXPos = 0;
		else
			if( xpos > 0 && m_DrawingXPos >= xpos ) m_DrawingXPos = xpos - 1;

		int rowcount = int( m_Lines->m_Size >> 4 ) + 1;

		if( m_TopRow < 0 ) m_TopRow = 0;
		else
			if( m_TopRow >= rowcount ) m_TopRow = rowcount - 1;

		int ymax = rowcount + m_PageRowCount - 1;

		if( xmax > m_ClientWidth )
		{
			m_HScrollBar->Enable();
			m_HScrollBar->SetScrollbar( m_DrawingXPos, m_ClientWidth, xmax, m_ClientWidth, true );
		}
		else
		{
#ifndef __WXMSW__
			m_HScrollBar->SetScrollbar( 0, 1, 1, 1, true );
#endif
			m_HScrollBar->Enable( false );
		}

		if( rowcount > 1 )
		{
			m_VScrollBar->Enable( true );
			m_VScrollBar->SetScrollbar( m_TopRow, m_PageRowCount, ymax, m_PageRowCount, true );
		}
		else
		{
#ifndef __WXMSW__
			m_VScrollBar->SetScrollbar( 0, 1, 1, 1, true );
#endif
			m_VScrollBar->Enable( false );
		}
	}
}

void MadEdit::BeginUpdateSelection()
{
	if( !m_Selection )
	{
		m_Selection = true;
		m_SelectionPos1 = m_CaretPos;
	}
    else if((m_SelectionPos1.pos != m_CaretPos.pos) && (m_SelectionPos2.pos != m_CaretPos.pos)) // Caret has moved by goto
    {
        m_SelectionPos1 = m_CaretPos;
    }

	m_ZeroSelection = false;
	if( int( m_CaretPos.rowid ) < m_SelFirstRow )
		m_SelFirstRow = m_CaretPos.rowid;

	if( int( m_CaretPos.rowid ) > m_SelLastRow )
		m_SelLastRow = m_CaretPos.rowid;
}

void MadEdit::EndUpdateSelection( bool bSelection )
{
	m_SelectionPos2 = m_CaretPos;

	if( int( m_CaretPos.rowid ) < m_SelFirstRow )
		m_SelFirstRow = m_CaretPos.rowid;

	if( int( m_CaretPos.rowid ) > m_SelLastRow )
		m_SelLastRow = m_CaretPos.rowid;

	if( !bSelection ||
			( m_SelectionPos1.pos == m_SelectionPos2.pos &&
			  m_SelectionPos1.extraspaces == m_SelectionPos2.extraspaces ) )
	{
		m_Selection = false;
		m_RepaintSelection = true;
		Refresh( false );
	}
	else
	{
		if( m_SelectionPos1.pos < m_SelectionPos2.pos )
		{
			m_SelectionBegin = &m_SelectionPos1;
			m_SelectionEnd = &m_SelectionPos2;
		}
		else
		{
			m_SelectionBegin = &m_SelectionPos2;
			m_SelectionEnd = &m_SelectionPos1;
		}

		if( m_EditMode == emColumnMode )
		{
			if( m_SelectionPos1.xpos < m_SelectionPos2.xpos )
			{
				m_SelLeftXPos = m_SelectionPos1.xpos;
				m_SelRightXPos = m_SelectionPos2.xpos;
			}
			else
			{
				m_SelLeftXPos = m_SelectionPos2.xpos;
				m_SelRightXPos = m_SelectionPos1.xpos;
			}
		}

		m_RepaintSelection = true;
		Refresh( false );
	}
	m_ZeroSelection = false;
}

void MadEdit::SetSelection( wxFileOffset beginpos, wxFileOffset endpos, bool bCaretAtBeginPos )
{
	wxFileOffset oldCaretPos = m_CaretPos.pos;

	if( bCaretAtBeginPos )
	{
		m_SelectionPos1.pos = endpos;
		m_SelectionPos2.pos = beginpos;
		m_SelectionBegin = &m_SelectionPos2;
		m_SelectionEnd = &m_SelectionPos1;
	}
	else
	{
		m_SelectionPos1.pos = beginpos;
		m_SelectionPos2.pos = endpos;
		m_SelectionBegin = &m_SelectionPos1;
		m_SelectionEnd = &m_SelectionPos2;
	}

	if( IsTextFile() )
	{
		MadUCQueue ucqueue;
		vector<int> widthArray;
		int ucharPos;
		m_UpdateValidPos = -1;  // update always
		UpdateCaretByPos( m_SelectionPos1, ucqueue, widthArray, ucharPos );
		m_UpdateValidPos = 0;
		UpdateCaretByPos( m_SelectionPos2, ucqueue, widthArray, ucharPos );
	}
	else
	{
		m_SelectionPos1.linepos = m_SelectionPos1.pos;
		m_SelectionPos2.linepos = m_SelectionPos2.pos;
	}

	if( m_Selection || m_EditMode == emHexMode )
		m_RepaintAll = true;

	Refresh( false );

	if( m_SelectionPos1.pos == m_SelectionPos2.pos )
	{
		m_Selection = false;
		m_ZeroSelection = true;
	}
	else
	{
		m_Selection = true;
		m_ZeroSelection = false;
		m_RepaintSelection = true;
		m_SelFirstRow = m_SelectionBegin->rowid;
		m_SelLastRow = m_SelectionEnd->rowid;

		if( m_EditMode == emColumnMode )
		{
			if( m_SelectionPos1.xpos < m_SelectionPos2.xpos )
			{
				m_SelLeftXPos = m_SelectionPos1.xpos;
				m_SelRightXPos = m_SelectionPos2.xpos;
			}
			else
			{
				m_SelLeftXPos = m_SelectionPos2.xpos;
				m_SelRightXPos = m_SelectionPos1.xpos;
			}
		}
	}

	if( IsTextFile() )
	{
		if( bCaretAtBeginPos )
			m_CaretPos = *m_SelectionBegin;
		else
			m_CaretPos = *m_SelectionEnd;

		UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
	}
	else
	{
		if( bCaretAtBeginPos )
		{
			m_CaretPos.pos = beginpos;
			m_CaretPos.linepos = beginpos;
		}
		else
		{
			m_CaretPos.pos = endpos;
			m_CaretPos.linepos = endpos;
		}
	}

	AppearCaret();
	UpdateScrollBarPos();
	m_LastCaretXPos = m_CaretPos.xpos;

	if( m_EditMode == emHexMode )
	{
		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
			m_LastTextAreaXPos = m_TextAreaXPos;
		}
	}

	DoSelectionChanged();

	if( m_RecordCaretMovements && oldCaretPos != m_CaretPos.pos )
	{
		m_CaretTracker->Add( oldCaretPos, m_CaretPos.pos );
	}
}

wxFileOffset MadEdit::GetColumnRange( wxString *ws, MadCaretPos *begpos, MadCaretPos *endpos )
{
	wxASSERT( m_EditMode == emColumnMode );
	wxFileOffset rangesize = 0;
	size_t firstrow = begpos->rowid;
	size_t subrowid = begpos->subrowid;
	MadLineIterator lit = begpos->iter;
	wxFileOffset pos = begpos->pos - begpos->linepos;
	size_t lastrow = endpos->rowid;
	MadUCQueue ucqueue;
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
	
	int leftxpos = -1;
	int rightxpos = -1;

	if( begpos->xpos < endpos->xpos )
	{
		leftxpos = begpos->xpos;
		rightxpos = endpos->xpos;
	}
	else
	{
		leftxpos = endpos->xpos;
		rightxpos = begpos->xpos;
	}

	for( ;; )
	{
		int rowwidth = lit->m_RowIndices[subrowid].m_Width;
		int nowxpos = 0;
		int xpos1 = leftxpos, xpos2 = rightxpos;

		if( leftxpos < rowwidth )
		{
			wxFileOffset rowpos = lit->m_RowIndices[subrowid].m_Start;
			wxFileOffset rowendpos = lit->m_RowIndices[subrowid + 1].m_Start;
			m_Lines->InitNextUChar( lit, rowpos );

			do
			{
				int uc = 0x0D;

				if( ( m_Lines->*NextUChar )( ucqueue ) )
					uc = ucqueue.back().first;

				if( uc == 0x0D || uc == 0x0A )  // EOL
				{
					break;
				}

				int ucwidth = GetUCharWidth( uc );

				if( uc == 0x09 )
				{
					int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
					ucwidth = rowwidth - nowxpos;
					tabwidth -= ( nowxpos % tabwidth );

					if( tabwidth < ucwidth )
						ucwidth = tabwidth;
				}

				nowxpos += ucwidth;
				int uchw = ucwidth >> 1;

				if( xpos1 > uchw )
				{
					rowpos += ucqueue.back().second;
					xpos1 -= ucwidth;
					xpos2 -= ucwidth;
				}
				else
				{
					xpos1 = 0;

					if( xpos2 > uchw )
					{
						rangesize += ucqueue.back().second;

						if( ws )
						{
#ifdef __WXMSW__

							if( uc < 0x10000 )
							{
								( *ws ) << wxChar( uc );
							}
							else
							{
								wchar_t wbuf[2];
								m_Encoding->UCS4toUTF16LE_U10000( uc, ( wxByte* )wbuf );
								( *ws ) << wbuf[0];
								( *ws ) << wbuf[1];
							}

#else
							( *ws ) << wxChar( uc );
#endif
						}

						xpos2 -= ucwidth;
					}
					else
						xpos2 = 0;
				}
			}
			while( xpos2 > 0 && rowpos < rowendpos );
		}

		if( ws )
		{
			// add newline
#ifdef __WXMSW__
			( *ws ) << wxChar( 0x0D );
#endif
			( *ws ) << wxChar( 0x0A );
		}

		if( firstrow == lastrow )
			break;

		++firstrow;
		ucqueue.clear();

		// to next row, line
		if( ++subrowid == lit->RowCount() )
		{
			pos += lit->m_Size;
			++lit;
			subrowid = 0;
		}
	}

	return rangesize;
}

wxFileOffset MadEdit::GetColumnSelection( wxString *ws )
{
	return GetColumnRange( ws, m_SelectionBegin, m_SelectionEnd );
}

void MadEdit::SelectWordFromCaretPos( wxString *ws, MadCaretPos * cpos/* = nullptr*/, bool bSelection/* = false*/ )
{
	MadCaretPos * caretPos = cpos;
	m_ZeroSelection = false;

	if( cpos == nullptr ) caretPos = &m_CaretPos;

	if( m_EditMode == emColumnMode && caretPos->extraspaces )
		return;

	wxFileOffset startpos = -1, endpos = -1;

	if( !IsTextFile() ) // not a text file (HexMode)
	{
		startpos = m_HexRowIndex[0];
		int count = GetVisibleHexRowCount();

		if( count == 0 ) ++count;

		endpos = m_HexRowIndex[count - 1] + 16;

		if( endpos > m_Lines->m_Size )
		{
			endpos = m_Lines->m_Size;
		}
	}
	else                          //TextMode
	{
		//may select whole line
		startpos = caretPos->pos - caretPos->linepos +
				   caretPos->iter->m_RowIndices[0].m_Start; // exclude BOM
		endpos = m_Lines->m_Size;
		//select wrapped-line only
		//startpos = caretPos->pos - caretPos->linepos +
		//         caretPos->iter->m_RowIndices[caretPos->subrowid].m_Start; // exclude BOM
		//endpos = caretPos->pos - caretPos->linepos +
		//       caretPos->iter->m_RowIndices[caretPos->subrowid+1].m_Start;
	}

	// now startpos is the begin of line
	// check the word between startpos and endpos
	MadLineIterator & lit = caretPos->iter;
	wxFileOffset pos = caretPos->linepos - ( caretPos->pos - startpos );
	m_Lines->InitNextUChar( lit, pos );
	MadUCQueue ucqueue;
	int type = 0, prevtype = 0;
	int idx = 0, posidx = 0;
	pos = startpos;
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

	if( ( m_Lines->*NextUChar )( ucqueue ) )
	{
		do
		{
			MadUCPair & ucp = ucqueue.back();
			int uc = ucp.first;

			if( type == 0 )
			{
				if( pos >= caretPos->pos )
				{
					type = GetUCharType( uc );
					posidx = idx;

					if( ( ws != nullptr || bSelection )
							&& ( ( type <= 3 && prevtype > 3 ) || ( type <= 2 && prevtype > 2 ) ) )
					{
						--posidx;
						type = prevtype;
						ucqueue.pop_back();
						break;
					}
				}
				else
					if( ws != nullptr || bSelection )
					{
						prevtype = GetUCharType( uc );
					}
			}
			else
			{
				if( GetUCharType( uc ) != type )
				{
					ucqueue.pop_back();
					break;
				}
			}

			++idx;
			pos += ucp.second;
		}
		while( pos < endpos && ( m_Lines->*NextUChar )( ucqueue ) );
	}

	idx = posidx - 1;

	while( idx >= 0 && GetUCharType( ucqueue[idx].first ) == type )
	{
		--idx;
	}

	if( idx >= 0 )
	{
		do
		{
			startpos += ucqueue.front().second;
			ucqueue.pop_front();
		}
		while( --idx >= 0 );
	}

	if( !ucqueue.empty() && type != 0 )
	{
		//wxASSERT(type != 0);
		if( ws != nullptr )            // get word
		{
			size_t s = ucqueue.size();

			for( size_t i = 0; i < s; ++i )
			{
#ifdef __WXMSW__
				ucs4_t &uc = ucqueue[i].first;

				if( uc < 0x10000 )
				{
					( *ws ) << wxChar( uc );
				}
				else
				{
					wchar_t wbuf[2];
					m_Encoding->UCS4toUTF16LE_U10000( uc, ( wxByte* )wbuf );
					( *ws ) << wbuf[0];
					( *ws ) << wbuf[1];
				}

#else
				( *ws ) << ucqueue[i].first;
#endif
			}

			return;
		}

		if( cpos ) return;

		if( m_MouseLeftDoubleClick && m_LeftBrace_rowid >= 0 )
		{
			MadLineIterator lit1;
			wxFileOffset pos1 = -1, pos2 = -1;
			GetLineByRow( lit1, pos1, m_LeftBrace_rowid );
			GetLineByRow( lit1, pos2, m_RightBrace_rowid );
			pos1 += m_LeftBrace.LinePos;
			pos2 += m_RightBrace.LinePos;

			if( startpos == pos1 )
			{
				pos = pos2 + m_RightBrace.Length/*for right brace*/;
			}
			else
			{
				if( startpos == pos2 )
				{
					startpos = pos1;
					pos = pos2 + m_RightBrace.Length/*for right brace*/;
				}
			}
		}

		m_Selection = true;
		m_SelectionPos1.pos = startpos;
		m_SelectionPos2.pos = pos;
		UpdateSelectionPos();
		*caretPos = m_SelectionPos2;
		m_SelectionBegin = &m_SelectionPos1;
		m_SelectionEnd = &m_SelectionPos2;
		m_SelFirstRow = m_SelectionBegin->rowid;
		m_SelLastRow = m_SelectionEnd->rowid;
		UpdateCaret( *caretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );

		if( m_EditMode == emHexMode )
		{
			AppearHexRow( caretPos->pos );
			m_CaretAtHalfByte = false;

			if( !m_CaretAtHexArea )
			{
				UpdateTextAreaXPos();
			}
		}

		AppearCaret();
		UpdateScrollBarPos();
		m_LastTextAreaXPos = m_TextAreaXPos;
		m_LastCaretXPos = caretPos->xpos;
		m_RepaintAll = true;
		Refresh( false );
		DoSelectionChanged();
	}
}

void MadEdit::GetWordFromCaretPos( wxString &ws )
{
	MadCaretPos * pos( nullptr );

	if( IsSelected() )
		pos = m_SelectionBegin;

	SelectWordFromCaretPos( &ws, pos, false );
}

void MadEdit::SelectLineFromCaretPos( wxString * WXUNUSED(ws), bool caretToBegOfSel )
{
	m_ZeroSelection = false;
	if( m_EditMode == emColumnMode && m_CaretPos.extraspaces )
		return;

	wxFileOffset startpos, endpos;

	if( !IsTextFile() ) // not a text file (HexMode)
	{
		startpos = m_HexRowIndex[0];
		int count = GetVisibleHexRowCount();

		if( count == 0 ) ++count;

		endpos = m_HexRowIndex[count - 1] + 16;

		if( endpos > m_Lines->m_Size )
		{
			endpos = m_Lines->m_Size;
		}
	}
	else                          //TextMode
	{
		//may select whole line
		startpos = m_CaretPos.pos - m_CaretPos.linepos +
				   m_CaretPos.iter->m_RowIndices[0].m_Start; // exclude BOM
		endpos = m_CaretPos.pos - m_CaretPos.linepos +
				 m_CaretPos.iter->m_Size;//m_RowIndices[1].m_Start;
	}

	m_Selection = true;
	m_SelectionPos1.pos = startpos;
	m_SelectionPos2.pos = endpos;
	UpdateSelectionPos();

	if( caretToBegOfSel )
		m_CaretPos = m_SelectionPos1;
	else
		m_CaretPos = m_SelectionPos2;

	m_SelectionBegin = &m_SelectionPos1;
	m_SelectionEnd = &m_SelectionPos2;
	m_SelFirstRow = m_SelectionBegin->rowid;
	m_SelLastRow = m_SelectionEnd->rowid;
	UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );

	if( m_EditMode == emHexMode )
	{
		AppearHexRow( m_CaretPos.pos );
		m_CaretAtHalfByte = false;

		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
		}
	}

	AppearCaret();
	UpdateScrollBarPos();
	m_LastTextAreaXPos = m_TextAreaXPos;
	m_LastCaretXPos = m_CaretPos.xpos;
	m_RepaintAll = true;
	Refresh( false );
	DoSelectionChanged();
}

bool MadEdit::PutTextToClipboard( const wxString &ws )
{
	if( wxTheClipboard->Open() )
	{
		bool ok = wxTheClipboard->SetData( new wxTextDataObject( ws ) );
		wxTheClipboard->Flush();
		wxTheClipboard->Close();
		return ok;
	}

	return false;
}

bool MadEdit::PutColumnDataToClipboard( const wxString &ws, int linecount )
{
	if( wxTheClipboard->Open() )
	{
		wxTheClipboard->Clear();
		wxDataObjectComposite *doc = new wxDataObjectComposite();
		// add column data
		MadColumnDataObject *coldata = new MadColumnDataObject();
		coldata->SetColumnData( ws, linecount );
		doc->Add( coldata );
		// add text data
		doc->Add( new wxTextDataObject( ws ) );
		bool ok = wxTheClipboard->AddData( doc );
		wxTheClipboard->Flush();
		wxTheClipboard->Close();
		return ok;
	}

	return false;
}

bool MadEdit::PutHexDataToClipboard( const char *cs, size_t length )
{
	if( wxTheClipboard->Open() )
	{
		wxTheClipboard->Clear();
		wxDataObjectComposite *doc = new wxDataObjectComposite();
		//add hex data
		MadHexDataObject *hexdata = new MadHexDataObject();
		hexdata->SetHexData( length, cs );
		doc->Add( hexdata );
		//add text data
		wxString ws = wxString( cs, wxConvLibc, length );
		doc->Add( new wxTextDataObject( ws ) );
		bool ok = wxTheClipboard->AddData( doc );
		wxTheClipboard->Flush();
		wxTheClipboard->Close();
		return ok;
	}

	return false;
}

// translate newline & utf16 surrogates
// return linecount
int MadEdit::TranslateText( const wxChar *pwcs, size_t count, vector<ucs4_t> *ucs, bool passNewLine )
{
	ucs4_t uc = 0;
	size_t i = 0;
	size_t linecount = 0;

	if( count != 0 ) ++linecount;

	while( i < count )
	{
		uc = *pwcs++;
		++i;
#ifdef __WXMSW__

		if( uc >= 0xD800 && uc <= 0xDBFF && i < count )
		{
			ucs4_t uc1 = *pwcs;

			if( uc1 >= 0xDC00 && uc1 <= 0xDFFF )
			{
				++pwcs;
				++i;
				uc = ( ( uc - 0xD800 ) << 10 ) + ( uc1 - 0xDC00 ) + 0x10000;
			}
		}
		else
#endif
			if( uc == 0x0D )
			{
				++linecount;

				if( !passNewLine )
				{
					switch( m_InsertNewLineType )
					{
					case nltDOS:
#ifdef __WXMSW__
					case nltDefault:
#endif
						ucs->push_back( uc );
						uc = 0x0A;
						break;

					case nltMAC:
						// do nothing
						break;

					case nltUNIX:
#ifndef __WXMSW__
					case nltDefault:
#endif
						uc = 0x0A;
						break;
					}

					if( i < count && *pwcs == 0x0A ) // ignore 0x0A
					{
						++pwcs;
						++i;
					}
				}
			}
			else
				if( uc == 0x0A )
				{
					++linecount;

					if( !passNewLine )
					{
						switch( m_InsertNewLineType )
						{
						case nltDOS:
#ifdef __WXMSW__
						case nltDefault:
#endif
							ucs->push_back( 0x0D );
							break;

						case nltMAC:
							uc = 0x0D;
							break;

						case nltUNIX:
#ifndef __WXMSW__
						case nltDefault:
#endif
							// do nothing
							break;
						}
					}
				}

		ucs->push_back( uc );
	}

	return linecount;
}

int MadEdit::GetTextFromClipboard( vector <ucs4_t> *ucs )
{
	int linecount = 0;

	if( wxTheClipboard->Open() )
	{
		if( wxTheClipboard->IsSupported( wxDF_UNICODETEXT ) )
		{
			wxTextDataObject data;
			wxTheClipboard->GetData( data );
			wxString str = data.GetText();
			linecount = TranslateText( str.c_str(), str.Len(), ucs, false );
		}

		wxTheClipboard->Close();
	}

	return linecount;
}

int MadEdit::GetColumnDataFromClipboard( vector <ucs4_t> *ucs )
{
	int linecount = 0;

	if( wxTheClipboard->Open() )
	{
		MadColumnDataObject coldata;

		if( wxTheClipboard->IsSupported( coldata.GetFormat() ) )
		{
			wxTheClipboard->GetData( coldata );
			wxString str;

			if( coldata.GetColumnData( str, linecount ) )
			{
				TranslateText( str.c_str(), str.Len(), ucs, false );
			}
		}

		wxTheClipboard->Close();
	}

	if( ucs->empty() )
	{
		linecount = GetTextFromClipboard( ucs );

		if( linecount != 0 )
		{
			switch( m_InsertNewLineType )
			{
			case nltDOS:
#ifdef __WXMSW__
			case nltDefault:
#endif
				ucs->push_back( 0x0D );
				ucs->push_back( 0x0A );
				break;

			case nltMAC:
				ucs->push_back( 0x0D );
				break;

			case nltUNIX:
#ifndef __WXMSW__
			case nltDefault:
#endif
				ucs->push_back( 0x0A );
				break;
			}
		}
	}

	if( m_AutoFillColumnPaste && m_Selection )
	{
		int rowcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;

		if( rowcount > linecount )
		{
			rowcount -= linecount;
			linecount += rowcount;
			size_t i = 0;

			while( rowcount > 0 )
			{
				for( i = 0; i < ucs->size(); ++i )
				{
					ucs4_t uch = ucs->at( i );

					if( uch == 0x0D || uch == 0x0A )
					{
						--rowcount;

						switch( m_InsertNewLineType )
						{
						case nltDOS:
#ifdef __WXMSW__
						case nltDefault:
#endif
							ucs->push_back( 0x0D );
							ucs->push_back( 0x0A );
							break;

						case nltMAC:
							ucs->push_back( 0x0D );
							break;

						case nltUNIX:
#ifndef __WXMSW__
						case nltDefault:
#endif
							ucs->push_back( 0x0A );
							break;
						}

						if( i < ( ucs->size() - 1 ) )
						{
							if( uch == 0x0D && ucs->at( i + 1 ) == 0x0A )
								++i;
						}
					}
					else
						ucs->push_back( uch );

					if( rowcount == 0 )break;
				}
			}
		}
	}

	return linecount;
}

void MadEdit::GetHexDataFromClipboard( vector <char> *cs )
{
	if( wxTheClipboard->Open() )
	{
		MadHexDataObject hexdata;

		if( wxTheClipboard->IsSupported( hexdata.GetFormat() ) )
		{
			wxTheClipboard->GetData( hexdata );
			size_t size = hexdata.GetHexDataSize();
			cs->resize( size );

			if( size )
			{
				hexdata.GetHexData( &( *cs )[0] );
			}
		}

		wxTheClipboard->Close();
	}

	if( cs->empty() )
	{
		vector<ucs4_t> ucs;
		GetTextFromClipboard( &ucs );

		if( ucs.size() > 0 )
		{
			MadMemData tempmem;
			MadBlock blk( &tempmem, -1, 0 );
			UCStoBlock( &ucs[0], ucs.size(), blk );
			size_t size = blk.m_Size;
			cs->resize( size );
			char *p = &( *cs )[0];

			for( size_t i = 0; i < size; ++i, ++p )
			{
				*p = char( tempmem.Get( i ) );
			}
		}
	}
}

void MadEdit::ReplaceWordFromCaretPos( wxString &ws )
{
	ucs4string out;
	vector<ucs4_t> ucs;
	TranslateText( ws.c_str(), ws.Len(), &ucs, true );

	for( size_t i = 0, size = ucs.size(); i < size; ++i )
	{
		out += ucs[i] ;
	}

	if( !m_Selection )
		SelectWordFromCaretPos( nullptr, nullptr, true );

	InsertString( out.c_str(), out.length(), false, true, false );
}

bool MadEdit::CanPaste()
{
	bool can = false;

	if( wxTheClipboard->Open() )
	{
		if( wxTheClipboard->IsSupported( wxDF_TEXT ) || wxTheClipboard->IsSupported( wxDF_UNICODETEXT ) )
		{
			can = true;
		}
		else
		{
			if( m_EditMode == emHexMode )
			{
				MadHexDataObject hexdata;

				if( wxTheClipboard->IsSupported( hexdata.GetFormat() ) )
				{
					can = true;
				}
			}
			else
				if( m_EditMode == emColumnMode )
				{
					MadColumnDataObject coldata;

					if( wxTheClipboard->IsSupported( coldata.GetFormat() ) )
					{
						can = true;
					}
				}
		}

		wxTheClipboard->Close();
	}

	return can;
}


void MadEdit::CopyFileDataToMem( MadBlockIterator begin, MadBlockIterator end )
{
	wxASSERT( begin != end );

	// ToDo: if the size is too large, move data to a temporary file

	if( m_Lines->m_FileData == nullptr )
		return;

	const size_t BUFFER_SIZE = 128 * 1024;
	static vector<wxByte> TempBufferVector;
	static wxByte *TempBuffer = nullptr;

	if( TempBuffer == nullptr )
	{
		TempBufferVector.resize( BUFFER_SIZE );
		TempBuffer = &TempBufferVector[0];
	}

	do
	{
		if( begin->m_Data == m_Lines->m_FileData )
		{
			wxFileOffset size = begin->m_Size;
			wxFileOffset newpos = -1;
			size_t bs = BUFFER_SIZE;

			do
			{
				// now bs == BUFFER_SIZE;
				if( BUFFER_SIZE > size )
					bs = size;

				begin->m_Data->Get( begin->m_Pos, TempBuffer, bs );
				begin->m_Pos += bs;

				if( newpos < 0 )
					newpos = m_Lines->m_MemData->Put( TempBuffer, bs );
				else
					m_Lines->m_MemData->Put( TempBuffer, bs );
			}
			while( ( size -= bs ) > 0 );

			begin->m_Data = m_Lines->m_MemData;
			begin->m_Pos = newpos;
		}
	}
	while( ++begin != end );
}

MadLineIterator MadEdit::DeleteInsertData( wxFileOffset pos,
		wxFileOffset delsize, /*OUT*/ MadBlockVector *deldata,
		wxFileOffset inssize, /*IN*/  MadBlockVector *insdata,
		int *lineid )
{
	MadLineIterator lit;
	wxFileOffset bpos = pos;
	int tmp_rowid = -1;
	m_UpdateValidPos = 1;   // update if pos < oldpos
	int lid = GetLineByPos( lit, bpos, tmp_rowid );

	if( lineid != nullptr ) *lineid = lid;

	m_UpdateValidPos = 0;
	MadBlockVector &blocks = lit->m_Blocks;
	wxFileOffset headsize = 0, tailsize;
	// separate lit->m_Blocks by bpos
	MadBlockIterator bit = blocks.begin();
	int dist0 = -1;

	if( bpos < pos && bpos + bit->m_Size <= pos )
	{
		do
		{
			headsize += bit->m_Size;
			bpos += bit->m_Size;
			++bit;
			++dist0;
		}
		while( bpos < pos && bpos + bit->m_Size <= pos );
	}

	if( bpos != pos ) // separate bit
	{
		MadBlock block_tmp = *bit;
		MadBlockIterator bit0 = blocks.insert( bit, block_tmp );
		//MadBlockIterator bit0 = blocks.insert(bit, *bit);
		bit0->m_Size = pos - bpos;
		headsize += bit0->m_Size;
		bit = bit0 + 1;
		bit->m_Pos += bit0->m_Size;
		bit->m_Size -= bit0->m_Size;
		++dist0;
	}

	tailsize = lit->m_Size - headsize;

	// delete data from bit
	if( delsize > 0 )
	{
		m_Lines->m_Size -= delsize;

		if( delsize < tailsize ) // in the same line
		{
			lit->m_Size -= delsize;
			MadBlockIterator bit1 = bit;
			bpos = 0;

			if( bit1->m_Size <= delsize )
				do
				{
					bpos += bit1->m_Size;
					++bit1;
				}
				while( bpos < delsize && bpos + bit1->m_Size <= delsize );

			if( bpos != delsize ) // tail, separate bit1
			{
				MadBlock block_tmp = *bit1;
				bit = blocks.insert( bit1, block_tmp );
				//bit = blocks.insert(bit1, *bit1);
				bit->m_Size = delsize - bpos;
				bit1 = bit + 1;
				bit1->m_Pos += bit->m_Size;
				bit1->m_Size -= bit->m_Size;
				bpos += bit->m_Size;
				bit = blocks.begin();
				int dist = dist0 + 1;

				if( dist > 0 ) bit += dist;
			}

			wxASSERT( bpos == delsize );

			if( deldata )
			{
				deldata->insert( deldata->end(), bit, bit1 );
			}

			bit = blocks.erase( bit, bit1 );
		}
		else  // beyond one line
		{
			lit->m_Size -= tailsize;
			MadLineIterator lit1 = lit;

			do
			{
				MadBlockVector &blocks1 = lit1->m_Blocks;

				if( deldata )
				{
					if( deldata->size() != 0 )
					{
						MadBlock & blk = deldata->back();

						if( blk.m_Pos + blk.m_Size == bit->m_Pos
								&& blk.m_Data == bit->m_Data )
						{
							blk.m_Size += bit->m_Size;

							if( bit + 1 != blocks1.end() )
							{
								deldata->insert( deldata->end(), bit + 1, blocks1.end() );
							}
						}
						else
						{
							deldata->insert( deldata->end(), bit, blocks1.end() );
						}
					}
					else
					{
						deldata->insert( deldata->end(), bit, blocks1.end() );
					}
				}

				delsize -= tailsize;

				if( lit1 == lit ) // first line
				{
					blocks1.erase( bit, blocks1.end() );
					++lit1;
				}
				else
				{
					lit1->Reset();
					lit1 = m_Lines->m_LineList.erase( lit1 );
				}

				if( lit1 == m_Lines->m_LineList.end() )
					break;

				tailsize = lit1->m_Size;
				--m_Lines->m_LineCount;
				m_Lines->m_RowCount -= lit1->RowCount();
				bit = lit1->m_Blocks.begin();
			}
			while( delsize > 0 && delsize >= tailsize );

			wxASSERT( delsize <= tailsize );

			if( tailsize == 0 ) // last line
			{
				lit1->Reset();
				lit1 = m_Lines->m_LineList.erase( lit1 );
			}
			else
				if( lit1 != m_Lines->m_LineList.end() )
				{
					lit->m_Size += ( lit1->m_Size - delsize );
					MadBlockVector &blocks1 = lit1->m_Blocks;
					bit = blocks1.begin();

					if( delsize != 0 )
					{
						bpos = 0;

						if( bit->m_Size <= delsize )
							do
							{
								bpos += bit->m_Size;
								++bit;
							}
							while( bpos < delsize && bpos + bit->m_Size <= delsize );

						if( bpos != delsize ) // separate bit
						{
							MadBlock block_tmp = *bit;
							MadBlockIterator bit2 = blocks1.insert( bit, block_tmp );
							//MadBlockIterator bit2 = blocks1.insert(bit, *bit);
							bit2->m_Size = delsize - bpos;
							bit = bit2 + 1;
							bit->m_Pos += bit2->m_Size;
							bit->m_Size -= bit2->m_Size;
						}

						MadBlockIterator bit1 = blocks1.begin();

						if( deldata )
						{
							if( deldata->size() != 0 )
							{
								MadBlock & blk = deldata->back();

								if( blk.m_Pos + blk.m_Size == bit1->m_Pos
										&& blk.m_Data == bit1->m_Data )
								{
									blk.m_Size += bit1->m_Size;

									if( bit1 + 1 != blocks1.end() )
									{
										deldata->insert( deldata->end(), bit1 + 1, bit );
									}
								}
								else
								{
									deldata->insert( deldata->end(), bit1, bit );
								}
							}
							else
							{
								deldata->insert( deldata->end(), bit1, bit );
							}
						}
					}

					blocks.insert( blocks.end(), bit, blocks1.end() );
					lit1->Reset();
					lit1 = m_Lines->m_LineList.erase( lit1 );
				}

			if( lit->m_Size == 0 )
			{
				m_Lines->m_RowCount -= ( lit->RowCount() - 1 );
				lit->Empty();
			}
		}

		if( deldata )
		{
			CopyFileDataToMem( deldata->begin(), deldata->end() );
		}
	}

	// insert data between bit0 and bit
	if( inssize > 0 )
	{
		m_Lines->m_Size += inssize;
		lit->m_Size += inssize;
		bit = blocks.begin();

		if( bit->m_Size == 0 ) // empty line
		{
			blocks.assign( insdata->begin(), insdata->end() );
		}
		else
		{
			MadBlockIterator bit0;

			if( dist0 >= 0 )
			{
				bit0 = bit + dist0;
				bit = bit0 + 1;
			}

			int datasize = int( insdata->size() );
			MadBlockIterator bit1 = insdata->begin();
			MadBlockIterator bit2 = insdata->end() - 1;

			// concat head
			if( dist0 >= 0 && ( bit0->m_Pos + bit0->m_Size ) == bit1->m_Pos
					&& bit0->m_Data == bit1->m_Data )
			{
				bit0->m_Size += bit1->m_Size;
				--datasize;
				++bit1;
			}

			// concat tail
			if( datasize > 0 && bit != blocks.end() )
			{
				if( ( bit2->m_Pos + bit2->m_Size ) == bit->m_Pos
						&& bit2->m_Data == bit->m_Data )
				{
					bit->m_Pos = bit2->m_Pos;
					bit->m_Size += bit2->m_Size;

					if( --datasize > 0 )
						--bit2;
				}
			}

			if( datasize > 0 ) // insert middle data
			{
				blocks.insert( bit, bit1, ++bit2 );
			}
			else     // concat bit0 and bit
			{
				if( dist0 >= 0 && bit != blocks.end() &&
						( bit0->m_Pos + bit0->m_Size ) == bit->m_Pos &&
						bit0->m_Data == bit->m_Data )
				{
					bit0->m_Size += bit->m_Size;
					bit = blocks.erase( bit );
				}
			}
		}
	}
	else
	{
		// concat bit0 and bit
		if( dist0 >= 0 )
		{
			MadBlockIterator bit0 = blocks.begin() + dist0;

			if( bit0 != blocks.end() )
			{
				bit = bit0 + 1;

				if( bit != blocks.end() )
				{
					if( ( bit0->m_Pos + bit0->m_Size ) == bit->m_Pos
							&& bit0->m_Data == bit->m_Data )
					{
						bit0->m_Size += bit->m_Size;
						bit = blocks.erase( bit );
					}
				}
			}
		}
	}

	return lit;
}

// block should be set before call UCStoBlock
void MadEdit::UCStoBlock( const ucs4_t *ucs, size_t count, MadBlock & block )
{
	MadMemData *md = dynamic_cast< MadMemData * > (block.m_Data);
	ucs4_t uc;
	size_t size, idx;
	wxByte mb[4], unitext[14]; // utf16: U+XXXX or utf32: U+XXXXXXXX
	wxByte *b;
	MadEncoding::UCS4toMultiByteFuncPtr UCS4toMultiByte = m_Encoding->UCS4toMultiByte;

	do
	{
		size = ( m_Encoding->*UCS4toMultiByte )( *ucs, mb );
		b = mb;

		if( size == 0 )             // the uc is not supported in current encoding
		{
			b = unitext;
			( m_Encoding->*UCS4toMultiByte )( wxT( '{' ), unitext );
			( m_Encoding->*UCS4toMultiByte )( wxT( 'U' ), unitext + 1 );
			( m_Encoding->*UCS4toMultiByte )( wxT( '+' ), unitext + 2 );
			uc = *ucs;
			wxASSERT( uc >= 0 && uc <= 0x10FFFF );

			if( uc <= 0xFFFF )
			{
				size = 8;
				idx = 3;
			}
			else //if(uc>=0x10000)
			{
				( m_Encoding->*UCS4toMultiByte )( wxT( '0' ), unitext + 3 );
				( m_Encoding->*UCS4toMultiByte )( wxT( '0' ), unitext + 4 );
				( m_Encoding->*UCS4toMultiByte )( ToHex( ( uc >> 20 ) & 0xF ), unitext + 5 );
				( m_Encoding->*UCS4toMultiByte )( ToHex( ( uc >> 16 ) & 0xF ), unitext + 6 );
				size = 12;
				idx = 7;
			}

			( m_Encoding->*UCS4toMultiByte )( ToHex( ( uc >> 12 ) & 0xF ), unitext + ( idx++ ) );
			( m_Encoding->*UCS4toMultiByte )( ToHex( ( uc >> 8 ) & 0xF ), unitext + ( idx++ ) );
			( m_Encoding->*UCS4toMultiByte )( ToHex( ( uc >> 4 ) & 0xF ), unitext + ( idx++ ) );
			( m_Encoding->*UCS4toMultiByte )( ToHex( uc & 0xF ), unitext + ( idx++ ) );
			( m_Encoding->*UCS4toMultiByte )( wxT( '}' ), unitext + idx );
		}

		if( block.m_Pos < 0 )
		{
			block.m_Pos = md->Put( b, size );
		}
		else
		{
			md->Put( b, size );
		}

		block.m_Size += size;
		++ucs;
	}
	while( --count > 0 );
}

void MadEdit::InsertString( const ucs4_t *ucs, size_t count, bool bColumnEditing, bool moveCaret, bool bSelText, bool insert/* = false*/ )
{
	if( IsReadOnly() ) // || (m_EditMode == emHexMode && m_CaretAtHexArea))
		return;

	bool oldModified = m_Modified;

	if( m_SingleLineMode )
	{
		const ucs4_t *ucs1 = ucs;
		int sss = 0;

		while( sss < int( count ) && *ucs1 != 0x0D && *ucs1 != 0x0A )
		{
			++sss;
			++ucs1;
		}

		int maxlen = m_MaxLineLength - 100;

		if( m_Lines->m_Size + sss > maxlen )
			sss = maxlen - int( m_Lines->m_Size );

		if( sss <= 0 )
			return;

		count = size_t( sss );
	}

	MadBlock blk( m_Lines->m_MemData, -1, 0 );
	m_ZeroSelection = false;

	if( m_Selection )
	{
		if( m_EditMode == emColumnMode )
		{
			int colcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;

			if( bColumnEditing && colcount > 1 )
			{
				vector<ucs4_t> colstr;
				int cc = colcount;

				do
				{
					const ucs4_t *pu = ucs;

					for( size_t cnt = count; cnt > 0; --cnt )
					{
						colstr.push_back( *pu++ );
					}

					colstr.push_back( ucs4_t( 0x0A ) ); // newline char
				}
				while( --cc > 0 );

				InsertColumnString( &colstr[0], colstr.size(), colcount, true, bSelText );
			}
			else
			{
				InsertColumnString( ucs, count, 0, false, bSelText );
			}
		}
		else //TextMode & HexMode replace text
		{
			if( !m_DragDrop )
			{
				bool replace = (!(insert && m_InsertMode && m_InsertPairForSelection && m_Selection));
				MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
				oudata->m_Pos = m_SelectionBegin->pos;
				if(replace)
				{
					oudata->m_DelSize = m_SelectionEnd->pos - m_SelectionBegin->pos;
				}
				else
				{
					oudata->m_DelSize = 0;
				}

				UCStoBlock( ucs, count, blk );
				oudata->m_InsSize = blk.m_Size;
				oudata->m_InsData.push_back( blk );

				if( !m_InsertMode )
				{
					wxFileOffset delsize = ( blk.m_Size > oudata->m_DelSize ) ? blk.m_Size : oudata->m_DelSize;

					if( delsize > ( m_Lines->m_Size - m_SelectionBegin->pos ) ) delsize = m_Lines->m_Size - m_SelectionBegin->pos;

					oudata->m_DelSize = delsize;
				}

				MadLineIterator lit = DeleteInsertData( oudata->m_Pos,
														oudata->m_DelSize, &oudata->m_DelData,
														oudata->m_InsSize, &oudata->m_InsData );
				MadUndo *undo = m_UndoBuffer->Add();
				SetNeedSync();
				undo->m_CaretPosBefore = m_CaretPos.pos;
				undo->m_CaretPosAfter = oudata->m_Pos + oudata->m_InsSize;
				undo->m_Undos.push_back( oudata );
				bool sc = ( oldModified == false );
				m_Modified = true;
				m_Selection = false;
				m_RepaintAll = true;
				Refresh( false );

				if( IsTextFile() )
				{
					m_Lines->Reformat( lit, lit );
					int oldsubrowid = m_CaretPos.subrowid, oldlineid = m_CaretPos.lineid;
					m_CaretPos.pos = undo->m_CaretPosAfter;
					UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					AppearCaret();
					
					if(m_TypewriterMode && (IsTextMode()))
					{
						m_TopRow += (m_CaretPos.lineid - oldlineid) + (m_CaretPos.subrowid - oldsubrowid);
					}

					UpdateScrollBarPos();

					if( m_EditMode == emHexMode )
					{
						if( !m_CaretAtHexArea )
						{
							UpdateTextAreaXPos();
							m_LastTextAreaXPos = m_TextAreaXPos;
						}
					}
				}
				else
				{
					m_CaretPos.pos = undo->m_CaretPosAfter;
					m_CaretPos.linepos = m_CaretPos.pos;
					AppearCaret();
					UpdateScrollBarPos();

					if( !m_CaretAtHexArea )
					{
						UpdateTextAreaXPos();
						m_LastTextAreaXPos = m_TextAreaXPos;
					}
				}

				m_LastCaretXPos = m_CaretPos.xpos;

				if( bSelText )
				{
					m_Selection = true;
					m_SelectionPos1.pos = oudata->m_Pos;
					m_SelectionPos2.pos = undo->m_CaretPosAfter;
					UpdateSelectionPos();
				}

				DoSelectionChanged();

				if( sc ) DoStatusChanged();
			}
			else
			{
				/*hacking for drag&drop*/
				{
					MadDeleteUndoData *oudata = nullptr;
					MadInsertUndoData *indata = new MadInsertUndoData();

					if( m_DragCopyFlag )
					{
						if( m_CaretPos.extraspaces )
						{
							ucs4_t *sp = new ucs4_t[m_CaretPos.extraspaces];

							for( int i = 0; i < m_CaretPos.extraspaces; ++i )
								sp[i] = 0x20;

							UCStoBlock( sp, m_CaretPos.extraspaces, blk );
							delete[]sp;
						}
					}

					UCStoBlock( ucs, count, blk );
					indata->m_Size = m_SelectionEnd->pos - m_SelectionBegin->pos;
					indata->m_Data.push_back( blk );
					wxFileOffset dltOff = 0;

					if( !m_DragCopyFlag )
					{
						oudata = new MadDeleteUndoData();
						oudata->m_Pos = m_SelectionBegin->pos;
						oudata->m_Size = indata->m_Size;
						oudata->m_Data.push_back( blk );

						if( m_CaretPos.pos > oudata->m_Pos )
						{
							dltOff = oudata->m_Size;
						}
					}

					MadLineIterator lit;
					indata->m_Pos = m_CaretPos.pos - dltOff;
					MadUndo *undo = m_UndoBuffer->Add();
					SetNeedSync();

					if( !m_DragCopyFlag )
					{
						lit = DeleteInsertData( oudata->m_Pos, oudata->m_Size, nullptr, 0, nullptr );

						if( IsTextFile() )
						{
							m_Lines->Reformat( lit, lit );
						}

						undo->m_CaretPosBefore = oudata->m_Pos;
						undo->m_Undos.push_back( oudata );
					}
					else
					{
						undo->m_CaretPosBefore = indata->m_Pos ;
					}

					lit = DeleteInsertData( indata->m_Pos, 0, nullptr, indata->m_Size, &indata->m_Data );
					undo->m_CaretPosAfter = indata->m_Pos;
					undo->m_Undos.push_back( indata );
					bool sc = ( oldModified == false );
					m_Modified = true;
					m_Selection = false;
					m_RepaintAll = true;
					Refresh( false );

					if( IsTextFile() )
					{
						m_Lines->Reformat( lit, lit );
						m_CaretPos.pos = undo->m_CaretPosAfter;
						UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						AppearCaret();
						UpdateScrollBarPos();

						if( m_EditMode == emHexMode )
						{
							if( !m_CaretAtHexArea )
							{
								UpdateTextAreaXPos();
								m_LastTextAreaXPos = m_TextAreaXPos;
							}
						}
					}
					else
					{
						m_CaretPos.pos = undo->m_CaretPosAfter - dltOff;
						m_CaretPos.linepos = m_CaretPos.pos;
						AppearCaret();
						UpdateScrollBarPos();

						if( !m_CaretAtHexArea )
						{
							UpdateTextAreaXPos();
							m_LastTextAreaXPos = m_TextAreaXPos;
						}
					}

					m_LastCaretXPos = m_CaretPos.xpos;
					DoSelectionChanged();

					if( sc ) DoStatusChanged();
				}
			}
		}
	}
	else
	{
		// Overwrite Mode
		if( m_InsertMode == false &&
				( ( m_EditMode == emHexMode && m_CaretPos.pos < m_Lines->m_Size ) ||
				  ( m_CaretPos.linepos < m_CaretPos.iter->m_Size - m_CaretPos.iter->m_NewLineSize ) ) )
		{
			MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
			oudata->m_Pos = m_CaretPos.pos;
			UCStoBlock( ucs, count, blk );
			oudata->m_InsSize = blk.m_Size;
			oudata->m_InsData.push_back( blk );

			if( m_EditMode == emHexMode )
			{
				oudata->m_DelSize = m_Lines->m_Size - m_CaretPos.pos;

				if( oudata->m_DelSize > oudata->m_InsSize )
					oudata->m_DelSize = oudata->m_InsSize;
			}
			else
			{
				oudata->m_DelSize = 0;
				MadUCQueue ucqueue;
				MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
				m_Lines->InitNextUChar( m_CaretPos.iter, m_CaretPos.linepos );

				do
				{
					if( !( m_Lines->*NextUChar )( ucqueue ) )
						break;

					ucs4_t uc = ucqueue.back().first;

					if( uc == 0x0D || uc == 0x0A )
						break;
				}
				while( ( oudata->m_DelSize += ucqueue.back().second ) < blk.m_Size );
			}

			MadLineIterator lit = DeleteInsertData( oudata->m_Pos,
													oudata->m_DelSize, &oudata->m_DelData,
													oudata->m_InsSize, &oudata->m_InsData );
			MadUndo *undo = m_UndoBuffer->Add();
			SetNeedSync();
			undo->m_CaretPosBefore = m_CaretPos.pos;
			undo->m_CaretPosAfter = oudata->m_Pos + oudata->m_InsSize;
			undo->m_Undos.push_back( oudata );
			bool sc = ( oldModified == false );
			m_Modified = true;
			m_Selection = false;
			m_RepaintAll = true;
			Refresh( false );

			if( IsTextFile() )
			{
				m_Lines->Reformat( lit, lit );
				m_CaretPos.pos = oudata->m_Pos + oudata->m_InsSize;
				UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
				AppearCaret();
				UpdateScrollBarPos();

				if( m_EditMode == emHexMode )
				{
					//if(!m_CaretAtHexArea)
					{
						UpdateTextAreaXPos();
						m_LastTextAreaXPos = m_TextAreaXPos;
					}
				}
			}
			else
			{
				m_CaretPos.pos = oudata->m_Pos + oudata->m_InsSize;
				m_CaretPos.linepos = m_CaretPos.pos;
				AppearCaret();
				UpdateScrollBarPos();

				if( !m_CaretAtHexArea )
				{
					UpdateTextAreaXPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
				}
			}

			m_LastCaretXPos = m_CaretPos.xpos;

			if( bSelText )
			{
				m_Selection = true;
				m_SelectionPos1.pos = oudata->m_Pos;
				m_SelectionPos2.pos = undo->m_CaretPosAfter;
				UpdateSelectionPos();
			}

			DoSelectionChanged();

			if( sc ) DoStatusChanged();
		}
		else // insert string to current pos
		{
			if( m_CaretPos.extraspaces )
			{
				ucs4_t *sp = new ucs4_t[m_CaretPos.extraspaces];

				for( int i = 0; i < m_CaretPos.extraspaces; ++i )
					sp[i] = 0x20;

				UCStoBlock( sp, m_CaretPos.extraspaces, blk );
				delete[]sp;
			}

			UCStoBlock( ucs, count, blk );
			MadInsertUndoData *insud = new MadInsertUndoData;
			insud->m_Pos = m_CaretPos.pos;
			insud->m_Size = blk.m_Size;
			insud->m_Data.push_back( blk );
			MadUndo *undo = m_UndoBuffer->Add();
			SetNeedSync();
			undo->m_CaretPosBefore = m_CaretPos.pos;

			if( moveCaret )
			{
				undo->m_CaretPosAfter = m_CaretPos.pos + blk.m_Size;
			}
			else
			{
				undo->m_CaretPosAfter = m_CaretPos.pos;
			}

			undo->m_Undos.push_back( insud );
			size_t oldrows = m_Lines->m_RowCount;
			size_t oldlines = m_Lines->m_LineCount;
			MadLineIterator lit = DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );

			if( IsTextFile() )
			{
				size_t cnt = m_Lines->Reformat( lit, lit );
				m_CaretPos.pos = undo->m_CaretPosAfter;
				UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );

				if( m_EditMode == emHexMode || oldrows != m_Lines->m_RowCount
						|| oldlines != m_Lines->m_LineCount || cnt > 1 )
				{
					if(m_TypewriterMode && IsTextMode()) //Wrapped
					{
						m_TopRow += (m_Lines->m_RowCount - oldrows);
					}
					m_RepaintAll = true;
				}
				else
				{
					m_RepaintSelection = true;
					m_SelFirstRow = m_CaretPos.rowid - m_CaretPos.subrowid;
					m_SelLastRow = m_SelFirstRow + int( lit->RowCount() - 1 );
				}

				AppearCaret();
				UpdateScrollBarPos();

				if( m_EditMode == emHexMode )
				{
					if( !m_CaretAtHexArea )
					{
						UpdateTextAreaXPos();
						m_LastTextAreaXPos = m_TextAreaXPos;
					}
				}
			}
			else
			{
				m_CaretPos.pos = undo->m_CaretPosAfter;
				m_CaretPos.linepos = m_CaretPos.pos;
				m_RepaintAll = true;
				AppearCaret();
				UpdateScrollBarPos();

				if( !m_CaretAtHexArea )
				{
					UpdateTextAreaXPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
				}
			}

			bool changed = ( m_Modified == false );
			m_Modified = true;
			m_LastCaretXPos = m_CaretPos.xpos;
			Refresh( false );

			if( bSelText )
			{
				m_Selection = true;
				m_SelectionPos1.pos = insud->m_Pos;
				m_SelectionPos2.pos = insud->m_Pos + insud->m_Size;
				UpdateSelectionPos();
			}

			DoSelectionChanged();

			if( changed ) DoStatusChanged();
		}
	}
}

wxFileOffset MadEdit::GetRowposXPos( int &xpos, MadLineIterator & lit,
									 int subrowid, wxFileOffset inssize )
{
	int rowwidth = lit->m_RowIndices[subrowid].m_Width;
	wxFileOffset rowpos = lit->m_RowIndices[subrowid].m_Start + inssize;
	wxFileOffset rowendpos = lit->m_RowIndices[subrowid + 1].m_Start + inssize;

	if( xpos < rowwidth )
	{
		int nowxpos = 0;
		MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
		MadUCQueue ucqueue;
		m_Lines->InitNextUChar( lit, rowpos );

		do
		{
			int uc = 0x0D;

			if( ( m_Lines->*NextUChar )( ucqueue ) )
				uc = ucqueue.back().first;

			if( uc == 0x0D || uc == 0x0A )    // EOL
			{
				break;
			}

			int ucwidth = GetUCharWidth( uc );

			if( uc == 0x09 )
			{
				int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
				ucwidth = rowwidth - nowxpos;
				tabwidth -= ( nowxpos % tabwidth );

				if( tabwidth < ucwidth )
					ucwidth = tabwidth;
			}

			nowxpos += ucwidth;
			int uchw = ucwidth >> 1;

			if( xpos > uchw )
			{
				rowpos += ucqueue.back().second;
				xpos -= ucwidth;
			}
			else
			{
				xpos = 0;
				return rowpos;
			}
		}
		while( xpos > 0 && rowpos < rowendpos );
	}
	else
	{
		xpos -= rowwidth;
		return rowendpos;
	}

	return rowpos;
}

void MadEdit::InsertColumnString( const ucs4_t *ucs, size_t count, int linecount, bool bColumnEditing, bool bSelText )
{
	MadUndo *undo = nullptr;
	vector < int >rpos;
	int xpos0;
	wxFileOffset pos, selbegin;
	MadLineIterator firstlit, lastlit;
	size_t rowcount;
	int subrowid, rowid, lineid;
	int lines = linecount;
	bool oldModified = m_Modified;

	m_ZeroSelection = false;
	// delete selection
	if( m_Selection )
	{
		selbegin = m_SelectionBegin->pos;
		xpos0 = m_SelLeftXPos;
		pos = m_SelectionBegin->pos - m_SelectionBegin->linepos;
		firstlit = m_SelectionBegin->iter;
		subrowid = m_SelectionBegin->subrowid;
		lastlit = m_SelectionEnd->iter;
		rowcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;
		rowid = m_SelectionBegin->rowid;
		lineid = m_SelectionBegin->lineid;
		undo = DeleteSelection( false, &rpos, false );
	}
	else
	{
		selbegin = m_CaretPos.pos;
		xpos0 = m_CaretPos.xpos;
		pos = m_CaretPos.pos - m_CaretPos.linepos;
		firstlit = m_CaretPos.iter;
		subrowid = m_CaretPos.subrowid;
		lastlit = firstlit;
		rowcount = 1;
		rowid = m_CaretPos.rowid;
		lineid = m_CaretPos.lineid;
	}

	if( undo == nullptr )
	{
		undo = m_UndoBuffer->Add();
		SetNeedSync();
	}

	undo->m_CaretPosBefore = m_CaretPos.pos;
	wxFileOffset rowpos;
	int xpos;
	size_t idx = rpos.size();

	if( idx )
	{
		xpos = rpos[--idx];
		rowpos = rpos[--idx];
	}
	else
	{
		xpos = xpos0;
		rowpos = GetRowposXPos( xpos, firstlit, subrowid, 0 );
	}

	MadBlock blk( m_Lines->m_MemData, -1, 0 );

	if( lines == 0 )              // insert normal string
	{
		if( xpos > 0 )
		{
			int spaces = 0;
			int sw = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
			spaces = xpos / sw;
			xpos -= spaces * sw;

			if( xpos > ( sw >> 1 ) )
			{
				++spaces;
			}

			if( spaces )
			{
				ucs4_t *sp = new ucs4_t[spaces];

				for( int i = 0; i < spaces; ++i )
					sp[i] = 0x20;

				UCStoBlock( sp, spaces, blk );
				delete[]sp;
			}
		}

		UCStoBlock( ucs, count, blk );
		wxFileOffset cpos;

		if( m_InsertMode )
		{
			MadInsertUndoData *insud = new MadInsertUndoData;
			insud->m_Pos = pos + rowpos;
			cpos = insud->m_Pos;
			insud->m_Size = blk.m_Size;
			insud->m_Data.push_back( blk );
			undo->m_Undos.push_back( insud );
			DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );
		}
		else
		{
			MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
			oudata->m_Pos = pos + rowpos;
			cpos = oudata->m_Pos;
			oudata->m_InsSize = blk.m_Size;
			oudata->m_InsData.push_back( blk );
			oudata->m_DelSize = blk.m_Size;

			if( oudata->m_DelSize > ( firstlit->m_Size - oudata->m_Pos ) ) oudata->m_DelSize = ( firstlit->m_Size -  oudata->m_Pos );

			if( oudata->m_DelSize > oudata->m_InsSize )
				oudata->m_DelSize = oudata->m_InsSize;

			undo->m_Undos.push_back( oudata );
			DeleteInsertData( oudata->m_Pos,
							  oudata->m_DelSize, &oudata->m_DelData,
							  oudata->m_InsSize, &oudata->m_InsData );
		}

		m_Lines->Reformat( firstlit, lastlit );
		m_CaretPos.pos = cpos + blk.m_Size;
		undo->m_CaretPosAfter = m_CaretPos.pos;
		UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		m_RepaintAll = true;
		AppearCaret();
		UpdateScrollBarPos();
		bool sc = ( oldModified == false );
		m_Modified = true;
		m_LastCaretXPos = m_CaretPos.xpos;
		Refresh( false );

		if( bSelText )
		{
			m_Selection = true;
			m_ZeroSelection = false;
			m_SelectionPos1.pos = cpos;
			m_SelectionPos2.pos = undo->m_CaretPosAfter;
			UpdateSelectionPos();
		}

		DoSelectionChanged();

		if( sc ) DoStatusChanged();
	}
	else // insert column string
	{
		MadLineIterator lit = firstlit;
		wxFileOffset inssize = 0, selcaretpos = -1;
		const ucs4_t *ucs1 = ucs;
		// newline
		ucs4_t nl[2];
		size_t cnt = 0;

		switch( m_InsertNewLineType )
		{
		case nltUNIX:
#ifndef __WXMSW__
		case nltDefault:
#endif
			nl[cnt++] = 0x0A;
			break;

		case nltMAC:
			nl[cnt++] = 0x0D;
			break;

		case nltDOS:
#ifdef __WXMSW__
		case nltDefault:
#endif
			nl[cnt++] = 0x0D;
			nl[cnt++] = 0x0A;
			break;

		default:
			break;
		}

		for( ;; )
		{
			int ulen = 0;

			while( *ucs1 != 0x0D && *ucs1 != 0x0A )
			{
				++ulen;
				++ucs1;
			}

			if( ulen > 0 && xpos > 0 )
			{
				int spaces;
				int sw = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
				spaces = xpos / sw;
				xpos -= spaces * sw;

				if( xpos > ( sw >> 1 ) )
				{
					++spaces;
				}

				if( spaces )
				{
					ucs4_t *sp = new ucs4_t[spaces];

					for( int i = 0; i < spaces; ++i )
						sp[i] = 0x20;

					UCStoBlock( sp, spaces, blk );
					delete[]sp;
				}
			}

			if( ulen != 0 )
			{
				UCStoBlock( ucs, ulen, blk );
			}

			if( lines == 1 && lit == m_Lines->m_LineList.end() )
			{
				// add newline
				UCStoBlock( &nl[0], cnt, blk );
			}

			if( blk.m_Size != 0 )
			{
				wxFileOffset cpos = 0, msize = 0;

				if( m_InsertMode || ( lit == m_Lines->m_LineList.end() ) )
				{
					MadInsertUndoData *insud = new MadInsertUndoData;
					insud->m_Pos = pos + rowpos;
					insud->m_Size = blk.m_Size;
					insud->m_Data.push_back( blk );
					cpos = insud->m_Pos;
					msize = insud->m_Size;
					undo->m_Undos.push_back( insud );
					DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );
				}
				else
				{
					MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
					oudata->m_Pos = pos + rowpos;
					oudata->m_InsSize = blk.m_Size;
					oudata->m_InsData.push_back( blk );
					oudata->m_DelSize = blk.m_Size;
					wxFileOffset leftsize = ( lit->m_Size - rowpos - lit->m_NewLineSize );

					if( oudata->m_DelSize > leftsize ) oudata->m_DelSize = leftsize;

					cpos = oudata->m_Pos;
					msize = oudata->m_InsSize;
					undo->m_Undos.push_back( oudata );
					DeleteInsertData( oudata->m_Pos,
									  oudata->m_DelSize, &oudata->m_DelData,
									  oudata->m_InsSize, &oudata->m_InsData );
				}

				if( bColumnEditing )
				{
					if( lines == linecount ) // firstline
					{
						m_SelectionPos2.pos = cpos + msize;
					}

					if( lines == 1 ) // lastline
					{
						m_SelectionPos1.pos = cpos + msize;
					}
				}

				if( bSelText && lines == 1 )
				{
					selcaretpos = cpos + msize;
				}
			}
			else
				if( bSelText && lines == 1 )
				{
					int sw = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
					int xpos1 = m_SelLeftXPos, xpos2 = m_SelRightXPos;
					int spaces1 = 0;

					if( xpos1 > 0 )
					{
						spaces1 = xpos1 / sw;
						xpos1 -= spaces1 * sw;

						if( xpos1 > ( sw >> 1 ) )
						{
							++spaces1;
						}
					}

					int spaces2 = 0;

					if( xpos2 > 0 )
					{
						spaces2 = xpos2 / sw;
						xpos2 -= spaces2 * sw;

						if( xpos2 > ( sw >> 1 ) )
						{
							++spaces2;
						}
					}

					wxFileOffset maxlinepos = lit->m_Size - lit->m_NewLineSize;

					if( rowpos + spaces2 - spaces1 >= maxlinepos )
					{
						selcaretpos = pos + maxlinepos;
					}
					else
					{
						selcaretpos = pos + rowpos + spaces2 - spaces1;
					}
				}

			if( --lines == 0 )
			{
				MadLineIterator lend = m_Lines->m_LineList.end();
				--lend;

				if( bColumnEditing == false && bSelText == false && lit == lend )
				{
					// append a newline char at end of file
					blk.m_Pos = -1;
					blk.m_Size = 0;
					UCStoBlock( &nl[0], cnt, blk );

					if( m_InsertMode )
					{
						MadInsertUndoData *insud = new MadInsertUndoData;
						insud->m_Pos = m_Lines->m_Size;
						insud->m_Size = blk.m_Size;
						insud->m_Data.push_back( blk );
						undo->m_Undos.push_back( insud );
						DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );
					}
					else
					{
						MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
						oudata->m_Pos = m_Lines->m_Size;
						oudata->m_InsSize = blk.m_Size;
						oudata->m_InsData.push_back( blk );
						oudata->m_DelSize = blk.m_Size;

						if( oudata->m_DelSize > ( lit->m_Size - oudata->m_Pos ) ) oudata->m_DelSize = ( lit->m_Size - oudata->m_Pos );

						if( oudata->m_DelSize > oudata->m_InsSize )
							oudata->m_DelSize = oudata->m_InsSize;

						undo->m_Undos.push_back( oudata );
						DeleteInsertData( oudata->m_Pos,
										  oudata->m_DelSize, &oudata->m_DelData,
										  oudata->m_InsSize, &oudata->m_InsData );
					}

					lit = m_Lines->m_LineList.end();
				}

				break;
			}

			if( *ucs1 == 0x0D )
			{
				++ucs1;

				if( *ucs1 == 0x0A )
					++ucs1;
			}
			else
				if( *ucs1 == 0x0A )
					++ucs1;

			ucs = ucs1;
			// to next row
			++rowid;

			if( subrowid != -1 )
			{
				if( ++subrowid == int( lit->RowCount() ) )
				{
					pos += lit->m_Size;
					++lit;
					subrowid = 0;
					++lineid;
					inssize = 0;
				}
				else
				{
					inssize += blk.m_Size;
				}
			}
			else
			{
				pos += blk.m_Size;
				inssize = 0;
			}

			blk.m_Pos = -1;
			blk.m_Size = 0;

			if( idx )
			{
				xpos = rpos[--idx];
				rowpos = rpos[--idx] + inssize;
			}
			else
			{
				if( subrowid == -1 || lit == m_Lines->m_LineList.end() )
				{
					subrowid = -1;
					rowpos = 0;
					xpos = xpos0;
					// add newline
					UCStoBlock( &nl[0], cnt, blk );
				}
				else
				{
					xpos = xpos0;
					rowpos = GetRowposXPos( xpos, lit, subrowid, inssize );
				}
			}
		}

		if( lit == m_Lines->m_LineList.end() )
		{
			lastlit = lit;
			--lastlit;
		}
		else
		{
			if( linecount > int( rowcount ) )
			{
				lastlit = lit;
			}

			rowcount = lit->RowCount();
		}

		m_Lines->Reformat( firstlit, lastlit );

		if( bColumnEditing )
		{
			m_Selection = true;
			UpdateSelectionPos();
			m_CaretPos.pos = m_SelectionBegin->pos;
			UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		}
		else
		{
			if( lit == m_Lines->m_LineList.end() )
			{
				--lit;
				m_CaretPos.iter = lit;
				m_CaretPos.subrowid = 0;
				m_CaretPos.rowid = int( m_Lines->m_RowCount - 1 );
				m_CaretPos.lineid = int( m_Lines->m_LineCount - 1 );
				m_CaretPos.pos = m_Lines->m_Size;
				m_CaretPos.linepos = 0;
				UpdateCaretByXPos( xpos0, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			}
			else
				if( bSelText )
				{
					m_Selection = true;
					m_SelectionPos1.pos = selbegin;
					m_SelectionPos2.pos = selcaretpos;
					UpdateSelectionPos();
					m_CaretPos = m_SelectionPos2;
					UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
				}
				else
				{
					// to next row
					if( ++subrowid == int( rowcount ) )
					{
						pos += lit->m_Size;
						++lit;
						subrowid = 0;
					}

					m_CaretPos.pos = pos + lit->m_RowIndices[subrowid].m_Start;
					UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					xpos = xpos0 - m_CaretPos.xpos;

					if( xpos > 0 )
					{
						if( m_CaretRowUCharPos < int( m_ActiveRowWidths.size() ) )
							do
							{
								int w = m_ActiveRowWidths[m_CaretRowUCharPos];

								if( xpos > ( w >> 1 ) )
								{
									xpos -= w;
									m_CaretPos.xpos += w;
									m_CaretPos.linepos += m_ActiveRowUChars[m_CaretRowUCharPos].second;
									m_CaretPos.pos += m_ActiveRowUChars[m_CaretRowUCharPos].second;
									++m_CaretRowUCharPos;
								}
								else
								{
									xpos = 0;
									break;
								}
							}
							while( xpos > 0 && m_CaretRowUCharPos < int( m_ActiveRowWidths.size() ) );

						if( xpos > 0 )
						{
							int spaces = 0;
							int sw = m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
							spaces = xpos / sw;
							xpos -= spaces * sw;

							if( xpos > ( sw >> 1 ) )
							{
								++spaces;
							}

							m_CaretPos.extraspaces = spaces;
							m_CaretPos.xpos += spaces * sw;
						}
					}
				}
		}

		undo->m_CaretPosAfter = m_CaretPos.pos;
		m_RepaintAll = true;
		AppearCaret();
		UpdateScrollBarPos();
		bool sc = ( undo->m_Undos.size() > 0 && oldModified == false );

		if( undo->m_Undos.size() > 0 )
		{
			m_Modified = true;
		}

		m_LastCaretXPos = m_CaretPos.xpos;
		Refresh( false );
		DoSelectionChanged();

		if( sc ) DoStatusChanged();
	}
}

// get indent spaces from current the line of caretpos
void MadEdit::GetIndentSpaces( int lineid, MadLineIterator lit, vector <ucs4_t> &spaces, bool autoIndent, bool unindentChar )
{
	MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
	wxFileOffset linepos = lit->m_RowIndices.front().m_Start;
	m_Lines->InitNextUChar( lit, linepos );
	vector<ucs4_t> sp;  // the indent spaces
	MadUCQueue ucqueue;
	ucs4_t uc = 0x0D;
	MadUCQueue::size_type delta = 0;

	for( ;; )
	{
		uc = 0x0D;
		delta = 0; //For the last line before EOF

		if( ( m_Lines->*NextUChar )( ucqueue ) )
		{
			MadUCPair &ucp = ucqueue.back();
			uc = ucp.first;
			linepos += ucp.second;
			delta = 1;
		}

		if( uc != 0x20 && uc != 0x09 )
		{
			int size = int( ucqueue.size() ) - delta;

			for( int i = 0; i < size; ++i )
			{
				sp.push_back( ucqueue[i].first );
			}

			break;
		}
		else
			if( uc == 0x0D || uc == 0x0A )
			{
				ucqueue.clear();

				if( lineid == 0 ) // first line
				{
					break;
				}
				else
				{
					--lit;
					--lineid;
					linepos = lit->m_RowIndices.front().m_Start;
					m_Lines->InitNextUChar( lit, linepos );
				}
			}
	}

	if( autoIndent && !m_Syntax->m_SynAttr->m_IndentChar.IsEmpty() ) // check the last char is IndentChar or not
	{
		MadCaretPos *maxpos;

		if( m_Selection )
		{
			maxpos = m_SelectionBegin;
		}
		else
		{
			maxpos = &m_CaretPos;
		}

		while( lineid < maxpos->lineid || ( lineid == maxpos->lineid && linepos < maxpos->linepos ) )
		{
			if( ( m_Lines->*NextUChar )( ucqueue ) == false )
			{
				break;
			}

			linepos += ucqueue.back().second;
		}

		// trim spaces
		while( ucqueue.size() > 0 && ucqueue.back().first <= 0x20 )
		{
			ucqueue.pop_back();
		}

		if( ucqueue.size() > 0 && m_Syntax->m_SynAttr->m_IndentChar.Find( wxChar( ucqueue.back().first ) ) >= 0 )
		{
			if( unindentChar == false )
			{
				IncreaseIndentSpaces( sp );
			}
		}
		else
		{
			if( unindentChar )
			{
				DecreaseIndentSpaces( sp );
			}
		}
	}

	if( sp.size() > 0 )
	{
		spaces.insert( spaces.end(), sp.begin(), sp.end() );
	}
}

size_t MadEdit::CalcColumns( vector <ucs4_t> &spaces )
{
	size_t columns = 0;
	int count = int( spaces.size() );

	for( int i = 0; i < count; ++i )
	{
		ucs4_t sp = spaces[i];
		wxASSERT( sp == 0x20 || sp == 0x09 );

		if( sp == 0x20 )
		{
			++columns;
		}
		else // sp==0x09
		{
			size_t mod = columns % m_TabColumns;
			columns += ( m_TabColumns - mod );
		}
	}

	return columns;
}

void MadEdit::ConvertColumns( size_t columns, vector <ucs4_t> &spaces )
{
	spaces.clear();

	if( columns > 0 )
	{
		if( m_InsertSpacesInsteadOfTab )
		{
			spaces.resize( columns, 0x20 );
		}
		else
		{
			size_t tabs = columns / m_TabColumns;
			spaces.resize( tabs, 0x09 );
			size_t spas = columns - ( tabs * m_TabColumns );

			if( spas > 0 )
			{
				spaces.insert( spaces.end(), spas, 0x20 );
			}
		}
	}
}

void MadEdit::IncreaseIndentSpaces( vector <ucs4_t> &spaces )
{
	// calc columns of spaces
	size_t columns = CalcColumns( spaces );
	// increase indent
	size_t mod = columns % m_IndentColumns;
	columns += ( m_IndentColumns - mod );
	// convert columns to Tabs or Spaces
	ConvertColumns( columns, spaces );
}

void MadEdit::DecreaseIndentSpaces( vector <ucs4_t> &spaces )
{
	// calc columns of spaces
	size_t columns = CalcColumns( spaces );
	// decrease indent
	size_t mod = ( columns % m_IndentColumns );

	if( mod == 0 ) mod = m_IndentColumns;

	if( columns > mod )
		columns -= mod;
	else
		columns = 0;

	// convert columns to Tabs or Spaces
	ConvertColumns( columns, spaces );
}

MadUndo *MadEdit::DeleteSelection( bool bCorrectCaretPos, vector <int> *rpos, bool bColumnEditing )
{
	m_ZeroSelection = false;

	if( !m_Selection || IsReadOnly() )
		return nullptr;

	bool oldModified = m_Modified;

	if( m_EditMode == emColumnMode )
	{
		MadUndo *undo = nullptr;
		int firstrow = m_SelectionBegin->rowid;
		int lastrow = m_SelectionEnd->rowid;
		int subrowid = m_SelectionEnd->subrowid;
		int subrows = 0;
		MadLineIterator lastlit = m_SelectionEnd->iter;
		wxFileOffset pos = m_SelectionEnd->pos - m_SelectionEnd->linepos;
		MadLineIterator firstlit = lastlit;
		MadUCQueue ucqueue;
		MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

		for( ;; )
		{
			int rowwidth = firstlit->m_RowIndices[subrowid].m_Width;
			int nowxpos = 0;
			wxFileOffset rowpos = firstlit->m_RowIndices[subrowid].m_Start;
			wxFileOffset rowendpos = firstlit->m_RowIndices[subrowid + 1].m_Start;
			int xpos1 = m_SelLeftXPos, xpos2 = m_SelRightXPos;
			wxFileOffset sellen = 0;

			if( m_SelLeftXPos < rowwidth )
			{
				m_Lines->InitNextUChar( firstlit, rowpos );

				do
				{
					int uc = 0x0D;

					if( ( m_Lines->*NextUChar )( ucqueue ) )
						uc = ucqueue.back().first;

					if( uc == 0x0D || uc == 0x0A ) // EOL
					{
						break;
					}

					int ucwidth = GetUCharWidth( uc );

					if( uc == 0x09 )
					{
						int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
						ucwidth = rowwidth - nowxpos;
						tabwidth -= ( nowxpos % tabwidth );

						if( tabwidth < ucwidth )
							ucwidth = tabwidth;
					}

					nowxpos += ucwidth;
					int wchw = ucwidth >> 1;

					if( xpos1 > wchw )
					{
						rowpos += ucqueue.back().second;
						xpos1 -= ucwidth;
						xpos2 -= ucwidth;
					}
					else
					{
						xpos1 = 0;

						if( xpos2 > wchw )
						{
							sellen += ucqueue.back().second;
							xpos2 -= ucwidth;
						}
						else
							xpos2 = 0;
					}
				}
				while( xpos2 > 0 && rowpos < rowendpos );

				if( sellen != 0 )
				{
					MadDeleteUndoData *dudata = new MadDeleteUndoData;
					dudata->m_Pos = pos + rowpos;
					dudata->m_Size = sellen;
#ifdef _DEBUG
					wxASSERT( DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr ) == firstlit );
#else
					DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr );
#endif

					if( undo == nullptr )
					{
						undo = m_UndoBuffer->Add();
						SetNeedSync();
					}

					undo->m_Undos.push_back( dudata );

					if( bColumnEditing )
					{
						if( lastrow < m_SelectionEnd->rowid )
						{
							m_SelectionPos1.pos -= sellen;
						}
					}
					else
						if( bCorrectCaretPos )
						{
							if( lastrow < m_CaretPos.rowid )
							{
								m_SelectionPos2.pos -= sellen;
							}
						}
				}
			}
			else
			{
				xpos1 -= rowwidth;
				rowpos = rowendpos;
			}

			if( bColumnEditing )
			{
				if( lastrow == firstrow )
				{
					m_SelectionPos2.pos = pos + rowpos;
				}

				if( lastrow == m_SelectionEnd->rowid )
				{
					m_SelectionPos1.pos = pos + rowpos;
				}
			}
			else
				if( bCorrectCaretPos )
				{
					if( lastrow == m_CaretPos.rowid )
					{
						m_SelectionPos2.pos = pos + rowpos;
					}
				}

			if( rpos )
			{
				if( subrows )           // adjust rowpos
				{
					size_t idx = rpos->size() - 2;

					for( int i = subrows; i > 0; --i, idx -= 2 )
					{
						( *rpos )[idx] -= sellen;
					}
				}

				rpos->push_back( rowpos );
				rpos->push_back( xpos1 );
			}

			if( lastrow == firstrow )
				break;

			--lastrow;

			// to prev line
			if( subrowid == 0 )
			{
				--firstlit;
				pos -= firstlit->m_Size;
				subrowid = int( firstlit->RowCount() - 1 );
				subrows = 0;
			}
			else
				// to prev row
			{
				--subrowid;
				++subrows;
			}
		}

		if( undo )
		{
			m_Modified = true;
		}

		m_Selection = false;
		m_RepaintAll = true;
		Refresh( false );

		if( bCorrectCaretPos )
		{
			if( undo )
			{
				undo->m_CaretPosBefore = m_CaretPos.pos;
				m_Lines->Reformat( firstlit, lastlit );
			}

			wxASSERT( m_CaretPos.rowid >= m_SelectionBegin->rowid
					  && m_CaretPos.rowid <= m_SelectionEnd->rowid );

			if( bColumnEditing )
			{
				if( m_SelectionPos1.pos == m_SelectionPos2.pos )
				{
					bColumnEditing = false;
					m_SelectionBegin = &m_SelectionPos2;
				}
				else
				{
					m_SelectionBegin = &m_SelectionPos1;
				}
			}
			else
			{
				m_SelectionBegin = &m_SelectionPos2;
			}

			for( ;; )
			{
				m_SelectionBegin->linepos = m_SelectionBegin->pos;
				m_SelectionBegin->lineid = GetLineByPos( m_SelectionBegin->iter, m_SelectionBegin->linepos, m_SelectionBegin->rowid );
				m_SelectionBegin->linepos = m_SelectionBegin->pos - m_SelectionBegin->linepos;
				m_SelectionBegin->subrowid = 0;
				MadRowIndexIterator riter = m_SelectionBegin->iter->m_RowIndices.begin() + 1;

				while( m_SelectionBegin->subrowid + 1 < int( m_SelectionBegin->iter->RowCount() ) && riter->m_Start <= m_SelectionBegin->linepos )
				{
					++m_SelectionBegin->subrowid;
					++m_SelectionBegin->rowid;
					++riter;
				}

				--riter;
				m_SelectionBegin->pos -= ( m_SelectionBegin->linepos - riter->m_Start );
				m_SelectionBegin->linepos = riter->m_Start;
				UpdateCaretByXPos( m_SelLeftXPos, *m_SelectionBegin, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );

				if( bColumnEditing == false || m_SelectionBegin == &m_SelectionPos2 )
					break;

				m_Selection = true;
				m_SelectionBegin = &m_SelectionPos2;
			}

			m_CaretPos = m_SelectionPos2;

			if( bColumnEditing )
			{
				m_SelectionBegin = &m_SelectionPos2;
				m_SelectionEnd = &m_SelectionPos1;

				if( m_SelectionPos1.xpos < m_SelectionPos2.xpos )
				{
					m_SelLeftXPos = m_SelectionPos1.xpos;
					m_SelRightXPos = m_SelectionPos2.xpos;
				}
				else
				{
					m_SelLeftXPos = m_SelectionPos2.xpos;
					m_SelRightXPos = m_SelectionPos1.xpos;
				}
			}

			AppearCaret();
			UpdateScrollBarPos();
			m_LastCaretXPos = m_CaretPos.xpos;
			DoSelectionChanged();

			if( undo )
			{
				undo->m_CaretPosAfter = m_CaretPos.pos;
				DoStatusChanged();
			}
		}

		return undo;
	}

	// TextMode & HexMode
	MadDeleteUndoData *dudata = new MadDeleteUndoData;
	dudata->m_Pos = m_SelectionBegin->pos;
	dudata->m_Size = m_SelectionEnd->pos - m_SelectionBegin->pos;
	MadLineIterator lit = DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr );
	MadUndo *undo = m_UndoBuffer->Add();
	SetNeedSync();
	undo->m_CaretPosBefore = m_CaretPos.pos;
	undo->m_CaretPosAfter = m_SelectionBegin->pos;
	undo->m_Undos.push_back( dudata );
	bool sc = ( oldModified == false );
	m_Modified = true;
	m_Selection = false;
	m_RepaintAll = true;
	Refresh( false );

	if( IsTextFile() )
	{
		m_Lines->Reformat( lit, lit );
		m_CaretPos.pos = m_SelectionBegin->pos;
		UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		AppearCaret();
		UpdateScrollBarPos();

		if( m_EditMode == emHexMode )
		{
			if( !m_CaretAtHexArea )
			{
				UpdateTextAreaXPos();
				m_LastTextAreaXPos = m_TextAreaXPos;
			}
		}
	}
	else
	{
		m_CaretPos.pos = m_SelectionBegin->pos;
		m_CaretPos.linepos = m_SelectionBegin->pos;
		AppearCaret();
		UpdateScrollBarPos();

		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
			m_LastTextAreaXPos = m_TextAreaXPos;
		}
	}

	m_LastCaretXPos = m_CaretPos.xpos;
	DoSelectionChanged();

	if( sc ) DoStatusChanged();

	return undo;
}

void MadEdit::InsertHexChar( int hc ) // handle input in hexarea
{
	wxASSERT( 0 <= hc && hc <= 15 );
	wxASSERT( m_EditMode == emHexMode && m_CaretAtHexArea );

	if( IsReadOnly() )
		return;

	bool oldModified = m_Modified;

	if( ( m_CaretPos.pos == m_Lines->m_Size || m_InsertMode ) && !m_CaretAtHalfByte ) // insert at end
	{
		MadBlock blk( m_Lines->m_MemData, 0, 1 );
		wxByte b = hc << 4;
		blk.m_Pos = m_Lines->m_MemData->Put( &b, 1 );
		MadInsertUndoData *insud = new MadInsertUndoData;
		insud->m_Pos = m_CaretPos.pos;
		insud->m_Size = 1;
		insud->m_Data.push_back( blk );
		MadUndo *undo = m_UndoBuffer->Add();
		SetNeedSync();
		undo->m_CaretPosBefore = undo->m_CaretPosAfter = m_CaretPos.pos;
		undo->m_Undos.push_back( insud );
		MadLineIterator lit = DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );
		m_CaretAtHalfByte = true;
		m_RepaintAll = true;

		if( IsTextFile() )
		{
			m_Lines->Reformat( lit, lit );
			UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			AppearCaret();
			UpdateScrollBarPos();
		}
		else
		{
			AppearCaret();
			UpdateScrollBarPos();
		}

		bool sc = ( oldModified == false );
		m_Modified = true;
		Refresh( false );
		DoSelectionChanged();

		if( sc ) DoStatusChanged();
	}
	else
	{
		MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
		oudata->m_Pos = m_CaretPos.pos;
		oudata->m_DelSize = 1;
		oudata->m_InsSize = 1;
		MadBlock blk( m_Lines->m_MemData, 0, 1 );
		wxByte b = m_CaretPos.iter->Get( m_CaretPos.linepos );

		if( m_CaretAtHalfByte )
		{
			b = ( b & 0xF0 ) | hc;
			++m_CaretPos.pos;
			m_CaretAtHalfByte = false;
		}
		else
		{
			b = ( hc << 4 ) | ( b & 0x0F );
			m_CaretAtHalfByte = true;
		}

		blk.m_Pos = m_Lines->m_MemData->Put( &b, 1 );
		oudata->m_InsData.push_back( blk );
		MadLineIterator lit = DeleteInsertData( oudata->m_Pos,
												oudata->m_DelSize, &oudata->m_DelData,
												oudata->m_InsSize, &oudata->m_InsData );
		MadUndo *undo = m_UndoBuffer->Add();
		SetNeedSync();
		undo->m_CaretPosBefore = undo->m_CaretPosAfter = m_CaretPos.pos;

		if( m_CaretAtHalfByte == false ) --undo->m_CaretPosBefore;

		undo->m_Undos.push_back( oudata );
		bool sc = ( oldModified == false );
		m_Modified = true;
		m_Selection = false;
		m_RepaintAll = true;
		Refresh( false );

		if( IsTextFile() )
		{
			m_Lines->Reformat( lit, lit );
			UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			AppearCaret();
			UpdateScrollBarPos();
		}
		else
		{
			m_CaretPos.linepos = m_CaretPos.pos;
			AppearCaret();
			UpdateScrollBarPos();
		}

		DoSelectionChanged();

		if( sc ) DoStatusChanged();
	}
}

void MadEdit::InsertHexData( wxByte *hex, size_t count )
{
	MadBlock blk( m_Lines->m_MemData, 0, count );
	blk.m_Pos = m_Lines->m_MemData->Put( hex, count );

	m_ZeroSelection = false;
	if( m_Selection || !m_InsertMode )
	{
		MadOverwriteUndoData *oudata = new MadOverwriteUndoData();

		if( !m_Selection )
		{
			wxFileOffset leftsize = m_Lines->m_Size - m_CaretPos.pos;
			oudata->m_Pos = m_CaretPos.pos;
			oudata->m_DelSize = ( leftsize < blk.m_Size ) ? leftsize : blk.m_Size;
		}
		else
		{
			oudata->m_Pos = m_SelectionBegin->pos;

			if( m_InsertMode )
			{
				oudata->m_DelSize = m_SelectionEnd->pos - m_SelectionBegin->pos;
			}
			else
			{
				wxFileOffset leftsize = m_Lines->m_Size - m_SelectionBegin->pos;
				oudata->m_DelSize = ( leftsize < blk.m_Size ) ? leftsize : blk.m_Size;
			}
		}

		oudata->m_InsSize = blk.m_Size;
		oudata->m_InsData.push_back( blk );
		MadLineIterator lit = DeleteInsertData( oudata->m_Pos,
												oudata->m_DelSize, &oudata->m_DelData,
												oudata->m_InsSize, &oudata->m_InsData );
		MadUndo *undo = m_UndoBuffer->Add();
		SetNeedSync();
		undo->m_CaretPosBefore = m_CaretPos.pos;
		undo->m_CaretPosAfter = oudata->m_Pos + oudata->m_InsSize;
		undo->m_Undos.push_back( oudata );
		m_Modified = true;
		m_Selection = false;
		m_RepaintAll = true;
		Refresh( false );

		if( IsTextFile() )
		{
			m_Lines->Reformat( lit, lit );
			m_CaretPos.pos = undo->m_CaretPosAfter;
			UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			AppearCaret();
			UpdateScrollBarPos();
		}
		else
		{
			m_CaretPos.pos = undo->m_CaretPosAfter;
			m_CaretPos.linepos = m_CaretPos.pos;
			AppearCaret();
			UpdateScrollBarPos();
		}

		DoSelectionChanged();
		DoStatusChanged();
	}
	else
	{
		MadInsertUndoData *insud = new MadInsertUndoData;
		insud->m_Pos = m_CaretPos.pos;
		insud->m_Size = blk.m_Size;
		insud->m_Data.push_back( blk );
		MadUndo *undo = m_UndoBuffer->Add();
		SetNeedSync();
		undo->m_CaretPosBefore = m_CaretPos.pos;
		undo->m_CaretPosAfter = m_CaretPos.pos + blk.m_Size;
		undo->m_Undos.push_back( insud );
		MadLineIterator lit = DeleteInsertData( insud->m_Pos, 0, nullptr, insud->m_Size, &insud->m_Data );
		m_RepaintAll = true;

		if( IsTextFile() )
		{
			m_Lines->Reformat( lit, lit );
			m_CaretPos.pos += blk.m_Size;
			UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			AppearCaret();
			UpdateScrollBarPos();
		}
		else
		{
			m_CaretPos.pos += blk.m_Size;
			m_CaretPos.linepos = m_CaretPos.pos;
			AppearCaret();
			UpdateScrollBarPos();
		}

		m_Modified = true;
		Refresh( false );
		DoSelectionChanged();
		DoStatusChanged();
	}
}

// overwrite data in a single Undo
void MadEdit::OverwriteDataSingle( vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
								   vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
								   vector<wxFileOffset> &ins_len )
{
	bool oldModified = m_Modified;
	MadMemData *md = m_Lines->m_MemData;
	MadBlock blk( md, md->m_Size, 0 );
	size_t count = del_bpos.size();
	size_t idx = 0;
	vector<wxFileOffset>::iterator  del_bpos_it = del_bpos.begin();
	vector<wxFileOffset>::iterator  del_epos_it = del_epos.begin();
	vector<const ucs4_t*>::iterator ins_ucs_it;
	vector<wxByte*>::iterator ins_data_it;

	if( ins_ucs ) ins_ucs_it = ins_ucs->begin();
	else        ins_data_it = ins_data->begin();

	vector<wxFileOffset>::iterator ins_len_it = ins_len.begin();
	MadLineIterator lit;
	wxFileOffset pos = -1, lpos = -1, newCaretPos = m_CaretPos.pos;
	wxFileOffset oldblksize = 0, caretdiffsize = 0, del_size = 0;
	int rowid = -1;
	vector<wxByte> buffer;
	buffer.resize( 10240 );
	wxByte *buf = &buffer[0];

	do
	{
		if( idx != 0 )
		{
			pos = *del_epos_it++;
			wxFileOffset size = *del_bpos_it - pos;

			if( size > 0 )
			{
				m_UpdateValidPos = -1;
				lpos = pos;
				GetLineByPos( lit, lpos, rowid );
				m_UpdateValidPos = 0;
				lpos = pos - lpos;

				for( ;; )
				{
					size_t len = 256 * 1024;

					if( len > (size_t)size ) len = (size_t)size;

					wxFileOffset ll = lit->m_Size - lpos;

					if( len > (size_t)ll ) len = (size_t)ll;

					if( len > buffer.size() )
					{
						buffer.resize( len );
						buf = &buffer[0];
					}

					lit->Get( lpos, buf, len ); // put LineData to BlockData
					md->Put( buf, len );
					blk.m_Size += len;

					if( ( size -= len ) == 0 ) break;

					++lit;          // to next line
					lpos = 0;
				}
			}
		}

		if( ( caretdiffsize = m_CaretPos.pos - *del_bpos_it ) > 0 )
		{
			del_size = *del_epos_it - *del_bpos_it;
		}

		oldblksize = blk.m_Size;

		// add ins_data
		if( ins_ucs )
		{
			if( *ins_len_it != 0 )
			{
				UCStoBlock( *ins_ucs_it, *ins_len_it, blk );
				*ins_len_it = blk.m_Size - oldblksize; // get byte-length of ucs
			}

			++ins_ucs_it;
		}
		else
		{
			if( *ins_len_it != 0 )
			{
				md->Put( *ins_data_it, *ins_len_it );
				blk.m_Size += *ins_len_it;
			}

			++ins_data_it;
		}

		++ins_len_it;

		// adjust caretpos
		if( caretdiffsize > 0 )
		{
			wxFileOffset ins_size = blk.m_Size - oldblksize;

			if( caretdiffsize >= del_size ) // not in the del-data
			{
				newCaretPos = newCaretPos - del_size + ins_size;
			}
			else        // in the del-data
			{
				if( ins_size < caretdiffsize )
				{
					newCaretPos -= ( caretdiffsize - ins_size );
				}
			}
		}

		++del_bpos_it;
	}
	while( ++idx < count );

	MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
	oudata->m_Pos = del_bpos.front();
	oudata->m_DelSize = del_epos.back() - oudata->m_Pos;
	oudata->m_InsSize = blk.m_Size;
	oudata->m_InsData.push_back( blk );
	lit = DeleteInsertData( oudata->m_Pos, oudata->m_DelSize, &oudata->m_DelData,
							oudata->m_InsSize, &oudata->m_InsData );
	MadUndo *undo = m_UndoBuffer->Add();
	SetNeedSync();
	undo->m_CaretPosBefore = m_CaretPos.pos;
	undo->m_CaretPosAfter = newCaretPos;
	undo->m_Undos.push_back( oudata );
	bool sc = ( oldModified == false );
	m_Modified = true;
	m_Selection = false;
	m_ZeroSelection = false;
	m_RepaintAll = true;
	Refresh( false );

	if( IsTextFile() )
	{
		m_Lines->Reformat( lit, lit );
		m_CaretPos.pos = newCaretPos;
		UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		//AppearCaret();
		UpdateScrollBarPos();

		if( m_EditMode == emHexMode )
		{
			if( !m_CaretAtHexArea )
			{
				UpdateTextAreaXPos();
				m_LastTextAreaXPos = m_TextAreaXPos;
			}
		}
	}
	else
	{
		m_CaretPos.pos = undo->m_CaretPosAfter;
		m_CaretPos.linepos = m_CaretPos.pos;
		//AppearCaret();
		UpdateScrollBarPos();

		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
			m_LastTextAreaXPos = m_TextAreaXPos;
		}
	}

	m_LastCaretXPos = m_CaretPos.xpos;
	DoSelectionChanged();

	if( sc ) DoStatusChanged();

	if( FindFocus() != this )
		DisplayCaret( true );
}

// overwrite data in multiple Undos
void MadEdit::OverwriteDataMultiple( vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
									 vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
									 vector<wxFileOffset> &ins_len )
{
	bool oldModified = m_Modified;
	MadMemData *md = m_Lines->m_MemData;
	size_t count = del_bpos.size();
	size_t idx = 0;
	vector<wxFileOffset>::iterator  del_bpos_it = del_bpos.end();
	vector<wxFileOffset>::iterator  del_epos_it = del_epos.end();
	vector<const ucs4_t*>::iterator ins_ucs_it;
	vector<wxByte*>::iterator ins_data_it;

	if( ins_ucs ) ins_ucs_it = ins_ucs->end();
	else        ins_data_it = ins_data->end();

	vector<wxFileOffset>::iterator ins_len_it = ins_len.end();
	MadLineIterator litfirst, litlast;
	wxFileOffset newCaretPos = m_CaretPos.pos;
	wxFileOffset caretdiffsize;
	MadUndo *undo = m_UndoBuffer->Add();
	SetNeedSync();

	do
	{
		MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
		oudata->m_Pos = *--del_bpos_it;
		oudata->m_DelSize = ( *--del_epos_it ) - oudata->m_Pos;
		MadBlock blk( md, -1, 0 );

		if( ins_ucs )
		{
			--ins_ucs_it;

			if( *--ins_len_it != 0 )
			{
				UCStoBlock( *ins_ucs_it, *ins_len_it, blk );
				*ins_len_it = blk.m_Size; // get byte-length of ucs
			}
		}
		else
		{
			--ins_data_it;

			if( ( blk.m_Size = *--ins_len_it ) != 0 )
			{
				blk.m_Pos = md->Put( *ins_data_it, blk.m_Size );
			}
		}

		oudata->m_InsSize = blk.m_Size;
		oudata->m_InsData.push_back( blk );

		if( idx == 0 )
		{
			litfirst = litlast = DeleteInsertData( oudata->m_Pos,
												   oudata->m_DelSize, &oudata->m_DelData,
												   oudata->m_InsSize, &oudata->m_InsData );
		}
		else
		{
			litfirst = DeleteInsertData( oudata->m_Pos,
										 oudata->m_DelSize, &oudata->m_DelData,
										 oudata->m_InsSize, &oudata->m_InsData );
		}

		undo->m_Undos.push_back( oudata );

		// adjust caretpos
		if( ( caretdiffsize = m_CaretPos.pos - oudata->m_Pos ) > 0 )
		{
			if( caretdiffsize >= oudata->m_DelSize )
			{
				newCaretPos = newCaretPos - oudata->m_DelSize + oudata->m_InsSize;
			}
			else
			{
				if( oudata->m_InsSize < caretdiffsize )
				{
					newCaretPos -= ( caretdiffsize - oudata->m_InsSize );
				}
			}
		}
	}
	while( ++idx < count );

	undo->m_CaretPosBefore = m_CaretPos.pos;
	undo->m_CaretPosAfter = newCaretPos;
	bool sc = ( oldModified == false );
	m_Modified = true;
	m_Selection = false;
	m_ZeroSelection = false;
	m_RepaintAll = true;
	Refresh( false );

	if( IsTextFile() )
	{
		m_Lines->Reformat( litfirst, litlast );
		m_CaretPos.pos = newCaretPos;
		UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
		//AppearCaret();
		UpdateScrollBarPos();

		if( m_EditMode == emHexMode )
		{
			if( !m_CaretAtHexArea )
			{
				UpdateTextAreaXPos();
				m_LastTextAreaXPos = m_TextAreaXPos;
			}
		}
	}
	else
	{
		m_CaretPos.pos = undo->m_CaretPosAfter;
		m_CaretPos.linepos = m_CaretPos.pos;
		//AppearCaret();
		UpdateScrollBarPos();

		if( !m_CaretAtHexArea )
		{
			UpdateTextAreaXPos();
			m_LastTextAreaXPos = m_TextAreaXPos;
		}
	}

	m_LastCaretXPos = m_CaretPos.xpos;
	DoSelectionChanged();

	if( sc ) DoStatusChanged();

	if( FindFocus() != this )
		DisplayCaret( true );
}

void MadEdit::FindLeftBrace( int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi )  // find by bpi.BraceIndex
{
	wxASSERT( m_Syntax->m_SynAttr->m_LeftBrace.size() != 0 );
	vector <BracePairIndex>::iterator bit = lit->m_BracePairIndices.end();
	size_t bcount = lit->m_BracePairIndices.size();

	// seek for the nearest bpi
	if( bcount != 0 )
	{
		do
		{
			--bit;

			if( bit->LinePos <= linepos )
			{
				break; // found
			}
		}
		while( --bcount > 0 );
	}

	/* example:
	   {   {  (  {  }  )  }     <-- seek direction
	   a-1 a0 b0 a1 a2 b1 a1
	   ^found
	*/
	vector< int > StackVector;

	if( bpi.BraceIndex < 0 )
	{
		StackVector.resize( m_Syntax->m_SynAttr->m_LeftBrace.size() );
	}
	else
	{
		StackVector.resize( 1 ); // seek the same BraceIndex only
	}

	for( ;; ) // for every bpi
	{
		if( bcount == 0 )
		{
			for( ;; )
			{
				if( rowid == 0 ) // at begin of file
				{
					rowid = -1;
					return;
				}

				// to prev line
				--lit;
				rowid -= int( lit->RowCount() );
				bcount = lit->m_BracePairIndices.size();

				if( bcount != 0 )
				{
					break;
				}
			}

			bit = lit->m_BracePairIndices.end() - 1;
		}

		if( bpi.BraceIndex < 0 ) // find the nearest unary-left brace
		{
			if( bit->LeftPair )
			{
				if( StackVector[bit->BraceIndex] == 0 ) // found
				{
					break;
				}

				--StackVector[bit->BraceIndex];
			}
			else
			{
				++StackVector[bit->BraceIndex];
			}
		}
		else // find the bit that has the same BraceIndex
		{
			if( bit->BraceIndex == bpi.BraceIndex )
			{
				if( bit->LeftPair )
				{
					if( StackVector[0] == 0 ) // found
					{
						break;
					}

					--StackVector[0];
				}
				else
				{
					++StackVector[0];
				}
			}
		}

		if( --bcount != 0 )
		{
			--bit;
		}
	}

	// found
	bpi = *bit;
	// set rowid
	int subrowid = 1;

	while( lit->m_RowIndices[subrowid].m_Start <= bpi.LinePos )
	{
		++subrowid;
	}

	rowid += ( subrowid - 1 );
}

void MadEdit::FindRightBrace( int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi )
{
	wxASSERT( m_Syntax->m_SynAttr->m_LeftBrace.size() != 0 );
	vector <BracePairIndex>::iterator bit = lit->m_BracePairIndices.begin();
	size_t bcount = lit->m_BracePairIndices.size();

	// seek for the nearest bpi
	if( bcount != 0 )
	{
		do
		{
			if( bit->LinePos >= linepos )
			{
				break; // found
			}

			++bit;
		}
		while( --bcount > 0 );
	}

	/* example:
	  seek direction -->  {  (  {  }  )  }  }
	                      a1 b1 a2 a1 b0 a0 a-1
	                                        ^found
	*/
	vector< int > StackVector;

	if( bpi.BraceIndex < 0 )
	{
		StackVector.resize( m_Syntax->m_SynAttr->m_LeftBrace.size() );
	}
	else
	{
		StackVector.resize( 1 ); // seek the same BraceIndex only
	}

	for( ;; ) // for every bpi
	{
		if( bcount == 0 )
		{
			for( ;; )
			{
				// to next line
				rowid += int( lit->RowCount() );

				if( rowid == int( m_Lines->m_RowCount ) ) // at end of file
				{
					rowid = -1;
					return;
				}

				++lit;
				bcount = lit->m_BracePairIndices.size();

				if( bcount != 0 )
				{
					break;
				}
			}

			bit = lit->m_BracePairIndices.begin();
		}

		if( bpi.BraceIndex < 0 ) // find the nearest unary-right brace
		{
			if( bit->LeftPair )
			{
				++StackVector[bit->BraceIndex];
			}
			else
			{
				if( StackVector[bit->BraceIndex] == 0 ) // found
				{
					break;
				}

				--StackVector[bit->BraceIndex];
			}
		}
		else // find the bit that has the same BraceIndex
		{
			if( bit->BraceIndex == bpi.BraceIndex )
			{
				if( bit->LeftPair )
				{
					++StackVector[0];
				}
				else
				{
					if( StackVector[0] == 0 ) // found
					{
						break;
					}

					--StackVector[0];
				}
			}
		}

		if( --bcount != 0 )
		{
			++bit;
		}
	}

	// found
	bpi = *bit;
	// set rowid
	int subrowid = 1;

	while( lit->m_RowIndices[subrowid].m_Start <= bpi.LinePos )
	{
		++subrowid;
	}

	rowid += ( subrowid - 1 );
}

void MadEdit::FindBracePairUnderCaretPos()
{
	if( m_Syntax->m_SynAttr->m_LeftBrace.empty() )
	{
		m_LeftBrace_rowid = -1;
		m_RightBrace_rowid = -1;
		return;
	}

	// check current caretpos is under a brace
	vector <BracePairIndex>::iterator bit = m_CaretPos.iter->m_BracePairIndices.begin();
	vector <BracePairIndex>::iterator bitend = m_CaretPos.iter->m_BracePairIndices.end();
	wxFileOffset &linepos = m_CaretPos.linepos;
	wxFileOffset rightpos = -1;

	while( bit != bitend )
	{
		rightpos = bit->LinePos + bit->Length;

		if( linepos >= bit->LinePos && linepos < rightpos ) // found bit
		{
			break;
		}
		else
			if( linepos == rightpos ) // under BracePos+1
			{
				vector <BracePairIndex>::iterator bit1 = bit + 1;

				if( bit1 != bitend && linepos == bit1->LinePos ) // found bit1
				{
					bit = bit1;
					break;
				}
				else    // found bit
				{
					break;
				}
			}
			else
				if( bit->LinePos > linepos ) // not found
				{
					bit = bitend;
					break;
				}

		++bit;
	}

	if( bit == bitend ) // not found
	{
		m_LeftBrace_rowid = -1;
		m_RightBrace_rowid = -1;
		return;
	}

	if( bit->LeftPair )
	{
		m_LeftBrace_rowid = m_CaretPos.rowid;

		if( bit->LinePos == m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid + 1].m_Start ) // && m_CaretPos.subrowid+1<mm_CaretPos.iter->RowCount())
		{
			++m_LeftBrace_rowid;   // at begin of next row
		}
		else
			if( rightpos == m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid].m_Start )
			{
				wxASSERT( m_CaretPos.subrowid > 0 );
				--m_LeftBrace_rowid;
			}

		m_LeftBrace = *bit;
		m_RightBrace.BraceIndex = bit->BraceIndex;
		MadLineIterator lit = m_CaretPos.iter;
		wxFileOffset lpos = m_LeftBrace.LinePos + m_LeftBrace.Length;
		int srid = m_CaretPos.subrowid + 1;

		if( lpos >= lit->m_RowIndices[srid].m_Start ) // to next row
		{
			if( srid == int( lit->RowCount() ) )
			{
				if( m_CaretPos.lineid == int( m_Lines->m_LineCount - 1 ) ) // at end of file
				{
					m_RightBrace_rowid = -1;
					return;
				}
				else // to next line
				{
					m_RightBrace_rowid = m_CaretPos.rowid + 1;
					++lit;
					lpos = 0;
					FindRightBrace( m_RightBrace_rowid, lit, lpos, m_RightBrace );
				}
			}
			else // to next row
			{
				m_RightBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
				FindRightBrace( m_RightBrace_rowid, lit, lpos, m_RightBrace );
			}
		}
		else
		{
			m_RightBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
			FindRightBrace( m_RightBrace_rowid, lit, lpos, m_RightBrace );
		}
	}
	else
	{
		m_RightBrace_rowid = m_CaretPos.rowid;

		if( bit->LinePos == m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid + 1].m_Start ) // && m_CaretPos.subrowid+1<mm_CaretPos.iter->RowCount())
		{
			++m_RightBrace_rowid;   // at begin of next row
		}
		else
			if( rightpos == m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid].m_Start )
			{
				wxASSERT( m_CaretPos.subrowid > 0 );
				--m_RightBrace_rowid;
			}

		m_RightBrace = *bit;
		m_LeftBrace.BraceIndex = bit->BraceIndex;
		MadLineIterator lit = m_CaretPos.iter;
		wxFileOffset lpos = m_RightBrace.LinePos;

		if( lpos == lit->m_RowIndices[m_CaretPos.subrowid].m_Start ) // at begin of row
		{
			if( m_CaretPos.subrowid == 0 )
			{
				if( m_CaretPos.lineid == 0 ) // at begin of file
				{
					m_LeftBrace_rowid = -1;
				}
				else // to prev line
				{
					--lit;
					lpos = lit->m_Size;
					m_LeftBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid - int( lit->RowCount() );
					FindLeftBrace( m_LeftBrace_rowid, lit, lpos, m_LeftBrace );
				}
			}
			else // to prev row
			{
				--lpos;
				m_LeftBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
				FindLeftBrace( m_LeftBrace_rowid, lit, lpos, m_LeftBrace );
			}
		}
		else // to prev lpos
		{
			--lpos;
			m_LeftBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
			FindLeftBrace( m_LeftBrace_rowid, lit, lpos, m_LeftBrace );
		}
	}
}

void MadEdit::ProcessCommand( MadEditCommand command )
{
	ucs4_t NewAutoCompleteRightChar = 0;
	m_ZeroSelection = false;

	if( command == m_AutoCompleteRightChar && m_CaretPos.pos == m_AutoCompletePos && IsTextMode() )
	{
		// if the previous action is that insert a AutoCompleteLeftChar,
		// and the following action is that insert a AutoCompleteRightChar,
		// replace the action to ecRight(just move caret right a char)
		command = ecRight;
	}
	else
		if( command == ecMouseNotify )
		{
			m_AutoCompleteRightChar = 0;
			return;
		}

	bool bSel = command > ecSelCommandFirst && command < ecSelCommandLast;
	wxFileOffset oldCaretPos = m_CaretPos.pos;

	switch( command )
	{
	case ecHighlightWords:
		{
			HighlightWords();
		}
		break;

	case ecSelectAll:
		{
			SelectAll();
		}
		break;

	case ecCut:
		{
			CutToClipboard();
		}
		break;

	case ecCopy:
		{
			CopyToClipboard();
		}
		break;

	case ecPaste:
		{
			PasteFromClipboard();
		}
		break;

	case ecTextMode:
		{
			SetEditMode( emTextMode );
		}
		break;

	case ecColumnMode:
		{
			SetEditMode( emColumnMode );
		}
		break;

	case ecHexMode:
		{
			SetEditMode( emHexMode );
		}
		break;

	case ecNoWrap:
		{
			SetWordWrapMode( wwmNoWrap );
		}
		break;

	case ecWrapByWindow:
		{
			SetWordWrapMode( wwmWrapByWindow );
		}
		break;

	case ecWrapByColumn:
		{
			SetWordWrapMode( wwmWrapByColumn );
		}
		break;

	case ecUndo:
		{
			Undo();
		}
		break;

	case ecRedo:
		{
			Redo();
		}
		break;

	case ecScrollLineUp:
		{
			--m_TopRow;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecScrollLineDown:
		{
			++m_TopRow;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecScrollPageUp:
		{
			m_TopRow -= m_PageRowCount;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecScrollPageDown:
		{
			m_TopRow += m_PageRowCount;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecScrollLeft:
		{
			m_DrawingXPos -= 25;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecScrollRight:
		{
			m_DrawingXPos += 25;
			UpdateScrollBarPos();
			m_RepaintAll = true;
			Refresh( false );
			NewAutoCompleteRightChar = m_AutoCompleteRightChar;
		}
		break;

	case ecToUpperCase:
		{
			ToUpperCase();
		}
		break;

	case ecToLowerCase:
		{
			ToLowerCase();
		}
		break;

	case ecInvertCase:
		{
			InvertCase();
		}
		break;

	case ecToHalfWidth:
		{
			ToHalfWidth();
		}
		break;

	case ecToFullWidth:
		{
			ToFullWidth();
		}
		break;

	default:
		if( m_EditMode != emHexMode )
		{
			if( bSel )
				BeginUpdateSelection();

			if( command >= ecCharFirst && command <= ecCharLast )
			{
				ucs4_t uc = command;
				// check for AutoCompletePair
				int idx;

				if( m_AutoCompletePair && IsTextMode() && ( idx = m_Syntax->m_SynAttr->m_AutoCompleteLeftChar.Find( wxChar( uc ) ) ) >= 0 )
				{
					// insert the AutoCompleteLeftChar
					if((!m_SingleLineMode) && IsMacroRecording())
						RecordAsMadMacro( this, wxString::Format( wxT( "%c" ), command ), true );
					bool inssel = (m_Selection && m_InsertPairForSelection);
					long sellen = 0;
					if(inssel)
					{
						sellen = m_SelectionEnd->pos - m_SelectionBegin->pos;
					}
					InsertString( &uc, 1, true, true, false, inssel );

					MadCaretPos *send;

					if( m_Selection )
					{
						send = m_SelectionEnd;
					}
					else
					{
						send = &m_CaretPos;
					}

					// check the right char of send is not a normal char,
					// if yes, insert the AutoCompleteRightChar
					m_Lines->InitNextUChar( send->iter, send->linepos );
					MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
					MadUCQueue ucqueue;
					ucs4_t ucb;

					if( inssel || ( m_Lines->*NextUChar )( ucqueue ) == false || ( ucb = ucqueue.back().first ) <= 0x20 || m_Syntax->IsDelimiter( ucb ) )
					{
						if (inssel)
						{
							SetCaretPosition(m_CaretPos.pos + sellen);
						}
						NewAutoCompleteRightChar = m_Syntax->m_SynAttr->m_AutoCompleteRightChar[idx];
						if((!m_SingleLineMode) && IsMacroRecording())
							RecordAsMadMacro( this, wxString::Format( wxT( "%c" ), ( int )NewAutoCompleteRightChar ), true);
						InsertString( &NewAutoCompleteRightChar, 1, true, false, false );
						
						m_AutoCompletePos = m_CaretPos.pos;
					}
				}
				else
				{
					bool inserted = false;

					// check for m_UnindentChar
					if( IsTextMode() && ( idx = m_Syntax->m_SynAttr->m_UnindentChar.Find( wxChar( uc ) ) ) >= 0 )
					{
						// check if the chars before uc are all spaces
						MadCaretPos *sbeg;

						if( m_Selection )
						{
							sbeg = m_SelectionBegin;
						}
						else
						{
							sbeg = &m_CaretPos;
						}

						wxFileOffset linepos = sbeg->iter->m_RowIndices[0].m_Start;
						m_Lines->InitNextUChar( sbeg->iter, linepos );
						MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
						MadUCQueue ucqueue;
						bool allspace = true;

						while( linepos < sbeg->linepos && ( m_Lines->*NextUChar )( ucqueue ) )
						{
							if( ucqueue.back().first > 0x20 )
							{
								allspace = false;
								break;
							}

							linepos += ucqueue.back().second;
						}

						if( linepos == sbeg->linepos && allspace && sbeg->lineid > 0 )
						{
							vector<ucs4_t> spaces;
							MadLineIterator lit = sbeg->iter;
							GetIndentSpaces( sbeg->lineid - 1, --lit, spaces, true, true );

							if( !m_Selection )
							{
								m_Selection = true;
								m_SelectionPos1 = m_CaretPos;
								m_SelectionPos2 = m_CaretPos;
								m_SelectionBegin = &m_SelectionPos1;
								m_SelectionEnd = &m_SelectionPos2;
							}

							// set the pos to the begin of line
							m_SelectionBegin->rowid -= m_SelectionBegin->subrowid;
							m_SelectionBegin->subrowid = 0;
							wxFileOffset spos = m_SelectionBegin->iter->m_RowIndices[0].m_Start;
							m_SelectionBegin->pos = m_SelectionBegin->pos - m_SelectionBegin->linepos + spos;
							m_SelectionBegin->linepos = spos;
							spaces.push_back( uc );
							if((!m_SingleLineMode) && IsMacroRecording())
							{
								wxString tp;
								for(size_t i = 0; i < spaces.size(); ++i)
								{
									tp << wxChar(spaces[i]);
								}
								RecordAsMadMacro( this, tp, true );
							}
							InsertString( &spaces[0], spaces.size(), true, false, false );
							
							
							inserted = true;
						}
					}

					if( !inserted )
					{
						if((!m_SingleLineMode) && IsMacroRecording())
							RecordAsMadMacro( this, wxString::Format( wxT( "%c" ), ( int )uc ), true );
						InsertString( &uc, 1, true, true, false );
					}
				}
			}
			else
			{
				switch( command )
				{
				case ecLeft:
				case ecSelLeft:
					if( m_Selection && command == ecLeft )
					{
						m_CaretPos = *m_SelectionBegin;
						UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					}
					else
						if( m_CaretPos.extraspaces != 0 )      // ColumnMode
						{
							--m_CaretPos.extraspaces;
							m_CaretPos.xpos -= m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
						}
						else
						{
							if( m_CaretRowUCharPos == 0 ) // at begin-of-row
							{
								if( IsTextMode() )
								{
									if( m_CaretPos.subrowid != 0 )   // to prev subrow
									{
										--m_CaretPos.rowid;
										--m_CaretPos.subrowid;
										UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
									}
									else
										if( m_CaretPos.rowid != 0 ) // to prev line's last row
										{
											MadLineIterator & lit = m_CaretPos.iter;
											--lit;
											--m_CaretPos.lineid;
											m_CaretPos.linepos = lit->m_Size - lit->m_NewLineSize;
											m_CaretPos.pos -= lit->m_NewLineSize;
											--m_CaretPos.rowid;
											m_CaretPos.subrowid = int( lit->RowCount() - 1 );
											UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
										}
								}
							}
							else                // in middle of row
							{
								--m_CaretRowUCharPos;
								MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
								m_CaretPos.pos -= ucp.second;
								m_CaretPos.linepos -= ucp.second;
								m_CaretPos.xpos -= m_ActiveRowWidths[m_CaretRowUCharPos];
							}
						}

					AppearCaret();
					UpdateScrollBarPos();
					m_LastCaretXPos = m_CaretPos.xpos;
					break;

				case ecRight:
				case ecSelRight:
					if( m_Selection && command == ecRight )
					{
						m_CaretPos = *m_SelectionEnd;
						UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					}
					else
						if( m_CaretRowUCharPos == int( m_ActiveRowUChars.size() ) ) // at end-of-row
						{
							if( IsTextMode() )
							{
								MadLineIterator & lit = m_CaretPos.iter;

								if( m_CaretPos.subrowid + 1 < int( lit->RowCount() ) )  // to next subrow
								{
									++m_CaretPos.rowid;
									++m_CaretPos.subrowid;
									UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
								}
								else
									if( m_CaretPos.rowid < int( m_Lines->m_RowCount - 1 ) ) //to next line
									{
										m_CaretPos.pos += lit->m_NewLineSize;
										++lit;
										++m_CaretPos.lineid;
										m_CaretPos.linepos = 0;
										++m_CaretPos.rowid;
										m_CaretPos.subrowid = 0;
										UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
									}
							}
							else                // ColumnMode
							{
								if( m_CaretPos.xpos < m_MaxColumnRowWidth )
								{
									//if(m_CaretPos.subrowid == m_CaretPos.iter->RowCount() - 1)
									{
										++m_CaretPos.extraspaces;
										m_CaretPos.xpos += m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
									}
								}
							}
						}
						else
						{
							MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
							m_CaretPos.xpos += m_ActiveRowWidths[m_CaretRowUCharPos];
							++m_CaretRowUCharPos;
							m_CaretPos.pos += ucp.second;
							m_CaretPos.linepos += ucp.second;
						}

					AppearCaret();
					UpdateScrollBarPos();
					m_LastCaretXPos = m_CaretPos.xpos;
					break;

				case ecUp:
					if( m_Selection )
						m_CaretPos = *m_SelectionBegin;

				case ecSelUp:
					if( m_CaretPos.rowid > 0 )
					{
						if( m_CaretPos.subrowid > 0 ) // to prev subrow
						{
							--m_CaretPos.rowid;
							--m_CaretPos.subrowid;
							MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
							std::advance( riter, m_CaretPos.subrowid );
							m_CaretPos.pos -= ( m_CaretPos.linepos - riter->m_Start );
							m_CaretPos.linepos = riter->m_Start;
							UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						}
						else                // to prev line's last row
						{
							--m_CaretPos.rowid;
							--m_CaretPos.lineid;
							MadLineIterator & lit = m_CaretPos.iter;
							--lit;
							m_CaretPos.subrowid = int( lit->RowCount() - 1 );
							MadRowIndexIterator riter = lit->m_RowIndices.begin();
							std::advance( riter, m_CaretPos.subrowid );
							m_CaretPos.pos -= ( m_CaretPos.linepos + ( lit->m_Size - riter->m_Start ) );
							m_CaretPos.linepos = riter->m_Start;
							UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						}
					}

					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecDown:
					if( m_Selection )
						m_CaretPos = *m_SelectionEnd;

				case ecSelDown:
					if( m_CaretPos.rowid < int( m_Lines->m_RowCount - 1 ) ) //to next line/row
					{
						MadLineIterator & lit = m_CaretPos.iter;

						if( m_CaretPos.subrowid + 1 < int( lit->RowCount() ) )    // to next subrow
						{
							++m_CaretPos.rowid;
							++m_CaretPos.subrowid;
							MadRowIndexIterator riter = lit->m_RowIndices.begin();
							std::advance( riter, m_CaretPos.subrowid );
							// set linepos pos for begin of row
							m_CaretPos.pos += ( riter->m_Start - m_CaretPos.linepos );
							m_CaretPos.linepos = riter->m_Start;
							UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						}
						else                // to next line
						{
							MadRowIndexIterator riter = lit->m_RowIndices.begin();
							std::advance( riter, m_CaretPos.subrowid + 1 );
							// set linepos pos for begin of line
							m_CaretPos.pos +=
								( riter->m_Start - m_CaretPos.linepos ) + lit->m_NewLineSize;
							m_CaretPos.linepos = 0;
							++lit;
							++m_CaretPos.lineid;
							++m_CaretPos.rowid;
							m_CaretPos.subrowid = 0;
							UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						}
					}

					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecBeginLine:
				case ecSelBeginLine:
					{
						// get first non-space char
						int charpos = 0;
						int xpos = 0;
						wxFileOffset len = 0;

						while( charpos < int( m_ActiveRowUChars.size() ) )
						{
							MadUCPair & ucp = m_ActiveRowUChars[charpos];
							int uc = ucp.first;

							if( uc != 0x20 && uc != 0x09 && uc != 0x3000 )    // fullwidth space char
								break;

							xpos += m_ActiveRowWidths[charpos];
							len += ucp.second;
							++charpos;
						}

						if( charpos == int( m_ActiveRowUChars.size() ) && charpos != 0 )
						{
							charpos = 0;
							xpos = 0;
							len = 0;
						}

						if( m_CaretRowUCharPos == 0 )
						{
							if( m_CaretPos.extraspaces == 0 )
							{
								if( charpos != 0 )
								{
									m_CaretRowUCharPos = charpos;
									m_CaretPos.xpos = xpos;
									m_CaretPos.pos += len;
									m_CaretPos.linepos += len;
								}
							}
							else
							{
								m_CaretPos.extraspaces = 0;
								m_CaretPos.xpos = 0;
							}
						}
						else
						{
							m_CaretPos.extraspaces = 0;

							if( charpos != 0 )
							{
								if( m_CaretRowUCharPos == charpos )
								{
									m_CaretRowUCharPos = 0;
									m_CaretPos.xpos = 0;
									m_CaretPos.pos -= len;
									m_CaretPos.linepos -= len;
								}
								else
								{
									wxFileOffset len2 = m_CaretPos.linepos -
														m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid].m_Start;
									m_CaretPos.pos = m_CaretPos.pos - len2 + len;
									m_CaretPos.linepos = m_CaretPos.linepos - len2 + len;
									m_CaretRowUCharPos = charpos;
									m_CaretPos.xpos = xpos;
								}
							}
							else
							{
								m_CaretRowUCharPos = 0;
								m_CaretPos.xpos = 0;
								len = m_CaretPos.linepos -
									  m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid].m_Start;
								m_CaretPos.pos -= len;
								m_CaretPos.linepos -= len;
							}
						}

						m_LastCaretXPos = m_CaretPos.xpos;
						AppearCaret();
						UpdateScrollBarPos();
					}
					break;

				case ecEndLine:
				case ecSelEndLine:
					if( m_CaretPos.extraspaces != 0 )
					{
						m_CaretPos.xpos -= int( m_CaretPos.extraspaces * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/ );
						m_CaretPos.extraspaces = 0;
					}
					else
					{
						while( m_CaretRowUCharPos < int( m_ActiveRowUChars.size() ) )
						{
							MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
							m_CaretPos.xpos += m_ActiveRowWidths[m_CaretRowUCharPos];
							m_CaretPos.pos += ucp.second;
							m_CaretPos.linepos += ucp.second;
							++m_CaretRowUCharPos;
						}
					}

					m_LastCaretXPos = m_CaretPos.xpos;
					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecBeginDoc:
				case ecSelBeginDoc:
					m_CaretPos.Reset( m_Lines->m_LineList.begin() );
					UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					m_LastCaretXPos = m_CaretPos.xpos;
					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecEndDoc:
				case ecSelEndDoc:
					m_CaretPos.iter = m_Lines->m_LineList.end();
					--m_CaretPos.iter;
					m_CaretPos.pos = m_Lines->m_Size;
					m_CaretPos.linepos = m_CaretPos.iter->m_Size;    //-m_CaretPos.iter->m_NewLineSize;
					m_CaretPos.rowid = int( m_Lines->m_RowCount - 1 );
					m_CaretPos.lineid = int( m_Lines->m_LineCount - 1 );
					m_CaretPos.subrowid = int( m_CaretPos.iter->RowCount() - 1 );
					UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					m_LastCaretXPos = m_CaretPos.xpos;
					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecPrevPage:
				case ecSelPrevPage:
					{
						int count = m_CompleteRowCount - 1;

						if( count == 0 ) ++count;

						if( m_CaretPos.rowid > count )
						{
							m_CaretPos.rowid -= count;
						}
						else
						{
							m_CaretPos.rowid = 0;
						}

						int rowid = m_CaretPos.rowid;
						m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, rowid );
						m_CaretPos.subrowid = m_CaretPos.rowid - rowid;
						MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
						std::advance( riter, m_CaretPos.subrowid );
						m_CaretPos.linepos = riter->m_Start;
						m_CaretPos.pos += m_CaretPos.linepos;
						UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					}

					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecNextPage:
				case ecSelNextPage:
					{
						int count = m_CompleteRowCount - 1;

						if( count == 0 ) ++count;

						if( m_CaretPos.rowid + count < int( m_Lines->m_RowCount ) )
						{
							m_CaretPos.rowid += count;
						}
						else
						{
							m_CaretPos.rowid = int( m_Lines->m_RowCount - 1 );
						}

						int rowid = m_CaretPos.rowid;
						m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, rowid );
						m_CaretPos.subrowid = m_CaretPos.rowid - rowid;
						MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
						std::advance( riter, m_CaretPos.subrowid );
						m_CaretPos.linepos = riter->m_Start;
						m_CaretPos.pos += m_CaretPos.linepos;
						UpdateCaretByXPos( m_LastCaretXPos, m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
					}

					AppearCaret();
					UpdateScrollBarPos();
					break;

				case ecPrevWord:
				case ecSelPrevWord:
					if( m_CaretPos.extraspaces != 0 )
					{
						m_CaretPos.xpos -= int( m_CaretPos.extraspaces * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/ );
						m_CaretPos.extraspaces = 0;
					}
					else
					{
						if( m_CaretRowUCharPos != 0 || m_CaretPos.rowid != 0 )
						{
							int uctype0 = 0;

							do
							{
								if( m_CaretRowUCharPos == 0 )
								{
									if( m_CaretPos.subrowid != 0 ) // to prev subrow
									{
										--m_CaretPos.rowid;
										--m_CaretPos.subrowid;
										UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
									}
									else
										if( m_CaretPos.rowid != 0 ) // to prev line's last row
										{
											MadLineIterator & lit = m_CaretPos.iter;
											--lit;
											--m_CaretPos.lineid;
											m_CaretPos.linepos = lit->m_Size - lit->m_NewLineSize;
											m_CaretPos.pos -= lit->m_NewLineSize;
											--m_CaretPos.rowid;
											m_CaretPos.subrowid = int( lit->RowCount() - 1 );
											UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
											break;
										}
										else          // at begin of file
										{
											break;
										}
								}

								wxASSERT( m_CaretRowUCharPos != 0 );

								if( uctype0 == 0 )
								{
									uctype0 = GetUCharType( m_ActiveRowUChars[m_CaretRowUCharPos - 1].first );
									//if(uctype0 == 1)
									//    uctype0 = 0;
								}
								else
								{
									int uctype = GetUCharType( m_ActiveRowUChars[m_CaretRowUCharPos - 1].first );

									if( uctype != uctype0 ) // && uctype != 1 )
									{
										break;
									}

									uctype0 = uctype;
								}

								--m_CaretRowUCharPos;
								MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
								m_CaretPos.pos -= ucp.second;
								m_CaretPos.linepos -= ucp.second;
								m_CaretPos.xpos -= m_ActiveRowWidths[m_CaretRowUCharPos];
							}
							while( m_CaretRowUCharPos != 0 || m_CaretPos.subrowid != 0 );
						}
					}

					AppearCaret();
					UpdateScrollBarPos();
					m_LastCaretXPos = m_CaretPos.xpos;
					break;

				case ecNextWord:
				case ecSelNextWord:
					if( m_CaretPos.pos < m_Lines->m_Size )
					{
						MadLineIterator & lit = m_CaretPos.iter;
						wxFileOffset lineendpos = lit->m_Size - lit->m_NewLineSize;

						if( m_CaretPos.linepos < lineendpos )
						{
							m_Lines->InitNextUChar( lit, m_CaretPos.linepos );
							MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
							int uctype0 = 0, uctype;

							do
							{
								( m_Lines->*NextUChar )( m_ActiveRowUChars );
								MadUCPair & ucp = m_ActiveRowUChars.back();

								if( uctype0 == 0 )
								{
									uctype0 = GetUCharType( ucp.first );
								}
								else
								{
									uctype = GetUCharType( ucp.first );

									if( uctype != uctype0 )
									{
										//if(uctype != 1)
										break;
										//uctype0 = 1;
									}
								}

								m_CaretPos.pos += ucp.second;
								m_CaretPos.linepos += ucp.second;

								if( m_CaretPos.linepos <= lineendpos &&
										m_CaretPos.subrowid < int( lit->RowCount() - 1 ) &&
										m_CaretPos.linepos >= lit->m_RowIndices[m_CaretPos.subrowid + 1].m_Start )
								{
									++m_CaretPos.subrowid;
									++m_CaretPos.rowid;
								}
							}
							while( m_CaretPos.linepos < lineendpos );

							UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						}
						else                // to next line
						{
							//if(m_CaretPos.rowid < m_Lines->m_RowCount - 1)
							{
								m_CaretPos.pos += lit->m_NewLineSize;
								++lit;
								++m_CaretPos.lineid;
								m_CaretPos.linepos = 0;
								++m_CaretPos.rowid;
								m_CaretPos.subrowid = 0;
								UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
							}
						}
					}

					AppearCaret();
					UpdateScrollBarPos();
					m_LastCaretXPos = m_CaretPos.xpos;
					break;

				case ecLeftBrace:
				case ecSelLeftBrace:
					if( m_LeftBrace_rowid < 0 )
					{
						MadLineIterator lit = m_CaretPos.iter;
						wxFileOffset linepos = m_CaretPos.linepos;
						m_LeftBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
						m_LeftBrace.BraceIndex = -1;
						FindLeftBrace( m_LeftBrace_rowid, lit, linepos, m_LeftBrace );
					}

					if( m_LeftBrace_rowid >= 0 )
					{
						int rowid = m_LeftBrace_rowid;
						m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, rowid );
						m_CaretPos.subrowid = m_LeftBrace_rowid - rowid;
						m_CaretPos.rowid = m_LeftBrace_rowid;
						m_CaretPos.linepos = m_LeftBrace.LinePos;
						m_CaretPos.pos += m_CaretPos.linepos;
						UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						m_LastCaretXPos = m_CaretPos.xpos;
						AppearCaret();
						UpdateScrollBarPos();
					}

					break;

				case ecRightBrace:
				case ecSelRightBrace:
					if( m_RightBrace_rowid < 0 )
					{
						MadLineIterator lit = m_CaretPos.iter;
						wxFileOffset linepos = m_CaretPos.linepos;
						m_RightBrace_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
						m_RightBrace.BraceIndex = -1;
						FindRightBrace( m_RightBrace_rowid, lit, linepos, m_RightBrace );
					}

					if( m_RightBrace_rowid >= 0 )
					{
						int rowid = m_RightBrace_rowid;
						m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, rowid );
						m_CaretPos.subrowid = m_RightBrace_rowid - rowid;
						m_CaretPos.rowid = m_RightBrace_rowid;
						m_CaretPos.linepos = m_RightBrace.LinePos;
						m_CaretPos.pos += m_CaretPos.linepos;
						UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
						m_LastCaretXPos = m_CaretPos.xpos;
						AppearCaret();
						UpdateScrollBarPos();
					}

					break;

				case ecReturn:
				case ecReturnNoIndent:
					if( !IsReadOnly() && !m_SingleLineMode )
					{
						if( m_Selection && m_EditMode == emColumnMode )
						{
							DeleteSelection( true, nullptr, false );
						}
						else
						{
							//if(m_CaretPos.extraspaces)
							m_CaretPos.extraspaces = 0;
							vector < ucs4_t > ucs;

							switch( m_InsertNewLineType )
							{
							case nltUNIX:
#ifndef __WXMSW__
							case nltDefault:
#endif
								ucs.push_back( 0x0A );
								break;

							case nltMAC:
								ucs.push_back( 0x0D );
								break;

							case nltDOS:
#ifdef __WXMSW__
							case nltDefault:
#endif
								ucs.push_back( 0x0D );
								ucs.push_back( 0x0A );
								break;
							}

							if( m_AutoIndent && command == ecReturn )
							{
								MadUCQueue ucqueue;
								MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
								ucs4_t uc(0);
								wxFileOffset pos, pos1;

								if( m_Selection )
								{
									pos = m_SelectionBegin->iter->m_RowIndices[0].m_Start;
									m_Lines->InitNextUChar( m_SelectionBegin->iter, pos );
									pos1 = m_SelectionBegin->pos;
									pos = pos1 - m_SelectionBegin->linepos + pos;
								}
								else
								{
									pos = m_CaretPos.iter->m_RowIndices[0].m_Start;
									m_Lines->InitNextUChar( m_CaretPos.iter, pos );
									pos1 = m_CaretPos.pos;
									pos = pos1 - m_CaretPos.linepos + pos;
								}

								while( pos < pos1 && ( m_Lines->*NextUChar )( ucqueue ) && ( ( uc = ucqueue.back().first ) == 0x20 || uc == 0x09 ) )
									pos += ucqueue.back().second;

								int spaces = 0;

								if( m_Selection )
									m_Lines->InitNextUChar( m_SelectionEnd->iter, m_SelectionEnd->linepos );
								else
									m_Lines->InitNextUChar( m_CaretPos.iter, m_CaretPos.linepos );

								while( ( m_Lines->*NextUChar )( ucqueue ) && ( ( uc = ucqueue.back().first ) == 0x20 || uc == 0x09 ) )
								{
									++spaces;
								}

								bool unindentChar = false;

								if( ucqueue.size() > 0 && m_Syntax->m_SynAttr->m_UnindentChar.Find( wxChar( uc ) ) >= 0 )
								{
									unindentChar = true;
								}

								if( spaces )
								{
									if( m_Selection )
									{
										m_SelectionEnd->pos += spaces;
									}
									else
									{
										m_Selection = true;
										m_SelectionPos1 = m_CaretPos;
										m_SelectionPos2 = m_CaretPos;
										m_SelectionPos2.pos += spaces;
										m_SelectionBegin = &m_SelectionPos1;
										m_SelectionEnd = &m_SelectionPos2;
									}

									UpdateSelectionPos();
								}

								MadLineIterator &lit = m_CaretPos.iter;
								int lineid = m_CaretPos.lineid;
								wxFileOffset linepos = m_CaretPos.linepos;

								if( m_Selection )
								{
									lit = m_SelectionBegin->iter;
									lineid = m_SelectionBegin->lineid;
									linepos = m_SelectionBegin->linepos;
								}

								GetIndentSpaces( lineid, lit, ucs, true, unindentChar );
							}

							InsertString( &ucs[0], ucs.size(), false, true, false );
						}
					}

					break;

				case ecTab:
					if( !IsReadOnly() )
					{
						if( m_Selection && ( m_SelectionPos1.lineid != m_SelectionPos2.lineid )  && IsTextMode())
						{
							if((!m_SingleLineMode) && IsMacroRecording())
								RecordAsMadMacro( this, wxString( wxT( "IncreaseIndent()" ) ) );
							IncreaseDecreaseIndent( true );
						}
						else
							if( m_InsertSpacesInsteadOfTab )
							{
								int tabwidth = m_TabColumns * m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;

								if( m_Selection ) tabwidth -= ( m_SelectionBegin->xpos % tabwidth );
								else tabwidth -= ( m_CaretPos.xpos % tabwidth );

								int spaces = tabwidth / m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;

								if( spaces == 0 ) spaces = m_TabColumns;

								ucs4_t *sp = new ucs4_t[spaces];

								for( int i = 0; i < spaces; ++i )
									sp[i] = 0x20;
								if((!m_SingleLineMode) && IsMacroRecording())
								{
									wxString tp;
									for(int i = 0; i < spaces; ++i)
									{
										tp << wxChar(sp[i]);
									}
									RecordAsMadMacro( this, tp, true );
								}

								InsertString( sp, spaces, true, true, false );
								delete []sp;
							}
							else
							{
								ucs4_t tab = 0x09;
								if((!m_SingleLineMode) && IsMacroRecording())
									RecordAsMadMacro( this, wxString( wxT( "\\t" ) ), true );
								InsertString( &tab, 1, true, true, false );
							}
					}

					break;

				case ecIncreaseIndent:
					{
						IncreaseDecreaseIndent( true );
					}
					break;

				case ecDecreaseIndent:
					{
						IncreaseDecreaseIndent( false );
					}
					break;

				case ecComment:
					{
						CommentUncomment( true );
					}
					break;

				case ecUncomment:
					{
						CommentUncomment( false );
					}
					break;

				case ecInsertTabChar:
					if( !IsReadOnly() )
					{
						ucs4_t tab = 0x09;
						InsertString( &tab, 1, true, true, false );
						
					}

					break;

				case ecDelete:
					if( !IsReadOnly() )
					{
						if((!m_SingleLineMode) && IsMacroRecording())
							RecordAsMadMacro( this, wxString( wxT( "Delete()" ) ) );
						if( m_Selection )
						{
							if( m_EditMode == emColumnMode && GetSelectionSize() == 0 )
							{
								if( m_CaretRowUCharPos == int( m_ActiveRowUChars.size() ) ) // at end-of-row
								{
									if( m_CaretPos.xpos < m_MaxColumnRowWidth )
									{
										++m_CaretPos.extraspaces;
										m_CaretPos.xpos += m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
									}
								}
								else
								{
									MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
									m_CaretPos.xpos += m_ActiveRowWidths[m_CaretRowUCharPos];
									++m_CaretRowUCharPos;
									m_CaretPos.pos += ucp.second;
									m_CaretPos.linepos += ucp.second;
								}

								EndUpdateSelection( true );
							}

							int oldsubrowid = m_CaretPos.subrowid, oldlineid = m_CaretPos.lineid;

							DeleteSelection( true, nullptr, true );
							
							if(m_TypewriterMode && IsTextMode())
							{
								m_TopRow += (m_CaretPos.lineid - oldlineid) + (m_CaretPos.subrowid - oldsubrowid);
								if( m_TopRow < 0 ) m_TopRow = 0;
							}
						}
						else                // no Selection
						{
							if( m_CaretPos.pos < m_Lines->m_Size )
							{
								// at End of Line
								if( m_CaretPos.extraspaces == 0 &&
										m_CaretPos.linepos == m_CaretPos.iter->m_Size - m_CaretPos.iter->m_NewLineSize )
								{
									MadDeleteUndoData *dudata = new MadDeleteUndoData;
									dudata->m_Pos = m_CaretPos.pos;
									dudata->m_Size = m_CaretPos.iter->m_NewLineSize;
									MadLineIterator lit =
										DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr );
									MadUndo *undo = m_UndoBuffer->Add();
									SetNeedSync();
									undo->m_CaretPosBefore = m_CaretPos.pos;
									undo->m_CaretPosAfter = m_CaretPos.pos;
									undo->m_Undos.push_back( dudata );
									bool sc = ( m_Modified == false );
									m_Modified = true;
									m_Lines->Reformat( lit, lit );
									UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
									AppearCaret();
									UpdateScrollBarPos();
									m_LastCaretXPos = m_CaretPos.xpos;
									m_RepaintAll = true;
									Refresh( false );
									DoSelectionChanged();

									if( sc ) DoStatusChanged();
								}
								else
								{
									// ColumnMode & beyond EOL
									if( m_CaretPos.extraspaces != 0
											&& m_CaretPos.subrowid == int( m_CaretPos.iter->RowCount() - 1 ) )
									{
										MadOverwriteUndoData *oudata = new MadOverwriteUndoData();
										// delete newline char
										oudata->m_Pos = m_CaretPos.pos;
										oudata->m_DelSize = m_CaretPos.iter->m_NewLineSize;
										// insert spaces
										vector < ucs4_t > ucs;
										ucs.insert( ucs.begin(), m_CaretPos.extraspaces, 0x20 );
										MadBlock blk( m_Lines->m_MemData, -1, 0 );
										UCStoBlock( &ucs[0], m_CaretPos.extraspaces, blk );
										oudata->m_InsSize = blk.m_Size;
										oudata->m_InsData.push_back( blk );
										MadLineIterator lit = DeleteInsertData( oudata->m_Pos,
																				oudata->m_DelSize, &oudata->m_DelData,
																				oudata->m_InsSize, &oudata->m_InsData );
										MadUndo *undo = m_UndoBuffer->Add();
										SetNeedSync();
										undo->m_CaretPosBefore = m_CaretPos.pos;
										undo->m_Undos.push_back( oudata );
										bool sc = ( m_Modified == false );
										m_Modified = true;
										m_Lines->Reformat( lit, lit );
										m_CaretPos.pos = oudata->m_Pos + oudata->m_InsSize;
										undo->m_CaretPosAfter = m_CaretPos.pos;
										UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
										AppearCaret();
										UpdateScrollBarPos();
										m_LastCaretXPos = m_CaretPos.xpos;
										m_RepaintAll = true;
										Refresh( false );
										DoSelectionChanged();

										if( sc ) DoStatusChanged();
									}
									else          // at middle of line
									{
										m_CaretPos.extraspaces = 0;
										m_Lines->InitNextUChar( m_CaretPos.iter, m_CaretPos.linepos );
										MadUCQueue ucqueue;
										MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;
										( m_Lines->*NextUChar )( ucqueue );
										MadDeleteUndoData *dudata = new MadDeleteUndoData;
										dudata->m_Pos = m_CaretPos.pos;
										dudata->m_Size = ucqueue.front().second;
										size_t oldrows = m_Lines->m_RowCount;
										size_t oldlines = m_Lines->m_LineCount;
										MadLineIterator lit =
											DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr );
										MadUndo *undo = m_UndoBuffer->Add();
										SetNeedSync();
										undo->m_CaretPosBefore = m_CaretPos.pos;
										undo->m_CaretPosAfter = m_CaretPos.pos;
										undo->m_Undos.push_back( dudata );
										bool sc = ( m_Modified == false );
										m_Modified = true;
										size_t count = m_Lines->Reformat( lit, lit );
										UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
										AppearCaret();
										UpdateScrollBarPos();
										m_LastCaretXPos = m_CaretPos.xpos;

										if( oldrows != m_Lines->m_RowCount
												|| oldlines != m_Lines->m_LineCount || count > 1 )
										{
											m_RepaintAll = true;
										}
										else
										{
											m_RepaintSelection = true;
											m_SelFirstRow = m_CaretPos.rowid - m_CaretPos.subrowid;
											m_SelLastRow = m_SelFirstRow + int( lit->RowCount() - 1 );
										}

										Refresh( false );
										DoSelectionChanged();

										if( sc ) DoStatusChanged();
									}
								}
							}
						}
					}

					break;

				case ecDeleteLine:
				case ecCutLine:
					if( !IsReadOnly() )
					{
						if( !m_Selection )
						{
							m_SelectionPos1 = m_CaretPos;
							m_SelectionPos2 = m_CaretPos;
							m_SelectionBegin = &m_SelectionPos1;
							m_SelectionEnd = &m_SelectionPos2;
							m_Selection = true;
						}

						m_SelectionBegin->extraspaces = 0;
						m_SelectionBegin->xpos = 0;
						wxFileOffset size = m_SelectionBegin->linepos - m_SelectionBegin->iter->m_RowIndices[m_SelectionBegin->subrowid].m_Start;
						m_SelectionBegin->linepos -= size;
						m_SelectionBegin->pos -= size;
						m_SelectionEnd->extraspaces = 0;
						m_SelectionEnd->xpos = 0;

						if( m_SelectionEnd->subrowid == int( m_SelectionEnd->iter->RowCount() - 1 ) ) // at last subrow
						{
							size = m_SelectionEnd->iter->m_Size - m_SelectionEnd->linepos;
							m_SelectionEnd->linepos += size;
							m_SelectionEnd->pos += size;

							if( m_SelectionEnd->lineid < int( m_Lines->m_LineCount - 1 ) ) // to next line
							{
								++m_SelectionEnd->lineid;
								++m_SelectionEnd->rowid;
								m_SelectionEnd->subrowid = 0;
							}
						}
						else // to next subrow
						{
							size = m_SelectionEnd->iter->m_RowIndices[m_SelectionEnd->subrowid + 1].m_Start - m_SelectionEnd->linepos;
							m_SelectionEnd->linepos += size;
							m_SelectionEnd->pos += size;
							++m_SelectionEnd->rowid;
							++m_SelectionEnd->subrowid;
						}

						int oldsubrowid = m_CaretPos.subrowid, oldlineid = m_CaretPos.lineid;
						if( m_SelectionBegin->pos == m_SelectionEnd->pos )
						{
							m_Selection = false;
							m_RepaintSelection = true;
							Refresh( false );
							m_CaretPos = *m_SelectionBegin;
							AppearCaret();
							UpdateScrollBarPos();
							m_LastCaretXPos = m_CaretPos.xpos;
							DoSelectionChanged();
							
							if(m_TypewriterMode && IsTextMode())
							{
								--m_TopRow;
								if( m_TopRow < 0 ) m_TopRow = 0;
							}
						}
						else
						{
							MadEditMode em = m_EditMode;
							m_EditMode = emTextMode;

							if( command == ecCutLine )
							{
								CopyToClipboard();
							}

							DeleteSelection( true, nullptr, false );
							m_EditMode = em;
							if(m_TypewriterMode && IsTextMode())
							{
								m_TopRow += (m_CaretPos.lineid - oldlineid) + (m_CaretPos.subrowid - oldsubrowid);
								if( m_TopRow < 0 ) m_TopRow = 0;
							}
						}
					}

					break;

				case ecBackSpace:
					if( !IsReadOnly() )
					{
						int oldsubrowid = m_CaretPos.subrowid, oldlineid = m_CaretPos.lineid;

						if( m_Selection )
						{
							if( m_EditMode == emColumnMode && GetSelectionSize() == 0 && m_SelLeftXPos != 0 )
							{
								if( m_CaretPos.extraspaces != 0 )
								{
									--m_CaretPos.extraspaces;
									m_CaretPos.xpos -= m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
								}
								else
								{
									if( m_CaretRowUCharPos != 0 ) // at middle-of-row
									{
										--m_CaretRowUCharPos;
										MadUCPair & ucp = m_ActiveRowUChars[m_CaretRowUCharPos];
										m_CaretPos.pos -= ucp.second;
										m_CaretPos.linepos -= ucp.second;
										m_CaretPos.xpos -= m_ActiveRowWidths[m_CaretRowUCharPos];
									}
								}

								EndUpdateSelection( true );
							}

							DeleteSelection( true, nullptr, true );
							
							if(m_TypewriterMode && IsTextMode())
							{
								m_TopRow += (m_CaretPos.lineid - oldlineid) + (m_CaretPos.subrowid - oldsubrowid);
								if( m_TopRow < 0 ) m_TopRow = 0;
							}
						}
						else                // no Selection
						{
							if( m_CaretPos.pos > 0 )
							{
								if( m_CaretPos.extraspaces )
								{
									--m_CaretPos.extraspaces;
									m_CaretPos.xpos -= m_TextFontSpaceWidth/*GetUCharWidth( 0x20 )*/;
									AppearCaret();
									UpdateScrollBarPos();
									m_LastCaretXPos = m_CaretPos.xpos;
								}
								else
								{
									if( m_CaretRowUCharPos == 0 )   // at begin-of-row
									{
										if( m_CaretPos.subrowid != 0 ) // to prev subrow
										{
											--m_CaretPos.rowid;
											--m_CaretPos.subrowid;
											UpdateCaret( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
											wxASSERT( m_CaretRowUCharPos != 0 );
											// delete last char of current row
											MadDeleteUndoData *dudata = new MadDeleteUndoData;
											size_t len = m_ActiveRowUChars.back().second;
											dudata->m_Pos = m_CaretPos.pos - len;
											dudata->m_Size = len;
											MadLineIterator lit = DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data,  0, nullptr );
											MadUndo *undo = m_UndoBuffer->Add();
											SetNeedSync();
											undo->m_CaretPosBefore = m_CaretPos.pos;
											undo->m_CaretPosAfter = dudata->m_Pos;
											undo->m_Undos.push_back( dudata );
											bool sc = ( m_Modified == false );
											m_Modified = true;
											m_Lines->Reformat( lit, lit );
											m_CaretPos.pos = dudata->m_Pos;
											UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
											AppearCaret();
											UpdateScrollBarPos();
											m_LastCaretXPos = m_CaretPos.xpos;
											m_RepaintAll = true;
											Refresh( false );
											DoSelectionChanged();

											if( sc ) DoStatusChanged();
										}
										else
											if( m_CaretPos.rowid != 0 )     // to prev line's last row
											{
												// delete prev line's newline char
												MadLineIterator lit = m_CaretPos.iter;
												--lit;
												MadDeleteUndoData *dudata = new MadDeleteUndoData;
												size_t len = lit->m_NewLineSize;
												dudata->m_Pos = m_CaretPos.pos - len;
												dudata->m_Size = len;
												lit = DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr );
												MadUndo *undo = m_UndoBuffer->Add();
												SetNeedSync();
												undo->m_CaretPosBefore = m_CaretPos.pos;
												undo->m_CaretPosAfter = dudata->m_Pos;
												undo->m_Undos.push_back( dudata );
												m_Modified = true;
												m_Selection = false;
												m_Lines->Reformat( lit, lit );
												m_CaretPos.pos = dudata->m_Pos;
												UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
												AppearCaret();
												UpdateScrollBarPos();
												m_LastCaretXPos = m_CaretPos.xpos;
												m_RepaintAll = true;
												Refresh( false );
												DoSelectionChanged();
												DoStatusChanged();
											}
											
											if(m_TypewriterMode && IsTextMode())
											{
												--m_TopRow;
												if( m_TopRow < 0 ) m_TopRow = 0;
											}
									}
									else          // in middle of row
									{
										MadDeleteUndoData *dudata = new MadDeleteUndoData;
										size_t len = m_ActiveRowUChars[m_CaretRowUCharPos - 1].second;
										dudata->m_Pos = m_CaretPos.pos - len;
										dudata->m_Size = len;
										size_t oldrows = m_Lines->m_RowCount;
										size_t oldlines = m_Lines->m_LineCount;
										MadLineIterator lit = DeleteInsertData( dudata->m_Pos, dudata->m_Size, &dudata->m_Data,  0, nullptr );
										MadUndo *undo = m_UndoBuffer->Add();
										SetNeedSync();
										undo->m_CaretPosBefore = m_CaretPos.pos;
										undo->m_CaretPosAfter = dudata->m_Pos;
										undo->m_Undos.push_back( dudata );
										bool sc = ( m_Modified == false );
										m_Modified = true;
										size_t count = m_Lines->Reformat( lit, lit );
										m_CaretPos.pos = dudata->m_Pos;
										UpdateCaretByPos( m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
										AppearCaret();
										UpdateScrollBarPos();
										m_LastCaretXPos = m_CaretPos.xpos;

										if( oldrows != m_Lines->m_RowCount
												|| oldlines != m_Lines->m_LineCount || count > 1 )
										{
											if(m_TypewriterMode && IsTextMode())
											{
												--m_TopRow;
												if( m_TopRow < 0 ) m_TopRow = 0;
											}

											m_RepaintAll = true;
										}
										else
										{
											m_RepaintSelection = true;
											m_SelFirstRow = m_CaretPos.rowid - m_CaretPos.subrowid;
											m_SelLastRow = m_SelFirstRow + int( lit->RowCount() - 1 );
										}

										Refresh( false );
										DoSelectionChanged();

										if( sc ) DoStatusChanged();
									}
								}
							}
						}
					}

					break;

				case ecToggleInsertMode:
					SetInsertMode( !m_InsertMode );
					break;

				case ecInsertDateTime:
					if( !IsReadOnly() )
					{
						wxString fmt;
						m_Config->Read( wxT( "/MadEdit/DateTimeFormat" ), &fmt, wxT( "%c" ) );

						if( !fmt.IsEmpty() )
						{
							char oldlocale[256];
							bool inEnglish = false;
							m_Config->Read( wxT( "/MadEdit/DateTimeInEnglish" ), &inEnglish );

							if( inEnglish )
							{
								strcpy( oldlocale, setlocale( LC_TIME, nullptr ) );
								setlocale( LC_TIME, "C" );
							}

							wxDateTime now = wxDateTime::Now();
							wxString text = now.Format( fmt.c_str() );

							if( inEnglish )
							{
								setlocale( LC_TIME, oldlocale );
							}

							vector<ucs4_t> ucs;
							TranslateText( text.c_str(), text.Len(), &ucs, true );
							InsertString( &ucs[0], ucs.size(), false, true, false );
						}
					}

					break;
				}
			}

			if( bSel )
			{
				EndUpdateSelection( true );
				DoSelectionChanged();
			}
			else
				if( command < ecCaretCommandLast && command > ecCaretCommandFirst )
				{
					EndUpdateSelection( false );
					DoSelectionChanged();
				}
		}
		else // HexMode
		{
			if( bSel )
				BeginUpdateSelection();

			if( !m_ActiveRowUChars.empty() )
				m_ActiveRowUChars.clear();

			MadLines::NextUCharFuncPtr NextUChar = m_Lines->NextUChar;

			if( command >= ecCharFirst && command <= ecCharLast )
			{
				if( m_CaretAtHexArea )
				{
					if( ( command >= ucs4_t( '0' ) && command <= ucs4_t( '9' ) ) ||
							( command >= ucs4_t( 'a' ) && command <= ucs4_t( 'f' ) ) ||
							( command >= ucs4_t( 'A' ) && command <= ucs4_t( 'F' ) ) )
					{
						int hc = command;

						if( hc > '9' )
							hc = ( hc | 0x20 ) - 87;
						else
							hc -= '0';

						InsertHexChar( hc );
					}
				}
				else
				{
					//bool oldim = m_InsertMode;
					//m_InsertMode = false;
					ucs4_t uc = command;
					InsertString( &uc, 1, false, true, false );
					//m_InsertMode = oldim;
				}
			}
			else
				switch( command )
				{
				case ecToggleInsertMode:
					SetInsertMode( !m_InsertMode );
					break;

				case ecPrevWord:
				case ecSelPrevWord:
				case ecLeft:
				case ecSelLeft:
					if( m_CaretPos.pos >= 0 )
					{
						if( m_CaretPos.pos > 0 && !m_CaretAtHalfByte )
						{
							// to prev line
							if( m_CaretPos.linepos == 0 )
							{
								--m_CaretPos.iter;
								--m_CaretPos.lineid;
								--m_CaretPos.rowid;
								m_CaretPos.linepos = m_CaretPos.iter->m_Size;
								m_CaretPos.subrowid = int( m_CaretPos.iter->RowCount() - 1 );
							}
							else
							{
								//to prev row
								if( m_CaretPos.subrowid > 0
										&& m_CaretPos.linepos ==
										m_CaretPos.iter->m_RowIndices[m_CaretPos.subrowid].m_Start )
								{
									--m_CaretPos.rowid;
									--m_CaretPos.subrowid;
								}
							}
						}

						if( m_CaretAtHexArea )
						{
							if( m_CaretAtHalfByte )
							{
								m_CaretAtHalfByte = false;
							}
							else
								if( m_CaretPos.pos > 0 )      // to prev byte
								{
									m_CaretAtHalfByte = true;
									--m_CaretPos.linepos;
									--m_CaretPos.pos;
								}
						}
						else                // at TextArea
						{
							if( m_CaretPos.pos > 0 )   // to prev char
							{
								// confirm the caret is within m_HexRowIndex
								int row = ( m_CaretPos.pos >> 4 );
								bool bAtBegin = ( size_t( m_CaretPos.pos ) & 0x0F ) == 0;

								if( row < m_TopRow )
								{
									if( bAtBegin )
										m_TopRow = row - 1;
									else
										m_TopRow = row;
								}
								else
									if( row == m_TopRow )
									{
										if( bAtBegin )
											--m_TopRow;
									}
									else
										//row>m_TopRow
									{
										if( row >= ( m_TopRow + m_HexRowCount ) )
										{
											m_TopRow = row - m_HexRowCount + 1;

											if( bAtBegin )
												--m_TopRow;
										}
										else
										{
											if( !bAtBegin )
											{
												if( m_HexRowIndex[row - m_TopRow] == m_CaretPos.pos )
												{
													bAtBegin = true;
												}
											}
										}
									}

								if( m_TopRow != m_HexTopRow )
									PrepareHexRowIndex( m_TopRow, m_HexRowCount );

								wxFileOffset oldpos, pos, linepos;
								MadLineIterator lit;
								int rowid_notused;

								if( bAtBegin )
								{
									pos = m_HexRowIndex[row - 1 - m_TopRow];
								}
								else
								{
									pos = m_HexRowIndex[row - m_TopRow];
								}

								linepos = pos;
								GetLineByPos( lit, linepos, rowid_notused );
								linepos = pos - linepos;
								m_Lines->InitNextUChar( lit, linepos );

								do
								{
									oldpos = pos;

									if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) )
									{
										linepos += m_ActiveRowUChars.back().second;
										pos += m_ActiveRowUChars.back().second;
									}
									else
									{
										++lit;
										linepos = 0;
										m_Lines->InitNextUChar( lit, 0 );
									}
								}
								while( pos < m_CaretPos.pos );

								wxASSERT( lit == m_CaretPos.iter );
								m_CaretPos.linepos -= ( pos - oldpos );
								m_CaretPos.pos = oldpos;
							}
						}
					}

					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecNextWord:
				case ecSelNextWord:
				case ecRight:
				case ecSelRight:
					if( m_CaretPos.pos < m_Lines->m_Size )
					{
						if( m_CaretAtHexArea )
						{
							if( m_CaretAtHalfByte )    // to next byte
							{
								m_CaretAtHalfByte = false;
								++m_CaretPos.linepos;
								++m_CaretPos.pos;
							}
							else
							{
								m_CaretAtHalfByte = true;
							}
						}
						else                // at TextArea
						{
							m_Lines->InitNextUChar( m_CaretPos.iter, m_CaretPos.linepos );

							if( ( m_Lines->*NextUChar )( m_ActiveRowUChars ) )
							{
								m_CaretPos.linepos += m_ActiveRowUChars.back().second;
								m_CaretPos.pos += m_ActiveRowUChars.back().second;
							}
							else
							{
								wxASSERT( false );
							}
						}

						if( m_CaretPos.linepos == m_CaretPos.iter->m_Size )    // to next line
						{
							if( m_CaretPos.lineid < int( m_Lines->m_LineCount - 1 ) )
							{
								++m_CaretPos.iter;
								++m_CaretPos.lineid;
								++m_CaretPos.rowid;
								m_CaretPos.linepos = 0;
								m_CaretPos.subrowid = 0;
							}
						}
						else
						{
							int subrow1 = m_CaretPos.subrowid + 1;

							if( subrow1 != int( m_CaretPos.iter->RowCount() )
									&& m_CaretPos.iter->m_RowIndices[subrow1].m_Start == m_CaretPos.linepos )
							{
								m_CaretPos.subrowid = subrow1;
								++m_CaretPos.rowid;
							}
						}
					}

					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecUp:
				case ecSelUp:
					{
						int row = ( m_CaretPos.pos >> 4 );

						if( row )
							--row;

						AppearHexRow( wxFileOffset( row ) << 4 );
						int xpos;

						if( m_CaretAtHexArea )
						{
							xpos = ( m_CaretPos.pos & 0x0F ) * 3 + 10;

							if( m_CaretAtHalfByte )
								++xpos;

							xpos *= m_HexFontMaxDigitWidth;
						}
						else
						{
							xpos = m_LastTextAreaXPos;
						}

						UpdateHexPosByXPos( row, xpos );
						m_RepaintAll = true;
						AppearCaret();
						UpdateScrollBarPos();
					}
					break;

				case ecDown:
				case ecSelDown:
					{
						int row = ( m_CaretPos.pos >> 4 );
						int rows = ( m_Lines->m_Size >> 4 );

						if( row < rows )
							++row;

						AppearHexRow( wxFileOffset( row ) << 4 );
						int xpos;

						if( m_CaretAtHexArea )
						{
							xpos = ( m_CaretPos.pos & 0x0F ) * 3 + 10;

							if( m_CaretAtHalfByte )
								++xpos;

							xpos *= m_HexFontMaxDigitWidth;
						}
						else
						{
							xpos = m_LastTextAreaXPos; //m_TextAreaXPos;
						}

						UpdateHexPosByXPos( row, xpos );
						m_RepaintAll = true;
						AppearCaret();
						UpdateScrollBarPos();
					}
					break;

				case ecBeginLine:
				case ecSelBeginLine:
					AppearHexRow( m_CaretPos.pos );
					UpdateHexPosByXPos( m_CaretPos.pos >> 4, 0 );
					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecEndLine:
				case ecSelEndLine:
					AppearHexRow( m_CaretPos.pos );
					UpdateHexPosByXPos( m_CaretPos.pos >> 4,
										2 * HexModeMaxColumns * m_HexFontMaxDigitWidth );
					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecBeginDoc:
				case ecSelBeginDoc:
					AppearHexRow( 0 );
					UpdateHexPosByXPos( 0, 0 );
					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecEndDoc:
				case ecSelEndDoc:
					AppearHexRow( m_Lines->m_Size );
					UpdateHexPosByXPos( m_Lines->m_Size >> 4,
										2 * HexModeMaxColumns * m_HexFontMaxDigitWidth );
					m_RepaintAll = true;
					AppearCaret();
					UpdateScrollBarPos();
					m_LastTextAreaXPos = m_TextAreaXPos;
					break;

				case ecPrevPage:
				case ecSelPrevPage:
					{
						int row = m_CaretPos.pos >> 4;
						int count = m_CompleteRowCount - 1;

						if( count > 1 ) --count;
						else
							if( count == 0 ) ++count;

						if( row > count )
						{
							row -= count;
						}
						else
						{
							row = 0;
						}

						AppearHexRow( wxFileOffset( row ) << 4 );
						int xpos;

						if( m_CaretAtHexArea )
						{
							xpos = ( m_CaretPos.pos & 0x0F ) * 3 + 10;

							if( m_CaretAtHalfByte )
								++xpos;

							xpos *= m_HexFontMaxDigitWidth;
						}
						else
						{
							xpos = m_LastTextAreaXPos;
						}

						UpdateHexPosByXPos( row, xpos );
						m_RepaintAll = true;
						AppearCaret();
						UpdateScrollBarPos();
					}
					break;

				case ecNextPage:
				case ecSelNextPage:
					{
						int row = m_CaretPos.pos >> 4;
						int rows = m_Lines->m_Size >> 4;
						int count = m_CompleteRowCount - 1;

						if( count > 1 )
							--count;
						else
							if( count == 0 )
								++count;

						if( row + count < rows )
						{
							row += count;
						}
						else
						{
							if( rows > 0 )
								row = rows;
						}

						AppearHexRow( wxFileOffset( row ) << 4 );
						int xpos;

						if( m_CaretAtHexArea )
						{
							xpos = ( m_CaretPos.pos & 0x0F ) * 3 + 10;

							if( m_CaretAtHalfByte )
								++xpos;

							xpos *= m_HexFontMaxDigitWidth;
						}
						else
						{
							xpos = m_LastTextAreaXPos;
						}

						UpdateHexPosByXPos( row, xpos );
						m_RepaintAll = true;
						AppearCaret();
						UpdateScrollBarPos();
					}
					break;

				case ecDelete:
					if( !IsReadOnly() && m_Selection )
					{
						DeleteSelection( true, nullptr, false );
					}

					break;

				case ecTab:
					ToggleHexTextArea();
#ifndef __WXMSW__
					m_RepaintAll = true;
					Refresh( false );
#endif
					break;
				}

			if( bSel )
			{
				EndUpdateSelection( true );
				DoSelectionChanged();
			}
			else
				if( command < ecCaretCommandLast && command > ecCaretCommandFirst )
				{
					EndUpdateSelection( false );
					DoSelectionChanged();
				}
		}
	}

	if( m_RecordCaretMovements &&
			command > ecCaretCommandFirst && command < ecSelCommandLast &&
			oldCaretPos != m_CaretPos.pos )
	{
		m_CaretTracker->Add( oldCaretPos, m_CaretPos.pos );
	}

	m_AutoCompleteRightChar = NewAutoCompleteRightChar;
}

//==================================================

void MadEdit::OnChar( wxKeyEvent& evt )
{
	//std::cout<<"edit char\n";
	if( m_DragDrop )
	{
		return;
	}

	int flags = wxACCEL_NORMAL;
	int key = evt.GetKeyCode();
	ucs4_t ucs4 = evt.GetUnicodeKey();

	if( evt.ControlDown() ) flags |= wxACCEL_CTRL;

	if( evt.AltDown() )     flags |= wxACCEL_ALT;

	if( evt.ShiftDown() )   flags |= wxACCEL_SHIFT;

	//wxLogDebug(wxT("edit OnChar: %X %X"),key, ucs4);

	if( key == WXK_TAB && m_WantTab == false )
	{
		if( flags == wxACCEL_NORMAL )
		{
			Navigate();
		}
		else
			if( flags == wxACCEL_SHIFT )
			{
				Navigate( wxNavigationKeyEvent::IsBackward );
			}

		return;
	}

	MadEditCommand cmd = ms_KeyBindings.FindCommand( flags, key );

	if( cmd == ecToggleWindow )
	{
		wxLogDebug( wxT( "edit toggle window" ) );
		DoToggleWindow();
	}
	else if(((key == MADK_NONE) || (ucs4 >= ( ucs4_t )0x80) ||
		(( ( !evt.HasModifiers() ) || ( evt.GetModifiers() == wxMOD_SHIFT ) ) && ucs4 >= ( ucs4_t )ecCharFirst )))
	{
		ProcessCommand( ucs4 );
	}
	else
	{
		evt.Skip();
	}
}

void MadEdit::OnKeyDown( wxKeyEvent& evt )
{
	//std::cout<<"edit Key down\n";
	int flags = wxACCEL_NORMAL;
	int key = evt.GetKeyCode();

	if( m_DragDrop )
	{
		if( evt.ControlDown() )
		{
			m_DragCopyFlag = true;
			//wxSetCursor( DragCopyCursor );
			if( ( m_EditMode != emHexMode ) && (evt.m_x > ( m_LineNumberAreaWidth + m_BookmarkWidth )))
				SetCursor(DragCopyCursor);
		}

		return;
	}

	if( evt.ControlDown() ) flags |= wxACCEL_CTRL;

	if( evt.AltDown() )     flags |= wxACCEL_ALT;

	if( evt.ShiftDown() )   flags |= wxACCEL_SHIFT;

	if( key == WXK_TAB && m_WantTab == false )
	{
		if( flags == wxACCEL_NORMAL )
		{
			Navigate();
		}
		else
			if( flags == wxACCEL_SHIFT )
			{
				Navigate( wxNavigationKeyEvent::IsBackward );
			}

		return;
	}

	MadEditCommand cmd = ms_KeyBindings.FindCommand( flags, key );

	if( cmd != ecNone )
	{
		//wxLogDebug( wxT( "edit cmd: %X" ), cmd );

		if( cmd == ecToggleWindow )
		{
			wxLogDebug( wxT( "edit toggle window" ) );
			DoToggleWindow();
		}

		// add: gogo, 30.08.2009 ---
		if( cmd == ecDelPrevWord || cmd == ecDelNextWord )
		{
			if( m_EditMode != emHexMode )
			{
				Freeze();

				// if there is a selection, maybe some caret movement have to be done here:
				// moving the caret before or after the current selection ?
				if( !m_Selection )
					ProcessCommand( cmd == ecDelPrevWord ? ecSelPrevWord : ecSelNextWord );

				if( m_Selection )
					ProcessCommand( cmd == ecDelPrevWord ? ecBackSpace : ecDelete );

				Thaw();
			}
		}
		else //--------------------------
			ProcessCommand( cmd );

		if((!m_SingleLineMode) && IsMacroRecording())
		{
			wxString cmdStr;
			if(FromCmdToString( cmdStr, cmd ))
				RecordAsMadMacro( this, cmdStr ); 
		}
#ifdef __WXMSW__

		if( key == int( evt.GetUnicodeKey() ) )
#endif
			return;
	}

	//wxLogDebug(wxT("edit OnKeyDown: %X %X"), key, evt.GetUnicodeKey());
	evt.Skip();
}

void MadEdit::OnKeyUp( wxKeyEvent& evt )
{
	//std::cout<<"edit Key up\n";
	if( m_DragDrop )
	{
		int key = evt.GetKeyCode();

		if( key == WXK_CONTROL )
		{
			m_DragCopyFlag = false;
			//wxSetCursor( DragMoveCursor );
			if(( m_EditMode != emHexMode ) && (evt.m_x > ( m_LineNumberAreaWidth + m_BookmarkWidth )))
				SetCursor(DragMoveCursor);
		}
	}

	evt.Skip();
}

void MadEdit::OnMouseLeftDown( wxMouseEvent &evt )
{
	//wxTheApp->GetTopWindow()->SetTitle(wxString::Format(wxT("LDown")));
	DBOUT( "LDown\n" );

	if( GetCapture() == this )
	{
		ReleaseMouse();
	}

	if( ( m_EditMode != emHexMode ) && evt.m_x >= 0 && evt.m_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) )
	{
		if( evt.m_controlDown )
		{
			SelectAll();
			evt.Skip();
			return;
		}
		else
		{
			int row = evt.m_y / m_RowHeight;
			// update current caretpos
			row += m_TopRow;

			if( row >= int( m_Lines->m_RowCount ) )
				row = int( m_Lines->m_RowCount - 1 );

			m_CaretPos.rowid = row;
			m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, row );
			m_CaretPos.subrowid = m_CaretPos.rowid - row;
			MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
			std::advance( riter, m_CaretPos.subrowid );
			m_CaretPos.linepos = riter->m_Start;
			m_CaretPos.pos += m_CaretPos.linepos;
			UpdateCaretByXPos( evt.m_x + m_DrawingXPos - m_LineNumberAreaWidth - m_BookmarkWidth - m_LeftMarginWidth,
							   m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			m_LastCaretXPos = m_CaretPos.xpos;

			//if( evt.m_x <= m_LineNumberAreaWidth )
			//	SelectLineFromCaretPos();
			//else
			if( evt.m_altDown )
			{
				ToggleBookmark();
				//m_CaretPos.pos = m_SelectionEnd->pos;
				evt.Skip();
				return;
			}
		}
	}

	ProcessCommand( ecMouseNotify );

	if( m_SingleLineMode && evt.m_x == m_LeftClickX && evt.m_y == m_LeftClickY )
	{
		//SelectAll();
		m_LeftClickX = m_LeftClickY = -9999;
		evt.Skip();
		return;
	}

	int row = evt.m_y / m_RowHeight;
	if( m_EditMode == emHexMode && row == 0 )
	{
		m_MouseLeftDown = false;
		m_MouseAtHexTextArea = false;
	}
	else
	{
		CaptureMouse();
		m_MouseLeftDown = true;
		const long TRIPLECLICK_LEN = 200; // 0.2 sec after doubleclick

		if( wxGetLocalTimeMillis() - m_lastDoubleClick <= TRIPLECLICK_LEN )
		{
			SelectLineFromCaretPos( nullptr, false );
			evt.Skip();
			return;
		}

		wxFileOffset oldCaretPos = m_CaretPos.pos;
		int rowid = m_CaretPos.rowid;
		m_MouseAtHexTextArea = true;

		if( evt.m_shiftDown )
		{
			BeginUpdateSelection();
			m_DragDrop = false;
		}
		else
		{
			if( m_Selection && ( !m_DragDrop ) )
			{
				//EndUpdateSelection(false);  // reset selection
			}
		}

		if( m_EditMode != emHexMode )
		{
			// update current caretpos
			row += m_TopRow;

			if( row >= int( m_Lines->m_RowCount ) )
				row = int( m_Lines->m_RowCount - 1 );

			m_CaretPos.rowid = row;
			m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, row );
			m_CaretPos.subrowid = m_CaretPos.rowid - row;
			MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
			std::advance( riter, m_CaretPos.subrowid );
			m_CaretPos.linepos = riter->m_Start;
			m_CaretPos.pos += m_CaretPos.linepos;
			UpdateCaretByXPos( evt.m_x + m_DrawingXPos - m_LineNumberAreaWidth - m_BookmarkWidth - m_LeftMarginWidth,
							   m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			m_LastCaretXPos = m_CaretPos.xpos;
		}
		else                    // HexMode
		{
			int xpos = evt.m_x + m_DrawingXPos;
			const int divide = 58 * m_HexFontMaxDigitWidth + ( m_HexFontMaxDigitWidth >> 1 );

			if( xpos < divide )
			{
				m_CaretAtHexArea = true;
			}
			else
			{
				m_CaretAtHexArea = false;
				m_CaretAtHalfByte = false;
			}

			--row;
			row += m_TopRow;

			if( row >= m_TopRow + m_HexRowCount )
				row = m_TopRow + m_HexRowCount - 1;

			UpdateHexPosByXPos( row, xpos );
			m_RepaintAll = true;
		}

		if( !evt.m_shiftDown )
		{
			if( ( m_Selection ) && ( evt.m_x > ( m_LineNumberAreaWidth + m_BookmarkWidth ) ) && ( m_CaretPos.pos > m_SelectionBegin->pos && m_CaretPos.pos < m_SelectionEnd->pos ) && ( m_EditMode != emHexMode ) && ( IsTextMode() ||
					( ( m_EditMode == emColumnMode ) && ( m_SelectionBegin->xpos != m_SelectionEnd->xpos ) && ( m_CaretPos.xpos > m_SelectionBegin->xpos && m_CaretPos.xpos < m_SelectionEnd->xpos ) ) ) )
			{
				m_DragDrop = true;
				DndBegDrag();

				if( evt.m_controlDown )
				{
					m_DragCopyFlag = true;
					//wxSetCursor( DragCopyCursor );
					SetCursor(DragCopyCursor);
				}
				else
				{
					m_DragCopyFlag = false;
					//wxSetCursor( DragMoveCursor );
					SetCursor(DragMoveCursor);
				}
			}
			else
			{
				int row_bk = m_CaretPos.rowid;
				m_CaretPos.rowid = rowid;
				EndUpdateSelection( false ); // reset selection
				m_CaretPos.rowid = row_bk;
			}
		}

		AppearCaret();
		UpdateScrollBarPos();
		m_LastTextAreaXPos = m_TextAreaXPos;

		if( !m_DragDrop )
		{
			if( evt.m_shiftDown )
			{
				EndUpdateSelection( true );
			}
		}

		DoSelectionChanged();

		if( m_RecordCaretMovements && oldCaretPos != m_CaretPos.pos )
		{
			m_CaretTracker->Add( oldCaretPos, m_CaretPos.pos );
		}
	}

	evt.Skip();
}

void MadEdit::OnMouseLeftUp( wxMouseEvent &evt )
{
	//wxTheApp->GetTopWindow()->SetTitle(wxString::Format(wxT("LUp")));
	DBOUT( "LUp\n" );

	if( m_MouseMotionTimer->IsRunning() )
	{
		m_MouseMotionTimer->Stop();
	}

	if( m_MouseLeftDown || m_MouseLeftDoubleClick )
	{
		if( m_MouseLeftDoubleClick )
		{
			m_MouseLeftDoubleClick = false;
		}
		else
			if( m_MouseLeftDown )
			{
				//Hacking for
				if( !m_DragDrop )
				{
					EndUpdateSelection( true );
					if( m_RecordCaretMovements && m_Selection )
					{
						m_CaretTracker->Add( m_SelectionPos1.pos, m_SelectionPos2.pos );
					}
				}
			}

		m_MouseLeftDown = false;
		ReleaseMouse();
	}

	if( m_EditMode != emHexMode )
	{
		if( evt.m_x >= 0 && evt.m_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) )
		{
			if( m_DragDrop )
			{
				m_DragDrop = false;
				m_DndData.Empty();
			}
            else if(( evt.m_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) ) && (! evt.m_altDown ) && (( !m_Selection ) || (m_CaretPos.pos > m_SelectionEnd->pos)))
				SelectLineFromCaretPos();

			evt.Skip();
			return;
		}
	}

	ProcessCommand( ecMouseNotify );

	if( m_MouseSelectToCopy )
	{
		if( ( evt.ControlDown() && m_MouseSelectToCopyWithCtrlKey ) || !m_MouseSelectToCopyWithCtrlKey )
		{
#if ((wxMAJOR_VERSION < 3) || defined(__WXGTK__))
			wxTheClipboard->UsePrimarySelection( true );
#endif
			CopyToClipboard();
#if ((wxMAJOR_VERSION < 3) || defined(__WXGTK__))
			wxTheClipboard->UsePrimarySelection( false );
#endif
		}
	}

	//Hacking for
	if( m_DragDrop )
	{
		if( m_MouseInWindow )
		{
			if( m_CaretPos.pos >= m_SelectionBegin->pos && m_CaretPos.pos <= m_SelectionEnd->pos )
			{
				EndUpdateSelection( false ); // reset selection
				m_DndData.Empty();
			}
			else
			{
				DndDrop();
			}
		}

		m_DragDrop = false;
		UpdateCursor( evt.m_x, evt.m_y );
	}

	evt.Skip();
}

void MadEdit::OnMouseLeftDClick( wxMouseEvent &evt )
{
	//wxTheApp->GetTopWindow()->SetTitle(wxString::Format(wxT("DClick")));
	DBOUT( "DClick\n" );

	if( ( ( m_EditMode != emHexMode ) && evt.m_x >= 0 && evt.m_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) ) )
	{
		evt.Skip();
		return;
	}

	if( GetCapture() == this )
		ReleaseMouse();

	ProcessCommand( ecMouseNotify );
	wxFileOffset oldCaretPos = m_CaretPos.pos;
	m_DoubleClickX = evt.m_x;
	m_DoubleClickY = evt.m_y;
	m_MouseLeftDoubleClick = true;
	m_MouseLeftDown = true;

	int row = evt.m_y / m_RowHeight;
	// update current caretpos
	row += m_TopRow;

	if( row >= int( m_Lines->m_RowCount ) )
		row = int( m_Lines->m_RowCount - 1 );

	m_CaretPos.rowid = row;
	m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, row );
	m_CaretPos.subrowid = m_CaretPos.rowid - row;
	MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
	std::advance( riter, m_CaretPos.subrowid );
	m_CaretPos.linepos = riter->m_Start;
	m_CaretPos.pos += m_CaretPos.linepos;
	UpdateCaretByXPos( evt.m_x + m_DrawingXPos - m_LineNumberAreaWidth - m_BookmarkWidth - m_LeftMarginWidth,
					   m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );

	SelectWordFromCaretPos( nullptr );
	if(m_LDClickHighlight)
	{
		HighlightWords();
	}
	m_lastDoubleClick = wxGetLocalTimeMillis();
	CaptureMouse();

	if( m_RecordCaretMovements && oldCaretPos != m_CaretPos.pos )
	{
		m_CaretTracker->Add( oldCaretPos, m_CaretPos.pos );
	}

	evt.Skip();
}

void MadEdit::OnMouseMotion( wxMouseEvent &evt )
{
	//wxTheApp->GetTopWindow()->SetTitle(wxString::Format(wxT("%d %d"), evt.m_x, evt.m_y));

	//ProcessCommand(ecMouseNotify); // no notify on mousemotion
	//DBOUT( "OnMM:TW:"<<m_TopRow<<", InW?"<<(m_MouseInWindow?"T":"F")<<", (" <<evt.m_x <<", " << evt.m_y <<")\n" );

	// fix vmware doubleclick bug
	if( m_MouseLeftDoubleClick && m_DoubleClickX == evt.m_x && m_DoubleClickY == evt.m_y )
	{
		m_DoubleClickX = m_DoubleClickY = -9999;
		return;
	}

	if( m_SingleLineMode && FindFocus() != this )
	{
		m_LeftClickX = evt.m_x;
		m_LeftClickY = evt.m_y;
	}

	UpdateCursor( evt.m_x, evt.m_y );

	if( m_MouseLeftDown )
	{
		if( !m_Selection )
		{
			BeginUpdateSelection();
			m_DragDrop = false;
		}

		if( !m_MouseMotionTimer->IsRunning() )
		{
			m_MouseMotionTimer->Start( 125 );
		}

		if( m_EditMode != emHexMode )
		{
			int row;

			if( evt.m_y < 0 )
			{
				row = ( -evt.m_y / m_RowHeight ) + 1;

				if( row > m_TopRow )
				{
					row = 0;
				}
				else
				{
					row = m_TopRow - row;
				}
			}
			else
			{
				row = evt.m_y / m_RowHeight;
				row += m_TopRow;

				if( row >= int( m_Lines->m_RowCount ) )
					row = int( m_Lines->m_RowCount - 1 );
			}

			m_CaretPos.rowid = row;
			m_CaretPos.lineid = GetLineByRow( m_CaretPos.iter, m_CaretPos.pos, row );
			m_CaretPos.subrowid = m_CaretPos.rowid - row;
			MadRowIndexIterator riter = m_CaretPos.iter->m_RowIndices.begin();
			std::advance( riter, m_CaretPos.subrowid );
			m_CaretPos.linepos = riter->m_Start;
			m_CaretPos.pos += m_CaretPos.linepos;
			UpdateCaretByXPos( evt.m_x + m_DrawingXPos - m_LineNumberAreaWidth - m_BookmarkWidth - m_LeftMarginWidth,
							   m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos );
			m_LastCaretXPos = m_CaretPos.xpos;
		}
		else                        //HexMode
		{
			int row;

			if( evt.m_y < m_RowHeight )
			{
				int y = -( evt.m_y - m_RowHeight );
				row = ( y / m_RowHeight ) + 1;

				if( row > m_TopRow )
				{
					row = 0;
				}
				else
				{
					row = m_TopRow - row;
				}
			}
			else
			{
				row = ( evt.m_y / m_RowHeight ) - 1;
				row += m_TopRow;
				int rows = ( m_Lines->m_Size >> 4 ) + 1;

				if( row >= rows )
				{
					if( rows > 0 )
						row = rows - 1;
					else
						row = 0;
				}
			}

			AppearHexRow( wxFileOffset( row ) << 4 );
			UpdateHexPosByXPos( row, evt.m_x + m_DrawingXPos );
			m_RepaintAll = true;
		}

		AppearCaret();
		UpdateScrollBarPos();
		m_LastTextAreaXPos = m_TextAreaXPos;

		if( !m_DragDrop )
		{
			if( m_Selection )
			{
				EndUpdateSelection( true );
			}
		}

		DoSelectionChanged();
	}

	evt.Skip();
}

void MadEdit::OnMouseRightUp( wxMouseEvent & WXUNUSED(evt) )
{
	ProcessCommand( ecMouseNotify );
	DoMouseRightUp();
}

void MadEdit::OnMouseMiddleUp( wxMouseEvent &evt )
{
	if( ( m_EditMode != emHexMode ) && evt.m_x >= 0 && evt.m_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) )
	{
		evt.Skip();
		return;
	}

	ProcessCommand( ecMouseNotify );

	if( m_MiddleMouseToPaste )
	{
#if ((wxMAJOR_VERSION < 3) || defined(__WXGTK__))
		wxTheClipboard->UsePrimarySelection( true );
#endif
		PasteFromClipboard();
#if ((wxMAJOR_VERSION < 3) || defined(__WXGTK__))
		wxTheClipboard->UsePrimarySelection( false );
#endif
		evt.Skip();
	}
}

void MadEdit::OnVSMouseRightUp( wxMouseEvent &evt )
{
	if (m_OnVSMouseRightUp)
	{
		m_VSMousePos = m_VScrollBar->GetRange() * evt.m_y / m_ClientHeight;
		m_OnVSMouseRightUp(this);
	}
	//evt.Skip();
}

void MadEdit::OnHSMouseRightUp( wxMouseEvent &evt )
{
	if(m_OnHSMouseRightUp)
	{
		m_HSMousePos = m_HScrollBar->GetRange() * evt.m_x / m_ClientWidth;
		m_OnHSMouseRightUp(this);
	}
	//evt.Skip();
}

/*No caret activity*/
void MadEdit::ScrollTo( int scrollcmd )
{
	wxScrollEvent event;
	bool vertical = false;
	int vpos = m_TopRow, hpos = m_DrawingXPos;

	switch(scrollcmd)
	{
		case VSCROLLHERE:
		{
			vpos = m_VSMousePos;
			vertical = true;
			break;
		}
		case VSCROLLTOP:
		{
			vpos = 0;
			vertical = true;
			break;
		}
		case VSCROLLBOTTOM:
		{
			vpos = GetFileSize();
			vertical = true;
			break;
		}
		case VSCROLLPAGEUP:
		{
			vpos -= m_VScrollBar->GetPageSize();
			vertical = true;
			break;
		}
		case VSCROLLPAGEDOWN:
		{
			vpos += m_VScrollBar->GetPageSize();
			vertical = true;
			break;
		}
		case VSCROLLUP:
		{
			--vpos;
			vertical = true;
			break;
		}
		case VSCROLLDOWN:
		{
			++vpos;
			vertical = true;
			break;
		}
		case HSCROLLHERE:
		{
			hpos = m_HSMousePos;
			break;
		}
		case HSCROLLLEFTEDGE:
		{
			hpos = 0;
			break;
		}
		case HSCROLLRIGHTEDGE:
		{
			hpos = 0x7FFFFFFF; // Max int 32 bits
			break;
		}
		case HSCROLLPAGELEFT:
		{
			hpos -= m_HScrollBar->GetPageSize();
			break;
		}
		case HSCROLLPAGERIGHT:
		{
			hpos += m_HScrollBar->GetPageSize();
			break;
		}
		case HSCROLLLEFT:
		{
			hpos -= (m_TabColumns * m_TextFontSpaceWidth);
			break;
		}
		case HSCROLLRIGHT:
		{
			hpos += (m_TabColumns * m_TextFontSpaceWidth);
			break;
		}
		default:
			return;
	}
	if(vertical)
	{
		event.SetPosition(vpos);
		OnVScroll( event );
	}
	else
	{
		event.SetPosition(hpos);
		OnHScroll( event );
	}
	UpdateScrollBarPos();
}

void MadEdit::OnSetFocus( wxFocusEvent &evt )
{
	//force updating font widths
	DBOUT( "OnSetFocus\n" );
	memset( m_TextFontWidths, 0, sizeof( m_TextFontWidths ) );
	memset( m_HexFontWidths, 0, sizeof( m_HexFontWidths ) );

	if( m_SingleLineMode )
	{
		SelectAll();
	}

	DoActivate();
	evt.Skip();
}

void MadEdit::OnKillFocus( wxFocusEvent &evt )
{
	wxWindow *win = FindFocus();
	DBOUT( "KillFocus\n" );

	if( win == m_VScrollBar || win == m_HScrollBar )
	{
		this->SetFocus();
	}
	else
	{
		DBOUT( "KillFocus1\n" );

		if( !m_Selection && m_MouseLeftDown )
		{
			EndUpdateSelection( true );
		}

		if( m_SingleLineMode && m_Selection )
		{
			m_Selection = false;
			m_ZeroSelection = false;
			m_RepaintAll = true;
			Refresh();
		}

#ifndef __WXMMSW__
		else
		{
			Refresh(); // HideCaret()
		}

#endif
	}

	m_DragDrop = false;
	m_MouseLeftDown = false;
	evt.Skip();
}

void MadEdit::OnSize( wxSizeEvent &evt )
{
	int w = 0, h = 0;
	GetClientSize( &w, &h );
	//wxTheApp->GetTopWindow()->SetTitle(wxString::Format(wxT("%d %d"), w,h));

	// set m_ClienWidth/Height & pos of scrollbars
	if( m_SingleLineMode )
	{
		m_ClientWidth = w;
		m_ClientHeight = h;
	}
	else
	{
		if( ( m_ClientWidth  = w - m_VSBWidth + 1 ) <= 0 ) m_ClientWidth = 10;

		if( ( m_ClientHeight = h - m_HSBHeight + 1 ) <= 0 ) m_ClientHeight = 10;

		m_VScrollBar->SetSize( wxRect( m_ClientWidth, 0, m_VSBWidth, h + 1 ) );
		m_HScrollBar->SetSize( wxRect( 0, m_ClientHeight, m_ClientWidth + 1, m_HSBHeight ) );
	}

	if( w < 400 ) w = 400;

	if( h < 100 ) h = 100;

	bool deletebitmap = false;
	bool newbitmap = false;

	if( m_ClientBitmap == nullptr )
	{
		newbitmap = true;
		m_MaxWidth = w;
		m_MaxHeight = h;
	}
	else
	{
		if( w > m_MaxWidth )
		{
			deletebitmap = true;
			newbitmap = true;
			m_MaxWidth = w;
		}
		else
		{
			w = m_MaxWidth;
		}

		if( h > m_MaxHeight )
		{
			deletebitmap = true;
			newbitmap = true;
			m_MaxHeight = h;
		}
		else
		{
			h = m_MaxHeight;
		}
	}

	if( deletebitmap )
	{
		delete m_ClientBitmap;
		delete m_MarkBitmap;
	}

	if( newbitmap )
	{
		m_ClientBitmap = new wxBitmap( w, h );
		m_MarkBitmap = new wxBitmap( w, h );
#if FIXINVERT != 0

		if( InvertRect == nullptr )
		{
			wxMemoryDC dc1, dc2;
			dc1.SelectObject( *m_ClientBitmap );
			dc1.SetBrush( *( wxTheBrushList->FindOrCreateBrush( *wxWHITE ) ) );
			dc1.SetPen( *( wxThePenList->FindOrCreatePen( *wxWHITE, 1, wxPENSTYLE_SOLID ) ) );
			dc1.DrawRectangle( 0, 0, 10, 10 );
			dc1.Blit( 0, 0, 10, 10, &dc1, 0, 0, wxINVERT );
			dc2.SelectObject( *m_MarkBitmap );
			dc2.Blit( 0, 0, 10, 10, &dc1, 0, 0 );
			wxColor c = *wxBLACK;
			dc2.GetPixel( 5, 5, &c );

			if( c == *wxWHITE ) // Blit(wxINVERT) not work
			{
				InvertRect = &MadEdit::InvertRectManual;
				//std::cout<<"XXX\n";
			}
			else
			{
				InvertRect = &MadEdit::InvertRectNormal;
				//std::cout<<"OOO\n";
			}
		}

#endif
	}

	m_RepaintAll = true;
	UpdateAppearance();
	GetSize( &w, &h );

	if( m_OldWidth != w || m_OldHeight != h )
	{
		if( m_EditMode != emHexMode )
		{
			RecountLineWidth( false );
		}
		else
		{
			m_DoRecountLineWidth = true;
		}

		m_OldWidth = w;
		m_OldHeight = h;
	}

	if( !m_Painted )
	{
		m_TopRow = 0;
		m_DrawingXPos = 0;
	}

	AppearCaret();
	UpdateScrollBarPos();
	Refresh( false );
	evt.Skip();
}

void MadEdit::OnVScroll( wxScrollEvent &evt )
{
	m_TopRow = evt.GetPosition();
	DBOUT( "OnVScroll:"<<m_TopRow<<"\n" );

	if( m_EditMode != emHexMode )
	{
		if( m_TopRow < 0 ) m_TopRow = 0;
		else
			if( m_TopRow >= int( m_Lines->m_RowCount ) ) m_TopRow = int( m_Lines->m_RowCount - 1 );
	}
	else
	{
		int rowcount = int( m_Lines->m_Size >> 4 ) + 1;

		if( m_TopRow < 0 ) m_TopRow = 0;
		else
			if( m_TopRow >= rowcount ) m_TopRow = rowcount - 1;
	}

	m_RepaintAll = true;
	Refresh( false );
	//evt.Skip();
}

void MadEdit::OnHScroll( wxScrollEvent &evt )
{
	m_DrawingXPos = evt.GetPosition();

	if( m_EditMode != emHexMode )
	{
		int xmax = GetLineNumberAreaWidth( int( m_Lines->m_LineCount ) ) + m_LeftMarginWidth
				   + m_Lines->m_MaxLineWidth + m_RightMarginWidth + m_BookmarkWidth;

		if( m_WordWrapMode == wwmWrapByWindow )
		{
			xmax -= m_RightMarginWidth;
		}

		int xpos = xmax - m_ClientWidth;

		if( m_DrawingXPos < 0 || xpos <= 0 ) m_DrawingXPos = 0;
		else
			if( xpos > 0 && m_DrawingXPos >= xpos ) m_DrawingXPos = xpos - 1;
	}
	else
	{
		int xmax = HexModeMaxColumns * m_HexFontMaxDigitWidth;
		int xpos = xmax - m_ClientWidth;

		if( m_DrawingXPos < 0 || xpos <= 0 ) m_DrawingXPos = 0;
		else
			if( xpos > 0 && m_DrawingXPos >= xpos ) m_DrawingXPos = xpos - 1;
	}

	m_RepaintAll = true;
	Refresh( false );
	//evt.Skip();
}

void MadEdit::OnMouseWheel( wxMouseEvent &evt )
{
	if( m_SingleLineMode )
		return;

	// Alt is igored so far
	if( evt.ControlDown()) // inc/dec font size
	{
		if(! evt.ShiftDown() )
		{
			wxString name;
			int size;
			GetFont( name, size );

			if( evt.m_wheelRotation < 0 )
				SetFont( name, size - 1 );
			else
				SetFont( name, size + 1 );
			return;
		}
		else
		{
			int scrollsize = m_PageRowCount;
			if( evt.m_wheelRotation < 0 )
				m_TopRow += scrollsize;
			else
				m_TopRow -= scrollsize;
		}
	}
	else
	{
		if( evt.ShiftDown() )
		{
			int scrollsize = m_TextFontSpaceWidth/2; //Half char width
			if(evt.ControlDown())
				scrollsize = 80 * m_TextFontSpaceWidth; // almost 80 columns
			if( evt.m_wheelRotation < 0 )
				m_DrawingXPos += scrollsize;
			else
				m_DrawingXPos -= scrollsize;
		}
		else
		{
			if( evt.m_wheelRotation < 0 )
				m_TopRow += 3;
			else
				m_TopRow -= 3;
		}
	}

	// for (Ctrl(+Shift)+)Wheel
	UpdateScrollBarPos();
	m_RepaintAll = true;
	Refresh( false );
	// end for (Ctrl(+Shift)+)Wheel
	//evt.Skip();
}

void MadEdit::OnMouseEnterWindow( wxMouseEvent &evt )
{
	m_MouseInWindow = true;
	UpdateCursor( evt.m_x, evt.m_y );
	evt.Skip();
}

void MadEdit::OnMouseLeaveWindow( wxMouseEvent &evt )
{
	m_MouseInWindow = false;
	if( m_DragDrop )
	{
		wxSetCursor( DragNoneCursor );
	}
	else
	{
		wxSize sz = GetClientSize();

		if( evt.m_x >= sz.x || evt.m_y >= sz.y )
		{
			wxSetCursor( wxNullCursor ); // reset to default
		}
		else
		{
			SetCursor( ArrowCursor ); // scrollbar
		}
	}

	evt.Skip();
}

void MadEdit::OnMouseCaptureLost( wxMouseCaptureLostEvent & WXUNUSED(evt) )
{
	DBOUT( "CaptureLost\n" );

	if( m_MouseLeftDown )
	{
		if( !m_DragDrop )
		{
			EndUpdateSelection( true );
		}
	}

	if( GetCapture() == this )
		ReleaseMouse();

	m_MouseLeftDoubleClick = false;
	m_MouseLeftDown = false;
	m_DragDrop = false;
	m_MouseAtHexTextArea = false;
	m_DragCopyFlag = false;//default move

	if( m_MouseMotionTimer->IsRunning() )
	{
		m_MouseMotionTimer->Stop();
	}

	//evt.Skip();
}

void MadEdit::UpdateCursor( int mouse_x, int mouse_y )
{
	if( m_EditMode != emHexMode )
	{
		if( mouse_x > ( m_LineNumberAreaWidth + m_BookmarkWidth ) && mouse_x <= m_ClientWidth && mouse_y <= m_ClientHeight )
		{
			if( m_DragDrop )
			{
				if( m_DragCopyFlag )
				{
					//wxSetCursor( DragCopyCursor );
					SetCursor(DragCopyCursor);
				}
				else
				{
					//wxSetCursor( DragMoveCursor );
					SetCursor(DragMoveCursor);
				}
			}
			else
			{
				//wxSetCursor( IBeamCursor );
				SetCursor(IBeamCursor);
			}
		}
		else
		{
			if( mouse_x >= 0 && mouse_x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) )
			{
				SetCursor( RightArrowCursor );
			}
			else
				SetCursor( ArrowCursor );

			//SetCursor(ArrowCursor);
		}
	}
	else
	{
		if( ( mouse_x + m_DrawingXPos ) > ( 9 * m_HexFontMaxDigitWidth ) && mouse_x <= m_ClientWidth && mouse_y > m_RowHeight    && mouse_y <= m_ClientHeight )
		{
			if( m_DragDrop )
			{
				if( m_DragCopyFlag )
				{
					//wxSetCursor( DragCopyCursor );
					SetCursor(DragCopyCursor);
				}
				else
				{
					//wxSetCursor( DragMoveCursor );
					SetCursor(DragMoveCursor);
				}
			}
			else
			{
				//wxSetCursor( IBeamCursor );
				SetCursor(IBeamCursor);
			}
		}
		else
		{
			//wxSetCursor( ArrowCursor );
			SetCursor(ArrowCursor);
		}
	}
}

void MadEdit::OnEraseBackground( wxEraseEvent & WXUNUSED(evt))
{
	// do nothing
}

void MadEdit::MadEditOnPaint( wxPaintEvent *evt /*=NULL*/  )
{
	//Patch from http://linuxtoy.org/archives/madedit.html
	//"segmentation fault " in Ubuntu 9.10 while open 2 files might be occurred when using iBus. I changed the default  IM to gcin by im-switch command,
	//and the problem is solved.
	//because the in the MadEdit::OnPaint function will be call when m_ClientBitmap=nullptr.
	//I change this by add follow code in the front of the MadEdit::OnPaint like this:
	//if(mClientWidth==0 || mClientHeight==0) return;
	wxPaintDC dc( this );
	wxMemoryDC memdc( &dc ), markdc( &dc );
	wxWindow *focuswin = FindFocus();

	if( nullptr == m_ClientBitmap )
	{
		wxSizeEvent evttmp;
		OnSize( evttmp );
	}

	if( InPrinting() )
	{
		switch( m_LastPaintBitmap )
		{
		case -1:// do nothing
			return;

		case 0:
			memdc.SelectObject( *m_ClientBitmap );
			break;

		case 1:
			memdc.SelectObject( *m_MarkBitmap );
			break;
		}

		dc.Blit( 0, 0, m_old_ClientWidth, m_old_ClientHeight, &memdc, 0, 0 );
	}
	else
	{
		if( m_EditMode != emHexMode )
		{
			if( m_ClientBitmap != nullptr )
			{
				memdc.SelectObject( *m_ClientBitmap );
			}

			memdc.SetFont( *m_TextFont );
			// calculate rows to paint
			if( m_TopRow < 0 ) m_TopRow = 0;
			if( m_Lines->m_RowCount < (size_t)m_TopRow )
			{
				wxASSERT(0);
				m_TopRow = m_Lines->m_RowCount - 1;
			}

			int rowcount = ( int )( m_Lines->m_RowCount - m_TopRow );

			if( rowcount > ( int )m_VisibleRowCount )
				rowcount = ( int )m_VisibleRowCount;

			const int lastrow = int( m_TopRow ) + rowcount - 1;

			if( m_RepaintAll )
			{
				// update ValidPos
				m_UpdateValidPos = -1;
				MadLineIterator lit;
				wxFileOffset tmppos = -1;
				int rowid = m_TopRow;
				int lineid = GetLineByRow( lit, tmppos, rowid );
				m_UpdateValidPos = 0;

				// update m_LineNumberAreaWidth
				if( m_DisplayLineNumber )
				{
					m_LineNumberAreaWidth = CalcLineNumberAreaWidth( lit, lineid, rowid, m_TopRow, rowcount );
				}
				else
				{
					m_LineNumberAreaWidth = 0;
				}

				// clear client area
				wxColor &bgcolor = m_Syntax->GetAttributes( aeText )->bgcolor;
				memdc.SetBrush( *( wxTheBrushList->FindOrCreateBrush( bgcolor ) ) );
				memdc.SetPen( *( wxThePenList->FindOrCreatePen( bgcolor, 1, wxPENSTYLE_SOLID ) ) );
				memdc.DrawRectangle( 0, 0, m_ClientWidth, m_ClientHeight );
				// paint rows
				wxRect rect( 0, 0, m_ClientWidth, m_ClientHeight );
				PaintTextLines( &memdc, rect, m_TopRow, rowcount, bgcolor );
				m_RepaintAll = false;
				m_RepaintSelection = false;
			}
			else
				if( m_RepaintSelection )
				{
					if( !( m_SelFirstRow > lastrow || m_SelLastRow < int( m_TopRow ) ) )
					{
						int firstrow = int( m_TopRow );
						wxRect rect( 0, 0, m_ClientWidth, m_ClientHeight );

						if( firstrow < m_SelFirstRow )
						{
							rect.y = m_RowHeight * ( m_SelFirstRow - firstrow );
							firstrow = m_SelFirstRow;
						}

						int lastrow1 = lastrow;

						if( lastrow1 > m_SelLastRow )
						{
							lastrow1 = m_SelLastRow;
						}

						int rows = ( int )( lastrow1 - firstrow + 1 );
						rect.height = rows * m_RowHeight;
						wxColor &bgcolor = m_Syntax->GetAttributes( aeText )->bgcolor;
						memdc.SetBrush( *( wxTheBrushList->FindOrCreateBrush( bgcolor ) ) );
						memdc.SetPen( *( wxThePenList->FindOrCreatePen( bgcolor, 1, wxPENSTYLE_SOLID ) ) );
						memdc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
						PaintTextLines( &memdc, rect, firstrow, rows, bgcolor );
					}

					m_RepaintSelection = false;
				}

			if( m_Selection )
			{
				m_SelFirstRow = int( m_SelectionBegin->rowid );
				m_SelLastRow = int( m_SelectionEnd->rowid );
			}
			else
			{
				m_SelFirstRow = INT_MAX;
				m_SelLastRow = -1;
			}

			bool bPaintMark = false;
			int SpacingHeight = m_RowHeight - m_TextFontHeight;

			if( m_MarkBracePair )
			{
				const int minleft = m_LineNumberAreaWidth + m_BookmarkWidth;
				const int maxright = m_ClientWidth;
				int h = m_TextFontHeight >= 4 ? 4 : m_TextFontHeight;
				int w = m_LeftBrace.Width;
				int x = m_LeftBrace.XPos + m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth -  m_DrawingXPos;

				if( m_LeftBrace_rowid >= m_TopRow && m_LeftBrace_rowid <= lastrow &&
						x < maxright && x + w > minleft )
				{
					int y = ( int )( m_LeftBrace_rowid - m_TopRow ) * ( SpacingHeight + m_TextFontHeight ) + ( SpacingHeight >> 1 ) + m_TextFontHeight - h;
					int dw = m_LineNumberAreaWidth + m_BookmarkWidth + 1 - x;

					if( dw > 0 )
					{
						w -= dw;
						x += dw;
					}

					if( w > 0 )
					{
						if( bPaintMark == false )
						{
							if( m_MarkBitmap != nullptr )
							{
								markdc.SelectObject( *m_MarkBitmap );
							}

							markdc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
							bPaintMark = true;
						}

						wxDC *pdc = &markdc;
						INVERT_RECT( pdc, x, y, w, h );
					}
				}

				w = m_RightBrace.Width;
				x = m_RightBrace.XPos + m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth - m_DrawingXPos;

				if( m_RightBrace_rowid >= m_TopRow && m_RightBrace_rowid <= lastrow &&
						x < maxright && x + w > minleft )
				{
					int y = ( int )( m_RightBrace_rowid - m_TopRow ) * ( SpacingHeight + m_TextFontHeight ) + ( SpacingHeight >> 1 )    + m_TextFontHeight - h;
					int dw = m_LineNumberAreaWidth + m_BookmarkWidth + 1 - x;

					if( dw > 0 )
					{
						w -= dw;
						x += dw;
					}

					if( w > 0 )
					{
						if( bPaintMark == false )
						{
							markdc.SelectObject( *m_MarkBitmap );
							markdc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
							bPaintMark = true;
						}

						wxDC *pdc = &markdc;
						INVERT_RECT( pdc, x, y, w, h );
					}
				}
			}

			if( m_MarkActiveLine && m_CaretPos.rowid >= m_TopRow
					&& m_CaretPos.rowid < m_TopRow + m_VisibleRowCount )
			{
				int x = m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth - 1 - m_DrawingXPos;

				if( x <= ( m_LineNumberAreaWidth + m_BookmarkWidth ) )
				{
					if( ( x = ( m_LineNumberAreaWidth + m_BookmarkWidth ) ) != 0 )
					{
						++x;
					}
				}

				int y = ( int )( m_CaretPos.rowid - m_TopRow ) * ( SpacingHeight + m_TextFontHeight ) +
						( SpacingHeight >> 1 );
				int w = m_ClientWidth - x;
				int h = m_TextFontHeight + 1;

				if( bPaintMark == false )
				{
					markdc.SelectObject( *m_MarkBitmap );
					markdc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
					bPaintMark = true;
				}

				wxColor &c = m_Syntax->GetAttributes( aeActiveLine )->color;
				markdc.SetPen( *( wxThePenList->FindOrCreatePen( c, m_ActiveLMSize, wxPENSTYLE_SOLID ) ) );
				markdc.SetBrush( *wxTRANSPARENT_BRUSH );
				markdc.DrawRectangle( x, y, w, h );
			}

#ifndef __WXMSW__
			else
				if( bPaintMark == false )
				{
					markdc.SelectObject( *m_MarkBitmap );
					markdc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
					bPaintMark = true;
				}

#endif

			if( bPaintMark )
			{
#ifndef __WXMSW__

				if( focuswin == this )
				{
					DisplayCaret( false );       // paint a caret in the MarkDC
					wxCaretNew *caret = dynamic_cast< wxCaretNew* >(GetCaret());

					if( caret->IsVisible() )
					{
						caret->DoDraw( &markdc );
					}
				}

				//wxCaretSuspend cs(this);
#endif
				dc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &markdc, 0, 0 );
				m_LastPaintBitmap = 1;
			}
			else
			{
				dc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
				m_LastPaintBitmap = 0;
			}
		}
		else // HexMode
		{
			memdc.SelectObject( *m_ClientBitmap );

			if( m_RepaintAll || m_RepaintSelection )
			{
				int count = GetVisibleHexRowCount();

				if( count == 0 )
					++count;

				if( m_TopRow != m_HexTopRow || count != m_HexRowCount )
					PrepareHexRowIndex( m_TopRow, count );

				// clear client area
				wxColor &bgcolor = m_Syntax->GetAttributes( aeText )->bgcolor;
				memdc.SetBrush( *( wxTheBrushList->FindOrCreateBrush( bgcolor ) ) );
				memdc.SetPen( *( wxThePenList->FindOrCreatePen( bgcolor, 1, wxPENSTYLE_SOLID ) ) );
				memdc.DrawRectangle( 0, 0, m_ClientWidth, m_ClientHeight );

				if( m_HexDigitBitmap == nullptr )
				{
					m_HexDigitBitmap = new wxBitmap( m_HexFontMaxDigitWidth * 76, m_RowHeight * 3 );
					memdc.SelectObject( *m_HexDigitBitmap );
					memdc.SetFont( *m_HexFont );

					for( int i = 0; i < 76; ++i )
						m_WidthBuffer[i] = m_HexFontMaxDigitWidth;

					// first line: aeText hexdigit
					memdc.DrawRectangle( 0, 0, m_HexFontMaxDigitWidth * 17, m_RowHeight );
					m_Syntax->nw_Color = m_Syntax->GetAttributes( aeText )->color;
					memdc.SetTextForeground( m_Syntax->nw_Color );
					PaintText( &memdc, 0, 0, &HexHeader[60], m_WidthBuffer, 17, 0, 9999999 );
					// second line: aeLineNumberArea hexdigit
					m_Syntax->SetAttributes( aeLineNumber );
					memdc.SetPen( *( wxThePenList->FindOrCreatePen( m_Syntax->nw_BgColor, 1, wxPENSTYLE_SOLID ) ) );
					memdc.SetBrush( *( wxTheBrushList->FindOrCreateBrush( m_Syntax->nw_BgColor ) ) );
					memdc.DrawRectangle( 0, m_RowHeight, m_HexFontMaxDigitWidth * 76, m_RowHeight * 2 );
					memdc.SetTextForeground( m_Syntax->nw_Color );
					PaintText( &memdc, 0, m_RowHeight, &HexHeader[60], m_WidthBuffer, 18, 0, 9999999 );
					// third line: hexheader
					PaintText( &memdc, 0, m_RowHeight * 2, HexHeader, m_WidthBuffer, 76, 0, 9999999 );
					memdc.SelectObject( *m_ClientBitmap );
				}

				markdc.SelectObject( *m_HexDigitBitmap );
				m_HexDigitDC = &markdc;
				wxRect rect( -m_DrawingXPos, 0, m_ClientWidth, m_ClientHeight );
				PaintHexLines( &memdc, rect, m_TopRow, ( int )count, true );
				m_HexDigitDC = nullptr;
				m_RepaintAll = false;
				m_RepaintSelection = false;
			}

#ifndef __WXMSW__

			if( focuswin == this )
			{
				DisplayCaret( false );       // paint a caret in the m_ClientBitmap
				wxCaretNew *caret = dynamic_cast< wxCaretNew* > (GetCaret());

				if( caret->IsVisible() )
				{
					caret->DoDraw( &memdc );
				}
			}

			wxCaretSuspend cs( this );
#endif
			dc.Blit( 0, 0, m_ClientWidth, m_ClientHeight, &memdc, 0, 0 );
			m_LastPaintBitmap = 0;
		}
	}

	if( focuswin == this )
	{
		DisplayCaret( false );
	}

#ifdef __WXMSW__
	else
		if( m_Painted == false )
		{
			DisplayCaret( false );
		}

#endif
	m_Painted = true;
}

void MadEdit::DoSelectionChanged()
{
	if( m_OnSelectionChanged )
		m_OnSelectionChanged( this );
}

void MadEdit::DoStatusChanged()
{
	if( m_OnStatusChanged )
		m_OnStatusChanged( this );
}

void MadEdit::DoToggleWindow()
{
	if( m_OnToggleWindow )
		m_OnToggleWindow( this );
}

void MadEdit::DoMouseRightUp()
{
	if( m_OnMouseRightUp )
		m_OnMouseRightUp( this );
}

void MadEdit::DoActivate()
{
	if( m_OnActivate )
		m_OnActivate( this );
}

#ifdef __WXMSW__
struct IMEAdjuster
{
	IMEAdjuster( HWND hWnd, const wxPoint & pos ): m_hWnd( hWnd ), m_hImc( ImmGetContext( hWnd ) ), m_pos( pos ) {}

	void InitStatus(const wxFont& font) const
	{
		if (m_hImc == (HIMC)nullptr || ImmGetOpenStatus(m_hImc) == 0)
			return;

#if 0 //WXWIN_COMPATIBILITY_3_0
		LOGFONT lf;
		wxFillLogFont(&lf, &font);
		ImmSetCompositionFont(m_hImc, &lf);
#else
		wxNativeFontInfo fontInfo;
		fontInfo.InitFromFont(font);
		ImmSetCompositionFont(m_hImc, &fontInfo.lf);
#endif

		UpdatePosition();
	}

	void UpdatePosition() const
	{
		if (m_hImc == (HIMC)nullptr)
			return;

		COMPOSITIONFORM cfs;
		cfs.dwStyle = CFS_POINT;
		cfs.ptCurrentPos.x = m_pos.x;
		cfs.ptCurrentPos.y = m_pos.y;
		ImmSetCompositionWindow(m_hImc, &cfs);
	}

	~IMEAdjuster()
	{
		ImmReleaseContext(m_hWnd, m_hImc);
	}
private:
	const wxPoint& m_pos;
	HWND m_hWnd;
	HIMC m_hImc;
};

WXLRESULT MadEdit::MSWWindowProc( WXUINT message, WXWPARAM wParam, WXLPARAM lParam )
{
	switch( message )
	{
	case WM_CUT:
		CutToClipboard();
		return TRUE;

	case WM_COPY:
		CopyToClipboard();
		return TRUE;

	case WM_PASTE:
		PasteFromClipboard();
		return TRUE;

	case WM_IME_NOTIFY:
		if (wParam == IMN_SETOPENSTATUS)
		{
			IMEAdjuster((HWND)GetHWND(), m_caret->GetPosition()).InitStatus(m_font);
		}
		break;

	case WM_IME_COMPOSITION:
		{
			IMEAdjuster((HWND)GetHWND(), m_caret->GetPosition()).UpdatePosition();
		}
		break;

	case WM_PAINT: // for Mouse-Over-Get-Word of Dr.Eye & StarDict
		{
			RECT rc;
			GetUpdateRect( ( HWND )GetHWND(), &rc, false );
			int x = rc.left;
			int y = rc.top;
			int h = rc.bottom - y, w = rc.right - x;

			if( h == 1 )
			{
				wxSize sz = GetClientSize();

				//((wxFrame*)wxTheApp->GetTopWindow())->SetTitle(wxString::Format(wxT("%d %d %d d (%d %d)"), rc.left, rc.top, rc.right, rc.bottom, sz.x, sz.y));

				if( w <= 10 || // dr.eye w==8
						w == sz.x ) // stardic w==sz.x
				{
					bool paint = false;
					int row = y / m_RowHeight;

					if( m_EditMode != emHexMode )
					{
						if( row <= m_VisibleRowCount && ( m_TopRow + row ) < int( m_Lines->m_RowCount ) )
							paint = true;
					}
					else
					{
						if( row > 0 && row - 1 <= GetVisibleHexRowCount() )
							paint = true;
					}

					if( paint )
					{
						/*wxClientDC dc( this );
						wxRect rect( 0, row * m_RowHeight, m_ClientWidth, m_RowHeight );

						if( m_EditMode != emHexMode )
						{
							PaintTextLines( &dc, rect, m_TopRow + row, 1, m_Syntax->GetAttributes( aeText )->bgcolor );
						}
						else
						{
							PaintHexLines( &dc, rect, m_TopRow + row - 1, 1, false );
						}

						m_RepaintAll = false;
						Refresh( false );*/
						wxPaintEvent evt;
						MadEditOnPaint( &evt);
						return 0;
					}
				}
			}
		}
		break;
		//case WM_NCPAINT:
		//{
		//DWORD dw = GetRegionData(*((HRGN*)&wParam), 0, nullptr);
		//if(dw)
		//{
		//static DWORD rgnsize = 0;
		//static RGNDATA *prgn = nullptr;
		//if(dw > rgnsize)
		//{
		//rgnsize = dw;
		//if(prgn) delete prgn;
		//prgn = (RGNDATA *) new wxByte[dw];
		//}
		//GetRegionData(*((HRGN*)&wParam), dw, prgn);
		//RECT rc=prgn->rdh.rcBound;
		//((wxFrame*)wxTheApp->GetTopWindow())->SetTitle(wxString::Format(wxT("%d %d %d %d"), rc.left, rc.top, rc.right, rc.bottom));
		//int x=rc.left;
		//int y=rc.top;
		//int h=rc.bottom-y, w=rc.right-x;
		//ScreenToClient(&x, &y);
		//if(x>=0 && y>=0 && h>0 && h<16 && w>0 && w<16)
		//{
		//bool paint=false;
		//int row = y / m_RowHeight;
		//if(m_EditMode != emHexMode)
		//{
		//if(row <= m_VisibleRowCount && (m_TopRow + row) < int(m_Lines->m_RowCount))
		//paint = true;
		//}
		//else
		//{
		//if(row > 0 && row-1 <= GetVisibleHexRowCount())
		//paint = true;
		//}
		//if(paint)
		//{
		//wxClientDC dc(this);
		//wxRect rect(0,row * m_RowHeight, m_ClientWidth,m_RowHeight);
		//if(m_EditMode != emHexMode)
		//{
		//PaintTextLines(&dc, rect, m_TopRow+row, 1, m_Syntax->GetAttributes(aeText)->bgcolor);
		//}
		//else
		//{
		//PaintHexLines(&dc, rect, m_TopRow+row-1, 1, false);
		//}
		//m_RepaintAll=true;
		//Refresh(false);
		//return TRUE;
		//}
		//}
		//}
		//}
		//break;
	}

	return MadEditSuperClass::MSWWindowProc( message, wParam, lParam );
}
#endif

//==================================================

void MadEdit::SetCaretType( MadCaretType type )
{
	m_CaretType = type;

	switch( type )
	{
	case ctVerticalLine:
		if( m_EditMode != emHexMode )
			GetCaret()->SetSize( 2, m_TextFontHeight );
		else
			GetCaret()->SetSize( 2, m_HexFontHeight );

		break;

	case ctHorizontalLine:
		if( m_EditMode != emHexMode )
			GetCaret()->SetSize( m_TextFontAveCharWidth, 2 );
		else
			GetCaret()->SetSize( m_HexFontMaxDigitWidth, 2 );

		break;

	case ctBlock:
		if( m_EditMode != emHexMode )
			GetCaret()->SetSize( m_TextFontAveCharWidth, m_TextFontHeight );
		else
			GetCaret()->SetSize( m_HexFontMaxDigitWidth, m_HexFontHeight );

		break;
	}

#if !defined(__WXMSW__)
	Refresh( false );
#endif

	if( FindFocus() == this )
		DisplayCaret( false );
}


void MadEdit::AppearCaret( bool middle )
{
	if( m_EditMode != emHexMode )
	{
		if( m_CaretPos.rowid < m_TopRow )
		{
			m_TopRow = m_CaretPos.rowid;

			if( middle )
			{
				m_TopRow -= ( m_CompleteRowCount / 2 );

				if( m_TopRow < 0 ) m_TopRow = 0;
			}

			m_RepaintAll = true;
			Refresh( false );
		}
		else
		{
			if( m_CaretPos.rowid >= ( m_TopRow + m_CompleteRowCount ) )
			{
				if( middle ) m_TopRow = m_CaretPos.rowid - ( m_CompleteRowCount / 2 );
				else       m_TopRow = m_CaretPos.rowid - m_CompleteRowCount + 1;

				m_RepaintAll = true;
				Refresh( false );
			}
		}

		if( m_CaretPos.xpos < m_DrawingXPos )
		{
			m_DrawingXPos = m_CaretPos.xpos;
			m_RepaintAll = true;
			Refresh( false );
		}
		else
		{
			int right = m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth + m_CaretPos.xpos -
						m_DrawingXPos + m_TextFontMaxDigitWidth + 2;
			int x = right - m_ClientWidth;

			if( x > 0 )
			{
				m_DrawingXPos += x;
				m_RepaintAll = true;
				Refresh( false );
			}
		}
	}
	else
	{
		AppearHexRow( m_CaretPos.pos );
		int xpos;

		if( m_CaretAtHexArea )
		{
			xpos = int( m_CaretPos.pos & 0x0F ) * 3 + 10;

			if( m_CaretAtHalfByte )
			{
				if( m_CaretPos.pos == m_Lines->m_Size )
				{
					m_CaretAtHalfByte = false;
				}
				else
				{
					++xpos;
				}
			}

			xpos *= m_HexFontMaxDigitWidth;
		}
		else                        // at TextArea
		{
			xpos = m_TextAreaXPos;
		}

		if( xpos < m_DrawingXPos )
		{
			m_DrawingXPos = xpos;
			m_RepaintAll = true;
			Refresh( false );
		}
		else
		{
			int x = xpos - m_DrawingXPos;
			int cw = m_ClientWidth - ( m_HexFontMaxDigitWidth << 1 );

			if( x > cw )
			{
				m_DrawingXPos += ( x - cw );
				m_RepaintAll = true;
				Refresh( false );
			}
		}
	}

	ShowCaret( true );
}

void MadEdit::ShowCaret( bool show )
{
	FindBracePairUnderCaretPos();

	if( !show )
	{
		wxCaret *caret = GetCaret();

		if( caret->IsVisible() )
		{
			caret->Show( false );
		}
	}
	else
	{
		if( FindFocus() == this )
		{
#ifndef __WXMSW__
			Refresh( false );   // always DisplayCaret() in OnPaint()
#else

			if( m_MarkActiveLine )
			{
				Refresh( false );
			}
			else
			{
				DisplayCaret( false );
			}

#endif
		}
	}
}

void MadEdit::DisplayCaret( bool moveonly )
{
	wxCaret *caret = GetCaret();

	if( m_EditMode != emHexMode )
	{
		const int xpos = m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth + m_CaretPos.xpos -  m_DrawingXPos;
		const int ypos = ( int )( m_CaretPos.rowid - m_TopRow ) * m_RowHeight + ( ( m_RowHeight - m_TextFontHeight ) >> 1 );

		if( m_CaretPos.xpos + m_LeftMarginWidth > m_DrawingXPos &&
				xpos >= ( m_LineNumberAreaWidth + m_BookmarkWidth ) && xpos < m_ClientWidth && ypos >= 0 && ypos < m_ClientHeight )
		{
			caret->Move( xpos, ypos );
//#ifdef __WXGTK__
#if 0
			extern GtkIMContext *GetWindowIMContext( wxWindow * win );
			GdkRectangle rect = {xpos, ypos + m_TextFontHeight, 0, 0};
			gtk_im_context_set_cursor_location( GetWindowIMContext( this ), &rect );
#endif

			if( !moveonly )
			{
				if( !caret->IsVisible() )
					caret->Show();
			}
		}
		else
			if( caret->IsVisible() )
			{
				caret->Show( false );
			}
	}
	else
	{
		int row = ( m_CaretPos.pos >> 4 );

		if( row >= m_TopRow )
		{
			int xpos;

			if( m_CaretAtHexArea )
			{
				xpos = int( m_CaretPos.pos & 0x0F ) * 3 + 10;

				if( m_CaretAtHalfByte )
					++xpos;

				xpos *= m_HexFontMaxDigitWidth;
			}
			else
			{
				xpos = m_TextAreaXPos;
			}

			xpos -= m_DrawingXPos;
			int ypos = ( int )( row - m_TopRow + 1 ) * m_RowHeight;
			caret->Move( xpos, ypos );
//#ifdef __WXGTK__
#if 0
			extern GtkIMContext *GetWindowIMContext( wxWindow * win );
			GdkRectangle rect = {xpos, ypos + m_RowHeight, 0, 0};
			gtk_im_context_set_cursor_location( GetWindowIMContext( this ), &rect );
#endif

			if( !moveonly )
			{
				if( !caret->IsVisible() )
					caret->Show();
			}
		}
		else
			if( caret->IsVisible() )
			{
				caret->Show( false );
			}
	}
}

int MadEdit::CalcLineNumberAreaWidth( MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount )
{
	int LineNumberAreaWidth = GetLineNumberAreaWidth( lineid );

	if( LineNumberAreaWidth != GetLineNumberAreaWidth( lineid + rowcount ) )
	{
		// find last lineid
		int count = rowcount + ( toprow - rowid );
		++lineid;

		while( ( count -= int( lit->RowCount() ) ) > 0 )
		{
			++lit;
			++lineid;
		}

		LineNumberAreaWidth = GetLineNumberAreaWidth( lineid );
	}

	return LineNumberAreaWidth;
}

int MadEdit::GetLineNumberAreaWidth( int number )
{
	if( m_DisplayLineNumber )
	{
		int n = 4;

		while( number >= 10000 )
		{
			number /= 10;
			++n;
		}

		return ( n * m_TextFontMaxDigitWidth ) + ( m_TextFontMaxDigitWidth >> 2 );
	}

	return 0;
}

int MadEdit::GetMaxWordWrapWidth()
{
	int maxwidth = ( ( unsigned int )( -1 ) ) / 4;

	switch( m_WordWrapMode )
	{
	case wwmWrapByWindow:
		{
			maxwidth = m_ClientWidth - ( GetLineNumberAreaWidth( m_Lines->m_Size >> 5 ) + m_BookmarkWidth +
										 m_LeftMarginWidth + m_RightMarginWidth + m_TextFontAveCharWidth );
		}
		break;

	case wwmWrapByColumn:
		maxwidth = m_MaxColumns * m_TextFontAveCharWidth;
		break;

	default: break;
	}

	if( maxwidth < 8 * m_TextFontAveCharWidth )
		return 8 * m_TextFontAveCharWidth;

	return maxwidth;
}

int MadEdit::GetUCharWidth( ucs4_t uc )
{
	if( uc > 0x10FFFF || uc < 0 ) uc = '?';

	__REGISTER int idx = uc >> 16;
	wxUint16 *widths = m_TextFontWidths[idx];

	if( widths == nullptr )
	{
		widths = m_TextFontWidths[idx] = FontWidthManager::GetFontWidths( idx, m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), this );
	}

	idx = uc & 0xFFFF;
	int w;
	wxUint16 *pw16 = widths + idx;

	if( ( w = *pw16 ) == 0 )
	{
		wxChar wcs[3];
#ifdef __WXMSW__

		if( uc < 0x10000 )
		{
			wcs[0] = wxChar( uc );
			wcs[1] = 0;
		}
		else
		{
			m_Encoding->UCS4toUTF16LE_U10000( uc, ( wxByte* )wcs );
			wcs[2] = 0;
		}

#else
		wcs[0] = uc;
		wcs[1] = 0;
#endif
		int h;
		GetTextExtent( wcs, &w, &h, nullptr, nullptr, m_TextFont );

		if( w <= 0 )
		{
			w = m_TextFontWidths[0][0x20];
		}

		*pw16 = w;
	}

	if( m_FixedWidthMode )
	{
		if( !HexPrinting() && w > ( m_TextFontAveCharWidth + ( m_TextFontAveCharWidth >> 2 ) ) )
		{
			return m_TextFontAveCharWidth << 1;
		}
		else
		{
			return m_TextFontAveCharWidth;
		}
	}

	return w;
}

int MadEdit::GetHexUCharWidth( ucs4_t uc )
{
	if( uc > 0x10FFFF || uc < 0 ) uc = '?';

	__REGISTER int idx = uc >> 16;
	wxUint16 *widths = m_HexFontWidths[idx];

	if( widths == nullptr )
	{
		widths = m_HexFontWidths[idx] = FontWidthManager::GetFontWidths( idx, m_HexFont->GetFaceName(), m_HexFont->GetPointSize(),   this );
	}

	idx = uc & 0xFFFF;
	int w;
	wxUint16 *pw16 = widths + idx;

	if( ( w = *pw16 ) == 0 )
	{
		wxChar wcs[3];
#ifdef __WXMSW__

		if( uc < 0x10000 )
		{
			wcs[0] = wxChar( uc );
			wcs[1] = 0;
		}
		else
		{
			m_Encoding->UCS4toUTF16LE_U10000( uc, ( wxByte* )wcs );
			wcs[2] = 0;
		}

#else
		wcs[0] = uc;
		wcs[1] = 0;
#endif
		int h;
		GetTextExtent( wcs, &w, &h, nullptr, nullptr, m_HexFont );

		if( w <= 0 )
		{
			w = m_HexFontWidths[0][0x20];
		}

		*pw16 = w;
	}

	return w;
}

int MadEdit::GetUCharType( ucs4_t uc )
{
	if( uc == 0x20 || uc == 0x09 )
		return 1;

	if( uc < 0x20 )
		return 2;

	if( m_Syntax->IsDelimiter( uc ) )
		return 3;

	if( uc < 0x100 )
		return 4;

	if( uc == 0x3000 )            // full-width white-space char
		return 5;

	if( uc < 0x4E00 )
		return 6;

	if( uc < 0xFB00 )
		return 7;

	return 8;
}

void MadEdit::OnCheckModificationTime( wxCommandEvent& WXUNUSED(evt) )
{
	DBOUT( "OnCheckModificationTime\n" );
	if(m_ModReloaded == false )
		ReloadByModificationTime();
}

void MadEdit::OnUpdateHScrollPos( wxCommandEvent& WXUNUSED(evt) )
{
	DBOUT( "OnUpdateHScrollPos\n" );
	AppearCaret();
	UpdateScrollBarPos();
}

void MadEdit::ShowZeroLenSelIndicator()
{
	if(m_ZeroLenSelIndicator == nullptr)
	{
		m_ZeroLenSelIndicator = new wxMenu( ( long )0 );
#ifdef __WXMSW__
		wxMenuItem * mit = m_ZeroLenSelIndicator->Append( wxNewId(),  _( "Zero Length Match" ) );
		mit->SetBitmap( wxBitmap( zerolen_xpm ) );
#else
		m_ZeroLenSelIndicator->Append( wxNewId(),  _( "^ Zero Length Match" ) );
#endif
	}

#ifdef __WXMSW__
	int xpos = m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth + m_CaretPos.xpos - m_DrawingXPos - 14;
#else
	int xpos = m_LineNumberAreaWidth + m_BookmarkWidth + m_LeftMarginWidth + m_CaretPos.xpos - m_DrawingXPos - (m_TextFontSpaceWidth+1)/2;
#endif
	int ypos = ( int )( m_CaretPos.rowid - m_TopRow ) * m_RowHeight + ( ( m_RowHeight - m_TextFontHeight ) >> 1 );
	if((ypos + 2 * m_RowHeight) >= m_MaxHeight) ypos -= m_RowHeight;
	else ypos += m_RowHeight;

	if(xpos <= 0) xpos = 0;
	if(ypos <= 0) ypos = 0;

	wxPoint pos(xpos, ypos);

	PopupMenu( m_ZeroLenSelIndicator, pos );
}

void MadEdit::ConfigNewDocument()
{
	long ll;
	bool bb;
	wxString enc(wxT("System Default")), ss = wxGetTranslation(wxT("Plain Text"));
	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath( wxT( "/MadEdit" ) );
	m_Config->Read( wxT( "NewDocumentLineEnding" ), &ll, nltDefault );
	m_NewLineType = (MadNewLineType)ll;
	m_InsertNewLineType = (MadNewLineType)ll;

	m_Config->Read( wxT( "NewDocumentEncoding" ), &enc);
	SetEncoding(enc);

	if(wxFONTENCODING_UTF8 == wxFontMapper::GetEncodingFromName( enc ))
	{
		m_Config->Read( wxT( "NewDocumentEncodingUTF8WithBOM" ), &bb, false );
		if(bb && (0 == m_Lines->m_LineList.begin()->m_RowIndices[0].m_Start))
		{
			ToggleBOM();
		}
	}

	m_Config->Read( wxT( "NewDocumentSyntax" ), &ss );
	if(MadSyntax::GetSyntaxByTitle( ss ) == nullptr) ss = wxGetTranslation(wxT("Plain Text"));
	SetSyntax(ss);
	ss.Empty();
	m_Config->Read( wxT( "NewDocumentTextFont" ), &ss );
	if (!ss.IsEmpty())
	{
		m_Config->Read(wxT("NewDocumentTextFontSize"), &ll, DEFAULT_FONT_SIZE);
		SetTextFont(ss, ll, true);
	}

	m_Config->SetPath( oldpath );
}

bool MadEdit::InsertString( const wxString & text )
{
	bool res = false;
	if( IsReadOnly() )
		return res;

	if( m_EditMode == emColumnMode )
	{
		vector < ucs4_t > ucs;
		int linecount = TranslateText( text.c_str(), text.Len(), &ucs, false );
		
		if( linecount != 0 )
		{
			switch( m_InsertNewLineType )
			{
			case nltDOS:
#ifdef __WXMSW__
			case nltDefault:
#endif
				ucs.push_back( 0x0D );
				ucs.push_back( 0x0A );
				break;

			case nltMAC:
				ucs.push_back( 0x0D );
				break;

			case nltUNIX:
#ifndef __WXMSW__
			case nltDefault:
#endif
				ucs.push_back( 0x0A );
				break;
			}
		}
		
		if( m_AutoFillColumnPaste && m_Selection )
		{
			int rowcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;
	
			if( rowcount > linecount )
			{
				rowcount -= linecount;
				linecount += rowcount;
				size_t i = 0;
	
				while( rowcount > 0 )
				{
					for( i = 0; i < ucs.size(); ++i )
					{
						ucs4_t uch = ucs.at( i );
	
						if( uch == 0x0D || uch == 0x0A )
						{
							--rowcount;
	
							switch( m_InsertNewLineType )
							{
							case nltDOS:
#ifdef __WXMSW__
							case nltDefault:
#endif
								ucs.push_back( 0x0D );
								ucs.push_back( 0x0A );
								break;
	
							case nltMAC:
								ucs.push_back( 0x0D );
								break;
	
							case nltUNIX:
#ifndef __WXMSW__
							case nltDefault:
#endif
								ucs.push_back( 0x0A );
								break;
							}
	
							if( i < ( ucs.size() - 1 ) )
							{
								if( uch == 0x0D && ucs.at( i + 1 ) == 0x0A )
									++i;
							}
						}
						else
							ucs.push_back( uch );
	
						if( rowcount == 0 )break;
					}
				}
			}
		}

		if( !ucs.empty() )
		{
			InsertColumnString( &ucs[0], ucs.size(), linecount, false, false );
			res = true;
		}
	}
	else
		if( m_EditMode == emHexMode && m_CaretAtHexArea )
		{
			vector < char >cs;
			vector<ucs4_t> ucs;
			TranslateText( text.c_str(), text.Len(), &ucs, false );
			
			if( ucs.size() > 0 )
			{
				MadMemData tempmem;
				MadBlock blk( &tempmem, -1, 0 );
				UCStoBlock( &ucs[0], ucs.size(), blk );
				size_t size = blk.m_Size;
				cs.resize( size );
				char *p = &cs[0];
			
				for( size_t i = 0; i < size; ++i, ++p )
				{
					*p = char( tempmem.Get( i ) );
				}
			}

			if( !cs.empty() )
			{
				InsertHexData( ( wxByte* )&cs[0], cs.size() );
				res = true;
			}
		}
		else //if(m_EditMode == emTextMode || !m_CaretAtHexArea)
		{
			vector < ucs4_t > ucs;
			TranslateText( text.c_str(), text.Len(), &ucs, false );
			size_t size = ucs.size();

			if( size )
			{
				//bool oldim = m_InsertMode;
				//m_InsertMode = true;
				InsertString( &ucs[0], size, false, true, false );
				res = true;
				//m_InsertMode = oldim;
			}
		}
	return res;
}
