#include "shader.h"

// Reads a text file and outputs a string with everything in the text file
ShaderProgramSource GetFileContents(const char* filepath)
{
	std::fstream stream;
	stream.open(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	if (stream) {
		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}

			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		stream.close();
		return { ss[0].str(), ss[1].str() };
	}
    std::cout << "FILE COULD NOT BE OPENED" << std::endl;
	stream.close();
	throw(errno);
}

Shader::Shader(const char* fileName) {
	ShaderProgramSource source = GetFileContents(fileName);
	std::string vertexCode = source.VertexSource;
	std::string fragmentCode = source.FragmentSource;

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//Creates a  vertex shader object - shader type
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Replaces the source code in a shader object - shader, count, shaderSourceCode, length
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//Compiles a shader object - shader
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX", fileName);

	//Creates a  vertex shader object - shader type
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Replaces the source code in a shader object - shader, count, shaderSourceCode, length
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//Compiles a shader object - shader
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT", fileName);

	//Creates a program object, A program object is an object to which shader objects can be attached
	ID = glCreateProgram();
	//Attaches a vertex shader object to a program object
	glAttachShader(ID, vertexShader);
	//Attaches a vertex shader object to a program object
	glAttachShader(ID, fragmentShader);
	//Links a program objec - program
	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM", fileName);

	//Deletes a shader object - shader (frees the momory)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	//Installs a program object as part of current rendering state - program
	glUseProgram(ID);
}

void Shader::Delete() {
	//Deletes a program object - program
	glDeleteProgram(ID);
}

void Shader::SetInt(const char* uniform, int unit)
{
	glUniform1i(glGetUniformLocation(ID, uniform), unit);
}

void Shader::SetFloat(const char* uniform, float unit) 
{
	glUniform1f(glGetUniformLocation(ID, uniform), unit);
}

void Shader::SetMat4(const char* uniform, glm::mat4 unit) 
{
	glUniformMatrix4fv(glGetUniformLocation(ID, uniform), 1, GL_FALSE, glm::value_ptr(unit));
}

void Shader::SetVec4(const char* uniform, glm::vec4 unit) 
{
	glUniform4f(glGetUniformLocation(ID, uniform), unit.x, unit.y, unit.z, unit.w);

}

void Shader::SetVec3(const char* uniform, glm::vec3 unit) 
{
	glUniform3f(glGetUniformLocation(ID, uniform), unit.x, unit.y, unit.z);

}

void Shader::SetVec2(const char* uniform, glm::vec2 unit) 
{
	glUniform2f(glGetUniformLocation(ID, uniform), unit.x, unit.y);

}



void Shader::CompileErrors(unsigned int shader, const char* type, const char* shaderName) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		//Returns a parameter from a shader object - shader, pname, *params
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			//Returns the information log for a shader object - shader, maxLength, length, infoLog
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << shaderName << ": SHADER COMPILATION ERROR:  " << type << std::endl;
		}
	}
	else {
		//Returns a parameter from a shader object - shader, pname, *params
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			//Returns the information log for a shader object - shader, maxLength, length, infoLog
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << shaderName<< ": SHADER LINKING ERROR FOR: " << type << std::endl;
		}
	}
}

