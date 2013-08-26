// Optitrack.h : main header file for the Optitrack application
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

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Optitracksampledlg.h"

// COptitrackApp:
// See Optitrack.cpp for the implementation of this class
//
class COptitrackApp : public CWinApp
{
public:
	COptitrackApp();
	~COptitrackApp();


// Overrides
public:
	virtual BOOL InitInstance();
    COptiTrackSampleDlg m_OptiTrackSampleDlg;


// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COptitrackApp theApp;
void DisplayError(HRESULT hr);

