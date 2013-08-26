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


/////////////////////////////////////
// variables nuevas
SOCKET sock2;  // CAM 1 socket
//SOCKET sock3;  // CAM 2 socket
struct sockaddr_in name2; // CAM 1  
//struct sockaddr_in name3; // CAM 2
float  matriz2[1][4];	  // CAM 1
float  matriz3[2][4];	  // CAM 2

int    err;
int    ttl=2;
int    msg_actual = 0;
int    puerto=2020;
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

void init_socket_emisor(int puerto_controles_1){ //, int camara){ // parece ser que no necesito saber la cámara...

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

void update_socket_emisor(int camara){
	int i;
	static float mtemp[2][4];	 // antes era 1/4 pero la aumenté para que quepa matriz3 (CAM_02) también

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
	     if(sendto (sock2, (char *)matriz3, sizeof(matriz3), 0, (struct sockaddr *)&name2, sizeof name2) == SOCKET_ERROR)		
            msg_actual = 5;	
	     else {			
 		    int m2, n2; 
		    for(m2=0; m2<2; m2++)
			    for(n2=0; n2<4; n2++)
			       mtemp[m2][n2] = matriz3[m2][n2];			
		 }
		 msg_actual = 4; 
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

	for(i=0;i<SMOOTH_VAL;i++){
	   yaw_smooth[i]   = 0.0f;
	   pitch_smooth[i] = 0.0f;
	   roll_smooth[i]  = 0.0f;
	}

    p_mano01 = NULL_VEC3; // 0 0 0
    p_dedo01 = NULL_VEC3; //   "

	c_mano01 = NULL_VEC3; //   "
	c_dedo01 = NULL_VEC3; //   "


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
	else if(cam_id == CAM_02) puerto = 2021;
	init_socket_emisor(puerto);
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
       fy *= 1.27f;
	   fp *= 1.35f;
	   fr *= 1.1f;

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
	}
//	else if(cam_id == CAM_02){
//	
//	
//	}


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


	if(cam_id == CAM_01){
	   yaw_centro   = varYaw.dblVal;
	   pitch_centro = varPitch.dblVal;
	   roll_centro  = varRoll.dblVal;
	}
//	else if(cam_id == CAM_02){
//
//	}

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
