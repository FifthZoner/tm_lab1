uniform sampler2D texture;
uniform float r0;
uniform float r1;
uniform float r2;
uniform float r3;
uniform float r4;
uniform float r5;
uniform float r6;
uniform float r7;
uniform float r8;
uniform float sizeX;
uniform float sizeY;

void main()
{
    vec2 coord = gl_FragCoord.xy;
    vec3 F = vec3(coord.x, coord.y, 1.0f);

    float FpX = r0 * F.x + r1 * F.y + r2 * F.z;
    float FpY = r3 * F.x + r4 * F.y + r5 * F.z;
    float FpZ = r6 * F.x + r7 * F.y + r8 * F.z;

    float i = FpX / FpZ;
    float j = FpY / FpZ;

    if (i >= 0.f && i < sizeX && j >= 0.f && j < sizeY) {
        gl_FragColor = texture2D(texture, vec2(float(i) / float(sizeX), float(j) / float(sizeY)));
    } else {
        gl_FragColor = vec4(1.f, 1.f, 1.f, 1.f);
    }
}
