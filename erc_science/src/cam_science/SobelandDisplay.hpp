#include <iostream>
#include <ros/ros.h>
#include <opencv2/opencv.hpp> // import OpenCV library.
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/image_encodings.h>


bool new_frame_left = false;
bool new_frame_right = false;

cv::Mat frames_left;
cv::Mat frames_right;

cv::VideoWriter video_or_l("src/erc_science/src/cam_science/videos/Original_left.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(1280,720),true);
cv::VideoWriter video_yat_l("src/erc_science/src/cam_science/videos/Yataylar_left.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(1280,720),true);
cv::VideoWriter video_crack_l("src/erc_science/src/cam_science/videos/Cracks_left.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(1280,720),true);
cv::VideoWriter video_or_r("src/erc_science/src/cam_science/videos/Original_right.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(640,480),true);
cv::VideoWriter video_yat_r("src/erc_science/src/cam_science/videos/Yataylar_right.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(640,480),true);
cv::VideoWriter video_crack_r("src/erc_science/src/cam_science/videos/Cracks_right.avi", cv::VideoWriter::fourcc('M','J','P','G'), 25, cv::Size(640,480),true);

void sobel_and_display_left(cv::Mat src) // sobel_and_display function
{   int WIDTH, HEIGHT; // frame width and height variables
    WIDTH = 640;
    HEIGHT = 480;
    cv::Mat frame, gray_frame, canny; // frame and gray_frame declaration
    
    video_or_l.write(src);
    cv::namedWindow("Original Image",  CV_WINDOW_NORMAL); // create "Original Image" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Original Image", WIDTH, HEIGHT); // resize frame
    cv::imshow("Original Image",src); // show original frame
    
    cv::GaussianBlur(src,frame, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT); // make gaussian blur filter
    cv::cvtColor(frame,gray_frame,cv::COLOR_BGR2GRAY); // make gray filter on image
    cv::Mat sobely; // sobely gradiant
    cv::Sobel(gray_frame, sobely, -CV_16U, 0, 1,5); //get frames sobely gradient //-CV_16U veya -CV_64F beğenildi.
    cv::Canny(gray_frame,canny,50,100);

    double min_value, max_value; // max and min value for sobel filter
    cv::minMaxLoc(sobely, &min_value, &max_value); // Sobel Y filter
    //std::cout << "Min value: " << min_value << " Max value: " << max_value << std::endl; // print max and min value

    cv::Mat draw; // draw matrix
    sobely.convertTo(draw, CV_8U, 255.0/(max_value - min_value), -min_value * 255.0/(max_value - min_value)); // convert sobel matrix to draw matrix
    
    cv::namedWindow("Sobel Y",  CV_WINDOW_NORMAL); // create "Sobel Y" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Sobel Y", WIDTH, HEIGHT); // resize frame
    cv::imshow("Sobel Y", draw); // show original frame
    
    cv::cvtColor(draw,draw,cv::COLOR_GRAY2BGR);
    video_yat_l.write(draw);

    cv::namedWindow("Cracks",  CV_WINDOW_NORMAL); // create "Original Image" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Cracks", WIDTH, HEIGHT); // resize frame
    cv::imshow("Cracks",canny); // show original frame
    
    cv::cvtColor(canny,canny,cv::COLOR_GRAY2BGR);
    video_crack_l.write(canny);

}

void sobel_and_display_right(cv::Mat src) // sobel_and_display function
{   int WIDTH, HEIGHT; // frame width and height variables
    WIDTH = 640;
    HEIGHT = 480;
    cv::Mat frame, gray_frame, canny; // frame and gray_frame declaration
    
    video_or_r.write(src);
    cv::namedWindow("Original Image",  CV_WINDOW_NORMAL); // create "Original Image" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Original Image", WIDTH, HEIGHT); // resize frame
    cv::imshow("Original Image",src); // show original frame
    
    cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);

    cv::GaussianBlur(src,frame, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT); // make gaussian blur filter
    cv::cvtColor(frame,gray_frame,cv::COLOR_BGR2GRAY); // make gray filter on image
    cv::Mat sobely; // sobely gradiant
    cv::Sobel(gray_frame, sobely, -CV_16U, 0, 1,5); //get frames sobely gradient //-CV_16U veya -CV_64F beğenildi.
    cv::Canny(gray_frame,canny,50,100);

    double min_value, max_value; // max and min value for sobel filter
    cv::minMaxLoc(sobely, &min_value, &max_value); // Sobel Y filter
    //std::cout << "Min value: " << min_value << " Max value: " << max_value << std::endl; // print max and min value

    cv::Mat draw; // draw matrix
    sobely.convertTo(draw, CV_8U, 255.0/(max_value - min_value), -min_value * 255.0/(max_value - min_value)); // convert sobel matrix to draw matrix
    
    cv::namedWindow("Sobel Y",  CV_WINDOW_NORMAL); // create "Sobel Y" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Sobel Y", WIDTH, HEIGHT); // resize frame
    cv::imshow("Sobel Y", draw); // show original frame
    
    cv::cvtColor(draw,draw,cv::COLOR_GRAY2BGR);
    video_yat_r.write(draw);

    cv::namedWindow("Cracks",  CV_WINDOW_NORMAL); // create "Original Image" window created with  CV_WINDOW_NORMAL therefore window can be resizable 
    cv::resizeWindow("Cracks", WIDTH, HEIGHT); // resize frame
    cv::imshow("Cracks",canny); // show original frame

    cv::cvtColor(canny,canny,cv::COLOR_GRAY2BGR);
    video_crack_r.write(canny);
}


void camera_bridge_check_left(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    frames_left = cv_ptr->image;
    new_frame_left = true;
}

void camera_bridge_check_right(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    frames_right = cv_ptr->image;
    new_frame_right = true;
}