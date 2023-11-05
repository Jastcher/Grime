#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 camera;

void main()
{
	gl_Position = camera * vec4(aPos, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
	gl_FragColor = vec4(color, 1.0f);
}

