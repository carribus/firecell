/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <windows.h>
#include <process.h>
#include <conio.h>
#include "w32service.h"

///////////////////////////////////////////////////////////////////////

int CW32Service::m_nStatusCount = 0;

///////////////////////////////////////////////////////////////////////

int CW32Service::m_nRefCnt = 0;

///////////////////////////////////////////////////////////////////////

bool CW32Service::m_bRunAsApp = false;

///////////////////////////////////////////////////////////////////////

CW32Service::stSvcStatus** CW32Service::m_ppStatus = NULL;

///////////////////////////////////////////////////////////////////////

CW32Service::CW32Service(const char* lpMachineName, const char* lpDataBaseName, DWORD dwDesiredAccess)
: m_lpszServiceDesc(NULL)
, m_serviceHandle(NULL)
, m_lpszServiceName(NULL)
, m_hThread(0)
, m_hThrdEvent(0)
, m_pLogic(NULL)
{
	memset(m_arrDispatchTable, 0, sizeof(SERVICE_TABLE_ENTRY)*2);
	m_scmHandle = OpenSCManager( lpMachineName, lpDataBaseName, dwDesiredAccess );
	m_nRefCnt++;
}

///////////////////////////////////////////////////////////////////////

CW32Service::~CW32Service()
{
	if ( m_lpszServiceName )
		delete [] m_lpszServiceName;
	if ( m_lpszServiceDesc )
		delete [] m_lpszServiceDesc;
	if ( m_scmHandle )
		CloseServiceHandle( m_scmHandle );
	if ( m_serviceHandle )
		CloseServiceHandle( m_serviceHandle );
	m_nRefCnt--;
	if ( !m_nRefCnt )
		CleanupMemory();
}

///////////////////////////////////////////////////////////////////////

DWORD CW32Service::Initialise(void* pParam)
{
  CW32Service* pThis = NULL;
  if ( !m_bRunAsApp )
  {
		CW32Service::stSvcStatus*	pStatus = (CW32Service::stSvcStatus*) pParam;
		pThis = (CW32Service*) pStatus->pServiceObj;

    if ( !pThis->m_pLogic )
      return 0;

    // create the event
    pThis->m_hThrdEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    unsigned int nID;
    if ( !(pThis->m_hThread = (HANDLE)_beginthreadex(NULL, 0, CW32Service::ThreadFunction, pThis, 0, &nID)) )
    {
      return 0;
    }
  }
  else
  {
    pThis = (CW32Service*) pParam;

    if ( !pThis->m_pLogic )
      return 0;

    pThis->m_pLogic->Start();
    while ( getch() != 'q' )
    {
      printf(".");
    }
    pThis->m_pLogic->Stop();
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

void CW32Service::CleanupMemory()
{
	int					i;

	for ( i = 0; i < m_nStatusCount; i++ )
	{
		stSvcStatus* pStatus = m_ppStatus[i];
		delete pStatus;
	}
	delete [] m_ppStatus;
	m_nStatusCount = 0;
}

///////////////////////////////////////////////////////////////////////

void CW32Service::SetServiceName(const char* lpszName)
{
	int			nLen = 0;

	if ( m_lpszServiceName )
		delete [] m_lpszServiceName;
	if ( lpszName )
	{
		nLen = lstrlen(lpszName);
		m_lpszServiceName = new TCHAR[nLen+1];
		ZeroMemory(m_lpszServiceName, (nLen+1)*sizeof(TCHAR));
		lstrcpyn(m_lpszServiceName, lpszName, nLen+1);
	}
	else
		m_lpszServiceName = NULL;
}

///////////////////////////////////////////////////////////////////////

void CW32Service::SetServiceDescription(const char* lpszDesc)
{
	SERVICE_DESCRIPTION			desc;

	// store the name
	int			nLen = 0;

	if ( m_lpszServiceDesc && m_lpszServiceDesc != lpszDesc )
		delete [] m_lpszServiceDesc;
	if ( lpszDesc && lpszDesc != m_lpszServiceDesc )
	{
		nLen = lstrlen(lpszDesc);
		m_lpszServiceDesc = new TCHAR[nLen+1];
		ZeroMemory(m_lpszServiceDesc, (nLen+1)*sizeof(TCHAR));
		lstrcpyn(m_lpszServiceDesc, lpszDesc, nLen+1);
	}
	else if ( !lpszDesc )
		m_lpszServiceDesc = NULL;

	// register it
	if ( !m_serviceHandle )
		return;
	desc.lpDescription = (TCHAR*)lpszDesc;
	::ChangeServiceConfig2(m_serviceHandle, SERVICE_CONFIG_DESCRIPTION, &desc);
}

///////////////////////////////////////////////////////////////////////

void CW32Service::SetInitProc(LP_SERVICE_INIT_PROC lpInitProc)
{
	stSvcStatus*				pStatus = FindServiceStatus(m_lpszServiceName);

	if ( !pStatus )
		return;

	pStatus->lpInitProc = lpInitProc;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::OpenService(DWORD dwDesiredAccess)
{
	stSvcStatus*				pStatus = NULL;

	if ( !m_scmHandle || !m_lpszServiceName )
		return FALSE;
	if ( !(m_serviceHandle = ::OpenService( m_scmHandle, m_lpszServiceName, dwDesiredAccess )) )
		return FALSE;
	pStatus = AddService( m_lpszServiceName );

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::CreateService(const char* lpDisplayName, const char* lpBinaryPathName, DWORD dwDesiredAccess, DWORD dwServiceType, DWORD dwStartType, DWORD dwErrorControl, const char* lpLoadOrderGroup, DWORD* lpdwTagID, const char* lpDependencies, const char* lpServiceStartName, const char* lpPassword)
{
	if ( !m_scmHandle || !m_lpszServiceName )
		return FALSE;
	if ( !(m_serviceHandle = ::CreateService( m_scmHandle, m_lpszServiceName, lpDisplayName, dwDesiredAccess, dwServiceType, dwStartType, dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagID, lpDependencies, lpServiceStartName, lpPassword )) )
		return false;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::DeleteService()
{
	if ( !m_serviceHandle )
		return FALSE;
	if ( !::DeleteService(m_serviceHandle) )
		return FALSE;
	CloseServiceHandle(m_serviceHandle);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::StartService(DWORD dwNumArgs, const char** lpServiceArgs)
{
	if ( !m_serviceHandle )
		return FALSE;

  return ( ::StartService( m_serviceHandle, dwNumArgs, lpServiceArgs ) ? true : false );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::StopService()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;

	::ControlService( m_serviceHandle, SERVICE_CONTROL_STOP, &pStatus->status );
	return ( pStatus->status.dwCurrentState == SERVICE_STOPPED );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::PauseService()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;

	::ControlService( m_serviceHandle, SERVICE_CONTROL_PAUSE, &pStatus->status );
	return ( pStatus->status.dwCurrentState == SERVICE_PAUSED );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::ResumeService()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;

	::ControlService( m_serviceHandle, SERVICE_CONTROL_CONTINUE, &pStatus->status );
	return ( pStatus->status.dwCurrentState == SERVICE_RUNNING );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::ISRV_AttachLogic(IServiceLogic* pLogic)
{
  if ( !pLogic )
    return false;

  SetServiceName( pLogic->GetName() );
  m_pLogic = pLogic;

  return true;
}

///////////////////////////////////////////////////////////////////////

void CW32Service::ISRV_RunAsApp(bool bAsApp)
{
  m_bRunAsApp = bAsApp;
}

///////////////////////////////////////////////////////////////////////

int CW32Service::ISRV_Run(void* pData)
{
  if ( !m_pLogic )
    return -1;

  // indicate whether the logic module can output to the console
  m_pLogic->HasConsole( m_bRunAsApp );

  if ( m_bRunAsApp )
  {
    Initialise(this);
  }
  else
  {
    if ( !OpenService() )
      return 0;

    SetInitProc( Initialise );
    ExecuteService();
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

int CW32Service::ISRV_Stop()
{
  if ( !m_pLogic )
    return -1;

  SetEvent(m_hThrdEvent);
  WaitForSingleObject(m_hThread, INFINITE);

  return 0;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::InstallService(CW32Service* pService, const char* lpszParams)
{
	if ( !pService )
		return FALSE;

	TCHAR	buffer[4096], filename[1024];
	TCHAR*	pPos = NULL, *pPos2 = NULL;

	ZeroMemory(buffer, sizeof(buffer));
	// get the binary's directory
	GetCurrentDirectory(sizeof(buffer), buffer);
	// get the module's filename
	GetModuleFileName( 0, filename, sizeof(filename));
	// get the end of the buffer (binary folder)
	pPos = buffer + lstrlen(buffer);
	*pPos++ = '\\';
	// find the beginning of the filename (in case the file was passed with its full path
	pPos2 = filename + lstrlen(filename)-1;
	while ( *pPos2 )
	{
		if ( *pPos2 == '\\' )
			break;
		pPos2--;
	}
	// concatenate the folder and binary name
	CopyMemory(pPos, pPos2+1, lstrlen(filename)*sizeof(TCHAR));
	strcat(buffer, " ");
	if ( lpszParams )
		strcat(buffer, lpszParams);
	if ( pService->CreateService( pService->GetServiceName(), buffer ) )
	{
		pService->SetServiceDescription(pService->GetServiceDesc());
		printf("%s service installed!\n", pService->GetServiceName() );
		return TRUE;
	}
	else
		printf("%s service is already installed!\n", pService->GetServiceName() );

	return FALSE;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::UninstallService(CW32Service* pService)
{
	if ( !pService )
		return FALSE;

	if ( pService->OpenService() )
	{
		if ( pService->DeleteService() )
		{
			printf("%s service uninstalled!\n", pService->GetServiceName());
			return TRUE;
		}
		else
			printf("%s service could not be uninstalled!\n", pService->GetServiceName());
	}
	else
		printf("%s service is not installed on this machine!\n", pService->GetServiceName());

	return FALSE;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::IsServiceRunning()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;
	if ( !(::QueryServiceStatus( m_serviceHandle, &pStatus->status )) )
		return FALSE;
	return ( pStatus->status.dwCurrentState == SERVICE_RUNNING );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::IsServicePaused()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;
	if ( !(::QueryServiceStatus( m_serviceHandle, &pStatus->status )) )
		return FALSE;
	return ( pStatus->status.dwCurrentState == SERVICE_PAUSED );
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::IsServiceStopped()
{
	stSvcStatus*			pStatus = FindServiceStatus( m_lpszServiceName );

	if ( !m_serviceHandle || !pStatus )
		return FALSE;
	if ( !(::QueryServiceStatus( m_serviceHandle, &pStatus->status )) )
		return FALSE;
	return ( pStatus->status.dwCurrentState == SERVICE_STOPPED );
}

///////////////////////////////////////////////////////////////////////

CW32Service::stSvcStatus* CW32Service::AddService(const char* lpszServiceName)
{
	if ( !m_ppStatus )
	{
		m_ppStatus = new stSvcStatus*[1];
	}
	else
	{
		stSvcStatus**				ppNew = NULL;

		ppNew = new stSvcStatus*[m_nStatusCount+1];
		memcpy(ppNew, m_ppStatus, m_nStatusCount);
		delete [] m_ppStatus;
		m_ppStatus = ppNew;
	}

	m_ppStatus[m_nStatusCount] = new stSvcStatus;
	memset(m_ppStatus[m_nStatusCount], 0, sizeof(stSvcStatus));
	m_ppStatus[m_nStatusCount]->lpszServiceName = new TCHAR[lstrlen(lpszServiceName)+1];
	ZeroMemory((void*)m_ppStatus[m_nStatusCount]->lpszServiceName, lstrlen(lpszServiceName)+1);
	lstrcpy(m_ppStatus[m_nStatusCount]->lpszServiceName, (char*)lpszServiceName);
	m_ppStatus[m_nStatusCount]->pServiceObj = this;
	
	return (m_ppStatus[m_nStatusCount++]);
}

///////////////////////////////////////////////////////////////////////

CW32Service::stSvcStatus* CW32Service::FindServiceStatus(const char* lpszServiceName)
{
	stSvcStatus*			pStatus = NULL;
	int						i;

	if ( !lpszServiceName )
		return NULL;

	for ( i = 0; i < m_nStatusCount; i++ )
	{
		// check to see if this is the status we are looking for...
		if ( !lstrcmp(m_ppStatus[i]->lpszServiceName, lpszServiceName) )
		{
			// yes, this is the service status handle that we wants....
			pStatus = m_ppStatus[i];
			break;
		}
	}

	return pStatus;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::ExecuteService()
{
	if ( !m_lpszServiceName )
		return FALSE;

	m_arrDispatchTable[0].lpServiceName = m_lpszServiceName;
	m_arrDispatchTable[0].lpServiceProc = _serviceMain;

  return ( StartServiceCtrlDispatcher( m_arrDispatchTable ) ? true : false );
}

///////////////////////////////////////////////////////////////////////

void WINAPI	CW32Service::_serviceMain(DWORD dwArgc, char** lpszArgv)
{
	stSvcStatus*				pStatus = FindServiceStatus(lpszArgv[0]);
	CW32Service*				pService = pStatus->pServiceObj;

    pStatus->status.dwServiceType				  = SERVICE_WIN32_OWN_PROCESS; 
    pStatus->status.dwCurrentState				= SERVICE_START_PENDING; 
    pStatus->status.dwControlsAccepted		= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
    pStatus->status.dwWin32ExitCode				= 0; 
    pStatus->status.dwServiceSpecificExitCode	= 0; 
    pStatus->status.dwCheckPoint				  = 0; 
    pStatus->status.dwWaitHint					  = 0; 

	// register the service control handler
	if ( !(pStatus->statusHandle = RegisterServiceCtrlHandlerEx( pStatus->lpszServiceName, _serviceHandler, pStatus )) )
		return;

	// call the initialisation function (if stipulated)
	if ( pStatus->lpInitProc )
		pStatus->lpInitProc((void*)pStatus);

	// now we can run the service... ;) YAY!!
	pStatus->status.dwCurrentState = SERVICE_RUNNING;
	pStatus->status.dwCheckPoint	= 0;
	pStatus->status.dwWaitHint = 0;
	SetServiceStatus( pStatus->statusHandle, &pStatus->status );
}

///////////////////////////////////////////////////////////////////////

DWORD WINAPI CW32Service::_serviceHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	stSvcStatus*				pStatus = (stSvcStatus*) lpContext;

	if ( !pStatus )
		return -1;

	switch (dwControl)
	{
        case SERVICE_CONTROL_PAUSE: 
			{
				pStatus->status.dwCurrentState = SERVICE_PAUSED; 
				pStatus->pServiceObj->OnServicePause(pStatus);
			}
            break; 
 
        case SERVICE_CONTROL_CONTINUE: 
			{
				pStatus->status.dwCurrentState = SERVICE_RUNNING; 
				pStatus->pServiceObj->OnServiceResume(pStatus);
			}
            break; 
 
        case SERVICE_CONTROL_STOP: 
			{
				pStatus->status.dwWin32ExitCode = 0; 
				pStatus->status.dwCurrentState  = SERVICE_STOPPED; 
				pStatus->status.dwCheckPoint    = 0; 
				pStatus->status.dwWaitHint      = 0; 
 
				SetServiceStatus(pStatus->statusHandle, &pStatus->status);
				pStatus->pServiceObj->OnServiceStop(pStatus);
			} 
            return NO_ERROR; 
 
        case SERVICE_CONTROL_INTERROGATE: 
			{
				pStatus->pServiceObj->OnServiceInterrogate(pStatus);
			}
            break; 
 
        default: 
			{
				pStatus->pServiceObj->OnServiceUnknownMsg(pStatus);
			}
			break;
    } 
 
    // Send current status. 
    SetServiceStatus(pStatus->statusHandle,  &pStatus->status);

	return NO_ERROR;
}

///////////////////////////////////////////////////////////////////////

unsigned int CW32Service::ThreadFunction(void* pParam)
{
  CW32Service* pThis = (CW32Service*) pParam;

  if ( !pThis )
    return 0;

  pThis->m_pLogic->Start();
  while ( WaitForSingleObject( pThis->m_hThrdEvent, 500) == WAIT_TIMEOUT )
  {
    ;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////
// EVENT HANDLERS BELOW... be afraid... be VERY afraid...
///////////////////////////////////////////////////////////////////////

bool CW32Service::OnServiceStop(const CW32Service::stSvcStatus* pStatus)
{
  ISRV_Stop();
	return true;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::OnServicePause(const CW32Service::stSvcStatus* pStatus)
{
	return true;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::OnServiceResume(const CW32Service::stSvcStatus* pStatus)
{
	return true;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::OnServiceInterrogate(const CW32Service::stSvcStatus* pStatus)
{
	return true;
}

///////////////////////////////////////////////////////////////////////

bool CW32Service::OnServiceUnknownMsg(const CW32Service::stSvcStatus* pStatus)
{
	return true;
}