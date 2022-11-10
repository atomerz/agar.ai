#include "agar/GameManager.h"
#include <sstream>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>

using namespace agarai;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
GameManager manager;
////////////////////////////////////////////////////////////////////////////////
void keyboardCallback(unsigned char key, int x, int y);
void specialCallback(int key, int x, int y);
void resizeCallback(int w, int h);
void displayCallback();
void timerCallback(int value);
void idleCallback();
void closeCallback();
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

	Dimension2D worldSize = {800, 800};
	manager.init(&argc, argv, worldSize);

	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutReshapeFunc(resizeCallback);
	glutDisplayFunc(displayCallback);
	glutTimerFunc(0, timerCallback, 0);
	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);

	manager.run();

	/*while(true)
	{
		cout << (int)_getch() << endl;
	}*/

	/*vector<int> v;
	v.push_back(4);
	v.push_back(10);
	v.push_back(5);
	v.push_back(8);
	v.push_back(3);

	set<int, std::greater<int> > s;
	copy_if(v.begin(), v.end(), inserter(s, s.begin()), [](int i) { return i >= 5; });

	for(auto i : s)
		cout << i << endl;*/


	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
void specialCallback(int key, int x, int y)
{
	manager.onSpacialKeyPress(key, x, y);
}
//------------------------------------------------------------------------------
void keyboardCallback(unsigned char key, int x, int y)
{
	manager.onKeyPress(key, x, y);
}
//------------------------------------------------------------------------------
void resizeCallback(int w, int h)
{
	manager.onWindowSizeChanged(w, h);
}
//------------------------------------------------------------------------------
void displayCallback()
{
	manager.onRender();
}
//------------------------------------------------------------------------------
void timerCallback(int value)
{
	manager.onTimer(value);
	glutTimerFunc(250, timerCallback, 1);
}
//------------------------------------------------------------------------------
void idleCallback()
{
	glutPostRedisplay();
}
//------------------------------------------------------------------------------
void closeCallback()
{
	manager.onCleanup();
}