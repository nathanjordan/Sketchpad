uniform vec2 translation;
uniform vec2 rotation;
uniform vec2 scale;

in vec4 vPosition;
in vec4 in_Color;

varying vec4 ex_Color;

void main() {
	
	gl_Position =  gl_ModelViewMatrix * (vPosition + vec4( translation[0] , translation[1] , 0.0 , 0.0 ) );
	
	ex_Color = in_Color;
	
	}