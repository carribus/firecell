#ifndef _PLAYERBASE_H_
#define _PLAYERBASE_H_

#include <string>
#include "../../Clients/common/Socket/ClientSocket.h"
#include "../fctypes.h"
#include "FCObject.h"

class PlayerBase : public FCObject
{
public:

  PlayerBase(const std::string typeName = "PlayerBase");
  PlayerBase(FCULONG accountID, FCULONG id, std::string name, std::string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, const std::string typeName = "PlayerBase");
  virtual ~PlayerBase(void);

  /*
	 *	Accessor/Mutators
	 */
  FCULONG GetAccountID() const            { return m_accountID; }
  FCULONG GetID() const                   { return m_id; }
  std::string GetName() const             { return m_name; }
  std::string GetEmail() const            { return m_email; }
  FCULONG GetXP() const                   { return m_xp; }
  FCULONG GetLevel() const                { return m_level; }
  FCINT GetFameScale() const              { return m_fameScale; }
  FCULONG GetCityID() const               { return m_cityID; }
  FCULONG GetCountryID() const            { return m_countryID; }
  FCSOCKET GetClientSocket() const        { return m_clientSocket; }
	BaseSocket* GetRouterSocket()						{ return m_pRouterSocket; }

  void SetAccountID(FCULONG id)           { m_accountID = id; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(std::string name)          { m_name = name; }
  void SetEmail(std::string email)        { m_email = email; }
  void SetXP(FCULONG xp)                  { m_xp = xp; }
  void SetLevel(FCULONG level)            { m_level = level; }
  void SetFameScale(FCINT scale)          { m_fameScale = scale; }
  void SetCityID(FCULONG id)              { m_cityID = id; }
  void SetCountryID(FCULONG id)           { m_countryID = id; }
  void SetClientSocket(FCSOCKET sock)     { m_clientSocket = sock; }
	void SetRouterSocket(BaseSocket* pSock)	{ m_pRouterSocket = pSock; }

protected:

  /*
   *  Private Members
   */
  FCULONG m_accountID;
  FCULONG m_id;
  string m_name;
  string m_email;
  FCULONG m_xp;
  FCULONG m_level;
  FCINT m_fameScale;
  FCULONG m_cityID;
  FCULONG m_countryID;
  FCSOCKET m_clientSocket;
	BaseSocket* m_pRouterSocket;
};

#endif//_PLAYERBASE_H_