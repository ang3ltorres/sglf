#version 460 core
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

struct S_CommonBitmask {
	mat4 v_ViewProjection;
	mat4 v_Model;
};

layout(std430, binding = 0) buffer v_CommonBuffer {
	S_CommonBitmask v_Data;
};

out vec2 f_TexCoord;

void main()
{
	S_CommonBitmask vertex = v_Data;

	f_TexCoord  = aTexCoord;
	gl_Position = vertex.v_ViewProjection * vertex.v_Model * vec4(aPos, 0.0, 1.0);
}
