// OptiTrackSampleDlg.cpp : implementation file
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
#include "OptiTrackApp.h"
#include "OptiTrackSampleDlg.h"
#include "CameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptiTrackSampleDlg dialog

COptiTrackSampleDlg::COptiTrackSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptiTrackSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptiTrackSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void COptiTrackSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptiTrackSampleDlg)
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_buttonOpen);
	DDX_Control(pDX, IDC_LIST_CAMERAS, m_listCameras);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptiTrackSampleDlg, CDialog)
	//{{AFX_MSG_MAP(COptiTrackSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_LBN_SELCHANGE(IDC_LIST_CAMERAS, OnSelchangeListCameras)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_CAMERAS, OnDblclkListCameras)

	//}}AFX_MSG_MAP

    ON_MESSAGE(WM_CAMERA_CLOSED, OnCameraClosed)

    ON_BN_CLICKED(IDC_BUTTON_MULTI, &COptiTrackSampleDlg::OnBnClickedButtonMulti)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptiTrackSampleDlg message handlers

BOOL COptiTrackSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// setup the connection point callbacks
    m_spCollection.CoCreateInstance(CLSID_NPCameraCollection);
    
    // hook up connection points
    this->DispEventAdvise(m_spCollection);

    // enumerate the cameras
    m_spCollection->Enum();

    // fill the list
    FillCameraList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptiTrackSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COptiTrackSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COptiTrackSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void COptiTrackSampleDlg::FillCameraList()
{
    HRESULT hr = S_OK;
    LONG lCount = 0;
    
    // clear the list
    m_listCameras.ResetContent();

    // get the number of cameras
    hr = m_spCollection->get_Count(&lCount);

    for (int i = 0; i < lCount; i++)
    {
        CComPtr<INPCamera> spCamera;
        
        // get the current camera's serial number
        m_spCollection->Item(i, &spCamera);

        LONG lSerial;
        spCamera->get_SerialNumber(&lSerial);

        CString str;

        // put the serial number in the listbox
        str.Format(TEXT("%d"), lSerial);
        int nIndex = m_listCameras.AddString(str);
        m_listCameras.SetItemData(nIndex, i);
    }

    if (lCount > 0)
    {
        // if there are cameras, select one
        m_listCameras.SetCurSel(0);
        
        // enable Open button if valid cameras are present and not already opened        
        BOOL fEnable = FALSE;
        int nIndex = m_listCameras.GetCurSel();

        if (nIndex != LB_ERR)
        {
            // check to see if this camera is already open
            CComPtr<INPCamera> spCamera;

            m_spCollection->Item(nIndex, &spCamera);

            if (m_listOpenCameras.Find((LPDWORD) (INPCamera*)spCamera) == NULL)
            {
                fEnable = TRUE;
            }
        }

        m_buttonOpen.EnableWindow(fEnable);

    }
    else
    {
        m_buttonOpen.EnableWindow(FALSE);
    }
}


void COptiTrackSampleDlg::OnButtonOpen() 
{
    // check to see if this camera is already open
    CComPtr<INPCamera> spCamera;

    int nIndex = m_listCameras.GetCurSel();

    m_spCollection->Item(nIndex, &spCamera);

    if (m_listOpenCameras.Find((LPDWORD) (INPCamera*)spCamera) == NULL)
    {
	    CCameraDlg * pDlg = new CCameraDlg;

        if (pDlg)
        {

            int nCurSel = m_listCameras.GetCurSel();
            int nIndex = m_listCameras.GetItemData(nCurSel);

            // grab a copy of the camera object and stick it in the dlg
            m_spCollection->Item(nIndex, &pDlg->m_spCamera);

            //save the open dialog in the camera, so we can access in reverse.
            CComVariant var = (int) pDlg;
            pDlg->m_spCamera->SetOption(NP_OPTION_USER_DEF1, var);

            pDlg->m_hwndParent = GetSafeHwnd();

            // create the modeless dialog
            pDlg->Create(IDD_CAMERA);
            pDlg->ShowWindow(SW_SHOW);

            m_listOpenCameras.AddTail((LPDWORD) (INPCamera*)pDlg->m_spCamera);

            m_buttonOpen.EnableWindow(FALSE);
        }
    }
}

void COptiTrackSampleDlg::OnSelchangeListCameras() 
{
    BOOL fEnable = FALSE;

    int nIndex = m_listCameras.GetCurSel();

    if (nIndex != LB_ERR)
    {
        // check to see if this camera is already open
        CComPtr<INPCamera> spCamera;

        m_spCollection->Item(nIndex, &spCamera);

        if (m_listOpenCameras.Find((LPDWORD) (INPCamera*)spCamera) == NULL)
        {
            fEnable = TRUE;
        }
    }

    m_buttonOpen.EnableWindow(fEnable);
}

void COptiTrackSampleDlg::OnDestroy() 
{
    CDialog::OnDestroy();
    // close down the connection points
    this->DispEventUnadvise(m_spCollection);
}

void COptiTrackSampleDlg::OnClose() 
{
    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        CComVariant var;
        spCamera->GetOption(NP_OPTION_USER_DEF1, &var);
        CCameraDlg *pDlg = (CCameraDlg *)var.intVal;
        if(pDlg)
        {
            pDlg->OnClose();
        }
    }
    CDialog::OnClose();
    ::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_CLOSE, (WPARAM)0, 0);
}

void COptiTrackSampleDlg::HandleDeviceRemoval(INPCamera * pCamera)
{
    // fill the list
    FillCameraList();
}

void COptiTrackSampleDlg::HandleDeviceArrival(INPCamera * pCamera)
{
    // fill the list
    FillCameraList();
}

void COptiTrackSampleDlg::OnDblclkListCameras() 
{
    OnButtonOpen();	
}

LRESULT COptiTrackSampleDlg::OnCameraClosed(WPARAM wparam, LPARAM lparam)
{
    
    if(POSITION pos = m_listOpenCameras.Find((LPDWORD) wparam))
    {
        m_listOpenCameras.RemoveAt(pos);    // remove the camera from our list
        OnSelchangeListCameras();           // update the open button state
    }
    return TRUE;
}

COptiTrackSampleDlg::~COptiTrackSampleDlg()
{
}

void n_err_msgxxx(char *box_name, char *fmt,...){ 
     
  va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
  char    str[500];			      // buffer para construir la cadena
  int     cnt;			          // contador para almacenar el resultado de sprintf

  va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
  vsprintf(str, fmt, arg_ptr);
  va_end(arg_ptr);			          // cierra las funciones va
  
  //sprintf(str, 
  MessageBoxA(NULL, str, box_name, MB_OK); 
}

void COptiTrackSampleDlg::OnBnClickedButtonMulti()
{

	n_err_msgxxx("Ayuda sobre el control de las cámaras", "Para iniciar el funcionamiento de las cámaras infrarrojas debes seleccionarlas\nen la lista donde aparecen todas las cámaras del sistema con su número de serie.\nHaz clic en cada una de ellas y se abrirá una nueva ventana para cada una de ellas donde podrás acceder a\nparámetros de configuración y ver qué información captura la cámara en todo momento.\nUna vez puesto todo en su sitio, haz clic en el botón COMENZAR en la ventana de cada cámara.");
	//MessageBoxA(NULL, str, box_name, MB_OK);

	/*
    for(int Index = 0; Index< m_listCameras.GetCount(); Index++)
    {
        m_listCameras.SetCurSel(Index);
        OnButtonOpen();
    }
    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        CComVariant var;
        spCamera->GetOption(NP_OPTION_USER_DEF1, &var);
        CCameraDlg *pDlg = (CCameraDlg *)var.intVal;
        pDlg->OnButtonStart();
    }
    // pick a camera for the commands. We could move this higher.
    if(POSITION pos = m_listOpenCameras.GetHeadPosition())
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        CComVariant var = MAKE_BOOL(FALSE);
        spCamera->SetOption(NP_OPTION_MULTICAM, var);
    }
    CComPtr<INPCamera> masterCamera;
    // find the master
    CComVariant var;
// do not change
    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        spCamera->GetOption(NP_OPTION_MULTICAM_MASTER, &var);
        if(var.intVal != 0)
        {
            masterCamera = spCamera;
            break;
        }
    }
    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        spCamera->SetOption(NP_OPTION_MULTICAM_MASTER, var);
    }
// end do not change

    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        var = MAKE_BOOL(TRUE);
        spCamera->SetOption(NP_OPTION_MULTICAM_GROUP_NOTIFY, var);
    }

    SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
    const CWnd *pWnd = this;
    int Index = 1;
    for(POSITION pos = m_listOpenCameras.GetHeadPosition(); pos; )
    {
        INPCamera *ptr = (INPCamera *)m_listOpenCameras.GetNext(pos);
        CComPtr<INPCamera> spCamera = ptr;
        CComVariant var;
        spCamera->GetOption(NP_OPTION_USER_DEF1, &var);
        CCameraDlg *pDlg = (CCameraDlg *)var.intVal;
        pDlg->SetWindowPos( pWnd, Index*150, Index*150, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
        pWnd = pDlg;
        Index++;
    } 
	*/
}

// we have received a report that a certain frame is available for the entire group.
// let the individual cameras know.
void COptiTrackSampleDlg::HandleFrameAvailableId(LONG Group, LONG Id)
{
}


