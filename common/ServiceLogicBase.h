#ifndef _SERVICELOGICBASE_H_
#define _SERVICELOGICBASE_H_

#include <string>
#include <map>
#include "binstream.h"
#include "threading.h"
#include "fctypes.h"
#include "inifile.h"
#include "interfaces/IServiceLogic.h"
#include "protocol/fcprotocol.h"
#include "PEPacket.h"
#include "PacketExtractor.h"
#include "Database/FCDatabase.h"
#include "../Clients/common/Socket/ClientSocket.h"

using namespace std;

class ServiceLogicBase : public IServiceLogic
                       , public IBaseSocketSink
{
public:

  class RouterSocket : public BaseSocket
  {
  public:
    RouterSocket() {}
    ~RouterSocket() {}

    void SetServer(string server)             { m_server = server; }
    string GetServer()                        { return m_server; }
    void SetPort(short port)                  { m_port = port; }
    short GetPort()                           { return m_port; }
    void AddData(FCBYTE* pData, FCULONG len)
    {
      m_stream.Concat(pData, len);
    }
    CBinStream<FCBYTE, true>& GetDataStream() { return m_stream; }

  private:
    string              m_server;
    short               m_port;
    CBinStream<FCBYTE, true> m_stream;
  };
  typedef std::map< string, RouterSocket* > ServiceSocketMap;
  typedef std::queue<FCSOCKET> CQueuedSocketArray;

/*
  struct DBJobContext
  {
    RouterSocket* pRouter;
    FCSOCKET clientSocket;
  };
*/
  // the void* param is the optional context data that each derived class will pass in on the FCDatabase::ExecuteJob() call
  typedef void (*DBHANDLERPROC)(DBIResultSet&, void*);

  ServiceLogicBase(const string& serviceName, bool bHasConsole);
  virtual ~ServiceLogicBase(void);

  /*
   *  IServiceLogic implementation
   */
  const char* GetName();
  void HasConsole(bool bHasConsole)               { m_bHasConsole = bHasConsole; }

  //
  // IBaseSocketSink implementation
  void OnAccepted(BaseSocket* pSocket, int nErrorCode) {}
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

  virtual ServiceType GetServiceType() = 0;

protected:

  virtual bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket) = 0;
  virtual bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket) = 0;
  virtual bool OnError(PEPacket* pPkt, BaseSocket* pSocket) = 0;

  bool HasConsole()                               { return m_bHasConsole; }

  bool LoadConfig(FCCSTR strFilename);
  bool LoadDBSettingsFromConfig(string& strEngine, string& strServer, string& strDBName, string& strUser, string& strPass);

  bool ConnectToRouters();
  void DisconnectFromRouters();

  bool ConfigureDatabase(string strEngine, string strServer, string strDBName, string strUser, string strPass);
  void RegisterDBHandler(const string strJobRef, DBHANDLERPROC handler);

  /*
   *  Protected members
   */
  INIFile m_config;
  ServiceSocketMap m_mapRouters;
  PacketExtractor m_pktExtractor;

private:

  void RegisterServiceWithRouter(RouterSocket* pSock);

  void HandlePacket(PEPacket* pPkt, BaseSocket* pSocket);

  static void* thrdDBWorker(void* pData);
  void HandleCompletedDBJob(FCDBJob& job);

  string m_ServiceName;
  bool m_bHasConsole;

  // Database related members
  FCDatabase m_db;
  pthread_t m_thrdDBMon;
  bool m_bDBMonRunning;

  // db proc handlers
  typedef map<string, DBHANDLERPROC> MapDBHandlers;
  MapDBHandlers m_mapDBHandlers;
};

#endif//_SERVICELOGICBASE_H_