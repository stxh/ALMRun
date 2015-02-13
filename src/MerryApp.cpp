#include "MerryApp.h"
#include <wx/stdpaths.h>

IMPLEMENT_APP(MerryApp)

bool MerryApp::OnInit()
{
	const wxString name = wxString::Format("ALMRun-%s", wxGetUserId().c_str());
    m_checker = new wxSingleInstanceChecker(name);
    if (m_checker->IsAnotherRunning())//�����Ѿ���������..
    {
        stClient* client = new stClient;

        // Create the connection
        wxConnectionBase* connection =
					//	client->MakeConnection( IPC_HOST,"4242","IPC TEST");
                     client->MakeConnection( IPC_HOST,IPC_SERVICE,IPC_TOPIC);

        if (connection)
        {
            // Ask the other instance to open a file or raise itself
			Execute_IPC_CMD(connection);
        }
        else
        {
            wxMessageBox(wxT("�����Ѿ�����,���ǽ���ͨ��ʧ��!"),
                wxT("ALMRun"), wxICON_INFORMATION|wxOK);
        }
		wxDELETE(client);
		wxDELETE(m_checker);
        return false;
    }
	    // Create a new server
    m_server = new stServer;
    if (!m_server->Create(IPC_SERVICE))
    {
        wxMessageBox("�����߼�����ͨ��ʧ��,�޷�ʵ�ֵ�һʵ�����̺��Ҽ����͵��ȹ���.");
		return false;
    }
	if (!wxApp::OnInit())
		return false;
	#if _DEBUG_LOG
        m_pLogFile = fopen( "log.txt", "w+" );
		wxLogStderr *log = new wxLogStderr(m_pLogFile);
        delete  wxLog::SetActiveTarget(log);

        wxLog::SetTimestamp(wxT("%Y-%m-%d %H:%M:%S"));
		
		wxLog::SetLogLevel(::wxLOG_Max);
		wxLogMessage("ALMRun_INIT");
	#endif
	#ifdef __WXMSW__
	wxStandardPaths std = wxStandardPaths::Get(); //<wx/stdpaths.h>
	wxFileName fname = wxFileName(std.GetExecutablePath());
	wxString volume;
	wxString pathTmp = fname.GetPathWithSep(); //<wx/filename.h>
	::wxSetEnv(wxT("ALMRUN_HOME"),pathTmp.c_str());
	::wxSetEnv(wxT("ALMRUN_ROOT"),pathTmp.c_str());
	::wxSetEnv(wxT("Desktop"),std.MSWGetShellDir(0x10));//CSIDL_DESKTOP 
	::wxSetEnv(wxT("Programs"),std.MSWGetShellDir(2));//CSIDL_PROGRAMS 
	::wxSetEnv(wxT("CommonDesktop"),std.MSWGetShellDir(0x19));//CSIDL_COMMON_DESKTOPDIRECTORY 
	::wxSetEnv(wxT("CommonPrograms"),std.MSWGetShellDir(0x17));//COMMON_PROGRAMS
	wxFileName::SplitVolume(pathTmp,&volume,NULL);
	if (!volume.empty())
	{
		volume.Append(':');
		::wxSetEnv(wxT("ALMRUN_DRIVE"),volume.c_str());
	}
	::wxSetWorkingDirectory(pathTmp);
	::wxSetEnv(wxT("ALMRUN_SYS"),IsX64()?"x64":"x86");
	//pathTmp.Clear();
	//volume.Clear();
	#endif
	m_frame = NULL;
	this->NewFrame();
	assert(m_frame);
	this->Connect(wxEVT_ACTIVATE_APP,wxObjectEventFunction(&MerryApp::EvtActive));
	return true;
}

void MerryApp::Execute_IPC_CMD(wxConnectionBase* conn)
{
	if (this->argc == 1)
		conn->Execute("SHOW");
	else
	{
		for(int i = 1;i < this->argc; ++i)
			conn->Execute(this->argv[i]);
	}
}

int MerryApp::OnExit()
{
	__DEBUG_BEGIN("")
	this->Disconnect(wxEVT_ACTIVATE_APP);
	//�Ӵ��ڻ��Զ���գ����Բ���Ҫ�����䣬�����п��ܻ����
	//if (m_frame)
	//	wxDELETE(m_frame);
	m_frame = NULL;
	wxDELETE(m_checker);
	wxDELETE(m_server);
	__DEBUG_END("")
	#if _DEBUG_LOG
	if (m_pLogFile)
	{
		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}
	#endif
	return 0;
}

void MerryApp::EvtActive(wxActivateEvent &e)
{
	if (!m_frame)
		return;
	bool Changed = false;
	if (g_config)
		Changed = g_config->Changed();
	if (!e.GetActive())
		m_frame->Hide();
#ifdef _ALMRUN_CONFIG_H_
	else if (Changed)
		m_frame->NewConfig();
#endif//ifdef _ALMRUN_CONFIG_H_
//	e.Skip();
}

void MerryApp::stServerDisconnect()
{
	m_server->Disconnect();
}

void MerryApp::NewFrame()
{
	if (m_frame)
	{
		bool ok = m_frame->Close();
		assert(ok);
	}
	
	m_frame = new MerryFrame();
	m_frame->OnInit();
}

MerryFrame& MerryApp::GetFrame()
{
	assert(m_frame);
	return *m_frame;
}
