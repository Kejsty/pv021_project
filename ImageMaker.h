//
// Created by xbendik on 03.01.2017.
//

#ifndef PV021_PROJECT_IMAGEMAKER_H
#define PV021_PROJECT_IMAGEMAKER_H


#include <utility>
#include <vector>
#include <string>

class ImageMaker {
    std::vector<std::pair<double,double>> locations;
    double width;
    double height;
    double curr_x;
    double curr_y;

public:
    ImageMaker(double w = 1500, double h = 800);
    void add_location(double x, double y);
    void render_input(std::string path = "C:\\Users\\xbendik\\ClionProjects\\pv021_project\\outputs\\test.pgm");
    double get_curr_x(){ return curr_x; }
    double get_curr_y(){ return curr_y; }
};


#endif //PV021_PROJECT_IMAGEMAKER_H
