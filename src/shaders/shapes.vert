uniform vec4 translationVec;
uniform mat4 rotation;
uniform mat4 scale;

in vec4 vPosition;
in vec4 in_Color;

varying vec4 ex_Color;

void
main() {

vec4 offset;
offset[0] = 60.0;
offset[1] = 60.0;
gl_Position =  gl_ModelViewProjectionMatrix * scale * rotation * (translationVec + vPosition);
ex_Color = in_Color;
}