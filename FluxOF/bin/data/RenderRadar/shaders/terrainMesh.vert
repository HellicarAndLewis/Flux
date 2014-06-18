uniform sampler2DRect radarMsk;
uniform sampler2DRect terrainMask;
uniform sampler2DRect background;

uniform vec3 radarColor;

uniform float resolution;

varying vec4 ambientGlobal, eyeSpaceVertexPos;
varying vec4 vertexPos;
varying vec3 vertexNormal;

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec2 uv = gl_TexCoord[0].xy*vec2(resolution);

	float radar = texture2DRect(radarMsk,uv).r;
	vec3 bg = texture2DRect(background,uv).rgb;

	vertexPos = gl_Vertex;
	vertexNormal = normalize(gl_NormalMatrix * gl_Normal);

	eyeSpaceVertexPos = gl_ModelViewMatrix * gl_Vertex;

	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient + gl_FrontMaterial.emission;

	gl_Position = ftransform();
	gl_FrontColor.rgb = mix(bg,radarColor,radar);
	gl_FrontColor.a = 1.0;
}