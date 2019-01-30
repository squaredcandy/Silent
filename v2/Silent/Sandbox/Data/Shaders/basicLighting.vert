#version 430 core
layout (location = 0) in vec3 Msh_Pos;
layout (location = 1) in vec3 Msh_Normal;
layout (location = 2) in vec2 Msh_TexCoord;
layout (location = 3) in vec3 Msh_Tangent;
layout (location = 4) in vec3 Msh_Bitangent;
layout (location = 5) in mat4 Model;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 Projection;
uniform mat4 View;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(Model * vec4(Msh_Pos, 1.0));   
    vs_out.TexCoords = Msh_TexCoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    vec3 T = normalize(normalMatrix * Msh_Tangent);
    vec3 N = normalize(normalMatrix * Msh_Bitangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
	gl_Position = Projection * View * Model * vec4(Msh_Pos, 1.0);
}