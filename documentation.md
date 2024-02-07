# Software Renderer Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [Custom Data Types](#custom-data-types)
   - [Vertex](#vertex)
   - [Point2D](#point2d)
   - [VertexNormal](#vertex-normal)
   - [UVVector](#uvvector)
   - [FaceElement](#face-element)
   - [WaveFrontData](#wavefront-data)
   - [Vec2](#vec2)
   - [Vec3](#vec3)
4. [Main Classes](#usage)
   - [Parser Class](#parser-class)
   - [Renderer Class](#renderer-class)
   - [Model Class](#model-class)
   - [TGAColor Class](#tgacolor-class)
   - [TGAImage Class](#tgaimage-class)

## 1. Introduction

Welcome to the documentation for the Software Renderer project! This document provides an in-depth understanding of the main classes within the project, offering insights into their functionalities and usage. If you are new to the project, we recommend checking the [README](README.md) for basic information and instructions on how to use the program.

This documentation is intended to serve as a comprehensive guide for developers, users, and contributors. Each class is documented with details on their responsibilities, methods, and example usages. Read on to explore the inner workings of the Software Renderer and learn how to leverage its capabilities for your graphics rendering needs.

## 2. Architecture
**Simple Flowchart:**
Obj file to be rendered --> Model --> Parser --> Renderer --> TGAImage

In this basic flowchart:
- The OBJ file to be rendered is processed by the `Model` class.
- The `Model` class utilizes the `PARSER` to extract and organize data.
- The organized data is then passed to the `Renderer` for drawing and rendering.
- The final rendered image is represented by the `TGAImage` class.


## 3. Custom Data Types

- **Vertex**: Represents a 3D point in space with coordinates (x, y, z).

- **VertexNormal**: Represents a 3D vector for normal data with coordinates (x, y, z).

- **UVVector**: Represents a 2D vector for texture mapping with coordinates (x, y).

- **FaceElement**: Represents an element of a 3D face with indices for vertex, texture, and normal.

- **WaveFrontData**: Holds data extracted from a Wavefront OBJ file.

- **Vec2**: Represents a 2D vector with integer coordinates. And cross product function.

- **Vec3**: Represents a 3D vector with float coordinates. Also has cross/dot product functions and normalize function.


## 4. Main Classes
The main classes in the Software Renderer play distinct roles in the rendering process. Explore each class to understand their functionalities and how they contribute to the overall rendering pipeline.

#### Class: `Parser`

The `Parser` class provides static methods for handling the parsing of Wavefront OBJ files.

##### Responsibilities:

- Handle the parsing of Wavefront OBJ files.
- Extract data from the files and manipulate the data for further processing.

##### Public Methods:

- `static WaveFrontData parseObjFile(std::string &filename)`: Parses the specified OBJ file and returns the extracted model data.

- `static void printDataInfo(WaveFrontData &data)`: Prints information about the parsed data, such as the number of vertices, normals, UV vectors, and faces.

- `static void printNthVertex(WaveFrontData &data, int n)`: Prints the coordinates of the nth vertex in the parsed data.

- `static void printNthFace(WaveFrontData &data, int n)`: Prints the indices of the nth face in the parsed data.

- `void normalizeVectors(WaveFrontData &data)`: Instance method to normalize vectors in the parsed data.

##### Private Methods:

- `static bool isValidFile(std::string &filename)`: Checks whether the specified file is a valid OBJ file.


### [Renderer Class](#renderer-class)

#### Class: `Renderer`

The `Renderer` class provides functions for drawing and rendering simpler objects like lines and triangles. It also includes storage for helper functions needed for rendering smaller objects.

##### Responsibilities:

- Handle the drawing and rendering of lines and triangles.
- Project vertices onto the 2D screen space.
- Calculate barycentric coordinates for points within triangles.
- Implement various triangle drawing functions, including flat shading, z-buffering, and textured rendering.
- Store helper functions required for rendering smaller objects.

##### Public Methods:

- `void drawLine(const Vertex& v1, const Vertex& v2, const TGAColor& color, TGAImage& image)`: Draws a line between two vertices with the specified color on the given image.

- `void projectVerts(int width, int height, std::unordered_map<int, Vertex>& vertices)`: Projects vertices onto the 2D screen space based on the provided width and height. So they are compatible with the image specification.

- `Vec3 barycentric(const Vertex &v1, const Vertex &v2, const Vertex &v3, const Vertex &p)`: Calculates barycentric coordinates for a point within a triangle defined by three vertices.

- `void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const TGAColor& color, TGAImage& image)`: Draws a filled triangle with the specified color on the given image.

- `void drawTriangleZ(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<float>& zbuffer, const TGAColor& color, TGAImage& image)`: Draws a filled triangle with z-buffering for depth handling.

- `void drawTriangleTextureZ(Vertex& v1, Vertex& v2, Vertex& v3, UVVector& uv1, UVVector& uv2, UVVector& uv3, float& intensity, std::vector<float>& zbuffer, TGAImage& texture, TGAImage& image)`: Draws a textured triangle with z-buffering, taking into account texture coordinates and intensity.

#### Class: `Model`

The `Model` class encapsulates functionality for rendering 3D models and supports various rendering options. And is the most high level. Basicly the render starter.

##### Responsibilities:

- Load and store 3D model data from a Wavefront OBJ file.
- Draw the model using different rendering techniques, including lines, colorful triangles, shadows, and textured rendering.
- Manage textures and z-buffer for enhanced rendering effects.
- Print the z-buffer for debugging purposes.

##### Public Methods:

- `Model(int width, int height, std::string filename)`: Constructor to initialize the model with the specified width, height, and OBJ file.

- `void drawModelLinesOnly(TGAImage &image, TGAColor &color)`: Draws the model using only lines with the specified color on the given image.

- `void drawModelColorfulTriangles(TGAImage &image)`: Draws the model with colorful filled triangles on the given image.

- `void drawModelWithShadows(TGAImage &image, Vec3 lightDirection, bool useZBuffer)`: Draws the model with shadows based on the provided light direction. Optionally uses z-buffer for depth handling.

- `void drawModelWithTexture(TGAImage &image, Vec3 lightDirection, bool useZBuffer)`: Draws the model with texture mapping, considering the provided light direction. Optionally uses z-buffer for depth handling.

- `TGAImage texture`: Holds the texture image for texture mapping.

- `std::vector<float> zBuffer`: Stores the z-buffer for depth handling.

- `void printZBuffer(const std::vector<float> zBuffer, int width, int height)`: Prints the z-buffer for debugging purposes.

##### Private Methods:

- `void loadTexture(std::string filename, TGAImage &image)`: Loads the texture from the specified file and assigns it to the `texture` member.

- `int width`: The width of the rendered image.

- `int height`: The height of the rendered image.

- `std::unordered_map<int, Vertex> originalVertices`: Stores the original vertices of the model.

- `WaveFrontData objData`: Holds the parsed data from the Wavefront OBJ file.


### Class: `TGAColor`

The `TGAColor` class represents a color in a TGA image.

#### Responsibilities

- Store RGBA color information.
- Provide methods for accessing and manipulating color values.
- Support different constructors for initializing color instances.

#### Constructors

- `TGAColor()`: Default constructor initializes the color to black with full transparency.
- `TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A)`: Constructor with explicit RGBA values.
- `TGAColor(int v, int bpp)`: Constructor with packed value and bytes per pixel.
- `TGAColor(const TGAColor &c)`: Copy constructor.
- `TGAColor(const unsigned char *p, int bpp)`: Constructor from raw data.

#### Methods

- `TGAColor & operator =(const TGAColor &c)`: Assignment operator.

---

### Class: `TGAImage`

The `TGAImage` class represents a TGA image and provides various operations on it.

#### Responsibilities

- Store image data and metadata.
- Read and write TGA files.
- Perform image manipulation operations such as flipping and scaling.
- Provide methods for accessing and modifying pixel values.

#### Fields

- `unsigned char* data`: Image data buffer.
- `int width`: Width of the image.
- `int height`: Height of the image.
- `int bytespp`: Bytes per pixel.

#### Constructors

- `TGAImage()`: Default constructor creates an empty image.
- `TGAImage(int w, int h, int bpp)`: Constructor with width, height, and bytes per pixel.
- `TGAImage(const TGAImage &img)`: Copy constructor.

#### Methods

- `bool read_tga_file(const char *filename)`: Read a TGA image from the specified file.
- `bool write_tga_file(const char *filename, bool rle=true)`: Write the image to a TGA file, optionally using RLE compression.
- `bool flip_horizontally()`: Flip the image horizontally.
- `bool flip_vertically()`: Flip the image vertically.
- `bool scale(int w, int h)`: Scale the image to the specified width and height.
- `TGAColor get(int x, int y)`: Get the color at the specified coordinates.
- `bool set(int x, int y, TGAColor c)`: Set the color at the specified coordinates.
- `~TGAImage()`: Destructor.
- `TGAImage & operator =(const TGAImage &img)`: Assignment operator.
- `int get_width()`: Get the width of the image.
- `int get_height()`: Get the height of the image.
- `int get_bytespp()`: Get the bytes per pixel.
- `unsigned char *buffer()`: Get the image data buffer.
- `void clear()`: Clear the image data.

