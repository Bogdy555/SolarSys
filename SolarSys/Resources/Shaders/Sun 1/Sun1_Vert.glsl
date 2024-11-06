uniform vec4 u_Color;

vec3 colPalette(float t)
{
    vec3 a = vec3(0.3, 0.6, 0.8);
    vec3 b = vec3(0.3, 0.1, 0.3);
    vec3 c = vec3(0.1, 1.0, 0.3);
    vec3 d = vec3(0.463, 0.816, 0.157);
    
    return a + b * cos( 3.28318 * (c*t+d) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    
    float dist = length(uv);
    
    vec3 col = colPalette(3.0 *(dist + iTime));
    
    if (dist < 0.4)
    {
        fragColor = vec4(col, 1.0f);
    }
    if (dist > 0.4 && dist <= 1.0)
    {
        float gradFactor = smoothstep(0.4, 1.0, dist);
        col = mix(col, vec3(1.0f, 1.0f, 1.0f), gradFactor); 
        fragColor = vec4(col ,1.0f);
    }
}