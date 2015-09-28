#ifndef _MYTEXTCOMPLETER_H_
#define _MYTEXTCOMPLETER_H_

#include "MerryWx.h"
#include <wx/textcompleter.h>
/*
Ϊ��ʵ������λ��ƥ��,��Ҫ�޸�wxWidgetԴ��
src\msw\textentry.cpp
���� pAutoComplete2->SetOptions
��� ACO_NOPREFIXFILTERING ʵ������λ��ƥ��

��һ���Ķ�����ͬһ���ļ���
���� wxBIND_OR_CONNECT_HACK(m_win, wxEVT_AFTER_CHAR,
�޸�Ϊ wxBIND_OR_CONNECT_HACK(m_win, wxEVT_CHAR,
*/
class MyTextCompleter : public wxTextCompleter
{
public:
	MyTextCompleter(wxComboBox *entry = NULL);
    virtual bool Start(const wxString& prefix);
    virtual wxString GetNext();
private:
	void OnCharHook(wxKeyEvent& event);
	wxString m_last_str;
	wxComboBox *m_entry;
	wxWindow *m_win;
	wxArrayString m_completions;
	unsigned m_index;
	size_t m_last_pos;
};

#endif