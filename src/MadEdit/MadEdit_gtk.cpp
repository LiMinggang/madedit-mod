///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEdit_gtk.cpp
// Description: fixed GTK2 KeyPress event handler for CJK InputMethod
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

#include "MadEdit.h"
//#include <wx/gtk/private.h> // wxGTK-2.6.3 above
#include <gdk/gdkx.h>
#include <gdk/gdkprivate.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib-object.h>


// source code from:
/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/window.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: window.cpp,v 1.680 2006/12/03 13:59:40 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#if wxMAJOR_VERSION < 3
extern bool g_isIdle;
extern bool g_blockEventsOnDrag;
extern bool g_mainThreadLocked;
//-----------------------------------------------------------------------------
// debug
//-----------------------------------------------------------------------------

#ifdef __WXDEBUG__

#if wxUSE_THREADS
#   define DEBUG_MAIN_THREAD if (wxThread::IsMain() && g_mainThreadLocked) printf("gui reentrance");
#else
#   define DEBUG_MAIN_THREAD
#endif
#else
#define DEBUG_MAIN_THREAD
#endif // Debug

//-----------------------------------------------------------------------------
// "key_press_event" from any window
//-----------------------------------------------------------------------------

// These are used when transforming Ctrl-alpha to ascii values 1-26
inline bool wxIsLowerChar(int code)
{
    return (code >= 'a' && code <= 'z' );
}

inline bool wxIsUpperChar(int code)
{
    return (code >= 'A' && code <= 'Z' );
}


// set WXTRACE to this to see the key event codes on the console
#define TRACE_KEYS  _T("keyevent")

// translates an X key symbol to WXK_XXX value
//
// if isChar is true it means that the value returned will be used for EVT_CHAR
// event and then we choose the logical WXK_XXX, i.e. '/' for GDK_KP_Divide,
// for example, while if it is false it means that the value is going to be
// used for KEY_DOWN/UP events and then we translate GDK_KP_Divide to
// WXK_NUMPAD_DIVIDE
static long wxTranslateKeySymToWXKey(KeySym keysym, bool isChar)
{
    long key_code;

    switch ( keysym )
    {
        // Shift, Control and Alt don't generate the CHAR events at all
        case GDK_Shift_L:
        case GDK_Shift_R:
            key_code = isChar ? 0 : WXK_SHIFT;
            break;
        case GDK_Control_L:
        case GDK_Control_R:
            key_code = isChar ? 0 : WXK_CONTROL;
            break;
        case GDK_Meta_L:
        case GDK_Meta_R:
        case GDK_Alt_L:
        case GDK_Alt_R:
        case GDK_Super_L:
        case GDK_Super_R:
            key_code = isChar ? 0 : WXK_ALT;
            break;

        // neither do the toggle modifies
        case GDK_Scroll_Lock:
            key_code = isChar ? 0 : WXK_SCROLL;
            break;

        case GDK_Caps_Lock:
            key_code = isChar ? 0 : WXK_CAPITAL;
            break;

        case GDK_Num_Lock:
            key_code = isChar ? 0 : WXK_NUMLOCK;
            break;


        // various other special keys
        case GDK_Menu:
            key_code = WXK_MENU;
            break;

        case GDK_Help:
            key_code = WXK_HELP;
            break;

        case GDK_BackSpace:
            key_code = WXK_BACK;
            break;

        case GDK_ISO_Left_Tab:
        case GDK_Tab:
            key_code = WXK_TAB;
            break;

        case GDK_Linefeed:
        case GDK_Return:
            key_code = WXK_RETURN;
            break;

        case GDK_Clear:
            key_code = WXK_CLEAR;
            break;

        case GDK_Pause:
            key_code = WXK_PAUSE;
            break;

        case GDK_Select:
            key_code = WXK_SELECT;
            break;

        case GDK_Print:
            key_code = WXK_PRINT;
            break;

        case GDK_Execute:
            key_code = WXK_EXECUTE;
            break;

        case GDK_Escape:
            key_code = WXK_ESCAPE;
            break;

        // cursor and other extended keyboard keys
        case GDK_Delete:
            key_code = WXK_DELETE;
            break;

        case GDK_Home:
            key_code = WXK_HOME;
            break;

        case GDK_Left:
            key_code = WXK_LEFT;
            break;

        case GDK_Up:
            key_code = WXK_UP;
            break;

        case GDK_Right:
            key_code = WXK_RIGHT;
            break;

        case GDK_Down:
            key_code = WXK_DOWN;
            break;

        case GDK_Prior:     // == GDK_Page_Up
            key_code = WXK_PAGEUP;
            break;

        case GDK_Next:      // == GDK_Page_Down
            key_code = WXK_PAGEDOWN;
            break;

        case GDK_End:
            key_code = WXK_END;
            break;

        case GDK_Begin:
            key_code = WXK_HOME;
            break;

        case GDK_Insert:
            key_code = WXK_INSERT;
            break;


        // numpad keys
        case GDK_KP_0:
        case GDK_KP_1:
        case GDK_KP_2:
        case GDK_KP_3:
        case GDK_KP_4:
        case GDK_KP_5:
        case GDK_KP_6:
        case GDK_KP_7:
        case GDK_KP_8:
        case GDK_KP_9:
            key_code = (isChar ? '0' : WXK_NUMPAD0) + keysym - GDK_KP_0;
            break;

        case GDK_KP_Space:
            key_code = isChar ? ' ' : WXK_NUMPAD_SPACE;
            break;

        case GDK_KP_Tab:
            key_code = isChar ? WXK_TAB : WXK_NUMPAD_TAB;
            break;

        case GDK_KP_Enter:
            key_code = isChar ? WXK_RETURN : WXK_NUMPAD_ENTER;
            break;

        case GDK_KP_F1:
            key_code = isChar ? WXK_F1 : WXK_NUMPAD_F1;
            break;

        case GDK_KP_F2:
            key_code = isChar ? WXK_F2 : WXK_NUMPAD_F2;
            break;

        case GDK_KP_F3:
            key_code = isChar ? WXK_F3 : WXK_NUMPAD_F3;
            break;

        case GDK_KP_F4:
            key_code = isChar ? WXK_F4 : WXK_NUMPAD_F4;
            break;

        case GDK_KP_Home:
            key_code = isChar ? WXK_HOME : WXK_NUMPAD_HOME;
            break;

        case GDK_KP_Left:
            key_code = isChar ? WXK_LEFT : WXK_NUMPAD_LEFT;
            break;

        case GDK_KP_Up:
            key_code = isChar ? WXK_UP : WXK_NUMPAD_UP;
            break;

        case GDK_KP_Right:
            key_code = isChar ? WXK_RIGHT : WXK_NUMPAD_RIGHT;
            break;

        case GDK_KP_Down:
            key_code = isChar ? WXK_DOWN : WXK_NUMPAD_DOWN;
            break;

        case GDK_KP_Prior: // == GDK_KP_Page_Up
            key_code = isChar ? WXK_PAGEUP : WXK_NUMPAD_PAGEUP;
            break;

        case GDK_KP_Next: // == GDK_KP_Page_Down
            key_code = isChar ? WXK_PAGEDOWN : WXK_NUMPAD_PAGEDOWN;
            break;

        case GDK_KP_End:
            key_code = isChar ? WXK_END : WXK_NUMPAD_END;
            break;

        case GDK_KP_Begin:
            key_code = isChar ? WXK_HOME : WXK_NUMPAD_BEGIN;
            break;

        case GDK_KP_Insert:
            key_code = isChar ? WXK_INSERT : WXK_NUMPAD_INSERT;
            break;

        case GDK_KP_Delete:
            key_code = isChar ? WXK_DELETE : WXK_NUMPAD_DELETE;
            break;

        case GDK_KP_Equal:
            key_code = isChar ? '=' : WXK_NUMPAD_EQUAL;
            break;

        case GDK_KP_Multiply:
            key_code = isChar ? '*' : WXK_NUMPAD_MULTIPLY;
            break;

        case GDK_KP_Add:
            key_code = isChar ? '+' : WXK_NUMPAD_ADD;
            break;

        case GDK_KP_Separator:
            // FIXME: what is this?
            key_code = isChar ? '.' : WXK_NUMPAD_SEPARATOR;
            break;

        case GDK_KP_Subtract:
            key_code = isChar ? '-' : WXK_NUMPAD_SUBTRACT;
            break;

        case GDK_KP_Decimal:
            key_code = isChar ? '.' : WXK_NUMPAD_DECIMAL;
            break;

        case GDK_KP_Divide:
            key_code = isChar ? '/' : WXK_NUMPAD_DIVIDE;
            break;


        // function keys
        case GDK_F1:
        case GDK_F2:
        case GDK_F3:
        case GDK_F4:
        case GDK_F5:
        case GDK_F6:
        case GDK_F7:
        case GDK_F8:
        case GDK_F9:
        case GDK_F10:
        case GDK_F11:
        case GDK_F12:
            key_code = WXK_F1 + keysym - GDK_F1;
            break;

        default:
            key_code = 0;
    }

    return key_code;
}

static inline bool wxIsAsciiKeysym(KeySym ks)
{
    return ks < 256;
}

static void wxFillOtherKeyEventFields(wxKeyEvent& event,
                                      wxWindowGTK *win,
                                      GdkEventKey *gdk_event)
{
    int x = 0;
    int y = 0;
    GdkModifierType state;
    if (gdk_event->window)
        gdk_window_get_pointer(gdk_event->window, &x, &y, &state);

    event.SetTimestamp( gdk_event->time );
    event.SetId(win->GetId());
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK) != 0;
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK) != 0;
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK) != 0;
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK) != 0;
    event.m_scanCode = gdk_event->keyval;
    event.m_rawCode = (wxUint32) gdk_event->keyval;
    event.m_rawFlags = 0;
#if wxUSE_UNICODE
    event.m_uniChar = gdk_keyval_to_unicode(gdk_event->keyval);
#endif
    wxGetMousePosition( &x, &y );
    win->ScreenToClient( &x, &y );
    event.m_x = x;
    event.m_y = y;
    event.SetEventObject( win );
}


static bool
wxTranslateGTKKeyEventToWx(wxKeyEvent& event,
                           wxWindowGTK *win,
                           GdkEventKey *gdk_event)
{
    // VZ: it seems that GDK_KEY_RELEASE event doesn't set event->string
    //     but only event->keyval which is quite useless to us, so remember
    //     the last character from GDK_KEY_PRESS and reuse it as last resort
    //
    // NB: should be MT-safe as we're always called from the main thread only
    static struct
    {
        KeySym keysym;
        long   keycode;
    } s_lastKeyPress = { 0, 0 };

    KeySym keysym = gdk_event->keyval;

    wxLogTrace(TRACE_KEYS, _T("Key %s event: keysym = %ld"),
               event.GetEventType() == wxEVT_KEY_UP ? _T("release")
                                                    : _T("press"),
               keysym);

    long key_code = wxTranslateKeySymToWXKey(keysym, false /* !isChar */);

    if ( !key_code )
    {
        // do we have the translation or is it a plain ASCII character?
        if ( (gdk_event->length == 1) || wxIsAsciiKeysym(keysym) )
        {
            // we should use keysym if it is ASCII as X does some translations
            // like "I pressed while Control is down" => "Ctrl-I" == "TAB"
            // which we don't want here (but which we do use for OnChar())
            if ( !wxIsAsciiKeysym(keysym) )
            {
                keysym = (KeySym)gdk_event->string[0];
            }

            // we want to always get the same key code when the same key is
            // pressed regardless of the state of the modifiers, i.e. on a
            // standard US keyboard pressing '5' or '%' ('5' key with
            // Shift) should result in the same key code in OnKeyDown():
            // '5' (although OnChar() will get either '5' or '%').
            //
            // to do it we first translate keysym to keycode (== scan code)
            // and then back but always using the lower register
            Display *dpy = (Display *)wxGetDisplay();
            KeyCode keycode = XKeysymToKeycode(dpy, keysym);

            wxLogTrace(TRACE_KEYS, _T("\t-> keycode %d"), keycode);

            KeySym keysymNormalized = XKeycodeToKeysym(dpy, keycode, 0);

            // use the normalized, i.e. lower register, keysym if we've
            // got one
            key_code = keysymNormalized ? keysymNormalized : keysym;

            // as explained above, we want to have lower register key codes
            // normally but for the letter keys we want to have the upper ones
            //
            // NB: don't use XConvertCase() here, we want to do it for letters
            // only
            key_code = toupper(key_code);
        }
        else // non ASCII key, what to do?
        {
            // by default, ignore it
            key_code = 0;

            // but if we have cached information from the last KEY_PRESS
            if ( gdk_event->type == GDK_KEY_RELEASE )
            {
                // then reuse it
                if ( keysym == s_lastKeyPress.keysym )
                {
                    key_code = s_lastKeyPress.keycode;
                }
            }
        }

        if ( gdk_event->type == GDK_KEY_PRESS )
        {
            // remember it to be reused for KEY_UP event later
            s_lastKeyPress.keysym = keysym;
            s_lastKeyPress.keycode = key_code;
        }
    }

    wxLogTrace(TRACE_KEYS, _T("\t-> wxKeyCode %ld"), key_code);

    // sending unknown key events doesn't really make sense
    if ( !key_code )
        return false;

    // now fill all the other fields
    wxFillOtherKeyEventFields(event, win, gdk_event);

    event.m_keyCode = key_code;
#if wxUSE_UNICODE
    if ( gdk_event->type == GDK_KEY_PRESS ||  gdk_event->type == GDK_KEY_RELEASE )
    {
        event.m_uniChar = key_code;
    }
#endif

    return true;
}


struct wxGtkIMData
{
    GtkIMContext *context;
    GdkEventKey  *lastKeyEvent;

    wxGtkIMData()
    {
        context = gtk_im_multicontext_new();
        lastKeyEvent = nullptr;
    }
    ~wxGtkIMData()
    {
        g_object_unref (context);
    }
};

extern "C" {
static gboolean
gtk_window_key_press_callback( GtkWidget *widget,
                               GdkEventKey *gdk_event,
                               wxWindow *win )
{
    DEBUG_MAIN_THREAD

    // don't need to install idle handler, its done from "event" signal

    if (!win->m_hasVMT)
        return FALSE;
     if (g_blockEventsOnDrag)
        return FALSE;


    wxKeyEvent event( wxEVT_KEY_DOWN );
    bool ret = false;
    bool return_after_IM = false;

    // madedit: fixed that keyevent should be processed by IM first
    if( wxTranslateGTKKeyEventToWx(event, win, gdk_event) == false )
    {
        // Return after IM processing as we cannot do
        // anything with it anyhow.
        return_after_IM = true;
    }

    // 2005.01.26 modified by Hong Jen Yee (hzysoft@sina.com.tw):
    // When we get a key_press event here, it could be originate
    // from the current widget or its child widgets.  However, only the widget
    // with the INPUT FOCUS can generate the INITIAL key_press event.  That is,
    // if the CURRENT widget doesn't have the FOCUS at all, this event definitely
    // originated from its child widgets and shouldn't be passed to IM context.
    // In fact, what a GTK+ IM should do is filtering keyEvents and convert them
    // into text input ONLY WHEN THE WIDGET HAS INPUT FOCUS.  Besides, when current
    // widgets has both IM context and input focus, the event should be filtered
    // by gtk_im_context_filter_keypress().
    // Then, we should, according to GTK+ 2.0 API doc, return whatever it returns.
    if ((!ret) && (win->m_imData != nullptr) && ( wxWindow::FindFocus() == win ))
    {
        // We should let GTK+ IM filter key event first. According to GTK+ 2.0 API
        // docs, if IM filter returns true, no further processing should be done.
        // we should send the key_down event anyway.
        bool intercepted_by_IM = gtk_im_context_filter_keypress(win->m_imData->context, gdk_event);
        win->m_imData->lastKeyEvent = nullptr;
        if (intercepted_by_IM)
        {
            wxLogTrace(TRACE_KEYS, _T("Key event intercepted by IM"));
            return TRUE;
        }
    }

    if (return_after_IM)
        return FALSE;


    // madedit: fixed that keyevent should be processed by IM first
    // Emit KEY_DOWN event
    ret = win->GetEventHandler()->ProcessEvent( event );


#if wxUSE_ACCEL
    if (!ret)
    {
        wxWindowGTK *ancestor = win;
        while (ancestor)
        {
            int command = ancestor->GetAcceleratorTable()->GetCommand( event );
            if (command != -1)
            {
                wxCommandEvent command_event( wxEVT_COMMAND_MENU_SELECTED, command );
                ret = ancestor->GetEventHandler()->ProcessEvent( command_event );
                break;
            }
            if (ancestor->IsTopLevel())
                break;
            ancestor = ancestor->GetParent();
        }
    }
#endif // wxUSE_ACCEL

    // Only send wxEVT_CHAR event if not processed yet. Thus, ALT-x
    // will only be sent if it is not in an accelerator table.
    if (!ret)
    {
        long key_code;
        KeySym keysym = gdk_event->keyval;
        // Find key code for EVT_CHAR and EVT_CHAR_HOOK events
        key_code = wxTranslateKeySymToWXKey(keysym, true /* isChar */);
        if ( !key_code )
        {
            if ( wxIsAsciiKeysym(keysym) )
            {
                // ASCII key
                key_code = (unsigned char)keysym;
            }
            // gdk_event->string is actually deprecated
            else if ( gdk_event->length == 1 )
            {
                key_code = (unsigned char)gdk_event->string[0];
            }
        }

        if ( key_code )
        {
            wxLogTrace(TRACE_KEYS, _T("Char event: %ld"), key_code);

            event.m_keyCode = key_code;

            // To conform to the docs we need to translate Ctrl-alpha
            // characters to values in the range 1-26.
            if ( event.ControlDown() &&
                 ( wxIsLowerChar(key_code) || wxIsUpperChar(key_code) ))
            {
                if ( wxIsLowerChar(key_code) )
                    event.m_keyCode = key_code - 'a' + 1;
                if ( wxIsUpperChar(key_code) )
                    event.m_keyCode = key_code - 'A' + 1;
#if wxUSE_UNICODE
                event.m_uniChar = event.m_keyCode;
#endif
            }

            // Implement OnCharHook by checking ancestor top level windows
            wxWindow *parent = win;
            while (parent && !parent->IsTopLevel())
                parent = parent->GetParent();
            if (parent)
            {
                event.SetEventType( wxEVT_CHAR_HOOK );
                ret = parent->GetEventHandler()->ProcessEvent( event );
            }

            if (!ret)
            {
                event.SetEventType(wxEVT_CHAR);
                ret = win->GetEventHandler()->ProcessEvent( event );
            }
        }
    }

    // win is a control: tab can be propagated up
    if ( !ret &&
         ((gdk_event->keyval == GDK_Tab) || (gdk_event->keyval == GDK_ISO_Left_Tab)) &&
// VZ: testing for wxTE_PROCESS_TAB shouldn't be done here - the control may
//     have this style, yet choose not to process this particular TAB in which
//     case TAB must still work as a navigational character
// JS: enabling again to make consistent with other platforms
//     (with wxTE_PROCESS_TAB you have to call Navigate to get default
//     navigation behaviour)
#if wxUSE_TEXTCTRL
         (! (win->HasFlag(wxTE_PROCESS_TAB) && win->IsKindOf(CLASSINFO(wxTextCtrl)) )) &&
#endif
         win->GetParent() && (win->GetParent()->HasFlag( wxTAB_TRAVERSAL)) )
    {
        wxNavigationKeyEvent new_event;
        new_event.SetEventObject( win->GetParent() );
        // GDK reports GDK_ISO_Left_Tab for SHIFT-TAB
        new_event.SetDirection( (gdk_event->keyval == GDK_Tab) );
        // CTRL-TAB changes the (parent) window, i.e. switch notebook page
        new_event.SetWindowChange( (gdk_event->state & GDK_CONTROL_MASK) );
        new_event.SetCurrentFocus( win );
        ret = win->GetParent()->GetEventHandler()->ProcessEvent( new_event );
    }

    return ret;
}
}

//-----------------------------------------------------------------------------
// "key_release_event" from any window
//-----------------------------------------------------------------------------

extern "C" {
static gboolean
gtk_window_key_release_callback( GtkWidget *widget,
                                 GdkEventKey *gdk_event,
                                 wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    // don't need to install idle handler, its done from "event" signal

    if (!win->m_hasVMT)
        return FALSE;

    if (g_blockEventsOnDrag)
        return FALSE;

    wxKeyEvent event( wxEVT_KEY_UP );
    if ( !wxTranslateGTKKeyEventToWx(event, win, gdk_event) )
    {
        // unknown key pressed, ignore (the event would be useless anyhow)
        return FALSE;
    }

    //[mad]return win->GTKProcessEvent(event);
    return win->GetEventHandler()->ProcessEvent(event);
}
}

GtkIMContext *GetWindowIMContext(wxWindow *win)
{
    return win->m_imData->context;
}
#else
#ifdef __WXGTK3__
#include <gdk/gdkkeysyms-compat.h>
#endif
static bool gs_isNewEvent;

#include <X11/XKBlib.h> 

//extern bool g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// debug
//-----------------------------------------------------------------------------

#if wxUSE_THREADS
#   define DEBUG_MAIN_THREAD \
        wxASSERT_MSG( !g_mainThreadLocked || !wxThread::IsMain(), \
                      "GUI reentrancy detected" );
#else
#   define DEBUG_MAIN_THREAD
#endif

// the trace mask used for the focus debugging messages
#define TRACE_FOCUS wxT("focus")

//-----------------------------------------------------------------------------
// "key_press_event" from any window
//-----------------------------------------------------------------------------

// set WXTRACE to this to see the key event codes on the console
#define TRACE_KEYS  wxT("keyevent")

// translates an X key symbol to WXK_XXX value
//
// if isChar is true it means that the value returned will be used for EVT_CHAR
// event and then we choose the logical WXK_XXX, i.e. '/' for GDK_KP_Divide,
// for example, while if it is false it means that the value is going to be
// used for KEY_DOWN/UP events and then we translate GDK_KP_Divide to
// WXK_NUMPAD_DIVIDE
static long wxTranslateKeySymToWXKey(KeySym keysym, bool isChar)
{
    long key_code;

    switch ( keysym )
    {
        // Shift, Control and Alt don't generate the CHAR events at all
        case GDK_Shift_L:
        case GDK_Shift_R:
            key_code = isChar ? 0 : WXK_SHIFT;
            break;
        case GDK_Control_L:
        case GDK_Control_R:
            key_code = isChar ? 0 : WXK_CONTROL;
            break;
        case GDK_Meta_L:
        case GDK_Meta_R:
        case GDK_Alt_L:
        case GDK_Alt_R:
        case GDK_Super_L:
        case GDK_Super_R:
            key_code = isChar ? 0 : WXK_ALT;
            break;

        // neither do the toggle modifies
        case GDK_Scroll_Lock:
            key_code = isChar ? 0 : WXK_SCROLL;
            break;

        case GDK_Caps_Lock:
            key_code = isChar ? 0 : WXK_CAPITAL;
            break;

        case GDK_Num_Lock:
            key_code = isChar ? 0 : WXK_NUMLOCK;
            break;


        // various other special keys
        case GDK_Menu:
            key_code = WXK_MENU;
            break;

        case GDK_Help:
            key_code = WXK_HELP;
            break;

        case GDK_BackSpace:
            key_code = WXK_BACK;
            break;

        case GDK_ISO_Left_Tab:
        case GDK_Tab:
            key_code = WXK_TAB;
            break;

        case GDK_Linefeed:
        case GDK_Return:
            key_code = WXK_RETURN;
            break;

        case GDK_Clear:
            key_code = WXK_CLEAR;
            break;

        case GDK_Pause:
            key_code = WXK_PAUSE;
            break;

        case GDK_Select:
            key_code = WXK_SELECT;
            break;

        case GDK_Print:
            key_code = WXK_PRINT;
            break;

        case GDK_Execute:
            key_code = WXK_EXECUTE;
            break;

        case GDK_Escape:
            key_code = WXK_ESCAPE;
            break;

        // cursor and other extended keyboard keys
        case GDK_Delete:
            key_code = WXK_DELETE;
            break;

        case GDK_Home:
            key_code = WXK_HOME;
            break;

        case GDK_Left:
            key_code = WXK_LEFT;
            break;

        case GDK_Up:
            key_code = WXK_UP;
            break;

        case GDK_Right:
            key_code = WXK_RIGHT;
            break;

        case GDK_Down:
            key_code = WXK_DOWN;
            break;

        case GDK_Prior:     // == GDK_Page_Up
            key_code = WXK_PAGEUP;
            break;

        case GDK_Next:      // == GDK_Page_Down
            key_code = WXK_PAGEDOWN;
            break;

        case GDK_End:
            key_code = WXK_END;
            break;

        case GDK_Begin:
            key_code = WXK_HOME;
            break;

        case GDK_Insert:
            key_code = WXK_INSERT;
            break;


        // numpad keys
        case GDK_KP_0:
        case GDK_KP_1:
        case GDK_KP_2:
        case GDK_KP_3:
        case GDK_KP_4:
        case GDK_KP_5:
        case GDK_KP_6:
        case GDK_KP_7:
        case GDK_KP_8:
        case GDK_KP_9:
            key_code = (isChar ? '0' : WXK_NUMPAD0) + keysym - GDK_KP_0;
            break;

        case GDK_KP_Space:
            key_code = isChar ? ' ' : WXK_NUMPAD_SPACE;
            break;

        case GDK_KP_Tab:
            key_code = isChar ? WXK_TAB : WXK_NUMPAD_TAB;
            break;

        case GDK_KP_Enter:
            key_code = isChar ? WXK_RETURN : WXK_NUMPAD_ENTER;
            break;

        case GDK_KP_F1:
            key_code = isChar ? WXK_F1 : WXK_NUMPAD_F1;
            break;

        case GDK_KP_F2:
            key_code = isChar ? WXK_F2 : WXK_NUMPAD_F2;
            break;

        case GDK_KP_F3:
            key_code = isChar ? WXK_F3 : WXK_NUMPAD_F3;
            break;

        case GDK_KP_F4:
            key_code = isChar ? WXK_F4 : WXK_NUMPAD_F4;
            break;

        case GDK_KP_Home:
            key_code = isChar ? WXK_HOME : WXK_NUMPAD_HOME;
            break;

        case GDK_KP_Left:
            key_code = isChar ? WXK_LEFT : WXK_NUMPAD_LEFT;
            break;

        case GDK_KP_Up:
            key_code = isChar ? WXK_UP : WXK_NUMPAD_UP;
            break;

        case GDK_KP_Right:
            key_code = isChar ? WXK_RIGHT : WXK_NUMPAD_RIGHT;
            break;

        case GDK_KP_Down:
            key_code = isChar ? WXK_DOWN : WXK_NUMPAD_DOWN;
            break;

        case GDK_KP_Prior: // == GDK_KP_Page_Up
            key_code = isChar ? WXK_PAGEUP : WXK_NUMPAD_PAGEUP;
            break;

        case GDK_KP_Next: // == GDK_KP_Page_Down
            key_code = isChar ? WXK_PAGEDOWN : WXK_NUMPAD_PAGEDOWN;
            break;

        case GDK_KP_End:
            key_code = isChar ? WXK_END : WXK_NUMPAD_END;
            break;

        case GDK_KP_Begin:
            key_code = isChar ? WXK_HOME : WXK_NUMPAD_BEGIN;
            break;

        case GDK_KP_Insert:
            key_code = isChar ? WXK_INSERT : WXK_NUMPAD_INSERT;
            break;

        case GDK_KP_Delete:
            key_code = isChar ? WXK_DELETE : WXK_NUMPAD_DELETE;
            break;

        case GDK_KP_Equal:
            key_code = isChar ? '=' : WXK_NUMPAD_EQUAL;
            break;

        case GDK_KP_Multiply:
            key_code = isChar ? '*' : WXK_NUMPAD_MULTIPLY;
            break;

        case GDK_KP_Add:
            key_code = isChar ? '+' : WXK_NUMPAD_ADD;
            break;

        case GDK_KP_Separator:
            // FIXME: what is this?
            key_code = isChar ? '.' : WXK_NUMPAD_SEPARATOR;
            break;

        case GDK_KP_Subtract:
            key_code = isChar ? '-' : WXK_NUMPAD_SUBTRACT;
            break;

        case GDK_KP_Decimal:
            key_code = isChar ? '.' : WXK_NUMPAD_DECIMAL;
            break;

        case GDK_KP_Divide:
            key_code = isChar ? '/' : WXK_NUMPAD_DIVIDE;
            break;


        // function keys
        case GDK_F1:
        case GDK_F2:
        case GDK_F3:
        case GDK_F4:
        case GDK_F5:
        case GDK_F6:
        case GDK_F7:
        case GDK_F8:
        case GDK_F9:
        case GDK_F10:
        case GDK_F11:
        case GDK_F12:
            key_code = WXK_F1 + keysym - GDK_F1;
            break;

        default:
            key_code = 0;
    }

    return key_code;
}

static inline bool wxIsAsciiKeysym(KeySym ks)
{
    return ks < 256;
}


static void wxFillOtherKeyEventFields(wxKeyEvent& event,
                                      wxWindowGTK *win,
                                      GdkEventKey *gdk_event)
{
    event.SetTimestamp( gdk_event->time );
    event.SetId(win->GetId());

    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK) != 0;
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK) != 0;
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK) != 0;
    event.m_metaDown = (gdk_event->state & GDK_META_MASK) != 0;

    // At least with current Linux systems, MOD5 corresponds to AltGr key and
    // we represent it, for consistency with Windows, which really allows to
    // use Ctrl+Alt as a replacement for AltGr if this key is not present, as a
    // combination of these two modifiers.
    if ( gdk_event->state & GDK_MOD5_MASK )
    {
        event.m_controlDown =
        event.m_altDown = true;
    }

    // Normally we take the state of modifiers directly from the low level GDK
    // event but unfortunately GDK uses a different convention from MSW for the
    // key events corresponding to the modifier keys themselves: in it, when
    // e.g. Shift key is pressed, GDK_SHIFT_MASK is not set while it is set
    // when Shift is released. Under MSW the situation is exactly reversed and
    // the modifier corresponding to the key is set when it is pressed and
    // unset when it is released. To ensure consistent behaviour between
    // platforms (and because it seems to make slightly more sense, although
    // arguably both behaviours are reasonable) we follow MSW here.
    //
    // Final notice: we set the flags to the desired value instead of just
    // inverting them because they are not set correctly (i.e. in the same way
    // as for the real events generated by the user) for wxUIActionSimulator-
    // produced events and it seems better to keep that class code the same
    // among all platforms and fix the discrepancy here instead of adding
    // wxGTK-specific code to wxUIActionSimulator.
    const bool isPress = gdk_event->type == GDK_KEY_PRESS;
    switch ( gdk_event->keyval )
    {
        case GDK_Shift_L:
        case GDK_Shift_R:
            event.m_shiftDown = isPress;
            break;

        case GDK_Control_L:
        case GDK_Control_R:
            event.m_controlDown = isPress;
            break;

        case GDK_Alt_L:
        case GDK_Alt_R:
            event.m_altDown = isPress;
            break;

        case GDK_Meta_L:
        case GDK_Meta_R:
        case GDK_Super_L:
        case GDK_Super_R:
            event.m_metaDown = isPress;
            break;
    }

    event.m_rawCode = (wxUint32) gdk_event->keyval;
    event.m_rawFlags = gdk_event->hardware_keycode;

    event.SetEventObject( win );
}

static bool
wxTranslateGTKKeyEventToWx(wxKeyEvent& event,
                           wxWindowGTK *win,
                           GdkEventKey *gdk_event)
{
    // VZ: it seems that GDK_KEY_RELEASE event doesn't set event->string
    //     but only event->keyval which is quite useless to us, so remember
    //     the last character from GDK_KEY_PRESS and reuse it as last resort
    //
    // NB: should be MT-safe as we're always called from the main thread only
    static struct
    {
        KeySym keysym;
        long   keycode;
    } s_lastKeyPress = { 0, 0 };

    KeySym keysym = gdk_event->keyval;

    wxLogTrace(TRACE_KEYS, wxT("Key %s event: keysym = %lu"),
               event.GetEventType() == wxEVT_KEY_UP ? wxT("release")
                                                    : wxT("press"),
               static_cast<unsigned long>(keysym));

    long key_code = wxTranslateKeySymToWXKey(keysym, false /* !isChar */);

    if ( !key_code )
    {
        // do we have the translation or is it a plain ASCII character?
        if ( (gdk_event->length == 1) || wxIsAsciiKeysym(keysym) )
        {
            // we should use keysym if it is ASCII as X does some translations
            // like "I pressed while Control is down" => "Ctrl-I" == "TAB"
            // which we don't want here (but which we do use for OnChar())
            if ( !wxIsAsciiKeysym(keysym) )
            {
                keysym = (KeySym)gdk_event->string[0];
            }

#ifdef GDK_WINDOWING_X11
            // we want to always get the same key code when the same key is
            // pressed regardless of the state of the modifiers, i.e. on a
            // standard US keyboard pressing '5' or '%' ('5' key with
            // Shift) should result in the same key code in OnKeyDown():
            // '5' (although OnChar() will get either '5' or '%').
            //
            // to do it we first translate keysym to keycode (== scan code)
            // and then back but always using the lower register
            Display *dpy = (Display *)wxGetDisplay();
            KeyCode keycode = XKeysymToKeycode(dpy, keysym);

            wxLogTrace(TRACE_KEYS, wxT("\t-> keycode %d"), keycode);

#ifdef HAVE_X11_XKBLIB_H
            KeySym keysymNormalized = XkbKeycodeToKeysym(dpy, keycode, 0, 0);
#else
            KeySym keysymNormalized = XKeycodeToKeysym(dpy, keycode, 0);
#endif

            // use the normalized, i.e. lower register, keysym if we've
            // got one
            key_code = keysymNormalized ? keysymNormalized : keysym;
#else
            key_code = keysym;
#endif

            // as explained above, we want to have lower register key codes
            // normally but for the letter keys we want to have the upper ones
            //
            // NB: don't use XConvertCase() here, we want to do it for letters
            // only
            key_code = toupper(key_code);
        }
        else // non ASCII key, what to do?
        {
            // by default, ignore it
            key_code = 0;

            // but if we have cached information from the last KEY_PRESS
            if ( gdk_event->type == GDK_KEY_RELEASE )
            {
                // then reuse it
                if ( keysym == s_lastKeyPress.keysym )
                {
                    key_code = s_lastKeyPress.keycode;
                }
            }
        }

        if ( gdk_event->type == GDK_KEY_PRESS )
        {
            // remember it to be reused for KEY_UP event later
            s_lastKeyPress.keysym = keysym;
            s_lastKeyPress.keycode = key_code;
        }
    }

    wxLogTrace(TRACE_KEYS, wxT("\t-> wxKeyCode %ld"), key_code);

    // sending unknown key events doesn't really make sense
    if ( !key_code )
        return false;

    event.m_keyCode = key_code;

#if wxUSE_UNICODE
    event.m_uniChar = gdk_keyval_to_unicode(key_code);
    if ( !event.m_uniChar && event.m_keyCode <= WXK_DELETE )
    {
        // Set Unicode key code to the ASCII equivalent for compatibility. E.g.
        // let RETURN generate the key event with both key and Unicode key
        // codes of 13.
        event.m_uniChar = event.m_keyCode;
    }
#endif // wxUSE_UNICODE

    // now fill all the other fields
    wxFillOtherKeyEventFields(event, win, gdk_event);

    return true;
}

namespace
{

// Send wxEVT_CHAR_HOOK event to the parent of the window and return true only
// if it was processed (and not skipped).
bool SendCharHookEvent(const wxKeyEvent& event, wxWindow *win)
{
    // wxEVT_CHAR_HOOK must be sent to allow the parent windows (e.g. a dialog
    // which typically closes when Esc key is pressed in any of its controls)
    // to handle key events in all of its children unless the mouse is captured
    // in which case we consider that the keyboard should be "captured" too.
    // Fixme: Assume g_captureWindow always nullptr because it's static in gtk/window.cpp
    //if ( !g_captureWindow )
    {
        wxKeyEvent eventCharHook(wxEVT_CHAR_HOOK, event);
        if ( win->HandleWindowEvent(eventCharHook)
                && !event.IsNextEventAllowed() )
            return true;
    }

    return false;
}

// Adjust wxEVT_CHAR event key code fields. This function takes care of two
// conventions:
// (a) Ctrl-letter key presses generate key codes in range 1..26
// (b) Unicode key codes are same as key codes for the codes in 1..255 range
void AdjustCharEventKeyCodes(wxKeyEvent& event)
{
    const int code = event.m_keyCode;

    // Check for (a) above.
    if ( event.ControlDown() )
    {
        // We intentionally don't use isupper/lower() here, we really need
        // ASCII letters only as it doesn't make sense to translate any other
        // ones into this range which has only 26 slots.
        if ( code >= 'a' && code <= 'z' )
            event.m_keyCode = code - 'a' + 1;
        else if ( code >= 'A' && code <= 'Z' )
            event.m_keyCode = code - 'A' + 1;

#if wxUSE_UNICODE
        // Adjust the Unicode equivalent in the same way too.
        if ( event.m_keyCode != code )
            event.m_uniChar = event.m_keyCode;
#endif // wxUSE_UNICODE
    }

#if wxUSE_UNICODE
    // Check for (b) from above.
    //
    // FIXME: Should we do it for key codes up to 255?
    if ( !event.m_uniChar && code < WXK_DELETE )
        event.m_uniChar = code;
#endif // wxUSE_UNICODE
}
} // anonymous namespace

// If a widget does not handle a key or mouse event, GTK+ sends it up the
// parent chain until it is handled. These events are not supposed to propagate
// in wxWidgets, so this code avoids handling them in any parent wxWindow,
// while still allowing the event to propagate so things like native keyboard
// navigation will work.
#define wxPROCESS_EVENT_ONCE(EventType, event) \
    static EventType eventPrev; \
    if (memcmp(&eventPrev, event, sizeof(EventType)) == 0) \
        return false; \
    eventPrev = *event

extern "C" {
static gboolean
gtk_window_key_press_callback( GtkWidget *WXUNUSED(widget),
                               GdkEventKey *gdk_event,
                               wxWindow *win )
{
    // Fixme: use DND of MadEdit could not link to wxGTK's
    //if (g_blockEventsOnDrag)
    //    return FALSE;

    wxPROCESS_EVENT_ONCE(GdkEventKey, gdk_event);

    wxKeyEvent event( wxEVT_KEY_DOWN );
    bool ret = false;
    bool return_after_IM = false;

    if( wxTranslateGTKKeyEventToWx(event, win, gdk_event) )
    {
        // Send the CHAR_HOOK event first
        if ( SendCharHookEvent(event, win) )
        {
            // Don't do anything at all with this event any more.
            return TRUE;
        }

        // Next check for accelerators.
#if wxUSE_ACCEL
        wxWindowGTK *ancestor = win;
        while (ancestor)
        {
            int command = ancestor->GetAcceleratorTable()->GetCommand( event );
            if (command != -1)
            {
                wxCommandEvent menu_event( wxEVT_MENU, command );
                ret = ancestor->HandleWindowEvent( menu_event );

                if ( !ret )
                {
                    // if the accelerator wasn't handled as menu event, try
                    // it as button click (for compatibility with other
                    // platforms):
                    wxCommandEvent button_event( wxEVT_BUTTON, command );
                    ret = ancestor->HandleWindowEvent( button_event );
                }

                break;
            }
            if (ancestor->IsTopLevel())
                break;
            ancestor = ancestor->GetParent();
        }
#endif // wxUSE_ACCEL

        // If not an accelerator, then emit KEY_DOWN event
        if ( !ret )
            ret = win->HandleWindowEvent( event );
    }
    else
    {
        // Return after IM processing as we cannot do
        // anything with it anyhow.
        return_after_IM = true;
    }

    if ( !ret )
    {
        // Indicate that IM handling is in process by setting this pointer
        // (which will remain valid for all the code called during IM key
        // handling).
        win->m_imKeyEvent = gdk_event;

        // We should let GTK+ IM filter key event first. According to GTK+ 2.0 API
        // docs, if IM filter returns true, no further processing should be done.
        // we should send the key_down event anyway.
        const int intercepted_by_IM = win->GTKIMFilterKeypress(gdk_event);

        win->m_imKeyEvent = nullptr;

        if ( intercepted_by_IM )
        {
            wxLogTrace(TRACE_KEYS, wxT("Key event intercepted by IM"));
            return TRUE;
        }
    }

    if (return_after_IM)
        return FALSE;

    // Only send wxEVT_CHAR event if not processed yet. Thus, ALT-x
    // will only be sent if it is not in an accelerator table.
    if (!ret)
    {
        KeySym keysym = gdk_event->keyval;
        // Find key code for EVT_CHAR and EVT_CHAR_HOOK events
        long key_code = wxTranslateKeySymToWXKey(keysym, true /* isChar */);
        if ( !key_code )
        {
            if ( wxIsAsciiKeysym(keysym) )
            {
                // ASCII key
                key_code = (unsigned char)keysym;
            }
            // gdk_event->string is actually deprecated
            else if ( gdk_event->length == 1 )
            {
                key_code = (unsigned char)gdk_event->string[0];
            }
        }

        if ( key_code )
        {
            wxKeyEvent eventChar(wxEVT_CHAR, event);

            wxLogTrace(TRACE_KEYS, wxT("Char event: %ld"), key_code);

            eventChar.m_keyCode = key_code;
#if wxUSE_UNICODE
            eventChar.m_uniChar = gdk_keyval_to_unicode(key_code);
#endif // wxUSE_UNICODE

            AdjustCharEventKeyCodes(eventChar);

            ret = win->HandleWindowEvent(eventChar);
        }
    }

    return ret;
}
}

//-----------------------------------------------------------------------------
// "key_release_event" from any window
//-----------------------------------------------------------------------------

extern "C" {
static gboolean
gtk_window_key_release_callback( GtkWidget * WXUNUSED(widget),
                                 GdkEventKey *gdk_event,
                                 wxWindowGTK *win )
{
    // Fixme: use DND of MadEdit could not link to wxGTK's
    //if (g_blockEventsOnDrag)
    //    return FALSE;

    wxPROCESS_EVENT_ONCE(GdkEventKey, gdk_event);

    wxKeyEvent event( wxEVT_KEY_UP );
    if ( !wxTranslateGTKKeyEventToWx(event, win, gdk_event) )
    {
        // unknown key pressed, ignore (the event would be useless anyhow)
        return FALSE;
    }

    return win->GTKProcessEvent(event);
}
}

GtkIMContext *GetWindowIMContext(wxWindow *win)
{
    return win->m_imContext;
}
// GSource callback functions for source used to detect new GDK events
extern "C" {
static gboolean source_prepare(GSource*, int*)
{
    return !gs_isNewEvent;
}

static gboolean source_check(GSource*)
{
    // 'check' will only be called if 'prepare' returned false
    return false;
}

static gboolean source_dispatch(GSource*, GSourceFunc, void*)
{
    gs_isNewEvent = true;
    // don't remove this source
    return true;
}
}

#endif

void MadEdit::ConnectToFixedKeyPressHandler()
{
    GtkWidget *connect_widget = GetConnectWidget();
#if wxMAJOR_VERSION < 3
    // get signal_id & signal_detail of "key_press_event"
    guint sid;
    GQuark det;
    gboolean bl=g_signal_parse_name("key_press_event", G_OBJECT_TYPE(connect_widget), &sid, &det, TRUE);

    // disconnect old key_press handler
    guint cnt=g_signal_handlers_disconnect_matched(connect_widget, GSignalMatchType(G_SIGNAL_MATCH_ID|G_SIGNAL_MATCH_DETAIL|G_SIGNAL_MATCH_DATA) , sid, det, 0, 0, this);

    //std::wcout<<int(bl)<<' '<<cnt<<std::endl;
    if(bl==false || cnt==0) // error!!!
    {
        std::wcout<<"cannot disconnect old key_press handler\n";
        return;
    }
    
    bl=g_signal_parse_name("key_release_event", G_OBJECT_TYPE(connect_widget), &sid, &det, TRUE);
    cnt=g_signal_handlers_disconnect_matched(connect_widget, GSignalMatchType(G_SIGNAL_MATCH_ID|G_SIGNAL_MATCH_DETAIL|G_SIGNAL_MATCH_DATA) , sid, det, 0, 0, this);

    if(bl==false || cnt==0) // error!!!
    {
        std::wcout<<"cannot disconnect old key_release handler\n";
        return;
    }
#else
    static bool isSourceAttached;
    if (!isSourceAttached)
    {
        // attach GSource to detect new GDK events
        isSourceAttached = true;
        static GSourceFuncs funcs = {
            source_prepare, source_check, source_dispatch,
            nullptr, nullptr, nullptr
        };
        GSource* source = g_source_new(&funcs, sizeof(GSource));
        // priority slightly higher than GDK_PRIORITY_EVENTS
        g_source_set_priority(source, GDK_PRIORITY_EVENTS - 1);
        g_source_attach(source, nullptr);
    }
#endif
    
    // connect to the fixed key_press handler
    g_signal_connect (connect_widget, "key_press_event", G_CALLBACK (gtk_window_key_press_callback), this);
    g_signal_connect (connect_widget, "key_release_event", G_CALLBACK (gtk_window_key_release_callback), this);
}

#endif // __WXGTK__
