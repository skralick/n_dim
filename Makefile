
SRC_DIR = src
BUILD_DIR = obj
TARGET_DIR = bin
COMMON_DIR = common
TRASH_DIR = trash

CXX = g++
LINK = g++

LFLAGS = -lGL -lglfw -lGLEW

.PHONY: all
all: $(TARGET_DIR)/test_program

$(BUILD_DIR)/shader.o: $(COMMON_DIR)/shader.cpp
	$(CXX) -std=c++11 -c $(COMMON_DIR)/shader.cpp -o $(BUILD_DIR)/shader.o -Icommon

$(BUILD_DIR)/test_program.o: $(SRC_DIR)/test_program.cpp
	$(CXX) -std=c++11 -c $(SRC_DIR)/test_program.cpp -o $(BUILD_DIR)/test_program.o -Icommon

$(TARGET_DIR)/test_program: $(BUILD_DIR)/test_program.o $(BUILD_DIR)/shader.o
	$(LINK) $(BUILD_DIR)/test_program.o $(BUILD_DIR)/shader.o -o $(TARGET_DIR)/test_program $(LFLAGS)

.PHONY: clean

clean: 
	# cuz im scared of rm
	mkdir -p $(TRASH_DIR)
	mv $(TARGET_DIR)/* $(TRASH_DIR)/ 
	mv $(BUILD_DIR)/* $(TRASH_DIR)/ 


