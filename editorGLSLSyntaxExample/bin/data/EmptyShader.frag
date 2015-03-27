uniform vec3      iResolution;  
uniform float     iGlobalTime; 
uniform float     iChannelTime[4];  
uniform vec4      iMouse;           
uniform sampler2D iChannel0;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 q = uv - vec2(0.5,0.5);
   
    vec3 color = vec3(uv,0.5+0.5*sin(iGlobalTime));

    float r = 0.12 + 0.3* cos(atan(q.x, q.y) * 600.0 + (sin(iGlobalTime*0.75)*16000.0) * q.x + 0.1);
//    color *= smoothstep(r, r+0.2, length(q));
    gl_FragColor = vec4(color,1.0);
}
