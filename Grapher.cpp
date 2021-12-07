#include "Grapher.hpp"
#include "raylib.h"
#include "raymath.h"

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

void Grapher::draw(double centerX, double centerY){
  if( !drawn ){
    clearDrawing();
    drawAxes(centerX, centerY);
    drawFuncs(centerX, centerY);
  }
  
  DrawTexture(tex.texture, 0, 0, WHITE);

}
