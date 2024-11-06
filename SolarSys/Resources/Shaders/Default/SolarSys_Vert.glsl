#version 330



layout (location = 0) in vec2 v_Position;
layout (location = 1) in vec2 v_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;



out vec2 vf_Position;
out vec2 vf_TextureCoords;



void main()
{
	vf_Position = v_Position;
	vf_TextureCoords = v_TextureCoords;

	gl_Position = u_Mvp * vec4(v_Position, 0.0f, 1.0f);
}
