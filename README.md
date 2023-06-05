Image Processing Library

This project is aimed at implementing a basic image processing library in C++. The library will include functionality for image enhancement, such as noise reduction, sharpening, and applying effects, as well as image analysis, including measurements, shape detection, and object classification. The library can be utilized in various industries and academic fields that require image processing capabilities.

Image Representation as Matrices
This project focus on processing grayscale images, where each pixel can have a value between 0 and 255, representing shades of gray from black to white. The images will be represented as matrices, with each cell being referred to as a "pixel" and containing a value that represents the grayscale shade.

Implementation
The project consists of two main parts: the Matrix class and the Filters implementation.
Matrix Class (Matrix.h, Matrix.cc)
Files: Matrix.h, Matrix.cc
The Matrix class is responsible for creating and manipulating the matrices required for applying operators to the images. The data type used for the matrix elements will be float to handle division and multiplication operations with ease.

Filters Implementation (cc.Filters)
File: cc.Filters
Implementation three different operators related to image processing. The Filters.cc file depends on the Filters.h file provided in the model. 
In this section:
1. Noise Reduction: Implement a function that reduces noise in the image.
2. Sharpening: Implement a function that enhances the sharpness of the image.
3. Apply Effects: Implement a function that applies various effects to the image.

To use the image processing library, follow these steps:
1. Include the Matrix.h and Filters.h files in your C++ project.
2. Create an instance of the Matrix class to represent the image you want to process.
3. Use the provided functions in the Filters class to apply various operators to the image.
4. Save or display the processed image as desired.
