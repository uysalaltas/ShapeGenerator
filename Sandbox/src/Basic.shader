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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDirection)
{
	float shadow = 0.0f;
	vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if (lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		float bias = max(0.025f * (1.0f - dot(fs_in.Normal, lightDirection)), 0.0005f);

		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for (int y = -sampleRadius; y <= sampleRadius; y++)
		{
			for (int x = -sampleRadius; x <= sampleRadius; x++)
			{
				float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;
			}
		}
		shadow /= pow((sampleRadius * 2 + 1), 2);
	}

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

	// Shadow
	float shadow = ShadowCalculation(fs_in.CrntPosLightSpace, lightDir);
	vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * fs_in.Color;

	FragColor = vec4(result, 1.0);
};