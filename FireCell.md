# Introduction #

FireCell is a game where a player becomes a hacker in a virtual world that has a global network resembling the current internet. The object of the game is to start as a 'newbie' hacker-wannabe, and progress through the game by learning, hacking, and gaining experience from each such event.

The game will be a network based game, with servers hosted somewhere on the internet. Players will be able to communicate with each other, compete against each other, and either co-operate on hacking missions, or interfere with other player's hacking attempts. Players may also be able to hack other players and 'steal' information, programs and other assets (maybe money?).

The game will be presented in an 3D rendered environment, although it should not be required to compete with titles such as Age of Conan, Guildwars, World of Warcraft etc with regards to graphical intensity or environments. At the heart of FireCell is the concept that it is an intelligent game for players that enjoy mental and strategic challenges, against the 'environment' as well as against other players.

# Components #

FireCell consists of the following high level components:

**- A server component:**

The Server component is based on a distributed architecture model and has core functions seperated into connectable service processes. Currently the model is comprised of a public facing traffic distribution gateway, an authentication gateway, and the actual game world.

**- A client component:**

The client is the means for players to interact with the FireCell world.

**- A tools component:**

The tools component comprises of many tools that aid either developers or content designers in enhancing, growing and modifying the FireCell world.

**- A database component:**

The database component offers the required persistance to the FireCell world. Currently the engine supports only mySql, but has been written in such a way that it is fairly simple to create connectors and query objects for different database engines.