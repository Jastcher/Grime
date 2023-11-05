#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 margin;
uniform vec2 offset;
uniform mat4 camera;
uniform mat4 model;

void main()
{
	gl_Position = camera * model * vec4(aPos + margin * gl_InstanceID + offset , 0.0f, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
	gl_FragColor = vec4(color, 1.0f);
}

