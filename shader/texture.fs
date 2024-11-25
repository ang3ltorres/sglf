#version 460 core

in vec4 f_TintColor;
in vec2 f_TexCoord;
in flat int f_FlipY;
layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	if (f_FlipY == 0)
		FragColor = texture(u_Texture, f_TexCoord) * f_TintColor;
	else
		FragColor = (texture(u_Texture, vec2(f_TexCoord.x, 1.0 - f_TexCoord.y))) * f_TintColor;
}
