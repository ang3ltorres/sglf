#version 460 core

in vec4 f_TintColor;
in vec2 f_TexCoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	float color = texture(u_Texture, f_TexCoord).r;
	FragColor = vec4(color, color, color, color) * f_TintColor;
}
