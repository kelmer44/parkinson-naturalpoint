// MainFrm.h : interface of the CMainFrame class
//


/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#pragma once

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CChildView    m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
};


