varying vec4 vPos;
varying vec3 ePos;

varying vec3 norm;

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vPos = gl_Vertex;

	norm = gl_NormalMatrix * gl_Normal;
	norm = norm;// * .5 + .5;

	vec4 ecPosition = gl_ModelViewMatrix * vPos;
	ePos = normalize(ecPosition.xyz/ecPosition.w);

	gl_Position = gl_ProjectionMatrix * ecPosition;
	gl_FrontColor = gl_Color;
}