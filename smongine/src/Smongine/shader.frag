#version 330 core
out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D texSampler;

void main()
{
    fragColour = texture(texSampler, texCoords);
}