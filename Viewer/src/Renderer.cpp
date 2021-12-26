#define PI 3.14159265359
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>
#include <algorithm> 


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
	paint_triangle = false;
	gray_scale = false;
	color_with_buffer = false;
	paintFlag = false;

	//array for 2d bolean
	bool_array = new bool* [(viewport_width + 1)];
	for (int i = 0; i < viewport_width + 1; i++)
		bool_array[i] = new bool[(viewport_height + 1)];

}

Renderer::~Renderer()
{
	delete[] color_buffer;
	delete[] z_buffer;

	//Delete the array created
	for (int i = 0; i < viewport_width+1; i++)  //To delete the inner arrays
		delete[] bool_array[i];
	delete[] bool_array;             //To delete the outer array
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
			//if ((paint_triangle || gray_scale || color_with_buffer) && paintFlag && (x <= viewport_width) && (y <= viewport_height) && x >= 0 && y >= 0)
			if ( (x <= viewport_width) && (y <= viewport_height) && x >= 0 && y >= 0)
				bool_array[x][y] = true;
			if(!paintFlag)
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
			//if ((paint_triangle || gray_scale || color_with_buffer) && paintFlag && (x <= viewport_width) && (y <= viewport_height) && x >= 0 && y >= 0)
			if ( (x <= viewport_width) && (y <= viewport_height) && x >= 0 && y >= 0)
				bool_array[x][y] = true;
			if(!paintFlag)
				PutPixel(x, y, color);
			y++;
			e = e + 2 * dx;
		}
	}
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[(3 * w * h)];
	z_buffer = new float[w * h];
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
			z_buffer[Z_INDEX(viewport_width, i, j)] = INFINITY;
		}

	}
}	

void Renderer::Render(Scene& scene)
{

	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	paint_triangle = scene.paint_triangles;
	gray_scale = scene.gray_scale;
	color_with_buffer = scene.color_with_buffer;
	//for every model in the scene do 
	for (int j = 0; j < scene.GetModelCount(); j++) {

		DrawMesh(scene, j);

		//if world/model checkbox is checked then draw axes
		if (scene.GetModel(j).DrawWorldAxes)
		{
			DrawWorldCoordinates(scene, j);
		}
		if (scene.GetModel(j).DrawModelAxes)
		{
			DrawLocalCoordinates(scene, j);
		}
		//check if draw box
		if (scene.draw_box)
			DrawBoundingBox(scene, scene.GetModel(j));

		//check if draw normals
		if (scene.draw_normals)
			DrawNormal(scene, scene.GetModel(j));

		//check if draw normals
		if (scene.draw_face_normals)
			DrawFaceNormal(scene, scene.GetModel(j));


	}

}

void Renderer::DrawMesh(Scene scene, int j)
{
	//get the number of faces in model
	int faceCounts = scene.GetModel(j).GetFacesCount();

	//array of colors
	vector<glm::vec3> colors;
	colors.push_back(glm::vec3(0, 0, 1)); colors.push_back(glm::vec3(0, 1, 0)); colors.push_back(glm::vec3(0, 1, 1));
	colors.push_back(glm::vec3(1, 0, 0)); colors.push_back(glm::vec3(1, 0, 1)); colors.push_back(glm::vec3(1, 1, 0));
	colors.push_back(glm::vec3(1, 1, 1));

	int number_of_colors = 7;



	//run on all faces and print triangles
	for (int i = 0; i < faceCounts; i++)
	{
		glm::vec3 black{ 0,0,0 };

		//find index of each face
		int index1 = scene.GetModel(j).GetFace(i).GetVertexIndex(0) - 1;
		int index2 = scene.GetModel(j).GetFace(i).GetVertexIndex(1) - 1;
		int index3 = scene.GetModel(j).GetFace(i).GetVertexIndex(2) - 1;

		//find actual vertices in homogeneous
		glm::vec4 p1{ scene.GetModel(j).GetVertex(index1,0),scene.GetModel(0).GetVertex(index1,1),scene.GetModel(j).GetVertex(index1,2), 1.0f };
		glm::vec4 p2{ scene.GetModel(j).GetVertex(index2,0),scene.GetModel(0).GetVertex(index2,1),scene.GetModel(j).GetVertex(index2,2), 1.0f };
		glm::vec4 p3{ scene.GetModel(j).GetVertex(index3,0),scene.GetModel(0).GetVertex(index3,1),scene.GetModel(j).GetVertex(index3,2), 1.0f };

		//apply matrices multiplications
		p1 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).GetTransform() * p1;
		p2 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).GetTransform() * p2;
		p3 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).GetTransform() * p3;

		//go to cartesian coordinates
		glm::vec3 q1 = HomToCartesian(p1);
		glm::vec3 q2 = HomToCartesian(p2);
		glm::vec3 q3 = HomToCartesian(p3);

		//screen coordinates
		viewport(q1, q2, q3, min(viewport_height, viewport_width));

		glm::vec3 rectangle_color = colors[i % number_of_colors];
		DrawTriangle(q1, q2, q3, black, scene.bounding_rectangles, rectangle_color, scene);

		if (scene.ambient_shading)
			DrawLight(scene);
	}
	

}

void Renderer::SetSize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}


void Renderer::DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles, glm::vec3 rectangle_color, Scene scene)
{

	//the function drawing the rectangles if needed and returns 2d array
	int* dims = DrawBoundingRectangleForTriangles(p1, p2, p3, rectangle_color, bounding_rectangles);

	if (paint_triangle || gray_scale || color_with_buffer)
		paintFlag = true;
		//initialize to false
		for (int i = offset_x-1; i < dims[0] + offset_x+1; i++)
			for (int j = offset_y-1; j < dims[1] + offset_y+1; j++)
				if ((i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0)
					bool_array[i][j] = false;
	
	//draw triangles
	DrawLine(p1, p2, color);
	DrawLine(p1, p3, color);
	DrawLine(p2, p3, color);

		//run through the bool array and if we need to paint the pixel accourding to scanline, change value to true

		for (int i = offset_x; i <= dims[0] + offset_x; i++)
		{
			bool beginFlag = false;
			int begin = 0;
			int end = 0;
			for (int j = offset_y; j <= dims[1] + offset_y; j++)
			{
				if ((i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0 && bool_array[i][j] == true && !beginFlag)//if we reached an egdle
				{
					begin = j;
					end = j;
					beginFlag = true;
				}
				if ((i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0 && bool_array[i][j] == true && beginFlag)
				{
					end = j;
				}
			}

			for (int j = begin; j <= end; j++) {

				if ( (i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0)
				{
					bool_array[i][j] = true;
				}
				
				//if ((color_with_buffer || gray_scale) && (i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0)
				if ((i < viewport_width) && (j < viewport_height) && i >= 0 && j >= 0)
				{
					bool_array[i][j] = false;
					float z = Find_z(i, j, p1, p2, p3);
					if (z <= Get_z(i, j))
					{
						Set_z(i, j, z);
						bool_array[i][j] = true;
					}
				}
			}
						
		
		}

		
	
	if (paint_triangle || gray_scale || color_with_buffer)
	{
		paintFlag = false;
		PaintTriangle(dims[0], dims[1], rectangle_color, paint_triangle, gray_scale, color_with_buffer, scene.GetActiveCamera().zFar);
	}

	delete[] dims;
}

void Renderer::PaintTriangle(int rows, int cols, glm::vec3 color, bool paint_triangle, bool gray_scale, bool color_with_buffer, float zFar)
{
	zFar += 1;
	zFar *= min(viewport_width, viewport_height);
	for (int i = offset_x; i < rows + offset_x+1; i++)
		for (int j = offset_y; j < cols + offset_y+1; j++)
		{
			//if we want to fill the triangles with colors
			if (paint_triangle && (i < viewport_width) && i >= 0 && j >= 0 && (j < viewport_height) && bool_array[i][j])
				PutPixel(i, j, color);

			//if gray scale
			if (gray_scale && (i < viewport_width) && i >= 0 && j >= 0 && (j < viewport_height) && bool_array[i][j])
			{
				float z = Get_z(i, j);
				color = glm::vec3((1 - z / zFar), (1 - z / zFar), (1 - z / zFar));
				PutPixel(i, j, color);
			}

			//if color with depth
			if (color_with_buffer && (i < viewport_width) && i >= 0 && j >= 0 && (j < viewport_height) && bool_array[i][j])
			{
				PutPixel(i, j, color);
			}
		}
}

void Renderer::DrawWorldCoordinates(Scene scene, int j)
{
	glm::vec4 world_x_axis{ 20.0f,0.0f,0.0f,1.0f };
	glm::vec4 world_y_axis{ 0.0f,20.0f,0.0f,1.0f };
	glm::vec4 world_z_axis{ 0.0f,0.0f,20.0f,1.0f };
	//glm::vec4 world_origin{ 0.0f,0.0f,0.0f,1.0f };

	glm::vec4 world_neg_x_axis{ -4.0f,0.0f,0.0f,1.0f };
	glm::vec4 world_neg_y_axis{ 0.0f,-4.0f,0.0f,1.0f };
	glm::vec4 world_neg_z_axis{ 0.0f,0.0f,-4.0f,1.0f };


	//calculate positive end
	world_x_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_x_axis;
	world_y_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_y_axis;
	world_z_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_z_axis;
	//world_origin = scene.GetActiveCamera().GetOrthographicProjection() * scene.GetActiveCamera().GetViewTransformation() *  world_origin;

	//calculate negative end
	world_neg_x_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_neg_x_axis;
	world_neg_y_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_neg_y_axis;
	world_neg_z_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * world_neg_z_axis;

	//go to cartesian coordinates
	glm::vec3 q1 = HomToCartesian(world_x_axis);
	glm::vec3 q2 = HomToCartesian(world_y_axis);
	glm::vec3 q3 = HomToCartesian(world_z_axis);


	glm::vec3 p1 = HomToCartesian(world_neg_x_axis);
	glm::vec3 p2 = HomToCartesian(world_neg_y_axis);
	glm::vec3 p3 = HomToCartesian(world_neg_z_axis);

	//screen coordinates
	viewport(q1, q2, q3, min(viewport_height, viewport_width));
	viewport(p1, p2, p3, min(viewport_height, viewport_width));

	DrawLine(p1, q1, { 1,0,0 });
	DrawLine(p2, q2, { 1,0,0 });
	DrawLine(p3, q3, { 1,0,0 });
}

void Renderer::DrawLocalCoordinates(Scene scene, int j)
{
	glm::vec4 local_x_axis{ 12.0f,0.0f,0.0f,1.0f };
	glm::vec4 local_y_axis{ 0.0f,12.0f,0.0f,1.0f };
	glm::vec4 local_z_axis{ 0.0f,0.0f,12.0f,1.0f };
	//glm::vec4 local_origin{ 0.0f,0.0f,0.0f,12.0f };

	glm::vec4 local_neg_x_axis{ -4.0f,0.0f,0.0f,1.0f };
	glm::vec4 local_neg_y_axis{ 0.0f,-4.0f,0.0f,1.0f };
	glm::vec4 local_neg_z_axis{ 0.0f,0.0f,-4.0f,1.0f };

	//calculate positive end
	local_x_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_x_axis;
	local_y_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_y_axis;
	local_z_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_z_axis;
	//local_origin = scene.GetActiveCamera().GetOrthographicProjection() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).objectTransform * local_origin;

	//calculate negative end
	local_neg_x_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_neg_x_axis;
	local_neg_y_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_neg_y_axis;
	local_neg_z_axis = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * scene.GetModel(j).worldTransform * local_neg_z_axis;


	//go to cartesian coordinates
	glm::vec3 q1 = HomToCartesian(local_x_axis);
	glm::vec3 q2 = HomToCartesian(local_y_axis);
	glm::vec3 q3 = HomToCartesian(local_z_axis);

	glm::vec3 p1 = HomToCartesian(local_neg_x_axis);
	glm::vec3 p2 = HomToCartesian(local_neg_y_axis);
	glm::vec3 p3 = HomToCartesian(local_neg_z_axis);

	//screen coordinates
	viewport(q1, q2, q3, min(viewport_height, viewport_width));
	viewport(p1, p2, p3, min(viewport_height, viewport_width));

	DrawLine(p1, q1, { 0,1,0 });
	DrawLine(p2, q2, { 1,0,0 });
	DrawLine(p3, q3, { 0,0,1 });
}

void Renderer::DrawBoundingBox(Scene scene, MeshModel model)
{
	//find the extreme vertices in model, start with face vertex 1 and find with a  loop
	float max_right = model.GetVertex(0, 0);
	float max_top = model.GetVertex(0, 1);
	float max_far = model.GetVertex(0, 2);
	float min_left = model.GetVertex(0, 0);
	float min_bottom = model.GetVertex(0, 1);
	float min_near = model.GetVertex(0, 2);

	for (int i = 0; i < model.getVerticesSize(); i++) {
		float modelX = model.GetVertex(i, 0);
		float modelY = model.GetVertex(i, 1);
		float modelZ = model.GetVertex(i, 2);

		max_right = max(modelX, max_right);
		max_top = max(modelY, max_top);
		max_far = max(modelZ, max_far);
		min_left = min(modelX, min_left);
		min_bottom = min(modelY, min_bottom);
		min_near = min(modelZ, min_near);
	}

	glm::vec4 a1{ min_left, min_bottom, min_near, 1.0f };
	glm::vec4 a2{ min_left, min_bottom, max_far, 1.0f };
	glm::vec4 a3{ min_left, max_top, min_near, 1.0f };
	glm::vec4 a4{ min_left, max_top, max_far, 1.0f };
	glm::vec4 a5{ max_right, min_bottom, min_near, 1.0f };
	glm::vec4 a6{ max_right, min_bottom, max_far, 1.0f };
	glm::vec4 a7{ max_right, max_top, min_near, 1.0f };
	glm::vec4 a8{ max_right, max_top, max_far, 1.0f };

	//transform edges
	a1 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a1;
	a2 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a2;
	a3 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a3;
	a4 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a4;
	a5 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a5;
	a6 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a6;
	a7 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a7;
	a8 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * a8;

	//go to cartesian coordinates
	glm::vec3 p1 = HomToCartesian(a1);
	glm::vec3 p2 = HomToCartesian(a2);
	glm::vec3 p3 = HomToCartesian(a3);
	glm::vec3 p4 = HomToCartesian(a4);
	glm::vec3 p5 = HomToCartesian(a5);
	glm::vec3 p6 = HomToCartesian(a6);
	glm::vec3 p7 = HomToCartesian(a7);
	glm::vec3 p8 = HomToCartesian(a8);
	glm::vec3 p9 = HomToCartesian(a8);

	//transform to screen coordinates
	viewport(p1, p2, p3, min(viewport_height, viewport_width));
	viewport(p4, p5, p6, min(viewport_height, viewport_width));
	viewport(p7, p8, p9, min(viewport_height, viewport_width));

	//connect edges
	DrawLine(p1, p2, glm::vec3(0, 0, 0));
	DrawLine(p1, p3, glm::vec3(0, 0, 0));
	DrawLine(p1, p5, glm::vec3(0, 0, 0));
	DrawLine(p2, p4, glm::vec3(0, 0, 0));
	DrawLine(p2, p6, glm::vec3(0, 0, 0));
	DrawLine(p3, p4, glm::vec3(0, 0, 0));
	DrawLine(p3, p7, glm::vec3(0, 0, 0));
	DrawLine(p4, p8, glm::vec3(0, 0, 0));
	DrawLine(p5, p6, glm::vec3(0, 0, 0));
	DrawLine(p5, p7, glm::vec3(0, 0, 0));
	DrawLine(p6, p8, glm::vec3(0, 0, 0));
	DrawLine(p7, p8, glm::vec3(0, 0, 0));


}

void Renderer::DrawNormal(Scene scene, MeshModel model)
{
	for (int i = 0; i < model.GetFacesCount(); i++) {
		Face face = model.GetFace(i);

		//get index
		int v_index1 = face.GetVertexIndex(0) - 1;
		int v_index2 = face.GetVertexIndex(1) - 1;
		int v_index3 = face.GetVertexIndex(2) - 1;

		//get normal index
		int vn_index1 = face.GetNormalIndex(0) - 1;
		int vn_index2 = face.GetNormalIndex(1) - 1;
		int vn_index3 = face.GetNormalIndex(2) - 1;

		//get vertices
		glm::vec4 vertex1 = { model.GetVertex(v_index1,0),model.GetVertex(v_index1,1) ,model.GetVertex(v_index1,2),1.0f };
		glm::vec4 vertex2 = { model.GetVertex(v_index2,0),model.GetVertex(v_index2,1) ,model.GetVertex(v_index2,2),1.0f };
		glm::vec4 vertex3 = { model.GetVertex(v_index3,0),model.GetVertex(v_index3,1) ,model.GetVertex(v_index3,2),1.0f };

		//get normals
		glm::vec4 normal1 = { model.GetNormal(vn_index1,0),model.GetNormal(vn_index1,1) ,model.GetNormal(vn_index1,2),1.0f };
		glm::vec4 normal2 = { model.GetNormal(vn_index2,0),model.GetNormal(vn_index2,1) ,model.GetNormal(vn_index2,2),1.0f };
		glm::vec4 normal3 = { model.GetNormal(vn_index3,0),model.GetNormal(vn_index3,1) ,model.GetNormal(vn_index3,2),1.0f };

		//add the normal to vertex
		normal1 = vertex1 + 0.05f * normal1;
		normal2 = vertex2 + 0.05f * normal2;
		normal3 = vertex3 + 0.05f * normal3;

		normal1.w = 1.0f;
		normal2.w = 1.0f;
		normal3.w = 1.0f;

		//apply transformations
		normal1 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * normal1;
		normal2 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * normal2;
		normal3 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * normal3;

		vertex1 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * vertex1;
		vertex2 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * vertex2;
		vertex3 = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * vertex3;

		//go to cartesian coordinates
		glm::vec3 q1 = HomToCartesian(normal1);
		glm::vec3 q2 = HomToCartesian(normal2);
		glm::vec3 q3 = HomToCartesian(normal3);

		glm::vec3 p1 = HomToCartesian(vertex1);
		glm::vec3 p2 = HomToCartesian(vertex2);
		glm::vec3 p3 = HomToCartesian(vertex3);

		//screen coordinates
		viewport(q1, q2, q3, min(viewport_height, viewport_width));
		viewport(p1, p2, p3, min(viewport_height, viewport_width));

		//draw the normals
		DrawLine(p1, q1, { 0,0,1 });
		DrawLine(p2, q2, { 0,0,1 });
		DrawLine(p3, q3, { 0,0,1 });


	}
}

void Renderer::viewport(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float height)
{
	//add 1 to fit to screen coordinates
	p1.x += 1;
	p1.y += 1;
	p1.z += 1;
	p2.x += 1;
	p2.y += 1;
	p2.z += 1;
	p3.x += 1;
	p3.y += 1;
	p3.z += 1;

	//scale by window size
	p1 = (float)height / 2 * p1;
	p2 = (float)height / 2 * p2;
	p3 = (float)height / 2 * p3;

}

void Renderer::viewport(glm::vec3& p1, float height)
{
	//add 1 to fit to screen coordinates
	p1.x += 1;
	p1.y += 1;
	p1.z += 1;

	//scale by window size
	p1 = (float)height / 2 * p1;
	
}

void Renderer::DrawFaceNormal(Scene scene, MeshModel model) {

	for (int i = 0; i < model.GetFacesCount(); i++) {
		Face face = model.GetFace(i);

		//get index
		int v_index1 = face.GetVertexIndex(0) - 1;
		int v_index2 = face.GetVertexIndex(1) - 1;
		int v_index3 = face.GetVertexIndex(2) - 1;

		//get vertices
		glm::vec3 vertex1 = { model.GetVertex(v_index1,0),model.GetVertex(v_index1,1) ,model.GetVertex(v_index1,2) };
		glm::vec3 vertex2 = { model.GetVertex(v_index2,0),model.GetVertex(v_index2,1) ,model.GetVertex(v_index2,2) };
		glm::vec3 vertex3 = { model.GetVertex(v_index3,0),model.GetVertex(v_index3,1) ,model.GetVertex(v_index3,2) };

		//calculate normals
		glm::vec4 average = { 0,0,0,1 };
		glm::vec4 normal = glm::vec4(glm::normalize(glm::cross(vertex1 - vertex2, vertex1 - vertex3)), 0);

		average += glm::vec4((vertex1 + vertex2 + vertex3) / 3.f, 0);
		normal = average + 0.1f * normal;
		glm::vec3 p1 = glm::vec3(1, 1, 1);

		average = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * average;
		normal = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * normal;

		//go to cartesian coordinates
		glm::vec3 p2 = HomToCartesian(average);
		glm::vec3 p3 = HomToCartesian(normal);

		//screen coordinates
		viewport(p1, p2, p3, min(viewport_height, viewport_width));

		DrawLine(p3, p2, { 1,0,1 });
	}


}

glm::vec3 Renderer::HomToCartesian(glm::vec4 vec)
{

	if (vec[3] == 0) {
		return glm::vec3(vec[0], vec[1], vec[2]);
	}
	return glm::vec3(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]);
}

void Renderer::TransformationMultiplications(Scene scene, MeshModel model, glm::vec4 p)
{
	p = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().GetViewTransformation() * model.GetTransform() * p;
}

int* Renderer::DrawBoundingRectangleForTriangles(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles)
{
	//find the extreme vertices in model, start with face vertex 1 and find with a  loop
	float max_x = max(p1.x, p2.x);
	max_x = max(max_x, p3.x);
	float max_y = max(p1.y, p2.y);
	max_y = max(max_y, p3.y);
	float max_z = max(p1.z, p2.z);
	max_z = max(max_z, p3.z);
	float min_x = min(p1.x, p2.x);
	min_x = min(min_x, p3.x);
	float min_y = min(p1.y, p2.y);
	min_y = min(min_y, p3.y);
	float min_z = min(p1.z, p2.z);
	min_z = min(min_z, p3.z);

	//rectangle coordinates
	glm::vec4 a1{ min_x, min_y,min_z,  1.0f };
	glm::vec4 a2{ min_x, min_y,max_z,  1.0f };
	glm::vec4 a3{ min_x, max_y,min_z,  1.0f };
	glm::vec4 a4{ min_x, max_y,max_z,  1.0f };
	glm::vec4 a5{ max_x, min_y,min_z,  1.0f };
	glm::vec4 a6{ max_x, min_y,max_z,  1.0f };
	glm::vec4 a7{ max_x, max_y,min_z,  1.0f };
	glm::vec4 a8{ max_x, max_y,max_z,  1.0f };

	//if needed to draw bounding rectangles for each triangle
	if (bounding_rectangles) {
		//connect edges
		DrawLine(a1, a2, color);
		DrawLine(a1, a3, color);
		DrawLine(a1, a5, color);
		DrawLine(a2, a4, color);
		DrawLine(a2, a6, color);
		DrawLine(a3, a4, color);
		DrawLine(a3, a7, color);
		DrawLine(a4, a8, color);
		DrawLine(a5, a6, color);
		DrawLine(a5, a7, color);
		DrawLine(a6, a8, color);
		DrawLine(a7, a8, color);

	}

	int dx = max_x - min_x;
	int dy = max_y - min_y;

	int* dims = new int[2];

	dims[0] = dx + 1;
	dims[1] = dy + 1;

	//offset for boolean array
	offset_x = min_x;
	offset_y = min_y;

	return dims;
}

float Renderer::CalculateArea(glm::vec3& q1, glm::vec3& q2, glm::vec3& q3)
{
	float x1 = q1.x;
	float y1 = q1.y;
	float x2 = q2.x;
	float y2 = q2.y;
	float x3 = q3.x;
	float y3 = q3.y;

	//triangle area formula
	return abs(((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2.0f);
}

//finding z coordinate of a point inside triangle using :Linear interpolation - Barycentric method
float Renderer::Find_z(int _x, int  _y, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	float A1 = CalculateArea(glm::vec3(_x, _y, 1), p2, p3);
	float A2 = CalculateArea(glm::vec3(_x, _y, 1), p1, p3);
	float A3 = CalculateArea(glm::vec3(_x, _y, 1), p1, p2);

	float Total_Area = A1 + A2 + A3;

	return ((A1 / Total_Area * p1.z) + (A2 / Total_Area * p2.z) + (A3 / Total_Area * p3.z));
}


//z buffer getter
float Renderer::Get_z(int i, int j) 
{
		return z_buffer[Z_INDEX(viewport_width, i, j)];
}

//z buffer setter
void Renderer::Set_z(int i, int j, float z)
{
	z_buffer[Z_INDEX(viewport_width, i, j)] = z;
}


void Renderer::DrawLight(Scene scene, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	
	Camera camera = scene.GetCamera(0);
	glm::vec3 cameraPosition = camera.eye;

	Light light = scene.GetLight(0);
	glm::vec3 LightPosition = light.GetPosition(); // light position
	viewport(LightPosition, min(viewport_height, viewport_width));
	viewport(cameraPosition, min(viewport_height, viewport_width));

	glm::vec3 average;
	average += glm::vec3((p1 + p2 + p3) / 3.f);


	MeshModel model = scene.GetModel(0);

	glm::vec3 Ia = light.Compute_Ia(model.Ka);
	//normal face
	light.N = compute_normal(p1, p2, p3);
	//compute I
	light.I = glm::normalize(glm::vec3(LightPosition.x - average.x, LightPosition.y - average.y, LightPosition.z - average.z));
	light.V= glm::normalize(glm::vec3(cameraPosition.x - average.x, cameraPosition.y - average.y, cameraPosition.y - average.z));
	int FaceCount = model.GetFacesCount();



	//find the extreme vertices in model, start with face vertex 1 and find with a  loop
	float max_x = max(p1.x, p2.x);
	max_x = max(max_x, p3.x);
	float max_y = max(p1.y, p2.y);
	max_y = max(max_y, p3.y);

	float min_x = min(p1.x, p2.x);
	min_x = min(min_x, p3.x);
	float min_y = min(p1.y, p2.y);
	min_y = min(min_y, p3.y);

	
	for (int y = min_x; y <= max_y; y++)
	{
		for (int x = min_y; x <= max_x; x++)
		{
			float z = Get_z(x, y);
			if (z != INFINITY)
			{
				if (scene.ambient_shading)

					if (scene.flat_shading)

			}
		}
	}

	if (scene.ambient_shading)
	{
		
		for (int i = 0; i < viewport_width; i++) {
			for (int j = 0; j < viewport_height; j++) {
				float z = Get_z(i, j);
					if (z != INFINITY)
					{
						PutPixel(i, j, Ia);
					}
			}

		}
	}

	if (scene.flat_shading)
	{
		for (int i = 0; i < FaceCount; i++)
		{
	
		}
	}
}



//compute nurmal faces
glm::vec3 Renderer::compute_normal(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3)
{
	glm::vec3 normal = glm::normalize(glm::cross(vertex1 - vertex2, vertex1 - vertex3));
	glm::vec3 p1 = normal, p2 = normal;

	return normal;

}










