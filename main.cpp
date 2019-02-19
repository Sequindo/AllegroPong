#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "math.h"
#include <iostream>
#define WINDOW_W 1380
#define WINDOW_H 1020
using namespace std;

const int SCREEN_W = 1260;
const int SCREEN_H = 720;
const int FPS = 60;
const int palette_w = 350;
const int maxBallSpeed = 10.0;

enum KEYS
{
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
enum KEYS_P2
{
	W, A, S, D
};
struct padStruct
{
	float speed = 15.0;
	float x1 = SCREEN_W / 2 - palette_w/2; 
	float x2 = SCREEN_W / 2 + palette_w/2;
} data;
struct ballStruct
{
	float rad = 10;
	float speedX = 0.0;
	float speedY = maxBallSpeed;
	float x = SCREEN_W / 2;
	float y = SCREEN_H / 2;
	void reset()
	{
		x = SCREEN_W / 2; y = SCREEN_H / 2; speedY = maxBallSpeed; speedX = 0.0;
	}
	void checkHit()
	{
		if (x < 0 || x > SCREEN_W) speedX = -speedX;
	}
	void updatePos()
	{
		x = x + speedX;
		y = y + speedY;
	}
} balldata;

padStruct paddle;
padStruct paddle2;
ballStruct ball;

void checkColiisionWithPad(void)
{
	if (ball.y >= 700 - ball.rad || ball.y <= 20+ball.rad)//pi³ka trafia na paletkê...
	{
		if ((ball.x >= paddle.x1 && ball.x <= paddle.x2) || (ball.x >= paddle2.x1&&ball.x <= paddle2.x2))
		{
			float dist, distSpeed;
			bool down = false;
			if (ball.y <= 20 + ball.rad)
			{
				dist = ball.x - paddle2.x1; 
				down = true;
			}
			else dist = ball.x - paddle.x1;
			distSpeed = -(palette_w / 2 - dist);
			ball.speedX = distSpeed * (maxBallSpeed / (palette_w / 2.0));
			ball.speedY = (sqrt(maxBallSpeed*maxBallSpeed - (ball.speedX*ball.speedX)));
			if (!down) ball.speedY *= (-1);
		}
	}
}
int main()
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_COLOR AL_WHITE, AL_FGREEN;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	bool noexit = true;
	bool redraw = true;
	bool keyP1[4] = { false, false, false, false };
	bool keyP2[4] = { false, false, false, false };
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	AL_WHITE = al_map_rgb(255, 255, 255);
	AL_FGREEN = al_map_rgb(34, 139, 34);
	al_flip_display();
	al_start_timer(timer);
	while (noexit)
	{
		while (!al_event_queue_is_empty(event_queue))
		{
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_TIMER)
			{
				if (keyP1[KEY_LEFT] && paddle.x1 > 0)
				{
					paddle.x1 -= paddle.speed;
					paddle.x2 -= paddle.speed;
				}
				if (keyP2[A] && paddle2.x1 > 0)
				{
					paddle2.x1 -= paddle2.speed;
					paddle2.x2 -= paddle2.speed;
				}
				if (keyP1[KEY_RIGHT] && paddle.x2 < SCREEN_W)
				{
					paddle.x1 += paddle.speed;
					paddle.x2 += paddle.speed;
				}
				if (keyP2[D] && paddle2.x2 < SCREEN_W)
				{
					paddle2.x1 += paddle2.speed;
					paddle2.x2 += paddle2.speed;
				}
				redraw = true;
			}
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				break;
			}
			else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				//	paddle.speed += 10;
				switch (event.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					keyP1[KEY_UP] = true;
					break;

				case ALLEGRO_KEY_DOWN:
					keyP1[KEY_DOWN] = true;
					break;

				case ALLEGRO_KEY_LEFT:
					keyP1[KEY_LEFT] = true;
					break;

				case ALLEGRO_KEY_RIGHT:
					keyP1[KEY_RIGHT] = true;
					break;
				case ALLEGRO_KEY_W:
					keyP2[W] = true;
					break;
				case ALLEGRO_KEY_A:
					keyP2[A] = true;
					break;
				case ALLEGRO_KEY_S:
					keyP2[S] = true;
					break;
				case ALLEGRO_KEY_D:
					keyP2[D] = true;
					break;
				case ALLEGRO_KEY_R:
					ball.reset();
					break;
				}
			}
			else if (event.type == ALLEGRO_EVENT_KEY_UP) {
				//paddle.reset();
				switch (event.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					keyP1[KEY_UP] = false;
					break;

				case ALLEGRO_KEY_DOWN:
					keyP1[KEY_DOWN] = false;
					break;

				case ALLEGRO_KEY_LEFT:
					keyP1[KEY_LEFT] = false;
					break;

				case ALLEGRO_KEY_RIGHT:
					keyP1[KEY_RIGHT] = false;
					break;
				case ALLEGRO_KEY_W:
					keyP2[W] = false;
					break;
				case ALLEGRO_KEY_A:
					keyP2[A] = false;
					break;
				case ALLEGRO_KEY_S:
					keyP2[S] = false;
					break;
				case ALLEGRO_KEY_D:
					keyP2[D] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					noexit = false;
					break;
				}
			}
		}
		if (redraw)
		{
			
			redraw = false;
		//	cout << paddle2.x1 << " " << paddle2.x2 << ", pileczka: " << ball.x << " " << ball.y << endl;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_filled_rectangle(paddle.x1, 720, paddle.x2, 700, AL_WHITE);
			al_draw_filled_rectangle(paddle2.x1, 0, paddle2.x2, 20, AL_FGREEN);
			al_draw_filled_circle(ball.x, ball.y, ball.rad, AL_WHITE);
			checkColiisionWithPad();
			ball.updatePos();
			ball.checkHit();
			al_flip_display();
		}
	}
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}