uniform vec4 u_Color;

vec3 colPalette(float t)
{
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.8, 0.3, 0.3);
    vec3 c = vec3(0.8, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    
    return a + b * cos( 3.28318 * (c*t+d) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    
    float dist = (1.0 - length(uv)) * 1.6;
    
    vec3 col = colPalette(dist + iTime);
    col *= dist;

    if(dist > 0.0)
    {
    fragColor = vec4(col, 1.0f);
    }
    else
    {
    fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}