// Register.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "Register.h"

#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

extern CPropertyApp theApp;

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegister)
	//}}AFX_DATA_INIT
}


void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegister)
	DDX_Control(pDX, IDC_REG_LABEL_INFO_1, m_LableInfo1);
	DDX_Control(pDX, IDC_REG_LABEL_INFO_3, m_LableInfo3);
	DDX_Control(pDX, IDC_REG_EDIT_QQ, m_EditQq);
	DDX_Control(pDX, IDC_REG_EDIT_INC, m_EditInc);
	DDX_Control(pDX, IDC_REG_TIME_BIRTHDAY, m_TimeBirthday);
	DDX_Control(pDX, IDC_REG_EDIT_ZIP, m_EditZip);
	DDX_Control(pDX, IDC_REG_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_REG_EDIT_EMAIL, m_EditEmail);
	DDX_Control(pDX, IDC_REG_EDIT_ADDRESS, m_EditAddress);
	DDX_Control(pDX, IDC_REG_EDIT_DUTY, m_EditDuty);
	DDX_Control(pDX, IDC_REG_COMBO_GENDER, m_ComboGender);
	DDX_Control(pDX, IDC_REG_COMBO_SALARY, m_ComboSalary);
	DDX_Control(pDX, IDC_REG_COMBO_METIER, m_ComboMetier);
	DDX_Control(pDX, IDC_REG_COMBO_DEGREE, m_ComboDegree);
	DDX_Control(pDX, IDC_REG_LABEL_WEB_STATION, m_WebStation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegister, CDialog)
	//{{AFX_MSG_MAP(CRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegister message handlers

BOOL CRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgResource();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CRegister::InitDlgResource()
{
	SetWindowText(GUI_REG_CAPTION);
	SetDlgItemText(IDC_REG_LABEL_INFO_1		, GUI_REG_LABEL_INFO_1);
	SetDlgItemText(IDC_REG_LABEL_INFO_2		, GUI_REG_LABEL_INFO_2);
	SetDlgItemText(IDC_REG_LABEL_INFO_3		, GUI_REG_LABEL_INFO_3);
	SetDlgItemText(IDC_REG_LABEL_EMAIL		, GUI_REG_LABEL_EMAIL);
	SetDlgItemText(IDC_REG_LABEL_NAME		, GUI_REG_LABEL_NAME);
	SetDlgItemText(IDC_REG_LABEL_GENDER		, GUI_REG_LABEL_GENDER);
	SetDlgItemText(IDC_REG_LABEL_INC		, GUI_REG_LABEL_INC);
	SetDlgItemText(IDC_REG_LABEL_QQ			, GUI_REG_LABEL_QQ);
	SetDlgItemText(IDC_REG_LABEL_BIRTHDAY	, GUI_REG_LABEL_BIRTHDAY);
	SetDlgItemText(IDC_REG_LABEL_DEGREE		, GUI_REG_LABEL_DEGREE);
	SetDlgItemText(IDC_REG_LABEL_METIER		, GUI_REG_LABEL_METIER);
	SetDlgItemText(IDC_REG_LABEL_DUTY		, GUI_REG_LABEL_DUTY);
	SetDlgItemText(IDC_REG_LABEL_ZIP		, GUI_REG_LABEL_ZIP);
	SetDlgItemText(IDC_REG_LABEL_SALARY		, GUI_REG_LABEL_SALARY);
	SetDlgItemText(IDC_REG_LABEL_ADDRESS	, GUI_REG_LABEL_ADDRESS);
	SetDlgItemText(IDOK						, GUI_BUTTON_OK);
	SetDlgItemText(IDCANCEL					, GUI_BUTTON_CANCEL);
	SetDlgItemText(IDC_REG_LABEL_WEB_STATION, ACL_HEADER_WEB_URL);

	InitCombo(&m_ComboGender, GUI_GENDER,  (sizeof(GUI_GENDER)  / sizeof(TCHAR*)), 0);
	InitCombo(&m_ComboDegree, GUI_DEGREE,  (sizeof(GUI_DEGREE)  / sizeof(TCHAR*)), 6);
	InitCombo(&m_ComboMetier, GUI_METIER,  (sizeof(GUI_METIER)  / sizeof(TCHAR*)), 1);
	InitCombo(&m_ComboSalary, GUI_SALARY,  (sizeof(GUI_SALARY)  / sizeof(TCHAR*)), 0);

	m_EditEmail		.SetLimitText(50);
	m_EditName		.SetLimitText(20);
	m_EditDuty		.SetLimitText(20);
	m_EditZip		.SetLimitText(6);
	m_EditAddress	.SetLimitText(50);
	m_EditQq		.SetLimitText(12);
	m_EditInc		.SetLimitText(50);

	m_TimeBirthday.SetTime(&CTime(0));
	m_LableInfo1.SetColor(RGB(0, 0, 255));
}

void CRegister::InitCombo(CComboBox *combo, TCHAR *string[], int count, int iSelected)
{
	int		iIndex;

	for(iIndex = 0; iIndex < count; iIndex ++)
		combo->InsertString(iIndex, string[iIndex]);

	combo->SetCurSel(iSelected);
}


void CRegister::OnOK() 
{
	CString tmpStr;
	m_EditEmail.GetWindowText(tmpStr);

	if(tmpStr.GetLength() == 0 || tmpStr.Find('@') == -1
		|| tmpStr.Left(1) == '@' || tmpStr.Right(1) == '@')
	{
		AfxMessageBox(GUI_REG_MESSAGE_MUST_INPUT_EMAIL);
		m_EditEmail.SetFocus();
		return;
	}

	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sEmail, tmpStr);

	CString sName, sQQ, sInc, sDuty, sZip, sAddress;

	m_EditName	.GetWindowText(sName);
	m_EditDuty	.GetWindowText(sDuty);
	m_EditZip	.GetWindowText(sZip);
	m_EditAddress.GetWindowText(sAddress);
	m_EditQq	.GetWindowText(sQQ);
	m_EditInc	.GetWindowText(sInc);

	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sName	, sName);
	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sDuty	, sDuty);
	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sZip	, sZip);
	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sAddress, sAddress);
	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sQQ 	, sQQ);
	_tcscpy(theApp.m_RegisterRequest.m_UserInfo.sInc	, sInc);

	theApp.m_RegisterRequest.m_UserInfo.bGender = m_ComboGender.GetCurSel();
	theApp.m_RegisterRequest.m_UserInfo.bDegree = m_ComboDegree.GetCurSel();
	theApp.m_RegisterRequest.m_UserInfo.bMetier = m_ComboMetier.GetCurSel();
	theApp.m_RegisterRequest.m_UserInfo.bSalary = m_ComboSalary.GetCurSel();

	m_TimeBirthday.GetTime(theApp.m_RegisterRequest.m_UserInfo.tBirthday);

	CDialog::OnOK();
}
