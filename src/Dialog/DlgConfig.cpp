/////////////////////////////////////////////////////////////////////////////
// Name:        DlgConfig.cpp
// Purpose:     
// Author:      test
// Modified by: 
// Created:     12/06/2013 09:14:07
// RCS-ID:      
// Copyright:   test
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DlgConfig.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "ALMRunConfig.h"
#ifdef _ALMRUN_CONFIG_H_
#include "DlgConfig.h"
////@begin XPM images
////@end XPM images


/*!
 * DlgConfig type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DlgConfig, wxDialog )


/*!
 * DlgConfig event table definition
 */

BEGIN_EVENT_TABLE( DlgConfig, wxDialog )

////@begin DlgConfig event table entries
////@end DlgConfig event table entries
//EVT_CHECKLISTBOX(ID_CHECKLISTBOX, OnCheck)
END_EVENT_TABLE()


/*!
 * DlgConfig constructors
 */

DlgConfig::DlgConfig()
{
    Init();
}

DlgConfig::DlgConfig( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * DlgConfig creator
 */

bool DlgConfig::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin DlgConfig creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end DlgConfig creation
    return true;
}


/*!
 * DlgConfig destructor
 */

DlgConfig::~DlgConfig()
{
	__DEBUG_BEGIN("")
////@begin DlgConfig destruction
	this->Disconnect(ID_CHECKLISTBOX,wxEVT_COMMAND_LISTBOX_SELECTED,wxObjectEventFunction(&DlgConfig::OnConfigCheck));
////@end DlgConfig destruction
	__DEBUG_END("")
}


/*!
 * Member initialisation
 */

void DlgConfig::Init()
{
////@begin DlgConfig member initialisation
    config = NULL;
////@end DlgConfig member initialisation
}

void DlgConfig::OnCheck(wxCommandEvent& e)
{
	e.Skip();
}

void DlgConfig::OnConfigCheck(wxCommandEvent& e)
{
	if (!g_config)
		return;
	config_tip->SetLabelText(g_config->config_tip[e.GetSelection()]);
	config_tip->SetToolTip(config_tip->GetLabelText());
}

void DlgConfig::OnMouseEvent(wxMouseEvent& e)
{
	e.Skip();
}
/*!
 * Control creation for DlgConfig
 */

void DlgConfig::CreateControls()
{    
////@begin DlgConfig content construction
    DlgConfig* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);


	wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemDialog1, wxID_ANY,"�����ȼ�");
	wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer1Static,wxVERTICAL);
	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("�����ȼ�:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_TOP|wxALL, 5);

    config_hotkey = new HotkeyCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_TEXTCTRL, wxEmptyString, wxDefaultPosition,wxSize(150, -1));
    itemBoxSizer3->Add(config_hotkey, 0, wxGROW|wxALL, 5);

    itemBoxSizer2->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, "��������");
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);
    wxArrayString configStrings;
	configStrings.Add("��ϵͳ�Զ�����");
	configStrings.Add("���ִ����ö�");
    configStrings.Add("�������ֿ�ݼ�");
    configStrings.Add("��ʾ����ͼ��");
    configStrings.Add("����ʾǰ10��");
	configStrings.Add("��ʾ������");
    configStrings.Add("��ʣһ��ʱ����ִ��");
	configStrings.Add("�������һ��ƥ��");
	configStrings.Add("������λ��ƥ��ؼ���");
    configStrings.Add("��������˳�� = 0, 1, ..., 8, 9");
    configStrings.Add("ǰ�ƥ������");
	configStrings.Add("��ʾ������ʾ��Ϣ");
	configStrings.Add("����WOW64�ļ�ϵͳ�ض���");
	configStrings.Add("��ӵ�<���͵�>�˵�");
	configStrings.Add("�����嵯��ʱ��������");
	configStrings.Add("���ո��ִ��");
	configStrings.Add("�����嵯��ʱ��ʾ�б�");
	configStrings.Add("�����γ����ȼ��ظ�ִ����һ������");
	configStrings.Add("���˫������");
	configStrings.Add("�����������ظ�");
	configStrings.Add("��ֹͬһ����������");
	configStrings.Add("��ݷ�ʽ��ȡĿ������");
	configStrings.Add("���洰��λ��");
    config = new wxCheckListBox(itemStaticBoxSizer3->GetStaticBox(), ID_CHECKLISTBOX, wxDefaultPosition, wxDefaultSize, configStrings, 0 );
	config_tip = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,wxEmptyString, wxDefaultPosition, wxSize(-1,50), wxALIGN_LEFT|wxST_NO_AUTORESIZE|wxNO_BORDER);
	config_tip->SetForegroundColour(wxColour(255, 0, 255));
    itemStaticBoxSizer3->Add(config, 0, wxGROW|wxALL, 5);
	itemStaticBoxSizer3->Add(config_tip, 0, wxGROW|wxALL, 5);
	wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("����(&O)"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton7->SetDefault();
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("ȡ��(&C)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);
	itemButton8->SetFocus();

    itemStdDialogButtonSizer6->Realize();

	this->Connect(ID_CHECKLISTBOX,wxEVT_COMMAND_LISTBOX_SELECTED,wxObjectEventFunction(&DlgConfig::OnConfigCheck));

////@end DlgConfig content construction
}


/*!
 * Should we show tooltips?
 */

bool DlgConfig::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap DlgConfig::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin DlgConfig bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end DlgConfig bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon DlgConfig::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin DlgConfig icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end DlgConfig icon retrieval
}
#endif//ifdef _ALMRUN_CONFIG_H_