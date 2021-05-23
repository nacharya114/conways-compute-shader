#version 430 core

in vec2 TexCoord;

uniform sampler2D myTexture;


void main()
{
    FragColor =texture(myTexture, TexCoord);
}