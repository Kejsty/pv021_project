//
// Created by xbendik on 03.01.2017.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "ImageMaker.h"

ImageMaker::ImageMaker(double w, double h): width(w), height(h), curr({0,0}), scale_ratio(300), frame(1) {}

void ImageMaker::addLocation(std::vector<double> location) {
    curr[0] += location[0] * scale_ratio;
    curr[1] += location[1] * scale_ratio;
    curr_ns = location;
    locations.push_back(curr);
}


void ImageMaker::renderInput(std::string path) {
    int offset = 100; //starting position is (offset,offset)
    int x = 0, y = 0;
    //build and fill canvas
    std::vector<std::vector<int>> canvas(height, std::vector<int>(width,255)); //1 is white, 0 is black
    for(auto& it : locations){
        x = int(it[0]) + offset;
        y = int(it[1]) + offset;
        //std::cout << "x,y: " << x << " " << y << std::endl;
        if(x >= 0 and x < width and y >= 0 and y < height) {
            canvas[y][x] = 200;
        }
    }

    //create PGM
    std::ofstream outfile (path);
    outfile << "P2" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    for(auto& it : canvas){
        for(auto& it2: it){
            outfile << it2 << " ";
        }
        outfile << std::endl;
    }

    outfile.close();
    return;
}

void ImageMaker::renderProbs(std::string path, std::vector<std::vector<double> > points){
    if(path == "") {
        std::stringstream p;
        p << "C:\\Users\\xbendik\\ClionProjects\\pv021_project\\outputs\\prob\\prob_";
        p << frame << "_" << (int(curr[0]) + 100) << "_" << (int(curr[1]) + 100) << ".ppm";
        path = p.str();
    }

    int offset = 100; //starting position is (offset,offset)
    int x = 0, y = 0;
    std::vector<std::vector<std::vector<int> > > canvas (height, std::vector<std::vector<int> >(width, std::vector<int>(3,255)));

    //calculate color scale
    double max = points[0][2];
    double min = max;
    for(auto& it : points){
        if(max < it[2])
            max = it[2];
        if(min > it[2])
            min = it[2];
    }
    double one = max - min;

    //prob of next points, the blacker the more probable
    int color = 0;
    for(auto& it : points){
        x = int(it[0] * scale_ratio) + int(curr[0]) + offset;
        y = int(it[1] * scale_ratio) + int(curr[1]) + offset;
        color = 255 - int(((it[2] - min)/one) * 255);
        if(x >= 0 and x < width and y >= 0 and y < height) {
            canvas[y][x] = {color, color, color};
        }
    }

    //underlying text
    for(auto& it : locations){
        x = int(it[0]) + offset;
        y = int(it[1]) + offset;
        if(x >= 0 and x < width and y >= 0 and y < height) {
            canvas[y][x] = {255,0,0};
        }
    }

    //mark current location (blue color)
    canvas[int(curr[1]) + offset][int(curr[0]) + offset] = {0,0,255};

    //create PPM
    std::ofstream outfile (path);
    outfile << "P3" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    for(auto& it : canvas){
        for(auto& it2: it){
            for(auto& color: it2) {
                outfile << color << " ";
            }
        }
        outfile << std::endl;
    }

    outfile.close();
    frame++;
    return;
}