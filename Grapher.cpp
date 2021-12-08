#include "Grapher.hpp"
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include <cstdio>

/**
 * The default comparator for graphable functions. Compares the values returned by the functions at mulptiple x values ranging from extremely small to extremely large. 
 */
bool default_graphable_comparer(const graphableFunc &f, const graphableFunc &g){
  double compare_list[] = {
    -999999999999,
    -999999999,
    -999999,
    -999,
    -99,
    -9,
    -1,
    -.1,
    -.01,
    -.0000000000001,
    0,
    .0000000000001,
    .01,
    .1,
    1,
    9,
    99,
    999,
    999999,
    999999999,
    999999999999
  };
  
  for( double x : compare_list ){
    auto fres = f(x);
    auto gres = g(x);
    
    if( fres.size() != gres.size() ) return false;
  
    for( double fy : fres ){
      bool in_g = false;
      for( double gy : gres ){
        if( fy == gy ){ in_g = true; break; } 
      }
      if( !in_g ) return false;
    }

  }
  
  return true;
}

int getAxis(double center, double size, int scrWH){
  int axis;
  if( (center - size) > 0 ){
    axis = 0;
  }else if( (center + size) < 0 ){
    axis = scrWH - 1;
  }else{
    double axispos = abs(center - size) / (abs(center+size) + abs(center-size)); 
    axis = roundf(axispos * (scrWH-1));
  }

  return axis;
}

Grapher::Grapher(int w, int h): width(w), height(h) {
  tex = LoadRenderTexture(width, height);
}

Grapher::Grapher() : width(GetScreenWidth()), height(GetScreenHeight()) {
  tex = LoadRenderTexture(width, height);
}

Grapher::~Grapher() {
  UnloadRenderTexture(tex);
}

void Grapher::clearDrawing() { 
  BeginTextureMode(tex);
  ClearBackground(RAYWHITE); 
  EndTextureMode();
}

/** Returns the index of the function given by comparing the function's output a various points. A different comparison function may be given.
 */
size_t Grapher::findfunc(const graphableFunc &fn, const function<bool(const graphableFunc &f, const graphableFunc &g)> &comp = default_graphable_comparer){
  for( auto it = funcs.begin(); it < funcs.end(); it++ ){
    if( comp(*it, fn) ){
      return it - funcs.begin();
    }
  }
  return -1;
}

void Grapher::drawAxes(double centerX, double centerY){
  
  if( drawn ) return;

  BeginTextureMode(tex);
  
  // Where on the screen the Axes should be drawn...
  int yAxis = getAxis(centerX, this->windowx, width),
      xAxis = getAxis(centerY, this->windowy, height); // Need to reflect over Y
  
  
  DrawLine(yAxis, 0, yAxis, height, BLACK);
  DrawLine(0, xAxis, width, xAxis, BLACK);
  

  if( this->graduations ){
    // X axis markings
    for( double x = ceil(centerX - this->windowx); x < (centerX + this->windowx); x += this->grad_freq ){
      double markerpos = abs(x - centerX + this->windowx) / (2 * this->windowx);
      int marker = markerpos * width;
      DrawLine(marker, xAxis-2, marker, xAxis+2, BLACK);
    }
    // Y axis markings
    for( double y = ceil(centerY - this->windowy); y < (centerY + this->windowy); y += this->grad_freq ){
      double markerpos = abs(y - centerY + this->windowy) / (2 * this->windowy);
      int marker = markerpos * height;
      DrawLine(yAxis-2, marker, yAxis+2, marker, BLACK);
    }
  }

  EndTextureMode();
}

void Grapher::drawFunc(double centerX, double centerY, GraphFunction func){
  
  for( int xpx = 0; xpx < width; xpx++ ){
    for( double xres = double(xpx) - 0.5; xres < (double(xpx) + 0.5); xres += (1.f / this->resolution) ){
      double xfact = xres / width;
      double x = (xfact * 2 * this->windowx) + centerX - this->windowx;
      vector<double> y = func(x);
      for( double pt : y ){
        if( pt > (centerY - this->windowy) && pt < (centerY + this->windowy) ){
          double ypos = abs(pt - centerY + this->windowy) / (2 * this->windowy);
          int ypt = ypos * height;
          DrawPixel(xpx, ypt, func.color);
        }
      }
    }
  }
}

void Grapher::drawFuncs(double centerX, double centerY) {
  if( drawn ) return;

  BeginTextureMode(tex);

  for( auto &fn : funcs ){
    drawFunc(centerX, centerY, fn);
  }

  EndTextureMode();
  
  drawn = true;
}

void Grapher::drawWidgets(double centerX, double centerY, double &zoomamt, double &moveamt, bool &reset, bool &zoomin, bool &zoomout){
  /* GUI Rectangles */
  const int panelW = 160;
  static const Rectangle UpArrow = {(width/2.f)-12, 0, 24, 24},
                         DownArrow = {(width/2.f)-12, width-24.f, 24, 24},
                         RightArrow = {width-24.f, (width/2.f)-12, 24, 24},
                         LeftArrow = {0, (width/2.f)-12, 24, 24},
                         ZoomOut = {0, 24, 72, 24},
                         ZoomIn = {0, 0, 72, 24},
                         XRange = {width*1.f, 72, 160, 24},
                         YRange = {width*1.f, 96, 160, 24};
  char txt[256];
  auto clrbuf = [](char txt[256]){ for( int i = 0; i < 256; i++ ) txt[i] = 0x00; };
  clrbuf(txt); 
  
  GuiPanel({(float)width, 0, panelW, (float)height});
  
  // Movement and Zoom sliders
  sprintf(txt, "Movement %%: %.3f", moveamt*100);
  moveamt = GuiSliderBar({width*1.f, 0, 160, 24}, nullptr, nullptr, moveamt, 0, 0.5);
  GuiLabel({width*1.f, 24, panelW, 12}, txt);
  clrbuf(txt);

  sprintf(txt, "Zoom %%: %.3f", zoomamt*100);
  zoomamt = GuiSliderBar({width*1.f, 36, panelW, 24}, nullptr, nullptr, zoomamt, 0, 0.5);
  GuiLabel({width*1.f, 60, panelW, 12}, txt);
  clrbuf(txt);

  // Reset Screen Button
  reset = GuiButton({width*1.f, height-24.f, panelW, 24}, "Reset Window");

  // Graph Info Labels
  sprintf(txt, "X Range: (%.2f, %.2f)", this->getCenterX()-this->getWindowX(), this->getCenterX()+this->getWindowX());
  GuiStatusBar(XRange, txt);
  clrbuf(txt);

  sprintf(txt, "Y Range: (%.2f, %.2f)", this->getCenterY()-this->getWindowY(), this->getCenterY()+this->getWindowY());
  GuiStatusBar(YRange, txt);
  clrbuf(txt);

}

void Grapher::draw(double centerX, double centerY){
  if( !drawn ){
    clearDrawing();
    drawAxes(centerX, centerY);
    drawFuncs(centerX, centerY);
  }
  
  DrawTexture(tex.texture, 0, 0, WHITE);

}
