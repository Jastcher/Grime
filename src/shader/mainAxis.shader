#shader vertex
#version 330 core

uniform mat4 view;
uniform mat4 proj;

uniform vec3 Xcolor;
uniform vec3 Ycolor;

out vec3 lineColor;

// Grid position are in xy clipped space
vec3 axisVertices[4] = vec3[](
    vec3(-1, 0, 0), vec3(1, 0, 0),
    vec3(0, -1, 0), vec3(0, 1, 0)
);

void main()
{

    mat4 newView = view;
    mat4 newProj = proj;
    if (gl_VertexID < 2) {
        newView[3][0] = 0.0;
        newProj[0][0] = 1.0;
        lineColor = Xcolor;
    } else {
        newView[3][1] = 0.0;
        newProj[1][1] = 1.0;
        lineColor = Ycolor;
    }

    gl_Position = newProj * newView * vec4(axisVertices[gl_VertexID], 1.0);
}


#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 lineColor;

void main() {
    gl_FragColor = vec4(lineColor, 1.0);
}
