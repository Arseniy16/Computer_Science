CXX = g++
CXXFLAGS = -Wall -g

SOURCES = main.cpp human.cpp model.cpp tview.cpp gview.cpp view.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = snake

.PHONY: all clean depend

all: $(EXEC)

$(EXEC): $(OBJECTS)
    $(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) 

.cpp.o:
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    $(RM) $(OBJECTS) $(EXEC)

depend: 
    $(CXX) $(CXXFLAGS) $(OBJECTS.o=.cpp) -MM > .depend

-include .depend