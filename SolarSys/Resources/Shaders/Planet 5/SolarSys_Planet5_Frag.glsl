#version 330



in vec2 vf_Position;
in vec2 vf_TextureCoords;



uniform mat4 u_Mvp;
uniform vec4 u_Color;
uniform float u_ElapsedTime;



out vec4 f_Color;



vec3 BackgroundColor(vec2 _UV, float _Iteration)
{
	float _Length = abs((1.0f - length(_UV)) * sin(0.5 * (u_ElapsedTime + _Iteration)));

	vec3 _OriginalColor1 = vec3(0.2f, 0.8f, 0.6f);
	vec3 _OriginalColor2 = vec3(0.6f, 0.2f, 0.8f);

	vec3 _Color1 = mix(_OriginalColor1, _OriginalColor2, abs(sin((u_ElapsedTime + _Iteration))));
	vec3 _Color2 = mix(_OriginalColor2, _OriginalColor1, abs(sin((u_ElapsedTime + _Iteration))));

	if (_Length < 0.5f)
	{
		return mix(_Color1, _Color2, _Length / 0.5f) * 3.0f + vec3(sin((u_ElapsedTime + _Iteration)), 0.0f, 0.0f);
	}

	return mix(_Color2, _Color1, (_Length - 0.5f) / (1.0f - 0.5f)) + vec3(1.0f);
}

float BackgroundMask(vec2 _UV, float _Iteration)
{
	float _BackgroundAmbiental = 0.8f;

	return pow(clamp((1.0f - length(_UV)) * 1.8f * sin((u_ElapsedTime + _Iteration) * 2.0f), 0.0f, 1.0f - _BackgroundAmbiental) + _BackgroundAmbiental, 10.0f);
}



void main()
{
	vec2 _CenterUV = vf_TextureCoords * 2.0f - 1.0f;

	float _UVLength = length(_CenterUV);

	if (_UVLength > 1.0f)
	{
		discard;
	}

	vec2 _UV = _CenterUV;
	vec2 _OriginalUV = _CenterUV;

	// Cerc mare

	float _Iteration = 0.0f;
	vec3 _Background = BackgroundColor(_UV, _Iteration) * BackgroundMask(_UV, _Iteration) * 0.2f;

	// Layer din spate

	_UV *= 5.0f * 3.14f / 2.0f;
	_UV.x = sin(_UV.x + u_ElapsedTime);
	_UV.y = sin(_UV.y + sin(2.0f * u_ElapsedTime));

	_Iteration += 1.0f;
	_Background += BackgroundColor(_UV, _Iteration) * BackgroundMask(_UV, _Iteration) * 0.4f;

	// Macro layer

	_UV *= 2.0f * 3.14f / 2.0f;
	_UV.x = sin(_UV.x + sin(0.2f * u_ElapsedTime));
	_UV.y = sin(_UV.y + sin(0.2f * u_ElapsedTime));

	_Iteration += 1.0f;
	_Background += BackgroundColor(_UV, _Iteration) * BackgroundMask(_UV, _Iteration) * 0.4f;

	// Particule floating

	_UV = _OriginalUV;
	_UV *= 2.0f * 3.14f / 2.0f;
	_UV.x = sin(_UV.x - 0.1f * u_ElapsedTime);
	_UV.y = sin(_UV.y - 0.1f * u_ElapsedTime);

	_Iteration += 1.0f;
	_Background += BackgroundColor(_UV, _Iteration) * BackgroundMask(_UV, _Iteration) * 0.2f;

	f_Color = u_Color * vec4(_Background, 1.0f);
}
