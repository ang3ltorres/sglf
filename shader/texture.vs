#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

//* SSBO *//
struct GPU_SSBO
{
	vec4 Src;
	vec4 TintColor;
	mat4 Model;
};

layout(std430, binding = 1) buffer GPU_COMMON
{
	GPU_SSBO SSBO[];
};

//* UBO Shared*//
layout(std140, binding = 0) uniform GPU_UBO_SHARED
{
	mat4 ViewProjection;
};

//* UBO *//
layout(std140, binding = 1) uniform GPU_UBO_NONSHARED {
	int TextureType;
};

out vec4 f_TintColor;
out vec2 f_TexCoord;
out flat int f_TextureType;

void main()
{
	GPU_SSBO vertex =  SSBO[gl_InstanceID];

	f_TintColor = vertex.TintColor / 255.0;
	f_TexCoord  = vertex.Src.xy + aTexCoord * vertex.Src.zw;
	gl_Position = ViewProjection * vertex.Model * vec4(aPos, 0.0, 1.0);
	
	f_TextureType = TextureType;
}
