#pragma once
#ifndef FRCVISION_HPP
#define FRCVISION_HPP
#include <mutex>
#include "Threading/Updateable.hpp"
#include "Threading/ConcurrentStreamReader.hpp"
#include <chrono>
#include <math.h>
#include <string>
#include <vector>

#define COLOR_WHITE cv::Scalar(255, 255, 255)	//bgr color space...
#define COLOR_RED cv::Scalar(0, 0, 255)
#define COLOR_CYAN cv::Scalar(255, 255, 0)
#define COLOR_ORANGE cv::Scalar(0, 128, 255)

namespace ov {
    class FRCVision : public Updateable {
    public:
        FRCVision(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream)
            : m_inputStream(inputStream) {
            m_outputStream = std::make_shared<ConcurrentStream<cv::Mat>>();        
            //kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
            fpsA = std::vector<std::chrono::high_resolution_clock::time_point>(45, std::chrono::high_resolution_clock::now());
        }
          
        void update() {        
            m_inputStream.read(m_data);
            if(!m_data.empty()) {                
                c+=1;
                fpsA[c % 45] = std::chrono::high_resolution_clock::now();
                             
                double fps = 1000.0 * 45.0 / (std::chrono::duration_cast<std::chrono::milliseconds>(fpsA[c % 45] - fpsA[(unsigned)(c + 1) % 45]).count());
                
                
                auto start = std::chrono::high_resolution_clock::now();
                cv::Mat channels[3];
                std::vector<std::vector<cv::Point>> contours;
                std::vector<cv::Point> contour;
                std::vector<cv::Vec4i> hierarchy;
                std::vector<std::vector<cv::Point>> acceptedTargets; 
                std::vector<cv::Point> hull;
                  
                  
                /*
                Main speedup  
                inRange for only green
                No morphologyEx or blurs
                Requires more work in filtering targets(which is faster)
                */
                cv::split(m_data, channels);
                //cv::inRange(channels[1], cv::Scalar(100), cv::Scalar(255), channels[1]);
                cv::threshold(channels[1], channels[1], 100, 255, cv::THRESH_BINARY);
                //cv::inRange(m_data, cv::Scalar(0, 80, 0), cv::Scalar(64, 255, 64), fbw);
                //cv::morphologyEx(fbw, fbw, MORPH_OPEN, kernel); 

                auto finishedThresh = std::chrono::high_resolution_clock::now();
                cv::findContours(channels[1], contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

                auto finishedContours = std::chrono::high_resolution_clock::now();

                //Convex hull on contours       
                for(int i = 0; i < contours.size(); ++i) {
                    contour.clear();
                    hull.clear();
                    contour = contours[i];
                    /*
                    Filter based on size
                    */            
                    double contArea = cv::contourArea(contour);

                    if(contArea < 500) {
                        continue;
                    }
                    /*
                    Filter based on number of points
                    
                    Not accurate?
                    Doesn't work approx algorithm doesn't reduce to that little points
                    if(contour.size() > 8) {
                      continue;
                    }            
                    */
                    /*
                    Filter based on aspect ratio
                    */
                    cv::RotatedRect rect = cv::minAreaRect(contour);
                    double aspectRatio;
                    if(rect.size.width > rect.size.height) {
                        aspectRatio = rect.size.height / rect.size.width;
                    } else {
                        aspectRatio = rect.size.width / rect.size.height;
                    }
                    if(aspectRatio > 0.5 || aspectRatio < 0.3) {
                        if(isDebugging) {              
                            std::string status = "Aspect Ratio: " + std::to_string(aspectRatio);
                            cv::putText(m_data, status, rect.center, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);
                        }
                        continue;
                    }
                    /*
                    Filter based on fullness
                    */
                    cv::convexHull(contour, hull);
                    double convArea = cv::contourArea(hull);
                    double fullness = contArea / convArea;
                    if(fullness < 0.80 || fullness > 1.0) {              
                        if(isDebugging) {              
                            std::string status = "fullness: " + std::to_string(fullness);
                            cv::putText(m_data, status, rect.center, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);
                        }
                        continue;
                    }
                    
                    if(isDebugging) {              
                        std::string status = std::to_string(contArea) + ", " + std::to_string(aspectRatio) + ", " + std::to_string(fullness);
                        cv::putText(m_data, status, rect.center, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);
                    }
                    acceptedTargets.push_back(contour);
                }
                  
                auto finishedFiltering = std::chrono::high_resolution_clock::now();
                  /*
                  Draws are expensive only do for debugging
                  */
                if(isDebugging) {          
                    for(int i = 0; i < contours.size(); ++i) {
                        cv::drawContours(m_data, contours, i, COLOR_CYAN, 2);
                    }
                    
                    for(int i = 0; i < acceptedTargets.size(); ++i) {
                        cv::drawContours(m_data, acceptedTargets, i, COLOR_RED, 2);
                    }
                }
                auto finishedDrawing = std::chrono::high_resolution_clock::now();
                  
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finishedThresh - start);
                auto diff1 = std::chrono::duration_cast<std::chrono::milliseconds>(finishedContours - finishedThresh);
                auto diff2 = std::chrono::duration_cast<std::chrono::milliseconds>(finishedFiltering - finishedContours);        
                auto diff3 = std::chrono::duration_cast<std::chrono::milliseconds>(finishedDrawing - finishedFiltering);      
                auto diff4 = std::chrono::duration_cast<std::chrono::milliseconds>(finishedDrawing - start);

                char fpsStr[5];
                sprintf(fpsStr, "%.0f", fps);
                cv::putText(m_data, fpsStr, cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);

                if(isDebugging || isPrinting) {            
                    std::cout << diff.count() << "  " << diff1.count() << "  " << diff2.count() << "  " << diff3.count() << "  " << diff4.count() << "  num contours: " << std::to_string(contours.size()) << std::endl;   
                    std::cout << "fps: " << std::to_string(fps) << "  possible fps: " << std::to_string(1000.0 / diff4.count()) << std::endl;       
                }
                 
               
                m_outputStream->write(m_data.clone());
            }
        }
    
        std::shared_ptr<ConcurrentStream<cv::Mat>> getImgStream() {
            return m_outputStream;
        }

    
    private:

        bool isDebugging = false;
        bool isPrinting = true;
        unsigned int c = 0;
        cv::Mat m_data;
        ConcurrentStreamReader<cv::Mat> m_inputStream;
        std::shared_ptr<ConcurrentStream<cv::Mat>> m_outputStream;
        std::vector<std::chrono::high_resolution_clock::time_point> fpsA;
  
    };
}

#endif