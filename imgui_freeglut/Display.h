

/*
================================================================================
Reza Adhitya Saputra
radhitya@uwaterloo.ca
================================================================================
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>

class Display
{
public:

	Display();
	~Display();

	void Draw();
	void Update(       int nScreenWidth = 0,  int nScreenHeight = 0);
	bool KeyboardEvent(unsigned char nChar,   int x, int y);
	bool MouseEvent(   int button, int state, int x,  int y);

public:
	static std::shared_ptr<Display> GetInstance();

	static void InitGL(int argc, char **argv);
	static void ResizeCallback(int w, int h);	
	static void ShowCallback();
	

	static void KeyboardCallback( unsigned char nChar, int x, int y);
	static void MouseCallback(    int button, int state, int x, int y);
	static void MouseDragCallback(int x, int y);
	static void MouseMoveCallback(int x, int y);

public:
	float _screenWidth;
	float _screenHeight;

	std::string _window_title;

	static std::shared_ptr<Display> _static_instance;
};

#endif
