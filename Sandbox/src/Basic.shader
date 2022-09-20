#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out VS_OUT
{
	vec3 CrntPos;
	vec3 Color;
	vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.CrntPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Color = aColor;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;

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
	vec3 Normal;
} fs_in;

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(1.0, 2.0, 3.0);

uniform vec3 camPos;

void main()
{
	// ambient
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.CrntPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * 0.5;

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - fs_in.CrntPos);
	vec3 reflectionDirection = reflect(-lightDir, fs_in.Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float spec = specAmount * specularLight;
	vec3 specular = spec * lightColor;

	vec3 result = (ambient + diffuse) * fs_in.Color;
	FragColor = vec4(result, 1.0);
};