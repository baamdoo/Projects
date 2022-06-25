#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 normal;
in layout(location=2) vec2 tex;
in layout (location = 3) vec3 tangent;
in layout (location = 4) vec3 bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 vertexPos;	
} vs_out;

void main()
{
	vec4 v = vec4(position, 1.0f);
	vec4 tmpPosition = modelMatrix * v;
	vec4 outPosition = projectionMatrix * viewMatrix * tmpPosition;
	gl_Position = outPosition;

	vec4 tmpNormal = modelMatrix * vec4(normal, 0);

	vs_out.texCoords = tex;
	vs_out.normal = tmpNormal.xyz;
	vs_out.vertexPos = tmpPosition.xyz;
}
