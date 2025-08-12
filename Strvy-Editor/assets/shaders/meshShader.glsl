
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;		

layout(location = 0) out vec2 v_TexCoords;
layout(location = 1) out flat int v_EntityID;

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_ViewProjection;
	vec3 u_ViewPos;
};

layout(std140, binding = 3) uniform InstanceBlock
{
	mat4 u_Model;
	int entityID;
};

void main()
{
	v_TexCoords = a_TexCoords;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	v_EntityID = entityID;
	//gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entityBuffer;

layout(location = 0) in vec2 v_TexCoords;
layout(location = 1) in flat int v_EntityID;


struct Light
{
	vec3 position; // (for non-directional lights, such as point light, which spreads out light rays in all directions)
	//vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(std140, binding = 1) uniform LightBlock
{
	int lightCount; // number of active lights
	Light lights[10];
};

layout(binding = 0) 
uniform sampler2D texture_diffuse1;


void main()
{
    fragColor = texture(texture_diffuse1, v_TexCoords);
	entityBuffer = v_EntityID;
}