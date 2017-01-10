//
// Created by xbendik on 03.01.2017.
//

#ifndef PV021_PROJECT_IMAGEMAKER_H
#define PV021_PROJECT_IMAGEMAKER_H


#include <utility>
#include <vector>
#include <string>

class ImageMaker {
    std::vector<std::vector<double> > locations;
    double width;
    double height;
    int frame; //current frame number
    int scale_ratio;
    int sequence;
    std::vector<double> curr; //current location
    std::vector<double> curr_ns; //current location without scale

public:
    ImageMaker(double w = 500, double h = 300);
    void addLocation(std::vector<double> location);
    void renderInput(std::string path = "C:\\Users\\xbendik\\ClionProjects\\pv021_project\\outputs\\test.pgm");
    void renderProbs(std::string path, std::vector<std::vector<double> > points);
    double get_curr_x(){ return curr[0]; }
    double get_curr_y(){ return curr[1]; }
    void restart();
};


#endif //PV021_PROJECT_IMAGEMAKER_H
