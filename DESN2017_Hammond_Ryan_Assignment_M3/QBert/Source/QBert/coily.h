#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>


class Coily{
private:
	float _i, _j;
	bool isActive = false;
	bool chasing = false;
	bool on = false;
public:
	Coily(float i, float j){
		_i = i;
		_j = j;
	}

	~Coily(){
	}

	void draw(){
		if (chasing == false){
			glTranslatef(_i, _j, -1.0);
			glColor4f(0.53, 0.12, 0.47, 1.0);
			glutSolidSphere(0.25, 100, 100);
			glLoadIdentity();
		}
		else if (chasing == true)
		{
			glTranslatef(_i, _j, -1.0);
			glColor4f(0,0, 0, 1.0);
			glutSolidTeapot(0.25);
			glLoadIdentity();
		}
	}

	void setPosition(float i, float j)
	{

		_i = i;
		_j = j;
		std::cout << _j << std::endl;
		if (_j <= -1.5)
		{
			chasing = true;
		}
	}

	void move(float playerX, float playerY){

		if (chasing == true)
		{
			if (playerX == _j)
			{
				float rnum = rand() % 2;
				if (playerY > _i)
				{
					if (rnum == 0)
					{
						setPosition(_i + 0.5, _j + 1);
					}
					else if (rnum == 1)
					{
						setPosition(_i + 0.5, _j - 1);
					}
				}
				else if (playerY < _i)
				{
					if (rnum == 0)
					{
						setPosition(_i - 0.5, _j + 1);
					}
					else if (rnum == 1)
					{
						setPosition(_i - 0.5, _j - 1);
					}
				}
			}
			else if (playerY == _i)
			{
				float rnum = rand() % 2;
				if (playerX > _j)
				{
					if (rnum == 0)
					{
						setPosition(_i + 0.5, _j + 1);
					}
					else if (rnum == 1)
					{
						setPosition(_i - 0.5, _j + 1);
					}
				}
				else if (playerX < _j)
				{
					if (rnum == 0)
					{
						setPosition(_i + 0.5, _j - 1);
					}
					else if (rnum == 1)
					{
						setPosition(_i - 0.5, _j - 1);
					}
				}
			}
			else if (playerX > _j && playerY < _i)
			{
				setPosition(_i - 0.5, _j + 1);
			}
			else if (playerX > _j && playerY > _i)
			{
				setPosition(_i + 0.5, _j + 1);
			}
			else if (playerX < _j && playerY > _i)
			{
				setPosition(_i + 0.5, _j - 1);
			}
			else if (playerX < _j && playerY < _i)
			{
				setPosition(_i - 0.5, _j - 1);
			}
		}
		if (chasing == false)
		{
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

		
	}

	int detectCollision(float playerX, float playerY)
	{
		if (playerX == _j && playerY == _i)
		{
			return -1;
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
	void turnOn()
	{
		on = true;
	}

	bool isOn()
	{
		return on;
	}

	void noChase()
	{
		chasing = false;
	}
};