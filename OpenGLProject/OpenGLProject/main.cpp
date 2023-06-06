
//Made by Miguel Ángel Hoyos Ramírez

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //Para la matriz de proyección
#include <glm/gtc/type_ptr.hpp>

// Dimensiones de la ventana de Windows
const GLint WIDTH = 800, HEIGHT = 600;

//Constantes
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

//Variables para el control de la animación (TEMP)
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.008f;

float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

#pragma region Shader Temporal del Triangulo
//Shader temporal para la prueba del triangulo

//Vertex shader
static const char* vShader = "								\n\
#version 330												\n\
															\n\
layout (location = 0) in vec3 pos;							\n\
															\n\
uniform mat4 model;															\n\
															\n\
void main()													\n\
{															\n\
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);			\n\
															\n\
}";

//Fragment shader
static const char* fShader = "								\n\
#version 330												\n\
															\n\
out vec4 colour;											\n\
															\n\
void main()													\n\
{															\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);						\n\
															\n\
}";

#pragma endregion

void CreateTriangle()
{
	//Posicion de los 3 vertices del triangulo 
	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	//Creación del VAO con el buffer de VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC_DRAW ya que no moveremos los puntos

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Desvinculando el VAO y el VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	//Verificar errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("(SHADER) Error compilando el shader %d : \n\
			Log :: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("(SHADER) Error creando el programa del shader \n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	//Manejo de errores de los shaders
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("(SHADER) Error vinculando el programa a la grafica: \n\
			Log :: '%s'\n", eLog);
		return;
	}

	//Validar el programa en el contexto actual
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("(SHADER) Error validando el programa: \n\
			Log :: '%s'\n", eLog);
		return;
	}

	//Move the triangle
	uniformModel = glGetUniformLocation(shader, "model");
}

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
#pragma endregion

#pragma region Dibujar en el viewport
	//OpenGL tamaño viewport, el lugar donde dibujaremos
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//Bucle hasta que la ventana se cierre
	while (!glfwWindowShouldClose(pMainWindow))
	{
		//Aquí podemos manejar el input del usuario
		glfwPollEvents();

		//Animación triangulo (cambio de dirección si llega al offset y rotación continua)
		#pragma region Animación

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxoffset)
		{
			direction = !direction;
		}

		currentAngle += 0.5f;
		if (currentAngle >= 360)
		{
			currentAngle -= 360;
		}

		if (sizeDirection)
		{
			currentSize += 0.001f;
		}
		else
		{
			currentSize -= 0.001f;
		}

		if (currentSize >= maxSize || currentSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		#pragma endregion

		//Limpiar la ventana
		glClearColor(.0f, .0f, .0f, .0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		
		model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));		
		model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//Desasignar
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(pMainWindow);
	}
#pragma endregion

	return 0;
}