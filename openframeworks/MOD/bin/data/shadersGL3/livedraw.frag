#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

uniform float thresh;
uniform float softness;
uniform float invert;

varying vec2 texCoordVarying;

void main()
{
    // Get color value from
    vec3 src = texture2DRect(tex0, texCoordVarying).rgb;

    // Get alpha value
    float mask = texture2DRect(maskTex, texCoordVarying).r;

    // Set
    gl_FragColor = vec4(src , mask);
}