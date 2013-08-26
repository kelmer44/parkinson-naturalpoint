// Optitrack.h : main header file for the Optitrack application
//

/*
    Nombre: Cam Control. Aplicaci�n para experiencias m�dicas para sujetos con Alzheimer.
	Autor:  Almar Su�rez
	Fecha de inicio: -
	�ltima revisi�n: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la c�mara con vista cenital para los ejes xz 
	                          de las dos manos de forma simult�nea
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

