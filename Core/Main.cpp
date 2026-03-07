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

static inline void zoomPlane(glm::mat4& in_matrix, double yoffset) {
	
}

static inline void orbitAroundPlane(glm::mat4& View, glm::mat4& Model) {

	if (abs(mouseDeltaX) <= 0.1f && abs(mouseDeltaY) <= 0.1f) return; 

	View = glm::rotate(View, glm::radians((float)mouseDeltaY), glm::vec3(1, 0, 0));

	glm::vec3 rotationMatrix = glm::normalize(glm::vec3(Model * glm::vec4(0, 0, 1, 0)));
	glm::vec3 normal = glm::cross(rotationMatrix, glm::vec3(1, 0, 0)); 
	Model = glm::rotate(Model, glm::radians((float)-mouseDeltaX), normal);

}

int main(void)
{

	ApplicationSpecification spec;

	spec.WindowWidth = 1920;
	spec.WindowHeight = 1080;
	spec.Name = "Having Fun";
	spec.HeightMapResolution = 512;

	Application app(spec);
	app.Run();


}
