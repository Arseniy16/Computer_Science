CXX = g++
CXXFLAGS = -Wall -g -std=c++2a
CXXFLAGS += -Iheaders

LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system 

HEADL = headers

SRCL = sources
SOURCES = view.cpp textView.cpp graphView.cpp main.cpp model.cpp control.cpp 
SRC := $(addprefix $(SRCL)/, $(SOURCES))

OBJECTS = $(SOURCES:.cpp=.o)
OBJ := $(addprefix $(SRCL)/, $(OBJECTS))

PROG = snake
RM = rm -f -r

$(SRCL)/%.o: $(SRCL)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(PROG)

depend:
	$(CXX) $(CXXFLAGS) $(SRC) -MM > .depend

$(PROG): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(PROG) $(LDFLAGS)

sfmlWindow: sfmlTest.cpp
	$(CXX) -c sfmlTest.cpp $(CXXFLAGS) -o sfmlTest.o
	$(CXX) sfmlTest.o -o sfml-app $(LDFLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) $(PROG)

-include .depend