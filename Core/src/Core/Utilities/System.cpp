#include "System.h"

#include <GLFW/glfw3.h>

double Core::System::GetTime() {

	return glfwGetTime();

}
