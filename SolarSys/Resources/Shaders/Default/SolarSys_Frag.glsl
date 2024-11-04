#version 330



in vec3 vf_Position;
in vec4 vf_Color;
in vec2 vf_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec2 u_TextureSize;
uniform vec2 u_TexturePosition;



out vec4 f_Color;



void main()
{
	f_Color = vf_Color * u_Color * texture(u_Texture, vf_TextureCoords * u_TextureSize + u_TexturePosition);
}
