#ifdef VERTEX_SHADER
layout(location=0)in vec4 mVertex;
layout(location=1)in vec4 mInfo;
layout(location=2)in vec4 mColor;
layout(location=3)in vec4 mColor2;

uniform float uWidth;
uniform float uHeight;

out vec4 vColor;
out vec4 vInfo;
out vec2 vPosition;

void main(){
    vInfo = mInfo;
    // vInfo = vec4(0,0,150, 20);
    vColor = mColor.abgr;
    vPosition = vec2(mVertex.x*vInfo.z, mVertex.y*vInfo.w);
    gl_Position = vec4((vPosition+floor(vInfo.xy))/vec2(uWidth/2, uHeight/2)-vec2(1), 0, 1);
    // gl_Position = vec4(vPosition, 0, 1);
}

#endif

#ifdef FRAGMENT_SHADER
out vec4 outColor;

in vec4 vColor;
in vec4 vInfo;
in vec2 vPosition;

void main(void){
    outColor = vColor;
}

#endif
