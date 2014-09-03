# Photographic Mosaic
This was created for an assignment for Computer Vision (COSC450 at The University of Otago). 
## Requirements
- OpenCV library
- Image Database such as (Labeled Faces in the Wild)[http://vis-www.cs.umass.edu/lfw/] _included list_


## Usage
Compile using hte inclued Make file. Ensuring OpenCV is installed and the Make file is updated to point to the library.

mosaic <input image> <list of images> <tile size> <method> <output image>

1. Path to an image to be used for the mosaic
2. A text file that contains a path to your image resources
3. Tile size to be chosen i.e 16 ,8 or 4
4. Image picking method (average, RGBGrid, adaptive, random)
5. Path and name for the output image.


## Notes
First programme written in C++. Also first time use of OpenCV.
