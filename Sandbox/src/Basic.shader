#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;	

out VS_OUT
{
	vec3 CrntPos;
	vec3 Color;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//vs_out.CrntPos = vec3(model * vec4(aPos, 1.0));
	vs_out.CrntPos = aPos;
	vs_out.Color = aColor;

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 CrntPos;
	vec3 Color;
} fs_in;

//uniform vec3 lightColor;
//uniform vec3 lightPos;
//uniform vec3 camPos;

void main()
{
	vec3 result = fs_in.Color;
	FragColor = vec4(result, 1.0);
};