# RayGraph
A RayLib/RayGUI based graphing tool. More of an exersize than any real practical, item but its fun. My goal was to provide a usable interface for a "graphing widget" if you will. It allows normal C/C++ functions to be plotted.

# Documentation
## Classes 
### GraphFunction 
Holds the information for displaying a function. 
#### Constructor
`GraphFunction(const std::function<std::vector<double>(double)> &f, Color clr)`
#### Attributes and Methods:
#### `std::function<std::vector<double>(double)> fn`

The function held by this object. It must be of the form:

```C++
std::vector<double> yourFunctionName( double ) { ... }
```

#### `Color color`
A Raymath `Color` object. Line is drawn in this color by the drawing functions.

#### `std::vector<double> operator()(double)`
Calls the held function `fn` and returns its output.

### Grapher
Holds functions and draws them with raylib.
#### Constructors
`Grapher(void)`
Creates a grapher with the entire screen as its domain.

`Grapher(int width, int height);`
Creates a grapher with a width and height determined by the respective parameters. Graphs in the top left corner of the screen with a menu on the right of it.

#### Methods
```c++
void addFunc(GraphFunction func);
void addFunc(const std::function<std::vector<double>(double)> &f, Color clr = BLACK);
```
Adds a function from either a `GraphFunction` object and a function by itself or with a color. Does not check for duplicate functions.

```c++
void setCenter(double x, double y);
void setCenterX(double x);
void setCenterY(double y);
```
Set the center point to graph from. This determines where on the cartesian plane the graph will be drawn. The `setCenterX` and `setCenterY` functions modify only the x or y coordinates, while `setCenter` modifies both.

```c++
void shiftCenter(double x, double y);
void shiftCenterX(double x);
void shiftCenterY(double y);
```
Shifts the center of the plot by the percentages given.

```c++
void drawAxes(double centerX, double centerY);
void drawAxes(void);
```
Draw the axes either with a specified center or with the center for the graph. Axes tha are out of bounds are drawn at the limits of the screen.

```c++
void drawFuncs(double centerX, double centerY);
void drawFuncts(void);
```
Draws all the functions held in the grapher object. If given a center to draw off of, all the functions draw based on that center.

```c++
void draw(double centerX, double centerY);
void draw(void);
```
Draws the entire graph, including axes. Includes optimizations to allow for less frequent drawing of functions.

```c++
void zoom(double amt);
void zoomX(double amt);
void zoomY(double amt);
void zoomPct(double amt);
void resetZoom();
```
The `zoom`, `zoomX`, `zoomY`, and `zoomPct` functions add or remove from the drawing range. `zoomPct` zooms in or out by a percentage of the current domain, while the others modify the domain by a specified amount in the X or Y dimension.



