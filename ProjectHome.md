# Introduction #

FireCell is a game where a player becomes a hacker in a virtual world that has a global network resembling the current internet. The object of the game is to start as a 'newbie' hacker-wannabe, and progress through the game by learning, hacking, and gaining experience from each such event.

The game will be a network based game, with servers hosted somewhere on the internet. Players will be able to communicate
with each other, compete against each other, and either co-operate on hacking missions, or interfere with other player's hacking
attempts. Players may also be able to hack other players and 'steal' information, programs and other assets (maybe money?).

The game will be presented in an 3D rendered environment (possibly even 2D), although it should not be required to compete with
titles such as Age of Conan, Guildwars, World of Warcraft etc with regards to graphical intensity or environments. At the heart of
FireCell is the concept that it is an intelligent game for players that enjoy mental and strategic challenges, against the 'environment'
as well as against other players.

# Current State #

**2010-05-02**

Qt port of the FireCell client is moving along nicely. Having learnt from my mistakes in the irrLicht client, I'm breaking up the FCModel into more granular, specific models. I've also managed to fit my PEPacket framework into the code without much rework. The only object that I've had to start rewriting is the FCServerObj which I was making use of in the irrLicht and console clients.

Although I'm fairly new to Qt, I'm starting to like the way the signal/slot mechanism works, although I am wondering what kind of maintenance nightmare it will be to come back to the code after not having seen it for months and try and figure out the relationships between the objects. I guess thats just one of the joys of having decoupled objects and logic. My current paranoia will probably force me to document even more.

**2010-04-29**

I have partially abandoned the irrLicht based client in favour of writing a Qt based client implementation. There are many reasons for this at this point, but the main reasons are:

1. irrLicht does not lend itself to complex windowing system logic. I will possible come back to the irrLicht implementation later, but for now, its so slow to do anything GUI based.

2. I want to do a fairly complex qt project to get up to speed with the 4.6 framework. This should suffice.

So, the bad news is that there will not be much progress in the development of the game at the moment, but the good news is that once the port is complete, development should move a lot quicker.

**2009-07-13**

I have added a XCode project for the irrLicht client app. It compiles perfectly, and even launches into the loading view, but thats about it. I think I know where the problem lies, and need to code a bit more to make things more generic.

Also, have started implementing player 'actions' such as port scanning which is working fine against other players and in-game company entities.

**2009-05-06**

The Visual Studio solution and projects have been moved to Visual Studio 2008. I will be working on creating a working Code::Blocks workspace and build now.

**First binary release posted!**

I have released a package of Windows binaries for anyone interested in seeing what I've been up to. There is not much functionality in terms of gameplay yet, but this should be enough to give an overview of the direction that I'm heading in.

Head on over to http://code.google.com/p/firecell/downloads/list and download the binary :)

Team Status: **Accepting applications**

Its time to get some experienced people on board. I'm looking for people who enjoy programming, who don't mind being moderated by a project owner, and most importantly, like the concept of the the project.

The skills needed at the moment are:

  * C++ developer (with distributed network development experience)
  * C++ developer (with irrLicht/GUI development experience)
  * Sound designer
  * Graphic designer with experience in working on GUI projects.

Development Status: **Alpha/Implementation**

**2009-02-24**

  * An irrLicht based client is now in development.
  * The Game world's event system in now operational
  * The mission engine is working with basic missions
  * Missions are now definable in the database
  * The Forum engine is up and running (with client support)
  * The FileSystem published events based on certain operations occurring
  * Work has started on new character creation
  * A lot of refactoring and memory leak fixing has taken place
  * Alpha client and server binaries will soon be available for download.

**2008-12-08**

Have completed most of the work on the FileSystem object. I have a feeling that I'm going to need to introduce a mediator pattern when it comes to remote/local handling of file systems. Will do that after Forums. Started work on the forum'ing portion of the game. The forum will literally act the same as a standard web based forum, with some added extras (wait and see :))

I am getting to the point where I will be looking for some help in development. If you have found this project, and think that its something you would like to get involved with, please drop me a line. Potential positions are: 1 c++ dev (preferably with 3d cross platform experience), a graphic artist and a game content designer.

**2008-11-28**

  * Have started work on the filesystems on online systems in the game, as well as the traversal of the file systems.
  * Initial prototype code is in and allows traversal of a player's local filesystem. Next step will be to connect to a 'remote' host and traverse that filesystem.
  * fter the file system is complete, a refactor will probably happen on the code, and then onto the online resources like forums, news and banks.

**2008-11-23**

Authentication Service is done for now.
Lots of work is going into designing the World Service. Currently working on loading the world data and providing efficient access to it, as well as designing the mechanisms for missions to be received (and completed) as well as player interaction with NPCs and game based 'targets'.

**2008-11-03**

Authentication service is almost complete.
Work has started on designing the world event system (emiting events as well as consuming them by the correct recipients).
Completed a refactor and abstraction on base service logic objects, making the development of new service logic layers much simpler.

Work is continuing on the console client to test various new functionality in the services. I would be really interested to know if someone has written a test bench for multi-process solutions such as this one.

Work is also commencing on the 'Administration frontend' which is a php based web application and will provide access to the entire game's data. The administrator will not only allow you to admin the players of the system, but also add new items, quests, npcs etc.