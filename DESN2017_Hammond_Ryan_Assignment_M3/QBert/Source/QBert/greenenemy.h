#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>


class GreenEnemy{
private:
	float _i, _j;
	bool isActive = false;
public:
	GreenEnemy(float i, float j){
		_i = i;
		_j = j;
	}

	~GreenEnemy(){
	}

	void draw(){
		glTranslatef(_i, _j, -1.0);
		glColor4f(1.0, 0.0, 1.0, 1.0);
		glutSolidSphere(0.25, 100, 100);
		glLoadIdentity();
	}

	void setPosition(float i, float j){

		_i = i;
		_j = j;
		if (_j < -2.8)
		{
			deactivate();
			_i = 100;
			_j = 100;
		}
	}

	void move(){
		
		float rnum = rand() % 2;
		if (rnum == 0)
		{
			setPosition(_i - 0.5, _j - 1);
		}
		else if (rnum == 1)
		{
			setPosition(_i + 0.5, _j - 1);
		}
	}

	int detectCollision(float playerX, float playerY)
	{
		if (playerX == _j && playerY == _i)
		{
			return 100;
		}
	}

	void activate()
	{
		isActive = true;
	}

	void deactivate()
	{
		isActive = false;
	}

	bool checkActive()
	{
		return isActive;
	}
};