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

	vec2 uv = gl_TexCoord[0].xy;
	vec3 radar = texture2DRect(radarMsk,uv*vec2(resolution)).rgb;
  	vec3 mask = texture2DRect(terrainMask,uv*vec2(resolution)).rgb;

	vertexPos = gl_Vertex;
	vertexNormal = normalize(gl_NormalMatrix * gl_Normal);

	eyeSpaceVertexPos = gl_ModelViewMatrix * gl_Vertex;

	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient + gl_FrontMaterial.emission;

	ambientGlobal.rgb = radar*mask.r;

	gl_Position = ftransform();
	gl_FrontColor = ambientGlobal;
}