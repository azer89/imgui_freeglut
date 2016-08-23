

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

	void Init();
	void Draw();
	void Update(int nScreenWidth = 0, int nScreenHeight = 0);
	bool KeyboardEvent(unsigned char nChar, int nX, int nY);
	bool MouseEvent(int button, int state, int x, int y);

public:
	float _screenWidth;
	float _screenHeight;

	std::string _window_title;

};

#endif
