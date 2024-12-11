#version 460 core

in vec4 f_TintColor;
in vec2 f_TexCoord;
in flat int f_type;
layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	switch (f_type)
	{
		case 0:
			FragColor = texture(u_Texture, f_TexCoord) * f_TintColor;
			break;

		case 1:
			FragColor = (texture(u_Texture, vec2(f_TexCoord.x, 1.0 - f_TexCoord.y))) * f_TintColor;
			break;

		case 2:
			float color = texture(u_Texture, f_TexCoord).r;
			FragColor = vec4(color, color, color, color) * f_TintColor;
			break;
	}
}
