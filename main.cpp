#include "Grapher.hpp"
#include "raylib.h"

#include "raygui.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdio>

#define DEFAULT_MOVE_AMT 0.1
#define DEFAULT_ZOOM_AMT 0.1

#define RICON_ZOOM_IN 200
#define RICON_ZOOM_OUT 201

void clearBuf(char buf[256], char clrval = 0x00){
  for( int i = 0; i < 256; i++ ){
    buf[i] = clrval;
  }
}

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
  
  /* Initialization */
  const int scrW = 640, scrH = 480, grfS = 480; 
  InitWindow(scrW, scrH, (argc > 1) ? argv[1] : "Graph of f(x)");
  
  /* Graph Object and Functions */
  Grapher graph(grfS, grfS);
  
  graph.addFunc(xcubed, MAGENTA);
  graph.addFunc(xsquared, MAROON);
  graph.addFunc(circle, BLUE);
  graph.addFunc(weird, RED);
  
  /* Set Timing */
  SetTargetFPS(60);
  
  /* Runtime Variables */
  char txbuf[256]; // C String for labels
  clearBuf(txbuf);
  double moveamt = DEFAULT_MOVE_AMT,
         zoomamt = DEFAULT_ZOOM_AMT;
  bool resetscr = false,
       zoomIn = false,
       zoomOut = false;
  // Counter to slow down repeated inputs
  unsigned long long framenum = 0;
  int frames_mouse_delay = 3;
  

  while( !WindowShouldClose() ){
    framenum++; 
    
    graph.checkInput(moveamt, zoomamt, resetscr, zoomIn, zoomOut, framenum, frames_mouse_delay);

    BeginDrawing();
    graph.draw(); // Graph integrated drawing function
    
    graph.drawWidgets(zoomamt, moveamt, resetscr, zoomIn, zoomOut);
    // DrawFPS(0,0);

    EndDrawing();

  }

}
