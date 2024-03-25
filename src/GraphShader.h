#pragma once
#include <string>
#include <unordered_map>

static std::string GetEmptyGLSL()
{
	std::string vertex = R""""(
      #version 330 core
      void main() {}
  )"""";
	return vertex;
}

static std::string GetVertex()
{

	std::string vertex = R""""(
#version 330 core

vec3 plane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);


uniform mat4 view;
uniform mat4 proj;

out vec2 point;

vec2 UnprojectPoint(float x, float y, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, 1.0, 1.0);
    return unprojectedPoint.xy / unprojectedPoint.w;
}
void main()
{
    vec3 p = plane[gl_VertexID];
    point = UnprojectPoint(p.x, p.y, view, proj);

    gl_Position = vec4(p, 1.0);
}
    )"""";

	return vertex;
}
static std::string GetFragment(const std::string& eq, const std::unordered_map<std::string, double>& variables)
{
	std::string variableString;

	for(const auto& var : variables) { variableString += "uniform float " + var.first + ";"; }

	std::string fragment = R""""(
#version 330 core

	out vec4 FragColor;

uniform vec3 graphColor;
uniform float thickness;
uniform vec2 resolution;
uniform float fov;
uniform vec2 translation;

uniform float time;

    in vec2 point;

  )"""" +
	    variableString + R""""(


	float F(float x, float y)
	{
    return )"""" +
	    eq + R""""(;
	}

	void main()
	{
		//vec2 uv = gl_FragCoord.xy / resolution.xy - vec2(0.5) + translation/fov;
    vec2 uv = point;

		float aspect = resolution.x / resolution.y;

		float x = uv.x;
		float y = uv.y;

		float eq = F(x, y);
		float pixelSize = 1.0 / resolution.x;

		if(eq < pixelSize * thickness * fov && eq > -pixelSize * thickness * fov)
		{
			gl_FragColor = (vec4(graphColor, 1.0));
			return;
		}
		//gl_FragColor = (vec4(0.0,0.0,0.0, 1.0));

		discard;
	}
  )"""";

	return fragment;
}
