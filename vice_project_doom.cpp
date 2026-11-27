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

// puts the tile character ch in column `col` from row r1 (top) down to row r2
// (ladders use 'H'; the top ladder cell should be level with the platform it leads to)
void place_column(char** lvl, const int col, const int r1, const int r2, const char ch)
{
	for (int i = r1; i <= r2; i += 1)
	{
		lvl[i][col] = ch;
	}
}

// builds one screen of level 1: girders floating over pits, as in the game video.
// every screen has girders at the left and right edges (row 11) so crossing between screens is always safe.
// tiles: G girder, C crumbling girder, S hazard girder, P decoration pillar, H ladder, E exit
void load_screen(char** lvl, const int screen, const int height, const int width)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			lvl[i][j] = ' ';
		}
	}

	if (screen == 0)
	{
		// start platform, a second girder over a pit, then the edge girder
		place_platform(lvl, 11, 0, 5, 'G');
		place_column(lvl, 1, 12, 13, 'P');
		place_column(lvl, 4, 12, 13, 'P');
		place_platform(lvl, 11, 8, 11, 'G');
		place_platform(lvl, 12, 9, 10, 'S');
		place_platform(lvl, 11, 14, 17, 'G');
	}
	else if (screen == 1)
	{
		// stepping girders over pits
		place_platform(lvl, 11, 0, 3, 'G');
		place_platform(lvl, 10, 6, 9, 'G');
		place_column(lvl, 7, 11, 13, 'P');
		place_column(lvl, 9, 11, 13, 'P');
		place_platform(lvl, 8, 11, 14, 'G');
		place_platform(lvl, 11, 15, 17, 'G');
	}
	else if (screen == 2)
	{
		// a bridge of crumbling girders
		place_platform(lvl, 11, 0, 2, 'G');
		place_platform(lvl, 11, 3, 6, 'C');
		place_platform(lvl, 11, 7, 10, 'G');
		place_column(lvl, 8, 12, 13, 'P');
		place_platform(lvl, 9, 12, 14, 'G');
		place_platform(lvl, 11, 15, 17, 'G');
	}
	else if (screen == 3)
	{
		// a long girder, then a ladder up to the exit
		place_platform(lvl, 11, 0, 9, 'G');
		place_column(lvl, 2, 12, 13, 'P');
		place_column(lvl, 6, 12, 13, 'P');
		place_column(lvl, 8, 5, 10, 'H');
		place_platform(lvl, 5, 9, 14, 'G');
		lvl[4][13] = 'E';
	}
}

// loads a screen and clears the crumbling-girder timers
void enter_screen(char** lvl, int** crumble, const int screen, const int height, const int width)
{
	load_screen(lvl, screen, height, width);
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			crumble[i][j] = 0;
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
	return lvl[(int)y / cell_size][(int)x / cell_size] == 'H';
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
// type 0 = spiked crawler, type 1 = blue jumper, type 2 = flying insect
// ---------------------------------------------------------------

int enemy_width(const int type)
{
	if (type == 0)
	{
		return 48;
	}
	if (type == 2)
	{
		return 78;
	}
	return 40;
}

int enemy_height(const int type)
{
	if (type == 0)
	{
		return 32;
	}
	if (type == 2)
	{
		return 48;
	}
	return 40;
}

int enemy_points(const int type)
{
	if (type == 0)
	{
		return 50;
	}
	return 75;
}

// true if two rectangles overlap
bool rects_overlap(const float ax, const float ay, const float aw, const float ah, const float bx, const float by, const float bw, const float bh)
{
	return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

// creates the level 1 enemies: 4 crawlers (indexes 0-3) then 5 blue jumpers (indexes 4-8).
// each gets a random screen and a random spot on the floor of that screen.
void spawn_enemies(int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], int en_dir[], int en_health[], bool en_alive[], bool en_ground[], int en_timer[], long long& seed, const int max_enemies, const int num_crawlers, const int num_jumpers, const int total_screens, const int screen_width, char** temp_lvl, const int height, const int width, const int cell_size)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (i < num_crawlers)
		{
			en_type[i] = 0;
		}
		else if (i < num_crawlers + num_jumpers)
		{
			en_type[i] = 1;
		}
		else
		{
			en_type[i] = 2;
		}

		// insects get a random screen and x; crawlers and jumpers are put on a random girder
		en_screen[i] = random_number(seed, total_screens);
		en_x[i] = 200 + random_number(seed, screen_width - 400);
		en_y[i] = 0;

		if (en_type[i] != 2)
		{
			int w = enemy_width(en_type[i]);
			int h = enemy_height(en_type[i]);
			bool found = false;

			for (int attempt = 0; attempt < 80 && !found; attempt += 1)
			{
				int screen = random_number(seed, total_screens);
				int col = 2 + random_number(seed, width - 4);

				// not on the start platform, so nothing spawns on top of the player
				if (screen == 0 && col < 6)
				{
					continue;
				}

				load_screen(temp_lvl, screen, height, width);

				// the first solid, non-crumbling cell in this column with free space above it
				for (int row = 2; row < height && !found; row += 1)
				{
					if (is_solid_char(temp_lvl[row][col]) && temp_lvl[row][col] != 'C' && temp_lvl[row - 1][col] == ' ' && temp_lvl[row - 2][col] == ' ')
					{
						found = true;
						en_screen[i] = screen;
						en_x[i] = col * cell_size + (cell_size - w) / 2;
						en_y[i] = row * cell_size - h;
					}
				}
			}

			if (!found)
			{
				// fallback: the long girder of screen 3
				en_screen[i] = 3;
				en_x[i] = 5 * cell_size;
				en_y[i] = 11 * cell_size - h;
			}
		}
		en_vx[i] = 0;
		en_vy[i] = 0;
		if (random_number(seed, 2) == 0)
		{
			en_dir[i] = 1;
		}
		else
		{
			en_dir[i] = -1;
		}
		en_health[i] = 10;
		en_alive[i] = true;
		en_ground[i] = true;
		en_timer[i] = 40 + random_number(seed, 40);

		if (en_type[i] == 2)
		{
			// a flying insect hovers high: en_vy keeps its base height, en_timer is its bobbing phase
			en_vy[i] = 380 + random_number(seed, 180);
			en_y[i] = en_vy[i];
			en_timer[i] = random_number(seed, 90);
			en_ground[i] = false;
		}
	}
}

// moves the enemies that are on the current screen
// crawler: walks along the floor and turns at walls and ledges
// blue jumper: waits on the ground, then hops toward the player
void update_enemies(char** lvl, const int current_screen, const float player_x, int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], int en_dir[], bool en_alive[], bool en_ground[], int en_timer[], long long& seed, const int max_enemies, const float gravity, const int screen_width, const int height, const int width, const int cell_size)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (!en_alive[i] || en_screen[i] != current_screen)
		{
			continue;
		}

		int w = enemy_width(en_type[i]);
		int h = enemy_height(en_type[i]);

		if (en_type[i] == 0)
		{
			// ---- spiked crawler ----
			// if the girder under it crumbled, it falls into the pit
			bool supported = is_solid(lvl, en_x[i] + 4, en_y[i] + h + 1, height, width, cell_size) || is_solid(lvl, en_x[i] + w - 5, en_y[i] + h + 1, height, width, cell_size);
			if (!supported)
			{
				en_y[i] += 6;
				if (en_y[i] + h >= height * cell_size - 1)
				{
					en_alive[i] = false;
				}
				continue;
			}

			float new_x = en_x[i] + en_dir[i] * 2;

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
				en_dir[i] = -en_dir[i];
			}
			else
			{
				en_x[i] = new_x;
			}
		}
		else if (en_type[i] == 1)
		{
			// ---- blue jumper ----

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

			// a jumper that falls into a pit is gone
			if (en_y[i] + h >= height * cell_size - 1)
			{
				en_alive[i] = false;
			}

			// on the ground: count down, then jump toward the player
			if (en_ground[i])
			{
				en_timer[i] -= 1;
				if (en_timer[i] <= 0)
				{
					if (player_x > en_x[i])
					{
						en_dir[i] = 1;
					}
					else
					{
						en_dir[i] = -1;
					}
					en_vx[i] = en_dir[i] * 3.5;
					en_vy[i] = -13;
					en_ground[i] = false;
					en_timer[i] = 40 + random_number(seed, 40);
				}
			}
		}
		else
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

void draw_enemies(RenderWindow& window, Sprite enemySprite[], const int current_screen, int en_type[], int en_screen[], float en_x[], float en_y[], int en_dir[], bool en_alive[], const int max_enemies)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (en_alive[i] && en_screen[i] == current_screen)
		{
			int t = en_type[i];

			// the crawler and the jumper face right, the flying insect picture faces left
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
void draw_hud(RenderWindow& window, Sprite& hud, Sprite& hudBack, const int screen_width, const int screen_height, const int weapon, const int magnum_ammo, const int grenade_ammo, const int health, const int max_health, const int time_left, const int lives, const int defeated, const int score)
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
	draw_hud_text(window, hud, "2-1", left + 56, row3, hud_x, hud_y);
	draw_hud_text(window, hud, "E", left + 144, row3, hud_x, hud_y);
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

	// flip-screen scrolling
	const int total_screens = 4;
	int current_screen = 0;

	// level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;

	// one picture for every kind of tile (see tile_number)
	const int num_tiles = 14;
	Texture tileTexture[num_tiles];
	Sprite tileSprite[num_tiles];
	const char* tile_file[num_tiles] = { "Data/block1.png", "Data/girder_h.png", "Data/girder_v.png", "Data/girder_crumble.png", "Data/container.png", "Data/mesh.png", "Data/rock.png", "Data/girder_hazard.png", "Data/ladder.png", "Data/exit.png", "Data/vine.png", "Data/vine_swing.png", "Data/water.png", "Data/waterfall.png" };

	if (!bgTex.loadFromFile("Data/bg.png"))
	{
		cout << "Could not load Data/bg.png (run the game from the folder that contains Data/)" << endl;
		return 1;
	}
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

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

	// weapons: 1 = laser whip, 2 = .44 magnum, 3 = grenades
	int weapon = 1;
	int magnum_ammo = 10;
	int grenade_ammo = 3;
	bool prev_fire = false;
	int whip_timer = 0;

	int health = 100;
	const int max_health = 100;

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

	// enemies (level 1: 4 crawlers + 5 blue jumpers + 6 flying insects)
	const int num_crawlers = 4;
	const int num_jumpers = 5;
	const int num_insects = 6;
	const int max_enemies = num_crawlers + num_jumpers + num_insects;
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

	int score = 0;
	int deaths = 0;
	int hurt_timer = 0;

	Texture enemyTexture[3];
	Sprite enemySprite[3];
	const char* enemy_file[3] = { "Data/crawler.png", "Data/jumper.png", "Data/insect.png" };
	for (int i = 0; i < 3; i += 1)
	{
		if (!enemyTexture[i].loadFromFile(enemy_file[i]))
		{
			cout << "Could not load " << enemy_file[i] << endl;
			return 1;
		}
		enemySprite[i].setTexture(enemyTexture[i]);
	}

	long long seed = time(0);
	// a temporary level array, only used by spawn_enemies to look for girders
	char** temp_lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		temp_lvl[i] = new char[width];
	}
	spawn_enemies(en_type, en_screen, en_x, en_y, en_vx, en_vy, en_dir, en_health, en_alive, en_ground, en_timer, seed, max_enemies, num_crawlers, num_jumpers, total_screens, screen_x, temp_lvl, height, width, cell_size);
	for (int i = 0; i < height; i += 1)
	{
		delete[] temp_lvl[i];
	}
	delete[] temp_lvl;

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

	// creating level array
	lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width];
	}

	// timers of the crumbling girders
	int** crumble = new int* [height];
	for (int i = 0; i < height; i += 1)
	{
		crumble[i] = new int[width];
	}
	enter_screen(lvl, crumble, current_screen, height, width);

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

		// remember where the player was, to know if he moved this frame
		float old_x = player_x;
		float old_y = player_y;

		// update (no gravity while climbing a ladder)
		player_input(lvl, player_x, player_y, velocityY, onGround, facingRight, ducking, climbing, PlayerHeight, PlayerWidth, StandHeight, DuckHeight, speed, jump_speed, climb_speed, screen_x, cell_size, height, width);
		if (!climbing)
		{
			player_gravity(lvl, velocityY, onGround, gravity, terminal_Velocity, player_x, player_y, cell_size, height, width, PlayerHeight, PlayerWidth);
		}

		// crumbling girders
		update_crumbling(lvl, crumble, player_x, player_y, PlayerHeight, PlayerWidth, onGround, height, width, cell_size);

		// remember the last safe spot (standing on a girder that does not crumble)
		if (onGround && !climbing && !ducking && player_y + PlayerHeight < height * cell_size - 1)
		{
			char under = lvl[(int)(player_y + PlayerHeight + 1) / cell_size][(int)(player_x + PlayerWidth / 2) / cell_size];
			if (is_solid_char(under) && under != 'C')
			{
				safe_x = player_x;
				safe_y = player_y;
				safe_screen = current_screen;
			}
		}

		// falling into a pit costs 20 health and puts the player back on the last safe spot
		if (player_y + PlayerHeight >= height * cell_size - 1)
		{
			health -= 20;
			hurt_timer = 60;
			current_screen = safe_screen;
			enter_screen(lvl, crumble, current_screen, height, width);
			player_x = safe_x;
			player_y = safe_y;
			velocityY = 0;
			climbing = false;
			ducking = false;
			PlayerHeight = StandHeight;
		}

		// animation state
		moving = (player_x != old_x) && !climbing;
		climb_moving = climbing && (player_y != old_y);
		anim_counter += 1;

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
		update_bullets(lvl, bullet_x, bullet_y, bullet_dir, bullet_active, max_bullets, 14, screen_x, height, width, cell_size);
		update_grenades(lvl, grenade_x, grenade_y, grenade_vx, grenade_vy, grenade_active, max_grenades, 0.8, screen_x, height, width, cell_size, current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, score);

		// enemies move, then weapons damage them
		update_enemies(lvl, current_screen, player_x, en_type, en_screen, en_x, en_y, en_vx, en_vy, en_dir, en_alive, en_ground, en_timer, seed, max_enemies, 0.8, screen_x, height, width, cell_size);

		for (int i = 0; i < max_bullets; i += 1)
		{
			if (bullet_active[i] && hit_enemies(current_screen, en_type, en_screen, en_x, en_y, en_health, en_alive, max_enemies, bullet_x[i], bullet_y[i], 16, 8, 10, score))
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

		// touching an enemy hurts, then the player is protected for 60 frames
		if (hurt_timer > 0)
		{
			hurt_timer -= 1;
		}
		else if (touching_enemy(current_screen, en_type, en_screen, en_x, en_y, en_alive, max_enemies, player_x, player_y, PlayerWidth, PlayerHeight))
		{
			health -= 10;
			hurt_timer = 60;
		}

		// TEMPORARY test keys for the health bar: Q = lose health, E = gain health
		if (Keyboard::isKeyPressed(Keyboard::Q) && health > 0)
		{
			health -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::E) && health < max_health)
		{
			health += 1;
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

		// dying: respawn at the start of the level (temporary, a proper game over comes later)
		if (health <= 0)
		{
			deaths += 1;
			health = max_health;
			time_left = 300;
			time_frames = 0;
			player_x = 100;
			player_y = 500;
			velocityY = 0;
			climbing = false;
			ducking = false;
			PlayerHeight = StandHeight;
			current_screen = 0;
			enter_screen(lvl, crumble, current_screen, height, width);
		}

		// switch screen when the player walks off an edge
		if (check_screen_change(player_x, current_screen, total_screens, screen_x, PlayerWidth))
		{
			enter_screen(lvl, crumble, current_screen, height, width);
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
		draw_enemies(window, enemySprite, current_screen, en_type, en_screen, en_x, en_y, en_dir, en_alive, max_enemies);

		// blink while protected after a hit
		if (hurt_timer % 12 < 6)
		{
			if (use_body_parts)
			{
				draw_player(window, part, player_x, player_y, facingRight, ducking, climbing, onGround, moving, climb_moving, anim_counter, whip_timer, shoot_timer, hurt_timer > 0);
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
		// lives left (3 at the start) and how many enemies are defeated
		int lives = 3 - deaths;
		if (lives < 0)
		{
			lives = 0;
		}
		int defeated = 0;
		for (int i = 0; i < max_enemies; i += 1)
		{
			if (!en_alive[i])
			{
				defeated += 1;
			}
		}
		draw_hud(window, hud, hudBack, screen_x, screen_y, weapon, magnum_ammo, grenade_ammo, health, max_health, time_left, lives, defeated, score);
		window.display();
	}

	// stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	for (int i = 0; i < height; i++)
	{
		delete[] crumble[i];
	}
	delete[] crumble;

	return 0;
}