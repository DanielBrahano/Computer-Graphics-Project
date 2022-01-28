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
uniform int alpha;

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
	vec3 reflectDirection = normalize(reflect(-LightDirection, norm));


	vec3 FinalAmbient = (AmbientLight * material.ambient);
	vec3 FinalDiffuse=(max(dot(-norm, LightDirection), 0.0f) * normalize(material.diffuse * DiffuseLight));
	vec3 FinalSpecular = (pow(max(dot(reflectDirection, CameraDirection),0.0f), alpha) * normalize(material.specular * SpecularLight));

	frag_color = vec4(FinalDiffuse, 1);
}