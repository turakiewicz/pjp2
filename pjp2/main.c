#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_primitives.h>

#include<stdlib.h>
#include<stdio.h>
#include<time.h>

//Włączanie/wyłączanie konsoli: Project -> [project name] Properties -> Configuration properties -> Linker -> System -> SubSystem -> (SUBSYSTEM:CONSOLE / SUBSYSTEM:WINDOWS)

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
	al_init_primitives_addon();
//Installing
	if (!al_install_keyboard()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install keyboard!", NULL, NULL);
	}
	if (!al_install_mouse()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install mouse!", NULL, NULL);
	}
//Main variables and functions
	int yon();
	int pom();
	int i, j;
	float f;
	const float FPS = 60.0f;
	float hFPS[6]; //Horse animation timer speed
	for (i = 0; i < 6; i++) {
		hFPS[i] = 10.0f;
	}
	srand(time(NULL));
	int layout = 1;
	bool sounds = true, gameDone = false, draw = true;
	int winner;
	int hLevel[6]; //Horses levels
	unsigned int balance, bet, betHorse, possibleProfit, add;
	float cameraPosition[2] = { 0, 0 };
//Allegro pointers
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT events;
	ALLEGRO_DISPLAY *display = al_create_display(640, 360);
	ALLEGRO_TRANSFORM camera;
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *h00Timer = al_create_timer(1.0 / hFPS[0]); //Horse00 animation timer
	ALLEGRO_TIMER *h01Timer = al_create_timer(1.0 / hFPS[1]); //Horse00 animation timer
	ALLEGRO_TIMER *h02Timer = al_create_timer(1.0 / hFPS[2]); //Horse00 animation timer
	ALLEGRO_TIMER *h03Timer = al_create_timer(1.0 / hFPS[3]); //Horse00 animation timer
	ALLEGRO_TIMER *h04Timer = al_create_timer(1.0 / hFPS[4]); //Horse00 animation timer
	ALLEGRO_TIMER *h05Timer = al_create_timer(1.0 / hFPS[5]); //Horse00 animation timer
	ALLEGRO_FONT *font20 = al_load_font("res/fonts/AmericanCaptain.ttf", 20, NULL);
	ALLEGRO_FONT *font25 = al_load_font("res/fonts/AmericanCaptain.ttf", 25, NULL);
	ALLEGRO_FONT *font30 = al_load_font("res/fonts/AmericanCaptain.ttf", 30, NULL);
	ALLEGRO_FONT *font60 = al_load_font("res/fonts/AmericanCaptain.ttf", 60, NULL);
	//Start layout graphics
	ALLEGRO_BITMAP *startBackground = al_load_bitmap("res/graphics/start/background.png");
	ALLEGRO_BITMAP *highlightBig = al_load_bitmap("res/graphics/start/highlightBig.png");
	ALLEGRO_BITMAP *highlightSmall = al_load_bitmap("res/graphics/start/highlightSmall.png");
	//Betting layout graphics
	ALLEGRO_BITMAP *bettingBackground = al_load_bitmap("res/graphics/betting/background.png");
	ALLEGRO_BITMAP *raceGreyOut = al_load_bitmap("res/graphics/betting/raceGreyOut.png");
	ALLEGRO_BITMAP *horseFace01 = NULL;
	ALLEGRO_BITMAP *horseFace02 = NULL;
	ALLEGRO_BITMAP *horseFace03 = NULL;
	ALLEGRO_BITMAP *horseFace04 = NULL;
	ALLEGRO_BITMAP *horseFace05 = NULL;
	ALLEGRO_BITMAP *horseFace06 = NULL;
	//Instructions layout graphics
	ALLEGRO_BITMAP *instructionsBackground = al_load_bitmap("res/graphics/instructions/background.png");
	//Race layout
	ALLEGRO_BITMAP *raceBackground = al_load_bitmap("res/graphics/race/background.png");
	ALLEGRO_BITMAP *finish = al_load_bitmap("res/graphics/race/finish.png");
	ALLEGRO_BITMAP *raceInfo = al_load_bitmap("res/graphics/race/raceInfo.png");
	ALLEGRO_BITMAP *box1 = al_load_bitmap("res/graphics/race/box1.png");
	ALLEGRO_BITMAP *box2 = al_load_bitmap("res/graphics/race/box2.png");
	ALLEGRO_BITMAP *box3 = al_load_bitmap("res/graphics/race/box3.png");
	ALLEGRO_BITMAP *box4 = al_load_bitmap("res/graphics/race/box4.png");
	ALLEGRO_BITMAP *box5 = al_load_bitmap("res/graphics/race/box5.png");
	ALLEGRO_BITMAP *box6 = al_load_bitmap("res/graphics/race/box6.png");
	ALLEGRO_BITMAP *horse01 = NULL;
	ALLEGRO_BITMAP *horse02 = NULL;
	ALLEGRO_BITMAP *horse03 = NULL;
	ALLEGRO_BITMAP *horse04 = NULL;
	ALLEGRO_BITMAP *horse05 = NULL;
	ALLEGRO_BITMAP *horse06 = NULL;
//Registering event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h00Timer));
	al_start_timer(h00Timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h01Timer));
	al_start_timer(h01Timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h02Timer));
	al_start_timer(h02Timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h03Timer));
	al_start_timer(h03Timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h04Timer));
	al_start_timer(h04Timer);
	al_register_event_source(event_queue, al_get_timer_event_source(h05Timer));
	al_start_timer(h05Timer);
//Reading high scores from file
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



//Game loop
	while (!gameDone) {



	//Start screen layout
		if (layout == 1) {
			balance = 1000;
			add = 100;

			bool highlightExitDraw = false, highlightStartDraw = false, highlightInstructionsDraw = false;

		//Start screen loop
			while (layout == 1) {
				if (events.type == ALLEGRO_EVENT_TIMER) {
					draw = true;
				}

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
					//Start button
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
				if (events.type == ALLEGRO_EVENT_MOUSE_AXES) {
					//Exit button highlight
					if (events.mouse.x >= 40 && events.mouse.x <= 160 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						highlightExitDraw = true;
					}
					else (highlightExitDraw = false);
					//Start button highlight
					if (events.mouse.x >= 200 && events.mouse.x <= 440 && events.mouse.y >= 220 && events.mouse.y <= 320) {
						highlightStartDraw = true;
					}
					else (highlightStartDraw = false);
					//Instructions highlight
					if (events.mouse.x >= 480 && events.mouse.x <= 600 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						highlightInstructionsDraw = true;
					}
					else (highlightInstructionsDraw = false);
				}

				if (draw) {
					draw = false;


					al_draw_bitmap(startBackground, 0, 0, NULL);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 64.0f, ALLEGRO_ALIGN_RIGHT, podiumName1);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 64.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore1);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 88.0f, ALLEGRO_ALIGN_RIGHT, podiumName2);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 88.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore2);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 112.0f, ALLEGRO_ALIGN_RIGHT, podiumName3);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 112.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore3);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 136.0f, ALLEGRO_ALIGN_RIGHT, podiumName4);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 136.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore4);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 160.0f, ALLEGRO_ALIGN_RIGHT, podiumName5);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 160.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore5);

					

					if (highlightExitDraw) {
						al_draw_bitmap(highlightSmall, 40.0f, 240.0f, NULL);
					}
					else if (highlightStartDraw) {
						al_draw_bitmap(highlightBig, 200.0f, 220.0f, NULL);
					}
					else if (highlightInstructionsDraw) {
						al_draw_bitmap(highlightSmall, 480.0f, 240.0f, NULL);
					}

					al_flip_display();
				}
			}
		}

	//Betting layout
		if (layout == 2) {
			bet = 0;
			betHorse = -1;
			possibleProfit = 0;

			for (i = 0; i < 6; i++) {
				hLevel[i] = rand() % 6 + 1;	//Setting random horse levels
				//printf(">horse0%i - %i\n", i + 1, hLevel[i]);
			}

			//Set camera to default position
			al_identity_transform(&camera);
			al_translate_transform(&camera, 0, 0);
			al_use_transform(&camera);

			//Setting random horse skins
			//Horse01
			i = rand() % 10;
			switch (i) {
			case 0: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse01 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}
			//Horse02
			i = rand() % 10;
			switch (i) {
			case 0: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse02 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}
			//Horse03
			i = rand() % 10;
			switch (i) {
			case 0: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse03 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}
			//Horse04
			i = rand() % 10;
			switch (i) {
			case 0: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse04 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}
			//Horse05
			i = rand() % 10;
			switch (i) {
			case 0: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse05 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}
			//Horse06
			i = rand() % 10;
			switch (i) {
			case 0: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet00.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet01.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet02.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet03.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet04.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet05.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet06.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			case 7: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet07.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace07.png");
				break;
			case 8: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet08.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace08.png");
				break;
			case 9: horse06 = al_load_bitmap("res/graphics/race/horseSpriteSheet09.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace09.png");
				break;
			}

			//Betting loop
			while (layout == 2) {
				if (events.type == ALLEGRO_EVENT_TIMER) {
					draw = true;
				}

				al_wait_for_event(event_queue, &events);
				if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
					//Exit
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						layout = 1;
						printf("Layout: 1\n");
					}
				}

				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					//Left mouse button clicked
					if (events.mouse.button & 1) {
						//Begin race button
						if (events.mouse.x >= 430 && events.mouse.x <= 620 && events.mouse.y >= 260 && events.mouse.y <= 340 &&
							bet > 0 && betHorse != -1) {
							layout = 4;
							printf("Layout: 4\n");
						}
						//Change bet addition
						if (events.mouse.y >= 270 && events.mouse.y <= 290) {
							//Multiple by 10
							if (events.mouse.x >= 340 && events.mouse.x <= 360 &&
								add * 10 <= balance + bet) {
								add *= 10;
							}
							//Divide by 10
							if (events.mouse.x >= 260 && events.mouse.x <= 280 &&
								add > 1) {
								add /= 10;
							}
						}
						//Change bet
						if (events.mouse.x >= 290 && events.mouse.x <= 330) {
							//Add
							if (events.mouse.y >= 290 && events.mouse.y <= 310 &&
								balance >= add) {
								bet += add;
								balance -= add;
							}
							//Subtract
							if (events.mouse.y >= 310 && events.mouse.y <= 330 &&
								bet >= add) {
								bet -= add;
								balance += add;
							}
						}

						//Horse choose buttons
						if (events.mouse.y >= 120 && events.mouse.y <= 180) {
							//Horse 1
							if (events.mouse.x >= 30 && events.mouse.x <= 110) {
								betHorse = 0;
								printf("horse0%i choosen\n", betHorse + 1);
							}
							//Horse 2
							if (events.mouse.x >= 130 && events.mouse.x <= 210) {
								betHorse = 1;
								printf("horse0%i choosen\n", betHorse + 1);
							}
							//Horse 3
							if (events.mouse.x >= 230 && events.mouse.x <= 310) {
								betHorse = 2;
								printf("horse0%i choosen\n", betHorse + 1);
							}
							//Horse 4
							if (events.mouse.x >= 330 && events.mouse.x <= 410) {
								betHorse = 3;
								printf("horse0%i choosen\n", betHorse + 1);
							}
							//Horse 5
							if (events.mouse.x >= 430 && events.mouse.x <= 510) {
								betHorse = 4;
								printf("horse0%i choosen\n", betHorse + 1);
							}
							//Horse 6
							if (events.mouse.x >= 530 && events.mouse.x <= 610) {
								betHorse = 5;
								printf("horse0%i choosen\n", betHorse + 1);
							}
						}
					}	
				}

				if (betHorse != -1) {
					int sum = 0;
					for (i = 0; i < 6; i++) {
						sum += hLevel[i];
					}
					float sumDivided = sum / hLevel[betHorse]; //sum divided by betHorse level
					possibleProfit = (unsigned int)(sumDivided < 0 ? (sumDivided - 0.5) : (sumDivided + 0.5)); //Rounding mechanism
					possibleProfit *= bet;
				}

				if (draw) {
					draw = false;

					al_draw_bitmap(horseFace01, 30.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace02, 130.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace03, 230.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace04, 330.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace05, 430.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace06, 530.0f, 120.0f, NULL);
					al_draw_bitmap(bettingBackground, 0, 0, NULL);

					for (i = 0; i < 6; i++) {
						for (j = 0; j < hLevel[i]; j++) {
							al_draw_filled_circle(42.0f + i * 100.0f + j * 11.0f, 190.0f, 3.0f, al_map_rgb(0, 70 + 30 * j, 0));	//Show horses' level
						}
					}

					if (betHorse != -1) {
						//al_draw_rectangle(28.0f + betHorse * 100, 118.0f, 112.0f + betHorse * 100, 182.0f, al_map_rgb(70, 200, 70), 4);
						al_draw_rounded_rectangle(28.0f + betHorse * 100, 118.0f, 112.0f + betHorse * 100, 182.0f, 12, 12, al_map_rgb(174, 0, 0), 5);
					}
					
					if (betHorse == -1 || bet == 0) {
						al_draw_bitmap(raceGreyOut, 430.0f, 260.0f, NULL);
					}

					al_draw_textf(font30, al_map_rgb(172, 129, 25), 340.0f, 38.0f, ALLEGRO_ALIGN_LEFT, "%u", balance);
					al_draw_textf(font25, al_map_rgb(5, 30, 17), 40.0f, 290.0f, ALLEGRO_ALIGN_LEFT, "%u", bet);
					al_draw_textf(font25, al_map_rgb(0, 0, 0), 440.0f, 220.0f, ALLEGRO_ALIGN_LEFT, "%u", possibleProfit);
					al_draw_textf(font20, al_map_rgb(43, 114, 78), 310.0f, 271.0f, ALLEGRO_ALIGN_CENTER, "%u", add);

					al_flip_display();
				}


			}
		}

	//Instructions layout
		if (layout == 3) {
			al_draw_bitmap(instructionsBackground, 0, 0, NULL);
			al_flip_display();

			//Set camera to default position
			al_identity_transform(&camera);
			al_translate_transform(&camera, 0, 0);
			al_use_transform(&camera);

			al_wait_for_event(event_queue, &events);
			if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					layout = 1;
					printf("Layout: 1\n");
				}
			}
		}

	//Race layout
		if (layout == 4) {
			bool race = true;
			float hPosX[6], hPosY[6];
			float bPosY[6];
			float hVelocity[6] = { 2.5, 2.5, 2.5, 2.5, 2.5, 2.5}; //Horses velocity
			int closestHorse;
			float offset = 0;

			//Animation variables
			float hSourceX[6], hSourceY[6];
			bool hDraw[6];
			for (i = 0; i < 6; i++) {
				hSourceX[i] = 0;
				hSourceY[i] = 0;
				hDraw[i] = false;
			}

			for (i = 0; i < 6; i++) {
				hPosX[i] = 20;
				hPosY[i] = 130 + 30 * i;
				bPosY[i] = 150 + 30 * i;
			}

			while (layout == 4) {
				al_wait_for_event(event_queue, &events);
				if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						if (!race) {
							layout = 2;
							printf("Layout: 2\n");
						}
						else {
							al_stop_timer(timer);
							al_stop_timer(h00Timer);
							al_stop_timer(h01Timer);
							al_stop_timer(h02Timer);
							al_stop_timer(h03Timer);
							al_stop_timer(h04Timer);
							al_stop_timer(h05Timer);
							printf("Race stopped\n");
							if (al_show_native_message_box(display, "Stop the race", "Your bet will be lost", "Are you sure you want to stop the race?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1){
								layout = 2;
								printf("Layout: 2\n");
								al_start_timer(timer);
								al_start_timer(h00Timer);
								al_start_timer(h01Timer);
								al_start_timer(h02Timer);
								al_start_timer(h03Timer);
								al_start_timer(h04Timer);
								al_start_timer(h05Timer);
							}
							else {
								al_start_timer(timer);
								al_start_timer(h00Timer);
								al_start_timer(h01Timer);
								al_start_timer(h02Timer);
								al_start_timer(h03Timer);
								al_start_timer(h04Timer);
								al_start_timer(h05Timer);
								printf("Race started\n");
							}
						}
					}
				}
				
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					if (events.mouse.button & 1 && !race) {
						layout = 2;
						printf("Layout: 2\n");
					}
				}

				
				if (events.type == ALLEGRO_EVENT_TIMER) {
					if (race) {
						//Every FPS events
						if (events.timer.source == timer) {
							for (i = 0; i < 6; i++) {
								hVelocity[i] += yon() * hLevel[i] * 0.002 + pom() * 0.03 + pom() * 0.03;	//Horses movement

								if (hVelocity[i] > 0) {
									hPosX[i] += hVelocity[i];
								}
							}
							
							//Checking which horse is closest to finish
							closestHorse = 0;
							f = 0;
							for (i = 0; i < 6; i++) {
								if (hPosX[i] > f) {
									f = hPosX[i];
									closestHorse = i;
								}
							}
							
							//Changing camera position after horse closest to finish
							if ((hPosX[closestHorse] > offset + al_get_display_width(display) / 3 * 2) && offset <= 640) {
								offset += hVelocity[closestHorse];
								al_identity_transform(&camera);
								al_translate_transform(&camera, -offset, 0);
								al_use_transform(&camera);
							}
							
						}
						//Horses animation timers
						else if (events.timer.source == h00Timer) {
							hDraw[0] = true;
						}
						else if (events.timer.source == h01Timer) {
							hDraw[1] = true;
						}
						else if (events.timer.source == h02Timer) {
							hDraw[2] = true;
						}
						else if (events.timer.source == h03Timer) {
							hDraw[3] = true;
						}
						else if (events.timer.source == h04Timer) {
							hDraw[4] = true;
						}
						else if (events.timer.source == h05Timer) {
							hDraw[5] = true;
						}
					}

					

					draw = true;
				}
					
				j = 1;
				
				if (draw) {
					draw = false;
					
					if (race) {
						al_draw_bitmap(raceBackground, 0, 0, NULL);

						//Info window
						al_draw_bitmap(raceInfo, 0 + offset, 0, NULL);
						for (i = 0; i < 6; i++) {
							for (j = 0; j < hLevel[i]; j++) {
								al_draw_filled_circle(35.0f + 10.0f * j + offset, 25.0f + 10.f * i, 3.0f, al_map_rgb(0, 70 + 30 * j, 0));	//Show horses' level
							}
						}


						//Win event
						for (i = 0; i < 6; i++) {
							if (hPosX[i] >= 1220){//590) {
								race = false;

								al_draw_filled_rectangle(offset, 140.0f + 30 * i, 640 + offset, 170.0f + 30 * i, al_map_rgb(172, 129, 25));
								al_draw_text(font25, al_map_rgb(248, 187, 40), 320.0f + offset, 145.0f + i * 30.0f, ALLEGRO_ALIGN_CENTER, "W I N N E R");

								al_draw_text(font20, al_map_rgb(32, 75, 31), 320.0f + offset, 333.0f, ALLEGRO_ALIGN_CENTER, "Click anywhere to continue...");

								printf("***Horse0%i wins!***\n", i + 1);
								if (i == betHorse) {
									balance += possibleProfit;

									al_draw_text(font60, al_map_rgb(174, 0, 0), 120.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "YOU WON");
									al_draw_textf(font60, al_map_rgb(172, 129, 25), 310.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "%u", possibleProfit);
								}
								else {
									al_draw_text(font60, al_map_rgb(174, 0, 0), 120.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "YOU LOST");
								}

								break;
							}
						}

						al_draw_bitmap(finish, 1230.0f, 140.0f, NULL);

						for (i = 0; i < 6; i++) {
							al_draw_textf(font20, al_map_rgb(181, 155, 137), 5.0f + offset, 147.0f + i * 30.0f, ALLEGRO_ALIGN_LEFT, "%i", i + 1);
						}

						al_draw_text(font20, al_map_rgb(181, 155, 137), hPosX[betHorse] - 20.0f, 147.0f + betHorse * 30.0f, ALLEGRO_ALIGN_RIGHT, "YOUR HORSE");


						//HORSES ANIMATION
						
						//Horse01 animation
						al_draw_bitmap_region(horse01, hSourceX[0], hSourceY[0], 40, 40, hPosX[0], hPosY[0], NULL);
						hFPS[0] = hVelocity[0] * 13;

						al_set_timer_speed(h00Timer, 1.0f / hFPS[0]);

						if (hDraw[0]) {
							hDraw[0] = false;

							if (hSourceX[0] < 120) {
								hSourceX[0] += 40;
							}
							else {
								hSourceX[0] = 0;
								hSourceY[0] += 40;
								if (hSourceY[0] >= 160) {
									hSourceY[0] = 0;
								}
							}
						}
						//al_draw_bitmap(box1, 20, bPosY[0], NULL);

						//Horse02 animation
						al_draw_bitmap_region(horse02, hSourceX[1], hSourceY[1], 40, 40, hPosX[1], hPosY[1], NULL);
						hFPS[1] = hVelocity[1] * 13;

						al_set_timer_speed(h01Timer, 1.0f / hFPS[1]);

						if (hDraw[1]) {
							hDraw[1] = false;

							if (hSourceX[1] < 120) {
								hSourceX[1] += 40;
							}
							else {
								hSourceX[1] = 0;
								hSourceY[1] += 40;
								if (hSourceY[1] >= 160) {
									hSourceY[1] = 0;
								}
							}
						}
						//al_draw_bitmap(box2, 20, bPosY[1], NULL);
						
						//Horse03 animation
						al_draw_bitmap_region(horse03, hSourceX[2], hSourceY[2], 40, 40, hPosX[2], hPosY[2], NULL);
						hFPS[2] = hVelocity[2] * 13;

						al_set_timer_speed(h02Timer, 1.0f / hFPS[2]);

						if (hDraw[2]) {
							hDraw[2] = false;

							if (hSourceX[2] < 120) {
								hSourceX[2] += 40;
							}
							else {
								hSourceX[2] = 0;
								hSourceY[2] += 40;
								if (hSourceY[2] >= 160) {
									hSourceY[2] = 0;
								}
							}
						}
						//al_draw_bitmap(box3, 20, bPosY[2], NULL);

						//Horse04 animation
						al_draw_bitmap_region(horse04, hSourceX[3], hSourceY[3], 40, 40, hPosX[3], hPosY[3], NULL);
						hFPS[3] = hVelocity[3] * 13;

						al_set_timer_speed(h03Timer, 1.0f / hFPS[3]);

						if (hDraw[3]) {
							hDraw[3] = false;

							if (hSourceX[3] < 120) {
								hSourceX[3] += 40;
							}
							else {
								hSourceX[3] = 0;
								hSourceY[3] += 40;
								if (hSourceY[3] >= 160) {
									hSourceY[3] = 0;
								}
							}
						}
						//al_draw_bitmap(box4, 20, bPosY[3], NULL);

						//Horse05 animation
						al_draw_bitmap_region(horse05, hSourceX[4], hSourceY[4], 40, 40, hPosX[4], hPosY[4], NULL);
						hFPS[4] = hVelocity[4] * 13;

						al_set_timer_speed(h04Timer, 1.0f / hFPS[4]);

						if (hDraw[4]) {
							hDraw[4] = false;

							if (hSourceX[4] < 120) {
								hSourceX[4] += 40;
							}
							else {
								hSourceX[4] = 0;
								hSourceY[4] += 40;
								if (hSourceY[4] >= 160) {
									hSourceY[4] = 0;
								}
							}
						}
						//al_draw_bitmap(box5, 20, bPosY[4], NULL);

						//Horse06 animation
						al_draw_bitmap_region(horse06, hSourceX[5], hSourceY[5], 40, 40, hPosX[5], hPosY[5], NULL);
						hFPS[5] = hVelocity[5] * 13;

						al_set_timer_speed(h05Timer, 1.0f / hFPS[5]);

						if (hDraw[5]) {
							hDraw[5] = false;

							if (hSourceX[5] < 120) {
								hSourceX[5] += 40;
							}
							else {
								hSourceX[5] = 0;
								hSourceY[5] += 40;
								if (hSourceY[5] >= 160) {
									hSourceY[5] = 0;
								}
							}
						}
						//al_draw_bitmap(box6, 20, bPosY[5], NULL);

						//END OF HORSES ANIMATION
						
					}

					al_flip_display();
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
	al_destroy_bitmap(raceBackground);
	al_destroy_bitmap(highlightBig);
	al_destroy_bitmap(highlightSmall);
	al_destroy_bitmap(raceGreyOut);
	al_destroy_bitmap(horseFace01);
	al_destroy_bitmap(horseFace02);
	al_destroy_bitmap(horseFace03);
	al_destroy_bitmap(horseFace04);
	al_destroy_bitmap(horseFace05);
	al_destroy_bitmap(horseFace06);
	al_destroy_bitmap(box1);
	al_destroy_bitmap(box2);
	al_destroy_bitmap(box3);
	al_destroy_bitmap(box4);
	al_destroy_bitmap(box5);
	al_destroy_bitmap(box6);
	al_destroy_bitmap(horse01);
	al_destroy_bitmap(horse02);
	al_destroy_bitmap(horse03);
	al_destroy_bitmap(horse04);
	al_destroy_bitmap(horse05);
	al_destroy_bitmap(horse06);
	al_destroy_bitmap(finish);
	al_destroy_bitmap(raceInfo);
	al_destroy_font(font20);
	al_destroy_timer(timer);
	al_destroy_timer(h00Timer);
	al_destroy_timer(h01Timer);
	al_destroy_timer(h02Timer);
	al_destroy_timer(h03Timer);
	al_destroy_timer(h04Timer);
	al_destroy_timer(h05Timer);


	return 0;
}

//Additional functions

int yon() {
	//'Yes or no' function (randomly returns 0 or 1)
	return rand() % 2;
}

int pom() {
	//'Plus or minus' function (randomly returns 1 or -1)
	int yn = rand() % 2;
	if (yn == 1) {
		return 1;
	}
	else {
		return -1;
	}
}
