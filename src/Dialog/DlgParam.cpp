#include "DlgParam.h"
#include "ALMRunConfig.h"
#include "MyTextCompleter.h"

#define PARAMHISTORY_FILE wxT("config/ParamHistory.txt")

BEGIN_EVENT_TABLE(DlgParam, wxDialog)
	EVT_BUTTON(wxID_OK, DlgParam::OnOKClick)
	EVT_SHOW(DlgParam::OnShow)
//	EVT_ACTIVATE(DlgParam::OnActivate)
END_EVENT_TABLE()

#ifndef _DISABLE_DND_
	#include <wx/dnd.h>
	class FileDropDnd : public wxFileDropTarget
	{
	public:
		FileDropDnd(DlgParam *pOwner) { m_pOwner = pOwner; }
 
		virtual bool OnDropFiles(wxCoord x, wxCoord y,
								 const wxArrayString& filenames);
	private:
		//�Ի����࣬��ԱTextCtrlPath�����ļ�·��
		DlgParam *m_pOwner;
 
	};
	bool FileDropDnd::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
	{
		if (filenames.Count() >= 1)
		{
			m_pOwner->SetValue(filenames[0]);
			return true;
		}
		return false;
	}
#endif


DlgParam::DlgParam( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(bSizer2 );

	if (g_config->get_u(ParamHistoryLimit))
	{
		Param_file = PARAMHISTORY_FILE;
		if (!wxFileExists(Param_file))
			tfile.Create(Param_file);

		if (tfile.Open(Param_file))
		{
			size_t i = tfile.GetLineCount();
			for(size_t j=0;j<i;++j)
			{
				m_array.Add(tfile[j].Trim());
			}
		}
	}
	comboBox = new wxComboBox(this, 1013,wxEmptyString, wxDefaultPosition,wxSize(this->GetSize().GetWidth()-80,this->GetSize().GetHeight()-5),m_array,wxCB_DROPDOWN );
	comboBox->AutoComplete(new MyTextCompleter(comboBox));
	comboBox->SetSelection(0);
	comboBox->Bind(wxEVT_KEY_DOWN,&DlgParam::OnKey,this);
	comboBox->SetToolTip("��ʾ: \n1.ѡ��һ����ʷ��¼��Alt+Delete����ɾ��.\n2.��[Alt+F]���Բ���һ���ļ�·��.\n3.��[Alt+D]���Բ���һ���ļ���·��");

	bSizer2->Add(comboBox, 0, wxALL, 5 );

	wxButton* ButtonOk = new wxButton(this,wxID_OK,"ȷ��(&O)",wxDefaultPosition,wxDefaultSize);
	wxButton* ButtonCancel = new wxButton(this,wxID_CANCEL,"ȡ��(&O)",wxDefaultPosition,wxDefaultSize);
	ButtonOk->SetDefault();
	bSizer2->Add(ButtonOk,0,wxALL,5);
	bSizer2->Add(ButtonCancel,0,wxALL,5);
	this->Layout();
	if (g_config->conf->ReadBool("/Window/DlgParamCenter",true))
		this->Centre( wxBOTH );

#ifndef _DISABLE_DND_
	this->SetDropTarget(new FileDropDnd(this));
#endif
//	this->SetEscapeId(wxID_CLOSE);
}

void DlgParam::OnKey(wxKeyEvent& e)
{
	int key =e.GetKeyCode();
	if (e.GetModifiers() == wxMOD_ALT)
	{
		wxString path;
		size_t pos = comboBox->GetInsertionPoint();
		if (key == WXK_DELETE)
		{
			int i = m_array.Index(comboBox->GetValue());
			if (i != wxNOT_FOUND)
			{
				if (wxMessageBox(wxString::Format("ȷ��Ҫɾ��������ʷ��¼:\n [ %s ]",comboBox->GetValue()),"��ʾ",wxYES_NO|wxICON_WARNING) == wxYES)
				{
					m_array.RemoveAt(i);
					tfile.RemoveLine(i);
					tfile.Write();
					comboBox->Set(m_array);
					return;
				}
			}
		}
		else if (key == 'F')
			path = wxFileSelector("��ѡ��һ���ļ�",wxEmptyString,wxEmptyString,wxEmptyString,wxFileSelectorDefaultWildcardStr,0,this);
		else if (key == 'D')
			path = wxDirSelector("��ѡ��һ��Ŀ¼",wxEmptyString,0,wxDefaultPosition,this);
		if (!path.empty())
		{
			wxString str = comboBox->GetValue();
			str.insert(pos,path);
			comboBox->ChangeValue(str);
			comboBox->SetInsertionPointEnd();
			return;
		}
	}
	e.Skip();
}

void DlgParam::OnOKClick(wxCommandEvent& e)
{
	wxString m_value = comboBox->GetValue();
	if (m_value.empty() || !tfile.IsOpened())
		return this->EndModal(wxID_OK);
	int i = comboBox->FindString(m_value);

	if (i > 0)
	{
		tfile.RemoveLine(i);
		if (tfile.GetLineCount() >= g_config->get_u(ParamHistoryLimit))
			tfile.RemoveLine(tfile.GetLineCount()-1);
	}

	if (i)
		tfile.InsertLine(comboBox->GetValue(),0);

	tfile.Write();
	this->EndModal(wxID_OK);
}

void DlgParam::OnShow(wxShowEvent& e)
{
	if (e.IsShown())
	{
	#ifdef __WXMSW__
		ActiveWindow(this->GetHWND());
	#endif
		comboBox->SetFocus();
		comboBox->SetFocusFromKbd();
	}
	e.Skip();
}

void DlgParam::SetValue(const wxString& str)
{
	comboBox->SetValue(str);
}

wxString DlgParam::getvalue()
{
	return comboBox->GetValue().Trim(true);
}

DlgParam::~DlgParam()
{
	__DEBUG_BEGIN("")
	if (tfile.IsOpened())
		tfile.Close();

#ifndef _DISABLE_DND_
	this->SetDropTarget(NULL);
#endif
	__DEBUG_END("")
}
