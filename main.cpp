
/*
 * Di Zhang
 * Feb 24, 2023
 * CS5330 - Computer Vision
 */

// library
#include <opencv2/opencv.hpp>
#include <filesystem>

// header functions
#include "header.h"
#include "csv_util.h"

// helper function to sort all matric values
bool sortByValue(const std::pair<char *, float> &a,
                 const std::pair<char *, float> &b)
{
    return a.second < b.second;
}

// main funciton
int main(int argc, char *argv[])
{
    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened())
    {
        printf("Unable to open video device\n");
        return (-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;

    std::vector<std::pair<char *, double>> result_vector;
    
    int components = 1;
    for (;;)
    {
        char csv_folder[512] = "database/";

        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty())
        {
            std::cout << "Could not open input file" << std::endl;
            return -1;
        }

        // thresholding image
        cv::Mat binary_img;
        threshold(frame, binary_img);

        // clean up image using morphologial operators
        cv::Mat dilation_img;
        cv::Mat d_cleanup_img;
        cv::Mat erosion_img;
        cv::Mat e_cleanup_img;
        char dilation[] = "dilation";
        char erosion[] = "erosion";
        // clean up images
        grassfireTransform(binary_img, dilation_img, 255);
        cleanup(dilation_img, d_cleanup_img, dilation);
        grassfireTransform(d_cleanup_img, erosion_img, 0);
        cleanup(erosion_img, e_cleanup_img, erosion);

        // display regions
        // connected component analysis on processed image
        cv::Mat cca_img, labels;
        int num_components;
        displayComponents(e_cleanup_img, cca_img, labels, num_components, 1000);

        std::vector<double> feature_vector;
        // help the main function to get into different modes
        char checker;

        // take in exit command
        char key = cv::waitKey(10);

        // adjust the components to mask over
        if (key == 'w') {
            components += 1;
        }
        if (key == 'x') {
            components -= 1;
        }

        // compute the features of images
        featureComputation(frame, e_cleanup_img, labels, feature_vector, components);

        // save the features of image to csv file
        if (key == 'n')
        {
            char *object_name = new char[256];
            // ask user to enter the name of the object
            std::cout << "Enter the category of the object: ";
            std::cin.getline(object_name, 256);
            std::cout << std::endl;
            // make the object name to the classifing name for later processing
            std::strcat(csv_folder, object_name);
            std::strcat(csv_folder, ".csv");

            // store data into csv file
            append_image_data_csv(csv_folder, object_name, feature_vector, 0);
            calc_std();
        }

        // 2 different distance metrics k-nearest neighbor and scaled euclidean distance
        if (key == 'c' || key == 'k' || checker == 'c' || checker == 'k')
        {
            // Define the font properties
            cv::Point text_pos1(50, 50);
            cv::Point text_pos2(50, 100);
            cv::Scalar font_color(255, 255, 255);
            int font_face = cv::FONT_HERSHEY_SIMPLEX;
            double font_scale = 1;
            int thickness = 2;

            if (key == 'c' || checker == 'c')
            {
                scaled_euclidean(feature_vector, result_vector);
                cv::putText(frame, "scaled euclidean distance", text_pos2, font_face, font_scale, font_color, thickness);
                checker = 'c';
            }
            if (key == 'k' || checker == 'k')
            {
                k_nearest_neighbor(feature_vector, result_vector);
                cv::putText(frame, "k-nearest neighbor", text_pos2, font_face, font_scale, font_color, thickness);
                checker = 'k';
            }
            // sort the distance between target image and images in database
            sort(result_vector.begin(), result_vector.end(), sortByValue);

            // get the sort image name in a separate vector
            std::vector<char *> sorted_classname;
            for (auto &p : result_vector)
            {
                sorted_classname.push_back(p.first);
            }

            // Add text to the image
            cv::putText(frame, sorted_classname[0], text_pos1, font_face, font_scale, font_color, thickness);
        }
        // reset checker/exit modes
        if (key == 'r')
        {
            checker = '\0';
            result_vector.clear();
            result_vector.resize(0);
        }

        // get the binary image of frame
        if (key == 'b' || checker == 'b') {
            frame = binary_img;
            checker = 'b';
        }
        // get the cleaned up image of frame
        if (key == 'e' || checker == 'e') {
            frame = e_cleanup_img;
            checker = 'e';
        }
        // get the image with separate regions
        if (key == 'm' || checker == 'm') {
            frame = cca_img;
            checker = 'm';
        }
        // to save the image to file
        if (key == 's') {
            cv::imwrite("capture.png", frame);
            std::string filename;
            std::string png = ".png";
            while (true) {
                std::cout << "Enter image name: ";
                std::getline(std::cin, filename);
                int result = rename("capture.png", (filename + png).c_str());
                if (result == 0) {
                    break;
                }
            }
        }

        cv::imshow("Video", frame);

        // quit program on q
        if (key == 'q' || key == 'Q')
        {
            cv::destroyAllWindows();
            break;
        }
        // to get the print the confusion matrix to csv file
        if (key == 'f') {
            confusion_matrix();
        }   
    }
    return 0;
}