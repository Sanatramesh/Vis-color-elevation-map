NOTE:
1. The data is cleaned with only salinity value and all the land values are maped to -999.
2. The data is read directly in the form of 90x60 grid.

Compilation Instructions
The program consists of two files:
1. 2dColorMap.cpp ----> It contains code for color map and contour.
   The color map are of two shade: Gray Scale and Green-Blue Shade

   To compile 2dColorMap.cpp:
   make colorMap

   and run using command:
  ./2dColorMap datafile

  To remove the binary file:
  make clear

      ##################### Useful Keys #####################
      z ---> Zoom In
      x ---> Zoom Out
      up arrow ---> translate up
      down arrow ---> translate down
      left arrow ---> translate left
      right arrow ---> translate down

2. Elevation3d.cc  ----> It contains code for elevation of color map and 5 contours

  To compile elevationMap.cpp:
  make elevationMap

  and run using command:
  ./elevationMap datafile

  To remove the binary file:
  make clear

      #################### Useful Keys ####################
      
      z ---> Zoom In
      x ---> Zoom Out
      up arrow ---> translate up
      down arrow ---> translate down
      left arrow ---> translate left
      right arrow ---> translate down

      mouse drag is used to rotate along x and y axis.

