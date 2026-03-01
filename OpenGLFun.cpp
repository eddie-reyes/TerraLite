#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType {
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

static inline ShaderProgramSource ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	if (!stream.is_open()) {
		std::cerr << "Failed to open shader file: " << filepath << '\n';
		return {};
	}

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

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
			ss[(int)type] << line << '\n';

		}
	}

	ShaderProgramSource source;
	source.VertexSource = ss[0].str();
	source.FragmentSource = ss[1].str();

	return source;
}

static inline unsigned int CompileShader(unsigned int type, const std::string& source) {

	//create vertex shader
	unsigned int id = glCreateShader(type);

	//cast to char array
	const char* src = source.c_str();

	//bind the source code to the shader object
	glShaderSource(id, 1, &src, nullptr);

	//compile the shader
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << "Failed to compile " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader!\n" << message << '\n';
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static inline unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){

	//create program for GPU
	unsigned int program = glCreateProgram();
	//create vertex and fragment shader
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach the shaders to the program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//delete the shaders since they are now linked to the program and no longer needed on their own
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{

	std::cout << "Starting program...\n";

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWwindow * handle = glfwCreateWindow(640, 480, "Having Fun", nullptr, nullptr);

	if (!handle)
	{
		std::cerr << "Failed to create GLFW window, cannot proceed!\n";
		return -11;
	}

	std::cout << "GLFW v" << GLFW_VERSION_MAJOR << '.' << GLFW_VERSION_MINOR << " Initalized Window\n";

	glfwMakeContextCurrent(handle);
	int loadGLresult = gladLoadGL(glfwGetProcAddress);

	if (!loadGLresult) {
		std::cerr << "Failed to initialize GLAD, cannot proceed!\n";
		return -12;
	}
	
	std::cout << "glad v" << GLAD_VERSION_MAJOR(loadGLresult) << '.' << GLAD_VERSION_MINOR(loadGLresult) << " Started\n";

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.0f, 0.5f, // 1
		 0.5f,  -0.5f, // 2

	};

	//holds memory location for buffer
	unsigned int buffer;

	//init buffer and bind it to variable
	glGenBuffers(1, &buffer);

	//tell gl to bind the buffer to the GL_ARRAY_BUFFER since we are drawing an array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//copy the data from our positions array into the buffer, we specify the size of the data and how we want to use it (static draw since it wont be changed during runtime)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //enable the vertex attribute at index 0, this is the position attribute in our shader

	//start at idx 0
	//position attribute has 2 components
	//type of position attribute is float
	//dont normalize the data since its already in the range of -1 to 1
	//size of each vertex is 2 floats
	//offset is 0 since position data starts at the beginning of the buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	ShaderProgramSource source = ParseShader("shaders/basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(handle))
	{
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		//tell gl how to interpret the data in the buffer, offset is 0
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(handle);
		glfwPollEvents();

	}

	glDeleteProgram(shader);


}
