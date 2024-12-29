#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

struct S_CommonTexture {
	vec4 v_Src;
	vec4 v_TintColor;
	mat4 v_ViewProjection;
	mat4 v_Model;
};

layout(std140, binding = 0) uniform CommonData {
	int v_type;
};

layout(std430, binding = 1) buffer v_CommonBuffer {
	S_CommonTexture v_Data[];
};

out vec4 f_TintColor;
out vec2 f_TexCoord;
out flat int f_type;

void main()
{
	S_CommonTexture vertex =  v_Data[gl_InstanceID];

	f_TintColor = vertex.v_TintColor / 255.0;
	f_TexCoord  = vertex.v_Src.xy + aTexCoord * vertex.v_Src.zw;
	gl_Position = vertex.v_ViewProjection * vertex.v_Model * vec4(aPos, 0.0, 1.0);
	
	f_type      = v_type;
}
