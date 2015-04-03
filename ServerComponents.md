# Introduction #

The FireCell service architecture is an n-tier distributed architecture that provides the ability to seperate domain of responsibility from one another, even across physical machines. Communications between the services are maintained via TCP/IP persistent connections. The structure, if visualised, would look as follows:

```
         Authentication Service                           Game World Service
                  \                                               /
                   \                                             /
                    \                                           /
                     ---------> Router Service <----------------
                                      ^
                                      |
                                      |
                                      |
                                 Game Clients
```

# Details #

The server framework is broken up into 3 primary components:

  * The 'Router' service (FCServer)
  * The Authentication service (FCAuthServer)
  * The Game World service (FCWorldServer)

Gaming clients always connect to a Router Service, which then forward packets to either the Authentication Service or Game World Service, depending on the packet content.

**The Router service** is designed to act as the guardian to the service framework, and has the ability to inspect connections (and accept/refuse them based on definable criteria), validate packet content as well as forwarding (wrapping) packets onto their final destinations. The router is also responsible for notifying the Auth and Game World services of unexpected client disconnections.

**The Authentication service** is responsible for providing an interface for gaming clients to log into accounts. Once logged into an account, the service can be interrogated for available characters linked to the account, and then a request can be sent from the client requesting the instantiation of a specific character into the game world. The service is responsible for validating these requests, and notifying the game world service of characters requiring instantiation.

**The Game World service** is where the game is played. Most player interactions (post login) will happen against this service. The service will be responsible for managing player state, quests, npc's, player-to-player interactions and so forth. I do not foresee the game service being responsible for managing any form of player to player communications.