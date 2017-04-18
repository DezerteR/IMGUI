#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;

void main(){
    gl_Position = vec4(mVertex.xy,0,1);
}

#endif

#ifdef FRAGMENT_SHADER
out vec4 outColor;

uniform float uThreshold;
uniform sampler2D uTexture;

// const vec3 luminance = vec3(1);
const vec3 luminance = vec3(0.299, 0.587, 0.114);

void main(void){
    vec3 a = texelFetch(uTexture, ivec2(gl_FragCoord.xy), 0).rgb;
    outColor.a = 1;
    float x = step(uThreshold, dot(a.rgb, luminance));
    outColor.rgb = clamp(a*x, 0, 10);
    // outColor *= abs(1-step(0.0, -(x));
}

#endif
