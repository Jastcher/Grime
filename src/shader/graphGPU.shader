#shader vertex
#version 330 core

uniform vec3 graphColor_u;
uniform float thickness_u;
uniform float fov_u;

out vec3 graphColor;
out float thickness;
out float fov;

vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

void main()
{
    
    graphColor = graphColor_u;
    thickness = thickness_u;
    fov = fov_u;

    gl_Position = vec4(axisVertices[gl_VertexID], 1.0);
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 graphColor;
in float thickness;
in vec2 resolution;
in float fov;

float F(float x, float y) {
    return y - x;
    //eq = pow(x,2.0) + pow(y,2.0) - abs(sin(iTime/2.0))/5.0;
  }

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy - vec2(0.5);

    float aspect = resolution.y/resolution.x;

    float x = uv.x * fov;
    float y = uv.y * aspect * fov;
    
    float eq = F(x, y);
    float pixelSize = 1.0/resolution.x;

    gl_FragColor = vec4(1.0);
    return;
    if (eq < pixelSize * 30.0 && eq > -pixelSize * 30.0)
    {
        gl_FragColor = (vec4(graphColor, 1.0));
        return;
    }

    discard;
}
