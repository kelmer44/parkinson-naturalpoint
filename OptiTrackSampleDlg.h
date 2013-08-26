// OptiTrackSampleDlg.h : header file
//

/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#if !defined(AFX_OPTITRACKSAMPLEDLG_H__CD3AE30F_26E7_4D4E_9B4F_25DDE62E4AB4__INCLUDED_)
#define AFX_OPTITRACKSAMPLEDLG_H__CD3AE30F_26E7_4D4E_9B4F_25DDE62E4AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef CList<DWORD *, DWORD *> CDwordPtrList;

/////////////////////////////////////////////////////////////////////////////
// COptiTrackSampleDlg dialog

class CCameraCollectionEvents :
    public IDispEventImpl<0, CCameraCollectionEvents, &DIID__INPCameraCollectionEvents, &LIBID_OptiTrack, 1, 0>
{
public:
    CCameraCollectionEvents() { ; }
    ~CCameraCollectionEvents() { ; }

public:
    // connection point notifications
    STDMETHOD_(void, OnDeviceRemoval)(INPCamera * pCamera) { HandleDeviceRemoval(pCamera); }
    STDMETHOD_(void, OnDeviceArrival)(INPCamera * pCamera) { HandleDeviceArrival(pCamera); }
    STDMETHOD_(void, OnFrameAvailableId)(LONG Group, LONG Id) { HandleFrameAvailableId(Group, Id); }
    virtual void HandleDeviceRemoval(INPCamera * pCamera) { ; }
    virtual void HandleDeviceArrival(INPCamera * pCamera) { ; }
    virtual void HandleFrameAvailableId(LONG Group, LONG Id) { ; }
    
public:
    BEGIN_SINK_MAP(CCameraCollectionEvents)
        SINK_ENTRY_EX(0, DIID__INPCameraCollectionEvents, 1, OnDeviceRemoval)
        SINK_ENTRY_EX(0, DIID__INPCameraCollectionEvents, 2, OnDeviceArrival)
        SINK_ENTRY_EX(0, DIID__INPCameraCollectionEvents, 3, OnFrameAvailableId)
    END_SINK_MAP()
};


class COptiTrackSampleDlg : 
    public CDialog,
    public CCameraCollectionEvents
{
// Construction
public:
	COptiTrackSampleDlg(CWnd* pParent = NULL);	// standard constructor
    ~COptiTrackSampleDlg();
// Dialog Data
	//{{AFX_DATA(COptiTrackSampleDlg)
	enum { IDD = IDD_CAMERA_LIST };
	CButton	m_buttonOpen;
	CListBox	m_listCameras;
	//}}AFX_DATA

	CComPtr<INPCameraCollection> m_spCollection;

    CDwordPtrList    m_listOpenCameras;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptiTrackSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

    void FillCameraList();

    void HandleDeviceRemoval(INPCamera * pCamera);
    void HandleDeviceArrival(INPCamera * pCamera);
    void HandleFrameAvailableId(LONG Group, LONG Id);

    afx_msg LRESULT OnCameraClosed(WPARAM wparam, LPARAM lparam);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COptiTrackSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOpen();
	afx_msg void OnSelchangeListCameras();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListCameras();
    afx_msg void OnClose( );
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonMulti();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTITRACKSAMPLEDLG_H__CD3AE30F_26E7_4D4E_9B4F_25DDE62E4AB4__INCLUDED_)
