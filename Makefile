# Detect OS
UNAME_S := $(shell uname -s)

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude -Wno-missing-field-initializers

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)
    # macOS
    CXXFLAGS += -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else ifeq ($(UNAME_S),Linux)
    # Linux
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
    # Windows (MinGW)
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Target executable
TARGET = time_master

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Header files
HEADERS = $(wildcard include/*.hpp)

# Default target
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete"

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all clean run rebuild
