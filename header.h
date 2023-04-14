/*
 * Di Zhang
 * Feb 24, 2023
 * CS5330 - Computer Vision
 */

// header file

// threshold image into foreground and background
int threshold(cv::Mat &src, cv::Mat &dest);

// use morphological operators to cleanup image
int grassfireTransform(cv::Mat& binaryImage, cv::Mat& outputImage, int morph);

// use the grassfire transformed images to get rid of the noise
int cleanup (cv::Mat &src, cv::Mat &dest, char* morph);

// connected component analysis using the opencv method connectedComponentsWithStats
int displayComponents(cv::Mat& binaryImage, cv::Mat& outputImage, cv::Mat& labels, int num_components, int minSize);

// calculate the image/object features and save them into a vector
int featureComputation(cv::Mat& img, cv::Mat& binary_img, cv::Mat& labels, std::vector<double> &feature_vector,int component);

// calculate the standard deviation 
int calc_std();

// calculate scaled euclidean distances
int scaled_euclidean(std::vector<double>& target_vector, std::vector<std::pair<char*, double>>& result_vector);

// calculate k-nearest neighbor
int k_nearest_neighbor(std::vector<double>& target_vector, std::vector<std::pair<char*, double>>& k_nearest_vector);

// calculat ethe confusion matrix
int confusion_matrix();