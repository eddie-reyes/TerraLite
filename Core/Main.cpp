#include "Application.h"

int main(void)
{
	ApplicationSpecification spec;

	spec.WindowWidth = 1920;
	spec.WindowHeight = 1080;
	spec.Name = "TerraLite";

	Application app(spec);
	app.Run();


}
