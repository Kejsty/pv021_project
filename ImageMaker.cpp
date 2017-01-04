//
// Created by xbendik on 03.01.2017.
//

#include <fstream>
#include <iostream>
#include "ImageMaker.h"

ImageMaker::ImageMaker(double w, double h): width(w), height(h), curr_x(0), curr_y(0){}

void ImageMaker::add_location(double x, double y) {
    int ratio = 500;
    curr_x += x * ratio;
    curr_y += y * ratio;
    //std::cout << "Move:" << x * ratio << " " << y * ratio << std::endl;
    locations.push_back(std::pair<double,double>(curr_x, curr_y));
}


void ImageMaker::render_input(std::string path) {
    std::cout << "Render:" << std::endl;

    int offset = 100; //starting position is (offset,offset)
    int x = 0, y = 0;
    //build and fill canvas
    std::vector<std::vector<int>> canvas(height, std::vector<int>(width,1)); //1 is white, 0 is black
    for(auto& it : locations){
        x = int(it.first) + offset;
        y = int(it.second) + offset;
        if(x >= 0 and x < width and y >= 0 and y < height) {
            canvas[y][x] = 0;
        }
    }

    //create PGM
    std::ofstream outfile (path);
    outfile << "P2" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "1" << std::endl;

    for(auto& it : canvas){
        for(auto& it2: it){
            outfile << it2 << " ";
        }
        outfile << std::endl;
    }

    outfile.close();
    return;
}