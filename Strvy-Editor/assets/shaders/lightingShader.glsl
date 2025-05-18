
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec3 viewPos;
layout(location = 3) out vec4 color;
layout(location = 4) out flat int entityID;

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_ViewProjection;
	vec3 u_ViewPos;
};

layout(std140, binding = 3) uniform InstanceBlock
{
	mat4 u_Model; // model matrix of each container
	int u_EntityID;
	vec4 u_Color;
};


void main()
{
	fragPos = vec3(u_Model * vec4(a_Position, 1.0));
	normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	viewPos = u_ViewPos;
	entityID = u_EntityID;
	color = u_Color;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entityBuffer;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 viewPos;
layout(location = 3) in vec4 color;
layout(location = 4) in flat int entityID;

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

layout(std140, binding = 2) uniform Material
{
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} material;


void main()
{
	// ambient
    vec3 ambient = lights[0].ambient * material.ambient;
  	
    // diffuse
    vec3 lightDir = normalize(fragPos - lights[0].position);
	vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[0].diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[0].specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0) * color;
	//fragColor = vec4((norm * 0.5) + 0.5, 1.0); // DEBUG
	entityBuffer = entityID;
}