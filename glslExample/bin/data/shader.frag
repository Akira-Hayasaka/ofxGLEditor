#pragma include "shaders/easings.frag"

uniform vec3      iResolution;
uniform float     iGlobalTime; 
uniform float     iChannelTime[4];  
uniform vec4      iMouse;           
uniform sampler2D iChannel0;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 q = uv - vec2(0.5,0.7);
   
	float pct =  bounceInOut(abs(fract(iGlobalTime*0.1)*2.0-01.) );

    vec3 color = vec3(uv,0.5+0.5*cos(uv.y*iGlobalTime* (backInOut(abs(sin(iGlobalTime*0.04)))*0.088)));
	
	float r = 0.75*sin(atan(q.y)*iGlobalTime) + .50 * q.y;
    vec3 color2 = color*vec3(r*8.0);

	vec3 tex = mix(color,color2,pct);    

	tex.rgb = tex.grb;
	gl_FragColor = vec4(tex,1.0);
}

