# FRACTAL RAYMARCHER MAKEFILE
#
# Stefan Wong 2020

# OUTPUT DIRS
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TEST_DIR=test
TEST_BIN_DIR=$(BIN_DIR)/test
PROGRAM_DIR=program

# Tool options
CXX=g++
OPT=-O0
CXXFLAGS=-Wall -g2 -std=c++14 $(OPT)
TESTFLAGS=
LDFLAGS= -lGL -lGLEW -lSDL2
LIBS = 
TEST_LIBS=

# Object targets
INCS=-I$(SRC_DIR)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Unit tests 
TEST_SOURCES  = $(wildcard $(TEST_DIR)/*.cpp)
# Tools (program entry points)
PROGRAM_SOURCES = $(wildcard $(PROGRAM_DIR)/*.cpp)


# Objects 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Unit tests 
TEST_OBJECTS  := $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)
$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@ 

# ==== TEST TARGETS ==== #
TESTS=test_camera test_display test_shader 
$(TESTS): $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(OBJ_DIR)/$@.o\
		-o $(TEST_BIN_DIR)/$@ $(LIBS) $(TEST_LIBS)

# ==== PROGRAM TARGETS ==== #
PROGRAMS=frayma 
PROGRAM_OBJECTS := $(PROGRAM_SOURCES:$(PROGRAM_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(PROGRAM_OBJECTS) : $(OBJ_DIR)/%.o : $(PROGRAM_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

$(PROGRAMS): $(OBJECTS) $(PROGRAM_OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) $(OBJ_DIR)/$@.o\
		-o $(BIN_DIR)/$@ $(LIBS) $(TEST_LIBS)

.PHONY: clean

# Main targets 
all : test programs

obj : $(OBJECTS)		# TODO : just for debugging

test : $(TESTS)

programs: $(PROGRAMS)

clean:
	rm -rfv *.o $(OBJ_DIR)/*.o 

print-%:
	@echo $* = $($*)
