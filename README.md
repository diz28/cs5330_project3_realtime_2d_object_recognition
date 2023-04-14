
### Di Zhang
### Feb 24, 2023
### CS5330 - Computer Vision

# cs5330_project3_realtime_2d_object_recognition

## Project Description

This is a 2d object recognition program that can be used to identify objects in image that has already have some data stored in the program. This program uses 10 features to identify a particular object - the least central moment, the oritented bound box filled ratio, height to width ratio and the 7 moments from opencv function `moments`.

## How to install and run

### Have the following 4 programs ready on your **Linux** machine:

- opencv 4.7.0
- g++
- vscode
- make

### How to run in terminal

1. To properly use this program, we first need to have some data stoed.
   1. make sure to have a directory called `database` at the same level as `main.cpp`.
   2. have a camara ready to stream images to the program.
   3. it is best to have the background of the object to be white.
   4. run the main class and you show be able to see a red line / red box on the image. use `w` and `x` to adjust the position of the box to have it surrend the object you want to record.
   5. once you are there prese `n` and type in the name of the object that you think it is. A csv file with the same name would appear in the database directory.
2. Once you have all the data you wanted you can then start identifying objects.
   1. have the object you want to identiy under the camara and use `w` and `x` to adjust. 
   2. press `c` to use the scalar euclidean metric to identify what object it is; press `k` to use the k-nearest neighbor to identify what object is under the camara.
   3. you can still use `n` to store the image to the database.
   4. press `r` if you want to exit a mode and re-enter the other one.
3. You can test the accuray of this program by using the confusion matrix feature.
   1. have a set of objects/images ready, and input them into the actual vector.
   2. in order use the program to identify those object and input the result in the predicted vector.
   3. start the program again and press `f` you will have a `confusion_matrix.csv` in your folder.


### Directory Structure
<pre>
cs5330_project3_realtime_2d_object_recognition
├── confusion_matrix.cpp
├── confusion_matrix.csv
├── csv_util.cpp
├── csv_util.h
├── database
│   ├── expo.csv
│   ├── pencil.csv
│   └── sharpie.csv
├── dataprocessing.cpp
├── header.h
├── LICENSE
├── main
├── main.cpp
├── Makefile
├── preprocessing.cpp
├── Proj03Examples
│   └── __MACOSX
├── README.md
├── std.csv
└── training_data
</pre>

## Program features (matching mechanisms)

### Basic requirments

#### functionalities
- `w` and `x` - help to adjust the component mask over.
- `c` - use the scalar euclidean metric to identify what object it is
- `k` - use the k-nearest neighbor to identify what object is under the camara.
- `n` - store data into database
- `r` - to exist a filter or metric 
- `f` - to calculate confusion matrix

#### extention
- `s` - save the image with filters with costum name

#### Distance metric
- `scaled euclidean distance` - calculate the euclidean distance between the target object and the feature vectors in the database and divide it by the stardard deviation
- `k-nearest neighbor` - still using the scaled euclidean distance but when comparing compared to the sum of the 3 cloest feature vectors in the database.
   
## Demo Video
https://youtu.be/Zv9itBFmS8w