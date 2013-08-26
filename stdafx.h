// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7CB4D7AA_5788_4CF3_9922_F3B8D7AD3658__INCLUDED_)
#define AFX_STDAFX_H__7CB4D7AA_5788_4CF3_9922_F3B8D7AD3658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>       // MFC template classes

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// atl includes
#include <atlbase.h>

extern CComModule _Module;

#include <atlcom.h>

// NP specific includes
#include "optitrack.h"

// import the type library for the event handlers
#import "optitrack.tlb"

#include "medicos_mat.h"
#include "medicos_net.h"

//#include "cam_idez.h"

// useful macro converting BOOL to bool
#define MAKE_BOOL(x)        (x != 0)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#pragma warning (disable:4244)
#pragma warning (disable:4311)
#pragma warning (disable:4312)

#define     WM_CAMERA_CLOSED        WM_APP + 100
#define     WM_FRAME_AVAILABLE      WM_APP + 101
#define     WM_FRAME_ID_AVAILABLE   WM_APP + 102

#endif // !defined(AFX_STDAFX_H__7CB4D7AA_5788_4CF3_9922_F3B8D7AD3658__INCLUDED_)

