CC=g++
CFLAGS=-c
LDFLAGS=-lpcap
SOURCES=BeaconFlood.cpp BeaconFloodMain.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=beacon-flood

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)