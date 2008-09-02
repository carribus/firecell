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
#if !defined(_W32SERVICE_H_)
#define _W32SERVICE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "../interfaces/IService.h"
#include "../interfaces/IServiceLogic.h"

typedef	DWORD (*LP_SERVICE_INIT_PROC)(void*);

class CW32Service : public IService
{
public:

	//
	// structures
	struct	stSvcStatus
	{
		char* 					      	lpszServiceName;		// name of service
		SERVICE_STATUS_HANDLE		statusHandle;			// service status handle
		SERVICE_STATUS				  status;					// service status struct
		LP_SERVICE_INIT_PROC		lpInitProc;				// function pointer to optional init function
		CW32Service*				    pServiceObj;
	};

	//
	// constructor/destructor
	CW32Service(const char* lpMachineName = NULL, const char* lpDataBaseName = NULL, DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS);
	virtual ~CW32Service();

	static DWORD		Initialise(void* pParam);

	//
	// Methods
	bool							ExecuteService();
	bool							OpenService(DWORD dwDesiredAccess = SERVICE_ALL_ACCESS);
	bool							CreateService(const char* lpDisplayName, const char* lpBinaryPathName, DWORD dwDesiredAccess = SERVICE_ALL_ACCESS, DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS, DWORD dwStartType = SERVICE_DEMAND_START, DWORD dwErrorControl = SERVICE_ERROR_NORMAL, const char* lpLoadOrderGroup = NULL, DWORD* lpdwTagID = NULL, const char* lpDependencies = NULL, const char* lpServiceStartName = NULL, const char* lpPassword = NULL);
	bool							DeleteService();
	bool							StartService(DWORD dwNumArgs = 0, const char** lpServiceArgs = 0);
	bool							StopService();
	bool							PauseService();
	bool							ResumeService();

  //
  // IService Implementation
  bool ISRV_AttachLogic(IServiceLogic* pLogic);
  void ISRV_RunAsApp(bool bAsApp);
  int ISRV_Run(void* pData);
  int ISRV_Stop();

	//
	// Utility Methods
	static bool						InstallService(CW32Service* pService, const char* lpszParams = NULL);
	static bool						UninstallService(CW32Service* pService);

	// statics
	stSvcStatus*					AddService(const char* lpszServiceName);
	static stSvcStatus*		FindServiceStatus(const char* lpszServiceName);

	// event handlers
	virtual	bool					OnServiceStop(const stSvcStatus* pStatus);
	virtual bool					OnServicePause(const stSvcStatus* pStatus);
	virtual bool					OnServiceResume(const stSvcStatus* pStatus);
	virtual bool					OnServiceInterrogate(const stSvcStatus* pStatus);
	virtual bool					OnServiceUnknownMsg(const stSvcStatus* pStatus);

	// getters
	const char*						GetServiceName()									{ return m_lpszServiceName; }
	const char*           GetServiceDesc()									{ return m_lpszServiceDesc; }
	bool							    IsServiceRunning();
	bool							    IsServicePaused();
	bool							    IsServiceStopped();

	// setters
	void							    SetServiceName(const char* lpszName);
	void							    SetServiceDescription(const char* lpszDesc);
	void							    SetInitProc(LP_SERVICE_INIT_PROC lpInitProc);		 

protected:

	// 
	// Methods
	static void WINAPI		_serviceMain(DWORD dwArgc, char** lpszArgv);
	static DWORD WINAPI		_serviceHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);
	void							    CleanupMemory();

  static unsigned int WINAPI   ThreadFunction(void*);

	// Attributes
  static bool           m_bRunAsApp;
	bool							    m_bInitialised;
	SERVICE_TABLE_ENTRY		m_arrDispatchTable[2];
	SC_HANDLE						  m_scmHandle;				  // Service Control Manager Handle
	SC_HANDLE						  m_serviceHandle;			// the service's handle
	char* 							  m_lpszServiceName;
	char* 							  m_lpszServiceDesc;
	static int						m_nStatusCount;				// number of service status handles being stored
	static stSvcStatus**	m_ppStatus;					  // array for the status handles
  HANDLE                m_hThread;
  HANDLE                m_hThrdEvent;

  IServiceLogic*        m_pLogic;
};

#endif//_W32SERVICE_H_