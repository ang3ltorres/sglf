#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

struct GPU_SSBO {
	vec4 gpu_Src;
	vec4 gpu_TintColor;
	mat4 gpu_ViewProjection;
	mat4 gpu_Model;
};

layout(std140, binding = 0) uniform gpu_CommonData {
	int gpu_type;
};

layout(std430, binding = 1) buffer gpu_CommonBuffer {
	GPU_SSBO gpu_Data[];
};

out vec4 f_TintColor;
out vec2 f_TexCoord;
out flat int f_type;

void main()
{
	GPU_SSBO vertex =  gpu_Data[gl_InstanceID];

	f_TintColor = vertex.gpu_TintColor / 255.0;
	f_TexCoord  = vertex.gpu_Src.xy + aTexCoord * vertex.gpu_Src.zw;
	gl_Position = vertex.gpu_ViewProjection * vertex.gpu_Model * vec4(aPos, 0.0, 1.0);
	
	f_type      = gpu_type;
}
