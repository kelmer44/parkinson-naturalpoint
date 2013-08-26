// CameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptiTrackApp.h"
#include "CameraDlg.h"

//#include "HiResTimer.h" // agregado para forzar los FPS que queramos nosotros


//////////////////////////////////////////////////////////////////////////////////////////////////// AGREGADO PARA SOCKETS ////////////////


#include <io.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include <winsock2.h>
#include <WS2TCPIP.h>

//#include "cam_idez.h"

#define GRUPO "127.0.0.1" //"193.144.46.66"

#define _msg_null       "..."
#define _msg_error      "<!> --- Error en socket ---"
#define _msg_err_send   "<!> Fallo en send"
#define _msg_escuchando "< > Escribiendo en el puerto definido"
#define _msg_enviando   "<+> Enviando bytes "
#define _msg_datagrama  "<!> No se puede enviar el datagrama "

#define CAM_01 79156
#define CAM_02 67724


bool first_time_cam02=true;
float prev_pmx=0.0f,  prev_pmz=0.0f;


/////////////////////////////////////
// variables nuevas
SOCKET sock2;  // CAM 1 socket
SOCKET sock3;  // CAM 2 socket
struct sockaddr_in name2; // CAM 1  
struct sockaddr_in name3; // CAM 2
float  matriz2[1][4];	  // CAM 1
float  matriz3[1][8];	  // CAM 2
//float  matriz3[1][4]; 

int    err;
int    ttl=2;
int    msg_actual = 0;
int    puerto=0; //2020;
//int    teclas[3];

void n_err_msg(char *box_name, char *fmt,...){ 
     
  va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
  char    str[500];			      // buffer para construir la cadena
  int     cnt;			          // contador para almacenar el resultado de sprintf

  va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
  vsprintf(str, fmt, arg_ptr);
  va_end(arg_ptr);			          // cierra las funciones va
  
  //sprintf(str, 
  MessageBoxA(NULL, str, box_name, MB_OK); 
}

void init_socket_emisor(int puerto_controles_1, int cam){ //, int camara){ // parece ser que no necesito saber la cámara...

	if(cam == CAM_01){
	   sock2 = socket(AF_INET, SOCK_DGRAM, 0);
	   WORD wVersionRequerida;
	   WSADATA wsaDatos;
	   wVersionRequerida = MAKEWORD(1,1);
	   int error = WSAStartup(wVersionRequerida, &wsaDatos);
	   if(error !=0){
	      n_err_msg("Error", "No se pudo inicializar la API Winsock\n");
	      WSACleanup();
	      msg_actual = 1;
	      return;
	   }

	   sock2 = socket(AF_INET, SOCK_DGRAM, 0);
	   if(sock2 == INVALID_SOCKET){
	      n_err_msg("Error", "No se ha podido crear el socket\n");
	      msg_actual = 1;
	      return;
	   }

	   u_long nobloqueante =1;
	   ioctlsocket(sock2, FIONBIO, &nobloqueante);
	   name2.sin_family = AF_INET;
	   name2.sin_addr.s_addr = inet_addr(GRUPO);
	   name2.sin_port=htons(puerto_controles_1);
  
       msg_actual = 3; // enviando al puerto blablabla...
	}
	else if(cam == CAM_02){
       sock3 = socket(AF_INET, SOCK_DGRAM, 0);
	   WORD wVersionRequerida2;
	   WSADATA wsaDatos2;
	   wVersionRequerida2 = MAKEWORD(1,1);
	   int error2 = WSAStartup(wVersionRequerida2, &wsaDatos2);
	   if(error2 !=0){
	      n_err_msg("Error", "No se pudo inicializar la API Winsock\n");
	      WSACleanup();
	      msg_actual = 1;
	      return;
	   }

	   sock3 = socket(AF_INET, SOCK_DGRAM, 0);
	   if(sock3 == INVALID_SOCKET){
	      n_err_msg("Error", "No se ha podido crear el socket\n");
	      msg_actual = 1;
	      return;
	   }

	   u_long nobloqueante2 =1;
	   ioctlsocket(sock3, FIONBIO, &nobloqueante2);
	   name3.sin_family = AF_INET;
	   name3.sin_addr.s_addr = inet_addr(GRUPO);
	   name3.sin_port=htons(puerto_controles_1);
  
       msg_actual = 3; // enviando al puerto blablabla...		
	   //n_err_msg("efrr", "PUPU");
	}
}

void update_socket_emisor(int camara){
	int i;
	static float mtemp[1][8];	 // antes era 1/4 pero la aumenté para que quepa matriz3 (CAM_02) también
	//static float mtemp[1][4];

    if(camara == CAM_01){
      //	if( (fabs(mtemp[0][0] - matriz2[0][0]) > 0) |
      //		(fabs(mtemp[0][1] - matriz2[0][1]) > 0) |
      //		(fabs(mtemp[0][2] - matriz2[0][2]) > 0) |
      //		(fabs(mtemp[0][3] - matriz2[0][3]) > 0) ){  
      //
	  if(sendto (sock2, (char *)matriz2, sizeof(matriz2), 0, (struct sockaddr *)&name2, sizeof name2) == SOCKET_ERROR)		
         msg_actual = 5;	
	  else {			
 		 int m, n; 
		 for(m=0; m<1; m++)
			 for(n =0; n<4; n++)
			    mtemp[m][n] = matriz2[m][n];			
		 }
		 msg_actual = 4; 
	     //}      
	 }
	 else if(camara == CAM_02){
         //	if( (fabs(mtemp[0][0] - matriz2[0][0]) > 0) |
         //		(fabs(mtemp[0][1] - matriz2[0][1]) > 0) |
         //		(fabs(mtemp[0][2] - matriz2[0][2]) > 0) |
         //		(fabs(mtemp[0][3] - matriz2[0][3]) > 0) ){  
         //
	     if(sendto (sock3, (char *)matriz3, sizeof(matriz3), 0, (struct sockaddr *)&name3, sizeof name3) == SOCKET_ERROR)		
            msg_actual = 5;	
	     else {			
 		    int m2, n2; 
		    for(m2=0; m2<1; m2++)
			    for(n2=0; n2<8; n2++) // ANTES 4
			       mtemp[m2][n2] = matriz3[m2][n2];			
		 }
		 msg_actual = 4; 
		 //n_err_msg("err", "PEPE");
	     //}      		
	 }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg dialog

CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCameraDlg::IDD, pParent), pCCameraOptionsDlg(NULL)
, m_FrameId(_T(""))
{
	//{{AFX_DATA_INIT(CCameraDlg)
	m_fLedFour = FALSE;
	m_fLedOne = FALSE;
	m_fLedThree = FALSE;
	m_fLedTwo = FALSE;
    m_fDrawVideo = TRUE;
	m_strDimensions = _T("");
	m_strDotArea = _T("");
	m_strDotPosition = _T("");
	m_strFrameRate = _T("");
	m_strModel = _T("");
	m_strPitch = _T("");
	m_strRevision = _T("");
	m_strRoll = _T("");
	m_strSerialNumber = _T("");
	m_strSwitchState = _T("");
	m_strX = _T("");
	m_strY = _T("");
	m_strYaw = _T("");
	m_strZ = _T("");
	//}}AFX_DATA_INIT

    m_fStarted = FALSE;
    m_nUpdateRate = 1;
    m_dwFrameCount = 0;

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCameraDlg)
    DDX_Control(pDX, IDC_COMBO_UPDATE_RATE, m_comboUpdateRate);
    DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
    DDX_Check(pDX, IDC_CHECK_LED_FOUR, m_fLedFour);
    DDX_Check(pDX, IDC_CHECK_LED_ONE, m_fLedOne);
    DDX_Check(pDX, IDC_CHECK_LED_THREE, m_fLedThree);
    DDX_Check(pDX, IDC_CHECK_LED_TWO, m_fLedTwo);
    DDX_Text(pDX, IDC_STATIC_DIMENSIONS, m_strDimensions);
    DDX_Text(pDX, IDC_STATIC_DOT_AREA, m_strDotArea);
    DDX_Check(pDX, IDC_CHECK_DRAW_VIDEO, m_fDrawVideo);
    DDX_Text(pDX, IDC_STATIC_DOT_POSITION, m_strDotPosition);
    DDX_Text(pDX, IDC_STATIC_FRAME_RATE, m_strFrameRate);
    DDX_Text(pDX, IDC_STATIC_MODEL, m_strModel);
    DDX_Text(pDX, IDC_STATIC_PITCH, m_strPitch);
    DDX_Text(pDX, IDC_STATIC_REVISION, m_strRevision);
    DDX_Text(pDX, IDC_STATIC_ROLL, m_strRoll);
    DDX_Text(pDX, IDC_STATIC_SERIAL_NUMBER, m_strSerialNumber);
    DDX_Text(pDX, IDC_STATIC_SWITCH_STATE, m_strSwitchState);
    DDX_Text(pDX, IDC_STATIC_X, m_strX);
    DDX_Text(pDX, IDC_STATIC_Y, m_strY);
    DDX_Text(pDX, IDC_STATIC_YAW, m_strYaw);
    DDX_Text(pDX, IDC_STATIC_Z, m_strZ);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_FRAMEID, m_FrameId);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraDlg)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_CHECK_LED_ONE, OnCheckLedOne)
	ON_BN_CLICKED(IDC_CHECK_LED_TWO, OnCheckLedTwo)
	ON_BN_CLICKED(IDC_CHECK_LED_THREE, OnCheckLedThree)
	ON_BN_CLICKED(IDC_CHECK_LED_FOUR, OnCheckLedFour)
    ON_BN_CLICKED(IDC_CHECK_DRAW_VIDEO, OnCheckDrawVideo)
	ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_OPTIONS, OnButtonOptions)
	ON_WM_PAINT()
	ON_WM_CLOSE()
    ON_WM_ACTIVATE()
    ON_MESSAGE(WM_FRAME_AVAILABLE, OnWMFrameAvailable)
    ON_BN_CLICKED(IDC_BUTTON_RESET_VECTOR, OnButtonResetVector)
	ON_CBN_SELCHANGE(IDC_COMBO_UPDATE_RATE, OnSelchangeComboUpdateRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg message handlers

BOOL CCameraDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    HRESULT hr = S_OK;
    int i; // agregado para inicializar el vector de smooth

    ////////////////////////////////////////////////////////////////////////
	// inicialización de los valores para el 0
	yaw_centro   = 0.0f;
	pitch_centro = 0.0f;
	roll_centro  = 0.0f;
    curr_smooth  = 0;
	curr_mano_smooth = 0;

	for(i=0;i<SMOOTH_VAL;i++){
	   yaw_smooth[i]   = 0.0f;
	   pitch_smooth[i] = 0.0f;
	   roll_smooth[i]  = 0.0f;
	}
    for(i=0;i<SMOOTH_MANO;i++){
	   manox_smooth[i] = 0.0f;
	   manoy_smooth[i] = 0.0f;
	   manoz_smooth[i] = 0.0f;
	}


    p_mano01 = NULL_VEC3; // 0 0 0
    p_dedo01 = NULL_VEC3; //   "
	p_dedo02 = NULL_VEC3;
	p_dedo03 = NULL_VEC3;

	c_mano01 = NULL_VEC3; //   "
	c_dedo01 = NULL_VEC3; //   "
    c_dedo02 = NULL_VEC3;
	c_dedo03 = NULL_VEC3;

    g_mano = vs_group3d_t(4); // dedo(3) y mano(1)
	g_mano.init();
	g_mano.addVertex(NULL_VEC3, 0); // si esto pasa de 5 empezaré a usar un bucle for...
	g_mano.addVertex(NULL_VEC3, 1);
	g_mano.addVertex(NULL_VEC3, 2);
	g_mano.addVertex(NULL_VEC3, 3);


	/*
    xm1=0.0f;
	ym1=0.0f;
	zm1=0.0f;       // posición mano 1
	
	xd1m1=0.0f;
	yd1m2=0.0f;
	zd1m3=0.0f; // posición dedo 1 mano 1. por qué carajo no estoy usando un puto vector?
	
	xposd1_centro=0.0f;
	yposd1_centro=0.0f;
	zposd1_centro=0.0f; // posiciones iniciales del dedo1
	
	xposm1_centro=0.0f; 
	yposm1_centro=0.0f; 
	zposm1_centro=0.0f;	
    */

//    fps  = 0.0f;
///	secs = 0.0f;
//	timer.Init();


    // hook up connection points
    this->DispEventAdvise(m_spCamera);

    // open the connection to the camera
    hr = m_spCamera->Open();

    // fill camera info
    FillCameraInfo();

    // create out helper objects
    m_spSmoothingDot.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingYaw.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingPitch.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingRoll.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingX.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingY.CoCreateInstance(CLSID_NPSmoothing);
    m_spSmoothingZ.CoCreateInstance(CLSID_NPSmoothing);

    m_spVector.CoCreateInstance(CLSID_NPVector);

    CString str;

    // fill in the update rate combo box
    for (int i = 0; i < 20; i++)
    {
        str.Format(TEXT("%d frame(s)"), i + 1);
        m_comboUpdateRate.AddString(str);
    }

    m_comboUpdateRate.SetCurSel(10);

    //////////////////////////////////////////////////////////
	if(cam_id == CAM_01)      puerto = 2020;
	else if(cam_id == CAM_02) puerto = 2022;
	init_socket_emisor(puerto, cam_id);
    //////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCameraDlg::FillCameraInfo()
{
    HRESULT hr = S_OK;

    LONG lData, lData2;

    // serial number
    hr = m_spCamera->get_SerialNumber(&lData);
    m_strSerialNumber.Format(TEXT("%d"), lData);

    CString strTitle;
    GetWindowText(strTitle);
    strTitle += TEXT(" ");
    strTitle += m_strSerialNumber;
    SetWindowText(strTitle);


    // model
    hr = m_spCamera->get_Model(&lData);
    m_strModel.Format(TEXT("0x%x"), lData);

    // revision
    hr = m_spCamera->get_Revision(&lData);
    m_strRevision.Format(TEXT("0x%x"), lData);

    // dimensions
    hr = m_spCamera->get_Width(&lData);
    hr = m_spCamera->get_Height(&lData2);
    m_strDimensions.Format(TEXT("%d x %d"), lData, lData2);

    // frame rate
    hr = m_spCamera->get_FrameRate(&lData);
    m_strFrameRate.Format(TEXT("%d"), lData);

    ////////////// agregado para controlar la cámara actual ///////////////////////
	CString serial1, serial2;

	serial1 += TEXT("79156");
	serial2 += TEXT("67724");
	if(m_strSerialNumber.Compare(serial1) == 0)      cam_id = CAM_01;
	else if(m_strSerialNumber.Compare(serial2) == 0) cam_id = CAM_02;


	///////////////////////////////////////////////////////////////////////////////

    // update the UI
    UpdateData(FALSE);
}

void CCameraDlg::OnButtonStart() 
{
    HRESULT hr = S_OK;

    HWND hWnd = GetDlgItem(IDC_STATIC_FRAME_IMAGE)->GetSafeHwnd();
    CComVariant var = (unsigned) hWnd;
    m_spCamera->SetOption(NP_OPTION_USER_HWND, var);

    if (!m_fStarted)
    {
        // camera not running, start it
        hr = m_spCamera->Start();

        if (SUCCEEDED(hr))
        {
            m_fStarted = TRUE;

            // change the button to stop
            CString str;
            str.LoadString(IDS_STOP);

            m_buttonStart.SetWindowText(str);
        }
        else
        {
            DisplayError(hr);
        }
    }
    else
    {
        // camera running, stop it
        hr = m_spCamera->Stop();

        if (SUCCEEDED(hr))
        {
            m_fStarted = FALSE;

            // change the button to start
            CString str;
            str.LoadString(IDS_START);

            m_buttonStart.SetWindowText(str);
        }
        else
        {
            DisplayError(hr);
        }
    }
}

void CCameraDlg::OnCheckDrawVideo() 
{
    UpdateData();
}

void CCameraDlg::OnCheckLedOne() 
{
    UpdateData();

    HRESULT hr = S_OK;

    hr = m_spCamera->SetLED(NP_LED_ONE, B2VB(m_fLedOne));
}

void CCameraDlg::OnCheckLedTwo() 
{
    UpdateData();

    HRESULT hr = S_OK;

    hr = m_spCamera->SetLED(NP_LED_TWO, B2VB(m_fLedTwo));
}

void CCameraDlg::OnCheckLedThree() 
{
    UpdateData();

    HRESULT hr = S_OK;

    hr = m_spCamera->SetLED(NP_LED_THREE, B2VB(m_fLedThree));
}

void CCameraDlg::OnCheckLedFour() 
{
    UpdateData();

    HRESULT hr = S_OK;

    hr = m_spCamera->SetLED(NP_LED_FOUR, B2VB(m_fLedFour));
}

void CCameraDlg::OnClose() 
{
    if(m_hWnd!=NULL)
    {
        CDialog::OnClose();
        DestroyWindow();
    }
}

void CCameraDlg::OnDestroy() 
{
    // close down the connection points
    this->DispEventUnadvise(m_spCamera);
    // shutdown the camera
    if (m_fStarted)
    {
        OnButtonStart();
    }
    if(m_spFrame)
    {
        m_spFrame->Free();          //free the buffer
        m_spFrame = NULL;           //free the COM object
    }
    ::PostMessage(m_hwndParent, WM_CAMERA_CLOSED, (WPARAM)(INPCamera*)m_spCamera, 0);
    if(m_spCamera)
    {
        m_spCamera->Close();        //close the camera
        m_spCamera = NULL;          //free the COM object
    }

	g_mano.uninit();
}

void CCameraDlg::PostNcDestroy( )
{
    delete this;
}

void CCameraDlg::HandleFrameAvailable(INPCamera * pCamera)
{
    CComPtr<INPCameraFrame> spFrame;
    if(S_OK == m_spCamera->GetFrame(0, &spFrame))
    {
        if (m_spFrame)
        {
            m_spFrame->Free();  //free the buffer
            m_spFrame = NULL;   //release the COM object.
        }
        m_spFrame = spFrame;    //save the new frame
        ::PostMessage(GetSafeHwnd(), WM_FRAME_AVAILABLE, (WPARAM) pCamera, 0);
    }
}

LRESULT CCameraDlg::OnWMFrameAvailable(WPARAM wparam, LPARAM lparam)
{
    INPCamera * pCamera = (INPCamera *)wparam;

//    TRACE2("Frame Available %p %p\n", this, pCamera);

    m_dwFrameCount++;
    if ( (m_dwFrameCount % m_nUpdateRate) == 0)
    {
        // update the dot positions
        UpdateDotInfo(m_spFrame);

        // update vector information
        UpdateVectorInfo(m_spFrame);

        // update the switch state
        UpdateSwitchState();

        long lId;
        m_spFrame->get_Id(&lId);
        m_FrameId.Format(TEXT("%ld"),lId);

        // update the UI
        UpdateData(FALSE);

        // draw the frame if rendering is enabled
        if (m_fDrawVideo)
        {
            m_spCamera->DrawFrame(m_spFrame, (LONG) GetDlgItem(IDC_STATIC_FRAME_IMAGE)->GetSafeHwnd());
        }
    }
    return TRUE;    
}

//float x1,y1,z1, x2,y2,z2, x3,y3,z3;

void CCameraDlg::UpdateDotInfo(INPCameraFrame * pFrame)
{

    LONG lCount = 0;
    pFrame->get_Count(&lCount);

    for (int i = 0; i < lCount; i++)
    {
        CComPtr<INPObject> spObject;

        pFrame->Item(i, &spObject);

        LONG lRank;

		/////////////////////////////////////////////////
            CComVariant varX2, varY2, varArea2;			

            // values are double's so we need to use a variant to query for them
            spObject->get_X(&varX2);
            spObject->get_Y(&varY2);
            spObject->get_Area(&varArea2);

            // update the UI
//            m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX2.dblVal, varY2.dblVal);
//            m_strDotArea.Format(TEXT("%.f"), varArea2.dblVal);
         ///////////////////////////

/*
        spObject->get_Rank(&lRank);
        if (lRank == 1)
        {
            // this is the currently tracked object... 
            CComVariant varX, varY, varArea;			

            // values are double's so we need to use a variant to query for them
            spObject->get_X(&varX);
            spObject->get_Y(&varY);
            spObject->get_Area(&varArea);

            // smooth the data
            m_spSmoothingDot->Update(varX, varY);

            // grab the smoothed data
            m_spSmoothingDot->get_X(&varX);
            m_spSmoothingDot->get_Y(&varY);

            // update the UI
            m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX.dblVal, varY.dblVal);
            m_strDotArea.Format(TEXT("%.f"), varArea.dblVal);
        }
		*/

    }
}

void CCameraDlg::UpdateVectorInfo(INPCameraFrame * pFrame)
{
    CComVariant varYaw, varPitch, varRoll, varX, varY, varZ, varEmpty;
	VARIANT p;
    float fy=0.0f, fp=0.0f, fr=0.0f;

    // update the vector positions
    m_spVector->Update(m_spCamera, pFrame);
    
    // get the values back
    m_spVector->get_Yaw(&varYaw);
    m_spVector->get_Pitch(&varPitch);
    m_spVector->get_Roll(&varRoll);
    m_spVector->get_X(&varX);
    m_spVector->get_Y(&varY);
    m_spVector->get_Z(&varZ);
    

	////////////////////////// SMOOTH DE OPTITRACK
/*
	//p.dblVal = 150.0f;
	p.fltVal = 1000.0f;
    m_spSmoothingYaw->put_Amount(p);
	m_spSmoothingPitch->put_Amount(p);
	m_spSmoothingRoll->put_Amount(p);
		//n_err_msg("rrr","eso: %f", p.dblVal);


    // now smooth the values
    m_spSmoothingYaw->Update(varYaw, varEmpty);
    m_spSmoothingPitch->Update(varPitch, varEmpty);
    m_spSmoothingRoll->Update(varRoll, varEmpty);
    m_spSmoothingX->Update(varX, varEmpty);
    m_spSmoothingY->Update(varY, varEmpty);
    m_spSmoothingZ->Update(varZ, varEmpty);
	

    // get the smoothed data
    m_spSmoothingYaw->get_X(&varYaw);
    m_spSmoothingPitch->get_X(&varPitch);
    m_spSmoothingRoll->get_X(&varRoll);
    m_spSmoothingX->get_X(&varX);
    m_spSmoothingY->get_X(&varY);
    m_spSmoothingZ->get_X(&varZ);
 */

	/*
    // now update the UI
    m_strYaw.Format(TEXT("%.3f"), varYaw.dblVal - (double)(yaw_centro));
    m_strPitch.Format(TEXT("%.3f"), varPitch.dblVal - (double)(pitch_centro));
    m_strRoll.Format(TEXT("%.3f"), varRoll.dblVal - (double)(roll_centro));
//    m_strYaw.Format(TEXT("%.3f"), varYaw.dblVal);
//    m_strPitch.Format(TEXT("%.3f"), varPitch.dblVal);
//    m_strRoll.Format(TEXT("%.3f"), varRoll.dblVal);

    m_strX.Format(TEXT("%.3f"), varX.dblVal);
    m_strY.Format(TEXT("%.3f"), varY.dblVal);
    m_strZ.Format(TEXT("%.3f"), varZ.dblVal);
*/
//////////////////////////////////
  
    double yaw, pitch, roll;
	int i;

	if(cam_id == CAM_01){  // CONTROL DE LA CABEZA
	   yaw   = varYaw.dblVal;
       pitch = varPitch.dblVal;
	   roll  = varRoll.dblVal;

       fy = (-1.0f)*((float)(yaw - yaw_centro));
	   fp = (float)(pitch - pitch_centro);
	   fr = (float)(roll - roll_centro);

	   // CONTROL DEL LÍMITE DE LOS ÁNGULOS PARA EL MOVIMIENTO DE LA CABEZA - para que no peten y para que no veas cosas que no deberías ver 
	   
       fy *= 1.12f; //1.27f;
	   fp *= 1.23f; //1.35f;
	   fr *= 1.01f; //1.1f;

       if(fy > 35.0f)  fy = 35.0f;
	   if(fy < -35.0f) fy = -35.0f;

	   if(fp > 35.0f)  fp = 35.0f;
	   if(fp < -35.0f) fp = -35.0f;

	   if(fr > 25.0f)  fr = 25.0f;
	   if(fr < -25.0f) fr = -25.0f;

	   ////////////////////////////////////// agregado
       yaw_smooth[curr_smooth]   = fy;
	   pitch_smooth[curr_smooth] = fp;
	   roll_smooth[curr_smooth]  = fr;

       curr_smooth++;
	   if(curr_smooth > SMOOTH_VAL - 1) curr_smooth = 0;

       fy = fp = fr = 0.0f;
	   for(i=0;i<SMOOTH_VAL;i++){
	      fy += yaw_smooth[i];
		  fp += pitch_smooth[i];
		  fr += roll_smooth[i];
	   }

	   fy /= (float)SMOOTH_VAL; //5.0f;
	   fp /= (float)SMOOTH_VAL; //5.0f;
	   fr /= (float)SMOOTH_VAL; //5.0f;
       ///////////////////////////////////// hasta acá
       
	   // original FUNCIONA
       matriz2[0][0] = (float)(cam_id); //1.0f;
	   matriz2[0][1] = fy;
	   matriz2[0][2] = fp;
	   matriz2[0][3] = fr;

	   // now update the UI
       m_strYaw.Format(TEXT("%.3f"), varYaw.dblVal - (double)(yaw_centro));
       m_strPitch.Format(TEXT("%.3f"), varPitch.dblVal - (double)(pitch_centro));
       m_strRoll.Format(TEXT("%.3f"), varRoll.dblVal - (double)(roll_centro));
       //    m_strYaw.Format(TEXT("%.3f"), varYaw.dblVal);
       //    m_strPitch.Format(TEXT("%.3f"), varPitch.dblVal);
       //    m_strRoll.Format(TEXT("%.3f"), varRoll.dblVal);
       
	   m_strX.Format(TEXT("%.3f"), varX.dblVal);
       m_strY.Format(TEXT("%.3f"), varY.dblVal);
       m_strZ.Format(TEXT("%.3f"), varZ.dblVal);
	}
	else if(cam_id == CAM_02){
        LONG lCount = 0;
        pFrame->get_Count(&lCount);

        // gestión de error - si hay más de dos puntos avisar
        //if(lCount > 4) n_err_msg("ADVERTENCIA", "Hay más de dos puntos visibles.\nSe usarán los 2 primeros encontrados (que quizás no sean información válida).");
        // SI HAY MÁS OBJETOS DE LOS QUE DEBERÍA, EN ESTE CASO - ME LA PELA

		for(int i = 0; i < lCount; i++){         // por ahora sólo tenemos dos cosas para trackear
            CComPtr<INPObject> spObject;
            pFrame->Item(i, &spObject);           
            CComVariant varX2, varY2, varArea2;			

            // values are double's so we need to use a variant to query for them
            spObject->get_X(&varX2);
            spObject->get_Y(&varY2);
            spObject->get_Area(&varArea2);
     
			// este me obliga a crear un nuevo vec3 cada vez que sobreescribo un valor. malo para la memoria.
			//g_mano.addVertex(tVector3((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal)), i);         		
			g_mano.addVertex((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal), i);
			//n_err_msg("info", "gmano [%d]: %.2f %.2f %.2f", i, g_mano.vertices[i].x, g_mano.vertices[i].y, g_mano.vertices[i].z);
		}

		/////////////////////////////////////////////// REORDENAMIENTO DE LOS DEDOS ///////////////////////////////////
        int   k=0,l=0;
		float xaux=0.0f, yaux=0.0f, zaux=0.0f;

		for(k=0;k<4;k++){
			for(l=0;l<4;l++){
				if(g_mano.getVertexX(k) < g_mano.getVertexX(l)){
					xaux = g_mano.getVertexX(l); // es una ELE no un UNO
					yaux = g_mano.getVertexY(l);
					zaux = g_mano.getVertexZ(l);

					g_mano.vertices[l].x = g_mano.vertices[k].x;
					g_mano.vertices[l].y = g_mano.vertices[k].y;
					g_mano.vertices[l].z = g_mano.vertices[k].z;

					g_mano.vertices[k].x = xaux;
					g_mano.vertices[k].y = yaux;
					g_mano.vertices[k].z = zaux;
				}
			}
		}

		p_dedo01.x = g_mano.vertices[0].x; p_dedo01.y = g_mano.vertices[0].y; p_dedo01.z = g_mano.vertices[0].z;
		p_dedo02.x = g_mano.vertices[1].x; p_dedo02.y = g_mano.vertices[1].y; p_dedo02.z = g_mano.vertices[1].z;
		p_dedo03.x = g_mano.vertices[2].x; p_dedo03.y = g_mano.vertices[2].y; p_dedo03.z = g_mano.vertices[2].z;
        p_mano01.x = g_mano.vertices[3].x; p_mano01.y = g_mano.vertices[3].y; p_mano01.z = g_mano.vertices[3].z;


        //n_err_msg("err", "%f %f %f %f", p_dedo01.x, p_dedo02.x, p_dedo03.x, p_mano01.x);

        /////////////////////////////// AGREGADO PARA CALCULAR EL ÁNGULO DE LOS DEDOS/01/02/03  //////////////////////////////////
        float yaw_ang_dedo01=0.0f, yaw_ang_dedo02=0.0f, yaw_ang_dedo03=0.0f;
		float auxx=0.0f, auxy=0.0f, auxz=0.0f;

		// ORIGINAL RULA 
		//yaw_ang_dedo01 = atan(abs(p_dedo01.y - p_mano01.y) / abs(p_dedo01.x - p_mano01.x));
        //if(p_mano01.y < p_dedo01.y) yaw_ang_dedo01 *= -1.0f; //-1 * yaw_ang_dedo01;

		// DEDO 01
		// ORIGINAL RULA FUNCIONA ENVÍA
	    //matriz3[0][0] = (float)(cam_id);  // CAM_02 obviamente
		//matriz3[0][1] = (float)(p_dedo01.x);
	    //matriz3[0][2] = (float)(p_dedo01.y);
	    //matriz3[0][3] = (float)(p_dedo01.z);

        yaw_ang_dedo01 = atan(abs(p_dedo01.y - p_dedo02.y) / abs(p_dedo01.x - p_dedo02.x));
        if(p_dedo02.y < p_dedo01.y) yaw_ang_dedo01 *= -1.0f; 
		if(yaw_ang_dedo01 > 0.0f) yaw_ang_dedo01 = 0.0f; // AGREGADO PARA QUE LA ARTICULACIÓN NO VAYA MÁS ALLÁ DE LO QUE DEBERÍA

		yaw_ang_dedo02 = atan(abs(p_dedo02.y - p_dedo03.y) / abs(p_dedo02.x - p_dedo03.x));
        if(p_dedo03.y < p_dedo02.y) yaw_ang_dedo02 *= -1.0f; 
//        if(yaw_ang_dedo02 > 0.0f) yaw_ang_dedo02 = 0.0f; // AGREGADO PARA QUE LA ARTICULACIÓN NO VAYA MÁS ALLÁ DE LO QUE DEBERÍA

		yaw_ang_dedo03 = atan(abs(p_dedo03.y - p_mano01.y) / abs(p_dedo03.x - p_mano01.x));
//        if(p_mano01.y < p_dedo03.y) yaw_ang_dedo03 *= -1.0f; 


        matriz3[0][0] = (float)(cam_id);  // CAM_02 obviamente
		matriz3[0][1] = yaw_ang_dedo01;
	    matriz3[0][2] = yaw_ang_dedo02;
	    matriz3[0][3] = yaw_ang_dedo03;

	    ////////////////////////////////////// agregado para smooth de la mano ///////////////

		manox_smooth[curr_mano_smooth] = (float)(p_mano01.x);
	    manoy_smooth[curr_mano_smooth] = (float)(p_mano01.y);
	    manoz_smooth[curr_mano_smooth] = (float)(p_mano01.z); // invierto el eje z para que el movimiento de la mano sea coherente con la realidad

        curr_mano_smooth++;
	    if(curr_mano_smooth > SMOOTH_MANO - 1) curr_mano_smooth = 0;


		p_mano01.x = p_mano01.y = p_mano01.z = 0.0f;
	    for(i=0;i<SMOOTH_MANO;i++){
	       p_mano01.x += manox_smooth[i];
		   p_mano01.y += manoy_smooth[i];
	 	   p_mano01.z += manoz_smooth[i];
	    } 
/*
		// control para que no se salga de los límites
        if(p_mano01.x < 130)      p_mano01.x = 130.0f;
		else if(p_mano01.x > 310) p_mano01.x = 310.0f;

        if(p_mano01.z < 50)       p_mano01.z = 50.0f;
		else if(p_mano01.z > 120) p_mano01.z = 120.0f;
*/
        for(i=0;i<SMOOTH_MANO-(SMOOTH_MANO / 4);i++){
	       auxx += manox_smooth[i];
		   auxy += manoy_smooth[i];
	 	   auxz += manoz_smooth[i];
	    } 
        auxx /= (float)(SMOOTH_MANO - (SMOOTH_MANO / 4));
        auxy /= (float)(SMOOTH_MANO - (SMOOTH_MANO / 4));
        auxz /= (float)(SMOOTH_MANO - (SMOOTH_MANO / 4));
 

		if(auxx < 130)      auxx = 130.0f;
		else if(auxx > 310) auxx = 310.0f;

        if(auxz < 50)       auxz = 50.0f;
		else if(auxz > 120) auxz = 120.0f;


	    p_mano01.x /= (float)SMOOTH_MANO; //5.0f;
	    p_mano01.y /= (float)SMOOTH_MANO; //5.0f;
	    p_mano01.z /= (float)SMOOTH_MANO; //5.0f;


//        if(fabs(auxx - p_mano01.x) > 1.46f)  p_mano01.x = auxx;
//		if(fabs(auxz - p_mano01.z) > 0.425f) p_mano01.z = auxz;

		manox_smooth[curr_mano_smooth] = (float)(p_mano01.x);
	    manoy_smooth[curr_mano_smooth] = (float)(p_mano01.y);
	    manoz_smooth[curr_mano_smooth] = (float)(p_mano01.z);

		///////////////////////////////////// hasta acá /////////////////////////////////////
        float pmx, pmy, pmz; // antes eran ints

// ESTO RULABA BIEN
//		pmx = (int)p_mano01.x;
//		pmy = (int)p_mano01.y;
//		pmz = (int)p_mano01.z;

		//if(p_mano01.x != 0.0f) pmz = ((MAX_OGRE_DIFF_X * (MAX_REAL_DIFF_X - p_mano01.x)) / (MAX_REAL_X)) - MIN_OGRE_REAL_X; 
        //if(p_mano01.y != 0.0f) pmy = ((MAX_OGRE_DIFF_Y * (MAX_REAL_DIFF_Y - p_mano01.y)) / (MAX_REAL_Y)) - MIN_OGRE_REAL_Y; 
        //if(p_mano01.z != 0.0f) pmx = ((MAX_OGRE_DIFF_Z * (MAX_REAL_DIFF_Z - p_mano01.z)) / (MAX_REAL_Z)) - MIN_OGRE_REAL_Z; 
  
        if(p_mano01.x != 0.0f) pmx = -1 * ((((MAX_REAL_X - p_mano01.x) * (MAX_OGRE_REAL_X - MIN_OGRE_REAL_X)) / (MAX_REAL_X - MIN_REAL_X)) + MAX_OGRE_REAL_X);   //(MAX_OGRE_DIFF_X * (MAX_REAL_DIFF_X - p_mano01.x)) / (MAX_REAL_X)) - MIN_OGRE_REAL_X; 
        if(p_mano01.y != 0.0f) pmy = -1 * ((((MAX_REAL_Y - p_mano01.y) * (MAX_OGRE_REAL_Y - MIN_OGRE_REAL_Y)) / (MAX_REAL_Y - MIN_REAL_Y)) + MAX_OGRE_REAL_Y);   //((MAX_OGRE_DIFF_Y * (MAX_REAL_DIFF_Y - p_mano01.y)) / (MAX_REAL_Y)) - MIN_OGRE_REAL_Y; 
        if(p_mano01.z != 0.0f) pmz = -1 * ((((MAX_REAL_Z - p_mano01.z) * (MAX_OGRE_REAL_Z - MIN_OGRE_REAL_Z)) / (MAX_REAL_Z - MIN_REAL_Z)) + MAX_OGRE_REAL_Z);   //((MAX_OGRE_DIFF_Z * (MAX_REAL_DIFF_Z - p_mano01.z)) / (MAX_REAL_Z)) - MIN_OGRE_REAL_Z; 
  
		// normalización de valores
		pmx *= -0.5f; //-1;
		pmz *= -0.5f; //-1;
		pmy *= 0.65f; // antes 0.5

		// corrección de la profundidad (eje z mío, eje x rubén)
		pmx -= 5;

		// corrección de la altura
		float auxy2=0.0f;
		if(p_mano01.y <= 110){
			auxy2 = (((1.35f * (110 - p_mano01.y)) / 50) + 1.35f); 
			pmy = auxy2 / 2.0f; //pmy + auxy2;
		}

		// adelante y atrás
		pmz += 10;

		if((fabs(pmz - prev_pmz) >= 0.015f) && (fabs(pmz - prev_pmz) <= 0.035f)) pmx = prev_pmx; 
		//if((fabs(p_mano01.z - prev_pmz) <= 1.5f) && (fabs(p_mano01.z - prev_pmz) >= 0.1f)) pmx = prev_pmx; 
		//if(pmx - prev_pmx >= 0.5f)  pmx = prev_pmx + ((pmx + prev_pmx)/3.0f);

		prev_pmx = pmx;
        prev_pmz = pmz;

		//n_err_msg("", "pmx: %f; prev_pmx: %f\nmpz: %f ; prev mpz: %f", pmx, prev_pmx, pmz, prev_pmz);

		if(pmx <= 3) pmx = 3.0f;
		if(pmx >= 37) pmx = 37.0f;


        //float paux=0.0f, pauz=0.0f;
		//pauz = pmx;
		//paux = pmz;
		//pmx = paux;
		//pmz = pauz;


//pmz -= 110;

        // MANO 01
		matriz3[0][4] = (float)(cam_id); 
	    matriz3[0][5] = (float)(pmx); //p_mano01.x);
	    matriz3[0][6] = (float)(pmy); //(p_mano01.y);
	    matriz3[0][7] = (float)(pmz); //(p_mano01.z);
        
		if(first_time_cam02){
			if(abs(pmx - 240) < 7 && abs(pmx - 240) > 1){
			   if(abs(pmz - 90) < 7 && abs(pmz - 90) > 1){
				  first_time_cam02 = false;
			      n_err_msg("CAM02", "La mano está en su sitio.");			      
			   }
			}		
		}

        // now update the UI
        //m_strYaw.Format(TEXT("%.3f"), matriz3[0][1]);
        //m_strPitch.Format(TEXT("%.3f"), matriz3[0][2]);
        //m_strRoll.Format(TEXT("%.3f"), matriz3[0][3]);
        m_strYaw.Format(TEXT("%.3f"),   p_mano01.x);
        m_strPitch.Format(TEXT("%.3f"), p_mano01.y );
        m_strRoll.Format(TEXT("%.3f"),  p_mano01.z);
        

        m_strX.Format(TEXT("%.3f"), matriz3[0][5]);
        m_strY.Format(TEXT("%.3f"), matriz3[0][6]);
        m_strZ.Format(TEXT("%.3f"), matriz3[0][7]);

		//n_err_msg("err", "dedo = %f %f %f\nmano = %f %f %f\n", p_dedo01.x,p_dedo01.y,p_dedo01.z,p_mano01.x,p_mano01.y, p_mano01.z);
	}


    update_socket_emisor(cam_id);

}

void CCameraDlg::UpdateSwitchState()
{
    LONG lSwitchState;

    if (m_spFrame)
    {
        m_spFrame->get_SwitchState(&lSwitchState);

        switch (lSwitchState)
        {
            case NP_SWITCH_STATE_BOTH_DOWN:
                m_strSwitchState = TEXT("Both DOWN");
                break;
        
            case NP_SWITCH_STATE_ONE_DOWN:
                m_strSwitchState = TEXT("1: DOWN 2: UP");
                break;

            case NP_SWITCH_STATE_TWO_DOWN:
                m_strSwitchState = TEXT("1: UP 2: DOWN");
                break;

            case NP_SWITCH_STATE_BOTH_UP:
                m_strSwitchState = TEXT("Both UP");
                break;
        }
    }
}

void CCameraDlg::OnButtonOptions() 
{
    pCCameraOptionsDlg->ShowWindow(SW_SHOW);
    pCCameraOptionsDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
}

void CCameraDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    if (m_spFrame)
    {
        m_spCamera->DrawFrame(m_spFrame, (LONG) GetDlgItem(IDC_STATIC_FRAME_IMAGE)->GetSafeHwnd());
    }
    
//	secs = timer.GetElapsedSeconds();
//	fps  = timer.GetFPS(1);
//	timer.LockFPS(25);

	// Do not call CDialog::OnPaint() for painting messages
}

void CCameraDlg::OnButtonResetVector() 
{

	// PARA RESETEAR Y COLOCAR EL CERO
	CComVariant varYaw, varPitch, varRoll, varX, varY, varZ, varEmpty;

    // get the values back
    m_spVector->get_Yaw(&varYaw);
    m_spVector->get_Pitch(&varPitch);
    m_spVector->get_Roll(&varRoll);


	///////////////////////////////////////////////// TOMA LA POSICIÓN INCIAL DE LA CABEZA 
	if(cam_id == CAM_01){
	   yaw_centro   = varYaw.dblVal;
	   pitch_centro = varPitch.dblVal;
	   roll_centro  = varRoll.dblVal;
	}

	///////////////////////////////////////////////// TOMA LA POSICIÓN INCIAL DEL DEDO Y DE LA MANO
	else if(cam_id == CAM_02){
        LONG lCount = 0;
        m_spFrame->get_Count(&lCount);

        // gestión de error - si hay más de dos puntos avisar
        if(lCount > 2) n_err_msg("ADVERTENCIA", "Hay más de dos puntos visibles.\nSe usarán los 2 primeros encontrados (que quizás no sean información válida).");
        
		// VOLVER A PONER LCOUNT < 2 CUANDO TERMINE DE HACER ESTAS COSITAS 
        //for(int i = 0; i < lCount; i++){
		for(int i = 0; i < lCount; i++){         // por ahora sólo tenemos dos cosas para trackear
            CComPtr<INPObject> spObject;
            m_spFrame->Item(i, &spObject);           
            CComVariant varX2, varY2, varArea2;			

            // values are double's so we need to use a variant to query for them
            spObject->get_X(&varX2);
            spObject->get_Y(&varY2);
            spObject->get_Area(&varArea2);
     
			///////////////////////////////////////// RANGO DE LOS PUNTOS
			/*
            LONG lRank;
            spObject->get_Rank(&lRank);
            if (lRank == 1)
            {
                // this is the currently tracked object... 
                CComVariant varX, varY, varArea;			

                // values are double's so we need to use a variant to query for them
                spObject->get_X(&varX);
                spObject->get_Y(&varY);
                spObject->get_Area(&varArea);

                // smooth the data
                m_spSmoothingDot->Update(varX, varY);

                // grab the smoothed data
                m_spSmoothingDot->get_X(&varX);
                m_spSmoothingDot->get_Y(&varY);

                // update the UI
                m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX.dblVal, varY.dblVal);
                m_strDotArea.Format(TEXT("%.f"), varArea.dblVal);
            }
            */
            //////////////////////////////////////////////////////////////////////////////////////

			//g_mano.addVertex(tVector3((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal)), i);         		
			g_mano.addVertex((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal), i);         					
			//n_err_msg("info", "gmano [%d]: %.2f %.2f %.2f", i, g_mano.vertices[i].x, g_mano.vertices[i].y, g_mano.vertices[i].z);
		}

////////////////////////////////////////////////// FALTA EL PUTO QUICKSORT EN RESET ///////////////////////////////////

		//tVector3 aux; //=NULL_VEC3;
		// EL PUNTO 0 ES EL DEDO : nota - HACER UN PUTO GETTER PORQUE VISUAL STUDIO ES QUISQUILLOSO A MORIR
		//if((float)(g_mano.vertices[0].x) < (float)(g_mano.vertices[1])){
        if(g_mano.getVertexX(0) < g_mano.getVertexX(1)){
			c_dedo01 = g_mano.vertices[0]; 
			c_mano01 = g_mano.vertices[1];
		}
		// LOS PUNTOS ESTÁN AL REVÉS Y EL 1 ES EL DEDO
		else{
			c_dedo01 = g_mano.vertices[1]; 
			c_mano01 = g_mano.vertices[0];
		}
	}
	/////////////////////////////////////////////////////////////////////

    //m_spVector->get_X(&varX);
    //m_spVector->get_Y(&varY);
    //m_spVector->get_Z(&varZ);
	
	m_spVector->Reset();	
    
    m_spSmoothingYaw->Reset();
    m_spSmoothingPitch->Reset();
    m_spSmoothingRoll->Reset();
    m_spSmoothingX->Reset();
    m_spSmoothingY->Reset();
    m_spSmoothingZ->Reset();
}

void CCameraDlg::OnSelchangeComboUpdateRate() 
{
    m_nUpdateRate = m_comboUpdateRate.GetCurSel() + 1;
}

BOOL CCameraDlg::Create( UINT nIDTemplate, CWnd* pParentWnd )
{
    BOOL Result;
    if(Result = CDialog::Create(nIDTemplate, pParentWnd))
    {
        pCCameraOptionsDlg = new CCameraOptionsDlg(NULL, m_spCamera);
        pCCameraOptionsDlg->Create(IDD_CAMERA_OPTIONS,NULL);
        CString strTitle;
        pCCameraOptionsDlg->GetWindowText(strTitle);
        strTitle += TEXT(" ");
        strTitle += m_strSerialNumber;
        pCCameraOptionsDlg->SetWindowText(strTitle);
    }
    return Result;
}
void CCameraDlg::OnActivate(   UINT nState,   CWnd* pWndOther,   BOOL bMinimized )
{
    CDialog::OnActivate(nState,pWndOther,bMinimized );
    if( nState == WA_ACTIVE)
    {
        SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
    }
}

void CCameraDlg::HandleFrameAvailableIdCamera(INPCamera * pCamera, LONG Group, LONG Id)
{
//    TRACE2("HandleFrame Available %p %p\n", this, pCamera);
    CComPtr<INPCameraFrame> spFrame;
    if(S_OK == m_spCamera->GetFrameById(Id, &spFrame))
    {
        if (m_spFrame)
        {
            m_spFrame->Free();  //free the buffer
            m_spFrame = NULL;   //release the COM object.
        }
        m_spFrame = spFrame;    //save the new frame
//        ::PostMessage(GetSafeHwnd(), WM_FRAME_AVAILABLE, (WPARAM) pCamera, 0);

        OnWMFrameAvailable((WPARAM)pCamera, 0);
        //        ::PostMessage(GetSafeHwnd(), WM_FRAME_AVAILABLE, (WPARAM) pCamera, 0);
    }
}
