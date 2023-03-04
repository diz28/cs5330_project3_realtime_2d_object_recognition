/*
 * Di Zhang
 * Feb 24, 2023
 * CS5330 - Computer Vision
 */
// library
#include <opencv2/opencv.hpp>
#include <vector>
#include <filesystem>
#include <list>

// header functions
#include "csv_util.h"

// function for calculating the standard diviation
int calc_std() {
    // initialize some variables
    std::vector<char *> filenames;
    std::vector<std::vector<double>> data;
    std::vector<double> mean_vec(10, 0.0);
    std::vector<double> std_vec(10, 0.0);
    char standard_dev[] = "standard_dev";
    char csv_std[] = "std.csv";
    std::string folder_path = "database";

    int num_data = 0;
    // loop through the database 
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {    
        std::string file_path = entry.path().string();
        char *f_path = &file_path[0];
        read_image_data_csv(f_path, filenames, data, 0);
        // loop through each row in a data csv file
        for (int i = 0; i < data.size(); i++) {
            // loop through each feature in a single row of data.
            for (int j = 0; j < data[i].size(); j++) {
                double mean = (num_data * mean_vec[j] + data[i][j]) / (num_data + 1);
                double diff = data[i][j] - mean;
                double st_d = 0.0;
                if (num_data != 0) {
                    st_d = ((num_data / (num_data + 1)) * std_vec[j]) + ((diff * diff) / num_data);
                } 
                mean_vec[j] = mean;
                std_vec[j] = st_d;
            }
            num_data += 1;
        }

    }
    // save the calculated standard deviation
    append_image_data_csv(csv_std, standard_dev, std_vec, 1);
    return 0;
}

// calculate scaled euclidean distances
int scaled_euclidean(std::vector<double>& target_vector, std::vector<std::pair<char*, double>>& result_vector) {
    // initialize some variables
    std::vector<char *> filenames;
    std::vector<char *> std_filenames;    
    char std_csv[] = "std.csv";
    std::vector<std::vector<double>> data;
    std::vector<std::vector<double>> std_data;
    read_image_data_csv(std_csv, std_filenames,std_data , 0);

    std::string folder_path = "database";
    // loop through the database 
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {    
        std::string file_path = entry.path().string();
        char *f_path = &file_path[0];
        // read the data from teh csv files
        read_image_data_csv(f_path, filenames, data, 0);
        for (int i = 0; i < data.size(); i++) {
            double euclidean_distance = 0.0;
            // loop through each row in a data csv file
            for (int j = 0; j < data[i].size(); j++) {
                double scaled_diff = (target_vector[j] - data[i][j]) / std_data[0][j];
                euclidean_distance = scaled_diff * scaled_diff;
            }
            // save the calculated euclidean distance to a vector and sort them to get the shortest distance
            result_vector.push_back(std::make_pair(filenames[i], euclidean_distance));
        }
    }
    return 0;
}

// calculate k-nearest neighbor
int k_nearest_neighbor(std::vector<double>& target_vector, std::vector<std::pair<char*, double>>& k_nearest_vector) {
    // initialize some variables
    char std_csv[] = "std.csv";
    std::vector<char *> std_filenames;    
    std::vector<std::vector<double>> std_data;
    // read data from the data csv files
    read_image_data_csv(std_csv, std_filenames, std_data, 0);
    std::vector<char *> filenames;
    std::vector<std::vector<double>> data;
    std::string folder_path = "database";
    // loop through the csv files in the database
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {        
        // initialize some variables
        std::string file_path = entry.path().string();
        char *f_path = &file_path[0];
        std::vector<double> euclidean_vec;
        read_image_data_csv(f_path, filenames, data, 0);
        double k_nearest_distance = 0.0;
        // loop through every row of data
        for (int i = 0; i < data.size(); i++) {
            double euclidean_distance = 0.0;
            // loop through every feature in one row of data and calculate the scaler euclidean difference
            for (int j = 0; j < data[i].size(); j++) {
                double scaled_diff = (target_vector[j] - data[i][j]) / std_data[0][j];
                euclidean_distance = scaled_diff * scaled_diff;
            }
            // save the calculated euclidean distance for later use
            euclidean_vec.push_back(euclidean_distance);
        }

        std::sort(euclidean_vec.begin(), euclidean_vec.end());

        for (int i = 0; i < 3; i++) {
            k_nearest_distance += euclidean_vec[i];
        }
        k_nearest_vector.push_back(std::make_pair(filenames[2], k_nearest_distance));

        // clear out the vectors for the next loop
        euclidean_vec.clear();
        euclidean_vec.resize(0);
        data.clear();
        data.resize(0);
        filenames.clear();
        filenames.resize(0);
    }
    return 0;
}