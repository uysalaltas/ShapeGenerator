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
	vec4 CrntPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	vs_out.CrntPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Color = aColor;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.CrntPosLightSpace = lightSpaceMatrix * vec4(vs_out.CrntPos, 1.0);

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
	vec4 CrntPosLightSpace;
} fs_in;

vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform sampler2D shadowMap;
uniform vec3 camPos;
uniform vec3 lightPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	//float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);
	float bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

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

	// specular
	vec3 viewDir = normalize(camPos - fs_in.CrntPos);
	vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	// calculate shadow
	float shadow = ShadowCalculation(fs_in.CrntPosLightSpace, lightDir);
	vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * fs_in.Color;

	FragColor = vec4(result, 1.0);
};