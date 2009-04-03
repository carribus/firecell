#include <sstream>
#include "PEPacketHelper.h"
#include "ServiceLogicBase.h"

ServiceLogicBase::ServiceLogicBase(const string& serviceName, const string serviceDesc, bool bHasConsole)
: m_ServiceName(serviceName)
, m_ServiceDesc(serviceDesc)
, m_bHasConsole(bHasConsole)
{
}

///////////////////////////////////////////////////////////////////////

ServiceLogicBase::~ServiceLogicBase(void)
{
  m_bDBMonRunning = false;
  pthread_join( m_thrdDBMon, NULL );
}

///////////////////////////////////////////////////////////////////////

const char* ServiceLogicBase::GetName()
{
  return m_ServiceName.c_str();
}

///////////////////////////////////////////////////////////////////////

const char* ServiceLogicBase::GetDesc()
{
  return m_ServiceDesc.c_str();
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  if ( !nErrorCode )
  {
    m_mapRouters[ pSock->GetServer() ] = pSock;
    RegisterServiceWithRouter(pSock);
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to connect to router (%s:%ld)", pSock->GetServer().c_str(), pSock->GetPort()) );
  }
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  // temporary code - we probably need to attempt to reconnect to the dropped router
  m_mapRouters.erase( pSock->GetServer() );
  delete pSock;
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen)
{
  PEPacket* pPkt = NULL;
	RouterSocket* pRouter = (RouterSocket*)pSocket;
  CBinStream<FCBYTE, true>& stream = pRouter->GetDataStream();
  size_t offset = 0;

  pRouter->AddData(pData, (FCULONG)nLen);

  while ( (pPkt = m_pktExtractor.Extract( (const char*)(FCBYTE*)stream, offset, (size_t)stream.GetLength() )) )
  {
    pPkt->DebugDump();
    stream.Delete(0, (unsigned long)offset);
    offset = 0;
    HandlePacket(pPkt, pSocket);
    delete pPkt;
  }
}


///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::RegisterServiceWithRouter(RouterSocket* pSock)
{
  if ( !pSock )
    return;

  PEPacket pkt;

  __FCPKT_REGISTER_SERVER d;

  d.type = GetServiceType();

  PEPacketHelper::CreatePacket( pkt, FCPKT_COMMAND, FCMSG_REGISTER_SERVICE );
  PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_REGISTER_SERVER) );

  // send the packet
  pSock->Send(&pkt);
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::HandlePacket(PEPacket* pPkt, BaseSocket* pSocket)
{
  bool bHandled = false;
  FCBYTE pktType = 0;

  pPkt->GetField("type", &pktType, sizeof(FCBYTE));

  switch ( pktType )
  {
  case  FCPKT_COMMAND:
    bHandled = OnCommand(pPkt, pSocket);
    break;

  case  FCPKT_RESPONSE:
    bHandled = OnResponse(pPkt, pSocket);
    break;

  case  FCPKT_ERROR:
    bHandled = OnError(pPkt, pSocket);
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::LoadDBSettingsFromConfig(string& strEngine, string& strServer, string& strDBName, string& strUser, string& strPass)
{
  bool bResult = false;
  INIFile::CSection* pSection = m_config.GetSection("Database");

  if ( pSection )
  {
    strEngine = pSection->GetValue("engine");
    strServer = pSection->GetValue("server");
    strDBName = pSection->GetValue("dbname");
    strUser = pSection->GetValue("user");
    strPass = pSection->GetValue("pass");
    bResult = true;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::ConnectToRouters()
{
  bool bResult = false;
  int nRouterCount = 0;
  string strKey;
  string strValue, strServer;
  short port;
  INIFile::CSection* pSection = m_config.GetSection("Routers");

  if ( pSection )
  {
    for ( nRouterCount = 0; ;nRouterCount++ )
    {
      stringstream ss;
      ss << "Router" << nRouterCount;
      strKey = ss.str();
      strValue = pSection->GetValue(strKey);
      if ( !strValue.empty() )
      {
        strServer = strValue.substr(0, strValue.find(':'));
        port = atoi( strValue.substr( strValue.find(':')+1, strValue.length() ).c_str() );

        DYNLOG_ADDLOG( DYNLOG_FORMAT("Connecting to router (%s:%ld)", strServer.c_str(), port) );

        RouterSocket* pSock = new RouterSocket;
        pSock->SetServer( strServer );
        pSock->SetPort( port );
        pSock->Subscribe(this);

        pSock->Create();
        pSock->Connect(strServer.c_str(), port);
      }
      else
        break;
    }
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::DisconnectFromRouters()
{
  ServiceSocketMap::iterator it;
  RouterSocket* pRouter = NULL;

  for ( it = m_mapRouters.begin(); it != m_mapRouters.end(); it++ )
  {
    pRouter = it->second;
    pRouter->Disconnect();
    delete pRouter;
  }

  m_mapRouters.clear();
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::ConfigureDatabase(string strEngine, string strServer, string strDBName, string strUser, string strPass)
{
  // start the database monitoring thread to monitor for any results that may become available
  if ( pthread_create( &m_thrdDBMon, NULL, thrdDBWorker, (void*)this ) != 0 )
  {
    return false;
  }

  return m_db.Initialise(strEngine, strServer, strDBName, strUser, strPass);
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::RegisterDBHandler(const string strJobRef, DBHANDLERPROC handler)
{
  if ( strJobRef.empty() || !handler )
    return;

  m_mapDBHandlers[strJobRef] = handler;
}

///////////////////////////////////////////////////////////////////////

void* ServiceLogicBase::thrdDBWorker(void* pData)
{
  ServiceLogicBase* pThis = (ServiceLogicBase*)pData;
  FCDBJob job;

  if ( !pThis )
    return NULL;

  pThis->m_bDBMonRunning = true;

  while ( pThis->m_bDBMonRunning )
  {
    while ( pThis->m_db.GetCompletedJobCount() )
    {
      pThis->m_db.GetNextCompletedJob(job);
      pThis->HandleCompletedDBJob(job);
    }
#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::HandleCompletedDBJob(FCDBJob& job)
{
  string jobRef = job.GetReference();
  DBIResults* pResults = job.GetResults();
  DBIResultSet* pResultSet = NULL;
  void* pCtx = job.GetData();
  bool bHandled = false;

  while ( pResults && pResults->GetCount() )
  {
    if ( (pResultSet = pResults->GetNextResultSet()) )
    {
      // Try and find a handler for this db context and forward it on....
      MapDBHandlers::iterator it = m_mapDBHandlers.find( jobRef );

      if ( it != m_mapDBHandlers.end() )
      {
        (*it->second)(*pResultSet, pCtx);
        bHandled = true;
      }

      // clear the result set
      delete pResultSet;
    }
  }

  // NOTE: We do not delete the context (pCtx) since it was not created/initialised by us. We expect the db handler to clean up after itself.
  if ( pCtx != NULL && HasConsole() )
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Possible leak of DBJob context: JobRef=%s", jobRef.c_str()) );
  }

  // delete the results object
  delete pResults;
}
