/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#include "afxwin.h"
#if !defined(AFX_CAMERAOPTIONSDLG_H__6F3A46CA_EEA0_4030_BC14_D06CBAE3169F__INCLUDED_)
#define AFX_CAMERAOPTIONSDLG_H__6F3A46CA_EEA0_4030_BC14_D06CBAE3169F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraOptionsDlg.h : header file
//
typedef enum
{
    TRACKING_TYPE_OBJECT_MASS,
    TRACKING_TYPE_OBJECT_RATIO,
    TRACKING_TYPE_PROXIMITY,
    TRACKING_TYPE_STATIC_COUNT,
    TRACKING_TYPE_SCREEN_CENTER,
    TRACKING_TYPE_LAST_OBJECT

} TRACKING_TYPE;
/////////////////////////////////////////////////////////////////////////////
// CCameraOptionsDlg dialog

class CCameraOptionsDlg : public CDialog
{
// Construction
public:
	CCameraOptionsDlg(CWnd* pParent,  CComPtr<INPCamera> m_p);   // standard constructor
    ~CCameraOptionsDlg();
// Dialog Data
	//{{AFX_DATA(CCameraOptionsDlg)
	enum { IDD = IDD_CAMERA_OPTIONS };

    CSliderCtrl	m_sliderThreshold;
    CSliderCtrl	m_SliderFrameRate;
    CSliderCtrl	m_SliderExposure;
    CSliderCtrl	m_SliderIntensity;
    CSliderCtrl	m_SegMinLen;
    CSliderCtrl	m_SegMaxLen;
    CComboBox	m_comboTrackingType;
	CComboBox	m_comboObjectColorType;
	BOOL	m_fGreenOnTracking;
	CString	m_strDrawScale;
	CString	m_strWeight;
	CString	m_strMin;
	CString	m_strMax;
	CString	m_strIdeal;
	CString	m_strOutOfRange;
	//}}AFX_DATA
    int m_Id;
    UINT m_X;
    UINT m_X_END;
    UINT m_Y;
    UINT m_Y_END;
    CButton m_VideoType;
    UINT m_Decimation;
    UINT m_Decimation_Spatial;

    CComPtr<INPCamera> m_spCamera;

    COLORREF TrackedColor;      //camera value
    COLORREF m_TrackedColor;    //value on screen
	CString	m_strTrackedColor;  //value on screen

    COLORREF UntrackedColor;    //camera value
    COLORREF m_UntrackedColor;  //value on screen
	CString	m_strUntrackedColor;//value on screen

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    void UpdateTrackingTypeInfo();
    void UpdateSlider(CSliderCtrl * pSlider, bool Set);

// Implementation
protected:
    BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
    virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CCameraOptionsDlg)
    afx_msg void OnClose( );
    afx_msg void OnActivate(   UINT nState,   CWnd* pWndOther,   BOOL bMinimized );
	afx_msg void OnButtonTrackedColor();
	afx_msg void OnButtonUntrackedColor();
	afx_msg void OnSelchangeComboTrackingType();
  	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedApply();
    afx_msg void OnBnClickedCancel();
public:
    BOOL bVideoType;
    BOOL bVideoOverlay;
    int m_Threshold;
    int m_FrameRate;
    int m_Exposure;
    int m_Intensity;
    int m_MinLen;
    int m_MaxLen;
    long m_ObjectColorType;

    virtual void PostNcDestroy( );

	afx_msg void OnEnChangeId();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERAOPTIONSDLG_H__6F3A46CA_EEA0_4030_BC14_D06CBAE3169F__INCLUDED_)
