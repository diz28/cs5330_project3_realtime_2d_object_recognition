
# Di Zhang
# Feb 24, 2023
# CS5330 - Computer Vision

CC = g++
PROJECT = main
SRC = main.cpp preprocessing.cpp csv_util.cpp dataprocessing.cpp confusion_matrix.cpp
LIBS = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
