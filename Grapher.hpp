#pragma once
#include "raylib.h"
#include <functional>
#include <vector>

#define DEFAULT_ZOOM 10

using namespace std;

typedef function<vector<double>(double)> graphableFunc;

class GraphFunction {
  public:
    
    GraphFunction(const graphableFunc &f, Color clr) : fn(f), color(clr) {}
    vector<double> operator()(double x) { return fn(x); }
    
    graphableFunc fn;
    Color color;
};

class Grapher {
  public:
    
    Grapher();
    Grapher(int, int);
    ~Grapher();
    
    size_t findfunc(const graphableFunc &fn, const function<bool(const graphableFunc&, const graphableFunc&)> &comp);

    void addFunc(GraphFunction func) { drawn = false; funcs.push_back(func); }
    void addFunc(const graphableFunc &f, Color clr = BLACK) { drawn = false; addFunc(GraphFunction(f, clr)); }
    void addFunc(const function<double(double)> &f, Color clr = BLACK) { drawn = false; graphableFunc lambda = [=](double x)->vector<double>{ return {f(x)}; }; addFunc(lambda, clr); }

    void setCenter(double x, double y) { drawn = false; cx = x; cy = y; }
    void setCenterX(double x) { drawn = false; cx = x; }
    void setCenterY(double y) { drawn = false; cy = y; }
    void shiftCenter(double x, double y) { drawn = false; cx += x*windowx; cy += y*windowy; }
    void shiftCenterX(double x) { drawn = false; cx += x*windowx; }
    void shiftCenterY(double y) { drawn = false; cy += y*windowy; }


    void drawAxes(double centerX, double centerY);
    void drawAxes() { drawAxes(cx, cy); }

    void drawFuncs(double centerX, double centerY);
    void drawFuncs() { drawFuncs(cx, cy); }
    
    void drawWidgets(double centerX, double centerY, double &zoomamt, double &moveamt, bool &reset, bool &zoomin, bool &zoomout);
    void drawWidgets(double &zoomamt, double &moveamt, bool &reset, bool &zoomin, bool &zoomout) { drawWidgets(cx, cy, zoomamt, moveamt, reset, zoomin, zoomout); }

    void draw(double centerX, double centerY);
    void draw() { draw(cx, cy); }
    
    void resetZoom() { drawn = false; windowx = DEFAULT_ZOOM; windowy = DEFAULT_ZOOM; }
    void zoom(double amt) { drawn = false; windowx += amt; windowy += amt; }
    void zoomX(double amt) { drawn = false; windowx += amt; }
    void zoomY(double amt) { drawn = false; windowy += amt; }
    void zoomPct(double pct) { drawn = false; windowx *= 1.f + (1.f-pct); windowy *= 1.f + (1.f-pct); }  
    
    double getCenterX() { return cx; }
    double getCenterY() { return cy; }
    double getWindowX() { return windowx; }
    double getWindowY() { return windowy; }

  private:  
    void clearDrawing(); 
    
    void drawFunc(double centerX, double centerY, GraphFunction func);
    void drawFunc(GraphFunction func) { drawFunc(cx, cy, func); }
    
    bool drawn = false;

    const int width, height;
    double cx = 0, cy = 0;

    double resolution = 100;
    double windowx = DEFAULT_ZOOM,
           windowy = DEFAULT_ZOOM;
    
    bool graduations = true;
    double grad_freq = 0.5;

    // Vector of function pointers of functions
    // of type
    // vector<double> func(double);
    vector<GraphFunction> funcs;
    
    RenderTexture2D tex;
       
};
