//=============================================================================
//
//   File : KviIpcSentinel.hcpp
//   Creation date : Tue Apr 10 2001 15:04:45 by Szymon Stefanek
//
//   This file is part of the KVIrc IRC client distribution
//   Copyright (C) 2001-2010 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//=============================================================================

// FIXME: This stuff is hopelessly complex... nearly pathetic.

#include <qmetatype.h>
#include "KviIpcSentinel.h"


#ifndef COMPILE_NO_IPC

	#include "KviMemory.h"
	#include "KviApplication.h"
	#include "KviQString.h"

#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)

	static HWND kvi_win_findIpcSentinel()
	{
		HWND hWnd = ::FindWindow("QWidget","[Non-Commercial] - kvirc4_ipc_sentinel");
		if(hWnd)return hWnd;
		hWnd = ::FindWindow("QWidget","[Freeware] - kvirc4_ipc_sentinel");
		if(hWnd)return hWnd;
		hWnd = ::FindWindow("QWidget","kvirc4_ipc_sentinel");
		return hWnd;
	}

#else

	#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)

		#include <X11/Xatom.h>

		#include "KviXlib.h" // for XEvent

		#include <unistd.h>    // for getuid, getpid
		#include <sys/types.h> // for getuid, getpid
		#include <string.h> // for memcpy
		#include <stdlib.h> // for malloc

		#include <QX11Info>

		#define kvi_ipc_get_xdisplay QX11Info::display
		#define kvi_ipc_get_xrootwin QX11Info::appRootWindow

		static Atom kvi_atom_ipc_sentinel_window;
		static Atom kvi_atom_ipc_remote_command;
		static Atom kvi_atom_ipc_remote_message;
		static KviCString kvi_sentinel_id;
		static bool g_bIpcAtomsLoaded = false;

		static kvi_u64_t g_uLocalInstanceId = 0;

		static void kvi_ipcLoadAtoms()
		{
			if(g_bIpcAtomsLoaded)
				return;
			g_bIpcAtomsLoaded = true;
			g_uLocalInstanceId = (kvi_u64_t)::getpid();
			kvi_sentinel_id.sprintf("pane tonno e mistero - %d",getuid());
			kvi_atom_ipc_sentinel_window = XInternAtom(kvi_ipc_get_xdisplay(),"XA_KVI4_IPC_SENTINEL_WINDOW",False);
			kvi_atom_ipc_remote_command = XInternAtom(kvi_ipc_get_xdisplay(),"XA_KVI4_IPC_REMOTE_COMMAND",False);
			kvi_atom_ipc_remote_message = XInternAtom(kvi_ipc_get_xdisplay(),"XA_KVI4_IPC_REMOTE_MESSAGE",False);
		}

		static void kvi_ipcSetRemoteCommand(Window w,const char * command)
		{
			if(!command)
				return;

			int len = kvi_strLen(command) + 1;

			char * buffer = (char *)::malloc(len + 8);

			*((kvi_u64_t *)buffer) = g_uLocalInstanceId;

			::memcpy(buffer+8,command,len);

			XChangeProperty(kvi_ipc_get_xdisplay(),w,kvi_atom_ipc_remote_command,
				XA_STRING,8,PropModeReplace,(const unsigned char *)buffer,len+8);

			::free(buffer);
		}

		static Window kvi_x11_findIpcSentinel(Window win)
		{
			Atom type;
			int format;
			unsigned long nItems,after;
			unsigned char * data = 0;
			if(XGetWindowProperty(kvi_ipc_get_xdisplay(),win,kvi_atom_ipc_sentinel_window,0,32,false,XA_STRING,
				&type,&format,&nItems,&after,&data) == Success)
			{
				if((type == XA_STRING) && (format == 8))
				{
					if((nItems == ((unsigned long)(kvi_sentinel_id.len()))) && data)
					{
						if(kvi_strEqualCSN((const char *)data,kvi_sentinel_id.ptr(),kvi_sentinel_id.len()))
						{
							XFree((char *)data);
							return win;
						}
					}
				}
			}

			Window root,parent;
			Window * children;
			unsigned int nChildren;

			if(!XQueryTree(kvi_ipc_get_xdisplay(),win,&root,&parent,&children,&nChildren))
			{
				if(children)XFree((char *)children);
				return 0;
			}

			Window found = 0;

		    for(int i=nChildren-1;(!found) && (i >= 0);i--)
				found = kvi_x11_findIpcSentinel(children[i]);

	 		if(children)XFree((char *)children);

			return found;
		}
	#endif //!COMPILE_NO_X

#endif


	#define KVI_WINDOWS_IPC_MESSAGE 0x2FACE5

	bool kvi_sendIpcMessage(const char * message)
	{
#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)

		HWND hSentinel = kvi_win_findIpcSentinel();
		if(hSentinel != NULL)
		{
			COPYDATASTRUCT cpd;
			cpd.cbData = strlen(message)+1;
			cpd.dwData = KVI_WINDOWS_IPC_MESSAGE;
			cpd.lpData = (void *)message;
			DWORD dwResult;

#if defined(COMPILE_ON_WINDOWS) && !(defined(MINGW))
			if(!::SendMessageTimeout(hSentinel,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cpd,SMTO_BLOCK,1000,(PDWORD_PTR)&dwResult))
#else
			if(!::SendMessageTimeout(hSentinel,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cpd,SMTO_BLOCK,1000,&dwResult))
#endif
			{
				qDebug("Failed to send IPC message: error code 0x%x",::GetLastError());
			}
			return true;
		}
#else
	#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)

		kvi_ipcLoadAtoms();

		Window sentinel = kvi_x11_findIpcSentinel(kvi_ipc_get_xrootwin());
		if(sentinel != 0)
		{
//			XChangeProperty(kvi_ipc_get_xdisplay(),sentinel,kvi_atom_ipc_remote_command,XA_STRING,8,
//				PropModeReplace,(const unsigned char *)message,kvi_strLen(message));
			kvi_ipcSetRemoteCommand(sentinel,message);

#if (QT_VERSION < 0x050000)

			XEvent e;
			KviMemory::set(&e,0,sizeof(XEvent));
			e.type = ClientMessage;
			e.xclient.display = kvi_ipc_get_xdisplay();
			e.xclient.window = sentinel;
			e.xclient.message_type = kvi_atom_ipc_remote_message;
			e.xclient.format = 8;

			Status ret = XSendEvent(kvi_ipc_get_xdisplay(),sentinel,False,0,&e);
#else
			// On Qt5 the ClientMessage events aren't propagated at all.. we have to rely on the property change only
#endif

			return true;
		}
	#endif //!COMPILE_NO_X
#endif
		return false;
	}


	//////////////////////////////////////////////////////////////////////
	// class KviIpcSentinel
	//
	//    hidden sentinel of ipc messages
	//
	//////////////////////////////////////////////////////////////////////

	KviIpcSentinel::KviIpcSentinel()
	: QWidget(0)
	{
		setObjectName("kvirc4_ipc_sentinel");
#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)
		setWindowTitle("kvirc4_ipc_sentinel");
		setWindowFlags(Qt::FramelessWindowHint);
#else
	#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)
		kvi_ipcLoadAtoms();

		XChangeProperty(kvi_ipc_get_xdisplay(),winId(),kvi_atom_ipc_sentinel_window,XA_STRING,8,
			PropModeReplace,(const unsigned char *)kvi_sentinel_id.ptr(),kvi_sentinel_id.len());

		kvi_ipcSetRemoteCommand(winId(),"");
	#endif //!COMPILE_NO_X
#endif

		move(-50,-50);
		resize(1,1);
#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)
		// we need to show the window once otherwise it will never get "realized"
		// and we will not be able to find it via FindWindow()
		show();
#endif
		hide();

	}

	KviIpcSentinel::~KviIpcSentinel()
	{
	}

#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)

	bool KviIpcSentinel::winEvent(MSG * msg, long * result)
	{
		if(msg->message == WM_COPYDATA)
		{
			COPYDATASTRUCT * cpd = (COPYDATASTRUCT *)msg->lParam;
			if(cpd)
			{
				if(cpd->dwData == KVI_WINDOWS_IPC_MESSAGE)
				{
					if(g_pApp)
						g_pApp->ipcMessage((char *)(cpd->lpData));
					if(result)
						*result = 0;
					return true;
				}
			}
		}
		return false;
	}

#else
	#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)

		bool KviIpcSentinel::x11GetRemoteMessage()
		{
			Atom type;
			int format;
			unsigned long nItems,after;
			unsigned char * data = 0;
			KviCString szData;

			if(XGetWindowProperty(kvi_ipc_get_xdisplay(),winId(),kvi_atom_ipc_remote_command,0,1024,false,XA_STRING,
				&type,&format,&nItems,&after,&data) == Success)
			{
				if((type == XA_STRING) && (format == 8) && (nItems > 8) && data)
				{
					kvi_u32_t uSenderId = *((kvi_u32_t *)(data));
					if(uSenderId != g_uLocalInstanceId)
					szData = (char *)(data + 8);
					XFree((char *)data);
				}
			}

			if(szData.isEmpty())
				return false; // no command, or our own command

			kvi_ipcSetRemoteCommand(winId(),"");

			if(g_pApp)
				g_pApp->ipcMessage(szData.ptr());
			return true;
		}

		bool KviIpcSentinel::x11Event(XEvent *e)
		{
			if(e->type == ClientMessage)
			{
				if((e->xclient.message_type = kvi_atom_ipc_remote_message) && (e->xclient.format == 8))
					return x11GetRemoteMessage();
			}
			return false;
		}
	#endif //!COMPILE_NO_X
#endif

#if (QT_VERSION >= 0x050000)
	#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)

	#else
		#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)
			// This is not an XEvent anymore.... it's a xcb_generic_event_t actually.
			// But we don't want to add yet another dependency.
			// Also it turns out that Qt5 filters out ClientMessage events (it uses them for its own purposes)
			// Have to rely on the sole PropertyNotify instead :(

			extern "C" {

				typedef struct {
					kvi_u8_t response_type;
					kvi_u8_t pad0;
					kvi_u16_t sequence;
					kvi_u32_t pad[7];
					kvi_u32_t full_sequence;
				} fake_xcb_generic_event_t;

				typedef struct xcb_property_notify_event_t {
					kvi_u8_t response_type;
					kvi_u8_t pad0;
					kvi_u16_t sequence;
					kvi_u32_t window;
					kvi_u32_t atom;
					// .. other stuff follows, but we don't care
				} fake_xcb_property_notify_event_t;

				#define FAKE_XCB_PROPERTY_NOTIFY 28

			}
		#endif
	#endif

	bool KviIpcSentinel::nativeEvent(const QByteArray &id,void * msg,long * res)
	{
		#if defined(COMPILE_ON_WINDOWS) || defined(COMPILE_ON_MINGW)
				return winEvent((MSG *)msg,res);
		#else
			#if defined(COMPILE_X11_SUPPORT) && defined(COMPILE_QX11INFO_SUPPORT)

				fake_xcb_generic_event_t *event = static_cast<fake_xcb_generic_event_t *>(msg);

				if(event->response_type == FAKE_XCB_PROPERTY_NOTIFY)
				{
					fake_xcb_property_notify_event_t * ev = static_cast<fake_xcb_property_notify_event_t *>(msg);

					if(ev->atom == (kvi_u32_t)kvi_atom_ipc_remote_command)
						return x11GetRemoteMessage();
				}
			#else
				return false;
			#endif
		#endif
	}
#endif

#endif
