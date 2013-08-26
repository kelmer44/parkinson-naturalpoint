// CameraOptionsDlg.cpp : implementation file
//

/*
    Nombre: Cam Control. Aplicación para experiencias médicas para sujetos con Alzheimer.
	Autor:  Almar Suárez
	Fecha de inicio: -
	Última revisión: 11:17 19/10/2009
	Notas: [11:17 19/10/2009] agrego la cámara con vista cenital para los ejes xz 
	                          de las dos manos de forma simultánea
*/

#include "stdafx.h"
#include "OptiTrackApp.h"
#include "CameraOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraOptionsDlg dialog


CCameraOptionsDlg::CCameraOptionsDlg(CWnd* pParent /*=NULL*/, CComPtr<INPCamera> m_p)
	: CDialog(CCameraOptionsDlg::IDD, pParent)
    
    , m_Id(0)
    , m_X(1)
    , m_Decimation(0)
    , m_Decimation_Spatial(0)
    , bVideoType(FALSE)
    , bVideoOverlay(FALSE)
    , m_Threshold(0)
    , m_FrameRate(0)
    , m_Exposure(0)
    , m_Intensity(0)
    , m_MinLen(0)
    , m_MaxLen(0)
    , m_ObjectColorType(0)
{
    m_spCamera = m_p;
	//{{AFX_DATA_INIT(CCameraOptionsDlg)
	m_fGreenOnTracking = FALSE;
	m_strTrackedColor = _T("");
	m_strUntrackedColor = _T("");
    m_TrackedColor = 0;
    m_UntrackedColor = 0;
	m_strDrawScale = _T("");
	m_strWeight = _T("");
	m_strMin = _T("");
	m_strMax = _T("");
	m_strIdeal = _T("");
	m_strOutOfRange = _T("");
	//}}AFX_DATA_INIT
}


void CCameraOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCameraOptionsDlg)
    DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
    DDX_Control(pDX, IDC_SLIDER_FRAMERATE, m_SliderFrameRate);
    DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_SliderExposure);
    DDX_Control(pDX, IDC_SLIDER_INTENSITY, m_SliderIntensity);
    DDX_Control(pDX, IDC_SLIDER_MINLEN, m_SegMinLen);
    DDX_Control(pDX, IDC_SLIDER_MAXLEN, m_SegMaxLen);
    DDX_Control(pDX, IDC_COMBO_TRACKING_TYPE, m_comboTrackingType);
    DDX_Control(pDX, IDC_COMBO_OBJECT_COLOR_TYPE, m_comboObjectColorType);
    DDX_Check(pDX, IDC_CHECK_GREEN_ON_TRACKING, m_fGreenOnTracking);
    DDX_Text(pDX, IDC_STATIC_TRACKED_COLOR, m_strTrackedColor);
    DDX_Text(pDX, IDC_STATIC_UNTRACKED_COLOR, m_strUntrackedColor);
    DDX_Text(pDX, IDC_EDIT_DRAW_SCALE, m_strDrawScale);
    DDX_Text(pDX, IDC_EDIT_WEIGHT, m_strWeight);
    DDX_Text(pDX, IDC_EDIT_MIN, m_strMin);
    DDX_Text(pDX, IDC_EDIT_MAX, m_strMax);
    DDX_Text(pDX, IDC_EDIT_IDEAL, m_strIdeal);
    DDX_Text(pDX, IDC_EDIT_OUT_OF_RANGE, m_strOutOfRange);
    DDX_Text(pDX, IDC_ID, m_Id);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_X_END, m_X_END);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
    DDX_Text(pDX, IDC_EDIT_Y_END, m_Y_END);
    DDX_Control(pDX, IDC_GREY, m_VideoType);
    DDX_Text(pDX, IDC_DECIMATION, m_Decimation);
    DDX_Text(pDX, IDC_DECIMATION_SPATIAL, m_Decimation_Spatial);
    DDX_Check(pDX, IDC_GREY, bVideoType);
    DDX_Check(pDX, IDC_VIDEOOVERLAY, bVideoOverlay);
    DDX_Slider(pDX, IDC_SLIDER_THRESHOLD, m_Threshold);
    DDX_Slider(pDX, IDC_SLIDER_FRAMERATE, m_FrameRate);
    DDX_Slider(pDX, IDC_SLIDER_EXPOSURE, m_Exposure);
    DDX_Slider(pDX, IDC_SLIDER_INTENSITY, m_Intensity);
    DDX_Slider(pDX, IDC_SLIDER_MINLEN, m_MinLen);
    DDX_Slider(pDX, IDC_SLIDER_MAXLEN, m_MaxLen);
}


BEGIN_MESSAGE_MAP(CCameraOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraOptionsDlg)
	ON_BN_CLICKED(IDC_BUTTON_TRACKED_COLOR, OnButtonTrackedColor)
	ON_BN_CLICKED(IDC_BUTTON_UNTRACKED_COLOR, OnButtonUntrackedColor)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKING_TYPE, OnSelchangeComboTrackingType)
    ON_WM_CLOSE( )
    ON_WM_ACTIVATE( )
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDAPPLY,  &CCameraOptionsDlg::OnBnClickedApply)
    ON_BN_CLICKED(IDOK,     &CCameraOptionsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CCameraOptionsDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_ID, &CCameraOptionsDlg::OnEnChangeId)
END_MESSAGE_MAP()

// CCameraOptionsDlg message handlers

void CCameraOptionsDlg::OnButtonTrackedColor() 
{
    CColorDialog dlgColor(m_TrackedColor);
    if (dlgColor.DoModal() == IDOK)
    {
        m_TrackedColor = dlgColor.GetColor();
        m_strTrackedColor.Format(TEXT("%06lx"), (unsigned long) m_TrackedColor);
        GetDlgItem(IDC_STATIC_TRACKED_COLOR)->SetWindowText(m_strTrackedColor);
    }
}

void CCameraOptionsDlg::OnButtonUntrackedColor() 
{
    CColorDialog dlgColor(m_UntrackedColor);
    if (dlgColor.DoModal() == IDOK)
    {
        m_UntrackedColor = dlgColor.GetColor();
        m_strUntrackedColor.Format(TEXT("%06lx"), (unsigned long) m_UntrackedColor);
        GetDlgItem(IDC_STATIC_TRACKED_COLOR)->SetWindowText(m_strUntrackedColor);
    }
}

BOOL CCameraOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CComVariant var;
    // set green on tracking state
//    m_spCamera->GetOption(NP_OPTION_STATUS_GREEN_ON_TRACKING, &var);
//    m_fGreenOnTracking = VB2B(var.boolVal);
    m_fGreenOnTracking = FALSE;
    var = MAKE_BOOL(m_fGreenOnTracking);
    m_spCamera->SetOption(NP_OPTION_STATUS_GREEN_ON_TRACKING, var);

    // set the object colors
    m_spCamera->GetOption(NP_OPTION_TRACKED_OBJECT_COLOR, &var);
    m_TrackedColor = TrackedColor = var.lVal;
    m_strTrackedColor.Format(TEXT("%ld"), var.ulVal);

    m_spCamera->GetOption(NP_OPTION_UNTRACKED_OBJECTS_COLOR, &var);
    m_UntrackedColor = UntrackedColor = var.lVal;
    m_strUntrackedColor.Format(TEXT("%ld"), var.ulVal);

    // fill in object color type options
    m_comboObjectColorType.AddString(TEXT("ALL_SAME"));
    m_comboObjectColorType.AddString(TEXT("TRACKED_ONLY"));
    m_comboObjectColorType.AddString(TEXT("SHADE_BY_RANK"));
    m_comboObjectColorType.AddString(TEXT("VECTOR"));
    m_comboObjectColorType.AddString(TEXT("VECTOR_SHADE_BY_RANK"));

    // get the current setting
    m_spCamera->GetOption(NP_OPTION_OBJECT_COLOR_OPTION, &var);

    // select the correct item in the list
    m_comboObjectColorType.SetCurSel(var.ulVal);
    m_ObjectColorType = var.lVal;

    // fill in dot tracking type options
    m_comboTrackingType.AddString(TEXT("OBJECT_MASS"));
    m_comboTrackingType.AddString(TEXT("OBJECT_RATIO"));
    m_comboTrackingType.AddString(TEXT("PROXIMITY"));
    m_comboTrackingType.AddString(TEXT("STATIC_COUNT"));
    m_comboTrackingType.AddString(TEXT("SCREEN_CENTER"));
    m_comboTrackingType.AddString(TEXT("LAST_OBJECT"));

    // set the draw scale
    m_spCamera->GetOption(NP_OPTION_DRAW_SCALE, &var);

    m_strDrawScale.Format(TEXT("%.1f"), var.dblVal);

    HRESULT hr;
    hr = m_spCamera->GetOption(NP_OPTION_WINDOW_EXTENTS_X_DEFAULT, &var);
    GetDlgItem(IDC_EDIT_X)->EnableWindow(S_OK==hr);
    m_X = (S_OK!=hr) ? 0 : var.intVal;

    hr = m_spCamera->GetOption(NP_OPTION_WINDOW_EXTENTS_X_END_DEFAULT, &var);
    GetDlgItem(IDC_EDIT_X_END)->EnableWindow(S_OK==hr);
    m_X_END = (S_OK!=hr) ? 0 : var.intVal;

    hr = m_spCamera->GetOption(NP_OPTION_WINDOW_EXTENTS_Y_DEFAULT, &var);
    GetDlgItem(IDC_EDIT_Y)->EnableWindow(S_OK==hr);
    m_Y = (S_OK!=hr) ? 0 : var.intVal;

    hr = m_spCamera->GetOption(NP_OPTION_WINDOW_EXTENTS_Y_END_DEFAULT, &var);
    GetDlgItem(IDC_EDIT_Y_END)->EnableWindow(S_OK==hr);
    m_Y_END = (S_OK!=hr) ? 0 : var.intVal;

    hr = m_spCamera->GetOption(NP_OPTION_CAMERA_ID_DEFAULT, &var);
    GetDlgItem(IDC_ID)->EnableWindow(S_OK==hr);
    m_Id = (S_OK!=hr) ? 0 : var.intVal;

    hr = m_spCamera->GetOption(NP_OPTION_FRAME_DECIMATION_DEFAULT, &var);
    GetDlgItem(IDC_DECIMATION)->EnableWindow(S_OK==hr);
    m_Decimation = (S_OK!=hr) ? 0 : var.intVal;

    m_Decimation_Spatial = 0;

    hr = m_spCamera->GetOption(NP_OPTION_VIDEO_TYPE_DEFAULT, &var);
    GetDlgItem(IDC_GREY)->EnableWindow(S_OK==hr);
    bVideoType = (var.intVal==1);
    //    m_VideoType.SetCheck(var.intVal==1);

    // set the threshold
    m_sliderThreshold.SetRange(NP_THRESHOLD_MIN, NP_THRESHOLD_MAX);
    hr = m_spCamera->GetOption(NP_OPTION_THRESHOLD, &var);
    m_Threshold = (S_OK!=hr) ? 100 : var.ulVal;
    SetWindowLong(m_sliderThreshold.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_THRESHOLD);

    m_SliderFrameRate.SetRange(3, 100);
    hr = m_spCamera->GetOption(NP_OPTION_FRAME_RATE_DEFAULT, &var);
    GetDlgItem(IDC_SLIDER_FRAMERATE)->EnableWindow(S_OK==hr);
    m_FrameRate = (S_OK!=hr) ? 100 : var.intVal;
    SetWindowLong(m_SliderFrameRate.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_FRAMERATE);

    m_SliderExposure.SetRange(0, 399);
    hr = m_spCamera->GetOption(NP_OPTION_EXPOSURE_DEFAULT, &var);
    GetDlgItem(IDC_SLIDER_EXPOSURE)->EnableWindow(S_OK==hr);
    m_Exposure = (S_OK!=hr) ? 100 : var.intVal;
    SetWindowLong(m_SliderExposure.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_EXPOSURE);

    m_SliderIntensity.SetRange(1, 15);
    hr = m_spCamera->GetOption(NP_OPTION_INTENSITY_DEFAULT, &var);
    GetDlgItem(IDC_SLIDER_INTENSITY)->EnableWindow(S_OK==hr);
    m_Intensity = (S_OK!=hr) ? 100 : var.intVal;
    SetWindowLong(m_SliderIntensity.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_INTENSITY);

    m_SegMinLen.SetRange(0, 1024);
    hr = m_spCamera->GetOption(NP_OPTION_MINIMUM_SEGMENT_LENGTH_DEFAULT, &var);
    GetDlgItem(IDC_SLIDER_MINLEN)->EnableWindow(S_OK==hr);
    m_MinLen = (S_OK!=hr) ? 100 : var.intVal;
    SetWindowLong(m_SegMinLen.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_MINLEN);

    m_SegMaxLen.SetRange(0, 1024);
    hr = m_spCamera->GetOption(NP_OPTION_MAXIMUM_SEGMENT_LENGTH_DEFAULT, &var);
    GetDlgItem(IDC_SLIDER_MAXLEN)->EnableWindow(S_OK==hr);
    m_MaxLen = (S_OK!=hr) ? 100 : var.intVal;
    SetWindowLong(m_SegMaxLen.GetSafeHwnd(), GWL_USERDATA, IDC_STATIC_MAXLEN);

    // reset frame count appears in the same interface as previous.
    GetDlgItem(IDC_RESET_FRAME_COUNT)->EnableWindow(S_OK==hr);
    // set the current selection
    m_comboTrackingType.SetCurSel(TRACKING_TYPE_OBJECT_MASS);
    m_comboTrackingType.GetCurSel();
    
    UpdateTrackingTypeInfo();                       // fetch the data associated with the Tracking Type
    UpdateData(FALSE);                              // send it to the dialog
    return TRUE;
}

void CCameraOptionsDlg::OnSelchangeComboTrackingType() 
{
    UpdateTrackingTypeInfo();                       // reset values
    UpdateData(FALSE);
}


void CCameraOptionsDlg::UpdateTrackingTypeInfo()
{
    CComVariant varWeight, varMin, varMax, varIdeal, varOutOfRange;

    switch (m_comboTrackingType.GetCurSel())
    {
        case TRACKING_TYPE_OBJECT_MASS:
            m_spCamera->GetOption(NP_OPTION_OBJECT_MASS_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_OBJECT_MASS_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_OBJECT_MASS_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_OBJECT_MASS_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_OBJECT_MASS_OUT_OF_RANGE, &varOutOfRange);
            break;

        case TRACKING_TYPE_OBJECT_RATIO:
            m_spCamera->GetOption(NP_OPTION_OBJECT_RATIO_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_OBJECT_RATIO_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_OBJECT_RATIO_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_OBJECT_RATIO_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_OBJECT_RATIO_OUT_OF_RANGE, &varOutOfRange);
            break;
        
        case TRACKING_TYPE_PROXIMITY:
            m_spCamera->GetOption(NP_OPTION_PROXIMITY_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_PROXIMITY_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_PROXIMITY_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_PROXIMITY_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_PROXIMITY_OUT_OF_RANGE, &varOutOfRange);
            break;
        
        case TRACKING_TYPE_STATIC_COUNT:
            m_spCamera->GetOption(NP_OPTION_STATIC_COUNT_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_STATIC_COUNT_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_STATIC_COUNT_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_STATIC_COUNT_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_STATIC_COUNT_OUT_OF_RANGE, &varOutOfRange);
            break;
        
        case TRACKING_TYPE_SCREEN_CENTER:
            m_spCamera->GetOption(NP_OPTION_SCREEN_CENTER_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_SCREEN_CENTER_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_SCREEN_CENTER_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_SCREEN_CENTER_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_SCREEN_CENTER_OUT_OF_RANGE, &varOutOfRange);
            break;
        
        case TRACKING_TYPE_LAST_OBJECT:
            m_spCamera->GetOption(NP_OPTION_LAST_OBJECT_TRACKED_WEIGHT, &varWeight);
            m_spCamera->GetOption(NP_OPTION_LAST_OBJECT_MIN, &varMin);
            m_spCamera->GetOption(NP_OPTION_LAST_OBJECT_MAX, &varMax);
            m_spCamera->GetOption(NP_OPTION_LAST_OBJECT_IDEAL, &varIdeal);
            m_spCamera->GetOption(NP_OPTION_LAST_OBJECT_OUT_OF_RANGE, &varOutOfRange);
            break;
    }

    m_strWeight.Format(TEXT("%.3f"), varWeight.dblVal);
    m_strMin.Format(TEXT("%.3f"), varMin.dblVal);
    m_strMax.Format(TEXT("%.3f"), varMax.dblVal);
    m_strIdeal.Format(TEXT("%.3f"), varIdeal.dblVal);
    m_strOutOfRange.Format(TEXT("%.3f"), varOutOfRange.dblVal);

}

void CCameraOptionsDlg::UpdateSlider(CSliderCtrl * pSlider, bool Set) 
{
    CString str;
    UINT    uId;
    UINT    uPos;

    uPos = pSlider->GetPos();
    uId = GetWindowLong(pSlider->GetSafeHwnd(), GWL_USERDATA);
    if(uId == 0)
    {
        return;
    }

    // generic slider control, handle as normal
    str.Format(_T("%d"), uPos);
    GetDlgItem(uId)->SetWindowText(str);
    return;
}

void CCameraOptionsDlg::OnBnClickedOk()
{
    OnBnClickedApply();
    OnClose();
    //    OnOK();
}

// Apply looks at the values and sees if they have changed. If so, we update the camera
void CCameraOptionsDlg::OnBnClickedApply()
{
    //
    // Save the old values
    //
    UINT X                  =  m_X;
    UINT X_END              =  m_X_END;
    UINT Y                  =  m_Y;
    UINT Y_END              =  m_Y_END;
    BOOL VideoType          =  bVideoType;
    BOOL VideoOverlay       =  bVideoOverlay;
    UINT Decimation         =  m_Decimation;
    UINT Decimation_Spatial =  m_Decimation_Spatial;
    long sliderThreshold    =  m_Threshold;
    long SliderFrameRate    =  m_FrameRate;
    long SliderExposure     =  m_Exposure;
    long SliderIntensity    =  m_Intensity;
    long SegMinLen          =  m_MinLen;
    long SegMaxLen          =  m_MaxLen;
    BOOL fGreenOnTracking   = m_fGreenOnTracking;
    int Id                  = m_Id;
    long ObjectColorType    = m_ObjectColorType;
    USES_CONVERSION;
    double Weight = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strWeight));
    double Min = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strMin));
    double Max = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strMax));
    double Ideal = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strIdeal));
    double OutOfRange = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strOutOfRange));
    int iWeight, iMin, iMax, iIdeal, iOutOfRange;
    double Scale = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strDrawScale));
    //
    // Update the dialog data
    //
    UpdateData();
    CComVariant var;
    //
    // Now see if any values have changed
    //
    if(TrackedColor != m_TrackedColor)
    {
        TrackedColor = m_TrackedColor;
        var = (long) m_TrackedColor;
        m_spCamera->SetOption(NP_OPTION_TRACKED_OBJECT_COLOR, var);
    }
    if(UntrackedColor != m_UntrackedColor)
    {
        UntrackedColor = m_UntrackedColor;
        var = (long) m_UntrackedColor;
        m_spCamera->SetOption(NP_OPTION_UNTRACKED_OBJECTS_COLOR, var);
    }
    if(Id != m_Id)
    {
        var = m_Id;
        m_spCamera->SetOption(NP_OPTION_CAMERA_ID, var);
    }
    if(X != m_X)
    {
        var = m_X;
        m_spCamera->SetOption(NP_OPTION_WINDOW_EXTENTS_X, var);
    }
    if(X_END != m_X_END)
    {
        var = m_X_END;
        m_spCamera->SetOption(NP_OPTION_WINDOW_EXTENTS_X_END, var);
    }
    if(Y != m_Y)
    {
        var = m_Y;
        m_spCamera->SetOption(NP_OPTION_WINDOW_EXTENTS_Y, var);
    }
    if(Y_END != m_Y_END)
    {
        var = m_Y_END;
        m_spCamera->SetOption(NP_OPTION_WINDOW_EXTENTS_Y_END, var);
    }
    if(VideoType != bVideoType)
    {
        var = (int)bVideoType;
        m_spCamera->SetOption(NP_OPTION_VIDEO_TYPE, var);
    }
    if(VideoOverlay != bVideoOverlay)
    {
        if (bVideoOverlay)
        {
            // enable video overlay flags
            var = (int)(NP_TEXT_OVERLAY_HEADER | NP_TEXT_OVERLAY_OBJECT);
        }
        else
        {
            // clear video overlay flags
            var = (int)0;
        }
        m_spCamera->SetOption(NP_OPTION_TEXT_OVERLAY_OPTION, var);
    }
    if(Decimation != m_Decimation)
    {
        var = m_Decimation;
        m_spCamera->SetOption(NP_OPTION_FRAME_DECIMATION, var);
    }
    if(Decimation_Spatial != m_Decimation_Spatial)
    {
        var = m_Decimation_Spatial;
        m_spCamera->SetOption(NP_OPTION_GRAYSCALE_DECIMATION, var);
    }
    if( sliderThreshold !=  m_Threshold)
    {
        var = (long) m_Threshold;
        m_spCamera->SetOption(NP_OPTION_THRESHOLD, var);
    }
    if( SliderExposure !=  m_Exposure)
    {
        var = (long) m_Exposure;
        m_spCamera->SetOption(NP_OPTION_EXPOSURE, var);
    }
    if( SliderFrameRate !=  m_FrameRate)
    {
        var = (long) m_FrameRate;
        m_spCamera->SetOption(NP_OPTION_FRAME_RATE, var);
    }
    if( SliderIntensity !=  m_Intensity)
    {
        var = (long) m_Intensity;
        m_spCamera->SetOption(NP_OPTION_INTENSITY, var);
    }
    if( SegMinLen !=  m_MinLen)
    {
        var = (long) m_MinLen;
        m_spCamera->SetOption(NP_OPTION_MINIMUM_SEGMENT_LENGTH, var);
    }
    if( SegMaxLen !=  m_MaxLen)
    {
        var = (long) m_MaxLen;
        m_spCamera->SetOption(NP_OPTION_MAXIMUM_SEGMENT_LENGTH, var);
    }
    if( fGreenOnTracking !=  m_fGreenOnTracking)
    {
        var = MAKE_BOOL(m_fGreenOnTracking);
        m_spCamera->SetOption(NP_OPTION_STATUS_GREEN_ON_TRACKING, var);
    }
    switch (m_comboTrackingType.GetCurSel())
    {
    case TRACKING_TYPE_OBJECT_MASS:
        iWeight = NP_OPTION_OBJECT_MASS_WEIGHT;
        iMin = NP_OPTION_OBJECT_MASS_MIN;
        iMax = NP_OPTION_OBJECT_MASS_MAX;
        iIdeal = NP_OPTION_OBJECT_MASS_IDEAL;
        iOutOfRange = NP_OPTION_OBJECT_MASS_OUT_OF_RANGE;
        break;

    case TRACKING_TYPE_OBJECT_RATIO:
        iWeight = NP_OPTION_OBJECT_RATIO_WEIGHT;
        iMin = NP_OPTION_OBJECT_RATIO_MIN;
        iMax = NP_OPTION_OBJECT_RATIO_MAX;
        iIdeal = NP_OPTION_OBJECT_RATIO_IDEAL;
        iOutOfRange = NP_OPTION_OBJECT_RATIO_OUT_OF_RANGE;
        break;
    
    case TRACKING_TYPE_PROXIMITY:
        iWeight = NP_OPTION_PROXIMITY_WEIGHT;
        iMin = NP_OPTION_PROXIMITY_MIN;
        iMax = NP_OPTION_PROXIMITY_MAX;
        iIdeal = NP_OPTION_PROXIMITY_IDEAL;
        iOutOfRange = NP_OPTION_PROXIMITY_OUT_OF_RANGE;
        break;
    
    case TRACKING_TYPE_STATIC_COUNT:
        iWeight = NP_OPTION_STATIC_COUNT_WEIGHT;
        iMin = NP_OPTION_STATIC_COUNT_MIN;
        iMax = NP_OPTION_STATIC_COUNT_MAX;
        iIdeal = NP_OPTION_STATIC_COUNT_IDEAL;
        iOutOfRange = NP_OPTION_STATIC_COUNT_OUT_OF_RANGE;
        break;
    
    case TRACKING_TYPE_SCREEN_CENTER:
        iWeight = NP_OPTION_SCREEN_CENTER_WEIGHT;
        iMin = NP_OPTION_SCREEN_CENTER_MIN;
        iMax = NP_OPTION_SCREEN_CENTER_MAX;
        iIdeal = NP_OPTION_SCREEN_CENTER_IDEAL;
        iOutOfRange = NP_OPTION_SCREEN_CENTER_OUT_OF_RANGE;
        break;
    
    case TRACKING_TYPE_LAST_OBJECT:
        iWeight = NP_OPTION_LAST_OBJECT_TRACKED_WEIGHT;
        iMin = NP_OPTION_LAST_OBJECT_MIN;
        iMax = NP_OPTION_LAST_OBJECT_MAX;
        iIdeal = NP_OPTION_LAST_OBJECT_IDEAL;
        iOutOfRange = NP_OPTION_LAST_OBJECT_OUT_OF_RANGE;
        break;
    }
    var = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strWeight));
    if(Weight !=  var.dblVal)
    {
        m_spCamera->SetOption(iWeight, var);
    }
    var = (double) _ttoi(m_strMin);
    if(Min != var.dblVal)
    {
        m_spCamera->SetOption(iMin, var);
    }
    var = (double) _ttoi(m_strMax);
    if(Max != var.dblVal)
    {
        m_spCamera->SetOption(iMax, var);
    }
    var = (double) _ttoi(m_strIdeal);
    if(Ideal != var.dblVal)
    {
        m_spCamera->SetOption(iIdeal, var);
    }
    var = (double) _ttoi(m_strOutOfRange);
    if(OutOfRange != var.dblVal)
    {
        m_spCamera->SetOption(iOutOfRange, var);
    }
    var = (long) m_comboObjectColorType.GetCurSel();
    if(ObjectColorType != var.lVal)
    {
        m_spCamera->SetOption(NP_OPTION_OBJECT_COLOR_OPTION, var);
    }
    var = (double) atof(T2A((LPTSTR) (LPCTSTR) m_strDrawScale));
    if(Scale != var.dblVal)
    {
        m_spCamera->SetOption(NP_OPTION_DRAW_SCALE, var);
    }
}

void CCameraOptionsDlg::OnBnClickedCancel()
{
    UpdateTrackingTypeInfo();                       // reset values
    UpdateData(FALSE);
}

void CCameraOptionsDlg::OnClose( )
{
    ShowWindow(SW_HIDE);
}

void CCameraOptionsDlg::OnActivate(   UINT nState,   CWnd* pWndOther,   BOOL bMinimized )
{
    CDialog::OnActivate(nState,pWndOther,bMinimized );
    if( nState == WA_INACTIVE)
    {
        //SetWindowPos(pWndOther, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
		SetWindowPos(pWndOther, 200, 200, 0, 0,SWP_NOMOVE | SWP_NOSIZE );
    }
}

void CCameraOptionsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    CSliderCtrl * pSlider = (CSliderCtrl *) pScrollBar;

    switch (nSBCode)
    {
        case TB_BOTTOM:
        case TB_ENDTRACK:
        case TB_LINEDOWN:
        case TB_LINEUP:
        case TB_PAGEDOWN:
        case TB_PAGEUP:
        case TB_THUMBPOSITION:
        case TB_THUMBTRACK:
            UpdateSlider(pSlider, true);
            break;

        default:
            break;
    }
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CCameraOptionsDlg::UpdateData(BOOL bSaveAndValidate)
{
    BOOL Result = CDialog::UpdateData(bSaveAndValidate);
    if(bSaveAndValidate == FALSE)
    {
        m_strTrackedColor.Format(TEXT("%06lx"), (unsigned long) m_TrackedColor);
        GetDlgItem(IDC_STATIC_TRACKED_COLOR)->SetWindowText(m_strTrackedColor);
        m_strUntrackedColor.Format(TEXT("%06lx"), (unsigned long) m_UntrackedColor);
        GetDlgItem(IDC_STATIC_UNTRACKED_COLOR)->SetWindowText(m_strUntrackedColor);
        UpdateSlider(&m_sliderThreshold, false);
        UpdateSlider(&m_SliderFrameRate, false);
        UpdateSlider(&m_SliderExposure, false);
        UpdateSlider(&m_SliderIntensity, false);
        UpdateSlider(&m_SegMinLen, false);
        UpdateSlider(&m_SegMaxLen, false);
    }
    return Result;
}

CCameraOptionsDlg::~CCameraOptionsDlg()
{
}
 
void CCameraOptionsDlg::PostNcDestroy( )
{
    delete this;
}

void CCameraOptionsDlg::OnEnChangeId()
{
	// TODO:  Si éste es un control RICHEDIT, el control no
	// enviará esta notificación a menos que se invalide la función CDialog::OnInitDialog()
	// y se llame a CRichEditCtrl().SetEventMask()
	// con el marcador ENM_CHANGE ORed en la máscara.

	// TODO:  Agregue aquí el controlador de notificación de controles
}
