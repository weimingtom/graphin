# Introduction #

Graphin application programming interface is a set of plain C functions defined in graphin.h file. Design goal is to provide as compact as possible set of primitives.

# List of functions #

## Bitmap functions ##

  * image\_create
  * image\_release
  * image\_get\_info
  * image\_blit

## Graphics initialization ##

  * graphics\_create
  * graphics\_release

  * graphics\_rgbt

## Basic shapes ##

  * graphics\_line
  * graphics\_triangle
  * graphics\_rectangle
  * graphics\_rounded\_rectangle
  * graphics\_ellipse
  * graphics\_arc
  * graphics\_star

## Path primitives ##

  * graphics\_open\_path
  * graphics\_move\_to
  * graphics\_line\_to
  * graphics\_hline\_to
  * graphics\_vline\_to
  * graphics\_arc\_to
  * graphics\_ellipse\_to
  * graphics\_quadratic\_curve\_to
  * graphics\_bezier\_curve\_to
  * graphics\_close\_path
  * graphics\_draw\_path

## Affine transformations ##

  * graphics\_rotate
  * graphics\_translate
  * graphics\_scale
  * graphics\_skew
  * graphics\_transform

## State ##

  * graphics\_state\_save
  * graphics\_state\_restore

## Drawing attributes ##

  * graphics\_line\_color
  * graphics\_fill\_color

  * graphics\_line\_linear\_gradient
  * graphics\_fill\_linear\_gradient

  * graphics\_line\_radial\_gradient
  * graphics\_fill\_radial\_gradient

  * graphics\_fill\_mode

## Text drawing ##

  * graphics\_font
  * graphics\_text

  * graphics\_text\_width
  * graphics\_text\_alignment
  * graphics\_font\_metrics

## Bitmap rendering ##

  * graphics\_draw\_image
  * graphics\_blit\_image
  * graphics\_blend\_image