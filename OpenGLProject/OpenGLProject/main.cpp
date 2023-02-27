#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Dimensiones de la ventana de Windows
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
	//Inicializar GLFW para el manejo de las ventanas
	if (!glfwInit())
	{
		printf("Fallo al inicializar GLFW");
		return 1;
	}



	return 0;
}