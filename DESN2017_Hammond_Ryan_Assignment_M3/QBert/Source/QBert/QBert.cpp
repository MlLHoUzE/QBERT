#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "redenemy.h"
#include "greenenemy.h"
#include "scoreboard.h"
#include "disk.h"
#include "coily.h"
#include "Texture.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio.hpp>
#include <gl/glut.h>
//	BY RYAN HAMMOND	//
using namespace std;
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;
Texture startTexture;
Texture helpTexture;
Texture pauseTexture;
Texture helpTexture2;
int levelOne[9][9] = 
{
	{ 9, 9, 9, 9, 9, 9, 9, 9, 9 },
	{ 9, 2, 2, 2, 2, 2, 2, 2, 9 },
	{ 9, 9, 2, 2, 2, 2, 2, 2, 9 },
	{ 0, 0, 9, 2, 2, 2, 2, 2, 9 },
	{ 0, 0, 0, 6, 2, 2, 2, 2, 5 },
	{ 0, 0, 0, 0, 9, 2, 2, 2, 9 },
	{ 0, 0, 0, 0, 0, 9, 2, 2, 9 },
	{ 0, 0, 0, 0, 0, 0, 9, 2, 9 },
	{ 0, 0, 0, 0, 0, 0, 0, 9, 9 }
};
int levelTwo[9][9] =
{
	{ 9, 9, 9, 9, 9, 9, 9, 9, 9 },
	{ 9, 3, 3, 3, 3, 3, 3, 3, 9 },
	{ 9, 9, 3, 3, 3, 3, 3, 3, 9 },
	{ 0, 0, 9, 3, 3, 3, 3, 3, 9 },
	{ 0, 0, 0, 6, 3, 3, 3, 3, 5 },
	{ 0, 0, 0, 0, 9, 3, 3, 3, 9 },
	{ 0, 0, 0, 0, 0, 9, 3, 3, 9 },
	{ 0, 0, 0, 0, 0, 0, 9, 3, 9 },
	{ 0, 0, 0, 0, 0, 0, 0, 9, 9 }
};
int levelThree[9][9] =
{
	{ 9, 9, 9, 9, 9, 9, 9, 9, 9 },
	{ 9, 3, 3, 3, 3, 3, 3, 3, 9 },
	{ 9, 9, 3, 3, 3, 3, 3, 3, 9 },
	{ 0, 0, 9, 3, 3, 3, 3, 3, 9 },
	{ 0, 0, 0, 6, 3, 3, 3, 3, 5 },
	{ 0, 0, 0, 0, 9, 3, 3, 3, 9 },
	{ 0, 0, 0, 0, 0, 9, 3, 3, 9 },
	{ 0, 0, 0, 0, 0, 0, 9, 3, 9 },
	{ 0, 0, 0, 0, 0, 0, 0, 9, 9 }
};
int playingfield[9][9] =	//3's are out of bounds, 2's are the playing field
{
	{9, 9, 9, 9, 9, 9, 9, 9, 9},
	{9, 2, 2, 2, 2, 2, 2, 2, 9},
	{9, 9, 2, 2, 2, 2, 2, 2, 9},
	{0, 0, 9, 2, 2, 2, 2, 2, 9},
	{0, 0, 0, 6, 2, 2, 2, 2, 5},
	{0, 0, 0, 0, 9, 2, 2, 2, 9},
	{0, 0, 0, 0, 0, 9, 2, 2, 9},
	{0, 0, 0, 0, 0, 0, 9, 2, 9},
	{0, 0, 0, 0, 0, 0, 0, 9, 9}
};
float offset = 0;
float red, green, blue;
int playerX = 7.0;
int playerY = 7.0;
int redX = 7.0;
int redY = 7.0;
int gameState = 0;
int score = 0;
int lives = 3;
int level = 1;
int requirement = 0;
float gameTime = 0;
float lastGreen = 0;
float lastRed = 0;
float lastCoily = 0;
float greenFreq = 16;
float redFreq = 13.3;
float coilyFreq = 7;
float lastMove = 0;
float lastCoilyMove = 0;
float moveFreq = 2;
float coilyMoveFreq = 2;
float playerXColl = 4.1;
float playerYColl = 0.5;
sf::Music mMusic;
sf::Sound jumpSound;
sf::Sound deathSound;
sf::SoundBuffer mJumpBuffer;
sf::SoundBuffer mDeathBuffer;
RedEnemy *red1 = new RedEnemy(7, 7);
RedEnemy *redArray[5];
GreenEnemy *green1 = new GreenEnemy(7, 7);
GreenEnemy *greenArray[5];
Scoreboard s;
string input;
Disk *leftDisk = new Disk(100, 100);
Disk *rightDisk = new Disk(101, 101);
Coily *coily = new Coily(100, 100);
char * c_input;
char * c_score;


void init()
{
	srand((unsigned)time(NULL));
	glClearColor(0, 0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		redArray[i] = new RedEnemy(7, 7);
		greenArray[i] = new GreenEnemy(7, 7);
	}
	mMusic.openFromFile("ChillingMusic.wav");
	mMusic.setVolume(25.f);
	mMusic.setLoop(true);
	mMusic.play();
	mJumpBuffer.loadFromFile("Hop.wav");
	jumpSound.setVolume(25.f);
	jumpSound.setBuffer(mJumpBuffer);

}

void drawText(const char *text, int length, float x, float y)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, 0.9, 10);
	glMatrixMode(GL_MODELVIEW);
	glRasterPos3f(x, y, -1);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
}

void firstTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &startTexture.texID);

	glBindTexture(GL_TEXTURE_2D, startTexture.texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, startTexture.bpp / 8, startTexture.width, startTexture.height, 0, startTexture.type, GL_UNSIGNED_BYTE, startTexture.imageData);
}

void secondTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &helpTexture.texID);

	glBindTexture(GL_TEXTURE_2D, helpTexture.texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, helpTexture.bpp / 8, helpTexture.width, helpTexture.height, 0, helpTexture.type, GL_UNSIGNED_BYTE, helpTexture.imageData);
}

void thirdTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &pauseTexture.texID);

	glBindTexture(GL_TEXTURE_2D, pauseTexture.texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, pauseTexture.bpp / 8, pauseTexture.width, pauseTexture.height, 0, pauseTexture.type, GL_UNSIGNED_BYTE, pauseTexture.imageData);
}

void fourthTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &helpTexture2.texID);

	glBindTexture(GL_TEXTURE_2D, helpTexture2.texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, helpTexture2.bpp / 8, helpTexture2.width, helpTexture2.height, 0, helpTexture2.type, GL_UNSIGNED_BYTE, helpTexture2.imageData);
}

void drawCube(int i, int j, float red, float green, float blue)
{
	glColor4f(red, green, blue, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(j - offset - 3.0, i - 3.5, -1);
	glVertex3f(j - offset - 2.5, i - 3.0, -1);
	glVertex3f(j - offset - 3.0, i - 2.5, -1);
	glVertex3f(j - offset - 3.5, i - 3.0, -1);
	glEnd();

	glColor4f(0.0, 0.0, 0.7, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(j - offset - 3.5, i - 3.0, -1);
	glVertex3f(j - offset - 3.0, i - 3.5, -1);
	glVertex3f(j - offset - 3.0, i - 4.0, -1);
	glVertex3f(j - offset - 3.5, i - 3.5, -1);
	glEnd();

	glColor4f(0.0, 0.0, 0.2, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(j - offset - 2.5, i - 3.0, -1.0);
	glVertex3f(j - offset - 2.5, i - 3.5, -1.0);
	glVertex3f(j - offset - 3.0, i - 4.0, -1.0);
	glVertex3f(j - offset - 3.0, i - 3.5, -1.0);
	glEnd();
}

void drawPlayer(int i, int j)
{
	glTranslatef(j - offset - 3.0, i - 2.9, -1.0);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glutSolidSphere(0.25, 100, 100);	
	glLoadIdentity();
}

void drawMenu()
{
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(-5.0, -5.0, -1.0);
	glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(5.0, -5.0, -1.0);
	glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(5.0, 5.0, -1.0);
	glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(-5.0, 5.0, -1.0);
	glEnd();
}

string getInput()
{
	static char input_char[256];
	cin.getline(&input_char[0], 256, '\n');
	return string(input_char);
}

int str2int(string str)
{
	istringstream ss(str);
	int result = 0;
	ss >> result;
	return result;
}

void resetSpawns()
{
	lastRed = gameTime;
	lastGreen = gameTime;
	lastCoily = gameTime;
	coily->noChase();
}

void idle()
{
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	s.load("Scoreboard");

	if(gameState == 0)
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		firstTexture();
		drawMenu();
	}
	
	if(gameState == 1)
	{
		float gameTime = glutGet(GLUT_ELAPSED_TIME) * 0.001;
		if (level == 1)
		{
			redFreq = 10.3;
			greenFreq = 15;
		}
		if (level >= 2)
		{
			redFreq = 8.3;
			greenFreq = 17;
			coily->turnOn();
		}
		if (level == 3)
		{
			redFreq = 5.3;
			greenFreq = 20;
		}
		
		for (int i = 0; i < 9; i++)
		{
			if (i > 0)
			offset += 0.5;
			for (int j = 8; j >= 0; j--)
			{
				if (playingfield[i][j] == 1)
				{
					red = 1.0;
					green = 0.0;
					blue = 0.0;
					drawCube(i, j, red, green, blue);
					if (i == playerX && j == playerY)
					{
						drawPlayer(i, j);
					}
					if (i == redX && j == redY)
					{
						if (coily->isOn() == true && lastCoily + coilyFreq <= gameTime && coily->checkActive() == false)
						{
							coily->setPosition(j - offset - 3.0, i - 2.9);
							coily->activate();
							lastCoily = gameTime;
							lastCoilyMove = gameTime;
						}
						if (lastRed + redFreq == gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (redArray[k]->checkActive() == false)
								{
									redArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									redArray[k]->activate();
									lastRed = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
						else if (lastGreen + greenFreq == gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (greenArray[k]->checkActive() == false)
								{
									greenArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									greenArray[k]->activate();
									lastGreen = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
						
					}
					
				}
				else if (playingfield[i][j] == 2)
				{
					red = 0.0;
					green = 1.0;
					blue = 0.0;
					drawCube(i, j, red, green, blue);
					if (i == playerX && j == playerY)
					{
						drawPlayer(i, j);
					}
					if (i == redX && j == redY)
					{
						if (coily->isOn() == true && lastCoily + coilyFreq <= gameTime && coily->checkActive() == false)
						{
							coily->setPosition(j - offset - 3.0, i - 2.9);
							coily->activate();
							lastCoily = gameTime;
							lastCoilyMove = gameTime;
						}
						if (lastRed + redFreq <= gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (redArray[k]->checkActive() == false)
								{
									redArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									redArray[k]->activate();
									lastRed = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
						else if (lastGreen + greenFreq <= gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (greenArray[k]->checkActive() == false)
								{
									greenArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									greenArray[k]->activate();
									lastGreen = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
					}
				}
				else if (playingfield[i][j] == 3)
				{
					red = 1.0;
					green = 1.0;
					blue = 1.0;
					drawCube(i, j, red, green, blue);
					if (i == playerX && j == playerY)
					{
						drawPlayer(i, j);
					}
					if (i == redX && j == redY)
					{
						if (coily->isOn() == true && lastCoily + coilyFreq <= gameTime && coily->checkActive() == false)
						{
							coily->setPosition(j - offset - 3.0, i - 2.9);
							coily->activate();
							lastCoily = gameTime;
							lastCoilyMove = gameTime;
						}
						if (lastRed + redFreq <= gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (redArray[k]->checkActive() == false)
								{
									redArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									redArray[k]->activate();
									lastRed = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
						else if (lastGreen + greenFreq <= gameTime)
						{
							for (int k = 0; k < 4; k++)
							{
								if (greenArray[k]->checkActive() == false)
								{
									greenArray[k]->setPosition(j - offset - 3.0, i - 2.9);
									greenArray[k]->activate();
									lastGreen = gameTime;
									lastMove = gameTime;
									break;
								}
							}
						}
					}
				}
				else if (playingfield[i][j] == 5)
				{
					red = 1.0;
					green = 1.0;
					blue = 1.0;
					rightDisk->setPosition(j - offset - 3.0, i - 3.4);
					rightDisk->draw();
					if (i == playerX && j == playerY)
					{
						drawPlayer(i, j);
					}
				}
				else if (playingfield[i][j] == 6)
				{
					red = 1.0;
					green = 1.0;
					blue = 1.0;
					leftDisk->setPosition(j - offset - 3.0, i - 3.4);
					leftDisk->draw();
					if (i == playerX && j == playerY)
					{
						drawPlayer(i, j);
					}
				}
			}
		}
		if (lastMove + moveFreq <= gameTime)
		{
			for (int i = 0; i < 4; i++)
			{
				if (redArray[i]->checkActive() == true)
				{
					redArray[i]->move();
				}
				if (greenArray[i]->checkActive() == true)
				{
					greenArray[i]->move();
				}
			}
			lastMove = gameTime;
		}
		if (lastCoilyMove + coilyMoveFreq <= gameTime)
		{
			if (coily->checkActive() == true)
			{
				coily->move(playerXColl, playerYColl);
			}
			lastCoilyMove = gameTime;
		}
		for (int i = 0; i < 4; i++)
		{
			greenArray[i]->draw();
		}
		for (int i = 0; i < 4; i++)
		{
			redArray[i]->draw();
		}
		coily->draw();
		if (coily->detectCollision(playerXColl, playerYColl) == -1)
		{
			lives -= 1;
			coily->setPosition(100, 100);
			coily->deactivate();
			for (int i = 0; i < 4; i++)
			{
				redArray[i]->setPosition(100, 100);
				redArray[i]->deactivate();
				greenArray[i]->setPosition(100, 100);
				greenArray[i]->deactivate();
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (redArray[i]->detectCollision(playerXColl, playerYColl) == -1)
			{
				lives -= 1;
				for (int j = 0; j < 4; j++)
				{
					redArray[j]->setPosition(100, 100);
					redArray[j]->deactivate();
					greenArray[j]->setPosition(100, 100);
					greenArray[j]->deactivate();
				}
				coily->setPosition(100, 100);
				coily->deactivate();
			}
			else if (greenArray[i]->detectCollision(playerXColl, playerYColl) == 100)
			{
				score += 100;
				greenArray[i]->setPosition(100, 100);
				greenArray[i]->deactivate();
			}
		}
		
		
		offset = 0;
		std::string scoreText("Score: " + std::to_string(score));
		glColor3f(1.0, 1.0, 1.0);
		drawText(scoreText.data(), scoreText.size(), -4, -4);

		std::string livesText("Lives: " + std::to_string(lives));
		drawText(livesText.data(), livesText.size(), -1, -4);

		std::string levelText("Level: " + std::to_string(level));
		drawText(levelText.data(), levelText.size(), 2, -4);

		std::string pauseText("'3' to Pause");
		drawText(pauseText.data(), pauseText.size(), 2, 4);

		std::string blueText("Target = RED");
		glColor4f(1.0, 0.0, 0.0, 1.0);
		drawText(blueText.data(), blueText.size(), -5, 4);

		
			
			
		if (level == 1 && requirement == 28)
		{
			playerX = 7.0;
			playerY = 7.0;
			playerXColl = 4.1;
			playerYColl = 0.5;
			level += 1;
			score += 1000;
			lives += 1;
			requirement = 0;
			resetSpawns();
			if (level % 5 == 0)
			{
				lives += 3;
			}
			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < 9; l++)
				{
					playingfield[k][l] = levelTwo[k][l];

				}
			}

		}if (level == 2 && requirement == 56)
		{
			playerX = 7.0;
			playerY = 7.0;
			playerXColl = 4.1;
			playerYColl = 0.5;
			level += 1;
			lives += 1;
			score += 1000;
			requirement = 0;
			resetSpawns();
			if (level % 5 == 0)
			{
				lives += 3;
			}
			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < 9; l++)
				{
					playingfield[k][l] = levelThree[k][l];

				}
			}

		}if (level == 3 && requirement == 56)
		{
			playerX = 7.0;
			playerY = 7.0;
			playerXColl = 4.1;
			playerYColl = 0.5;
			level += 1;
			lives += 1;
			score += 1000;
			resetSpawns();
			requirement = 0;
			if (level % 5 == 0)
			{
				lives += 3;
			}
			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < 9; l++)
				{
					playingfield[k][l] = levelThree[k][l];

				}
			}

		}
		if (level >= 4 && requirement == 56)
		{
			playerX = 7.0;
			playerY = 7.0;
			playerXColl = 4.1;
			playerYColl = 0.5;
			level += 1;
			lives += 1;
			score += 1000;
			requirement = 0;
			moveFreq += 0.1;
			coilyMoveFreq += 0.15;
			resetSpawns();
			if (level % 5 == 0)
			{
				lives += 3;
			}
			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < 9; l++)
				{
					playingfield[k][l] = levelThree[k][l];

				}
			}

		}
		if (lives <= 0)
		{
			gameState = 4;
			
			for (int k = 0; k < 9; k++)	//set the playing field to level one
			{
				for (int l = 0; l < 9; l++)
				{
					playingfield[k][l] = levelOne[k][l];

				}
			}
		}
	
	}

	if(gameState == 2)	//help menu
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		secondTexture();
		drawMenu();
	}

	if (gameState == 3)	//pause menu
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		thirdTexture();
		drawMenu();
	}

	if (gameState == 6)	//pause help screen
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		fourthTexture();
		drawMenu();
	}

	if (gameState == 4)
	{
		std::string titleText("Game Over");
		glColor4f(1.0, 0.0, 1.0, 1.0);
		drawText(titleText.data(), titleText.size(), -1.5, 4.5);

		std::string promptText("Please type in your name");
		glColor4f(1.0, 0.0, 1.0, 1.0);
		drawText(promptText.data(), promptText.size(), -2.5, 3.5);


		glColor4f(1.0, 0.0, 1.0, 1.0);
		drawText(input.data(), input.size(), -2.5, 0);
		/*while (input != "exit")
		{
			cout << "Command: ";
			input = getInput();

			if (input == "load")
			{
				cout << "Data File: ";
				input = getInput();
				s.load(input.c_str());
			}
			else if (input == "save")
			{
				cout << "Data File: ";
				input = getInput();
				s.save(input.c_str());
			}
			else if (input == "add")
			{
				Scoreboard::Entry entry;
				cout << "Name: ";
				entry.setName(getInput());
				cout << "Score: ";
				entry.score = str2int(getInput());
				int pos = s.addEntry(entry);
				cout << "Entry was added as position #" << pos << "\n";
			}
			else if (input == "scores")
			{
				for (int i = 0; i < s.size(); i++)
				{
					Scoreboard::Entry e = s.getEntry(i);
					cout << e.name << "\t-\t" << e.score << endl;
				}
			}
			else if (input == "prune")
			{
				int max = str2int(getInput());
				s.prune(max);
			}
			else if (input == "size")
			{
				cout << "Size = " << s.size() << endl;
			}
		}*/
	}

	if (gameState == 5)
	{
		string title = "High Scores.";
		drawText(title.data(), title.size(), -1.5, 4.5);
		string Continue = "press 'space' to continue";
		drawText(Continue.data(), Continue.size(), -2.5, 3.5);
		if (s.size() > 10)
		{
			s.prune(10);
		}
		for (int i = 0; i < s.size(); i++)
		{
			Scoreboard::Entry e = s.getEntry(i);
			string name = e.name;
			string str_score = to_string(e.score);
			drawText(name.data(), name.size(), -2.5, 2.5 - i);
			drawText(str_score.data(), str_score.size(), 0, 2.5 -i);
		}

		lives = 3;
		score = 0;
		level = 1;
	}
	
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, 0.9, 10);
}

void keyboard(unsigned char key, int x, int y)
{
	if (gameState == 1)
	{

		if (key == 'a' || key == 'A')
		{
			jumpSound.play();
			if (playingfield[playerX + 1][playerY] != 0)
			{
				if (playingfield[playerX + 1][playerY] == 9)
				{
					playerX = 7.0;
					playerY = 7.0;
					playerXColl = 4.1;
					playerYColl = 0.5;
					lives -= 1;
					for (int i = 0; i < 4; i++)
					{
						redArray[i]->setPosition(100, 100);
						redArray[i]->deactivate();
						greenArray[i]->setPosition(100, 100);
						greenArray[i]->deactivate();
					}
					coily->noChase();
					coily->setPosition(100, 100);
					coily->deactivate();
					lastCoily = gameTime;
					lastGreen = gameTime;
					lastRed = gameTime;
				}
				else if (playingfield[playerX + 1][playerY] == 2)
				{
					playerX += 1;
					playerXColl += 1.0;
					playerYColl -= 0.5;
					playingfield[playerX][playerY] = 1;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX + 1][playerY] == 3)
				{
					playerX += 1;
					playerXColl += 1.0;
					playerYColl -= 0.5;
					playingfield[playerX][playerY] = 2;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX + 1][playerY] == 6)
				{
					playerX += 1;
					playerXColl += 1.0;
					playerYColl -= 0.5;
					playingfield[playerX][playerY] = 9;
					playingfield[7][6] = 6;
					playerX = 7;
					playerY = 6;
					playerXColl = 4.1;
					playerYColl = -0.5;
				}
				else
				{
					playerX += 1;
					playerXColl += 1.0;
					playerYColl -= 0.5;
				}
			}
		}

		if (key == 'd' || key == 'D')
		{
			jumpSound.play();
			if (playingfield[playerX - 1][playerY] != 0)
			{
				if (playingfield[playerX - 1][playerY] == 9)
				{
					playerX = 7.0;
					playerY = 7.0;
					playerXColl = 4.1;
					playerYColl = 0.5;
					lives -= 1;
					for (int i = 0; i < 4; i++)
					{
						redArray[i]->setPosition(100, 100);
						redArray[i]->deactivate();
						greenArray[i]->setPosition(100, 100);
						greenArray[i]->deactivate();
					}
					coily->noChase();
					coily->setPosition(100, 100);
					coily->deactivate();
					lastCoily = gameTime;
					lastGreen = gameTime;
					lastRed = gameTime;
				}
				else if (playingfield[playerX - 1][playerY] == 2)
				{
					playerX -= 1;
					playerXColl -= 1;
					playerYColl += 0.5;
					playingfield[playerX][playerY] = 1;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX - 1][playerY] == 3)
				{
					playerX -= 1;
					playerXColl -= 1.0;
					playerYColl += 0.5;
					playingfield[playerX][playerY] = 2;
					score += 25;
					requirement += 1;
				}
				else
				{
					playerX -= 1;
					playerXColl -= 1;
					playerYColl += 0.5;
				}
			}
		}

		if (key == 'w' || key == 'W')
		{
			jumpSound.play();
			if (playingfield[playerX + 1][playerY + 1] != 0)
			{
				if (playingfield[playerX + 1][playerY + 1] == 9)
				{
					playerX = 7.0;
					playerY = 7.0;
					playerXColl = 4.1;
					playerYColl = 0.5;
					lives -= 1;
					for (int i = 0; i < 4; i++)
					{
						redArray[i]->setPosition(100, 100);
						redArray[i]->deactivate();
						greenArray[i]->setPosition(100, 100);
						greenArray[i]->deactivate();
					}
					coily->noChase();
					coily->setPosition(100, 100);
					coily->deactivate();
					lastCoily = gameTime;
					lastGreen = gameTime;
					lastRed = gameTime;
				}
				else if (playingfield[playerX + 1][playerY + 1] == 2)
				{
					playerX += 1;
					playerY += 1;
					playerXColl += 1;
					playerYColl += 0.5;
					playingfield[playerX][playerY] = 1;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX + 1][playerY + 1] == 3)
				{
					playerX += 1;
					playerY += 1;
					playerXColl += 1.0;
					playerYColl += 0.5;
					playingfield[playerX][playerY] = 2;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX + 1][playerY + 1] == 5)
				{
					playerX += 1;
					playerY += 1;
					playerXColl += 1.0;
					playerYColl += 0.5;
					playingfield[playerX][playerY] = 9;
					playingfield[7][8] = 5;
					playerX = 7;
					playerY = 8;
					playerXColl = 4.1;
					playerYColl = 1.5;
				}
				else
				{
					playerX += 1;
					playerY += 1;
					playerXColl += 1;
					playerYColl += 0.5;
				}
			}
		}

		if (key == 's' || key == 'S')
		{
			jumpSound.play();
			if (playingfield[playerX - 1][playerY - 1] != 0)
			{
				if (playingfield[playerX - 1][playerY - 1] == 9)
				{
					playerX = 7.0;
					playerY = 7.0;
					playerXColl = 4.1;
					playerYColl = 0.5;
					lives -= 1;
					for (int i = 0; i < 4; i++)
					{
						redArray[i]->setPosition(100, 100);
						redArray[i]->deactivate();
						greenArray[i]->setPosition(100, 100);
						greenArray[i]->deactivate();
					}
					coily->noChase();
					coily->setPosition(100, 100);
					coily->deactivate();
					lastCoily = gameTime;
					lastGreen = gameTime;
					lastRed = gameTime;
					
				}
				else if (playingfield[playerX - 1][playerY - 1] == 2)
				{
					playerX -= 1;
					playerY -= 1;
					playerXColl -= 1;
					playerYColl -= 0.5;
					playingfield[playerX][playerY] = 1;
					score += 25;
					requirement += 1;
				}
				else if (playingfield[playerX - 1][playerY - 1] == 3)
				{
					playerX -= 1;
					playerY -= 1;
					playerXColl -= 1.0;
					playerYColl -= 0.5;
					playingfield[playerX][playerY] = 2;
					score += 25;
					requirement += 1;
				}
				else
				{
					playerX -= 1;
					playerY -= 1;
					playerXColl -= 1;
					playerYColl -= 0.5;
				}
			}
		}
	}

	if (gameState == 0)	//in main menu
	{
		if (key == '1')
		{
			gameState = 1;	//change to game
		}
		if (key == '2')
		{
			gameState = 2;	//change to help screen
		}
		if (key == 27)
		{
			exit(0);
		}
	}
	if (gameState == 1)	//game screen
	{
		if (key == '3')
		{
			glColor4f(1, 1, 1, 1);
			gameState = 3;	//pause menu
		}
	}
	if (gameState == 2)
	{
		if (key == '0')
		{
			gameState = 0;
		}
		if (key == '1')
		{
			gameState = 1;
		}
		if (key == 27)
		{
			exit(0);
		}
	}
	if (gameState == 3)	//pause menu
	{
		if (key == '1')
		{
			gameState = 1;	//return to game screen
		}
		if (key == '2')
		{
			gameState = 6;	//help screen that doesnt link back to main menu
		}
		if (key == 27)
		{
			exit(0);
		}
	}
	if (gameState == 6)//paused help menu
	{
		if (key == '1')
		{
			gameState = 3;
		}
	}

	if (gameState == 4)
	{
		if (key == 13 || input.size() == 13)
		{
			Scoreboard::Entry entry;
			entry.setName(input);
			entry.score = score;
			s.addEntry(entry);
			s.save("Scoreboard");
			gameState = 5;
		}
		input += key;
		
	}
	if (gameState == 5)
	{
		if (key == ' ')
		{
			gameState = 0;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	LoadTGA(&pauseTexture, "Paused.tga");
	LoadTGA(&startTexture, "title.tga");
	LoadTGA(&helpTexture, "HowTo.tga");
	LoadTGA(&helpTexture2, "HowTo2.tga");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Q*Bert");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

