#version 330



in vec2 vf_Position;
in vec2 vf_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;
uniform float u_ElapsedTime;



out vec4 f_Color;



vec3 ColorPalette(float _Time)
{
	vec3 a = vec3(0.9f, 0.6f, 0.8f);
	vec3 b = vec3(0.9f, 0.1f, 0.3f);
	vec3 c = vec3(0.8f, 0.0f, 1.0f);
	vec3 d = vec3(0.863f, 0.116f, 0.257f);

	return a + b * cos(5.28318f * (c * _Time + d));
}



void main()
{
	vec2 _CenterUV = vf_TextureCoords * 2.0f - 1.0f;

	float _UVLength = length(_CenterUV);

	if (_UVLength > 1.0f)
	{
		discard;
	}

	if (_UVLength <= 0.4f)
	{
		f_Color = u_Color * vec4(ColorPalette((2.0f - length(sin(u_ElapsedTime) * fract(5.0f * _CenterUV) + 0.5f)) * 1.6f + u_ElapsedTime), 1.0f);

		return;
	}

	f_Color = u_Color * vec4(ColorPalette((2.0f - length(sin(u_ElapsedTime) * fract(5.0f * _CenterUV) + 0.5f)) * 1.6f + u_ElapsedTime), mix(1.0f, 0.0f, smoothstep(0.4f, 1.0f, _UVLength)));
}
