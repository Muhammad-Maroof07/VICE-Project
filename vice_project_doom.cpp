#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;

// ---------------------------------------------------------------
// LEVEL
// '#' = solid block, 'H' = ladder, ' ' = empty
// ---------------------------------------------------------------

// puts the tile character ch in row `row` from column c1 to column c2
void place_platform(char** lvl, const int row, const int c1, const int c2, const char ch)
{
	for (int j = c1; j <= c2; j += 1)
	{
		lvl[row][j] = ch;
	}
}

// builds one screen of a level. Tile characters:
// # G V C S B M R solid (G girder, C crumbling girder, S hazard girder, B container, M mesh, R rock)
// H ladder, Y breakable vine, Z swing vine, P pillar (decoration), W water, F waterfall (decoration), E exit
// every screen has solid ground on row 11 at both edges, so crossing between screens is always safe.
void load_screen(char** lvl, const int level, const int screen, const int height, const int width)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			lvl[i][j] = ' ';
		}
	}
	if (level == 1)
	{
		// Level 1 - Construction Site
		if (screen == 0)
		{
			place_platform(lvl, 11, 0, 5, 'G');
			place_platform(lvl, 11, 8, 11, 'G');
			place_platform(lvl, 11, 14, 17, 'G');
			place_platform(lvl, 12, 1, 1, 'P');
			place_platform(lvl, 12, 4, 4, 'P');
			place_platform(lvl, 12, 9, 10, 'S');
			place_platform(lvl, 13, 1, 1, 'P');
			place_platform(lvl, 13, 4, 4, 'P');
		}
		else if (screen == 1)
		{
			place_platform(lvl, 8, 11, 14, 'G');
			place_platform(lvl, 10, 6, 9, 'G');
			place_platform(lvl, 11, 0, 3, 'G');
			place_platform(lvl, 11, 7, 7, 'P');
			place_platform(lvl, 11, 9, 9, 'P');
			place_platform(lvl, 11, 15, 17, 'G');
			place_platform(lvl, 12, 7, 7, 'P');
			place_platform(lvl, 12, 9, 9, 'P');
			place_platform(lvl, 13, 7, 7, 'P');
			place_platform(lvl, 13, 9, 9, 'P');
		}
		else if (screen == 2)
		{
			place_platform(lvl, 9, 12, 14, 'G');
			place_platform(lvl, 11, 0, 2, 'G');
			place_platform(lvl, 11, 3, 6, 'C');
			place_platform(lvl, 11, 7, 10, 'G');
			place_platform(lvl, 11, 15, 17, 'G');
			place_platform(lvl, 12, 8, 8, 'P');
			place_platform(lvl, 13, 8, 8, 'P');
		}
		else if (screen == 3)
		{
			place_platform(lvl, 4, 13, 13, 'E');
			place_platform(lvl, 5, 8, 8, 'H');
			place_platform(lvl, 5, 9, 14, 'G');
			place_platform(lvl, 6, 8, 8, 'H');
			place_platform(lvl, 7, 8, 8, 'H');
			place_platform(lvl, 8, 8, 8, 'H');
			place_platform(lvl, 9, 8, 8, 'H');
			place_platform(lvl, 10, 8, 8, 'H');
			place_platform(lvl, 11, 0, 9, 'G');
			place_platform(lvl, 12, 2, 2, 'P');
			place_platform(lvl, 12, 6, 6, 'P');
			place_platform(lvl, 13, 2, 2, 'P');
			place_platform(lvl, 13, 6, 6, 'P');
		}
	}
	else if (level == 2)
	{
		// Level 2 - Ricardo Port
		if (screen == 0)
		{
			place_platform(lvl, 5, 12, 12, 'H');
			place_platform(lvl, 5, 13, 15, 'M');
			place_platform(lvl, 6, 12, 12, 'H');
			place_platform(lvl, 7, 2, 2, 'H');
			place_platform(lvl, 7, 3, 6, 'M');
			place_platform(lvl, 7, 12, 12, 'H');
			place_platform(lvl, 8, 2, 2, 'H');
			place_platform(lvl, 8, 12, 12, 'H');
			place_platform(lvl, 9, 2, 2, 'H');
			place_platform(lvl, 9, 9, 11, 'B');
			place_platform(lvl, 9, 12, 12, 'H');
			place_platform(lvl, 10, 2, 2, 'H');
			place_platform(lvl, 10, 9, 11, 'B');
			place_platform(lvl, 10, 12, 12, 'H');
			place_platform(lvl, 11, 0, 17, 'M');
		}
		else if (screen == 1)
		{
			place_platform(lvl, 8, 6, 9, 'M');
			place_platform(lvl, 8, 10, 10, 'H');
			place_platform(lvl, 9, 10, 10, 'H');
			place_platform(lvl, 9, 12, 14, 'B');
			place_platform(lvl, 10, 3, 5, 'B');
			place_platform(lvl, 10, 10, 10, 'H');
			place_platform(lvl, 10, 12, 14, 'B');
			place_platform(lvl, 11, 0, 17, 'M');
		}
		else if (screen == 2)
		{
			place_platform(lvl, 7, 5, 5, 'H');
			place_platform(lvl, 7, 6, 10, 'M');
			place_platform(lvl, 8, 5, 5, 'H');
			place_platform(lvl, 9, 2, 4, 'B');
			place_platform(lvl, 9, 5, 5, 'H');
			place_platform(lvl, 10, 2, 4, 'B');
			place_platform(lvl, 10, 5, 5, 'H');
			place_platform(lvl, 10, 12, 15, 'B');
			place_platform(lvl, 11, 0, 17, 'M');
		}
		else if (screen == 3)
		{
			place_platform(lvl, 5, 9, 9, 'H');
			place_platform(lvl, 5, 10, 14, 'M');
			place_platform(lvl, 6, 9, 9, 'H');
			place_platform(lvl, 7, 9, 9, 'H');
			place_platform(lvl, 8, 2, 2, 'H');
			place_platform(lvl, 8, 3, 7, 'M');
			place_platform(lvl, 8, 9, 9, 'H');
			place_platform(lvl, 9, 2, 2, 'H');
			place_platform(lvl, 9, 9, 9, 'H');
			place_platform(lvl, 10, 2, 2, 'H');
			place_platform(lvl, 10, 9, 9, 'H');
			place_platform(lvl, 11, 0, 17, 'M');
		}
		else if (screen == 4)
		{
			place_platform(lvl, 6, 11, 11, 'E');
			place_platform(lvl, 7, 7, 7, 'H');
			place_platform(lvl, 7, 8, 12, 'M');
			place_platform(lvl, 8, 7, 7, 'H');
			place_platform(lvl, 9, 4, 6, 'B');
			place_platform(lvl, 9, 7, 7, 'H');
			place_platform(lvl, 10, 4, 6, 'B');
			place_platform(lvl, 10, 7, 7, 'H');
			place_platform(lvl, 11, 0, 17, 'M');
		}
	}
	else if (level == 3)
	{
		// Level 3 - Cliffs, Waterfalls and Jungle
		if (screen == 0)
		{
			place_platform(lvl, 0, 9, 10, 'F');
			place_platform(lvl, 1, 9, 10, 'F');
			place_platform(lvl, 2, 9, 10, 'F');
			place_platform(lvl, 3, 9, 10, 'F');
			place_platform(lvl, 4, 9, 10, 'F');
			place_platform(lvl, 5, 9, 10, 'F');
			place_platform(lvl, 6, 9, 10, 'F');
			place_platform(lvl, 7, 9, 10, 'F');
			place_platform(lvl, 8, 8, 9, 'R');
			place_platform(lvl, 9, 5, 6, 'R');
			place_platform(lvl, 9, 11, 12, 'R');
			place_platform(lvl, 11, 0, 3, 'R');
			place_platform(lvl, 11, 4, 13, 'W');
			place_platform(lvl, 11, 14, 17, 'R');
			place_platform(lvl, 12, 4, 13, 'W');
			place_platform(lvl, 13, 4, 13, 'W');
		}
		else if (screen == 1)
		{
			place_platform(lvl, 0, 13, 13, 'Z');
			place_platform(lvl, 1, 13, 13, 'Z');
			place_platform(lvl, 2, 13, 13, 'Z');
			place_platform(lvl, 3, 13, 13, 'Z');
			place_platform(lvl, 4, 7, 7, 'Y');
			place_platform(lvl, 4, 8, 11, 'R');
			place_platform(lvl, 4, 15, 15, 'R');
			place_platform(lvl, 5, 7, 7, 'Y');
			place_platform(lvl, 6, 7, 7, 'Y');
			place_platform(lvl, 7, 7, 7, 'Y');
			place_platform(lvl, 8, 3, 6, 'R');
			place_platform(lvl, 8, 7, 7, 'Y');
			place_platform(lvl, 9, 7, 7, 'Y');
			place_platform(lvl, 10, 7, 7, 'Y');
			place_platform(lvl, 11, 0, 17, 'R');
		}
		else if (screen == 2)
		{
			place_platform(lvl, 7, 7, 7, 'Y');
			place_platform(lvl, 7, 8, 11, 'R');
			place_platform(lvl, 8, 7, 7, 'Y');
			place_platform(lvl, 9, 3, 5, 'R');
			place_platform(lvl, 9, 7, 7, 'Y');
			place_platform(lvl, 9, 13, 15, 'R');
			place_platform(lvl, 10, 3, 5, 'R');
			place_platform(lvl, 10, 7, 7, 'Y');
			place_platform(lvl, 10, 13, 15, 'R');
			place_platform(lvl, 11, 0, 17, 'R');
		}
		else if (screen == 3)
		{
			place_platform(lvl, 9, 7, 8, 'R');
			place_platform(lvl, 9, 10, 11, 'R');
			place_platform(lvl, 11, 0, 5, 'R');
			place_platform(lvl, 11, 6, 11, 'W');
			place_platform(lvl, 11, 12, 17, 'R');
			place_platform(lvl, 12, 6, 11, 'W');
			place_platform(lvl, 13, 6, 11, 'W');
		}
		else if (screen == 4)
		{
			place_platform(lvl, 2, 7, 7, 'Z');
			place_platform(lvl, 3, 7, 7, 'Z');
			place_platform(lvl, 4, 7, 7, 'Z');
			place_platform(lvl, 5, 7, 7, 'Z');
			place_platform(lvl, 6, 1, 5, 'R');
			place_platform(lvl, 6, 6, 6, 'Y');
			place_platform(lvl, 6, 9, 13, 'R');
			place_platform(lvl, 7, 6, 6, 'Y');
			place_platform(lvl, 8, 6, 6, 'Y');
			place_platform(lvl, 9, 6, 6, 'Y');
			place_platform(lvl, 10, 6, 6, 'Y');
			place_platform(lvl, 11, 0, 17, 'R');
		}
		else if (screen == 5)
		{
			place_platform(lvl, 7, 6, 6, 'E');
			place_platform(lvl, 8, 4, 8, 'R');
			place_platform(lvl, 8, 9, 9, 'Y');
			place_platform(lvl, 9, 9, 9, 'Y');
			place_platform(lvl, 10, 9, 9, 'Y');
			place_platform(lvl, 11, 0, 17, 'R');
		}
	}
}

// loads a screen and clears the timers of the crumbling girders and of the breakable vines
void enter_screen(char** lvl, int** crumble, int** vine, const int level, const int screen, const int height, const int width)
{
	load_screen(lvl, level, screen, height, width);
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			crumble[i][j] = 0;
			vine[i][j] = 0;
		}
	}
}

// true for every tile you can stand on or bump into
bool is_solid_char(const char c)
{
	return c == '#' || c == 'G' || c == 'V' || c == 'C' || c == 'S' || c == 'B' || c == 'M' || c == 'R';
}

// which tile picture a map character uses (-1 = nothing to draw)
int tile_number(const char c)
{
	if (c == '#') return 0;
	if (c == 'G') return 1;
	if (c == 'V' || c == 'P') return 2;
	if (c == 'C') return 3;
	if (c == 'B') return 4;
	if (c == 'M') return 5;
	if (c == 'R') return 6;
	if (c == 'S') return 7;
	if (c == 'H') return 8;
	if (c == 'E') return 9;
	if (c == 'Y') return 10;
	if (c == 'Z') return 11;
	if (c == 'W') return 12;
	if (c == 'F') return 13;
	return -1;
}

void display_level(RenderWindow& window, char** lvl, Sprite& bgSprite, Sprite tileSprite[], const int height, const int width, const int cell_size)
{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			int t = tile_number(lvl[i][j]);
			if (t >= 0)
			{
				tileSprite[t].setPosition(j * cell_size, i * cell_size);
				window.draw(tileSprite[t]);
			}
		}
	}
}

// ---------------------------------------------------------------
// COLLISION HELPERS
// ---------------------------------------------------------------

// true if the pixel point (x, y) is inside a solid block.
// left, right and bottom outside the level count as solid,
// above the top of the level counts as empty.
bool is_solid(char** lvl, const float x, const float y, const int height, const int width, const int cell_size)
{
	if (x < 0 || x >= width * cell_size || y >= height * cell_size)
	{
		return true;
	}
	if (y < 0)
	{
		return false;
	}
	return is_solid_char(lvl[(int)y / cell_size][(int)x / cell_size]);
}

// true if the pixel point (x, y) is inside a ladder cell
bool is_ladder(char** lvl, const float x, const float y, const int height, const int width, const int cell_size)
{
	if (x < 0 || x >= width * cell_size || y < 0 || y >= height * cell_size)
	{
		return false;
	}
	char c = lvl[(int)y / cell_size][(int)x / cell_size];
	return c == 'H' || c == 'Y';
}

// true if a falling player can stand on this point:
// a solid block, or the top cell of a ladder
bool is_floor(char** lvl, const float x, const float y, const int height, const int width, const int cell_size)
{
	if (is_solid(lvl, x, y, height, width, cell_size))
	{
		return true;
	}
	if (is_ladder(lvl, x, y, height, width, cell_size) && !is_ladder(lvl, x, y - cell_size, height, width, cell_size))
	{
		return true;
	}
	return false;
}

// ---------------------------------------------------------------
// PLAYER
// ---------------------------------------------------------------

// moves the player left or right by dx and stops at walls (3 points on the leading side)
void player_move_x(char** lvl, float& player_x, const float player_y, const float dx, const int screen_width, const int cell_size, const int height, const int width, const int Pheight, const int Pwidth)
{
	float new_x = player_x + dx;

	// keep inside the window
	if (new_x < 0)
	{
		new_x = 0;
	}
	if (new_x > screen_width - Pwidth)
	{
		new_x = screen_width - Pwidth;
	}

	// x of the edge we are moving toward
	float edge_x = new_x;
	if (dx > 0)
	{
		edge_x = new_x + Pwidth - 1;
	}

	bool top_hit = is_solid(lvl, edge_x, player_y, height, width, cell_size);
	bool mid_hit = is_solid(lvl, edge_x, player_y + Pheight / 2, height, width, cell_size);
	bool bottom_hit = is_solid(lvl, edge_x, player_y + Pheight - 1, height, width, cell_size);

	if (!top_hit && !mid_hit && !bottom_hit)
	{
		player_x = new_x;
	}
	else
	{
		// snap to the side of the wall so there is no gap
		int col = (int)edge_x / cell_size;
		if (dx > 0)
		{
			player_x = col * cell_size - Pwidth;
		}
		else
		{
			player_x = (col + 1) * cell_size;
		}
	}
}

// keyboard controls:
// Left/Right = move, Space = jump, Down = duck (slower walk),
// Up/Down on a ladder = climb, Space or Left/Right = leave the ladder
void player_input(char** lvl, float& player_x, float& player_y, float& velocityY, const bool onGround, bool& facingRight, bool& ducking, bool& climbing, int& Pheight, const int Pwidth, const int stand_height, const int duck_height, const float speed, const float jump_speed, const float climb_speed, const int screen_width, const int cell_size, const int height, const int width)
{
	bool left = Keyboard::isKeyPressed(Keyboard::Left);
	bool right = Keyboard::isKeyPressed(Keyboard::Right);
	bool up = Keyboard::isKeyPressed(Keyboard::Up);
	bool down = Keyboard::isKeyPressed(Keyboard::Down);
	bool jump = Keyboard::isKeyPressed(Keyboard::Space);

	float center_x = player_x + Pwidth / 2;

	// ---- start climbing ----
	if (!climbing && !ducking)
	{
		bool ladder_mid = is_ladder(lvl, center_x, player_y + Pheight / 2, height, width, cell_size);
		bool ladder_feet = is_ladder(lvl, center_x, player_y + Pheight - 1, height, width, cell_size);
		bool ladder_below = is_ladder(lvl, center_x, player_y + Pheight + 1, height, width, cell_size);

		if ((up && (ladder_mid || ladder_feet)) || (down && ladder_below))
		{
			climbing = true;
			velocityY = 0;
			// center the player on the ladder
			player_x = (int)center_x / cell_size * cell_size + cell_size / 2 - Pwidth / 2;
		}
	}

	// ---- climbing ----
	if (climbing)
	{
		velocityY = 0;

		if (jump)
		{
			climbing = false;
			velocityY = jump_speed;
		}
		else if (left || right)
		{
			climbing = false;
		}
		else if (up)
		{
			player_y -= climb_speed;
			if (!is_ladder(lvl, center_x, player_y + Pheight / 2, height, width, cell_size))
			{
				// reached the top: stand on top of the ladder
				int row = (int)(player_y + Pheight) / cell_size;
				player_y = row * cell_size - Pheight;
				climbing = false;
			}
		}
		else if (down)
		{
			player_y += climb_speed;
			if (is_solid(lvl, center_x, player_y + Pheight, height, width, cell_size))
			{
				// reached the ground: stand on it
				int row = (int)(player_y + Pheight) / cell_size;
				player_y = row * cell_size - Pheight;
				climbing = false;
			}
		}

		if (climbing)
		{
			return;
		}
	}

	// ---- ducking ----
	bool want_duck = down && onGround;

	if (want_duck && !ducking)
	{
		ducking = true;
		Pheight = duck_height;
		player_y += stand_height - duck_height;
	}
	else if (!want_duck && ducking)
	{
		// only stand up if there is room above the head
		float new_top = player_y - (stand_height - duck_height);
		bool left_hit = is_solid(lvl, player_x, new_top, height, width, cell_size);
		bool mid_hit = is_solid(lvl, player_x + Pwidth / 2, new_top, height, width, cell_size);
		bool right_hit = is_solid(lvl, player_x + Pwidth - 1, new_top, height, width, cell_size);

		if (!left_hit && !mid_hit && !right_hit)
		{
			ducking = false;
			Pheight = stand_height;
			player_y = new_top;
		}
	}

	// ---- walking ----
	float current_speed = speed;
	if (ducking)
	{
		current_speed = speed / 2;
	}

	float dx = 0;
	if (left)
	{
		dx -= current_speed;
		facingRight = false;
	}
	if (right)
	{
		dx += current_speed;
		facingRight = true;
	}

	if (dx != 0)
	{
		player_move_x(lvl, player_x, player_y, dx, screen_width, cell_size, height, width, Pheight, Pwidth);
	}

	// ---- jumping ----
	if (jump && onGround && !ducking)
	{
		velocityY = jump_speed;
	}
}

// gravity, landing on floors and hitting ceilings
void player_gravity(char** lvl, float& velocityY, bool& onGround, const float gravity, const float terminal_velocity, float& player_x, float& player_y, const int cell_size, const int height, const int width, const int Pheight, const int Pwidth)
{
	float new_y = player_y + velocityY;

	float left_x = player_x;
	float mid_x = player_x + Pwidth / 2;
	float right_x = player_x + Pwidth - 1;

	if (velocityY >= 0)
	{
		// falling or standing: check the three points under the feet
		float feet_y = new_y + Pheight;

		bool left_hit = is_floor(lvl, left_x, feet_y, height, width, cell_size);
		bool mid_hit = is_floor(lvl, mid_x, feet_y, height, width, cell_size);
		bool right_hit = is_floor(lvl, right_x, feet_y, height, width, cell_size);

		if (left_hit || mid_hit || right_hit)
		{
			// land exactly on top of the block
			int row = (int)feet_y / cell_size;
			player_y = row * cell_size - Pheight;
			velocityY = 0;
			onGround = true;
			return;
		}

		player_y = new_y;
		onGround = false;
	}
	else
	{
		// rising: check the three points above the head
		onGround = false;

		if (new_y < 0)
		{
			player_y = 0;
			velocityY = 0;
		}
		else
		{
			bool left_hit = is_solid(lvl, left_x, new_y, height, width, cell_size);
			bool mid_hit = is_solid(lvl, mid_x, new_y, height, width, cell_size);
			bool right_hit = is_solid(lvl, right_x, new_y, height, width, cell_size);

			if (left_hit || mid_hit || right_hit)
			{
				// bump the head and start falling
				int row = (int)new_y / cell_size;
				player_y = (row + 1) * cell_size;
				velocityY = 0;
			}
			else
			{
				player_y = new_y;
			}
		}
	}

	velocityY += gravity;
	if (velocityY >= terminal_velocity)
	{
		velocityY = terminal_velocity;
	}
}

// ---------------------------------------------------------------
// HELPERS (rand, srand and string are not allowed, so we write our own)
// ---------------------------------------------------------------

// simple random number generator: returns a number from 0 to max - 1.
// the seed is a variable in main (seeded once with time(0)), so there is no global.
int random_number(long long& seed, const int max)
{
	seed = (seed * 1103515245 + 12345) % 2147483648;
	return (int)((seed / 65536) % max);
}

// adds the text src at the end of the text dest (both end with '\0')
void append_text(char dest[], const char src[])
{
	int d = 0;
	while (dest[d] != '\0')
	{
		d += 1;
	}
	int i = 0;
	while (src[i] != '\0')
	{
		dest[d] = src[i];
		d += 1;
		i += 1;
	}
	dest[d] = '\0';
}

// adds a whole number (0 or more) at the end of the text dest
void append_number(char dest[], int number)
{
	char digits[12];
	int count = 0;

	if (number == 0)
	{
		digits[0] = '0';
		count = 1;
	}
	while (number > 0)
	{
		digits[count] = '0' + number % 10;
		number = number / 10;
		count += 1;
	}

	int d = 0;
	while (dest[d] != '\0')
	{
		d += 1;
	}
	for (int i = count - 1; i >= 0; i -= 1)
	{
		dest[d] = digits[i];
		d += 1;
	}
	dest[d] = '\0';
}

// ---------------------------------------------------------------
// CRUMBLING GIRDERS ('C'): 30 frames after the player stands on one it disappears,
// and it comes back 180 frames later (when the player is not inside it).
// crumble[row][col]: 0 = normal, 1..29 = about to fall, negative = gone, counting up to 0
// ---------------------------------------------------------------
void update_crumbling(char** lvl, int** crumble, const float player_x, const float player_y, const int Pheight, const int Pwidth, const bool onGround, const int height, const int width, const int cell_size)
{
	// start the timer of the girder under the player's feet
	if (onGround)
	{
		int row = (int)(player_y + Pheight + 1) / cell_size;
		for (int k = 0; k < 3; k += 1)
		{
			float x = player_x + k * (Pwidth - 1) / 2;
			int col = (int)x / cell_size;
			if (row < height && col >= 0 && col < width && lvl[row][col] == 'C' && crumble[row][col] == 0)
			{
				crumble[row][col] = 1;
			}
		}
	}

	for (int r = 0; r < height; r += 1)
	{
		for (int c = 0; c < width; c += 1)
		{
			if (crumble[r][c] > 0)
			{
				crumble[r][c] += 1;
				if (crumble[r][c] >= 30)
				{
					lvl[r][c] = ' ';
					crumble[r][c] = -180;
				}
			}
			else if (crumble[r][c] < 0)
			{
				crumble[r][c] += 1;
				if (crumble[r][c] == 0)
				{
					// do not rebuild the girder around the player
					bool inside = player_x < (c + 1) * cell_size && player_x + Pwidth > c * cell_size && player_y < (r + 1) * cell_size && player_y + Pheight > r * cell_size;
					if (inside)
					{
						crumble[r][c] = -1;
					}
					else
					{
						lvl[r][c] = 'C';
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------
// ENEMIES
// every enemy lives in a set of parallel arrays (index i = one enemy)
// type 0 = spiked crawler, 1 = blue jumper, 2 = flying insect, 3 = leopard,
// 4 = green shooter, 5 = tribal warrior, 6 = leaping fish
// ---------------------------------------------------------------

int enemy_width(const int type)
{
	if (type == 0 || type == 5)
	{
		return 48;
	}
	if (type == 2)
	{
		return 78;
	}
	if (type == 3)
	{
		return 64;
	}
	if (type == 6)
	{
		return 32;
	}
	return 40;
}

int enemy_height(const int type)
{
	if (type == 0 || type == 3)
	{
		return 32;
	}
	if (type == 2)
	{
		return 48;
	}
	if (type == 4)
	{
		return 56;
	}
	if (type == 5)
	{
		return 88;
	}
	return 40;
}

// points for killing an enemy
int enemy_points(const int type)
{
	if (type == 0)
	{
		return 50;
	}
	if (type == 1 || type == 2)
	{
		return 75;
	}
	if (type == 5)
	{
		return 125;
	}
	return 100;
}

// damage an enemy does when the player touches it
int enemy_damage(const int type)
{
	if (type == 3 || type == 5 || type == 6)
	{
		return 15;
	}
	return 10;
}

// true if two rectangles overlap
bool rects_overlap(const float ax, const float ay, const float aw, const float ah, const float bx, const float by, const float bw, const float bh)
{
	return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

// creates the enemies of a level. returns how many were created.
// level 1: 4 crawlers, 5 jumpers, 6 insects
// level 2: 10 leopards, 4 shooters, 8 tribal warriors
// level 3: 10 fish, 8 tribal warriors, 10 leopards, 7 insects, 5 jumpers
// crawlers, jumpers, leopards and tribal warriors are put on a random girder, insects fly at a random height,
// shooters and fish have fixed places.
int spawn_enemies(const int level, int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], int en_dir[], int en_health[], bool en_alive[], bool en_ground[], int en_timer[], long long& seed, const int max_enemies, const int total_screens, const int screen_width, char** temp_lvl, const int height, const int width, const int cell_size)
{
	int count[7] = { 0, 0, 0, 0, 0, 0, 0 };
	if (level == 1)
	{
		count[0] = 4;
		count[1] = 5;
		count[2] = 6;
	}
	else if (level == 2)
	{
		count[3] = 10;
		count[4] = 4;
		count[5] = 8;
	}
	else
	{
		count[6] = 10;
		count[5] = 8;
		count[3] = 10;
		count[2] = 7;
		count[1] = 5;
	}

	// fixed places: level 2 shooters (screen, column, girder row) and level 3 fish (screen, water column)
	const int shooter_screen[4] = { 1, 2, 3, 4 };
	const int shooter_col[4] = { 8, 9, 13, 9 };
	const int shooter_row[4] = { 8, 7, 5, 7 };
	const int fish_screen[10] = { 0, 0, 0, 0, 0, 3, 3, 3, 3, 3 };
	const int fish_col[10] = { 5, 7, 9, 11, 12, 6, 7, 9, 10, 11 };

	for (int i = 0; i < max_enemies; i += 1)
	{
		en_alive[i] = false;
		en_screen[i] = -1;
		en_type[i] = 0;
		en_x[i] = 0;
		en_y[i] = 0;
		en_vx[i] = 0;
		en_vy[i] = 0;
		en_dir[i] = 1;
		en_health[i] = 10;
		en_ground[i] = true;
		en_timer[i] = 0;
	}

	int n = 0;
	int shooters = 0;
	int fish = 0;

	for (int type = 0; type < 7; type += 1)
	{
		for (int k = 0; k < count[type] && n < max_enemies; k += 1)
		{
			int i = n;
			n += 1;

			int w = enemy_width(type);
			int h = enemy_height(type);

			en_type[i] = type;
			en_alive[i] = true;
			en_ground[i] = true;
			en_health[i] = 10;
			if (type == 5)
			{
				en_health[i] = 20;
			}
			if (random_number(seed, 2) == 0)
			{
				en_dir[i] = 1;
			}
			else
			{
				en_dir[i] = -1;
			}
			en_timer[i] = 40 + random_number(seed, 40);

			if (type == 2)
			{
				// flying insect: random screen and x; en_vy keeps its base height, en_timer is its bobbing phase
				en_screen[i] = random_number(seed, total_screens);
				en_x[i] = 200 + random_number(seed, screen_width - 400);
				en_vy[i] = 380 + random_number(seed, 180);
				en_y[i] = en_vy[i];
				en_timer[i] = random_number(seed, 90);
				en_ground[i] = false;
			}
			else if (type == 4)
			{
				// green shooter: stands on a fixed girder
				int s = shooters % 4;
				shooters += 1;
				en_screen[i] = shooter_screen[s];
				en_x[i] = shooter_col[s] * cell_size + (cell_size - w) / 2;
				en_y[i] = shooter_row[s] * cell_size - h;
				en_timer[i] = 60 + random_number(seed, 60);
			}
			else if (type == 6)
			{
				// leaping fish: waits under the water, hidden by the status bar (y = 790)
				int f = fish % 10;
				fish += 1;
				en_screen[i] = fish_screen[f];
				en_x[i] = fish_col[f] * cell_size + (cell_size - w) / 2;
				en_y[i] = 790;
				en_timer[i] = 40 + random_number(seed, 120);
			}
			else
			{
				// ground enemy: a random girder
				bool found = false;
				for (int attempt = 0; attempt < 120 && !found; attempt += 1)
				{
					int screen = random_number(seed, total_screens);
					int col = 2 + random_number(seed, width - 4);

					// not on the start platform, so nothing spawns on top of the player
					if (screen == 0 && col < 6)
					{
						continue;
					}

					load_screen(temp_lvl, level, screen, height, width);

					// every solid, non-crumbling cell in this column that has free space above it
					int candidates[14];
					int number = 0;
					for (int row = 2; row < height; row += 1)
					{
						if (is_solid_char(temp_lvl[row][col]) && temp_lvl[row][col] != 'C' && temp_lvl[row - 1][col] == ' ' && temp_lvl[row - 2][col] == ' ')
						{
							candidates[number] = row;
							number += 1;
						}
					}

					if (number > 0)
					{
						int row = candidates[random_number(seed, number)];
						found = true;
						en_screen[i] = screen;
						en_x[i] = col * cell_size + (cell_size - w) / 2;
						en_y[i] = row * cell_size - h;
					}
				}

				if (!found)
				{
					// fallback: a spot on the last screen of the level
					en_screen[i] = total_screens - 1;
					en_x[i] = 5 * cell_size;
					en_y[i] = 11 * cell_size - h;
				}
			}
		}
	}

	return n;
}

// moves a ground enemy sideways along its girder.
// returns 0 = moved, 1 = blocked by a wall, a ledge or the screen edge, 2 = the girder under it is gone (it falls)
int walk_enemy(char** lvl, const int i, float en_x[], float en_y[], int en_dir[], bool en_alive[], const int w, const int h, const float speed, const int screen_width, const int height, const int width, const int cell_size)
{
	bool supported = is_solid(lvl, en_x[i] + 4, en_y[i] + h + 1, height, width, cell_size) || is_solid(lvl, en_x[i] + w - 5, en_y[i] + h + 1, height, width, cell_size);
	if (!supported)
	{
		en_y[i] += 6;
		if (en_y[i] + h >= height * cell_size - 1)
		{
			en_alive[i] = false;
		}
		return 2;
	}

	float new_x = en_x[i] + en_dir[i] * speed;

	// x of the edge we are walking toward
	float lead_x = new_x;
	if (en_dir[i] > 0)
	{
		lead_x = new_x + w - 1;
	}

	bool wall = is_solid(lvl, lead_x, en_y[i] + h / 2, height, width, cell_size);
	bool ledge = !is_solid(lvl, lead_x, en_y[i] + h + 1, height, width, cell_size);
	bool edge = new_x < 0 || new_x > screen_width - w;

	if (wall || ledge || edge)
	{
		return 1;
	}
	en_x[i] = new_x;
	return 0;
}

// gravity and sideways movement in the air for jumpers and leaping leopards:
// they land on girders, bump into walls and ceilings, and disappear in a pit
void enemy_physics(char** lvl, const int i, float en_x[], float en_y[], float en_vx[], float en_vy[], bool en_alive[], bool en_ground[], const int w, const int h, const float gravity, const int screen_width, const int height, const int width, const int cell_size)
{
	// horizontal movement (only while in the air)
	if (en_vx[i] != 0)
	{
		float new_x = en_x[i] + en_vx[i];
		float lead_x = new_x;
		if (en_vx[i] > 0)
		{
			lead_x = new_x + w - 1;
		}

		if (new_x < 0 || new_x > screen_width - w || is_solid(lvl, lead_x, en_y[i] + 2, height, width, cell_size) || is_solid(lvl, lead_x, en_y[i] + h - 2, height, width, cell_size))
		{
			en_vx[i] = 0;
		}
		else
		{
			en_x[i] = new_x;
		}
	}

	// vertical movement with gravity
	en_vy[i] += gravity;
	if (en_vy[i] > 16)
	{
		en_vy[i] = 16;
	}
	float new_y = en_y[i] + en_vy[i];

	if (en_vy[i] >= 0)
	{
		float feet_y = new_y + h;
		if (is_solid(lvl, en_x[i] + 2, feet_y, height, width, cell_size) || is_solid(lvl, en_x[i] + w - 3, feet_y, height, width, cell_size))
		{
			// landed: stand on the block and stop moving sideways
			int row = (int)feet_y / cell_size;
			en_y[i] = row * cell_size - h;
			en_vy[i] = 0;
			en_vx[i] = 0;
			en_ground[i] = true;
		}
		else
		{
			en_y[i] = new_y;
			en_ground[i] = false;
		}
	}
	else
	{
		en_ground[i] = false;
		if (new_y < 0 || is_solid(lvl, en_x[i] + 2, new_y, height, width, cell_size) || is_solid(lvl, en_x[i] + w - 3, new_y, height, width, cell_size))
		{
			en_vy[i] = 0;
		}
		else
		{
			en_y[i] = new_y;
		}
	}

	// falling into a pit removes the enemy
	if (en_y[i] + h >= height * cell_size - 1)
	{
		en_alive[i] = false;
	}
}

// moves the enemies that are on the current screen
// crawler: walks along its girder and turns at walls and ledges
// blue jumper: waits, then hops toward the player
// flying insect: flies sideways and bobs up and down
// leopard: patrols, sprints when the player is near on the same level, then leaps
// green shooter: stands still and turns toward the player (its shots are made by enemy_fire)
// tribal warrior: patrols and charges at the player when he is near on the same level
// leaping fish: waits under the water, then jumps out
void update_enemies(char** lvl, const int current_screen, const float player_x, const float player_y, const int Pheight, int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], int en_dir[], bool en_alive[], bool en_ground[], int en_timer[], long long& seed, const int max_enemies, const float gravity, const int screen_width, const int height, const int width, const int cell_size)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (!en_alive[i] || en_screen[i] != current_screen)
		{
			continue;
		}

		int type = en_type[i];
		int w = enemy_width(type);
		int h = enemy_height(type);

		float dx = player_x - en_x[i];
		int toward = 1;
		if (dx < 0)
		{
			toward = -1;
			dx = -dx;
		}
		// is the player standing at about the same height as this enemy?
		float level_gap = (player_y + Pheight) - (en_y[i] + h);
		if (level_gap < 0)
		{
			level_gap = -level_gap;
		}
		bool same_level = level_gap < 40;

		if (type == 0)
		{
			// ---- spiked crawler ----
			if (walk_enemy(lvl, i, en_x, en_y, en_dir, en_alive, w, h, 2, screen_width, height, width, cell_size) == 1)
			{
				en_dir[i] = -en_dir[i];
			}
		}
		else if (type == 1)
		{
			// ---- blue jumper ----
			enemy_physics(lvl, i, en_x, en_y, en_vx, en_vy, en_alive, en_ground, w, h, gravity, screen_width, height, width, cell_size);

			// on the ground: count down, then jump toward the player
			if (en_alive[i] && en_ground[i])
			{
				en_timer[i] -= 1;
				if (en_timer[i] <= 0)
				{
					en_dir[i] = toward;
					en_vx[i] = en_dir[i] * 3.5;
					en_vy[i] = -13;
					en_ground[i] = false;
					en_timer[i] = 40 + random_number(seed, 40);
				}
			}
		}
		else if (type == 2)
		{
			// ---- flying insect ----
			// flies sideways at 2 pixels per frame and turns at the screen edges,
			// while bobbing up and down around its base height (en_vy)
			float new_x = en_x[i] + en_dir[i] * 2;
			if (new_x < 0 || new_x > screen_width - w)
			{
				en_dir[i] = -en_dir[i];
			}
			else
			{
				en_x[i] = new_x;
			}

			en_timer[i] += 1;
			en_y[i] = en_vy[i] + 40 * sin(en_timer[i] * 6.2832 / 90);
		}
		else if (type == 3)
		{
			// ---- leopard ----
			enemy_physics(lvl, i, en_x, en_y, en_vx, en_vy, en_alive, en_ground, w, h, gravity, screen_width, height, width, cell_size);

			if (en_alive[i] && en_ground[i])
			{
				if (en_timer[i] > 0)
				{
					en_timer[i] -= 1;
				}

				bool near_player = same_level && dx < 6 * cell_size;
				float speed = 2;
				if (near_player)
				{
					speed = 5;
					en_dir[i] = toward;
				}

				if (near_player && dx < 3 * cell_size && en_timer[i] <= 0)
				{
					// leap at the player
					en_vx[i] = 7 * toward;
					en_vy[i] = -12;
					en_ground[i] = false;
					en_timer[i] = 90;
				}
				else if (walk_enemy(lvl, i, en_x, en_y, en_dir, en_alive, w, h, speed, screen_width, height, width, cell_size) == 1)
				{
					// patrolling leopards turn around, sprinting ones stop at the ledge
					if (!near_player)
					{
						en_dir[i] = -en_dir[i];
					}
				}
			}
		}
		else if (type == 4)
		{
			// ---- green shooter ----
			en_dir[i] = toward;
		}
		else if (type == 5)
		{
			// ---- tribal warrior ----
			bool charge = same_level && dx < 4 * cell_size;
			float speed = 1.5;
			if (charge)
			{
				speed = 3;
				en_dir[i] = toward;
			}
			if (walk_enemy(lvl, i, en_x, en_y, en_dir, en_alive, w, h, speed, screen_width, height, width, cell_size) == 1)
			{
				if (!charge)
				{
					en_dir[i] = -en_dir[i];
				}
			}
		}
		else
		{
			// ---- leaping fish ----
			// en_y = 790 means it rests under the water (hidden by the status bar).
			// en_vx is 0 while it has not shot yet during this leap.
			en_dir[i] = toward;
			if (en_ground[i])
			{
				en_timer[i] -= 1;
				if (en_timer[i] <= 0)
				{
					en_ground[i] = false;
					en_vy[i] = -16;
					en_vx[i] = 0;
				}
			}
			else
			{
				en_y[i] += en_vy[i];
				en_vy[i] += 0.8;
				if (en_y[i] >= 790)
				{
					en_y[i] = 790;
					en_vy[i] = 0;
					en_ground[i] = true;
					en_timer[i] = 80 + random_number(seed, 80);
				}
			}
		}
	}
}

// damages every living enemy on the current screen that overlaps the rectangle.
// returns true if at least one enemy was hit. a kill adds the enemy's points to the score.
bool hit_enemies(const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], int en_health[], bool en_alive[], const int max_enemies, const float rx, const float ry, const float rw, const float rh, const int damage, int& score)
{
	bool hit = false;

	for (int i = 0; i < max_enemies; i += 1)
	{
		if (en_alive[i] && en_screen[i] == current_screen)
		{
			if (rects_overlap(rx, ry, rw, rh, en_x[i], en_y[i], enemy_width(en_type[i]), enemy_height(en_type[i])))
			{
				hit = true;
				en_health[i] -= damage;
				if (en_health[i] <= 0)
				{
					en_alive[i] = false;
					score += enemy_points(en_type[i]);
				}
			}
		}
	}

	return hit;
}

// true if a living enemy on the current screen overlaps the rectangle
bool touching_enemy(const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], bool en_alive[], const int max_enemies, const float rx, const float ry, const float rw, const float rh)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (en_alive[i] && en_screen[i] == current_screen)
		{
			if (rects_overlap(rx, ry, rw, rh, en_x[i], en_y[i], enemy_width(en_type[i]), enemy_height(en_type[i])))
			{
				return true;
			}
		}
	}
	return false;
}

// the biggest contact damage of the living enemies that touch the rectangle (0 = none)
int touching_damage(const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], bool en_alive[], const int max_enemies, const float rx, const float ry, const float rw, const float rh)
{
	int worst = 0;
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (en_alive[i] && en_screen[i] == current_screen)
		{
			if (rects_overlap(rx, ry, rw, rh, en_x[i], en_y[i], enemy_width(en_type[i]), enemy_height(en_type[i])))
			{
				if (enemy_damage(en_type[i]) > worst)
				{
					worst = enemy_damage(en_type[i]);
				}
			}
		}
	}
	return worst;
}

void draw_enemies(RenderWindow& window, Sprite enemySprite[], const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], int en_dir[], bool en_alive[], const int max_enemies)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (en_alive[i] && en_screen[i] == current_screen)
		{
			int t = en_type[i];

			// every picture faces right except the flying insect
			bool face_right = en_dir[i] > 0;
			if (t == 2)
			{
				face_right = !face_right;
			}

			if (face_right)
			{
				enemySprite[t].setScale(1, 1);
				enemySprite[t].setPosition(en_x[i], en_y[i]);
			}
			else
			{
				enemySprite[t].setScale(-1, 1);
				enemySprite[t].setPosition(en_x[i] + enemy_width(t), en_y[i]);
			}
			window.draw(enemySprite[t]);
		}
	}
}

// ---------------------------------------------------------------
// ENEMY SHOTS: bolts of the green shooters (kind 0, 24 x 8) and fireballs of the fish (kind 1, 16 x 16)
// ---------------------------------------------------------------

void clear_shots(bool shot_active[], const int max_shots)
{
	for (int i = 0; i < max_shots; i += 1)
	{
		shot_active[i] = false;
	}
}

void add_shot(float shot_x[], float shot_y[], float shot_vx[], int shot_kind[], bool shot_active[], const int max_shots, const float x, const float y, const float vx, const int kind)
{
	for (int i = 0; i < max_shots; i += 1)
	{
		if (!shot_active[i])
		{
			shot_active[i] = true;
			shot_x[i] = x;
			shot_y[i] = y;
			shot_vx[i] = vx;
			shot_kind[i] = kind;
			return;
		}
	}
}

// shooters fire a bolt every 90 frames, fish throw a fireball at the top of their jump
void enemy_fire(const int current_screen, const float player_x, int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], bool en_alive[], bool en_ground[], int en_timer[], const int max_enemies, float shot_x[], float shot_y[], float shot_vx[], int shot_kind[], bool shot_active[], const int max_shots)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (!en_alive[i] || en_screen[i] != current_screen)
		{
			continue;
		}

		int dir = 1;
		if (player_x < en_x[i])
		{
			dir = -1;
		}

		if (en_type[i] == 4)
		{
			en_timer[i] -= 1;
			if (en_timer[i] <= 0)
			{
				float x = en_x[i] + enemy_width(4);
				if (dir < 0)
				{
					x = en_x[i] - 24;
				}
				add_shot(shot_x, shot_y, shot_vx, shot_kind, shot_active, max_shots, x, en_y[i] + 24, 6 * dir, 0);
				en_timer[i] = 90;
			}
		}
		else if (en_type[i] == 6)
		{
			if (!en_ground[i] && en_vx[i] == 0 && en_vy[i] >= 0)
			{
				add_shot(shot_x, shot_y, shot_vx, shot_kind, shot_active, max_shots, en_x[i] + 8, en_y[i] + 8, 5 * dir, 1);
				en_vx[i] = 1;
			}
		}
	}
}

// moves the shots; they disappear at walls and at the edge of the screen
void update_enemy_shots(char** lvl, float shot_x[], float shot_y[], float shot_vx[], int shot_kind[], bool shot_active[], const int max_shots, const int screen_width, const int height, const int width, const int cell_size)
{
	for (int i = 0; i < max_shots; i += 1)
	{
		if (shot_active[i])
		{
			shot_x[i] += shot_vx[i];

			int w = 24;
			int h = 8;
			if (shot_kind[i] == 1)
			{
				w = 16;
				h = 16;
			}

			float lead_x = shot_x[i];
			if (shot_vx[i] > 0)
			{
				lead_x += w;
			}

			if (lead_x < 0 || lead_x >= screen_width || is_solid(lvl, lead_x, shot_y[i] + h / 2, height, width, cell_size))
			{
				shot_active[i] = false;
			}
		}
	}
}

void draw_enemy_shots(RenderWindow& window, Sprite shotSprite[], float shot_x[], float shot_y[], float shot_vx[], int shot_kind[], bool shot_active[], const int max_shots)
{
	for (int i = 0; i < max_shots; i += 1)
	{
		if (shot_active[i])
		{
			int k = shot_kind[i];
			if (shot_vx[i] > 0 || k == 1)
			{
				shotSprite[k].setScale(1, 1);
				shotSprite[k].setPosition(shot_x[i], shot_y[i]);
			}
			else
			{
				// the bolt picture points right: mirror it for shots going left
				shotSprite[k].setScale(-1, 1);
				shotSprite[k].setPosition(shot_x[i] + 24, shot_y[i]);
			}
			window.draw(shotSprite[k]);
		}
	}
}

// ---------------------------------------------------------------
// WEAPONS AND HUD
// weapon 1 = laser whip (unlimited), 2 = .44 magnum, 3 = grenades
// ---------------------------------------------------------------

// keys 1, 2, 3 select the weapon
void switch_weapon(int& weapon)
{
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		weapon = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		weapon = 2;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3))
	{
		weapon = 3;
	}
}

// X fires the selected weapon, one shot per key press
void fire_weapon(const int weapon, bool& prev_fire, int& magnum_ammo, int& grenade_ammo, int& whip_timer, float bullet_x[], float bullet_y[], float bullet_dir[], bool bullet_active[], const int max_bullets, float grenade_x[], float grenade_y[], float grenade_vx[], float grenade_vy[], bool grenade_active[], const int max_grenades, const float player_x, const float player_y, const bool facingRight, const int Pwidth, const int Pheight)
{
	bool fire = Keyboard::isKeyPressed(Keyboard::X);

	if (fire && !prev_fire)
	{
		if (weapon == 1)
		{
			if (whip_timer == 0)
			{
				whip_timer = 12;
			}
		}
		else if (weapon == 2 && magnum_ammo > 0)
		{
			for (int i = 0; i < max_bullets; i += 1)
			{
				if (!bullet_active[i])
				{
					bullet_active[i] = true;
					bullet_y[i] = player_y + Pheight * 45 / 100;
					if (facingRight)
					{
						bullet_dir[i] = 1;
						bullet_x[i] = player_x + Pwidth;
					}
					else
					{
						bullet_dir[i] = -1;
						bullet_x[i] = player_x - 16;
					}
					magnum_ammo -= 1;
					break;
				}
			}
		}
		else if (weapon == 3 && grenade_ammo > 0)
		{
			for (int i = 0; i < max_grenades; i += 1)
			{
				if (!grenade_active[i])
				{
					grenade_active[i] = true;
					grenade_x[i] = player_x + Pwidth / 2 - 8;
					grenade_y[i] = player_y + 36;
					grenade_vy[i] = -10;
					if (facingRight)
					{
						grenade_vx[i] = 9;
					}
					else
					{
						grenade_vx[i] = -9;
					}
					grenade_ammo -= 1;
					break;
				}
			}
		}
	}

	prev_fire = fire;
}

// moves the magnum bullets and removes them when they hit a wall or leave the screen
void update_bullets(char** lvl, float bullet_x[], float bullet_y[], float bullet_dir[], bool bullet_active[], const int max_bullets, const float bullet_speed, const int screen_width, const int height, const int width, const int cell_size)
{
	for (int i = 0; i < max_bullets; i += 1)
	{
		if (bullet_active[i])
		{
			bullet_x[i] += bullet_dir[i] * bullet_speed;

			// leading edge of the bullet (the bullet is 16 pixels wide)
			float hit_x = bullet_x[i];
			if (bullet_dir[i] > 0)
			{
				hit_x += 16;
			}

			if (hit_x < 0 || hit_x >= screen_width || is_solid(lvl, hit_x, bullet_y[i] + 4, height, width, cell_size))
			{
				bullet_active[i] = false;
			}
		}
	}
}

// moves the grenades in an arc. a grenade explodes when it hits something
// and deals 40 damage to every enemy within about 100 pixels.
void update_grenades(char** lvl, float grenade_x[], float grenade_y[], float grenade_vx[], float grenade_vy[], bool grenade_active[], const int max_grenades, const float gravity, const int screen_width, const int height, const int width, const int cell_size, const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], int en_health[], bool en_alive[], const int max_enemies, int& score)
{
	for (int i = 0; i < max_grenades; i += 1)
	{
		if (grenade_active[i])
		{
			grenade_x[i] += grenade_vx[i];
			grenade_y[i] += grenade_vy[i];
			grenade_vy[i] += gravity;

			// centre of the grenade (it is 16 x 16 pixels)
			float cx = grenade_x[i] + 8;
			float cy = grenade_y[i] + 8;

			bool hit_enemy = touching_enemy(current_screen, en_type, en_screen, en_x, en_y, en_alive, max_enemies, grenade_x[i], grenade_y[i], 16, 16);

			if (cx < 0 || cx >= screen_width || is_solid(lvl, cx, cy, height, width, cell_size) || hit_enemy)
			{
				grenade_active[i] = false;
				// explosion: damage everything inside a 200 x 200 square around the grenade
				hit_enemies(current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, cx - 100, cy - 100, 200, 200, 40, score);
			}
		}
	}
}

void draw_projectiles(RenderWindow& window, Sprite& bulletSprite, Sprite& grenadeSprite, float bullet_x[], float bullet_y[], bool bullet_active[], const int max_bullets, float grenade_x[], float grenade_y[], bool grenade_active[], const int max_grenades)
{
	for (int i = 0; i < max_bullets; i += 1)
	{
		if (bullet_active[i])
		{
			bulletSprite.setPosition(bullet_x[i], bullet_y[i]);
			window.draw(bulletSprite);
		}
	}
	for (int i = 0; i < max_grenades; i += 1)
	{
		if (grenade_active[i])
		{
			grenadeSprite.setPosition(grenade_x[i], grenade_y[i]);
			window.draw(grenadeSprite);
		}
	}
}

// ---------------------------------------------------------------
// HUD: the status bar at the bottom of the screen, laid out like the NES game.
// All pieces (font, icons, weapon boxes, bar segments) come from Data/hud_sheet.png.
// HUD pixels are drawn 4 times bigger; (x, y) are in HUD pixels from the top left of the bar.
// ---------------------------------------------------------------

// draws the part (tx, ty, tw, th) of hud_sheet.png at HUD pixel (x, y)
void draw_hud_piece(RenderWindow& window, Sprite& hud, const int tx, const int ty, const int tw, const int th, const int x, const int y, const float hud_x, const float hud_y)
{
	hud.setTextureRect(IntRect(tx, ty, tw, th));
	hud.setScale(4, 4);
	hud.setPosition(hud_x + x * 4, hud_y + y * 4);
	window.draw(hud);
}

// draws digits, capital letters and '-' (8 HUD pixels per character)
void draw_hud_text(RenderWindow& window, Sprite& hud, const char text[], const int x, const int y, const float hud_x, const float hud_y)
{
	int i = 0;
	while (text[i] != '\0')
	{
		char c = text[i];
		int tx = -1;
		int ty = 0;

		if (c >= '0' && c <= '9')
		{
			tx = (c - '0') * 8;
			ty = 24;
		}
		else if (c >= 'A' && c <= 'O')
		{
			tx = (c - 'A' + 1) * 8;
			ty = 32;
		}
		else if (c >= 'P' && c <= 'Z')
		{
			tx = (c - 'P') * 8;
			ty = 40;
		}
		else if (c == '-')
		{
			tx = 88;
			ty = 24;
		}

		if (tx >= 0)
		{
			draw_hud_piece(window, hud, tx, ty, 8, 8, x + i * 8, y, hud_x, hud_y);
		}
		i += 1;
	}
}

// draws a whole number so that its last digit ends at x_end
void draw_hud_number(RenderWindow& window, Sprite& hud, const int number, const int x_end, const int y, const float hud_x, const float hud_y)
{
	char text[12];
	text[0] = '\0';
	append_number(text, number);

	int length = 0;
	while (text[length] != '\0')
	{
		length += 1;
	}
	draw_hud_text(window, hud, text, x_end - length * 8, y, hud_x, hud_y);
}

// the whole status bar: score, ammo, time, lives, kills, weapon boxes, health bar and stage
void draw_hud(RenderWindow& window, Sprite& hud, Sprite& hudBack, const int screen_width, const int screen_height, const int weapon, const int magnum_ammo, const int grenade_ammo, const int health, const int max_health, const int time_left, const int lives, const int defeated, const int score, const int level)
{
	const int strip_height = 28 * 4;
	float hud_x = (screen_width - 256 * 4) / 2;
	float hud_y = screen_height - strip_height;

	// black strip along the bottom of the window
	hudBack.setScale(screen_width, strip_height);
	hudBack.setPosition(0, hud_y);
	window.draw(hudBack);

	const int left = 2;
	const int row1 = 3;
	const int row2 = 10;
	const int row3 = 17;

	// row 1: SCORE, first ammo, TIME, lives, defeated enemies
	draw_hud_text(window, hud, "SCORE", left, row1, hud_x, hud_y);
	draw_hud_piece(window, hud, 24, 16, 8, 8, left + 56, row1, hud_x, hud_y);
	draw_hud_number(window, hud, magnum_ammo, left + 80, row1, hud_x, hud_y);
	draw_hud_text(window, hud, "TIME", left + 88, row1, hud_x, hud_y);
	draw_hud_number(window, hud, time_left, left + 152, row1, hud_x, hud_y);
	draw_hud_text(window, hud, "P", left + 160, row1, hud_x, hud_y);
	draw_hud_number(window, hud, lives, left + 176, row1, hud_x, hud_y);
	draw_hud_piece(window, hud, 40, 16, 8, 8, left + 192, row1, hud_x, hud_y);
	draw_hud_number(window, hud, defeated, left + 216, row1, hud_x, hud_y);

	// row 2: the score, second ammo, the three weapon boxes, P + health bar
	draw_hud_number(window, hud, score, left + 48, row2, hud_x, hud_y);
	draw_hud_piece(window, hud, 32, 16, 8, 8, left + 56, row2, hud_x, hud_y);
	draw_hud_number(window, hud, grenade_ammo, left + 80, row2, hud_x, hud_y);

	for (int k = 0; k < 3; k += 1)
	{
		// the chosen weapon uses the orange box (48 pixels further right on the sheet)
		int tx = k * 16;
		if (weapon == k + 1)
		{
			tx += 48;
		}
		draw_hud_piece(window, hud, tx, 112, 16, 16, left + 88 + k * 16, 10, hud_x, hud_y);
	}

	draw_hud_text(window, hud, "P", left + 144, row2, hud_x, hud_y);

	// 20 bar segments: full ones are orange, the rest are empty outlines
	int filled = 0;
	if (health > 0)
	{
		filled = (health * 20 + max_health - 1) / max_health;
	}
	for (int i = 0; i < 20; i += 1)
	{
		int tx = 80;
		if (i < filled)
		{
			tx = 64;
		}
		draw_hud_piece(window, hud, tx, 16, 4, 8, left + 152 + i * 4, row2, hud_x, hud_y);
	}

	// row 3: STAGE and the (empty) enemy bar label
	draw_hud_text(window, hud, "STAGE", left, row3, hud_x, hud_y);
	// the stage number: 2-1, 2-2, 2-3 (as in the NES game)
	char stage[4];
	stage[0] = '2';
	stage[1] = '-';
	stage[2] = '0' + level;
	stage[3] = '\0';
	draw_hud_text(window, hud, stage, left + 56, row3, hud_x, hud_y);
	draw_hud_text(window, hud, "E", left + 144, row3, hud_x, hud_y);
}

// ---------------------------------------------------------------
// BREAKABLE VINES ('Y'): climbed like ladders. After the player has held on to a vine cell for 90 frames
// it breaks (the cell disappears) and it grows back 240 frames later.
// vine[row][col]: 0 = normal, 1..89 = about to break, negative = gone, counting up to 0
// ---------------------------------------------------------------
void update_vines(char** lvl, int** vine, const float player_x, const int Pwidth, const float player_y, const int Pheight, const bool climbing, const int height, const int width, const int cell_size)
{
	if (climbing)
	{
		int col = (int)(player_x + Pwidth / 2) / cell_size;
		int row = (int)(player_y + Pheight / 2) / cell_size;
		if (row >= 0 && row < height && col >= 0 && col < width && lvl[row][col] == 'Y' && vine[row][col] == 0)
		{
			vine[row][col] = 1;
		}
	}

	for (int r = 0; r < height; r += 1)
	{
		for (int c = 0; c < width; c += 1)
		{
			if (vine[r][c] > 0)
			{
				vine[r][c] += 1;
				if (vine[r][c] >= 90)
				{
					lvl[r][c] = ' ';
					vine[r][c] = -240;
				}
			}
			else if (vine[r][c] < 0)
			{
				vine[r][c] += 1;
				if (vine[r][c] == 0)
				{
					lvl[r][c] = 'Y';
				}
			}
		}
	}
}

// ---------------------------------------------------------------
// SWING VINES ('Z'): press Up next to one to grab it. Left / Right swing the player along an arc
// (up to 2 cells to each side), Space or Down lets go, and reaching the end of the arc lets go too.
// ---------------------------------------------------------------
void swing_vine(char** lvl, bool& swinging, float& anchor_x, float& base_y, float& vine_len, float& swing_dx, int& swing_frames, float& player_x, float& player_y, float& velocityY, bool& facingRight, const bool climbing, const int Pwidth, const int Pheight, const int height, const int width, const int cell_size)
{
	bool up = Keyboard::isKeyPressed(Keyboard::Up);
	bool down = Keyboard::isKeyPressed(Keyboard::Down);
	bool left = Keyboard::isKeyPressed(Keyboard::Left);
	bool right = Keyboard::isKeyPressed(Keyboard::Right);
	bool jump = Keyboard::isKeyPressed(Keyboard::Space);

	if (!swinging)
	{
		if (!up || climbing)
		{
			return;
		}

		// is there a swing vine at the player's body?
		float center_x = player_x + Pwidth / 2;
		int col = (int)center_x / cell_size;
		int found_row = -1;
		if (col >= 0 && col < width)
		{
			for (float y = player_y + 4; y < player_y + Pheight && found_row < 0; y += 16)
			{
				int row = (int)y / cell_size;
				if (row >= 0 && row < height && lvl[row][col] == 'Z')
				{
					found_row = row;
				}
			}
		}
		if (found_row < 0)
		{
			return;
		}

		// measure the vine: the player hangs with his feet at its lower end
		int r1 = found_row;
		while (r1 > 0 && lvl[r1 - 1][col] == 'Z')
		{
			r1 -= 1;
		}
		int r2 = found_row;
		while (r2 < height - 1 && lvl[r2 + 1][col] == 'Z')
		{
			r2 += 1;
		}

		anchor_x = col * cell_size + cell_size / 2;
		vine_len = (r2 + 1 - r1) * cell_size;
		base_y = (r2 + 1) * cell_size - Pheight;
		swing_dx = center_x - anchor_x;
		if (swing_dx > 128)
		{
			swing_dx = 128;
		}
		if (swing_dx < -128)
		{
			swing_dx = -128;
		}
		swing_frames = 0;
		swinging = true;
		velocityY = 0;
	}

	swing_frames += 1;

	// the arc is 128 pixels to each side (less on a very short vine)
	float limit = 128;
	if (vine_len * 0.9 < limit)
	{
		limit = vine_len * 0.9;
	}

	// let go (not in the first 20 frames, so the jump that grabbed the vine does not let go again)
	if ((jump || down) && swing_frames > 20)
	{
		swinging = false;
		velocityY = -8;
		return;
	}

	if (left)
	{
		swing_dx -= 4;
		facingRight = false;
	}
	else if (right)
	{
		swing_dx += 4;
		facingRight = true;
	}
	else if (swing_dx > 1)
	{
		swing_dx -= 1;
	}
	else if (swing_dx < -1)
	{
		swing_dx += 1;
	}

	if (swing_dx > limit)
	{
		swing_dx = limit;
	}
	if (swing_dx < -limit)
	{
		swing_dx = -limit;
	}

	// the player moves on a circle: he rises a little at both ends of the arc
	float rise = vine_len - sqrt(vine_len * vine_len - swing_dx * swing_dx);
	player_x = anchor_x + swing_dx - Pwidth / 2;
	player_y = base_y - rise;

	// the end of the arc: let go and drop onto the platform there
	if ((swing_dx >= limit || swing_dx <= -limit) && swing_frames > 5)
	{
		swinging = false;
		velocityY = -4;
	}
}

// ---------------------------------------------------------------
// PICKUPS: kind 0 ammo, 1 health, 2 damage boost, 3 time dilation, 4 double jump, 5 health regeneration
// ---------------------------------------------------------------

void add_pickup(int pk_kind[], int pk_screen[], float pk_x[], float pk_y[], bool pk_active[], int pk_timer[], const int max_pickups, const int kind, const int screen, const float x, const float y, const int timer)
{
	for (int i = 0; i < max_pickups; i += 1)
	{
		if (!pk_active[i])
		{
			pk_active[i] = true;
			pk_kind[i] = kind;
			pk_screen[i] = screen;
			pk_x[i] = x;
			pk_y[i] = y;
			pk_timer[i] = timer;
			return;
		}
	}
}

// the five pickups of a level: ammo, health, damage boost, time dilation, double jump
// (screen, column, row of the girder they lie on)
void place_level_pickups(const int level, int pk_kind[], int pk_screen[], float pk_x[], float pk_y[], bool pk_active[], int pk_timer[], const int max_pickups, const int cell_size)
{
	for (int i = 0; i < max_pickups; i += 1)
	{
		pk_active[i] = false;
	}

	int screen[5];
	int col[5];
	int row[5];

	if (level == 1)
	{
		int s[5] = { 0, 2, 1, 1, 3 };
		int c[5] = { 9, 9, 12, 8, 11 };
		int r[5] = { 11, 11, 8, 10, 5 };
		for (int k = 0; k < 5; k += 1)
		{
			screen[k] = s[k];
			col[k] = c[k];
			row[k] = r[k];
		}
	}
	else if (level == 2)
	{
		int s[5] = { 0, 2, 3, 1, 4 };
		int c[5] = { 14, 14, 12, 2, 10 };
		int r[5] = { 5, 10, 5, 11, 7 };
		for (int k = 0; k < 5; k += 1)
		{
			screen[k] = s[k];
			col[k] = c[k];
			row[k] = r[k];
		}
	}
	else
	{
		int s[5] = { 2, 1, 4, 3, 5 };
		int c[5] = { 9, 9, 12, 14, 5 };
		int r[5] = { 7, 4, 6, 11, 8 };
		for (int k = 0; k < 5; k += 1)
		{
			screen[k] = s[k];
			col[k] = c[k];
			row[k] = r[k];
		}
	}

	// kinds 0 to 4 in this order
	for (int k = 0; k < 5; k += 1)
	{
		add_pickup(pk_kind, pk_screen, pk_x, pk_y, pk_active, pk_timer, max_pickups, k, screen[k], col[k] * cell_size + 16, row[k] * cell_size - 32, -1);
	}
}

// finds a random place to stand on the current screen: a solid, non-crumbling cell with free space above it
bool find_standable_spot(char** lvl, long long& seed, const int height, const int width, const int cell_size, float& x, float& y)
{
	for (int attempt = 0; attempt < 40; attempt += 1)
	{
		int col = 2 + random_number(seed, width - 4);
		int candidates[14];
		int number = 0;
		for (int row = 2; row < height; row += 1)
		{
			if (is_solid_char(lvl[row][col]) && lvl[row][col] != 'C' && lvl[row - 1][col] == ' ' && lvl[row - 2][col] == ' ')
			{
				candidates[number] = row;
				number += 1;
			}
		}
		if (number > 0)
		{
			int row = candidates[random_number(seed, number)];
			x = col * cell_size + 16;
			y = row * cell_size - 32;
			return true;
		}
	}
	return false;
}

// timed pickups (the health regeneration) vanish when their timer runs out;
// touching a pickup on the current screen collects it
void update_pickups(const int current_screen, const float player_x, const float player_y, const int Pwidth, const int Pheight, int pk_kind[], int pk_screen[], float pk_x[], float pk_y[], bool pk_active[], int pk_timer[], const int max_pickups, int& magnum_ammo, int& grenade_ammo, int& health, const int max_health, int& extra_hp, int& boost_timer, int& slow_timer, bool& double_jump_on, int& shield_timer)
{
	for (int i = 0; i < max_pickups; i += 1)
	{
		if (!pk_active[i])
		{
			continue;
		}

		if (pk_timer[i] > 0)
		{
			pk_timer[i] -= 1;
			if (pk_timer[i] == 0)
			{
				pk_active[i] = false;
				continue;
			}
		}

		if (pk_screen[i] == current_screen && rects_overlap(player_x, player_y, Pwidth, Pheight, pk_x[i], pk_y[i], 32, 32))
		{
			pk_active[i] = false;
			int kind = pk_kind[i];

			if (kind == 0)
			{
				magnum_ammo = 10;
				grenade_ammo = 3;
			}
			else if (kind == 1)
			{
				health += 30;
				if (health > max_health)
				{
					health = max_health;
				}
			}
			else if (kind == 2)
			{
				boost_timer = 600;
			}
			else if (kind == 3)
			{
				slow_timer = 420;
			}
			else if (kind == 4)
			{
				double_jump_on = true;
			}
			else
			{
				// health regeneration: +100 health, the part that does not fit is saved for the next life,
				// and damage is reduced by 75% for 4 seconds
				int room = max_health - health;
				int heal = 100;
				if (heal > room)
				{
					heal = room;
				}
				health += heal;
				extra_hp += 100 - heal;
				if (extra_hp > 100)
				{
					extra_hp = 100;
				}
				shield_timer = 240;
			}
		}
	}
}

void draw_pickups(RenderWindow& window, Sprite pickupSprite[], const int current_screen, int pk_kind[], int pk_screen[], float pk_x[], float pk_y[], bool pk_active[], const int max_pickups)
{
	for (int i = 0; i < max_pickups; i += 1)
	{
		if (pk_active[i] && pk_screen[i] == current_screen)
		{
			int picture = pk_kind[i];
			if (picture == 5)
			{
				picture = 1;
			}
			pickupSprite[picture].setPosition(pk_x[i], pk_y[i]);
			window.draw(pickupSprite[picture]);
		}
	}
}

// ---------------------------------------------------------------
// LEVEL EXIT AND DAMAGE
// ---------------------------------------------------------------

// true if the player touches an exit cell ('E')
bool touching_exit(char** lvl, const float player_x, const float player_y, const int Pwidth, const int Pheight, const int height, const int width, const int cell_size)
{
	for (int r = 0; r < height; r += 1)
	{
		for (int c = 0; c < width; c += 1)
		{
			if (lvl[r][c] == 'E' && rects_overlap(player_x, player_y, Pwidth, Pheight, c * cell_size, r * cell_size, cell_size, cell_size))
			{
				return true;
			}
		}
	}
	return false;
}

// damage after the health regeneration shield (75% less while it lasts)
int reduced_damage(const int amount, const int shield_timer)
{
	if (shield_timer > 0)
	{
		int reduced = amount / 4;
		if (reduced < 1)
		{
			reduced = 1;
		}
		return reduced;
	}
	return amount;
}

// ---------------------------------------------------------------
// GAME OVER / YOU WIN screens, written with the HUD font
// ---------------------------------------------------------------

// draws a line of text in the middle of the window, y is in screen pixels
void draw_centered_text(RenderWindow& window, Sprite& hud, const char text[], const int y, const int screen_width)
{
	int length = 0;
	while (text[length] != '\0')
	{
		length += 1;
	}
	float x = (screen_width - length * 32) / 2;
	draw_hud_text(window, hud, text, 0, 0, x, y);
}

void draw_end_screen(RenderWindow& window, Sprite& hud, const int screen_width, const bool won, const int score)
{
	if (won)
	{
		draw_centered_text(window, hud, "YOU WIN", 280, screen_width);
	}
	else
	{
		draw_centered_text(window, hud, "GAME OVER", 280, screen_width);
	}

	char line[40];
	line[0] = '\0';
	append_text(line, "SCORE ");
	append_number(line, score);
	draw_centered_text(window, hud, line, 380, screen_width);
	draw_centered_text(window, hud, "PRESS R TO PLAY AGAIN", 480, screen_width);
	draw_centered_text(window, hud, "PRESS ESCAPE TO QUIT", 540, screen_width);
}

// ---------------------------------------------------------------
// SCREENS (flip-screen scrolling)
// ---------------------------------------------------------------

// if the player walks off the left or right edge, switch to the neighbouring screen.
// returns true when the screen changed so the caller can load the new layout.
bool check_screen_change(float& player_x, int& current_screen, const int total_screens, const int screen_width, const int Pwidth)
{
	if (Keyboard::isKeyPressed(Keyboard::Right) && player_x >= screen_width - Pwidth && current_screen < total_screens - 1)
	{
		current_screen += 1;
		player_x = 0;
		return true;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && player_x <= 0 && current_screen > 0)
	{
		current_screen -= 1;
		player_x = screen_width - Pwidth;
		return true;
	}
	return false;
}

// ---------------------------------------------------------------
// PLAYER BODY PARTS
// Quinn is drawn from 5 sprites that share one image, Data/player_parts.png:
// part 0 = back arm, 1 = legs, 2 = torso, 3 = head, 4 = front arm.
// Every frame is chosen with setTextureRect, so real sprite pieces can be used later
// by changing only the rectangles. The body box is 18 x 32 pixels, drawn at scale 3.
// ---------------------------------------------------------------

// draws one body part: (tx, ty, tw, th) is its frame on the image,
// (ox, oy) is where it sits inside the 18 x 32 body box (before scaling)
void show_part(RenderWindow& window, Sprite& part, const int tx, const int ty, const int tw, const int th, const int ox, const int oy, const float player_x, const float player_y, const bool facingRight, const float yscale)
{
	part.setTextureRect(IntRect(tx, ty, tw, th));

	float draw_y = player_y + oy * 3 * yscale;

	if (facingRight)
	{
		part.setScale(3, 3 * yscale);
		part.setPosition(player_x + ox * 3, draw_y);
	}
	else
	{
		// a negative scale mirrors the part, so its position is the mirrored left edge
		part.setScale(-3, 3 * yscale);
		part.setPosition(player_x + (18 - ox) * 3, draw_y);
	}

	window.draw(part);
}

// chooses the frame of every body part from what the player is doing, then draws them
void draw_player(RenderWindow& window, Sprite part[], const float player_x, const float player_y, const bool facingRight, const bool ducking, const bool climbing, const bool onGround, const bool moving, const bool climb_moving, const int anim_counter, const int whip_timer, const int shoot_timer, const bool hurt)
{
	// walk cycle: the legs and the arms swing in opposite directions
	const int leg_seq[4] = { 1, 0, 2, 0 };
	const int front_arm_seq[4] = { 2, 0, 1, 0 };
	const int back_arm_seq[4] = { 1, 0, 2, 0 };

	int leg_frame = 0;
	int front_arm = 0;
	int back_arm = 0;
	int torso_frame = 0;
	int bob = 0;
	float yscale = 1;

	int walk_step = (anim_counter / 6) % 4;
	int climb_step = (anim_counter / 8) % 2;

	if (climbing)
	{
		leg_frame = 5;
		front_arm = 3;
		back_arm = 3;
		if (climb_moving)
		{
			// arms and legs alternate while climbing
			leg_frame = 5 + climb_step;
			if (climb_step == 0)
			{
				back_arm = 0;
			}
			else
			{
				front_arm = 0;
			}
		}
	}
	else if (!onGround)
	{
		// jumping or falling: legs tucked, arms up
		leg_frame = 3;
		front_arm = 3;
		back_arm = 3;
	}
	else if (ducking)
	{
		// crouching: legs folded, body squashed to half height
		leg_frame = 4;
		torso_frame = 1;
		yscale = 0.5;
	}
	else if (moving)
	{
		leg_frame = leg_seq[walk_step];
		front_arm = front_arm_seq[walk_step];
		back_arm = back_arm_seq[walk_step];
		if (walk_step == 0 || walk_step == 2)
		{
			bob = 1;
		}
	}

	// the front arm points forward while attacking
	if ((whip_timer > 0 || shoot_timer > 0) && !climbing)
	{
		front_arm = 4;
	}

	int head_frame = 0;
	if (hurt)
	{
		head_frame = 1;
	}

	// drawing order: back arm, legs, torso, head, front arm
	show_part(window, part[0], back_arm * 10, 22, 10, 12, 0, 11 - bob, player_x, player_y, facingRight, yscale);
	show_part(window, part[1], leg_frame * 14, 34, 14, 10, 2, 22, player_x, player_y, facingRight, yscale);
	show_part(window, part[2], torso_frame * 10, 10, 10, 12, 4, 10 - bob, player_x, player_y, facingRight, yscale);
	show_part(window, part[3], head_frame * 10, 0, 10, 10, 5, 0 - bob, player_x, player_y, facingRight, yscale);
	show_part(window, part[4], front_arm * 10, 22, 10, 12, 8, 11 - bob, player_x, player_y, facingRight, yscale);
}

// ---------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------
int main()
{
	RenderWindow window(VideoMode(screen_x, screen_y), "VICE: Project Doom", Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	// level specifics
	const int cell_size = 64;
	const int height = 14;
	const int width = 18;
	char** lvl;

	// the level being played (1 to 3), its number of screens, and the screen the player is on
	int level = 1;
	int total_screens = 4;
	int current_screen = 0;

	// one background for every level
	Texture bgTexture[3];
	Sprite bgSprite;
	const char* bg_file[3] = { "Data/bg.png", "Data/bg_port.png", "Data/bg_jungle.png" };
	for (int i = 0; i < 3; i += 1)
	{
		if (!bgTexture[i].loadFromFile(bg_file[i]))
		{
			cout << "Could not load " << bg_file[i] << " (run the game from the folder that contains Data/)" << endl;
			return 1;
		}
	}
	bgSprite.setTexture(bgTexture[0]);
	bgSprite.setPosition(0, 0);

	// one picture for every kind of tile (see tile_number)
	const int num_tiles = 14;
	Texture tileTexture[num_tiles];
	Sprite tileSprite[num_tiles];
	const char* tile_file[num_tiles] = { "Data/block1.png", "Data/girder_h.png", "Data/girder_v.png", "Data/girder_crumble.png", "Data/container.png", "Data/mesh.png", "Data/rock.png", "Data/girder_hazard.png", "Data/ladder.png", "Data/exit.png", "Data/vine.png", "Data/vine_swing.png", "Data/water.png", "Data/waterfall.png" };
	for (int i = 0; i < num_tiles; i += 1)
	{
		if (!tileTexture[i].loadFromFile(tile_file[i]))
		{
			cout << "Could not load " << tile_file[i] << endl;
			return 1;
		}
		tileSprite[i].setTexture(tileTexture[i]);
	}

	// music initialisation
	Music lvlMusic;

	if (!lvlMusic.openFromFile("Data/mus.ogg"))
	{
		cout << "Could not load Data/mus.ogg (game will run without music)" << endl;
	}
	lvlMusic.setVolume(20);
	lvlMusic.play();
	lvlMusic.setLoop(true);

	// player data
	float player_x = 100;
	float player_y = 500;

	// last safe spot (standing on a girder): the player comes back here after falling into a pit
	float safe_x = 100;
	float safe_y = 608;
	int safe_screen = 0;

	const float gravity = 1;
	const float speed = 5;
	const float jump_speed = -18;
	const float climb_speed = 4;

	Texture PlayerTexture;
	Sprite PlayerSprite;

	bool onGround = false;
	bool facingRight = true;
	bool ducking = false;
	bool climbing = false;

	float velocityY = 0;
	float terminal_Velocity = 20;

	const int StandHeight = 96;
	const int DuckHeight = 48;
	int PlayerHeight = StandHeight;
	const int PlayerWidth = 54;

	if (!PlayerTexture.loadFromFile("Data/player.png"))
	{
		cout << "Could not load Data/player.png" << endl;
		return 1;
	}
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setScale(3, 3);
	PlayerSprite.setPosition(player_x, player_y);

	// double jump (a pickup) and swinging on a swing vine
	bool double_jump_on = false;
	bool used_double = false;
	bool jump_prev = false;
	bool swinging = false;
	float swing_anchor_x = 0;
	float swing_base_y = 0;
	float swing_len = 0;
	float swing_dx = 0;
	int swing_frames = 0;

	// weapons: 1 = laser whip, 2 = .44 magnum, 3 = grenades
	int weapon = 1;
	int magnum_ammo = 10;
	int grenade_ammo = 3;
	bool prev_fire = false;
	int whip_timer = 0;

	// health, and the effects of the power-ups (timers count frames, 60 per second)
	int health = 100;
	const int max_health = 100;
	int extra_hp = 0;
	int boost_timer = 0;
	int slow_timer = 0;
	int shield_timer = 0;

	const int max_bullets = 10;
	float bullet_x[max_bullets];
	float bullet_y[max_bullets];
	float bullet_dir[max_bullets];
	bool bullet_active[max_bullets];

	const int max_grenades = 3;
	float grenade_x[max_grenades];
	float grenade_y[max_grenades];
	float grenade_vx[max_grenades];
	float grenade_vy[max_grenades];
	bool grenade_active[max_grenades];

	for (int i = 0; i < max_bullets; i += 1)
	{
		bullet_active[i] = false;
	}
	for (int i = 0; i < max_grenades; i += 1)
	{
		grenade_active[i] = false;
	}

	Texture bulletTexture;
	Texture grenadeTexture;
	Texture whipTexture;
	Texture hudTexture;
	Texture hudBackTexture;
	Sprite bulletSprite;
	Sprite grenadeSprite;
	Sprite whipSprite;
	Sprite hud;
	Sprite hudBack;

	if (!bulletTexture.loadFromFile("Data/bullet.png") || !grenadeTexture.loadFromFile("Data/grenade.png") || !whipTexture.loadFromFile("Data/whip.png") || !hudTexture.loadFromFile("Data/hud_sheet.png") || !hudBackTexture.loadFromFile("Data/hud_bg.png"))
	{
		cout << "Could not load a weapon or HUD image from Data/ (bullet, grenade, whip, hud_sheet, hud_bg)" << endl;
		return 1;
	}
	bulletSprite.setTexture(bulletTexture);
	grenadeSprite.setTexture(grenadeTexture);
	whipSprite.setTexture(whipTexture);
	hud.setTexture(hudTexture);
	hudBack.setTexture(hudBackTexture);

	// the TIME counter of the HUD counts down once per second (60 frames)
	int time_left = 300;
	int time_frames = 0;
	int level_frames = 0;

	// enemies (the biggest level, level 3, has 40)
	const int max_enemies = 40;
	int en_type[max_enemies];
	int en_screen[max_enemies];
	float en_x[max_enemies];
	float en_y[max_enemies];
	float en_vx[max_enemies];
	float en_vy[max_enemies];
	int en_dir[max_enemies];
	int en_health[max_enemies];
	bool en_alive[max_enemies];
	bool en_ground[max_enemies];
	int en_timer[max_enemies];
	bool was_alive[max_enemies];
	int en_total = 0;

	// shots of the enemies
	const int max_shots = 12;
	float shot_x[max_shots];
	float shot_y[max_shots];
	float shot_vx[max_shots];
	int shot_kind[max_shots];
	bool shot_active[max_shots];
	clear_shots(shot_active, max_shots);

	// pickups and power-ups
	const int max_pickups = 16;
	int pk_kind[max_pickups];
	int pk_screen[max_pickups];
	float pk_x[max_pickups];
	float pk_y[max_pickups];
	bool pk_active[max_pickups];
	int pk_timer[max_pickups];
	for (int i = 0; i < max_pickups; i += 1)
	{
		pk_active[i] = false;
	}
	int regen_wait = 1500;

	int score = 0;
	int deaths = 0;
	int hurt_timer = 0;

	// 0 = playing, 1 = game over, 2 = you win
	int game_state = 0;
	bool new_level = true;

	Texture enemyTexture[7];
	Sprite enemySprite[7];
	const char* enemy_file[7] = { "Data/crawler.png", "Data/jumper.png", "Data/insect.png", "Data/leopard.png", "Data/shooter.png", "Data/tribal.png", "Data/fish.png" };
	for (int i = 0; i < 7; i += 1)
	{
		if (!enemyTexture[i].loadFromFile(enemy_file[i]))
		{
			cout << "Could not load " << enemy_file[i] << endl;
			return 1;
		}
		enemySprite[i].setTexture(enemyTexture[i]);
	}

	Texture pickupTexture[5];
	Sprite pickupSprite[5];
	const char* pickup_file[5] = { "Data/pickup_ammo.png", "Data/pickup_health.png", "Data/pickup_boost.png", "Data/pickup_time.png", "Data/pickup_doublejump.png" };
	for (int i = 0; i < 5; i += 1)
	{
		if (!pickupTexture[i].loadFromFile(pickup_file[i]))
		{
			cout << "Could not load " << pickup_file[i] << endl;
			return 1;
		}
		pickupSprite[i].setTexture(pickupTexture[i]);
	}

	Texture shotTexture[2];
	Sprite shotSprite[2];
	const char* shot_file[2] = { "Data/bolt.png", "Data/fireball.png" };
	for (int i = 0; i < 2; i += 1)
	{
		if (!shotTexture[i].loadFromFile(shot_file[i]))
		{
			cout << "Could not load " << shot_file[i] << endl;
			return 1;
		}
		shotSprite[i].setTexture(shotTexture[i]);
	}

	// player body parts (see draw_player)
	bool use_body_parts = true;
	Texture partsTexture;
	Sprite part[5];
	if (!partsTexture.loadFromFile("Data/player_parts.png"))
	{
		cout << "Could not load Data/player_parts.png" << endl;
		return 1;
	}
	for (int i = 0; i < 5; i += 1)
	{
		part[i].setTexture(partsTexture);
	}
	int anim_counter = 0;
	int shoot_timer = 0;
	bool moving = false;
	bool climb_moving = false;

	long long seed = time(0);

	// a temporary level array, only used by spawn_enemies to look for girders
	char** temp_lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		temp_lvl[i] = new char[width];
	}

	// creating level array
	lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width];
	}

	// timers of the crumbling girders and of the breakable vines
	int** crumble = new int* [height];
	int** vine = new int* [height];
	for (int i = 0; i < height; i += 1)
	{
		crumble[i] = new int[width];
		vine[i] = new int[width];
	}
	enter_screen(lvl, crumble, vine, level, current_screen, height, width);

	Event ev;
	// main loop
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}
		}

		// pressing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// the end screens: R starts a new game
		if (game_state != 0)
		{
			if (Keyboard::isKeyPressed(Keyboard::R))
			{
				level = 1;
				score = 0;
				deaths = 0;
				health = max_health;
				extra_hp = 0;
				game_state = 0;
				new_level = true;
			}

			window.clear();
			draw_end_screen(window, hud, screen_x, game_state == 2, score);
			window.display();
			continue;
		}

		// a new level starts: build everything that belongs to it
		if (new_level)
		{
			new_level = false;

			if (level == 1)
			{
				total_screens = 4;
			}
			else if (level == 2)
			{
				total_screens = 5;
			}
			else
			{
				total_screens = 6;
			}
			current_screen = 0;
			bgSprite.setTexture(bgTexture[level - 1]);

			en_total = spawn_enemies(level, en_type, en_screen, en_x, en_y, en_vx, en_vy, en_dir, en_health, en_alive, en_ground, en_timer, seed, max_enemies, total_screens, screen_x, temp_lvl, height, width, cell_size);
			for (int i = 0; i < max_enemies; i += 1)
			{
				was_alive[i] = en_alive[i];
			}

			clear_shots(shot_active, max_shots);
			for (int i = 0; i < max_bullets; i += 1)
			{
				bullet_active[i] = false;
			}
			for (int i = 0; i < max_grenades; i += 1)
			{
				grenade_active[i] = false;
			}
			place_level_pickups(level, pk_kind, pk_screen, pk_x, pk_y, pk_active, pk_timer, max_pickups, cell_size);
			regen_wait = 1500;

			// ammo is restocked at the start of every level and the power-ups end
			magnum_ammo = 10;
			grenade_ammo = 3;
			boost_timer = 0;
			slow_timer = 0;
			shield_timer = 0;
			double_jump_on = false;
			used_double = false;

			player_x = 100;
			player_y = 500;
			velocityY = 0;
			climbing = false;
			ducking = false;
			swinging = false;
			PlayerHeight = StandHeight;
			safe_x = 100;
			safe_y = 608;
			safe_screen = 0;
			hurt_timer = 0;

			time_left = 300;
			time_frames = 0;
			level_frames = 0;

			enter_screen(lvl, crumble, vine, level, current_screen, height, width);
		}

		level_frames += 1;
		anim_counter += 1;

		// remember where the player was, to know if he moved this frame
		float old_x = player_x;
		float old_y = player_y;
		bool was_ground = onGround;

		// swinging on a vine, or normal movement (no gravity while climbing a ladder or a vine)
		swing_vine(lvl, swinging, swing_anchor_x, swing_base_y, swing_len, swing_dx, swing_frames, player_x, player_y, velocityY, facingRight, climbing, PlayerWidth, PlayerHeight, height, width, cell_size);
		if (swinging)
		{
			onGround = false;
		}
		else
		{
			player_input(lvl, player_x, player_y, velocityY, onGround, facingRight, ducking, climbing, PlayerHeight, PlayerWidth, StandHeight, DuckHeight, speed, jump_speed, climb_speed, screen_x, cell_size, height, width);
			if (!climbing)
			{
				player_gravity(lvl, velocityY, onGround, gravity, terminal_Velocity, player_x, player_y, cell_size, height, width, PlayerHeight, PlayerWidth);
			}
		}

		// double jump: a new press of Space while already in the air
		bool jump_now = Keyboard::isKeyPressed(Keyboard::Space);
		if (onGround || climbing || swinging)
		{
			used_double = false;
		}
		if (jump_now && !jump_prev && !was_ground && !onGround && !climbing && !swinging && double_jump_on && !used_double)
		{
			velocityY = jump_speed;
			used_double = true;
		}
		jump_prev = jump_now;

		// crumbling girders and breakable vines
		update_crumbling(lvl, crumble, player_x, player_y, PlayerHeight, PlayerWidth, onGround, height, width, cell_size);
		update_vines(lvl, vine, player_x, PlayerWidth, player_y, PlayerHeight, climbing, height, width, cell_size);

		// a vine that broke under a climbing player makes him fall
		if (climbing && !is_ladder(lvl, player_x + PlayerWidth / 2, player_y + PlayerHeight / 2, height, width, cell_size) && !is_ladder(lvl, player_x + PlayerWidth / 2, player_y + PlayerHeight - 1, height, width, cell_size))
		{
			climbing = false;
		}

		// remember the last safe spot (standing on a girder that does not crumble)
		if (onGround && !climbing && !swinging && !ducking && player_y + PlayerHeight < height * cell_size - 1)
		{
			char under = lvl[(int)(player_y + PlayerHeight + 1) / cell_size][(int)(player_x + PlayerWidth / 2) / cell_size];
			if (is_solid_char(under) && under != 'C')
			{
				safe_x = player_x;
				safe_y = player_y;
				safe_screen = current_screen;
			}
		}

		// falling into a pit or into the water costs 20 health and puts the player back on the last safe spot
		if (player_y + PlayerHeight >= height * cell_size - 1)
		{
			health -= reduced_damage(20, shield_timer);
			hurt_timer = 60;
			current_screen = safe_screen;
			enter_screen(lvl, crumble, vine, level, current_screen, height, width);
			clear_shots(shot_active, max_shots);
			player_x = safe_x;
			player_y = safe_y;
			velocityY = 0;
			climbing = false;
			swinging = false;
			ducking = false;
			PlayerHeight = StandHeight;
		}

		// animation state
		moving = (player_x != old_x) && !climbing && !swinging;
		climb_moving = (climbing && player_y != old_y) || swinging;

		// weapons
		switch_weapon(weapon);
		int old_magnum = magnum_ammo;
		int old_grenade = grenade_ammo;
		fire_weapon(weapon, prev_fire, magnum_ammo, grenade_ammo, whip_timer, bullet_x, bullet_y, bullet_dir, bullet_active, max_bullets, grenade_x, grenade_y, grenade_vx, grenade_vy, grenade_active, max_grenades, player_x, player_y, facingRight, PlayerWidth, PlayerHeight);
		if (whip_timer > 0)
		{
			whip_timer -= 1;
		}

		// a magnum shot or a grenade throw makes the front arm point forward for 10 frames
		if (magnum_ammo < old_magnum || grenade_ammo < old_grenade)
		{
			shoot_timer = 10;
		}
		if (shoot_timer > 0)
		{
			shoot_timer -= 1;
		}

		// time dilation: enemies, their shots and the player's bullets run at half speed
		bool slow_frame = slow_timer > 0 && anim_counter % 2 == 1;
		if (!slow_frame)
		{
			update_bullets(lvl, bullet_x, bullet_y, bullet_dir, bullet_active, max_bullets, 14, screen_x, height, width, cell_size);
			update_grenades(lvl, grenade_x, grenade_y, grenade_vx, grenade_vy, grenade_active, max_grenades, 0.8, screen_x, height, width, cell_size, current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, score);

			update_enemies(lvl, current_screen, player_x, player_y, PlayerHeight, en_type, en_screen, en_x, en_y, en_vx, en_vy, en_dir, en_alive, en_ground, en_timer, seed, max_enemies, 0.8, screen_x, height, width, cell_size);
			enemy_fire(current_screen, player_x, en_type, en_screen, en_x, en_y, en_vx, en_vy, en_alive, en_ground, en_timer, max_enemies, shot_x, shot_y, shot_vx, shot_kind, shot_active, max_shots);
			update_enemy_shots(lvl, shot_x, shot_y, shot_vx, shot_kind, shot_active, max_shots, screen_x, height, width, cell_size);
		}

		// the weapons damage the enemies (the damage boost doubles the magnum)
		int bullet_damage = 10;
		if (boost_timer > 0)
		{
			bullet_damage = 20;
		}
		for (int i = 0; i < max_bullets; i += 1)
		{
			if (bullet_active[i] && hit_enemies(current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, bullet_x[i], bullet_y[i], 16, 8, bullet_damage, score))
			{
				bullet_active[i] = false;
			}
		}

		if (whip_timer > 0)
		{
			float whip_x = player_x - 64;
			if (facingRight)
			{
				whip_x = player_x + PlayerWidth;
			}
			hit_enemies(current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, whip_x, player_y + 40, 64, 16, 10, score);
		}

		// a crawler that was killed (not lost in a pit) may drop ammo or a damage boost
		for (int i = 0; i < max_enemies; i += 1)
		{
			if (was_alive[i] && !en_alive[i] && en_type[i] == 0 && en_y[i] + 32 < height * cell_size - 8)
			{
				int kind = 0;
				if (random_number(seed, 2) == 0)
				{
					kind = 2;
				}
				add_pickup(pk_kind, pk_screen, pk_x, pk_y, pk_active, pk_timer, max_pickups, kind, en_screen[i], en_x[i] + 8, en_y[i], -1);
			}
			was_alive[i] = en_alive[i];
		}

		// touching an enemy or being hit by a shot hurts, then the player is protected for 60 frames
		if (hurt_timer > 0)
		{
			hurt_timer -= 1;
		}
		else
		{
			int hit = touching_damage(current_screen, en_type, en_screen, en_x, en_y, en_alive, max_enemies, player_x, player_y, PlayerWidth, PlayerHeight);
			for (int i = 0; i < max_shots; i += 1)
			{
				if (shot_active[i])
				{
					int sw = 24;
					int sh = 8;
					if (shot_kind[i] == 1)
					{
						sw = 16;
						sh = 16;
					}
					if (rects_overlap(player_x, player_y, PlayerWidth, PlayerHeight, shot_x[i], shot_y[i], sw, sh))
					{
						shot_active[i] = false;
						if (hit < 10)
						{
							hit = 10;
						}
					}
				}
			}
			if (hit > 0)
			{
				health -= reduced_damage(hit, shield_timer);
				hurt_timer = 60;
			}
		}

		// pickups and power-ups
		update_pickups(current_screen, player_x, player_y, PlayerWidth, PlayerHeight, pk_kind, pk_screen, pk_x, pk_y, pk_active, pk_timer, max_pickups, magnum_ammo, grenade_ammo, health, max_health, extra_hp, boost_timer, slow_timer, double_jump_on, shield_timer);

		// the health regeneration power-up appears every 25 seconds on a random girder of the current screen for 6 seconds
		regen_wait -= 1;
		if (regen_wait <= 0)
		{
			float spot_x = 0;
			float spot_y = 0;
			if (find_standable_spot(lvl, seed, height, width, cell_size, spot_x, spot_y))
			{
				add_pickup(pk_kind, pk_screen, pk_x, pk_y, pk_active, pk_timer, max_pickups, 5, current_screen, spot_x, spot_y, 360);
			}
			regen_wait = 1500;
		}

		if (boost_timer > 0)
		{
			boost_timer -= 1;
		}
		if (slow_timer > 0)
		{
			slow_timer -= 1;
		}
		if (shield_timer > 0)
		{
			shield_timer -= 1;
		}

		// the TIME counter: 1 per second, running out of time is death
		time_frames += 1;
		if (time_frames >= 60)
		{
			time_frames = 0;
			if (time_left > 0)
			{
				time_left -= 1;
			}
		}
		if (time_left <= 0)
		{
			health = 0;
		}

		// dying: the third death is game over, otherwise start the level again
		if (health <= 0)
		{
			deaths += 1;
			if (deaths >= 3)
			{
				game_state = 1;
			}
			else
			{
				// health saved by the regeneration power-up is added to the next life
				health = max_health + extra_hp;
				extra_hp = 0;
				time_left = 300;
				time_frames = 0;
				player_x = 100;
				player_y = 500;
				velocityY = 0;
				climbing = false;
				swinging = false;
				ducking = false;
				PlayerHeight = StandHeight;
				current_screen = 0;
				safe_x = 100;
				safe_y = 608;
				safe_screen = 0;
				hurt_timer = 60;
				enter_screen(lvl, crumble, vine, level, current_screen, height, width);
				clear_shots(shot_active, max_shots);
			}
		}

		// reaching the exit finishes the level: bonuses, then the next level or the win screen
		if (game_state == 0 && touching_exit(lvl, player_x, player_y, PlayerWidth, PlayerHeight, height, width, cell_size))
		{
			if (level == 1)
			{
				score += 1000;

				// speed bonus of level 1
				int seconds = level_frames / 60;
				if (seconds < 30)
				{
					score += 1200;
				}
				else if (seconds < 45)
				{
					score += 1000;
				}
				else if (seconds < 60)
				{
					score += 500;
				}
			}
			else if (level == 2)
			{
				score += 1500;
			}
			else
			{
				score += 2000;
				// all three levels without dying
				if (deaths == 0)
				{
					score += 5000;
				}
			}

			level += 1;
			if (level > 3)
			{
				game_state = 2;
			}
			else
			{
				new_level = true;
			}
		}
		else if (game_state == 0 && check_screen_change(player_x, current_screen, total_screens, screen_x, PlayerWidth))
		{
			// switch screen when the player walks off an edge
			enter_screen(lvl, crumble, vine, level, current_screen, height, width);
			clear_shots(shot_active, max_shots);
		}

		// temporary ducking look: squash the sprite to half height (replace with a real duck frame later)
		float scale_y = 3;
		if (ducking)
		{
			scale_y = 1.5;
		}

		// face left or right (a negative scale mirrors the sprite, so shift it back by its width)
		if (facingRight)
		{
			PlayerSprite.setScale(3, scale_y);
			PlayerSprite.setPosition(player_x, player_y);
		}
		else
		{
			PlayerSprite.setScale(-3, scale_y);
			PlayerSprite.setPosition(player_x + PlayerWidth, player_y);
		}

		// draw
		window.clear();
		display_level(window, lvl, bgSprite, tileSprite, height, width, cell_size);
		draw_pickups(window, pickupSprite, current_screen, pk_kind, pk_screen, pk_x, pk_y, pk_active, max_pickups);
		draw_enemies(window, enemySprite, current_screen, en_type, en_screen, en_x, en_y, en_dir, en_alive, max_enemies);
		draw_enemy_shots(window, shotSprite, shot_x, shot_y, shot_vx, shot_kind, shot_active, max_shots);

		// blink while protected after a hit
		if (hurt_timer % 12 < 6)
		{
			if (use_body_parts)
			{
				draw_player(window, part, player_x, player_y, facingRight, ducking, climbing || swinging, onGround, moving, climb_moving, anim_counter, whip_timer, shoot_timer, hurt_timer > 0);
			}
			else
			{
				window.draw(PlayerSprite);
			}
		}

		// whip: a short-lived sprite in front of the player
		if (whip_timer > 0)
		{
			if (facingRight)
			{
				whipSprite.setScale(1, 1);
				whipSprite.setPosition(player_x + PlayerWidth, player_y + 40);
			}
			else
			{
				whipSprite.setScale(-1, 1);
				whipSprite.setPosition(player_x, player_y + 40);
			}
			window.draw(whipSprite);
		}

		draw_projectiles(window, bulletSprite, grenadeSprite, bullet_x, bullet_y, bullet_active, max_bullets, grenade_x, grenade_y, grenade_active, max_grenades);

		// lives left (3 at the start) and how many enemies of this level are defeated
		int lives = 3 - deaths;
		if (lives < 0)
		{
			lives = 0;
		}
		int defeated = 0;
		for (int i = 0; i < en_total; i += 1)
		{
			if (!en_alive[i])
			{
				defeated += 1;
			}
		}
		draw_hud(window, hud, hudBack, screen_x, screen_y, weapon, magnum_ammo, grenade_ammo, health, max_health, time_left, lives, defeated, score, level);
		window.display();
	}

	// stopping music and deleting the arrays
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
		delete[] temp_lvl[i];
		delete[] crumble[i];
		delete[] vine[i];
	}
	delete[] lvl;
	delete[] temp_lvl;
	delete[] crumble;
	delete[] vine;

	return 0;
}