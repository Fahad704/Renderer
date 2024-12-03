# Renderer

This is a C++ Application used for rendering using Ray tracer or Rasterizer , i started this project to learn the graphics pipeline and the windows API. It can Render any 3d obj file with light and soon it will support material files.
I made this Project following Gabriel Gambetta's "Computer Graphics From scratch" and some of my own stuff

## Installation

Currently there is no proper Build system but there is a batch file that has g++ flags and there is Visual studio project file in the repo

and the project only works in windows 

if you have iconv and windres
```bash
cd src
Build.bat
```
if you only have g++ 
```bash
cd src
build-no-icon.bat
```
## Usage
"R" - To toggle ray tracing/Rasterization(Ray tracing not recommanded , only use ray tracing when the poly count is below 100)<br>
"W A S D" - to move<br>
"Space and Shift" to move up and down<br>
Press "G" to Lock/Unlock mouse<br>
"V" - turn off wireframe view<br>
"B" - to see bounding boxes<br>
"C" - to toggle backface culling<br>
"P" - to export current frame to ppm file<br>
"T" - to turn on wireframe mode<br>
"Q" - to reset position and rotation of camera to 0,0,0<br>
"L" - to view model(s) details<br>
"F" - to toggle anti aliasing (FXAA)<br>
"ESC" - to close<br>
# Renders
### Ray tracer <br>
#### <span style="color;">Ray traced white king<br></span>
![WhiteKing](https://github.com/user-attachments/assets/d1ca4791-e568-4340-90c2-9f07e56d71ed)
<br>
#### Ray traced spheres<br>
![RayTracerReflection](https://github.com/user-attachments/assets/7bfc2390-8e0e-4a74-9dea-2c2b927a367c)<br>
### Rasterizer
#### King rendered with light, coloured according to its surface normal<br>
![KingColorNorm](https://github.com/user-attachments/assets/39916a7d-256b-4a05-9f69-a9e6daded15a)<br>
#### Same king rendered as triangle wireframe
![KingTrisNorm](https://github.com/user-attachments/assets/32c47d44-8504-477a-a5ba-feb240a973cd)<br>
