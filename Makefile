
SRC_DIR = src
BUILD_DIR = obj
TARGET_DIR = bin
TRASH_DIR = trash

CXX = g++
LINK = g++

LFLAGS = -lGL -lglfw

.PHONY: all
all: $(TARGET_DIR)/test_program

$(BUILD_DIR)/test_program.o: $(SRC_DIR)/test_program.cpp
	$(CXX) -std=c++11 -c $(SRC_DIR)/test_program.cpp -o $(BUILD_DIR)/test_program.o

$(TARGET_DIR)/test_program: $(BUILD_DIR)/test_program.o
	$(LINK) $(BUILD_DIR)/test_program.o -o $(TARGET_DIR)/test_program $(LFLAGS)

.PHONY: clean

clean: 
	# cuz im scared of rm
	mkdir -p $(TRASH_DIR)
	mv $(TARGET_DIR)/* $(TRASH_DIR)/ 
	mv $(BUILD_DIR)/* $(TRASH_DIR)/ 


