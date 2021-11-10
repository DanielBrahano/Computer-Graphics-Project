#define PI 3.14159265359
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x1 = p1.x; int x2 = p2.x; int y1 = p1.y; int y2 = p2.y; // i prefer using x1,x2,... in order to swap when needed
	bool mirrorFlag = false, swapFlag = false;// flags to decide if we need to mirror/swap between x and y
	int x = p1.x;
	int y = p1.y;
	int dx = (p2.x - p1.x);
	int dy = (p2.y - p1.y);
	int e = -(p2.x - p1.x);


	float a = (float)dy / dx;//cast the result to float
	int c = p1.y - a * p1.x;

	// if a<0 then treat it as positive and raise the flag
	if (a < 0) {
		mirrorFlag = true;
		a = a * (-1);
	}

	if (a < 1)// in cases when -1<a<1
	{
		if (mirrorFlag) {
			dy = dy * (-1);
		}
		if (x2 < x1) { // if x2>x1 then switch points
			int t = x1;
			x1 = x2;
			x2 = t;

			t = y1;
			y1 = y2;
			y2 = t;

			//update rest of values
			x = x1;
			y = y1;
			e = dx;
			dy = -dy;
			dx = -dx;

		}
		//go from x1 to x2 and decide where y values are going to be according to the algorithm
		while (x <= x2)
		{

			if (e > 0) {
				mirrorFlag ? y-- : y++;
				e = e - 2 * dx;
			}
			PutPixel(x, y, color);
			x++;
			e = e + 2 * dy;
		}
	}

	//else, cases when a>1 or a<1 so we need to run on y's
	else
	{
		e = -dy;
		if (mirrorFlag) {
			dx = dx * (-1);
		}
		//if y2<y1 then we need to swap
		if (y2 < y1) {
			int t = x1;
			x1 = x2;
			x2 = t;

			t = y1;
			y1 = y2;
			y2 = t;

			//update values
			x = x1;
			y = y1;
			e = dy;
			dy = -dy;
			dx = -dx;
		}

		//go from y1 to y2 and decide where x values are going to be according to the algorithm
		while (y <= y2)
		{
			if (e > 0) {
				mirrorFlag ? x-- : x++;
				e = e - 2 * dy;
			}
			PutPixel(x, y, color);
			y++;
			e = e + 2 * dx;
		}
	}
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	//get the number of faces in model
	int faceCounts = scene.GetModel(0).GetFacesCount();

	//run on all faces and print triangles
	for (int i = 0; i < faceCounts; i++)
	{
	
		glm::vec3 black{ 0,0,0 };

		//find index of each face
		int index1 = scene.GetModel(0).GetFace(i).GetVertexIndex(0)-1;
		int index2 = scene.GetModel(0).GetFace(i).GetVertexIndex(1)-1;
		int index3 = scene.GetModel(0).GetFace(i).GetVertexIndex(2)-1;

		//find actual vertices in homogeneous
		glm::vec4 p1{scene.GetModel(0).GetVertex(index1,0),scene.GetModel(0).GetVertex(index1,1),scene.GetModel(0).GetVertex(index1,2), 1.0f };
		glm::vec4 p2{scene.GetModel(0).GetVertex(index2,0),scene.GetModel(0).GetVertex(index2,1),scene.GetModel(0).GetVertex(index2,2), 1.0f };
		glm::vec4 p3{scene.GetModel(0).GetVertex(index3,0),scene.GetModel(0).GetVertex(index3,1),scene.GetModel(0).GetVertex(index3,2), 1.0f };

		//perform transformation on vertices
		 p1 = scene.GetModel(0).GetTransform()* p1;
		 p2 = scene.GetModel(0).GetTransform()* p2;
		 p3 = scene.GetModel(0).GetTransform()* p3;

		//draw triangle
		DrawTriangle(p1, p2, p3, black);
	}

}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

void Renderer::DrawTriangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec3 color)
{
	//2 dim for drawing triangles
	glm::vec2 q1 = p1;
	glm::vec2 q2 = p2;
	glm::vec2 q3 = p3;

	//draw triangles
	DrawLine(q1, q2, color);
	DrawLine(q1, q3, color);
	DrawLine(q2, q3, color);
}