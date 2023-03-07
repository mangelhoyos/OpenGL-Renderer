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

#pragma region Parametros ventana GLFW
	//Establecer la versión de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Habilitar solo código que no este obsoleto y compatible con versiones actualizadas
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *pMainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ventana OpenGL", NULL, NULL);

	if (!pMainWindow)
	{
		printf("Fallo al crear la ventana de GLFW");
		glfwTerminate();
		return 1;
	}

	//Buffer de la ventana

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(pMainWindow, &bufferWidth, &bufferHeight);

	//Definir el contexto de GLFW como el que debe usar GLEW
	glfwMakeContextCurrent(pMainWindow);

	//Permitir extensiones experimentales
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo al inicializar GLEW");
		//Si falla al inicializar GLEW debemos cerrar la ventana de GLFW ya que uno es independiente del otro
		glfwDestroyWindow(pMainWindow);
		glfwTerminate();
		return 1;
	}

	//OpenGL tamaño viewport, el lugar donde dibujaremos
	glViewport(0, 0, bufferWidth, bufferHeight);
#pragma endregion

	//Bucle hasta que la ventana se cierre
	while (!glfwWindowShouldClose(pMainWindow))
	{
		//Aquí podemos manejar el input del usuario
		glfwPollEvents();

		//Limpiar la ventana
		glClearColor(1.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(pMainWindow);
	}


	return 0;
}