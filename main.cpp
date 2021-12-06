#include "Grapher.hpp"
#include "raylib.h"

#include <iostream>
#include <vector>
#include <cmath>

std::vector<double> xsquared(double x){
  return {x*x};
}

std::vector<double> xcubed(double x){
  return {x*x*x};
}

std::vector<double> circle(double x){
  double res = std::sqrt(4 - x*x);
  return {-res, res, res*x, -res*x};
}

std::vector<double> weird(double x){
  double result = std::log(x);

  return {result, -result, result*x, -x*result};
}

int main(int argc, char** argv) {
  
  const int scrW = 640, scrH = 480;
  
  InitWindow(scrW, scrH, (argc > 1) ? argv[1] : "Graph of f(x)");

  Grapher graph;
  
  double moveamt = 0.1;

  graph.addFunc(xcubed, MAGENTA);
  graph.addFunc(xsquared, MAROON);
  graph.addFunc(circle, BLUE);
  graph.addFunc(weird, RED);

  SetTargetFPS(60);

  while( !WindowShouldClose() ){
    
    if( IsKeyDown(KEY_DOWN) ){
      graph.shiftCenterY(moveamt);
    }
    if( IsKeyDown(KEY_UP) ){
      graph.shiftCenterY(-moveamt);
    }
    if( IsKeyDown(KEY_RIGHT) ){
      graph.shiftCenterX(moveamt);
    }
    if( IsKeyDown(KEY_LEFT) ){
      graph.shiftCenterX(-moveamt);
    }
    if( IsKeyDown(KEY_S) ){
      graph.zoomPct(0.90);
    }
    if( IsKeyDown(KEY_W) ){
      graph.zoomPct(1.10);
    }
 
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawFPS(0,0); 
    graph.drawAxes(); 
    graph.drawFuncs();

    EndDrawing();

  }

}
