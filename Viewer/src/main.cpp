#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>

//variables for window size
static float top = 1000.0f;
static float bottom = 0.0f;
static float _left = 0.0;
static float _right = 1000.0f;
static float zNear = 0.1f;
static float zFar = 1000.0f;
static float fovy = 45.0f;

//control lookAt
static glm::vec3 eye = { 0,0,1 };
static glm::vec3 at = { 0,0,0 };
static glm::vec3 up = { 0,1,0 };

bool world_axes = false;
bool model_axes = false;

bool Ortho = false;
bool BoundingBox = false;
bool DrawNormals = false;
int Orthographic = 0;


/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1000, windowHeight = 1000;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	/*********************************************************************************************/
	Camera camera;

	///*88888888888----------------------------------------------------------------*/
	glm::mat4 proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	glm::vec4 test = { -0.21627, 0.31021, 0.64313,1.0f };
	test = proj * test;
	std::cout << "Pres:" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			std::cout <<(test[j]+1) << " ";
		}
		std::cout << std::endl;
	}

	///*88888888888----------------------------------------------------------------*/
	scene.AddCamera(std::make_shared<Camera>(camera));
	scene.SetActiveCameraIndex(0);

	/*********************************************************************************************/
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
		if (scene.GetCameraCount() != 0) {
			renderer.SetSize(frameBufferWidth, frameBufferHeight);
			scene.GetActiveCamera().SetOrthographicProjection(0, frameBufferWidth, 0, frameBufferHeight, 1, -1);
			top = frameBufferHeight;
			_right = frameBufferWidth;
		}
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown['A']) //A key is down - move right
		{
			scene.GetModel(0).ObjectTranslateModel(-0.05, 0, 0);
		}
		else if (io.KeysDown['D'])// D key is down - move left
		{
			scene.GetModel(0).ObjectTranslateModel(0.05, 0, 0);
		}
		else if (io.KeysDown['W'])// W key is down - move up
		{
			scene.GetModel(0).ObjectTranslateModel(0, 0.05, 0);
		}
		else if (io.KeysDown['S'])// s key is down - move down
		{
			scene.GetModel(0).ObjectTranslateModel(0, -0.05, 0);
		}

		//rotation transforamtion
		else if (io.KeysDown['J']) //J key is down - rotate around X 
		{
			scene.GetModel(0).ObjectRotateModel(5, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (io.KeysDown['L']) //L key is down - rotate around X 
		{
			scene.GetModel(0).ObjectRotateModel(-5, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (io.KeysDown['I']) //I key is down - rotate around Y 
		{
			scene.GetModel(0).ObjectRotateModel(5, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (io.KeysDown['K']) //K key is down - rotate around Y 
		{
			scene.GetModel(0).ObjectRotateModel(-5, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		//scale transformation
		else if (io.KeysDown['T']) //T key is down - make it bigger
		{
			scene.GetModel(0).ObjectScaleModel(1.1, 1.1, 1.1);
		}
		else if (io.KeysDown['Y']) //Y key is down - make it smaller 
		{
			scene.GetModel(0).ObjectScaleModel(0.9, 0.9, 0.9);
		}

	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		//static float f = 0.0f;

		//before we open the imgui window, let's resize it
		ImGui::SetNextWindowSize(ImVec2(390, 400));


		static int counter = 0;
		ImGui::Begin("Model Transformations Control");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

								// Create a window called "Hello, world!" and append into it.
		/*---------------------------------------------MY ADDITION---=======================================---*/
		/*---------------------------------------------MY ADDITION---=======================================---*/

		//vectors and varibale for transformations we start with 0
		static glm::vec3 world_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 world_rotation(0.0f, 0.0f, 0.0f);
		static float world_scale = 1.0f;

		//vectors and varibale for transformations we start with 0
		static glm::vec3 local_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 local_rotation(0.0f, 0.0f, 0.0f);
		static float local_scale = 1.0f;

		//(object) vectors and variables to calculate delta (difference in transformations)
		static glm::vec3 object_previous_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 object_previous_rotation(0.0f, 0.0f, 0.0f);
		static float object_previous_scale = 1.0f;
		//(world) vectors and variables to calculate delta (difference in transformations)
		static glm::vec3 world_previous_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 world_previous_rotation(0.0f, 0.0f, 0.0f);
		static float world_previous_scale = 1.0f;


		ImGui::Text("        ");
		ImGui::Text("       Local Transformations  ");
		ImGui::Text("      X           Y           Z  ");

		//make sliders for the transformations
		ImGui::SliderFloat3("translation_L", &local_translation.x, -1.5f, 1.5f);
		ImGui::SliderFloat3("rotation_L", &local_rotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat("scale_L", &local_scale, 0.5f, 1.5f);

		ImGui::Text("        ");
		ImGui::Text("       World Transformations  ");
		ImGui::Text("      X           Y           Z  ");
		//make sliders for the transformations
		ImGui::SliderFloat3("translation_W", &world_translation.x, -1.5f, 1.5f);
		ImGui::SliderFloat3("rotation_W", &world_rotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat("scale_W", &world_scale, 0.5f, 1500.0f);

		/*I am handling transformations by saving each in a vector and comparing to the last on and apply transformation only if something has changed*/

		//switch on which transformations we are about to do. 1 for OBJECT 0 for WORLD

			//handle translation and scale
			if ((object_previous_scale != local_scale) || (object_previous_translation != local_translation) || (object_previous_rotation != local_rotation)) {
				scene.GetModel(0).ObjectTranslateModel(local_translation.x - object_previous_translation.x, local_translation.y - object_previous_translation.y, local_translation.z - object_previous_translation.z);
				object_previous_scale = local_scale;
				scene.GetModel(0).objectScale[0][0] = scene.GetModel(0).objectScale[1][1] = scene.GetModel(0).objectScale[2][2] = local_scale;
			}

			//handle rotations in all directions
			if (local_rotation.x != object_previous_rotation.x) {
				scene.GetModel(0).ObjectRotateModel(local_rotation.x - object_previous_rotation.x, { 1.0f,0.0f,0.0f });
			}
			if (local_rotation.y != object_previous_rotation.y) {
				scene.GetModel(0).ObjectRotateModel(local_rotation.y - object_previous_rotation.y, { 0.0f,1.0f,0.0f });
			}
			if (local_rotation.z != object_previous_rotation.z) {
				scene.GetModel(0).ObjectRotateModel(local_rotation.z - object_previous_rotation.z, { 0.0f,0.0f,1.0f });
			}

			//save the last changes
			object_previous_scale = local_scale;
			object_previous_translation = local_translation;
			object_previous_rotation = local_rotation;

			//handle translation and scale
			if ((world_previous_scale != world_scale) || (world_previous_translation != world_translation) || (world_previous_rotation != world_rotation)) {
				scene.GetModel(0).WorldTranslateModel(5 * world_translation.x - 5 * world_previous_translation.x, 5 * world_translation.y - 5 * world_previous_translation.y, 5 * world_translation.z - 5 * world_previous_translation.z);
				world_previous_scale = world_scale;
				scene.GetModel(0).worldScale[0][0] = scene.GetModel(0).worldScale[1][1] = scene.GetModel(0).worldScale[2][2] = world_scale;
			}

			//handle rotations in all directions
			if (world_rotation.x != world_previous_rotation.x) {
				scene.GetModel(0).WorldRotateModel(world_rotation.x - world_previous_rotation.x, { 1.0f,0.0f,0.0f });
			}
			if (world_rotation.y != world_previous_rotation.y) {
				scene.GetModel(0).WorldRotateModel(world_rotation.y - world_previous_rotation.y, { 0.0f,1.0f,0.0f });
			}
			if (world_rotation.z != world_previous_rotation.z) {
				scene.GetModel(0).WorldRotateModel(world_rotation.z - world_previous_rotation.z, { 0.0f,0.0f,1.0f });
			}

			//save the last changes
			world_previous_scale = world_scale;
			world_previous_translation = world_translation;
			world_previous_rotation = world_rotation;

		/*---------------------------------------------MY ADDITION---=======================================---*/
		/*---------------------------------------------MY ADDITION---=======================================---*/
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	//before we open the imgui window, let's resize it
	ImGui::SetNextWindowSize(ImVec2(390, 390));

	ImGui::Begin("Camera/Projection  Control");
	ImGui::Text("Choose Projection");
	static int projection = 0;
	ImGui::RadioButton("Orthographic", &projection, 1); ImGui::SameLine();
	ImGui::RadioButton("Perspective", &projection, 2);
	
	

	/* UP, DOWN , TOP , BOTTOM sliders*/
	ImGui::SliderFloat("down", &bottom, -1000.0f, 1000.0f);
	ImGui::SliderFloat("up", &top, -1000.0f, 1000.0f);

	ImGui::SliderFloat("left", &_left, -1000.0f, 1000.0f);
	ImGui::SliderFloat("right", &_right, -1000.0f, 1000.0f);


	ImGui::SliderFloat("near", &zNear, -1.00f, 1000.0f);
	ImGui::SliderFloat("far", &zFar, -1000.0f, 1000.0f);

	if ((projection == 1) && (scene.GetModelCount()))
	{
		scene.GetActiveCamera().SetOrthographicProjection(_left, _right, bottom, top, zNear, zFar);
	}
	if ((projection == 2) && (scene.GetModelCount()))
	{
		ImGui::SliderFloat("fovy", &fovy, 0.0f, 180.0f);
		float aspectRatio = (_right - _left) / (top - bottom);
		scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(fovy), aspectRatio, zNear, zFar);
	}

	
	ImGui::Checkbox("Bounding Box", &BoundingBox);
	scene.draw_box = BoundingBox;

	ImGui::Checkbox("Draw Normals", &DrawNormals);
	scene.draw_normals = DrawNormals;

	ImGui::Text("           ");
	ImGui::Text("           LookAt Control");
	
	
	ImGui::Text("      X           Y           Z  ");
	ImGui::InputFloat3("Eye", &eye.x);
	ImGui::InputFloat3("At", &at.x);
	ImGui::InputFloat3("Up", &up.x);
	scene.GetActiveCamera().SetCameraLookAt(eye, at, up);

	//option to draw axis for model and world
	ImGui::Checkbox("World axes", &world_axes);
	ImGui::Checkbox("Model axes", &model_axes);
	if (scene.GetModelCount()) {
		scene.GetModel(0).DrawWorldAxes = world_axes;
		scene.GetModel(0).DrawModelAxes = model_axes;
	}



	ImGui::End();
}