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
/**
 *  \file W32Service.h
    \brief Contains the CW32Service class used to abstract Win32/64 service boiler plate code
    \defgroup Server
 */

#if !defined(_W32SERVICE_H_)
#define _W32SERVICE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "../../interfaces/IService.h"
#include "../../interfaces/IServiceLogic.h"

typedef	DWORD (*LP_SERVICE_INIT_PROC)(void*);

/**
 *  \brief The CW32Service implements the boiler plate code for a Win32/64 service and exposes the
 *  IService interface.
 *  \ingroup Server
 */
class CW32Service : public IService
{
public:

	//
	// structures
  /**
   *  \brief stSvcStatus represent a Windows' service structure
   */
	struct	stSvcStatus
	{
		char* 					      	lpszServiceName;		//!< name of service
		SERVICE_STATUS_HANDLE		statusHandle;			  //!< service status handle
		SERVICE_STATUS				  status;					    //!< service status struct
		LP_SERVICE_INIT_PROC		lpInitProc;				  //!< function pointer to optional init function
		CW32Service*				    pServiceObj;        //!< pointer to the actual CW32Service object that owns this structure
	};

	//
	// constructor/destructor
  /**
   *  \brief The constructor initialises the object
   *
   *  During construction, a connection to the SCM on the specified machine, with the specified name and access will be created
   *  @param lpMachineName The machine to which to connect (NULL means <i>localhost</i>
   *  @param lpDataBaseName If a custom named SCM database exists, use this parameter to specify it
   *  @param dwDesiredAccess The type of access rights you want the service to have
   */
	CW32Service(const char* lpMachineName = NULL, const char* lpDataBaseName = NULL, DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS);
	virtual ~CW32Service();

  /**
   *  \brief  Initialises the object after construction
   *
   *  Call this function after construction.
   *  If the m_bRunAsApp is set to true, the logic component will be started and a console will appear with a prompt to press 'q' to quit execution.
   *  If the m_bRunAsApp is set to false, the a thread is created for the service and executes the code at CW32Service::ThreadFunction.
   */
	static DWORD		Initialise(void* pParam);

  /**
   *  \brief  If running as a service, this function populates the SCM dispatch table and calls the Win32/64 function StartServiceCtrlDispatcher( ).
   */
  bool							ExecuteService();

  /**
   *  \brief  Call this function to gain access to the SCM installed service that this object represents
   */
	bool							OpenService(DWORD dwDesiredAccess = SERVICE_ALL_ACCESS);

  /**
   *  \brief  Call this function to install the service into the SCM
   */
	bool							CreateService(const char* lpDisplayName, const char* lpBinaryPathName, DWORD dwDesiredAccess = SERVICE_ALL_ACCESS, DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS, DWORD dwStartType = SERVICE_DEMAND_START, DWORD dwErrorControl = SERVICE_ERROR_NORMAL, const char* lpLoadOrderGroup = NULL, DWORD* lpdwTagID = NULL, const char* lpDependencies = NULL, const char* lpServiceStartName = NULL, const char* lpPassword = NULL);

  /**
   *  \brief  Calling this function will attempt to remove the service from the SCM if it exists
   */
	bool							DeleteService();

  /**
   *  \brief  Calling this function will request the SCM to start the service in the SCM execution context
   */
	bool							StartService(DWORD dwNumArgs = 0, const char** lpServiceArgs = 0);

  /**
   *  \brief  Calling this function will request the SCM to stop the service if it is running
   */
	bool							StopService();

  /**
   *  \brief  Call this function to pause the service's execution
   */
	bool							PauseService();

  /**
   *  \brief  Call this function to resume the service's execution if suspended.
   */
	bool							ResumeService();

  //
  // IService Implementation
  bool ISRV_AttachLogic(IServiceLogic* pLogic);
  void ISRV_RunAsApp(bool bAsApp);
  int ISRV_Run(void* pData);
  int ISRV_Stop();
  void ISRV_GetBinPath(char* outBuffer, size_t& bufferSize);

	//
	// Utility Methods
  /**
   *  \brief  Install the service into the SCM
   */
	static bool						InstallService(CW32Service* pService, const char* lpszParams = NULL);

  /**
   *  \brief  Uninstall the service from the SCM
   */
	static bool						UninstallService(CW32Service* pService);

	// statics
	stSvcStatus*					AddService(const char* lpszServiceName);
	static stSvcStatus*		FindServiceStatus(const char* lpszServiceName);

	// event handlers
  /**
   *  \brief  This callback will be invoked when a service is stopped
   */
	virtual	bool					OnServiceStop(const stSvcStatus* pStatus);

  /**
   *  \brief  This callback will be invoked when a service is paused
   */
  virtual bool					OnServicePause(const stSvcStatus* pStatus);

  /**
   *  \brief  This callback will be invoked when a service is resumed
   */
  virtual bool					OnServiceResume(const stSvcStatus* pStatus);

  /**
   *  \brief  This callback will be invoked when a service is interrogated
   */
  virtual bool					OnServiceInterrogate(const stSvcStatus* pStatus);

  /**
   *  \brief  This callback will be invoked when a service receives an unknown message/signal
   */
  virtual bool					OnServiceUnknownMsg(const stSvcStatus* pStatus);

	// getters
  /**
   *  \brief Returns the name of the service
   */
	const char*						GetServiceName()									{ return m_lpszServiceName; }

  /**
   *  \brief Returns the description of the service
   */
  const char*           GetServiceDesc()									{ return m_lpszServiceDesc; }

  /**
   *  \brief Returns true if the service is running
   */
  bool							    IsServiceRunning();

  /**
   *  \brief Returns true if the service is paused
   */
  bool							    IsServicePaused();

  /**
   *  \brief Returns true if the service is stopped
   */
  bool							    IsServiceStopped();

	// setters
  /**
   *  \brief Sets the name of the service
   */
	void							    SetServiceName(const char* lpszName);

  /**
   *  \brief Sets the description of the service
   */
  void							    SetServiceDescription(const char* lpszDesc);

  /**
   *  \brief Sets the initialisation procedure for the service
   */
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
	SC_HANDLE						  m_scmHandle;				    //!< Service Control Manager Handle
	SC_HANDLE						  m_serviceHandle;			  //!< the service's handle
	char* 							  m_lpszServiceName;
	char* 							  m_lpszServiceDesc;
	static int						m_nStatusCount;				  //!< number of service status handles being stored
	static stSvcStatus**	m_ppStatus;					    //!< array for the status handles
  HANDLE                m_hThread;              //!< The service thread handle
  HANDLE                m_hThrdEvent;           //!< The service thread event handle

  IServiceLogic*        m_pLogic;
};

#endif//_W32SERVICE_H_