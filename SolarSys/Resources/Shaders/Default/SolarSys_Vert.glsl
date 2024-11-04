#version 330



layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in vec2 v_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec2 u_TextureSize;
uniform vec2 u_TexturePosition;



out vec3 vf_Position;
out vec4 vf_Color;
out vec2 vf_TextureCoords;



void main()
{
	vf_Position = v_Position;
	vf_Color = v_Color;
	vf_TextureCoords = v_TextureCoords;

	gl_Position = u_Mvp * vec4(v_Position, 1.0f);
}
