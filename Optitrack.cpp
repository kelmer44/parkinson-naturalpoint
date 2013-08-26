// Optitrack.cpp : Defines the class behaviors for the application.
//

/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#include "stdafx.h"
#include "OptitrackApp.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptitrackApp

BEGIN_MESSAGE_MAP(COptitrackApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COptitrackApp::OnAppAbout)
END_MESSAGE_MAP()


// COptitrackApp construction

COptitrackApp::COptitrackApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
COptitrackApp::~COptitrackApp()
{
}

// The one and only COptitrackApp object

COptitrackApp theApp;


// COptitrackApp initialization


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


BOOL COptitrackApp::InitInstance()
{
    CoInitialize(NULL);
    _Module.Init(ObjectMap, m_hInstance);
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();

    // create the modeless dialog
    m_OptiTrackSampleDlg.Create(IDD_CAMERA_LIST);
    m_OptiTrackSampleDlg.ShowWindow(SW_SHOW);


	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// App command to run the dialog
void COptitrackApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void DisplayError(HRESULT hr)
{
    CString str;

    // tell the user what happened
    switch (hr)
    {
        case NP_E_DEVICE_NOT_SUPPORTED:
            str = TEXT("The device is not supported.");
            break;

        case NP_E_DEVICE_DISCONNECTED:
            str = TEXT("The device has been disconnected.");
            break;
    
        default:
            {
                LPVOID     lpMsgBuffer;
	            DWORD dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
								              NULL,
								              hr,
								              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								              reinterpret_cast<LPTSTR>(&lpMsgBuffer),
								              0,
								              NULL);
                str = (LPCTSTR) lpMsgBuffer;
                LocalFree(lpMsgBuffer);
            }
            break;
    }

    AfxMessageBox(str);  
}
void CAboutDlg::OnBnClickedOk()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	OnOK();
}
