#version 460 core
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

struct S_CommonText {
	vec4 v_TintColor;
	mat4 v_ViewProjection;
	mat4 v_Model;
};

layout(std430, binding = 0) buffer v_CommonBuffer {
	S_CommonText v_Data;
};

out vec4 f_TintColor;
out vec2 f_TexCoord;

void main()
{
	S_CommonText vertex = v_Data;

	f_TintColor = vertex.v_TintColor;
	f_TexCoord  = aTexCoord;
	gl_Position = vertex.v_ViewProjection * vertex.v_Model * vec4(aPos, 0.0, 1.0);
}
