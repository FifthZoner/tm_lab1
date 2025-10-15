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
uniform bool linear;

void main()
{
    // zdobywanie koordynatów w obrazie
    vec2 coord = gl_FragCoord.xy;
    vec3 F = vec3(coord.x, coord.y, 1.0f);

    // wyliczanie wektora przekształceń
    float FpX = r0 * F.x + r1 * F.y + r2 * F.z;
    float FpY = r3 * F.x + r4 * F.y + r5 * F.z;
    float FpZ = r6 * F.x + r7 * F.y + r8 * F.z;

    // wyliczanie koordynatów w obrazie wejściowym
    float i = FpX / FpZ;
    float j = FpY / FpZ;

    if (i >= 0.f && i < sizeX && j >= 0.f && j < sizeY) {
        if (linear) {
            // Wyliczanie koordynatów całkowitych
            int x0 = int(floor(i));
            int y0 = int(floor(j));
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            // Wyliczanie przesunięć dla interpolacji
            float a = i - float(x0);
            float b = j - float(y0);

            // Odczytywanie kolorów z czterech najbliższych pikseli
            vec4 color00 = texture2D(texture, vec2(float(x0) / float(sizeX), float(y0) / float(sizeY)));
            vec4 color10 = texture2D(texture, vec2(float(x1) / float(sizeX), float(y0) / float(sizeY)));
            vec4 color01 = texture2D(texture, vec2(float(x0) / float(sizeX), float(y1) / float(sizeY)));
            vec4 color11 = texture2D(texture, vec2(float(x1) / float(sizeX), float(y1) / float(sizeY)));

            // Interpolacja liniowa
            vec4 color = (1.0 - a) * (1.0 - b) * color00 +
            a * (1.0 - b) * color10 +
            (1.0 - a) * b * color01 +
            a * b * color11;

            gl_FragColor = color;
        }
        else {
            gl_FragColor = texture2D(texture, vec2(float(i) / float(sizeX), float(j) / float(sizeY)));
        }



    }
    else {
        gl_FragColor = vec4(1.f, 1.f, 1.f, 1.f); // Kolor w przypadku wyjścia poza zakres
    }
}
