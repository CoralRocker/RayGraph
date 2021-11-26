#include "Grapher.hpp"
#include "raylib.h"

#include <iostream>
#include <vector>

std::vector<double> xsquared(double x){
  return {x*x};
}

std::vector<double> xcubed(double x){
  return {x*x*x};
}



int main(int argc, char** argv) {
  
  const int scrW = 640, scrH = 480;
  
  InitWindow(scrW, scrH, (argc > 1) ? argv[1] : "Graph of f(x)");

  Grapher graph;
  
  double moveamt = 0.1;
  double cx = 0, cy = 0;

  graph.addFunc(xcubed);
  graph.addFunc(xsquared);

  SetTargetFPS(60);

  while( !WindowShouldClose() ){
    
    if( IsKeyDown(KEY_DOWN) ){
      cy += moveamt;
    }
    if( IsKeyDown(KEY_UP) ){
      cy -= moveamt;
    }
    if( IsKeyDown(KEY_RIGHT) ){
      cx += moveamt;
    }
    if( IsKeyDown(KEY_LEFT) ){
      cx -= moveamt;
    }
    if( IsKeyDown(KEY_S) ){
      graph.zoom(0.1);
    }
    if( IsKeyDown(KEY_W) ){
      graph.zoom(-0.1);
    }


    graph.setCenter(cx, cy);
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    graph.drawAxes(); 
    graph.drawFuncs();

    EndDrawing();

  }

}
