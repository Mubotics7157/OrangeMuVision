#pragma once
#ifndef FRCVISION_HPP
#define FRCVISION_HPP
#include <mutex>
#include "Threading/Updateable.hpp"
#include "Threading/ConcurrentStreamReader.hpp"
#include <chrono>
#include <math.h>
#include <string>

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
        prevTime = std::chrono::high_resolution_clock::now();
      }
      
      void update() {        
			  m_inputStream.read(m_data);
        if(!m_data.empty()) {                
          auto cur = std::chrono::high_resolution_clock::now();
      		c+=1;
      		std::stringstream logLine;
          
          
          
          double deltaT = ((double)std::chrono::duration_cast<std::chrono::milliseconds>(cur-prevTime).count());
      		fpsA[c%15] = 1000.0/deltaT;
      		double fps = 0;
      		for(int i = 0; i < 15; i++) fps+=fpsA[i];
      		fps /= 15;
          prevTime = cur;
     
     
     
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
      		cv::putText(m_data, fpsStr, cv::Point(590, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);
         
          if(isDebugging || isPrinting) {            
            std::cout << diff.count() << "  " << diff1.count() << "  " << diff2.count() << "  " << diff3.count() << "  " << diff4.count() << std::endl;   
            std::cout << "fps: " << fpsStr << std::endl;       
          }
         
          /*
                    
      		vector<cv::Moments> hullMoments(hull.size()); //1st, 2nd and 3rd order moments of each hull
      		vector<cv::Point2d> centroids(hull.size());	//centroids of each hull
      		vector<double> angles(hull.size()); //angle from x axis of each hull
      		vector<cv::Point2d> targets; //centroids of each FRC vision target
      		vector<cv::Point2d> o_target(hull.size()); //normalized direction vector of each hull
      
      		for(int i = 0; i < hull.size(); i++) {
      			hullMoments[i] = cv::moments(hull[i], true);
      			//printf("good: %0.2f bad: %0.2f \n", t.m01, hullMoments[i].m01);
      			//end = std::chrono::high_resolution_clock::now();
      			centroids[i] = cv::Point2d(hullMoments[i].m10/hullMoments[i].m00, hullMoments[i].m01/hullMoments[i].m00);
      			//if(hullMoments[i].m00 < 10) ;
      			//calculate second order mu prime central moments
      			double mp11 = hullMoments[i].mu11/hullMoments[i].m00;
      			double mp20 = hullMoments[i].mu20/hullMoments[i].m00;
      			double mp02 = hullMoments[i].mu02/hullMoments[i].m00;
      
      			double y = 2.0*mp11;
      			double x = mp20-mp02;
      			
      			angles[i] = 0.5 * std::atan2( y, x );
      			if(y<0) angles[i] += M_PI; //keep angles in positive domain
      
      			o_target[i] = cv::Point2d(cos(angles[i]), sin(angles[i]));
      		}      
      
      		vector<vector<cv::Point2d> > projections; //projection vectors
      		vector<cv::Point> pairs; //indicies of each pair
      		for(int i = 0; i < hull.size(); i++) {
      			if(hullMoments[i].m00 < SMALL_PIXEL_CULL) continue; //magic numbers
      			vector<cv::Point2d> current;
      			for(int n = i+1; n < hull.size(); n++) {
      				cv::Point2d connector = centroids[n] - centroids[i]; //calculate line that passes two hulls
      				
      				//projections of hull vectors i and n on connector vector
      				cv::Point2d proj_i = (connector.ddot(o_target[i])/
      						 (connector.x*connector.x + connector.y*connector.y)) * connector; 
      				cv::Point2d proj_n = (connector.ddot(o_target[n])/
      						 (connector.x*connector.x + connector.y*connector.y)) * connector; 
      				proj_i = proj_i/magnitude(connector);
      				proj_n = proj_n/magnitude(connector);
      				current.push_back(proj_i);
      				current.push_back(proj_n); //TODO normalize projections
      				
      				//Verify & store pairs by checking that projections net zero and face the right way
      				cv::Point2d sum = proj_i + proj_n;
      				double mag = magnitude(sum);
      				//printf("mag of sum %f\n", mag);
      				double ratio = magnitude(connector)/sqrt(hullMoments[i].m00 + hullMoments[n].m00);
      				//printf("ratio %f\n", ratio);
      				//TODO choose shortest connector pair test
      				if(mag <= MAX_MAG_ERROR && connector.ddot(o_target[i]) < 0 
      				   && abs(ratio-SIZE_TO_DISTANCE_RATIO) <= MAX_SIZE_TO_DISTANCE_ERROR) {
      					
      					bool flag = false;
      					for(int z = 0; z < pairs.size(); z++) {
      						if(pairs[z].x == i || pairs[z].y == i) {
      							flag = true;
      							if(magnitude(connector) <= magnitude(centroids[pairs[z].x] - centroids[pairs[z].y])) {
      								pairs[z] = cv::Point(i, n);
      								targets[z] = (centroids[i] + centroids[n])/2;
      								//printf("hull size %n", hull[i].size());
      							} else {
      								break;
      							} 
      						}
      					} 
      					if(flag == false) {
      						pairs.push_back(cv::Point(i, n)); 
      						targets.push_back((centroids[i] + centroids[n])/2);
      					}
      				}
      			}
      			projections.push_back(current);	
          }
      		cv::Mat drawing = m_data;
      		for( int i = 0; i< contours.size(); i++ ) {
      			if(hullMoments[i].m00 < SMALL_PIXEL_CULL) continue;
   			    cv::drawContours(drawing, hull, i, COLOR_RED, 2);
      			cv::circle(drawing, centroids[i], 3, COLOR_CYAN, -1);
      			cv::line(drawing, centroids[i], cv::Point(centroids[i].x+20*cos(angles[i]), centroids[i].y+20*sin(angles[i])), COLOR_CYAN, 2);
      		}
      		cv::line(drawing, cv::Point(320, 0), cv::Point(320, 480), COLOR_WHITE, 2);
      		//draw pair connectors and centroids only if they are an actual pair
      		for(int n = 0; n < pairs.size(); n++) {
      			cv::line(drawing, centroids[pairs[n].x], centroids[pairs[n].y], COLOR_ORANGE, 2);
      			cv::circle(drawing, (centroids[pairs[n].x] + centroids[pairs[n].y])/2, 5, COLOR_ORANGE, -1);
      		}
      
      		for(int n = 0; n < pairs.size(); n++) {
      			float mag = magnitude(centroids[pairs[n].x] - centroids[pairs[n].y]);
      			float distance = 6232.6 * pow(mag, -1.084);
      			logLine << centroids[pairs[n].x].x << " " << centroids[pairs[n].x].y << " " 
      				<< centroids[pairs[n].y].x << " " << centroids[pairs[n].y].y << " "
      				<< hullMoments[pairs[n].x].m00 << " " << hullMoments[pairs[n].y].m00 << "\n";
      			char dist[10];
      			sprintf(dist, "%.2f", distance);
      			cv::putText(drawing, dist, (centroids[pairs[n].x] + centroids[pairs[n].y])/2, cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_WHITE, 2, cv::LINE_AA);
      			//log(logLine.str());
      		}
           */
      
          /*
      		char hullCnt[5]; 
      		sprintf(hullCnt, "%d", (int)hull.size());
          cv::putText(drawing, hullCnt, cv::Point(590, 400), cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_RED, 2, cv::LINE_AA);
          */
          m_outputStream->write(m_data.clone());
        }
      }
    
		std::shared_ptr<ConcurrentStream<cv::Mat>> getImgStream() {
			return m_outputStream;
		}

    
    private:
  
      bool isDebugging = false;
      bool isPrinting = true;
      std::chrono::high_resolution_clock::time_point prevTime;
      int c = 0;
      cv::Mat m_data;
		  ConcurrentStreamReader<cv::Mat> m_inputStream;
		  std::shared_ptr<ConcurrentStream<cv::Mat>> m_outputStream;
      double fpsA[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  };
}

#endif