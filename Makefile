#WINDOWS
INCL = -IC:/SFML-2.6.2/include
LIBS = -LC:/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system
EXT = .exe
RM = del

#LINUX
#INCL =
#LIBS = -lsfml-graphics -lsfml-window -lsfml-system
#EXT =
#RM = rm -f

CC = g++ --std=c++11 -Wall
CCO = $(CC) -c $<

OBJECTS = main.o Game.o Board.o Cell.o Entity.o RuleManager.o DesignManager.o Level.o LevelManager.o MainMenu.o PauseMenu.o

all : $(OBJECTS)
		$(CC) $(INCL) -o baba$(EXT) $(OBJECTS) $(LIBS)
		./baba$(EXT)

main.o : src/main.cpp src/Game.hpp
	$(CCO) $(INCL)

Game.o : src/Game.cpp src/Game.hpp src/model/Board.hpp src/Types.hpp src/RuleManager.hpp src/DesignManager.hpp
	$(CCO) $(INCL)

RuleManager.o : src/RuleManager.cpp src/RuleManager.hpp src/model/Board.hpp src/Types.hpp
	$(CCO) $(INCL)

DesignManager.o : src/DesignManager.cpp src/DesignManager.hpp src/model/Board.hpp src/Types.hpp
	$(CCO) $(INCL)

Level.o : src/Level.cpp src/Level.hpp src/model/Board.hpp src/Types.hpp
	$(CCO) $(INCL)

LevelManager.o : src/LevelManager.cpp src/LevelManager.hpp src/model/Board.hpp src/Types.hpp src/Level.hpp
	$(CCO) $(INCL)

MainMenu.o : src/MainMenu.cpp src/MainMenu.hpp src/Types.hpp
	$(CCO) $(INCL)

PauseMenu.o : src/PauseMenu.cpp src/PauseMenu.hpp src/Types.hpp
	$(CCO) $(INCL)

Board.o : src/model/Board.cpp src/model/Board.hpp src/model/Cell.hpp src/model/Entity.hpp src/Types.hpp
	$(CCO) $(INCL)

Cell.o : src/model/Cell.cpp src/model/Cell.hpp src/model/Entity.hpp src/Types.hpp
	$(CCO) $(INCL)

Entity.o : src/model/Entity.cpp src/model/Entity.hpp src/Types.hpp
	$(CCO) $(INCL)

clean : 
	-$(RM) *.o
	-$(RM) baba$(EXT)