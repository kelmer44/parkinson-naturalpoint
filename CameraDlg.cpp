// CameraDlg.cpp : implementation file
//

/*
Nombre: Cam Control. Aplicaci�n para experiencias m�dicas para sujetos con Alzheimer.
Autor:  Almar Su�rez
Fecha de inicio: -
�ltima revisi�n: 11:17 19/10/2009
Notas: [11:17 19/10/2009] agrego la c�mara con vista cenital para los ejes xz 
de las dos manos de forma simult�nea
*/

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
#include <time.h>
#include <winsock2.h>
#include <WS2TCPIP.h>
#include <windows.h>
#include "J_Enviar.h"
#include "J_Recibir.h"

//#include "cam_idez.h"

#define GRUPO "127.0.0.1" //"193.144.46.66" //"127.0.0.1" //"193.144.46.66"

#define PI 3.14159265


#define _msg_null       "..."
#define _msg_error      "<!> --- Error en socket ---"
#define _msg_err_send   "<!> Fallo en send"
#define _msg_escuchando "< > Escribiendo en el puerto definido"
#define _msg_enviando   "<+> Enviando bytes "
#define _msg_datagrama  "<!> No se puede enviar el datagrama "


#define CAM_01 67724  // CAMBIO EL ORDEN OTRA PUTA VEZ
#define CAM_02 79156 
//#define CAM_02 98432 //79156 ///98432   //  // 79154  // 67724 - la 677 era la antigua c�mara para la mano derecha, pero como se jodi� ah� queda
#define CAM_03  89897 

//#define CAM_03 100088 //#define CAM_03 98433  // 79154 - la 79154 ahora va para la mano derecha, la 677 no funciona
//#define CAM_04 79154 
#define CAM_04 85760
//#define CAM_04 98431 // //98431 //79154 //98431 //79154 //98431  // 79156  //98431 // c�mara vista cenital  - N�MERO ORIGINAL M�DICOS EN CORU�A - 98431

bool  first_time_cam02=true;
bool  congelaX=false,  congelaZ=false;
int   congela_contX=0, congela_contZ=0;
float prev_pmx=0.0f,   prev_pmz=0.0f,   prev_pmx2=0.0f,  prev_pmz2=0.0f;


/////////////////////////////////////
// variables nuevas
SOCKET sock2;  // CAM 1 socket
SOCKET sock3;  // CAM 2 socket
SOCKET sock4;  // CAM 3 socket
SOCKET sock5;  // CAM 4 socket

struct sockaddr_in name2; // CAM 1  
struct sockaddr_in name3; // CAM 2
struct sockaddr_in name4; // CAM 3
struct sockaddr_in name5; // CAM 4

sockets::J_Recibir *j_receiver[4];
sockets::J_Enviar  *j_sender[4];

float remix[40]; // variable usada para juntar la informaci�n de todas las c�maras, centralizarla, y enviarla

float  matriz2[1][4];	  // CAM 1
float  matriz3[1][8];	  // CAM 2
float  matriz4[1][8];	  // CAM 3
float  matriz5[1][8];     // CAM 4 [xz] mano_derecha, [xz] mano_izquierda
//float  matriz3[1][4]; 

int    err;
int    ttl=2;
int    msg_actual = 0;
int    puerto=0; //2020;
//int    teclas[3];
int frameCount4 = 0;
int frameCount2 = 0;
int frameCount3 = 0;
/*
void n_err_msg(char *box_name, char *fmt,...){ 

va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
char    str[500];			      // buffer para construir la cadena
int     cnt;			          // contador para almacenar el resultado de sprintf

va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
vsprintf(str, fmt, arg_ptr);
va_end(arg_ptr);			          // cierra las funciones va

//sprintf(str, 
MessageBoxA(NULL, str, box_name, MB_OK); 
}*/ 

void init_socket_emisor(int puerto_controles_1, int cam){ //, int camara){ // parece ser que no necesito saber la c�mara...

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
		//	   n_err_msg("err", "ACTIVADO SOCKET CAM01");
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
		//	   n_err_msg("err", "ACTIVADO SOCKET CAM02");
		//n_err_msg("efrr", "PUPU");
		}
	else if(cam == CAM_03){
		sock4 = socket(AF_INET, SOCK_DGRAM, 0);
		WORD wVersionRequerida3;
		WSADATA wsaDatos3;
		wVersionRequerida3 = MAKEWORD(1,1);
		int error3 = WSAStartup(wVersionRequerida3, &wsaDatos3);
		if(error3 !=0){
			n_err_msg("Error", "No se pudo inicializar la API Winsock\n");
			WSACleanup();
			msg_actual = 1;
			return;
			}

		sock4 = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock4 == INVALID_SOCKET){
			n_err_msg("Error", "No se ha podido crear el socket\n");
			msg_actual = 1;
			return;
			}

		u_long nobloqueante3 =1;
		ioctlsocket(sock4, FIONBIO, &nobloqueante3);
		name4.sin_family = AF_INET;
		name4.sin_addr.s_addr = inet_addr(GRUPO);
		name4.sin_port=htons(puerto_controles_1);

		msg_actual = 3; // enviando al puerto blablabla...		
		//	   n_err_msg("err", "ACTIVADO SOCKET CAM03");
		//n_err_msg("efrr", "PUPU");
		}
	else if(cam == CAM_04){
		sock5 = socket(AF_INET, SOCK_DGRAM, 0);
		WORD wVersionRequerida4;
		WSADATA wsaDatos4;
		wVersionRequerida4 = MAKEWORD(1,1);
		int error4 = WSAStartup(wVersionRequerida4, &wsaDatos4);
		if(error4 !=0){
			n_err_msg("Error", "No se pudo inicializar la API Winsock\n");
			WSACleanup();
			msg_actual = 1;
			return;
			}

		sock5 = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock5 == INVALID_SOCKET){
			n_err_msg("Error", "No se ha podido crear el socket\n");
			msg_actual = 1;
			return;
			}

		u_long nobloqueante4 = 1;
		ioctlsocket(sock5, FIONBIO, &nobloqueante4);
		name5.sin_family = AF_INET;
		name5.sin_addr.s_addr = inet_addr(GRUPO);
		name5.sin_port=htons(puerto_controles_1);

		msg_actual = 3; // enviando al puerto blablabla...		
		//	   n_err_msg("err", "ACTIVADO SOCKET CAM03");
		//n_err_msg("efrr", "PUPU");
		}
	}

void update_socket_emisor(int camara){
	int i;
	static float mtemp[1][8];	 // antes era 1/4 pero la aument� para que quepa matriz3 (CAM_02) tambi�n
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
	else if(camara == CAM_03){
		//	if( (fabs(mtemp[0][0] - matriz2[0][0]) > 0) |
		//		(fabs(mtemp[0][1] - matriz2[0][1]) > 0) |
		//		(fabs(mtemp[0][2] - matriz2[0][2]) > 0) |
		//		(fabs(mtemp[0][3] - matriz2[0][3]) > 0) ){  
		//
		if(sendto (sock4, (char *)matriz4, sizeof(matriz4), 0, (struct sockaddr *)&name4, sizeof name4) == SOCKET_ERROR)		
			msg_actual = 5;	
		else {			
			int m3, n3; 
			for(m3=0; m3<1; m3++)
				for(n3=0; n3<8; n3++) // ANTES 4
					mtemp[m3][n3] = matriz4[m3][n3];			
		 }
		msg_actual = 4; 
		//n_err_msg("err", "PEPE");
		//}      		
		}
	else if(camara == CAM_04){
		//	if( (fabs(mtemp[0][0] - matriz2[0][0]) > 0) |
		//		(fabs(mtemp[0][1] - matriz2[0][1]) > 0) |
		//		(fabs(mtemp[0][2] - matriz2[0][2]) > 0) |
		//		(fabs(mtemp[0][3] - matriz2[0][3]) > 0) ){  
		//
		if(sendto(sock5, (char *)matriz5, sizeof(matriz5), 0, (struct sockaddr *)&name5, sizeof name5) == SOCKET_ERROR)		
			msg_actual = 5;	
		else {			
			int m4, n4; 
			for(m4=0; m4<1; m4++)
				for(n4=0; n4<8; n4++) // ANTES 4
					mtemp[m4][n4] = matriz4[m4][n4];			
		 }
		msg_actual = 4; 
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
	ON_STN_CLICKED(IDC_FRAMEID, &CCameraDlg::OnStnClickedFrameid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg message handlers

BOOL CCameraDlg::OnInitDialog() 
	{
	CDialog::OnInitDialog();

	HRESULT hr = S_OK;
	int i; // agregado para inicializar el vector de smooth

	////////////////////////////////////////////////////////////////////////
	// inicializaci�n de los valores para el 0
	yaw_centro   = 0.0f;
	pitch_centro = 0.0f;
	roll_centro  = 0.0f;
	curr_smooth  = 0;
	curr_mano_smooth  = 0;
	curr_mano2_smooth = 0;

	for(i=0;i<SMOOTH_VAL;i++){
		yaw_smooth[i]    = 0.0f;
		pitch_smooth[i]  = 0.0f;
		roll_smooth[i]   = 0.0f;
		}
	for(i=0;i<SMOOTH_MANO;i++){
		manox_smooth[i]  = 0.0f;
		manoy_smooth[i]  = 0.0f;
		manoz_smooth[i]  = 0.0f;

		mano2x_smooth[i] = 0.0f;
		mano2y_smooth[i] = 0.0f;
		mano2z_smooth[i] = 0.0f;
		}

	p_mano01   = NULL_VEC3; // 0 0 0
	p_dedo01   = NULL_VEC3; //   "
	p_dedo02   = NULL_VEC3;
	p_dedo03   = NULL_VEC3;

	c_mano01   = NULL_VEC3; //   "
	c_dedo01   = NULL_VEC3; //   "
	c_dedo02   = NULL_VEC3;
	c_dedo03   = NULL_VEC3;

	p_mano21   = NULL_VEC3; // 0 0 0
	p_dedo21   = NULL_VEC3; //   "
	p_dedo22   = NULL_VEC3;
	p_dedo23   = NULL_VEC3;

	c_mano21   = NULL_VEC3; //   "
	c_dedo21   = NULL_VEC3; //   "
	c_dedo22   = NULL_VEC3;
	c_dedo23   = NULL_VEC3;

	xz_mano01  = NULL_VEC3; // es obvio, no gabriel? si no te das cuenta qu� es esto, MARICA ERES xdd
	xz_mano02  = NULL_VEC3;

	//xz_punta01 = NULL_VEC3;
	//xz_punta02 = NULL_VEC3;

	//xz_muneca01 =NULL_VEC3;
	//xz_muneca02 =NULL_VEC3;

	// MANO DERECHA ---dedo y altura---
	g_mano = vs_group3d_t(4); // dedo(3) y mano(1)
	g_mano.init();
	g_mano.addVertex(NULL_VEC3, 0); // si esto pasa de 5 empezar� a usar un bucle for...
	g_mano.addVertex(NULL_VEC3, 1);
	g_mano.addVertex(NULL_VEC3, 2);
	g_mano.addVertex(NULL_VEC3, 3);

	// MANO IZQUIERDA ---dedo y altura---
	g_mano2 = vs_group3d_t(4); // dedo(3) y mano(1)
	g_mano2.init();
	g_mano2.addVertex(NULL_VEC3, 0); // si esto pasa de 5 empezar� a usar un bucle for...
	g_mano2.addVertex(NULL_VEC3, 1);
	g_mano2.addVertex(NULL_VEC3, 2);
	g_mano2.addVertex(NULL_VEC3, 3);

	// LAS DOS MANOS ---movimiento en xz---
	g_manos = vs_group3d_t(15); // inicializo 12 v�rtices porque es el n�mero m�ximo de valores que deber�a llegar a leer -suponiendo que se vean todos los marcadores-
	g_manos.init();
	for(i=0;i<12;i++) g_manos.addVertex(NULL_VEC3, i);  // pas� de 5, usamos el bucle XD


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
	/*
	if(cam_id == CAM_01)      puerto = 2020;
	else if(cam_id == CAM_02) puerto = 2022;
	else if(cam_id == CAM_03) puerto = 2024;
	else if(cam_id == CAM_04) puerto = 2022; //2026;
	init_socket_emisor(puerto, cam_id);
	*/		
	if(cam_id == CAM_01){
		//emisorCAM01 = net_emisor_t("127.0.0.1", PUERTO_COM_CAM01, 3); // 3 valores: �ngulo cabeza [xyz] (por si tal...)
		//emisorCAM01.init_emisor();
		}      
	else if(cam_id == CAM_02){
		j_sender[1] = new sockets::J_Enviar();
		j_sender[1]->setPort(PUERTO_COM_CAM02);
		//emisorCAM02 = net_emisor_t("127.0.0.1", PUERTO_COM_CAM02, 4); // 4 valores: �ngulo dedos, altura mano [MANO DERECHA]
		//emisorCAM02.init_emisor();
		} 
	else if(cam_id == CAM_03){
		j_sender[2] = new sockets::J_Enviar();
		j_sender[2]->setPort(PUERTO_COM_CAM03);
		//emisorCAM03 = net_emisor_t("127.0.0.1", PUERTO_COM_CAM03, 4); // 4 valores: �ngulo dedos, altura mano [MANO IZQUIERDA]
		//emisorCAM03.init_emisor();	
		} 
	else if(cam_id == CAM_04){
		j_receiver[0] = new sockets::J_Recibir();
		j_receiver[0]->setPort(PUERTO_COM_CAM01);	

		j_receiver[1] = new sockets::J_Recibir();
		j_receiver[1]->setPort(PUERTO_COM_CAM02);

		j_receiver[2] = new sockets::J_Recibir();
		j_receiver[2]->setPort(PUERTO_COM_CAM03);

		j_sender[3] = new sockets::J_Enviar();
		j_sender[3]->setPort(PUERTO_COM_CAM04OUTPUT);
		//   receptorCAM04deCAM01 = net_receptor_t("127.0.0.1", PUERTO_COM_CAM01, 3); 
		//receptorCAM04deCAM01.init_receptor();
		//
		//receptorCAM04deCAM02 = net_receptor_t("127.0.0.1", PUERTO_COM_CAM02, 4); 
		//receptorCAM04deCAM02.init_receptor();

		//receptorCAM04deCAM03 = net_receptor_t("127.0.0.1", PUERTO_COM_CAM03, 4); 
		//receptorCAM04deCAM03.init_receptor();

		//emisorCAM04TODO      = net_emisor_t("127.0.0.1",   PUERTO_COM_CAM04OUTPUT, 17); // 3 + 4 + 4 + 2[xz mano derecha] + 2[xz mano izquierda] + 2[xz ang pitch] = 17 
		//emisorCAM04TODO.init_emisor();
		} 
	//init_socket_emisor(puerto, cam_id);
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

	////////////// agregado para controlar la c�mara actual ///////////////////////
	CString serial1, serial2, serial3, serial4;

	//	serial1 += TEXT("79156");
	//	serial2 += TEXT("67724");
	//	serial3 += TEXT("79154");
	serial1 += TEXT("67724");
	serial2 += TEXT("79156"); 
	//serial2 += TEXT("98432"); //("98431"); //TEXT("79154");  // CAMBI� EL PUTO ORDEN PORQUE CASC� LA OTRA C�MARA DE LOS HUEVOS.
	//serial3 += TEXT("100088"); //serial3 += TEXT("89897");//serial3 += TEXT("98433"); //("79156");
	serial3 += TEXT("89897"); //("79156");
	//serial4 += TEXT("98431"); //"79156"); //"98431");  - ESTA ES LA DE VERDAD EN EL EQUIPO DE LOS M�DICOS
	serial4 += TEXT("85760"); //"79156"); //"98431");  - ESTA ES LA DE VERDAD EN EL EQUIPO DE LOS M�DICOS
	if(m_strSerialNumber.Compare(serial1) == 0)      cam_id = CAM_01;
	else if(m_strSerialNumber.Compare(serial2) == 0) cam_id = CAM_02;
	else if(m_strSerialNumber.Compare(serial3) == 0) cam_id = CAM_03;
	else if(m_strSerialNumber.Compare(serial4) == 0) cam_id = CAM_04;

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

	//  LONG lCount = 0;
	//  pFrame->get_Count(&lCount);

	//  for (int i = 0; i < lCount; i++)
	//  {
	//      CComPtr<INPObject> spObject;

	//      pFrame->Item(i, &spObject);

	//      LONG lRank;

	///////////////////////////////////////////////////
	//      CComVariant varX2, varY2, varArea2;			

	//      // values are double's so we need to use a variant to query for them
	//      spObject->get_X(&varX2);
	//      spObject->get_Y(&varY2);
	//      spObject->get_Area(&varArea2);

	//      // update the UI
	//      m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX2.dblVal, varY2.dblVal);
	//      m_strDotArea.Format(TEXT("%.f"), varArea2.dblVal);
	//       ///////////////////////////


	//      spObject->get_Rank(&lRank);
	//      if (lRank == 1)
	//      {
	//          //this is the currently tracked object... 
	//          CComVariant varX, varY, varArea;			

	//          //values are double's so we need to use a variant to query for them
	//          spObject->get_X(&varX);
	//          spObject->get_Y(&varY);
	//          spObject->get_Area(&varArea);

	//          //smooth the data
	//          m_spSmoothingDot->Update(varX, varY);

	//          //grab the smoothed data
	//          m_spSmoothingDot->get_X(&varX);
	//          m_spSmoothingDot->get_Y(&varY);

	//          //update the UI
	//          m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX.dblVal, varY.dblVal);
	//          m_strDotArea.Format(TEXT("%.f"), varArea.dblVal);
	//      }
	//

	//  }
	}

void CCameraDlg::UpdateVectorInfo(INPCameraFrame * pFrame)
	{
	LONG lCount = 0;
	if(cam_id == CAM_02){
		pFrame->get_Count(&lCount);
		// si no hay exactamente 2 puntos, se congelan los c�lculos, �seguro que quiero hacerlo as� y no coger los m�s a la izda?
		if(lCount >= 3){

			float xaux=0.0f, yaux=0.0f, zaux=0.0f;
			// SI HAY M�S OBJETOS DE LOS QUE DEBER�A, EN ESTE CASO - ME LA PELA
			for(int i = 0; i < lCount; i++){         // por ahora s�lo tenemos dos cosas para trackear
				CComPtr<INPObject> spObject;
				pFrame->Item(i, &spObject);           
				CComVariant varX2, varY2, varArea2;			

				// values are double's so we need to use a variant to query for them
				spObject->get_X(&varX2);
				spObject->get_Y(&varY2);
				spObject->get_Area(&varArea2);

				g_mano.addVertex((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal), i);
				}

			/////////////////////////////////////////////// REORDENAMIENTO DE LOS DEDOS ///////////////////////////////////
			// SEGUN EL EJE X
			for(int k=0;k<4;k++){
				for(int l=0;l<4;l++){
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


			p_dedo01.x = g_mano.vertices[0].x;
			p_dedo01.y = g_mano.vertices[0].y; 
			p_dedo01.z = g_mano.vertices[0].z;

			p_mano01.x = g_mano.vertices[1].x; 
			p_mano01.y = g_mano.vertices[1].y; 
			p_mano01.z = g_mano.vertices[1].z;

			p_muneca01.x = g_mano.vertices[2].x; 
			p_muneca01.y = g_mano.vertices[2].y; 
			p_muneca01.z = g_mano.vertices[2].z;


			//////////////////////////////////////////////////////////////
			float buffer1[6];

			buffer1[0] = p_mano01.x; //enviamos la coord z segun esta camara
			buffer1[1] = p_mano01.y; //enviamos la coord z segun esta camara
			buffer1[2] = p_muneca01.y;
			buffer1[3] = p_dedo01.y;
			buffer1[4] = frameCount2++;
				VARIANT* pVal = new VARIANT;
				pFrame->get_TimeStamp(pVal);
				buffer1[5] = pVal->dblVal;

			j_sender[1]->setBuffer((void *)buffer1,sizeof(buffer1));
			j_sender[1]->sync();
			//////////////////////////////////////////////////////////////

			m_strYaw.Format(TEXT("%.3f"),   p_mano01.x);
			m_strPitch.Format(TEXT("%.3f"), p_mano01.y);
			m_strRoll.Format(TEXT("%.3f"),  p_muneca01.y);

			m_strX.Format(TEXT("PMY: %.3f"), p_dedo01.y);
			m_strY.Format(TEXT("%.3f"), p_dedo01.y);
			m_strZ.Format(TEXT("%.3f"), matriz3[0][7]);

			} // END IF LCOUNT == 4
		} // END IF CAM02
	else if(cam_id == CAM_03)
	{
		pFrame->get_Count(&lCount);
		// si no hay exactamente 2 puntos, se congelan los c�lculos, �seguro que quiero hacerlo as� y no coger los m�s a la izda?
		if(lCount >= 3){


			float xaux=0.0f, yaux=0.0f, zaux=0.0f;

			for(int i = 0; i < lCount; i++){         // por ahora s�lo tenemos dos cosas para trackear
				CComPtr<INPObject> spObject2;
				pFrame->Item(i, &spObject2);           
				CComVariant varX22, varY22, varArea22;			

				// values are double's so we need to use a variant to query for them
				spObject2->get_X(&varX22);
				spObject2->get_Y(&varY22);
				spObject2->get_Area(&varArea22);

				g_mano2.addVertex((float)(varX22.dblVal), (float)(varY22.dblVal), (float)(varArea22.dblVal), i);
				}

			/////////////////////////////////////////////// REORDENAMIENTO DE LOS DEDOS ///////////////////////////////////
			float xaux2=0.0f, yaux2=0.0f, zaux2=0.0f;

			for(int k=0;k<4;k++){
				for(int l=0;l<4;l++){
					if(g_mano2.getVertexX(k) > g_mano2.getVertexX(l)){
						xaux2 = g_mano2.getVertexX(l); // es una ELE no un UNO
						yaux2 = g_mano2.getVertexY(l);
						zaux2 = g_mano2.getVertexZ(l);

						g_mano2.vertices[l].x = g_mano2.vertices[k].x;
						g_mano2.vertices[l].y = g_mano2.vertices[k].y;
						g_mano2.vertices[l].z = g_mano2.vertices[k].z;

						g_mano2.vertices[k].x = xaux2;
						g_mano2.vertices[k].y = yaux2;
						g_mano2.vertices[k].z = zaux2;
						}
					}
				}


			p_dedo01.x = g_mano2.vertices[0].x; 
			p_dedo01.y = g_mano2.vertices[0].y; 
			p_dedo01.z = g_mano2.vertices[0].z;
			p_mano01.x = g_mano2.vertices[1].x; 
			p_mano01.y = g_mano2.vertices[1].y; 
			p_mano01.z = g_mano2.vertices[1].z;
			p_muneca01.x = g_mano2.vertices[2].x; 
			p_muneca01.y = g_mano2.vertices[2].y; 
			p_muneca01.z = g_mano2.vertices[2].z;


			float buffer2[5];

			buffer2[0] = p_mano01.x;
			buffer2[1] = p_mano01.y;
			buffer2[2] = p_muneca01.y;
			buffer2[3] = p_dedo01.y;
			buffer2[4] = frameCount3++;
				VARIANT* pVal = new VARIANT;
				pFrame->get_TimeStamp(pVal);
				buffer2[5] = pVal->dblVal;

			j_sender[2]->setBuffer((void *)buffer2,sizeof(buffer2));
			j_sender[2]->sync();

			m_strYaw.Format(TEXT("%.3f"),   p_mano01.x);
			m_strPitch.Format(TEXT("%.3f"), p_mano01.y);
			m_strRoll.Format(TEXT("%.3f"),  p_mano01.z);

			m_strX.Format(TEXT("%.3f"), matriz4[0][5]);
			m_strY.Format(TEXT("%.3f"), matriz4[0][6]);
			m_strZ.Format(TEXT("%.3f"), matriz4[0][7]);
			} // EN IF LCOUNT2 == 4
	}
	else if(cam_id == CAM_04){        
		pFrame->get_Count(&lCount);
		int num_vertices=0; 
		tVector3 xz_punta01 = NULL_VEC3, xz_punta02 = NULL_VEC3; // las puntas de los dedos en xz para calcular el puto movimiento lateral a�adido
		tVector3 xz_muneca01 = NULL_VEC3, xz_muneca02 = NULL_VEC3; // la jodida muneca para corregir el giro de los dedos
		CComVariant varX, varY, varArea;

		if( lCount >= 3){
			// SI HAY M�S OBJETOS DE LOS QUE DEBER�A, EN ESTE CASO - ME LA PELA
			for(int i = 0; i < lCount; i++){         // por ahora s�lo tenemos dos cosas para trackear

				CComPtr<INPObject> spObject40;
				pFrame->Item(i, &spObject40);           

				// values are double's so we need to use a variant to query for them
				spObject40->get_X(&varX);
				spObject40->get_Y(&varY);
				spObject40->get_Area(&varArea);

				g_manos.addVertex((float)(varX.dblVal), (float)(varY.dblVal), (float)(varArea.dblVal), i);
				}

			num_vertices = (int)lCount;


			float xaux3=0.0f, yaux3=0.0f, zaux3=0.0f;
			//ORDENA DE MAYOR A MENOR
			for(int k3=0;k3<num_vertices;k3++){
				for(int l3=0;l3<num_vertices;l3++){
					if(g_manos.getVertexX(k3) > g_manos.getVertexX(l3)){
						xaux3 = g_manos.getVertexX(l3); // es una ELE no un UNO
						yaux3 = g_manos.getVertexY(l3);
						zaux3 = g_manos.getVertexZ(l3);

						g_manos.vertices[l3].x = g_manos.vertices[k3].x;
						g_manos.vertices[l3].y = g_manos.vertices[k3].y;
						g_manos.vertices[l3].z = g_manos.vertices[k3].z;

						g_manos.vertices[k3].x = xaux3;
						g_manos.vertices[k3].y = yaux3;
						g_manos.vertices[k3].z = zaux3;
						}
					}
				}

			// los puntos con menor x (ultimos en la lista) son la mano derecha
			tVector3 aux_array_der[3];
			tVector3 aux_array_iz[3];

			aux_array_der[0] = g_manos.vertices[num_vertices-1];  // antes 0
			aux_array_der[1] = g_manos.vertices[num_vertices-2];  // antes 0
			aux_array_der[2] = g_manos.vertices[num_vertices-3];  // antes 0

			// los puntos con mayor x (primeros en la lista) son la mano izquierda
			aux_array_iz[0] = g_manos.vertices[0];  // antes 0
			aux_array_iz[1] = g_manos.vertices[1];  // antes 0
			aux_array_iz[2] = g_manos.vertices[2];  // antes 0


			//AHORA LOS ORDENAMOS POR Y, MU�ECA CON EL MENOR Y, LUEGO MANO Y LUEGO PUNTA
			for(int k3=0;k3<3;k3++){
				for(int l3=0;l3<3;l3++){
					if(aux_array_der[k3].y > aux_array_der[l3].y){
						xaux3 = aux_array_der[l3].x; // es una ELE no un UNO
						yaux3 = aux_array_der[l3].y;
						zaux3 = aux_array_der[l3].z;

						aux_array_der[l3] = aux_array_der[k3];

						aux_array_der[k3].x = xaux3;
						aux_array_der[k3].y = yaux3;
						aux_array_der[k3].z = zaux3;
						}
					}
				}
			//MANO IZQUIERDA
			for(int k3=0;k3<3;k3++){
				for(int l3=0;l3<3;l3++){
					if(aux_array_iz[k3].y > aux_array_iz[l3].y){
						xaux3 = aux_array_iz[l3].x; // es una ELE no un UNO
						yaux3 = aux_array_iz[l3].y;
						zaux3 = aux_array_iz[l3].z;

						aux_array_iz[l3] = aux_array_iz[k3];

						aux_array_iz[k3].x = xaux3;
						aux_array_iz[k3].y = yaux3;
						aux_array_iz[k3].z = zaux3;
						}
					}
				}


			// los puntos con menor x (ultimos en la lista) son la mano derecha

			xz_muneca01		= aux_array_der[2];
			xz_mano01		= aux_array_der[1];  
			xz_punta01		= aux_array_der[0];  

			// los puntos con mayor x (primeros en la lista) son la mano izquierda
			xz_muneca02		= aux_array_iz[2];
			xz_mano02		= aux_array_iz[1];
			xz_punta02		= aux_array_iz[0];




			/* POSICION MANOS */
				if(false){
				// Esta conversion es similar a la que se hace de Celsius a Fahrenheit, simplemente pasamos
				// de un sistema de coordenadas en el que vamos de 0 a 355 (el de la camara) en el eje x
				// y lo tenemos que convertir al espacio en el que permitimos que la mano se mueva
				// multiplicamos el valor de x por el intervalo en X del sistema de ogre y sumamos el desplazamiento en 
				// el que empezamos (alli no empezamos en 0 sino en un numero determinado)
				// en este caso consideramos intervalo -2.5--->3 pero como necesitamos que la mano se mueva "al rev�s" de como
				// lo percibe la c�mara, lo hacemos al rev�s
				xz_mano01.x = xz_mano01.x * (-4.5/355) + 3;
				xz_mano01.y = xz_mano01.y * (-2.5/290) + 28;

				xz_mano02.x = xz_mano02.x * (-4.5/355) + 1.5;
				xz_mano02.y = xz_mano02.y * (-2.5/290) + 28;



				xz_punta01.x = xz_punta01.x * (-4.5/355) + 3;
				xz_punta01.y = xz_punta01.y * (-2.5/290) + 28 + 0.5;
				// Ahora 
				}


				/* RECECPCION Y ENVIO SOCKETS */
				{

				float buffer2[5],buffer3[5];
				bool recibe1, recibe2;

				j_receiver[1]->setBuffer((void *)buffer2,sizeof(buffer2));

				if(j_receiver[1]->sync()>=0)
					recibe1 = true;
				else
					recibe1 = false;

				j_receiver[2]->setBuffer((void *)buffer3,sizeof(buffer3));
				if(j_receiver[2]->sync()>=0)
					recibe2 = true;
				else
					recibe2 = false;

				// recibo de la c�mara 1 [CABEZA]
				remix[0]  = 1; //xz_punta01.x; // ang cabeza
				remix[1]  = 2; //xz_punta01.y; // ang cabeza
				remix[2]  = 3; // ang cabeza
				remix[23] = frameCount4++;
				VARIANT* pVal = new VARIANT;
				pFrame->get_TimeStamp(pVal);
				SYSTEMTIME SystemTime;
				GetSystemTime(&SystemTime);
				
				remix[24] = SystemTime.wMilliseconds;
				// recibo de la c�mara 2 [MANO DERECHA]
				if(recibe1){
					remix[3]  = buffer2[0];
					remix[4]  = buffer2[1];
					remix[5]  = buffer2[2];
					remix[6]  = buffer2[3];
					remix[25] = buffer2[4]; //frameId
					remix[26] = buffer2[5]; //frametime
					}

				// recibo de la c�mara 3 [MANO DERECHA]
				if(recibe2){
					remix[7]  = buffer3[0]; // ang dedo
					remix[8]  = buffer3[1]; // ang dedo
					remix[9]  = buffer3[2]; // ang dedo
					remix[10] = buffer3[3]; // pos y mano izquierda
					
					remix[27] = buffer3[4]; //frameId
					remix[28] = buffer3[5]; //frametime
					}


				// valores calculados ahora en la c�mara 04
				remix[11] = xz_muneca01.x;						// pos x muneca derecha
				remix[12] = xz_muneca01.y;						// pos z muneca derecha
				remix[13] = xz_mano01.x;						// pos x mano derecha
				remix[14] = xz_mano01.y;						// pos z mano derecha
				remix[15] = xz_punta01.x;						// pos x dedo derecho
				remix[16] = xz_punta01.y;						// pos z dedo derecho


				remix[17] = xz_muneca02.x;						// pos x muneca izquierda
				remix[18] = xz_muneca02.y;						// pos z muneca izquierda
				remix[19] = xz_mano02.x;						// pos x mano izquierda
				remix[20] = xz_mano02.y;						// pos z mano izquierda
				remix[21] = xz_punta02.x;						// pos x dedo izquierdo
				remix[22] = xz_punta02.y;						// pos z dedo izquierdo

				m_strYaw.Format(TEXT("%.3f"), remix[17]);
				m_strPitch.Format(TEXT("%.3f"), remix[18]);

				m_strRoll.Format(TEXT("%.3f"), remix[19]);
				m_strX.Format(TEXT("%.3f"), remix[20]);

				m_strY.Format(TEXT("%.3f"), remix[21]);
				m_strZ.Format(TEXT("%.3f"), remix[22]);


				j_sender[3]->setBuffer((void *)remix, sizeof(remix));
				j_sender[3]->sync();

				}
			}
		}  // END IF CAM04
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


	///////////////////////////////////////////////// TOMA LA POSICI�N INCIAL DE LA CABEZA 
	if(cam_id == CAM_01){
		yaw_centro   = varYaw.dblVal;
		pitch_centro = varPitch.dblVal;
		roll_centro  = varRoll.dblVal;
		}

	/*
	///////////////////////////////////////////////// TOMA LA POSICI�N INCIAL DEL DEDO Y DE LA MANO
	else if(cam_id == CAM_02){
	LONG lCount = 0;
	m_spFrame->get_Count(&lCount);

	// gesti�n de error - si hay m�s de dos puntos avisar
	//if(lCount > 2) n_err_msg("ADVERTENCIA", "Hay m�s de dos puntos visibles.\nSe usar�n los 2 primeros encontrados (que quiz�s no sean informaci�n v�lida).");

	// VOLVER A PONER LCOUNT < 2 CUANDO TERMINE DE HACER ESTAS COSITAS 
	//for(int i = 0; i < lCount; i++){
	for(int i = 0; i < lCount; i++){         // por ahora s�lo tenemos dos cosas para trackear

	CComPtr<INPObject> spObject;
	m_spFrame->Item(i, &spObject);           
	CComVariant varX2, varY2, varArea2;			

	// values are double's so we need to use a variant to query for them
	spObject->get_X(&varX2);
	spObject->get_Y(&varY2);
	spObject->get_Area(&varArea2);

	///////////////////////////////////////// RANGO DE LOS PUNTOS

	//LONG lRank;
	//spObject->get_Rank(&lRank);
	//if (lRank == 1)
	//{
	//    // this is the currently tracked object... 
	//    CComVariant varX, varY, varArea;			
	//
	//                // values are double's so we need to use a variant to query for them
	//                spObject->get_X(&varX);
	//                spObject->get_Y(&varY);
	//                spObject->get_Area(&varArea);
	//
	//                // smooth the data
	//                m_spSmoothingDot->Update(varX, varY);
	//
	//                // grab the smoothed data
	//                m_spSmoothingDot->get_X(&varX);
	//                m_spSmoothingDot->get_Y(&varY);
	//
	//                // update the UI
	//                m_strDotPosition.Format(TEXT("%.3f, %.3f"), varX.dblVal, varY.dblVal);
	//                m_strDotArea.Format(TEXT("%.f"), varArea.dblVal);
	//            }

	//////////////////////////////////////////////////////////////////////////////////////

	//g_mano.addVertex(tVector3((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal)), i);         		
	g_mano.addVertex((float)(varX2.dblVal), (float)(varY2.dblVal), (float)(varArea2.dblVal), i);         					
	//n_err_msg("info", "gmano [%d]: %.2f %.2f %.2f", i, g_mano.vertices[i].x, g_mano.vertices[i].y, g_mano.vertices[i].z);
	}

	////////////////////////////////////////////////// FALTA EL PUTO QUICKSORT EN RESET ///////////////////////////////////

	////tVector3 aux; //=NULL_VEC3;
	//// EL PUNTO 0 ES EL DEDO : nota - HACER UN PUTO GETTER PORQUE VISUAL STUDIO ES QUISQUILLOSO A MORIR
	////if((float)(g_mano.vertices[0].x) < (float)(g_mano.vertices[1])){

	//if(g_mano.getVertexX(0) < g_mano.getVertexX(1)){
	//	c_dedo01 = g_mano.vertices[0]; 
	//	c_mano01 = g_mano.vertices[1];
	//}
	//// LOS PUNTOS EST�N AL REV�S Y EL 1 ES EL DEDO
	//else{
	//	c_dedo01 = g_mano.vertices[1]; 
	//	c_mano01 = g_mano.vertices[0];
	//}
	} */
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
		//SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
		SetWindowPos(&CWnd::wndTop, 200, 200, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
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

void CCameraDlg::OnStnClickedFrameid()
	{
	// TODO: Agregue aqu� su c�digo de controlador de notificaci�n de control
	}
