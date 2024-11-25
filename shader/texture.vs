#version 460 core
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

struct S_CommonTexture {
	vec2 v_SrcOffset;
	vec2 v_SrcSize;
	vec4 v_TintColor;
	mat4 v_ViewProjection;
	mat4 v_Model;
	int  v_FlipY;
};

layout(std430, binding = 0) buffer v_CommonBuffer {
	S_CommonTexture v_Data[];
};

out vec4 f_TintColor;
out vec2 f_TexCoord;
out flat int f_FlipY;

void main()
{
	S_CommonTexture vertex =  v_Data[gl_InstanceID];

	f_TintColor = vertex.v_TintColor;
	f_TexCoord  = vertex.v_SrcOffset + aTexCoord * vertex.v_SrcSize;
	f_FlipY     = vertex.v_FlipY;
	gl_Position = vertex.v_ViewProjection * vertex.v_Model * vec4(aPos, 0.0, 1.0);
}
