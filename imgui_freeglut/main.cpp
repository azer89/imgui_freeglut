// imgui_freeglut.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Display.h"

#include <iostream>

#include "glew.h"
#include "freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"

Display* _display;

// functions
void Show();
void Resize(int nW, int nH);
void InitGL();

void KeyboardEvent(unsigned char nChar, int nX, int nY);
void MouseEvent(int button, int state, int x, int y);
void MouseDrag(int x, int y);
void MouseMove(int x, int y);

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char **argv)
{
	_display = new Display;
	_display->_screenWidth = 500;
	_display->_screenHeight = 500;

	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(_display->_window_title.c_str());

	// callback
	glutDisplayFunc(Show);
	glutKeyboardFunc(KeyboardEvent);
	glutMouseFunc(MouseEvent);
	glutMotionFunc(MouseDrag);
	glutPassiveMotionFunc(MouseMove);
	glutReshapeFunc(Resize);
	
	InitGL();

	// enter the main loop
	glutMainLoop();

	delete _display;

	std::getchar();

	return 0;
}

void Show()
{
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplGLUT_Init();

	//CORE_ASSERT(glGetError() == GL_NO_ERROR);
	_display->Draw();
	//CORE_ASSERT(glGetError() == GL_NO_ERROR);
	glutSwapBuffers();
}

void Resize(int nW, int nH)
{
	glViewport(0, 0, nW, nH);
	_display->Update(nW, nH);
}

void InitGL()
{
	_display->Init();
}

void KeyboardEvent(unsigned char nChar, int nX, int nY)
{
	if (_display->KeyboardEvent(nChar, nX, nY))
	{
		glutPostRedisplay();
	}
}

void MouseEvent(int button, int state, int x, int y)
{
	if (_display->MouseEvent(button, state, x, y))
	{
		glutPostRedisplay();
	}
}

void MouseDrag(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);

	glutPostRedisplay();
}

void MouseMove(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);

	glutPostRedisplay();
}



