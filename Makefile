CC = g++
CC_OPTIONS = -g

SRC_DIR = src
BIN_DIR = bin
TMP_DIR = tmp
INCLUDE_DIR = include

INCLUDE_DIRS = -I$(TMP_DIR) -I$(INCLUDE_DIR)

SRC_INPUT_FILES = $(SRC_DIR)/complex.cpp $(SRC_DIR)/draw.cpp $(INCLUDE_DIR)/common/shader.cpp $(INCLUDE_DIR)/common/common.cpp $(SRC_DIR)/mandelbrot.cpp $(SRC_DIR)/main.cpp
OUTPUT_FILE = $(BIN_DIR)/mandelbrot

all:
	$(CC) $(CC_OPTIONS) $(SRC_INPUT_FILES) $(INCLUDE_DIRS) -o $(OUTPUT_FILE) -lm -lGLEW -lglfw -lGL
	
clean:
	rm $(TMP_DIR)/* $(OUTPUT_FILE)
