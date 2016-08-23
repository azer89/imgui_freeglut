
#include "Display.h"

#include "glew.h"
#include "freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"


Display::Display() :
	_screenWidth(0.0f),
	_screenHeight(0.0f),
	_window_title("imgui_freeglut")
{
}

Display::~Display()
{
}

void Display::Init()
{
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.7, 1.0, 0.7, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::Draw()
{
	//glFlush();

	ImGui_ImplGLUT_NewFrame(this->_screenWidth, this->_screenHeight, 1.0f / 30.0f);

	//ImGui::NewFrame();
	bool show_another_window = true;
	ImGui::Begin("Another Window", &show_another_window, ImVec2(200, 100));
	ImGui::Text("Hello");
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
	_screenWidth = (nScreenWidth != 0) ? (float)nScreenWidth : _screenWidth;
	_screenHeight = (nScreenHeight != 0) ? (float)nScreenHeight : _screenHeight;
	//CORE::Rectf domainRect = _vfdProxy->GetDomainRectangle();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (_screenWidth > _screenHeight)
	{
		float fWidth(_screenWidth / _screenHeight);
		float fOffset((fWidth - 1.0f)*0.5f);
		gluOrtho2D(0 - fOffset, fWidth - fOffset, 1.0f, 0.0f);

		//_vfdProxy->SetDomainRectangle(0 - fOffset, 0, fWidth - fOffset, 1.0f);
	}
	else
	{
		float fHeight(_screenHeight / _screenWidth);
		float fOffset((fHeight - 1.0f)*0.5f);
		gluOrtho2D(0, 1.0f, fHeight - fOffset, 0 - fOffset);

		//_vfdProxy->SetDomainRectangle(0, 0 - fOffset, 1.0f, fHeight - fOffset);
	}
	glMatrixMode(GL_MODELVIEW);

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