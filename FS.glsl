#version 330

in vec4 frag_color;
 
const int MaxItr = 150;
const float zoom = 2.2; 
const vec2 move = vec2(1.5,0);
uniform float time;

float mandelbroat(vec2 uv)
{
    vec2 c = uv / zoom - move;
    //c /= pow(time,4);
    //c -= vec2(0.65,0.45);
    vec2 z = vec2(0,0);

    for (int i = 0; i < MaxItr; i++)
    {
        z = vec2(z.x*z.x - z.y*z.y, 2 * z.x*z.y);
        z += c;
        if(dot(z,z) > 4) return float(i)/MaxItr;
    }
    return 0;
}

void main()
{
    vec3 color = vec3(0);
    color += mandelbroat(frag_color.xy);
    gl_FragColor = vec4(color,1);
}