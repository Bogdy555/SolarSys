uniform vec4 u_Color;

vec3 colPalette(float t)
{
    vec3 a = vec3(0.9, 0.6, 0.8);
    vec3 b = vec3(0.9, 0.1, 0.3);
    vec3 c = vec3(0.8, 0.0, 1.0);
    vec3 d = vec3(0.863, 0.116, 0.257);
    
    return a + b * cos( 5.28318 * (c*t+d) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    
    vec2 uv2 = sin(iTime) * fract(5.0 * uv) + 0.5;
    
    float dist = length(uv);
        
    if (dist < 0.4)
    {
        float distAux = (2.0 - length(uv2)) * 1.6;
        vec3 col = colPalette(distAux + iTime);
        fragColor = vec4(col, 1.0f);
    }
        
    if (dist < 1.0 && dist >= 0.4)
    {
        float distAux = (2.0 - length(uv2)) * 1.6;
        vec3 col = colPalette(distAux + iTime);
        float gradFactor = smoothstep(0.4, 1.0, dist);
        col = mix(col, vec3(1.0f, 1.0f, 1.0f), gradFactor); 
        fragColor = vec4(col ,1.0f);
    }
}