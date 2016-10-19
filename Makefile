### Compiler Settings ###
CXX				= g++
CXXFLAGS 	= -Wall -O3
CXXLIBS		= 

VPATH = src/ includes/ 

### Executable Params ###
BIN		 = ./bin
TARGET = acsasp
DEBUG  = 0

ifeq ($(DEBUG),1)
CXXFLAGS += -DDEBUG
endif

### Versioning Settings ###
OS := $(shell uname)

### Includes and Libs ###
CPP_INCLUDES 		= -I./includes/
CPP_LIBS				= -lboost_regex
CPP_LDFLAGS			=

INCLUDES = $(CPP_INCLUDES)

### Compile rules ###

OBJECTS = 	$(BIN)/project.o \
	 		$(BIN)/file_handler.o \
	 		$(BIN)/helpers.o \
	 		$(BIN)/acoimp.o \
			$(BIN)/main.o


.PHONY: clean 

all: $(BIN)/$(TARGET)
	
$(BIN)/$(TARGET): $(OBJECTS)
	$(CXX) $(CPLEX_FLAGS) $(INCLUDES) $(CPLEX_LIBS) $^ -o $@ $(CPLEX_LDFLAGS) $(CPP_LIBS)

$(BIN)/main.o: main.cpp representation.hpp
	$(CXX) $(CPLEX_FLAGS) $(INCLUDES) -c $< -o $@ 
	
$(BIN)/project.o: project.cpp project.hpp
	$(CXX) $(CXXFLAGS) $(CPP_INCLUDES) -c $< -o $@

$(BIN)/file_handler.o: file_handler.cpp file_handler.hpp
	$(CXX) $(CXXFLAGS) $(CPP_INCLUDES) -c $< -o $@

$(BIN)/helpers.o: helpers.cpp helpers.hpp representation.hpp
	$(CXX) $(CXXFLAGS) $(CPP_INCLUDES) -c $< -o $@
	
$(BIN)/acoimp.o: acoimp.cpp acoimp.hpp representation.hpp
	$(CXX) $(CXXFLAGS) $(CPP_INCLUDES) $(CPP_LIBS) -c $< -o $@	

clean:
	rm $(BIN)/*.o $(BIN)/$(TARGET);
	
