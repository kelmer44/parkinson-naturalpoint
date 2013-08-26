/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#if !defined(AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_)
#define AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CameraDlg.h : header file
//

//#include "HiResTimer.h"

#define SMOOTH_VAL   15 //20
#define SMOOTH_MANO   5

/*
#define MIN_REAL_X       270 //322
#define MIN_REAL_Y       169 //214
#define MIN_REAL_Z        62 //80

#define MAX_REAL_X       322
#define MAX_REAL_Y       214
#define MAX_REAL_Z        80

#define MAX_REAL_DIFF_X   52
#define MAX_REAL_DIFF_Y   45
#define MAX_REAL_DIFF_Z   18
*/

///////////////////////////////////////////////////////// PUERTOS DE COMUNICACIÓN

#define PUERTO_COM_CAM01        3301
#define PUERTO_COM_CAM02        3302
#define PUERTO_COM_CAM03        3303
#define PUERTO_COM_CAM04INPUT   3304 //?
#define PUERTO_COM_CAM04OUTPUT  3305


////////////////////////////////////////////////////////// MANO DERECHA

#define MAX_REAL_X       270
#define MAX_REAL_Y        60
#define MAX_REAL_Z        60

#define MIN_REAL_X       312 //322
#define MIN_REAL_Y       230
#define MIN_REAL_Z       100 


#define MIN_OGRE_REAL_X    3 
#define MIN_OGRE_REAL_Y   -1.5 //-3.35
#define MIN_OGRE_REAL_Z   21.4 // 22.4

#define MAX_OGRE_REAL_X   37 
#define MAX_OGRE_REAL_Y    1.35
#define MAX_OGRE_REAL_Z   24.4 // 24.4

//#define MAX_OGRE_DIFF_X   34 
//#define MAX_OGRE_DIFF_Y    2
//#define MAX_OGRE_DIFF_Z    4.5  

////////////////////////////////////////////////////////// MANO IZQUIERDA


#define MAX_REAL_2X       270
#define MAX_REAL_2Y        60
#define MAX_REAL_2Z        60

#define MIN_REAL_2X       312 //322
#define MIN_REAL_2Y       230
#define MIN_REAL_2Z       100 


#define MIN_OGRE_REAL_2X  -40.1  
#define MIN_OGRE_REAL_2Y    2.85   //-3.35
#define MIN_OGRE_REAL_2Z  -23.0    // 22.4

#define MAX_OGRE_REAL_2X   -24.1
#define MAX_OGRE_REAL_2Y    -2.65
#define MAX_OGRE_REAL_2Z   -17.0

//#define MAX_OGRE_DIFF_2X   34 
//#define MAX_OGRE_DIFF_2Y    2
//#define MAX_OGRE_DIFF_2Z    4.5  


class CCameraEvents :
    public IDispEventImpl<0, CCameraEvents, &DIID__INPCameraEvents, &LIBID_OptiTrack, 1, 0>
{
public:
    CCameraEvents() { ; }
    ~CCameraEvents() { ; }

public:
    // connection point notifications
    STDMETHOD_(void, OnFrameAvailableIdCamera)(INPCamera * pCamera, LONG Group, LONG Id) { HandleFrameAvailableIdCamera(pCamera, Group, Id); }
    STDMETHOD_(void, OnFrameAvailable)(INPCamera * pCamera) { HandleFrameAvailable(pCamera); }
    STDMETHOD_(void, OnSwitchChange)(INPCamera * pCamera, LONG lNewSwitchState) { HandleSwitchChange(pCamera, lNewSwitchState); }

    virtual void HandleFrameAvailable(INPCamera * pCamera) { ; }
    virtual void HandleSwitchChange(INPCamera * pCamera, LONG lNewSwitchState) { ; }
    virtual void HandleFrameAvailableIdCamera(INPCamera * pCamera, LONG Group, LONG Id) { ; }
    
public:
    BEGIN_SINK_MAP(CCameraEvents)
        SINK_ENTRY_EX(0, DIID__INPCameraEvents, 1, OnFrameAvailable)
        SINK_ENTRY_EX(0, DIID__INPCameraEvents, 2, OnSwitchChange)
        SINK_ENTRY_EX(0, DIID__INPCameraEvents, 3, OnFrameAvailableIdCamera)
    END_SINK_MAP()

};

#include "CameraOptionsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg dialog

class CCameraDlg : 
    public CDialog,
    public CCameraEvents
{
// Construction
public:
	CCameraDlg(CWnd* pParent = NULL);   // standard constructor
    CCameraOptionsDlg *pCCameraOptionsDlg;
    BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL ); 
    ~CCameraDlg();
// Dialog Data
	//{{AFX_DATA(CCameraDlg)
	enum { IDD = IDD_CAMERA };
	CComboBox	m_comboUpdateRate;
	CButton	m_buttonStart;
	BOOL	m_fLedFour;
	BOOL	m_fLedOne;
	BOOL	m_fLedThree;
	BOOL	m_fLedTwo;
	CString	m_strDimensions;
	CString	m_strDotArea;
	CString	m_strDotPosition;
	CString	m_strFrameRate;
	CString	m_strModel;
	CString	m_strPitch;
	CString	m_strRevision;
	CString	m_strRoll;
	CString	m_strSerialNumber;
	CString	m_strSwitchState;
	CString	m_strX;
	CString	m_strY;
	CString	m_strYaw;
	CString	m_strZ;
    BOOL	m_fDrawVideo;
	
	//////////////////////////////////////  ATRIBUTOS AGREGADOS PARA GESTIONAR COSAS ADICIONALES ////////////////////////////
	int   cam_id;              // para saber cuál es la cámara que estoy usando
	float yaw_centro, pitch_centro,   roll_centro; // posiciones iniciales para centrar la cabeza
	float yaw_smooth[SMOOTH_VAL],     pitch_smooth[SMOOTH_VAL],   roll_smooth[SMOOTH_VAL];
	float manox_smooth[SMOOTH_MANO],  manoy_smooth[SMOOTH_MANO],  manoz_smooth[SMOOTH_MANO];
	float mano2x_smooth[SMOOTH_MANO], mano2y_smooth[SMOOTH_MANO], mano2z_smooth[SMOOTH_MANO];
	int   curr_smooth, curr_mano_smooth, curr_mano2_smooth;

	/////////////////////////////////////// MANOS VISTAS DE PERFIL ///////////////////////////////////////////////
	// (derecha)                                     |||||   (izquierda)                                        //
	//            * * * * * *D03                     |||||                          *DO3 * * * *                // 
	//      * * *D02          * * * * * * *M01       |||||       *M01 * * * * * *  * *       *DO2 * * *         //
	//   *D01                                        |||||                                           *DO1       //
	//                                               |||||                                                      //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	tVector3 p_mano01,  p_dedo01,  p_dedo02,  p_dedo03, p_muneca01; // posición mano01 (mano-derecha/marcador 1) posición dedo01 (mano-derecha/marcador1)
	tVector3 c_mano01,  c_dedo01,  c_dedo02,  c_dedo03; // "centros" (posiciones tras el reset)

	tVector3 p_mano21,  p_dedo21,  p_dedo22,  p_dedo23, p_muneca22; // posición mano01 (mano-derecha/marcador 1) posición dedo01 (mano-derecha/marcador1)
	tVector3 c_mano21,  c_dedo21,  c_dedo22,  c_dedo23; // "centros" (posiciones tras el reset)

    tVector3 xz_mano01, xz_mano02; // posiciones xz de las dos manos -vistos desde la cámara cenital-
 //   tVector3 xz_punta01, xz_punta02; // las puntas de los dedos en xz para calcular el puto movimiento lateral añadido
	//tVector3 xz_muneca01, xz_muneca02; // la jodida muneca para corregir el giro de los dedos

    vs_group3d_t g_mano;  // el grupo con los vértices de la mano (derecha).   compuesto por: p_mano01, p_dedo01, p_dedo02 y p_dedo03
    vs_group3d_t g_mano2; // el grupo con los vértices de la mano (izquierda). compuesto por: p_mano01, p_dedo01, p_dedo02 y p_dedo03
    vs_group3d_t g_manos; // el grupo con los vértices de las 2 manos juntas (izquierda y derecha). se compone de hasta 10 vértices. sólo se tienen en cuenta los de los extremos.

	// nuevo sistema de red. mola, eh? XD
    net_emisor_t   emisorCAM01, emisorCAM02, emisorCAM03, emisorCAM04TODO;
	net_receptor_t receptorCAM04deCAM01, receptorCAM04deCAM02, receptorCAM04deCAM03; // el último receptor estará en la aplicación de OGRE


	///////////// VARIABLES PARA PIERNAS /////////////////////////////////////////
	//							||
	//				PIERNAI01	||	PIERNAD01
	//					*		||      *
	//				   *		||		 *	
	//				  *			||		  *
	//				 *			||		   *
	//			 PIERNAI02		||		PIERNAD02
	//				*			||			*
	//				*			||			*
	//				*			||			*
	//				*			||			*
	//				*			||			*
	//			PIERNAI03		||		PIERNAD03
	//////////////////////////////////////////////////////////////////////////////
	
	tVector3 p_piernad01, p_piernad02, p_piernad03;
	vs_group3d_t g_pierna_derecha; // el grupo con los vértices de la pierna (derecha). 

	tVector3 p_piernai01, p_piernai02, p_piernai03;
	vs_group3d_t g_pierna_izquierda; // el grupo con los vértices de la pierna (derecha). 
	





	//float xm1, ym1, zm1;       // posición mano 1
	//float xd1m1, yd1m2, zd1m3; // posición dedo 1 mano 1. por qué carajo no estoy usando un puto vector?
	//float xposd1_centro, yposd1_centro, zposd1_centro; // posiciones iniciales del dedo1
    //float xposm1_centro, yposm1_centro, zposm1_centro;	
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //}}AFX_DATA

    BOOL m_fStarted;

    HWND m_hwndParent;

    int     m_nUpdateRate;
    DWORD   m_dwFrameCount;

    CComPtr<INPCamera> m_spCamera;
    CComPtr<INPCameraFrame> m_spFrame;

    CComPtr<INPSmoothing> m_spSmoothingDot;
    CComPtr<INPSmoothing> m_spSmoothingYaw;
    CComPtr<INPSmoothing> m_spSmoothingPitch;
    CComPtr<INPSmoothing> m_spSmoothingRoll;
    CComPtr<INPSmoothing> m_spSmoothingX;
    CComPtr<INPSmoothing> m_spSmoothingY;
    CComPtr<INPSmoothing> m_spSmoothingZ;

    CComPtr<INPVector> m_spVector;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy( );


	//}}AFX_VIRTUAL
    void FillCameraInfo();
    void HandleFrameAvailable(INPCamera * pCamera);
    void HandleFrameAvailableIdCamera(INPCamera * pCamera, LONG Group, LONG Id);
    void UpdateDotInfo(INPCameraFrame * pFrame);
    void UpdateVectorInfo(INPCameraFrame * pFrame);
    void UpdateSwitchState();

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CCameraDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnButtonStart();
	afx_msg void OnCheckLedOne();
	afx_msg void OnCheckLedTwo();
	afx_msg void OnCheckLedThree();
	afx_msg void OnCheckLedFour();
	afx_msg void OnDestroy();
	afx_msg void OnButtonOptions();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnButtonResetVector();
	afx_msg void OnSelchangeComboUpdateRate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    afx_msg LRESULT OnWMFrameAvailable(WPARAM wparam, LPARAM lparam);
    afx_msg void OnActivate(   UINT nState,   CWnd* pWndOther,   BOOL bMinimized );
    afx_msg void OnCheckDrawVideo();
    CString m_FrameId;
	afx_msg void OnStnClickedFrameid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_)
