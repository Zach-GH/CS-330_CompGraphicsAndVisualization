#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <algorithm>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };

int score = 0;
int state = 0;
int z = 0;

class Brick
{
public:

	float red, green, blue;
	float x, y, width, reflectiveWidth, reflectiveHeight;
	BRICKTYPE brick_type;
	ONOFF onoff;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb, float rw, float rh)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb; reflectiveWidth = rw, reflectiveHeight = rh;
		onoff = ON;
	};

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
	
	void drawReflectiveBrick()
	{
		if (onoff == ON)
		{
			double halfside = reflectiveWidth / 2;
			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + 0.25, y + 0.05);
			glVertex2d(x + 0.25, y - 0.05);
			glVertex2d(x - 0.25, y - 0.05);
			glVertex2d(x - 0.25, y + 0.05);

			glEnd();
		}
	}

	void moveBrickLeft()
	{
		x = x - 0.03;
	}

	void moveBrickRight()
	{
		x = x + 0.03;
	}
};


class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.03;
	int direction; // 1 = up 2 = right 3 = down 4 = left 5 = up right 6 = up left  7 = down right  8 = down left
	ONOFF onoff;
	
	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
		onoff = ON;
	}

	void drawBrick(Brick* brk)
	{
		if (brk->onoff == ON)
		{
			double halfside = brk->width / 2;

			glColor3d(brk->red, brk->green, brk->blue);
			glBegin(GL_POLYGON);

			glVertex2d(brk->x + halfside, brk->y + halfside);
			glVertex2d(brk->x + halfside, brk->y - halfside);
			glVertex2d(brk->x - halfside, brk->y - halfside);
			glVertex2d(brk->x - halfside, brk->y + halfside);

			glEnd();
		}
	}
	
	void drawReflectiveBrick(Brick* brk)
	{
		if (brk->onoff == ON)
		{
			double halfside = brk->width / 2;

			glColor3d(brk->red, brk->green, brk->blue);
			glBegin(GL_POLYGON);

			glVertex2d(brk->x + halfside, brk->y + halfside);
			glVertex2d(brk->x + halfside, brk->y - halfside);
			glVertex2d(brk->x - halfside, brk->y - halfside);
			glVertex2d(brk->x - halfside, brk->y + halfside);

			glEnd();
		}
	}
	
	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				red = ((rand() / 10000) + 1);
				green = rand() / 10000;
				blue = rand() / 10000;
				x = x + 0.03;
				y = y + 0.04;
			}
		}
		
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x > brk->x - brk->width  && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				if (brk->width < 0.2)
				{
					direction = 1;
					red = ((rand() / 10000) + 1);
					green = rand() / 10000;
					blue = rand() / 10000;
					brk->onoff = OFF;
					brk->width = 0;
					score = score + 2;
				}
				else
				{
					direction = 1;
					red = ((rand() / 10000) + 1);
					green = rand() / 10000;
					blue = rand() / 10000;
					brk->width = brk->width - 0.1;
					score = score + 1;
				}
			}
		}
	}

	
	void moveBrickLeft(Brick* brk)
	{
		brk->x = brk->x - 0.03;
	}
	
	void moveBrickRight(Brick* brk)
	{
		brk->x = brk->x + 0.03;
	}
	
	int GetRandomDirection()
	{
		int n = (rand() % 8 + 1);
		if (n == 1) n++;
		return n;
	}


	void MoveOneStep()
	{
		if (direction == 5 || direction == 6)  // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
		
		if (direction == 1)  // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = 1;
			}
		}

		if (direction == 2)  // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = 2;
			}
		}

		if (direction == 5 || direction == 7)  // right
		{
			if (x < 1 - radius)
			{
				x += speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 3 || direction == 7 || direction == 8)  // down
		{
			if (y < 1 - radius) {
				y += speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 4 || direction == 6 || direction == 8)  // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
		
	}

	void DrawCircle()
	{
		if (onoff == ON)
		{
			glColor3f(red, green, blue);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++) {
				float degInRad = i * DEG2RAD;
				glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		}
		glEnd();
	}
};


vector<Circle> world;

double r = ((rand() / 10000) + 1);
double g = ((rand() / 10000) + 1);
double b = ((rand() / 10000) + 1);
Circle B(0, 0, 02, 3, 0.05, r, g, b);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (score > 5)
		{
			B.direction = (rand() % 8) + 1;
			world.push_back(B);
			B.direction = (rand() % 8) + 1;
			world.push_back(B);
			B.direction = (rand() % 8) + 1;
			world.push_back(B);
			B.direction = (rand() % 8) + 1;
			world.push_back(B);
			B.direction = (rand() % 8) + 1;
			world.push_back(B);
		}
		else
		{
			world.push_back(B);
		}
	}
}

void CheckWorld()
{
	for (int i = 0; i < world.size(); i++)
	{
		if (world.size() >= 20)
		{
			world.erase(world.begin(), world.begin() + 5);
		}
	}
}


int main(void) {
	srand(time(NULL));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Random World of Circles", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// top row
	Brick brickA1(DESTRUCTABLE, -0.9, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA2(DESTRUCTABLE, -0.7, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA3(DESTRUCTABLE, -0.5, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA4(DESTRUCTABLE, -0.3, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA5(DESTRUCTABLE, -0.1, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA6(DESTRUCTABLE, 0.1, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA7(DESTRUCTABLE, 0.3, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA8(DESTRUCTABLE, 0.5, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA9(DESTRUCTABLE, 0.7, 0.9, 0.2, 0, 1, 0, 0, 0);
	Brick brickA10(DESTRUCTABLE, 0.9, 0.9, 0.2, 0, 1, 0, 0, 0);
	
	// second row
	Brick brickB1(DESTRUCTABLE, -0.9, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB2(DESTRUCTABLE, -0.7, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB3(DESTRUCTABLE, -0.5, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB4(DESTRUCTABLE, -0.3, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB5(DESTRUCTABLE, -0.1, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB6(DESTRUCTABLE, 0.1, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB7(DESTRUCTABLE, 0.3, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB8(DESTRUCTABLE, 0.5, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB9(DESTRUCTABLE, 0.7, 0.7, 0.2, 1, 1, 0, 0, 0);
	Brick brickB10(DESTRUCTABLE, 0.9, 0.7, 0.2, 1, 1, 0, 0, 0);
	
	// third row
	Brick brickC1(DESTRUCTABLE, -0.9, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC2(DESTRUCTABLE, -0.7, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC3(DESTRUCTABLE, -0.5, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC4(DESTRUCTABLE, -0.3, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC5(DESTRUCTABLE, -0.1, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC6(DESTRUCTABLE, 0.1, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC7(DESTRUCTABLE, 0.3, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC8(DESTRUCTABLE, 0.5, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC9(DESTRUCTABLE, 0.7, 0.5, 0.2, 0, 1, 1, 0, 0);
	Brick brickC10(DESTRUCTABLE, 0.9, 0.5, 0.2, 0, 1, 1, 0, 0);
	
	
	// platform
	Brick brick(REFLECTIVE, 0.0, -0.40, 0.2, 1, 1, 1, 0.2, 0);

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);
		CheckWorld();
		glfwSetKeyCallback(window, key_callback);
		
		//Movement
		for (int i = 0; i < world.size(); i++)
		{
			world[i].CheckCollision(&brick);
			world[i].CheckCollision(&brickA2);

			world[i].CheckCollision(&brickA1);
			world[i].CheckCollision(&brickA2);
			world[i].CheckCollision(&brickA3);
			world[i].CheckCollision(&brickA4);
			world[i].CheckCollision(&brickA5);
			world[i].CheckCollision(&brickA6);
			world[i].CheckCollision(&brickA7);
			world[i].CheckCollision(&brickA8);
			world[i].CheckCollision(&brickA9);
			world[i].CheckCollision(&brickA10);
			
			world[i].CheckCollision(&brickB1);
			world[i].CheckCollision(&brickB2);
			world[i].CheckCollision(&brickB3);
			world[i].CheckCollision(&brickB4);
			world[i].CheckCollision(&brickB5);
			world[i].CheckCollision(&brickB6);
			world[i].CheckCollision(&brickB7);
			world[i].CheckCollision(&brickB8);
			world[i].CheckCollision(&brickB9);
			world[i].CheckCollision(&brickB10);
			
			world[i].CheckCollision(&brickC1);
			world[i].CheckCollision(&brickC2);
			world[i].CheckCollision(&brickC3);
			world[i].CheckCollision(&brickC4);
			world[i].CheckCollision(&brickC5);
			world[i].CheckCollision(&brickC6);
			world[i].CheckCollision(&brickC7);
			world[i].CheckCollision(&brickC8);
			world[i].CheckCollision(&brickC9);
			world[i].CheckCollision(&brickC10);
						
			world[i].MoveOneStep();
			
			world[i].DrawCircle();

			for (int j = 0; j < world.size(); j++)
			{
				
				Circle* circleA = &world[i];
				Circle* circleB = &world[j];

				if (circleA != circleB)
				{
					if (((world[i].x > world[j].x - world[j].radius && world[i].x <= world[j].x + world[j].radius)
						&& (world[i].y > world[j].y - world[j].radius && world[i].y <= world[j].y + world[j].radius)))
					{
						world[i].direction = world[i].GetRandomDirection();
						world[i].red = ((rand() / 10000) + 1);
						world[i].green = rand() / 10000;
						world[1].blue = rand() / 10000;
						world[j].direction = world[j].GetRandomDirection();
						world[j].red = ((rand() / 10000) + 1);
						world[j].green = rand() / 10000;
						world[j].blue = rand() / 10000;

					}
				}

			}
		}

		// top row
		brickA1.drawBrick();
		brickA2.drawBrick();
		brickA3.drawBrick();
		brickA4.drawBrick();
		brickA5.drawBrick();
		brickA6.drawBrick();
		brickA7.drawBrick();
		brickA8.drawBrick();
		brickA9.drawBrick();
		brickA10.drawBrick();
		
		// second row
		brickB1.drawBrick();
		brickB2.drawBrick();
		brickB3.drawBrick();
		brickB4.drawBrick();
		brickB5.drawBrick();
		brickB6.drawBrick();
		brickB7.drawBrick();
		brickB8.drawBrick();
		brickB9.drawBrick();
		brickB10.drawBrick();

		// third row
		brickC1.drawBrick();
		brickC2.drawBrick();
		brickC3.drawBrick();
		brickC4.drawBrick();
		brickC5.drawBrick();
		brickC6.drawBrick();
		brickC7.drawBrick();
		brickC8.drawBrick();
		brickC9.drawBrick();
		brickC10.drawBrick();
		
		brick.drawReflectiveBrick();
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			brick.moveBrickLeft();

		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{

			brick.moveBrickRight();

		}

		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		{
			world.clear();
			score = 0;
		}

		if (state == 0)
		{
			brick.green++;
			if (brick.green == 255)
			{
				state = 1;
			}
		}

		if (state == 1)
		{
			brick.red--;
			if (brick.red == 0)
			{
				state = 2;
			}
		}

		if (state == 2)
		{
			brick.blue++;
			if (brick.blue == 255)
			{
				state = 3;
			}
		}

		if (state == 3)
		{
			brick.green--;
			if (brick.green == 0)
			{
				state = 4;
			}
		}

		if (state == 4)
		{
			brick.red++;
			if (brick.red == 255)
			{
				state = 5;
			}
		}

		if (state == 5)
		{
			brick.blue--;
			if (brick.blue == 0)
			{
				state = 0;
			}
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	std::cout << score << endl;
	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
}