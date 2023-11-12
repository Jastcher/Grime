#shader vertex
#version 330 core

uniform mat4 view;
uniform mat4 proj;

out vec3 nearPoint;

vec3 plane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, 1.0, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}
void main()
{
    vec3 p = plane[gl_VertexID].xyz;
    nearPoint = UnprojectPoint( p.x, p.y, view, proj).xyz; // unprojecting on the near plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates}
}


#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec2 resolution;

uniform float zoom;

in vec3 nearPoint;

float Plot(vec2 st, float pct){
    float width = 1.0 / resolution.x * 10;
    return smoothstep(pct - width, pct, st.y) - smoothstep(pct, pct + width, st.y);
}

void main() {
    float x = nearPoint.x;
    float y = pow(x, 2.0);

    vec3 color = vec3(1.0);

    float value = Plot(nearPoint.xy, y);
    if (value == 0.0) discard;
    else value = 1.0;

    vec3 line = color * value;

    gl_FragColor = vec4(line, 1.0);
}
