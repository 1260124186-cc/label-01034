CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsqlite3

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = campus_system

.PHONY: all clean run

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p data

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) data/campus.db
