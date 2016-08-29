
#include "Display.h"

#include "glew.h"
#include "freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"


std::shared_ptr<Display> Display::_static_instance = nullptr;


Display::Display() :
	_screenWidth(0.0f),
	_screenHeight(0.0f),
	_window_title("imgui_freeglut")
{
}

Display::~Display()
{
}

// static
std::shared_ptr<Display> Display::GetInstance()
{
	if (_static_instance == nullptr)
	{ 
		_static_instance = std::shared_ptr<Display>(new Display()); 
		_static_instance->_screenWidth = 500;
		_static_instance->_screenHeight = 500;
	}
	return _static_instance;
}

void Display::Draw()
{
	//std::cout << "draw\n";
	//glFlush();

	//float slice = 7;
	//float radius = 100;
	//float pi_2 = 3.141592 * 2.0f;
	
	//glPointSize(5.0);
	glLineWidth(1.0);

	glBegin(GL_LINES);

	// diagonal
	glColor3f(1.0, 0.0, 0.0);
	glVertex2d(0.0, 0.0);
	glVertex2d(500.0, 500.0);

	// top
	glColor3f(0.0, 1.0, 0.0); // green
	glVertex2d(0.0, 0.0);
	glVertex2d(500.0, 0.0);

	// left
	glColor3f(0.0, 1.0, 0.0); // green
	glVertex2d(0.0, 0.0);
	glVertex2d(0.0, 500.0);

	// bottom
	glColor3f(0.0, 0.0, 1.0); // blue
	glVertex2d(0.0, 500.0);
	glVertex2d(500.0, 500.0);

	// right
	glColor3f(0.0, 0.0, 1.0); // blue
	glVertex2d(500.0, 0.0);
	glVertex2d(500.0, 500.0);

	glEnd();

	ImGui_ImplGLUT_NewFrame(this->_screenWidth, this->_screenHeight, 1.0f / 30.0f);
	bool show_another_window = true;
	ImGui::Begin("Decorative Ornaments", &show_another_window, ImVec2(200, 100));
	ImGui::Text("Hello World 1");
	ImGui::Text("Hello World 2");
	ImGui::Text("Hello World 3");
	ImGui::End();
	ImGui::Render();
	

	/*
	bool show_another_window = true;
	ImGui::Begin("Another Window", &show_another_window, ImVec2(200, 100));
	ImGui::Text("Hello");
	ImGui::End();

	bool testOpen = true;
	ImGui::ShowTestWindow(&testOpen);
	
	ImGui::Render();
	*/
}


void Display::Update(int nScreenWidth, int nScreenHeight)
{
	_screenWidth = nScreenWidth;
	_screenHeight = nScreenHeight;

	float xOffset = 0;
	float yOffset = 0;
	float scaleVal = 1.0;

	if (_screenWidth > _screenHeight) // wide
	{
		xOffset = (_screenWidth - _screenHeight)* 0.5f;
		scaleVal = _screenHeight / 500.0f;
	}
	else // tall
	{
		yOffset = (_screenHeight - _screenWidth) * 0.5f;
		scaleVal = _screenWidth / 500.0f;
	}

	glViewport(0, 0, _screenWidth, _screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluOrtho2D(0 - xOffset,     // left
		_screenWidth - xOffset, // right
		_screenHeight - yOffset, //bottom
		0 - yOffset);	// flip the y axis // top


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(scaleVal, scaleVal, scaleVal);
}

bool Display::KeyboardEvent(unsigned char nChar, int nX, int nY)
{
	ImGuiIO& io = ImGui::GetIO();

	io.AddInputCharacter(nChar);

	return true;
}

bool Display::MouseEvent(int button, int state, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		//std::cout << "mouse down\n";
		io.MouseDown[0] = true;
	}
	else
	{
		io.MouseDown[0] = false;
	}

	return true;
}

// static
void Display::ShowGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);
	glutCreateWindow( Display::GetInstance()->_window_title.c_str());

	// callback
	glutDisplayFunc(      ShowCallback);
	glutReshapeFunc(      ResizeCallback);	
	glutKeyboardFunc(     KeyboardCallback);
	glutMouseFunc(        MouseCallback);
	glutMotionFunc(       MouseDragCallback);
	glutPassiveMotionFunc(MouseMoveCallback);
	
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.7, 1.0, 0.7, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMainLoop();
}

// static
void Display::ResizeCallback(int w, int h)
{
	glViewport(0, 0, w, h);
	Display::GetInstance()->Update(w, h);
}

// static
void Display::ShowCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplGLUT_Init(); // shoud it always be here ?

	Display::GetInstance()->Draw();

	glutSwapBuffers();
}

// static
void Display::KeyboardCallback(unsigned char nChar, int x, int y)
{
	if (Display::GetInstance()->KeyboardEvent(nChar, x, y))
	{
		glutPostRedisplay();
	}
}

// static
void Display::MouseCallback(int button, int state, int x, int y)
{
	if (Display::GetInstance()->MouseEvent(button, state, x, y))
	{
		glutPostRedisplay();
	}
}

// static
void Display::MouseDragCallback(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);

	glutPostRedisplay();
}

// static
void Display::MouseMoveCallback(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);

	glutPostRedisplay();
}