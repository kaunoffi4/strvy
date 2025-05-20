
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in mat4 a_InstanceModel;		// per instance
layout(location = 6) in vec4 a_InstanceColor;		// per instance
layout(location = 7) in int	 a_EntityID;			// per instance


layout(location = 0) out vec3 v_FragPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec3 v_ViewPos;
layout(location = 3) out vec4 v_Color;
layout(location = 4) out flat int v_EntityID;

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_ViewProjection;
	vec3 u_ViewPos;
};

//layout(std140, binding = 3) uniform InstanceBlock
//{
//	mat4 u_Model; // model matrix of each container
//	int u_EntityID;
//	vec4 u_Color;
//};


void main()
{
	mat4 M = a_InstanceModel;
	v_FragPos = vec3(M * vec4(a_Position, 1.0));
	v_Normal = mat3(transpose(inverse(M))) * a_Normal;
	v_ViewPos = u_ViewPos;
	v_EntityID = a_EntityID;
	v_Color = a_InstanceColor;

	gl_Position = u_ViewProjection * M * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entityBuffer;

layout(location = 0) in vec3 v_Normal;
layout(location = 1) in vec3 v_FragPos;
layout(location = 2) in vec3 v_ViewPos;
layout(location = 3) in vec4 v_Color;
layout(location = 4) in flat int v_EntityID;

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

struct Material
{
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(std140, binding = 2) uniform MaterialBlock
{
	Material materials[5];
};


void main()
{
	// ambient
    vec3 ambient = lights[0].ambient * materials[0].ambient;
  	
    // diffuse
    vec3 lightDir = normalize(v_FragPos - lights[0].position);
	vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[0].diffuse * (diff * materials[0].diffuse);
    
    // specular
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[0].shininess);
    vec3 specular = lights[0].specular * (spec * materials[0].specular);

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0) * v_Color;
	//fragColor = vec4((norm * 0.5) + 0.5, 1.0); // DEBUG
	entityBuffer = v_EntityID;
}