#if !defined(AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_)
#define AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CameraDlg.h : header file
//

//#include "HiResTimer.h"

#define SMOOTH_VAL  20
#define SMOOTH_MANO  7

#define MAX_REAL_X  1
#define MAX_REAL_Y  2
#define MAX_REAL_Z  3

#define MIN_REAL_X  4
#define MIN_REAL_Y  5 
#define MIN_REAL_Z  6


#define MAX_OGRE_REAL_X  1
#define MAX_OGRE_REAL_Y  2
#define MAX_OGRE_REAL_Z  3 

#define MIN_OGRE_REAL_X  4
#define MIN_OGRE_REAL_Y  5
#define MIN_OGRE_REAL_Z  6


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
	float yaw_centro, pitch_centro, roll_centro; // posiciones iniciales para centrar la cabeza
	float yaw_smooth[SMOOTH_VAL], pitch_smooth[SMOOTH_VAL], roll_smooth[SMOOTH_VAL];
	int   curr_smooth, curr_mano_smooth;
	float manox_smooth[SMOOTH_MANO], manoy_smooth[SMOOTH_MANO], manoz_smooth[SMOOTH_MANO];

	tVector3 p_mano01, p_dedo01, p_dedo02, p_dedo03; // posición mano01 (mano-derecha/marcador 1) posición dedo01 (mano-derecha/marcador1)
	tVector3 c_mano01, c_dedo01, c_dedo02, c_dedo03;

	
    vs_group3d_t g_mano; // el grupo con los vértices de la mano. compuesto por: p_mano01 y p_dedo01

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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERADLG_H__9B2BD893_6E69_4580_B303_129DE0417487__INCLUDED_)
