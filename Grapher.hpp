#pragma once
#include <functional>
#include <vector>

using namespace std;

class Grapher {
  public:
    
    void addFunc(function<vector<double>(double)> func) { funcs.push_back(func); }

    void setCenter(double x, double y) { cx = x; cy = y; }
    void setCenterX(double x) { cx = x; }
    void setCenterY(double y) { cy = y; }

    void drawAxes(double centerX, double centerY);
    void drawAxes() { drawAxes(cx, cy); }

    void drawFuncs(double centerX, double centerY);
    void drawFuncs() { drawFuncs(cx, cy); }

    void drawFunc(double centerX, double centerY, function<vector<double>(double)> func);
    void drawFunc(function<vector<double>(double)> func) { drawFunc(cx, cy, func); }
    
    void zoom(double amt) { windowx += amt; windowy += amt; }
    void zoomX(double amt) { windowx += amt; }
    void zoomY(double amt) { windowy += amt; }

  private:  
    double cx = 0, cy = 0;

    double resolution = 10;
    double windowx = 10,
           windowy = 10;
    
    bool graduations = true;
    double grad_freq = 0.5;

    // Vector of function pointers of functions
    // of type
    // vector<double> func(double);
    vector< function< vector<double>(double)>> funcs;
    
     
};
