#include "../../common/protocol/fcprotocol.h"
#include "fccontroller.h"

FCController::FCController(void)
: m_bConnected(false)
{
}

///////////////////////////////////////////////////////////////////////

FCController::~FCController(void)
{
}

///////////////////////////////////////////////////////////////////////

bool FCController::Initialise()
{
  if ( !m_sock.Create() )
  {
    fprintf(stderr, "Failed to create socket\n");
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

FCINT FCController::Run()
{
  string strServer = "127.0.0.1";
  short port = 6666;

  if ( !ConnectToServer(strServer, port) )
  {
    return -1;
  }

  RequestServerInfo();

  // game loop
  m_hGameEvent = CreateEvent(0, 0, 0, 0);
  while ( WaitForSingleObject(m_hGameEvent, 250) )
  {

  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

bool FCController::ConnectToServer(string server, short port)
{
  DWORD dwRes = 0;

  printf("Attempting to connect to %s:%ld....", server.c_str(), port);

  m_sock.Create();
  m_sock.Subscribe(this);

  m_hSockEvent = CreateEvent(0, 0, 0, 0);

  m_sock.Connect(server.c_str(), port);

  dwRes = WaitForSingleObject(m_hSockEvent, INFINITE);

  return m_bConnected;
}

///////////////////////////////////////////////////////////////////////

void FCController::CreateBasePacket(PEPacket& pkt, FCBYTE type)
{
  unsigned char header[8] = { 0x0A, 0x0A, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40 };

  pkt.AddField("magic", 1, 8, header);
  pkt.AddField("type", 1, 1, &type);
  pkt.AddField("msg", 2, 1);
  pkt.AddField("dataLen", 4, 1);
}

///////////////////////////////////////////////////////////////////////

void FCController::RequestServerInfo()
{
  PEPacket pkt;
  int nVal;

  printf("Requesting Server Info:\n");
  CreateBasePacket(pkt, FCPKT_COMMAND);
  pkt.SetFieldValue("msg", (void*)&FCMSG_INFO_SERVER);
  nVal = 0;
  pkt.AddField("data", 1, 1, &nVal);
  nVal = 1;
  pkt.SetFieldValue("dataLen", &nVal);

  char* pData = NULL;
  size_t blockLen = 0;

  pkt.GetDataBlock(pData, blockLen);
  m_sock.Send((FCBYTE*)pData, (int)blockLen);
}

///////////////////////////////////////////////////////////////////////

void FCController::OnAccepted(BaseSocket* pSocket, int nErrorCode)
{
}

///////////////////////////////////////////////////////////////////////

void FCController::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
  if ( !nErrorCode )
  {
    m_bConnected = true;
    printf("OK\n");
  }
  else
  {
    m_bConnected = false;
    printf("ERROR: %ld\n", nErrorCode);
  }

  SetEvent(m_hSockEvent);
}

///////////////////////////////////////////////////////////////////////

void FCController::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
  m_bConnected = false;
}

///////////////////////////////////////////////////////////////////////

void FCController::OnDataReceived(BaseSocket* pSocket, BYTE* pData, int nLen)
{
  printf("DATA RECEIVED [%ld bytes]\n", nLen);
}
