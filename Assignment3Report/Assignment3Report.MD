
# ASSIGNMENT #3


## Task 1

In the first two lines the function declares that it wants 1 texture in the name of 'gl_screen_tex' and 1 vertex array in the name of 'gl_screen_vtc'.then we generate a buffer.

In the function we create and arrays vtc and tex which represent the cubes [-1,1] and [0,1] respectively, those are going to be the sceen and texture coordinates.

Then we allocate memory in the gpu for both vtc and tex, with the command glBufferSubData we update seperately the buffer that is bound and copy the memory of the arrays.

The command InitShader is used to compile the shader and prepare it for use. Then setting the shaders to 'program' indentifier, 
the function creates variables to represent the screen and texture coordinates and send them to vertex shader, which updates and send to the fragment shader to cover the screen with the texture.


## Task 2


## Task 3 
```
#version 330 core

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}
```

## Task 4
```
#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
};

// We set this field's properties from the C++ code
uniform Material material;
uniform vec3 AmbientLight;
uniform vec3 DiffuseLight;
uniform vec3 SpecularLight;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform int Alpha;
uniform bool ToonShading;
uniform float levels;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;


void main()
{
	
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	vec3 LightDirection = normalize ( LightPosition - fragPos);
	vec3 CameraDirection = normalize ( CameraPosition - fragPos);
	vec3 norm = normalize(fragNormal);
	vec3 reflectDirection = reflect(LightDirection, norm);
	float spec = pow(max(dot(CameraDirection, reflectDirection), 0.0f), Alpha);

	vec3 FinalAmbient = (AmbientLight * material.ambient);
	//vec3 FinalDiffuse=(max(dot(-norm, LightDirection), 0.0f) * normalize(material.diffuse * DiffuseLight));
	vec3 FinalDiffuse= (material.diffuse * DiffuseLight) * max(dot(norm, LightDirection),0.0f);
	vec3 FinalSpecular;
	if (dot(norm, LightDirection)>=0){
		FinalSpecular = vec3( SpecularLight.x * spec * material.specular.x,  SpecularLight.y * spec * material.specular.y,  SpecularLight.z * spec * material.specular.z);
		}
		else 
		FinalSpecular = vec3(0,0,0);
	
	vec3 FinalColor = vec3(FinalAmbient + FinalDiffuse + FinalSpecular);

	if(ToonShading)
	{
		vec3 level = vec3(floor(FinalColor.x * levels),floor(FinalColor.y * levels),floor(FinalColor.z * levels));
		FinalColor = level/levels;
	}
	frag_color = vec4(FinalColor, 1);

	//frag_color = vec4(textureColor,1);
}
```

## Task 5
```
		if (scene.GetModelCount() == 0)
		return;

	MeshModel& model = scene.GetModel(0);
	Camera& camera = scene.GetActiveCamera();

	glm::vec4 temp;
		temp = model.GetTransform() * glm::vec4(model.modelVertices[0].position,1);
	cout << "model position: " << temp.x << " " << temp.y << " " << temp.z << " " << endl;

	// Activate the 'colorShader' program (vertex and fragment shaders)
	colorShader.use();

	// Set the uniform variables
	colorShader.setUniform("model", model.GetTransform());
	colorShader.setUniform("view", camera.GetViewTransformation());
	colorShader.setUniform("projection", camera.GetProjectionTransformation());
	colorShader.setUniform("material.textureMap", 0);
	colorShader.setUniform("ToonShading", scene.toon_shading);
	colorShader.setUniform("levels", scene.levels);

	if (scene.lighting)
	{
		Light light = scene.GetLight(0);

		colorShader.setUniform("AmbientLight", light.AmbientColor);
		colorShader.setUniform("DiffuseLight", light.DiffuseColor);
		colorShader.setUniform("SpecularLight",light.SpecularColor);
		colorShader.setUniform("material.ambient", model.Ka);
		colorShader.setUniform("material.diffuse", model.Kd);
		colorShader.setUniform("material.specular", model.Ks);
		colorShader.setUniform("Alpha", light.alpha);
		colorShader.setUniform("LightPosition", light.GetPosition());
		colorShader.setUniform("CameraPosition", camera.eye);
	}

	// Set 'texture1' as the active texture at slot #0
	texture1.bind(0);

	// Drag our model's faces (triangles) in fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(model.GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
	glBindVertexArray(0);


## Task 6 

## Task 7



## Task 8









