#include "Grapher.hpp"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
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
  GuiLoadIcons("GrapherIcons.rgi", false);
  
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

  /* GUI Rectangles */ 
  Rectangle UpArrow = {(grfS/2.f)-12, 0, 24, 24},
            DownArrow = {(grfS/2.f)-12, grfS-24, 24, 24},
            RightArrow = {grfS-24, (grfS/2.f)-12, 24, 24},
            LeftArrow = {0, (grfS/2.f)-12, 24, 24},
            ZoomOut = {0, 24, 72, 24},
            ZoomIn = {0, 0, 72, 24},
            XRange = {grfS, 72, scrW - grfS, 24},
            YRange = {grfS, 96, scrW - grfS, 24};
  

  while( !WindowShouldClose() ){
    framenum++; 
    
    // Check for graph movement
    if( IsKeyDown(KEY_DOWN) || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), DownArrow)) ){
      graph.shiftCenterY(-moveamt);
    }
    if( IsKeyDown(KEY_UP) || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), UpArrow)) ){
      graph.shiftCenterY(moveamt);
    }
    if( IsKeyDown(KEY_RIGHT) || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), RightArrow)) ){
      graph.shiftCenterX(moveamt);
    }
    if( IsKeyDown(KEY_LEFT) || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), LeftArrow)) ){
      graph.shiftCenterX(-moveamt);
    }

    // Check for zoom in/out
    if( IsKeyDown(KEY_S)  || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), ZoomOut)) ){
      graph.zoomPct(1.f - zoomamt);
    }
    if( IsKeyDown(KEY_W) || ((framenum%frames_mouse_delay) == 0 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), ZoomIn)) ){
      graph.zoomPct(1.f + zoomamt);
    }
    
    // Reset Screen Values
    if( resetscr ){
      graph.setCenter(0, 0);
      graph.resetZoom();
      moveamt = DEFAULT_MOVE_AMT;
      zoomamt = DEFAULT_ZOOM_AMT;
      resetscr = false;
    }

    BeginDrawing();
    graph.draw(); // Graph integrated drawing function
    // DrawFPS(0,0);
    
    graph.drawWidgets(zoomamt, moveamt, resetscr, zoomIn, zoomOut);
    // GuiPanel({grfS, 0, scrW - grfS, scrH});

    // // Movement and Zoom sliders
    // sprintf(txbuf, "Movement %%: %.3f", moveamt*100);  
    // moveamt = GuiSliderBar({grfS, 0, scrW - grfS, 24}, nullptr, nullptr, moveamt, 0, 0.5);
    // GuiLabel({grfS, 24, scrW - grfS, 12}, txbuf);
    // clearBuf(txbuf);

    // sprintf(txbuf, "Zoom %%: %.3f", zoomamt*100);
    // zoomamt = GuiSliderBar({grfS, 36, scrW - grfS, 24}, nullptr, nullptr, zoomamt, 0, 0.5);
    // GuiLabel({grfS, 60, scrW - grfS, 12}, txbuf);
    // clearBuf(txbuf);
    // 
    // // Reset Screen Button
    // resetscr = GuiButton({grfS, scrH-24, scrW - grfS, 24}, "Reset Window");
    // 
    // // Graph Info Labels
    // sprintf(txbuf, "X Range: (%.2f, %.2f)", graph.getCenterX()-graph.getWindowX(), graph.getCenterX()+graph.getWindowX());
    // GuiStatusBar(XRange, txbuf);
    // clearBuf(txbuf);

    // sprintf(txbuf, "Y Range: (%.2f, %.2f)", graph.getCenterY()-graph.getWindowY(), graph.getCenterY()+graph.getWindowY());
    // GuiStatusBar(YRange, txbuf);
    // clearBuf(txbuf);
    // 
    // // Zoom Buttons
    // GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

    // if( GuiButton(ZoomIn, GuiIconText(RICON_ZOOM_IN, "zoom in")) ){
    //   graph.zoomPct(1.f + zoomamt);
    // }
    // if( GuiButton(ZoomOut, GuiIconText(RICON_ZOOM_OUT, "zoom out")) ){
    //   graph.zoomPct(1.f - zoomamt);
    // }
    // GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);


    // // Arrow icons
    // // Up Arrow
    // if( GuiButton(UpArrow, GuiIconText(RICON_ARROW_UP, "")) ){
    //   graph.shiftCenterY(moveamt);
    // } 
    // 
    // // Down Arrow
    // if( GuiButton(DownArrow, GuiIconText(RICON_ARROW_DOWN, "")) ){
    //   graph.shiftCenterY(-moveamt);
    // } 
    // 
    // // Right Arrow
    // if( GuiButton(RightArrow, GuiIconText(RICON_ARROW_RIGHT, "")) ){
    //   graph.shiftCenterX(moveamt);
    // }

    // // Left Arrow
    // if( GuiButton(LeftArrow, GuiIconText(RICON_ARROW_LEFT, "")) ){
    //   graph.shiftCenterX(-moveamt);
    // } 

    EndDrawing();

  }

}
