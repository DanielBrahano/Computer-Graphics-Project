#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>//include for mat operations
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetSize(int width, int height);


private:
	void PutPixel(const int i, const int j, const glm::vec3& color);

	//assignment 1a, drawline
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	//draw world and model ccordinates fucntions
	void DrawWorldCoordinates(Scene scene, int j);
	void DrawLocalCoordinates(Scene scene, int j);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	float* z_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;

	bool** bool_array;
	int offset_x;
	int offset_y;
	bool paintFlag;
	bool paint_triangle;
	bool gray_scale;
	bool color_with_buffer;


	


	//drawing triangles
	void DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles, glm::vec3 rectangle_color, Scene scene);
	//drawing mesh using triangles
	void DrawMesh(Scene scene, int j);

	//canonical view volume to screen coordinates
	void viewport(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float height);

	void viewport(glm::vec3& p1, float height);

	void undo_viewport(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float height);
	void undo_viewport(glm::vec3& p1, float height);

	void DrawBoundingBox(Scene scene, MeshModel model);

	void DrawNormal(Scene scene, MeshModel model);

	void DrawFaceNormal(Scene scene, MeshModel model);

	glm::vec3 HomToCartesian(glm::vec4 vec);

	void TransformationMultiplications(Scene scene, MeshModel model, glm::vec4 p);

	int* DrawBoundingRectangleForTriangles(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles);

	void PaintTriangle(int rows, int cols, glm::vec3 color, bool paint_triangle, bool gray_scale, bool color_with_buffer, float zFar);

	void CreateBoolArray(bool** bool_array);

	float CalculateArea(glm::vec3& q1, glm::vec3& q2, glm::vec3& q3);

	float Find_z(int _x, int  point_y, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	void Set_z(int i, int j, float z);

	float Get_z(int i, int j);

	void Set_ColorBuffer(int i, int j, glm::vec3 v);

	glm::vec3 Get_ColorBuffer(int i, int j);

	void Renderer::DrawLight(Scene scene, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int faceNumber);

	glm::vec3 compute_normal(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);

	glm::vec3 Renderer::InterpolatedVec(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 position, glm::vec3 normal1, glm::vec3 normal2, glm::vec3 normal3);

};