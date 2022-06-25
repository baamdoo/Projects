#version 430

in layout(location=0) vec3 position;

out vec3 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	texCoords = position;
	vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0f);
	gl_Position = pos.xyww;
}