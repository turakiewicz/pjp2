#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_font.h>

#include<stdlib.h>
#include<stdio.h>

int main() {
	
//Initializing
	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot initialize Allegro!", NULL, NULL);
	}
	if (!al_init_image_addon()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot initialize image addon!", NULL, NULL);
	}	
	al_init_font_addon();
	al_init_ttf_addon();

//Installing
	if (!al_install_keyboard()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install keyboard!", NULL, NULL);
	}
	if (!al_install_mouse()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install mouse!", NULL, NULL);
	}

//Declaring
	const float FPS = 60.0f;

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT events;
	ALLEGRO_DISPLAY *display = al_create_display(640, 360);
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_FONT *font1 = al_load_font("res/fonts/AmericanCaptain.ttf", 20, NULL);
	ALLEGRO_BITMAP *startBackground = al_load_bitmap("res/graphics/start/background.png");
	ALLEGRO_BITMAP *bettingBackground = al_load_bitmap("res/graphics/betting/background.png");
	ALLEGRO_BITMAP *instructionsBackground = al_load_bitmap("res/graphics/instructions/background.png");

	int layout = 1;
	bool sounds = true;
	bool gameDone = false;

//Registering event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
//Reading high scores from file
	int i, j;
	//Loading scoreList.txt
	FILE *fp;
	char scoreList[101];
	if (fp = fopen("res/saves/scoreList.txt", "r")) {
		fscanf(fp, "%s", &scoreList);
		fclose(fp);
		printf("scoreList succesfully loaded.\n");
	}
	//Creating scoreList.txt if it doesn't exist
	else {
		printf("Cannot load scoreList.txt\n");
		FILE *fp1;
		fp1 = fopen("res/saves/scoreList.txt", "w");
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 10; j++) {
				scoreList[i * 20 + j] = 'A';
			}			
			for (j = 0; j < 10; j++) {
				scoreList[i * 20 + 10 + j] = '0';
			}		
		}
		for (i = 0; i < 100; i++) {
			fprintf(fp1, "%c", scoreList[i]);
		}
		fclose(fp1);
		printf("scoreList.txt template generated\n");
	}
	//Divide scoreList[100] into smaller arrays and integers
	char podiumName1[10];
	char podiumName2[10];
	char podiumName3[10];
	char podiumName4[10];
	char podiumName5[10];
	
	for (i = 0; i < 10; i++) {
		podiumName1[i] = scoreList[i];
		podiumName2[i] = scoreList[i + 20];
		podiumName3[i] = scoreList[i + 40];
		podiumName4[i] = scoreList[i + 60];
		podiumName5[i] = scoreList[i + 80];
	}

	unsigned long long int k;
	unsigned long long int podiumScore1 = 0, podiumScore2 = 0, podiumScore3 = 0, podiumScore4 = 0, podiumScore5 = 0;	
	j = 0;
	k = 1;
	while (j < 10) {
		podiumScore1 += (scoreList[0 * 20 + 10 + j] - 48) * k;
		podiumScore2 += (scoreList[1 * 20 + 10 + j] - 48) * k;
		podiumScore3 += (scoreList[2 * 20 + 10 + j] - 48) * k;
		podiumScore4 += (scoreList[3 * 20 + 10 + j] - 48) * k;
		podiumScore5 += (scoreList[4 * 20 + 10 + j] - 48) * k;
		j += 1;
		k *= 10;
	}



	al_start_timer(timer);

//Game loop
	while (!gameDone) {

	//Start screen layout
		if (layout == 1) {
			al_draw_bitmap(startBackground, 0, 0, NULL);
			al_draw_text(font1,	al_map_rgb(0, 0, 0), 310.0f, 64.0f, ALLEGRO_ALIGN_RIGHT, podiumName1);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 330.0f, 64.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore1);
			al_draw_text(font1, al_map_rgb(0, 0, 0), 310.0f, 88.0f, ALLEGRO_ALIGN_RIGHT, podiumName2);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 330.0f, 88.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore2);
			al_draw_text(font1, al_map_rgb(0, 0, 0), 310.0f, 112.0f, ALLEGRO_ALIGN_RIGHT, podiumName3);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 330.0f, 112.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore3);
			al_draw_text(font1, al_map_rgb(0, 0, 0), 310.0f, 136.0f, ALLEGRO_ALIGN_RIGHT, podiumName4);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 330.0f, 136.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore4);
			al_draw_text(font1, al_map_rgb(0, 0, 0), 310.0f, 160.0f, ALLEGRO_ALIGN_RIGHT, podiumName5);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 330.0f, 160.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore5);
			al_flip_display();

		//Start screen loop
			while (layout == 1) {
				al_wait_for_event(event_queue, &events);
				if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						layout = -1;
						gameDone = true;
					}
				}
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					//Exit button
					if (events.mouse.button & 1 && events.mouse.x >= 40 && events.mouse.x <= 160 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						layout = -1;
						gameDone = true;
					}
					//New game button
					if (events.mouse.button & 1 && events.mouse.x >= 200 && events.mouse.x <= 440 && events.mouse.y >= 220 && events.mouse.y <= 320) {
						layout = 2;
						printf("Layout: 2\n");
					}
					//Instructions button
					if (events.mouse.button & 1 && events.mouse.x >= 480 && events.mouse.x <= 600 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						layout = 3;
						printf("Layout: 3\n");
					}
					//Sounds button (doesn't work yet)
					if (events.mouse.button & 1 && events.mouse.x >= 20 && events.mouse.x <= 60 && events.mouse.y >= 20 && events.mouse.y <= 60 && sounds == true) {
						sounds = false;
						printf("Sounds: %i\n", sounds);
					}
				}
			}
		}

	//Betting layout
		if (layout == 2) {
			al_draw_bitmap(bettingBackground, 0, 0, NULL);
			al_flip_display();

			al_wait_for_event(event_queue, &events);
			if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					layout = 1;
					printf("Layout: 1\n");
				}
			}
		}

	//Instructions layout
		if (layout == 3) {
			al_draw_bitmap(instructionsBackground, 0, 0, NULL);
			al_flip_display();

			al_wait_for_event(event_queue, &events);
			if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					layout = 1;
					printf("Layout: 1\n");
				}
			}
		}




	}

//Dealocate memory
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(startBackground);
	al_destroy_bitmap(bettingBackground);
	al_destroy_bitmap(instructionsBackground);
	al_destroy_font(font1);
	al_destroy_timer(timer);

	return 0;
}