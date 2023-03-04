
/*
 * Di Zhang
 * Feb 24, 2023
 * CS5330 - Computer Vision
 */

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

// create confusion matrix on input data actual / predicted 
int confusion_matrix() {
    // define the test data
    //0: pencil
    //1: sharpie
    //2: expo

    // scaled euclidean distance
    //std::vector<int> actual = { 0, 0, 0, 1, 1, 1, 2, 2, 2};
    //std::vector<int> predicted = { 0, 0, 0, 1, 1, 2, 2, 2, 2};
    
    // k - nearest neighbor 
    std::vector<int> actual = { 0, 0, 0, 1, 1, 1, 2, 2, 2};
    std::vector<int> predicted = { 1, 0, 0, 1, 2, 1, 2, 1, 2};
    int num_classes = 3;

    // initialize the confusion matrix
    cv::Mat confusionMatrix = cv::Mat::zeros(num_classes, num_classes, CV_32S);

    // iterate over the test data and update the confusion matrix
    for (int i = 0; i < actual.size(); i++) {
        confusionMatrix.at<int>(actual[i], predicted[i])++;
    }

    // print the confusion matrix to a CSV file
    std::ofstream outfile("confusion_matrix.csv");
    if (outfile.is_open()) {
        // print the header row
        outfile << "Actual\\Predicted,";
        for (int i = 0; i < num_classes; i++) {
            outfile << i << ",";
        }
        outfile << std::endl;

        // print the matrix rows
        for (int i = 0; i < num_classes; i++) {
            outfile << i << ",";
            for (int j = 0; j < num_classes; j++) {
                outfile << confusionMatrix.at<int>(i, j) << ",";
            }
            outfile << std::endl;
        }

        outfile.close();
    } else {
        std::cout << "Error: could not open file for writing" << std::endl;
    }

    return 0;
}