#version 330 core


in vec3 FragPos;
in vec3 FragNormal;
out vec4 FragColor;

uniform vec3 outColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 ViewPos;

void main()
{
	vec3 Normal = normalize(FragNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 ViewDir = normalize(ViewPos - FragPos);

	float dis = 10.0 / distance(lightPos, FragPos);

	float diffuseLight = max(dot(lightDir, Normal), 0);
	vec3 diffuse = diffuseLight * lightColor;
	
	float shininess = 128.0;
	vec3 reflectDir = normalize(reflect(-lightDir, Normal));
	float specularLight = diffuseLight > 0.0 ? max(dot(reflectDir, ViewDir), 0.0) : 0.0;
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor; 

	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 result = ((diffuse + ambient) * outColor + specular) * dis;

	FragColor = vec4(result, 1.0);
}