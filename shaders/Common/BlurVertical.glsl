#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;

out vec2 vUV;

void main(){
    vUV = mVertex.zw;
    gl_Position = (vec4(mVertex.xy,0,1));
}


#endif

#ifdef FRAGMENT_SHADER
out vec4 outColor;

uniform sampler2D uTexture;
uniform float uBlurSize;
uniform vec2 uPixelSize;

in vec2 vUV;

void main(void){

    vec4 sum = vec4(0.0);
    vec2 direction = vec2(0, 1*uBlurSize)*uPixelSize;
    sum += texture(uTexture, vUV + direction*4) * 0.05;
    sum += texture(uTexture, vUV + direction*3) * 0.09;
    sum += texture(uTexture, vUV + direction*2) * 0.12;
    sum += texture(uTexture, vUV + direction*1) * 0.15;

    sum += texture(uTexture, vUV + direction*0) * 0.18;

    sum += texture(uTexture, vUV - direction*1) * 0.15;
    sum += texture(uTexture, vUV - direction*2) * 0.12;
    sum += texture(uTexture, vUV - direction*3) * 0.09;
    sum += texture(uTexture, vUV - direction*4) * 0.05;
    outColor = sum;
}

#endif
