sources = main.cpp bits.cpp parser.cpp writer.cpp
objects = $(patsubst %.cpp,%.o,$(sources))
CPP = g++
OPTS = -Wall
PROG = iHevc

$(PROG): $(objects)
	$(CPP) $(OPTS) -o $@ $(objects)

main.o: main.cpp
	$(CPP) -c $<

parser.o: parser.cpp
	$(CPP) -c $<

writer.o: writer.cpp
	$(CPP) -c $<

bits.o: bits.cpp
	$(CPP) $(OPTS) -c $<

clean:
	$(RM) $(PROG) $(objects)

