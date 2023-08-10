sources = main.cpp bits.cpp
objects = $(patsubst %.cpp,%.o,$(sources))
CPP = g++
OPTS = -Wall
PROG = iHevc

$(PROG): $(objects)
	$(CPP) $(OPTS) -o $@ $(objects)

main.o: main.cpp
	$(CPP) -c $<

bits.o: bits.cpp
	$(CPP) $(OPTS) -c $<

clean:
	$(RM) $(PROG) $(objects)

