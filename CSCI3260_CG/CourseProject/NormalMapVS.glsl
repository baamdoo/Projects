#version 430 core

in layout (location = 0) vec3 position;
in layout (location = 1) vec3 normal;
in layout (location = 2) vec2 tex;
in layout (location = 3) vec3 tangent;
in layout (location = 4) vec3 bitangent;

out VS_OUT {
    vec3 vertexPos;
    vec2 texCoords;
    vec3 tangentDirLightPos;
    vec3 tangentPointLightPos;
    vec3 tangentViewPos;
    vec3 tangentVertexPos;
} vs_out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 pointLightPos;
uniform vec3 dirLightPos;
uniform vec3 cameraPos;

void main()
{
    vs_out.vertexPos = vec3(modelMatrix * vec4(position, 1.0));   
    vs_out.texCoords = tex;
    
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.tangentPointLightPos = TBN * pointLightPos;
    vs_out.tangentViewPos  = TBN * cameraPos;
    vs_out.tangentVertexPos  = TBN * vs_out.vertexPos;
        
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}