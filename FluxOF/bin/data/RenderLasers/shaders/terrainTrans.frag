// uniform sampler2DRect backbuffer;
// uniform sampler2DRect normalMap;
uniform sampler2DRect depthMap;
uniform sampler2DRect lasersMask;
uniform sampler2DRect previusMap;

uniform vec3 dstColor1;
uniform vec3 dstColor2;
uniform vec3 dstColor3;
uniform vec3 dstColor4;
uniform vec3 dstColor5;

uniform float time;
uniform float resolution;

#define PI 3.14159265358979323846

void main(void){
	vec2 st = gl_FragCoord.xy;

	vec3 zones = texture2DRect(lasersMask,st).rgb;
	float depth = texture2DRect(depthMap,st).r;
    
    vec4 bg = texture2DRect(previusMap, st);
    
    vec3 tint = vec3(0.0);

    vec3 colors[5];
    colors[0] = dstColor1;
    colors[1] = dstColor2;
    colors[2] = dstColor3;
    colors[3] = dstColor4;
    colors[4] = dstColor5;

    gl_FragColor = bg;

    if (zones.x>0.0 && zones.y>0.0){
        float index = depth*3.0;

        float fIndex = floor(index);
        float mIndex = mod(index,fIndex);

        vec3 tint = mix(colors[int(index)],colors[int(index)+1],mIndex);
        gl_FragColor.rgb = tint;
    } else if (zones.x>0.0){
        float index = depth*2.0;

        float fIndex = floor(index);
        float mIndex = mod(index,fIndex);

        vec3 tint = mix(colors[int(index)],colors[int(index)+1],mIndex);
        gl_FragColor.rgb = tint;
    } else if (zones.y>0.0){
        float index = depth*2.0+2.0;

        float fIndex = floor(index);
        float mIndex = mod(index,fIndex);

        vec3 tint = mix(colors[int(index)],colors[int(index)+1],mIndex);
        gl_FragColor.rgb = tint;
    }
    
    gl_FragColor.a = 1.0;
}