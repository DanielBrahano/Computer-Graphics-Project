Assignment1Report Part 1:
=================

The algorithm implementation
-----------------------------------
* **DrawLine function**

```
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x1 = p1.x; int x2 = p2.x; int y1 = p1.y; int y2 = p2.y; // i prefer using x1,x2,... in order to swap when needed
	bool mirrorFlag = false, swapFlag = false;// flags to decide if we need to mirror/swap between x and y
	int x = p1.x;
	int y = p1.y;
	int dx = (p2.x - p1.x);
	int dy = (p2.y - p1.y);
	int e = -(p2.x - p1.x);


	float a = (float)dy / dx;//cast the result to float
	int c = p1.y - a * p1.x;

	// if a<0 then treat it as positive and raise the flag
	if (a < 0) {
		mirrorFlag = true;
		a = a * (-1);
	}

	if (a < 1)// in cases when -1<a<1
	{
		if (mirrorFlag) {
			dy = dy * (-1);
		}
		if (x2 < x1) { // if x2>x1 then switch points
			int t = x1;
			x1 = x2;
			x2 = t;

			t = y1;
			y1 = y2;
			y2 = t;

			//update rest of values
			x = x1;
			y = y1;
			e = dx;
			dy = -dy;
			dx = -dx;

		}
		//go from x1 to x2 and decide where y values are going to be according to the algorithm
		while (x <= x2)
		{

			if (e > 0) {
				mirrorFlag ? y-- : y++;
				e = e - 2 * dx;
			}
			PutPixel(x, y, color);
			x++;
			e = e + 2 * dy;
		}
	}

	//else, cases when a>1 or a<1 so we need to run on y's
	else
	{
		e = -dy;
		if (mirrorFlag) {
			dx = dx * (-1);
		}
		//if y2<y1 then we need to swap
		if (y2 < y1) {
			int t = x1;
			x1 = x2;
			x2 = t;

			t = y1;
			y1 = y2;
			y2 = t;

			//update values
			x = x1;
			y = y1;
			e = dy;
			dy = -dy;
			dx = -dx;
		}

		//go from y1 to y2 and decide where x values are going to be according to the algorithm
		while (y <= y2)
		{
			if (e > 0) {
				mirrorFlag ? x-- : x++;
				e = e - 2 * dy;
			}
			PutPixel(x, y, color);
			y++;
			e = e + 2 * dx;
		}
	}
}
```
* **Render function**

```
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// draw circle


	glm::vec3 black{ 0,0,0 };
	glm::vec2 center{ 320 + half_width,half_height }; // this point is going to be the center of the drawing

	int a = 100;
	float radius = 200; // choosing radius and constant
	

	//in a loop drwing the line given the formula 
	for (int i = 0; i < a; i++)
	{
		glm::vec2 point{ 320 + half_width + radius * sin(2 * PI * i / a),half_height + radius * cos(2 * PI * i / a) };
		DrawLine(center, point, black);
	}
	/************I am going to draw the logo of our Uiniversity*****************************/
	 

	// we can tell which loop is on the screen according to the color we send to Drawline
	for (int i = 0; i < 80; i++) {
		glm::vec2 start{ 100 ,100 + i };
		glm::vec2 finish{ 300 ,200 + i };
		glm::vec3 light_blue{ 0,1,1 };
		if (i % 2 == 0)
			DrawLine(start, finish, light_blue);
	}

	for (int i = 0; i < 80; i++) {
		glm::vec2 start{ 300 ,200 + i };
		glm::vec2 finish{ 500 ,100 + i };
		glm::vec3 green{ 0,1,0 };
		if (i % 2 == 0)

			DrawLine(start, finish, green);
	}

	for (int i = 0; i < 80; i++) {
		glm::vec2 start{ 300 ,100 + i };
		glm::vec2 finish{ 500 ,200 + i };
		glm::vec3 red{ 1,0,0 };
		if (i % 2 == 1)
			DrawLine(start, finish, red);
	}

	for (int i = 0; i < 80; i++) {
		glm::vec2 start{ 500 ,200 + i };
		glm::vec2 finish{ 700 ,100 + i };
		glm::vec3 red{ 1,0,0 };
		if (i % 2 == 1)
			DrawLine(start, finish, red);
	}

	for (int i = 0; i < 80; i++) {
		glm::vec2 start{ 600 ,150 + i };
		glm::vec2 finish{ 700 ,200 + i };
		glm::vec3 yellow{ 1,1,0 };
		if (i % 2 == 0)
			DrawLine(start, finish, yellow);
	}

	for (int i = 0; i < 40; i++) {
		glm::vec2 start{ 600 - 2 * i ,150 + i };
		glm::vec2 finish{ 680 - 2 * i ,190 + i };
		glm::vec3 yellow{ 1,1,0 };
		//	if (i % 2 == 0)
		DrawLine(start, finish, yellow);
	}
}
```





Screen shot of the sanity check and drawing of my choice:
i chose to draw the logo of our University

<a href="https://ibb.co/8XgDksT"><img src="https://i.ibb.co/LvdzKRb/image.png" alt="image" border="0"></a><br /><a target='_blank' href='https://imgbb.com/'></a><br />.