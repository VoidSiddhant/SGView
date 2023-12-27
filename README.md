<h3> SGView </h3>
Image processing framework built in C++. This framework uses external libraries like OpenImageIO for I/O image file operations, supporting wide set of extensions and Glut/OpenGL for rendering image pixels onto the screen. Project is for linux only and will require extra work for cross compilation on windows system (due to external library dependency). Project compiles into a static library called myLib.a which is then linked to the main program for building final application using the framework. Please check "makefile". Rendered images can be saved by pressing the "j" key.

<h3>Julia Set Generation : </h3>

![julia 0001](https://github.com/VoidSiddhant/SGView/assets/25640729/082924c5-e519-4408-ac59-4f8d3c15b613)

![julia 0002](https://github.com/VoidSiddhant/SGView/assets/25640729/2e627643-fb3e-4ce7-852d-aedde5cd1d95)

<h3> Histogram Operation : </h3>
Original Image

![hist](https://github.com/VoidSiddhant/SGView/assets/25640729/460ccb5b-1191-4f6a-8f0e-51ba3af0c414)

Histogram Equalized Image

![saveFile_0](https://github.com/VoidSiddhant/SGView/assets/25640729/45bec095-a5a2-45d5-b1d6-2c4d967c0628)

Contrast Units computation

![contrast](https://github.com/VoidSiddhant/SGView/assets/25640729/051898c9-37cc-4dd3-b354-e21544933716)

