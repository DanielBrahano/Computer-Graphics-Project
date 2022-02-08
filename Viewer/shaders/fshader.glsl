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

	frag_color = vec4(textureColor,1);
}