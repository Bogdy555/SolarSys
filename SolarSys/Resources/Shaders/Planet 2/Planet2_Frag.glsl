uniform vec4 u_Color;

vec3 colPalette(float t)
{
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.3, 0.3, 0.3);
    vec3 c = vec3(0.1, 1.0, 1.0);
    vec3 d = vec3(0.463, 0.216, 0.157);
    
    return a + b * cos( 3.28318 * (c*t+d) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    
    vec2 uv2 = fract(2.0 * uv) - 0.5;
    
    float dist = length(uv);
        
    if (dist < 1.0)
    {
    dist = (2.0 - length(uv2)) * 1.6;
    vec3 col = colPalette(dist + iTime);
    fragColor = vec4(col, 1.0f);
    }
}