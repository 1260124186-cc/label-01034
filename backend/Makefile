CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsqlite3

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
MAIN_OBJECT = $(BUILD_DIR)/Main.o
TEST_MAIN_OBJECT = $(BUILD_DIR)/TestMain.o
COMMON_OBJECTS = $(filter-out $(MAIN_OBJECT) $(TEST_MAIN_OBJECT), $(OBJECTS))
TARGET = campus_system
TEST_TARGET = test_campus_system

.PHONY: all clean run test test-run

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

test: $(BUILD_DIR) $(TEST_TARGET)

$(TEST_TARGET): $(TEST_MAIN_OBJECT) $(COMMON_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

test-run: test
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET) data/campus.db data/test_campus.db data/test_*.csv data/test_*.txt
