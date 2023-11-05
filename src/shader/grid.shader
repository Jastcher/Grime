#shader vertex
#version 330 core

uniform mat4 view;
uniform mat4 proj;


out vec3 nearPoint;

// Grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, 1.0, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

// normal vertice projection
void main() {
    vec3 p = gridPlane[gl_VertexID].xyz;
    nearPoint = UnprojectPoint( p.x, p.y, view, proj).xyz; // unprojecting on the near plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates}
}

#shader fragment
#version 330 core

uniform float gridSpacing;
out vec4 FragColor;

in vec3 nearPoint;

vec4 grid(vec3 fragPos3D, float scale, float thickness) {
    vec2 coord = fragPos3D.xy * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord) * thickness;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    return color;
}

void main() {
    gl_FragColor = mix(grid(nearPoint, gridSpacing, 1.0), grid(nearPoint, gridSpacing / 4, 2.0), 0.5);
    //gl_FragColor = grid(nearPoint, gridSpacing, 1.0) * grid(nearPoint, gridSpacing / 4, 5.0);

}
