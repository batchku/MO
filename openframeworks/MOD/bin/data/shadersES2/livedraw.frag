precision highp float;

uniform sampler2D tex0;
uniform sampler2D maskTex;

uniform float thresh;
uniform float softness;
uniform float invert;
uniform float opacity;

varying vec2 texCoordVarying;

void main()
{
	// Get color value from
    vec3 src = texture2D(tex0, texCoordVarying).rgb;

    // Thresholding
    float fValue = src.r*0.29+src.g*0.6+src.b*0.11;
    float l1 = thresh - softness * 0.5;
    float l2 = l1 + softness; //thresh + softness * 0.5;
    fValue = smoothstep(max(l1,0.0), min(l2, 1.0), fValue);
    fValue = float(invert) * (1.-fValue) + (1.-float(invert))*fValue;

    // Get alpha value
    float mask = texture2D(maskTex, texCoordVarying).a;

    // combine mask and threshold
    vec4 calc=vec4(fValue*opacity,fValue*opacity,fValue*opacity,fValue*mask*opacity);

    // Set
    gl_FragColor = calc;
}
