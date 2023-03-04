
/*
 * Di Zhang
 * Feb 24, 2023
 * CS5330 - Computer Vision
 */

// library
#include <opencv2/opencv.hpp>

// header functions
#include "header.h"

// threshold image into foreground and background
int threshold(cv::Mat &src, cv::Mat &dest) {

    cv::Mat gus_blur_img;
    cv::Mat gray_img;
    // Gussian blur the image
    cv::GaussianBlur(src, gus_blur_img, cv::Size(5, 5), 0, 0);

    // convert color image to gray
    cv::cvtColor(gus_blur_img, gray_img, cv::COLOR_RGB2GRAY);

    // threshold image to binary image
    cv::threshold(gray_img, dest, 80, 255, 1);
    return 0;
}

// use morphological operators - grassfire transforem - to cleanup image
int grassfireTransform(cv::Mat &src, cv::Mat &dest, int morph) {

    // create data type for grayscale image
    typedef cv::Vec<uchar, 1> Vec1b;

    // initilize id number
    int id = 1;

    dest = cv::Mat::zeros(src.size(), CV_8UC1);

    // Pass 1
    // loop through all pixels from upper left to lower right 
    for (int i = 1; i < src.rows; i++) {

        // pointer to image id matrix
        Vec1b *dest_urowptr = dest.ptr<Vec1b>(i - 1);
        Vec1b *dest_crowptr = dest.ptr<Vec1b>(i);

        // pointer to image
        Vec1b *src_rowptr = src.ptr<Vec1b>(i);

        for (int j = 1; j < src.cols; j++) {
            // current pixel value
            int src_current = src_rowptr[j][0];

            // get id from ID matrix
            int dest_upper = dest_urowptr[j][0];

            int dest_back = dest_crowptr[j-1][0];

            // checking if the current pixel is foreground or background
            if (src_current == morph) {
                // if current pixel is background assign it to 0                
                dest_crowptr[j][0] = 0;

            } else {
                // assign pixel smallest neighber value
                int minval;
                minval = dest_upper < dest_back ? dest_upper : dest_back;
                dest_crowptr[j][0] = minval + 1;
            }
        }
    }

    // Pass 2
    // loop through all pixels from lower right to upper left 
    for (int i = src.rows - 1; i > 0; i--) {
        // pointer to image id matrix
        Vec1b *dest_drowptr = dest.ptr<Vec1b>(i + 1);
        Vec1b *dest_crowptr = dest.ptr<Vec1b>(i);

        // pointer to image
        Vec1b *src_rowptr = src.ptr<Vec1b>(i);

        // destination pointer
        Vec1b *dest_rowptr = dest.ptr<Vec1b>(i);

        for (int j = src.cols; j > 0; j--) {
            // current pixel value
            int src_current = src_rowptr[j][0];

            // get id from ID matrix
            int dest_down = dest_drowptr[j][0];
            int dest_front = dest_crowptr[j+1][0];

            // checking if the current pixel is foreground or background
            if (src_current == morph) {
                // if current pixel is background assign it to 0                
                dest_crowptr[j][0] = 0;
            } else {
                // assign pixel smallest neighber value
                int minval;
                int dest_current = dest_crowptr[j][0];
                minval = dest_down < dest_front ? dest_down : dest_front;
                minval = minval + 1;
                dest_crowptr[j][0] = dest_current < minval ? dest_current : minval;
            }
        }
    }
    return 0;
}

// use the grassfire transformed images to get rid of the noise
int cleanup (cv::Mat &src, cv::Mat &dest, char* morph) {
    // create data type for grayscale image
    typedef cv::Vec<uchar, 1> Vec1b;

    dest = cv::Mat::zeros(src.size(), CV_8UC1);

    // Pass 1
    // loop through all pixels from upper left to lower right 
    for (int i = 0; i < src.rows; i++) {

        // pointer to image
        Vec1b *src_rowptr = src.ptr<Vec1b>(i);

        // pointer to destination img
        Vec1b *dest_rowptr = dest.ptr<Vec1b>(i);

        for (int j = 0; j < src.cols; j++) {
            // get current pixel value
            int cur_pixel = src_rowptr[j][0];

            if (strcmp(morph, "dilation") == 0) { 
                if (cur_pixel < 15) {
                    dest_rowptr[j][0] = 255;
                } else {
                    dest_rowptr[j][0] = 0;
                }
            } else if (strcmp(morph, "erosion") == 0) {
                if (cur_pixel < 15) {
                    dest_rowptr[j][0] = 0;
                } else {
                    dest_rowptr[j][0] = 255;
                }
            }
        }
    }
    return 0;
}

// connected component analysis using the opencv method connectedComponentsWithStats
int displayComponents(cv::Mat& binaryImage, cv::Mat& outputImage, cv::Mat& labels, int components, int minSize) {
    // Apply connected component analysis to the binary image
    cv::Mat stats, centroids;
    int num_components = cv::connectedComponentsWithStats(binaryImage, labels, stats, centroids);

    components = num_components;

    // Create a random color map for visualization
    std::vector<cv::Vec3b> colors(num_components);
    colors[0] = cv::Vec3b(0, 0, 0);  // Background color
    for (int i = 1; i < num_components; i++) {
        colors[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
    }
    // Display the components that meet the minimum size requirement
    outputImage.create(binaryImage.size(), CV_8UC3);
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            int label = labels.at<int>(i, j);
            if (label != 0 && stats.at<int>(label, cv::CC_STAT_AREA) >= minSize) {
                outputImage.at<cv::Vec3b>(i, j) = colors[label];
            } else {
                outputImage.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    return 0;
}

// calculate the image/object features and save them into a vector
int featureComputation(cv::Mat& img, cv::Mat& binaryImg, cv::Mat& region_map, std::vector<double> &feature_vector, int component ) {

    // calculate the axis of least central moment

    cv::Mat label_mask = (region_map == component);

    // Compute the moments of the image
    cv::Moments moments = cv::moments(label_mask, true);

    // calculate the x-axis and y axis
    double x_bar = moments.m10 / moments.m00;
    double y_bar = moments.m01 / moments.m00;
    double theta = 0.5 * atan2(2 * moments.mu11, moments.mu20 - moments.mu02);

    cv::Point2f center(x_bar, y_bar);
    cv::Point2f endpoint1(center.x + std::cos(theta) * 100, center.y + std::sin(theta) * 100);
    cv::Point2f endpoint2(center.x - std::cos(theta) * 100, center.y - std::sin(theta) * 100);

    // calculate the humoments
    double hu[10];
    cv::HuMoments(moments, hu);

    // calculate orientated bounding box
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(label_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::RotatedRect box;
    for (int i = 0; i < contours.size(); i++)
    {
        box = cv::minAreaRect(contours[i]);
        cv::Point2f vertices[4];
        box.points(vertices);

        std::vector<cv::Point> box_points;
        for (int j = 0; j < 4; j++)
        {
            box_points.push_back(vertices[j]);
        }

        std::vector<std::vector<cv::Point>> box_contours;
        box_contours.push_back(box_points);

        cv::drawContours(img, box_contours, 0, cv::Scalar(0, 0, 255), 2);
    }
    double boxarea = box.size.width * box.size.height;
    double filledarea = cv::countNonZero(label_mask);
    double obb = filledarea / boxarea; 
    double hwratio = box.size.width / box.size.height;

    // drawing axis of the least central moment on the image
    cv::line(img, endpoint1, endpoint2, cv::Scalar(0, 0, 255), 2);
    // add the axis of the least central moment to the array
    hu[7] = theta;

    // add obb to the data array
    hu[8] = obb;

    // add hight width ration to the data array
    hu[9] = hwratio;

    int size = sizeof(hu)/sizeof(double);

    std::vector<double> vec(hu, hu + size);

    feature_vector = vec;

    return 0;
    
}