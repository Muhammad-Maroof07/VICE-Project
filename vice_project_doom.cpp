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

// puts solid blocks in row `row` from column c1 to column c2
void place_platform(char** lvl, const int row, const int c1, const int c2)
{
	for (int j = c1; j <= c2; j += 1)
	{
		lvl[row][j] = '#';
	}
}

// puts a ladder in column `col` from row r1 (top) down to row r2
// the top ladder cell should be level with the platform it leads to
void place_ladder(char** lvl, const int col, const int r1, const int r2)
{
	for (int i = r1; i <= r2; i += 1)
	{
		lvl[i][col] = 'H';
	}
}

// builds one screen of the level (temporary layouts, real ones come later)
// keep the left and right edges of every screen open so the player can walk across
void load_screen(char** lvl, const int screen, const int height, const int width)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			lvl[i][j] = ' ';
		}
	}

	// floor
	place_platform(lvl, 13, 0, width - 1);

	if (screen == 0)
	{
		place_platform(lvl, 10, 3, 6);
		place_ladder(lvl, 2, 10, 12);
		place_platform(lvl, 8, 9, 13);
		place_ladder(lvl, 8, 8, 12);
	}
	else if (screen == 1)
	{
		place_platform(lvl, 10, 2, 5);
		place_ladder(lvl, 1, 10, 12);
		place_platform(lvl, 7, 7, 10);
		place_ladder(lvl, 6, 7, 12);
		place_platform(lvl, 10, 12, 15);
		place_ladder(lvl, 11, 10, 12);
	}
	else if (screen == 2)
	{
		place_platform(lvl, 11, 4, 7);
		place_platform(lvl, 9, 9, 12);
		place_ladder(lvl, 13, 9, 12);
	}
	else if (screen == 3)
	{
		place_platform(lvl, 10, 1, 4);
		place_ladder(lvl, 5, 10, 12);
		place_platform(lvl, 10, 6, 9);
		place_platform(lvl, 8, 12, 16);
		place_ladder(lvl, 11, 8, 12);
	}
}

void display_level(RenderWindow& window, char** lvl, Texture& bgTex, Sprite& bgSprite, Texture& blockTexture, Sprite& blockSprite, Texture& ladderTexture, Sprite& ladderSprite, const int height, const int width, const int cell_size)
{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == '#')
			{
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			}
			else if (lvl[i][j] == 'H')
			{
				ladderSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(ladderSprite);
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
	return lvl[(int)y / cell_size][(int)x / cell_size] == '#';
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
// ENEMIES
// every enemy lives in a set of parallel arrays (index i = one enemy)
// type 0 = spiked crawler, type 1 = blue jumper
// ---------------------------------------------------------------

int enemy_width(const int type)
{
	if (type == 0)
	{
		return 48;
	}
	return 40;
}

int enemy_height(const int type)
{
	if (type == 0)
	{
		return 32;
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
void spawn_enemies(int en_type[], int en_screen[], float en_x[], float en_y[], float en_vx[], float en_vy[], int en_dir[], int en_health[], bool en_alive[], bool en_ground[], int en_timer[], long long& seed, const int max_enemies, const int num_crawlers, const int total_screens, const int screen_width, const int floor_y)
{
	for (int i = 0; i < max_enemies; i += 1)
	{
		if (i < num_crawlers)
		{
			en_type[i] = 0;
		}
		else
		{
			en_type[i] = 1;
		}

		en_screen[i] = random_number(seed, total_screens);
		en_x[i] = 200 + random_number(seed, screen_width - 400);
		en_y[i] = floor_y - enemy_height(en_type[i]);
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
		else
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
			if (en_dir[i] > 0)
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

// health bar (top left) and the weapon / ammo text under it
void draw_hud(RenderWindow& window, Sprite& barBack, Sprite& barFill, Text& hudText, const int health, const int max_health, const int weapon, const int magnum_ammo, const int grenade_ammo, const int score)
{
	barBack.setPosition(16, 16);
	barBack.setScale(204, 20);
	window.draw(barBack);

	barFill.setPosition(18, 18);
	barFill.setScale(200.0 * health / max_health, 16);
	window.draw(barFill);

	char line[80];
	line[0] = '\0';
	if (weapon == 1)
	{
		append_text(line, "1 Laser Whip   ammo: unlimited");
	}
	else if (weapon == 2)
	{
		append_text(line, "2 .44 Magnum   ammo: ");
		append_number(line, magnum_ammo);
	}
	else
	{
		append_text(line, "3 Grenades   ammo: ");
		append_number(line, grenade_ammo);
	}
	append_text(line, "\nScore: ");
	append_number(line, score);
	hudText.setString(line);
	window.draw(hudText);
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
	Texture blockTexture;
	Sprite blockSprite;

	if (!bgTex.loadFromFile("Data/bg.png"))
	{
		cout << "Could not load Data/bg.png (run the game from the folder that contains Data/)" << endl;
		return 1;
	}
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	if (!blockTexture.loadFromFile("Data/block1.png"))
	{
		cout << "Could not load Data/block1.png" << endl;
		return 1;
	}
	blockSprite.setTexture(blockTexture);

	Texture ladderTexture;
	Sprite ladderSprite;

	if (!ladderTexture.loadFromFile("Data/ladder.png"))
	{
		cout << "Could not load Data/ladder.png" << endl;
		return 1;
	}
	ladderSprite.setTexture(ladderTexture);

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
	float player_x = 780;
	float player_y = 60;

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
	Texture barBackTexture;
	Texture barFillTexture;
	Sprite bulletSprite;
	Sprite grenadeSprite;
	Sprite whipSprite;
	Sprite barBack;
	Sprite barFill;
	Font font;
	Text hudText;

	if (!bulletTexture.loadFromFile("Data/bullet.png") || !grenadeTexture.loadFromFile("Data/grenade.png") || !whipTexture.loadFromFile("Data/whip.png") || !barBackTexture.loadFromFile("Data/bar_back.png") || !barFillTexture.loadFromFile("Data/bar_fill.png"))
	{
		cout << "Could not load a weapon or HUD image from Data/ (bullet, grenade, whip, bar_back, bar_fill)" << endl;
		return 1;
	}
	if (!font.loadFromFile("Data/font.ttf"))
	{
		cout << "Could not load Data/font.ttf" << endl;
		return 1;
	}
	bulletSprite.setTexture(bulletTexture);
	grenadeSprite.setTexture(grenadeTexture);
	whipSprite.setTexture(whipTexture);
	barBack.setTexture(barBackTexture);
	barFill.setTexture(barFillTexture);
	hudText.setFont(font);
	hudText.setCharacterSize(22);
	hudText.setFillColor(Color(188, 190, 0));
	hudText.setPosition(16, 44);

	// enemies (level 1: 4 crawlers + 5 blue jumpers)
	const int num_crawlers = 4;
	const int max_enemies = 9;
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

	Texture enemyTexture[2];
	Sprite enemySprite[2];
	const char* enemy_file[2] = { "Data/crawler.png", "Data/jumper.png" };
	for (int i = 0; i < 2; i += 1)
	{
		if (!enemyTexture[i].loadFromFile(enemy_file[i]))
		{
			cout << "Could not load " << enemy_file[i] << endl;
			return 1;
		}
		enemySprite[i].setTexture(enemyTexture[i]);
	}

	long long seed = time(0);
	spawn_enemies(en_type, en_screen, en_x, en_y, en_vx, en_vy, en_dir, en_health, en_alive, en_ground, en_timer, seed, max_enemies, num_crawlers, total_screens, screen_x, (height - 1) * cell_size);

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
	load_screen(lvl, current_screen, height, width);

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

		// dying: respawn at the start of the level (temporary, a proper game over comes later)
		if (health <= 0)
		{
			deaths += 1;
			health = max_health;
			player_x = 780;
			player_y = 60;
			velocityY = 0;
			climbing = false;
			current_screen = 0;
			load_screen(lvl, current_screen, height, width);
		}

		// switch screen when the player walks off an edge
		if (check_screen_change(player_x, current_screen, total_screens, screen_x, PlayerWidth))
		{
			load_screen(lvl, current_screen, height, width);
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
		display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, ladderTexture, ladderSprite, height, width, cell_size);
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
		draw_hud(window, barBack, barFill, hudText, health, max_health, weapon, magnum_ammo, grenade_ammo, score);
		window.display();
	}

	// stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}