#version 460 core

in vec2 f_TexCoord; // Texture coordinates
layout(location = 0) out vec4 FragColor; // Output color

layout(binding = 0) uniform sampler2D u_Texture;

float rand(vec2 co)
{
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec2 texResolution = vec2(256.0, 240.0); // Texture resolution
	float pixelSize = 8.0;

	vec2 pixelSizeUV = pixelSize / texResolution;

	// Quantize the texture coordinates
	vec2 quantizedCoord = floor(f_TexCoord / pixelSizeUV);

	// Generate noise for each channel
	float noiseR = rand(quantizedCoord * 123.0); // Red channel noise
	float noiseG = rand(quantizedCoord * 321.0); // Green channel noise
	float noiseB = rand(quantizedCoord * 213.0); // Blue channel noise

	// Sample the red channel from the texture
	float texRed = texture(u_Texture, f_TexCoord).r;

	// Generate pastel psychedelic colors
	float r = texRed * (0.5 + noiseR * 0.5);
	float g = texRed * (0.5 + noiseG * 0.5);
	float b = texRed * (0.5 + noiseB * 0.5);

	// Output the color
	FragColor = vec4(1, g, 1, texRed);
}
