
uniform sampler2DRect terrainMask;
uniform sampler2DRect radarMask;
uniform sampler2DRect background;

uniform vec3 radarColor;

uniform float time;
uniform float resolution;

void main(void){

	vec2 st = gl_FragCoord.xy;

    vec3 bg = texture2DRect(background,st*vec2(resolution)).rgb;
	vec3 radar = texture2DRect(radarMask,st*vec2(resolution)).rgb;
	float mask = texture2DRect(terrainMask,st*vec2(resolution)).r;
                
    gl_FragColor.rgb = mix(bg,mix(bg,radarColor,radar.r ),mask);
    gl_FragColor.a = 1.0;
}