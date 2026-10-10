CXX      := clang++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -pthread
BUILD    := build
TARGET   := $(BUILD)/main

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rebuild an object when its source or the shared header changes
$(BUILD)/%.o: %.cpp mat_operations.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD)
