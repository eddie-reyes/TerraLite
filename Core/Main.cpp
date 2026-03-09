#include "Application.h"

int main(void)
{

	ApplicationSpecification spec;

	spec.WindowWidth = 1920;
	spec.WindowHeight = 1080;
	spec.Name = "Having Fun";
	spec.HeightMapResolution = 128;

	Application app(spec);
	app.Run();


}
