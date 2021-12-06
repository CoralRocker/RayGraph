#pragma once
#include "raylib.h"
#include <functional>
#include <vector>

using namespace std;

class GraphFunction {
  public:
    
    GraphFunction(const function<vector<double>(double)> &f, Color clr) : fn(f), color(clr) {}
    vector<double> operator()(double x) { return fn(x); }
    
    function<vector<double>(double)> fn;
    Color color;
};

class Grapher {
  public:
    
    void addFunc(GraphFunction func) { funcs.push_back(func); }
    void addFunc(const function<vector<double>(double)> &f, Color clr = BLACK) { addFunc(GraphFunction(f, clr)); }

    void setCenter(double x, double y) { cx = x; cy = y; }
    void setCenterX(double x) { cx = x; }
    void setCenterY(double y) { cy = y; }
    void shiftCenter(double x, double y) { cx += x*windowx; cy += y*windowy; }
    void shiftCenterX(double x) { cx += x*windowx; }
    void shiftCenterY(double y) { cy += y*windowy; }


    void drawAxes(double centerX, double centerY);
    void drawAxes() { drawAxes(cx, cy); }

    void drawFuncs(double centerX, double centerY);
    void drawFuncs() { drawFuncs(cx, cy); }

    void drawFunc(double centerX, double centerY, GraphFunction func);
    void drawFunc(GraphFunction func) { drawFunc(cx, cy, func); }
    
    void zoom(double amt) { windowx += amt; windowy += amt; }
    void zoomX(double amt) { windowx += amt; }
    void zoomY(double amt) { windowy += amt; }
    void zoomPct(double pct) { windowx *= 1.f + (1.f-pct); windowy *= 1.f + (1.f-pct); }  
    

  private:  
    double cx = 0, cy = 0;

    double resolution = 100;
    double windowx = 10,
           windowy = 10;
    
    bool graduations = true;
    double grad_freq = 0.5;

    // Vector of function pointers of functions
    // of type
    // vector<double> func(double);
    vector<GraphFunction> funcs;
    

       
};
