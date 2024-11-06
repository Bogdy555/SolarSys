#version 330



in vec2 vf_Position;
in vec2 vf_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;



out vec4 f_Color;



void main()
{
	vec2 _CenterUV = vf_TextureCoords * 2.0f - 1.0f;

	if (length(_CenterUV) > 1.0f)
	{
		discard;
	}

	f_Color = u_Color;
}
