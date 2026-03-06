#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <string>
#include <sstream>

#include "Application.h"

double mouseDeltaX = 0, mouseDeltaY = 0;
double mouseX, mouseY;
double scrollY;
bool isMiddleButtonPressed = false;
bool isScrollActive = false;

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType {
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

static inline void zoomPlane(glm::mat4& in_matrix, double yoffset) {
	
}

static inline void orbitAroundPlane(glm::mat4& View, glm::mat4& Model) {

	if (abs(mouseDeltaX) <= 0.1f && abs(mouseDeltaY) <= 0.1f) return; 

	View = glm::rotate(View, glm::radians((float)mouseDeltaY), glm::vec3(1, 0, 0));

	glm::vec3 rotationMatrix = glm::normalize(glm::vec3(Model * glm::vec4(0, 0, 1, 0)));
	glm::vec3 normal = glm::cross(rotationMatrix, glm::vec3(1, 0, 0)); 
	Model = glm::rotate(Model, glm::radians((float)-mouseDeltaX), normal);

}


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

	return { ss[0].str(), ss[1].str() };
}

//static inline unsigned int CompileShader(unsigned int type, const std::string& source) {
//
//	//create vertex shader
//	unsigned int id = glCreateShader(type);
//
//	//cast to char array
//	const char* src = source.c_str();
//
//	//bind the source code to the shader object
//	glShaderSource(id, 1, &src, nullptr);
//
//	//compile the shader
//	glCompileShader(id);
//
//	int result;
//	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//
//	if (result == GL_FALSE) {
//		int length;
//		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//		std::string message = std::string(length, ' ');
//		glGetShaderInfoLog(id, length, &length, (char*)message.c_str());
//		std::cerr << "Failed to compile " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader!\n" << message << '\n';
//		glDeleteShader(id);
//		return 0;
//	}
//
//	return id;
//}
//
//static inline unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
//
//	//create program for GPU
//	unsigned int program = glCreateProgram();
//	//create vertex and fragment shader
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	//attach the shaders to the program
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);
//	glLinkProgram(program);
//	glValidateProgram(program);
//
//	//delete the shaders since they are now linked to the program and no longer needed on their own
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//
//	return program;
//}

int main(void)
{

	ApplicationSpecification spec;

	spec.WindowWidth = 1920;
	spec.WindowHeight = 1080;
	spec.Name = "Having Fun";
	spec.HeightMapResolution = 512;

	Application app(spec);
	app.Run();

	//std::cout << "Starting program...\n";

	//glfwInit();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	////core profile for modern opengl
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	//GLFWwindow* handle = glfwCreateWindow(1920, 1080, "Having Fun", nullptr, nullptr);

	//if (!handle)
	//{
	//	std::cerr << "Failed to create GLFW window, cannot proceed!\n";
	//	return -11;
	//}

	//std::cout << "GLFW v" << GLFW_VERSION_MAJOR << '.' << GLFW_VERSION_MINOR << " Initalized Window\n";

	//glfwMakeContextCurrent(handle);
	//glfwSwapInterval(1);
	//GLuint loadGLresult = gladLoadGL(glfwGetProcAddress);

	//if (!loadGLresult) {
	//	std::cerr << "Failed to initialize GLAD, cannot proceed!\n";
	//	return -12;
	//}

	//std::cout << "glad v" << GLAD_VERSION_MAJOR(loadGLresult) << '.' << GLAD_VERSION_MINOR(loadGLresult) << " Started\n";

	////vao stores the state of the vertex atrributes for a specific buffer or set of buffers
	////this prevents us from having to set up the vertex attributes every time we want to draw a new peice of geometry, which may or may not have different requirement for its vertices
	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	////all vertex position required for square -- if we drew each individual triangle we would be duplicating verticies
	//float positions[] = {
	//	-0.5f, -0.5f, // 0
	//	 0.5f, -0.5f, // 1
	//	 0.5f,  0.5f, // 2
	//	-0.5f,  0.5f, // 3

	//};

	////we can use indicies to reuse verticies and draw the square with 2 triangles instead of 4 verticies, this is more efficient since we are reusing verticies instead of duplicating them for each triangle
	//unsigned int indicies[] = {
	//	0, 1, 2,
	//	2, 3, 0
	//};

	////holds memory location for buffer
	//GLuint buffer;

	////init buffer and bind it to variable
	//glGenBuffers(1, &buffer);

	////tell gl to bind the buffer to the GL_ARRAY_BUFFER since we are drawing an array of vertices
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);

	////copy the data from our positions array into the buffer, we specify the size of the data and how we want to use it (static draw since it wont be changed during runtime)
	//glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0); //enable the vertex attribute at index 0, this is the position attribute in our shader

	////start at idx 0 - binds vertex attribute to vao object 0
	////position attribute has 2 components
	////type of position attribute is float
	////dont normalize the data since its already in the range of -1 to 1
	////size of each vertex is 2 floats
	////offset is 0 since position data starts at the beginning of the buffer
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//GLuint ibo;
	////create index butter
	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	////copy the data from our indicies array into the index buffer, we specify the size of the data and how we want to use it (static draw since it wont be changed during runtime)
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indicies, GL_STATIC_DRAW);

	//int width, height;
	//glfwGetFramebufferSize(handle, &width, &height); 


	//glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(0, 2, 2), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	//// Model matrix: an identity matrix (model will be at the origin)
	//glm::mat4 Model = glm::mat4(1.0f);
	//Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1, 0, 0)); // Rotate the model 90 degrees around the x-axis to make it face the camera initially
	//Model = glm::rotate(Model, glm::radians(45.0f), glm::vec3(0, 0, 1)); 

	//// Our ModelViewProjection: multiplication of our 3 matrices
	//glm::mat4 mvp = proj * View * Model; // Remember, matrix multiplication is the other way around


	//ShaderProgramSource source = ParseShader("shaders/basic.shader");

	//unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	//glUseProgram(shader);

	//float r = 0.0f;

	////retrieve location of the uniform variable in the shader and set its value to a color (RGBA)
	//int location = glGetUniformLocation(shader, "u_Color");
	//int matrixID = glGetUniformLocation(shader, "MVP");

	//glfwSetMouseButtonCallback(handle, [](GLFWwindow* handle, int button, int action, int mods)
	//	{

	//		switch (action)
	//		{
	//			case GLFW_PRESS:
	//			{
	//				if (button == GLFW_MOUSE_BUTTON_MIDDLE && !isMiddleButtonPressed) { isMiddleButtonPressed = true; }
	//				break;
	//			}
	//			case GLFW_RELEASE:
	//			{
	//				if (button == GLFW_MOUSE_BUTTON_MIDDLE) { isMiddleButtonPressed = false; }
	//				
	//				break;
	//			}
	//		}
	//	});


	//glfwSetScrollCallback(handle, [](GLFWwindow* handle, double xoffset, double yoffset) {
	//		
	//	scrollY = yoffset;
	//	
	//});

	//while (!glfwWindowShouldClose(handle))
	//{
	//	//clear screen
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	//set uniform
	//	glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
	//	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	//	if (r > 1.0f) r = 0.0f;
	//	r += 0.01f;

	//	glfwSwapBuffers(handle);

	//	glfwPollEvents();

	//	if (isMiddleButtonPressed) {
	//	
	//		double currMouseX = mouseX;
	//		double currMouseY = mouseY;
	//		glfwGetCursorPos(handle, &mouseX, &mouseY);
	//		mouseDeltaX = mouseX - currMouseX;
	//		mouseDeltaY = mouseY - currMouseY;

	//		orbitAroundPlane(View, Model);

	//		mvp = proj * View * Model;

	//	}
	//	
	//}

	//glDeleteProgram(shader);


}
