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
//Global variables
	int i, j;
	const float FPS = 60.0f;
	srand(time(NULL));
	int layout = 1;
	bool sounds = true, gameDone = false, draw = true;
	int winner;
	int hLevel[6]; //Horses levels
	unsigned int balance, bet, betHorse, possibleProfit, add;
//Allegro pointers
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT events;
	ALLEGRO_DISPLAY *display = al_create_display(640, 360);
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_FONT *font1 = al_load_font("res/fonts/AmericanCaptain.ttf", 20, NULL);
	//Start layout graphics
	ALLEGRO_BITMAP *startBackground = al_load_bitmap("res/graphics/start/background.png");
	//Betting layout graphics
	ALLEGRO_BITMAP *bettingBackground = al_load_bitmap("res/graphics/betting/background.png");
	ALLEGRO_BITMAP *horseFace01 = NULL;
	ALLEGRO_BITMAP *horseFace02 = NULL;
	ALLEGRO_BITMAP *horseFace03 = NULL;
	ALLEGRO_BITMAP *horseFace04 = NULL;
	ALLEGRO_BITMAP *horseFace05 = NULL;
	ALLEGRO_BITMAP *horseFace06 = NULL;
	//Instructions layout graphics
	ALLEGRO_BITMAP *instructionsBackground = al_load_bitmap("res/graphics/instructions/background.png");
	//Race layout graphics
	ALLEGRO_BITMAP *raceBackground = al_load_bitmap("res/graphics/race/background.png");
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
			bet = 0;
			betHorse = -1;
			possibleProfit = 0;

			for (i = 0; i < 6; i++) {
				hLevel[i] = rand() % 6 + 1;	//Setting random horse levels
				printf(">horse0%i - %i\n", i + 1, hLevel[i]);
			}

			//Setting random horse skins
			//Horse01
			i = rand() % 7;
			switch (i) {
			case 0: horse01 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse01 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse01 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse01 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse01 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse01 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse01 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace01 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			}
			//Horse02
			i = rand() % 7;
			switch (i) {
			case 0: horse02 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse02 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse02 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse02 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse02 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse02 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse02 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace02 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			}
			//Horse03
			i = rand() % 7;
			switch (i) {
			case 0: horse03 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse03 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse03 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse03 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse03 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse03 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse03 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace03 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			}
			//Horse04
			i = rand() % 7;
			switch (i) {
			case 0: horse04 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse04 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse04 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse04 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse04 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse04 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse04 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace04 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			}
			//Horse05
			i = rand() % 7;
			switch (i) {
			case 0: horse05 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse05 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse05 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse05 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse05 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse05 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse05 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace05 = al_load_bitmap("res/graphics/betting/horseFace06.png");
				break;
			}
			//Horse06
			i = rand() % 7;
			switch (i) {
			case 0: horse06 = al_load_bitmap("res/graphics/race/horse00.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace00.png");
				break;
			case 1: horse06 = al_load_bitmap("res/graphics/race/horse01.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace01.png");
				break;
			case 2: horse06 = al_load_bitmap("res/graphics/race/horse02.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace02.png");
				break;
			case 3: horse06 = al_load_bitmap("res/graphics/race/horse03.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace03.png");
				break;
			case 4: horse06 = al_load_bitmap("res/graphics/race/horse04.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace04.png");
				break;
			case 5: horse06 = al_load_bitmap("res/graphics/race/horse05.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace05.png");
				break;
			case 6: horse06 = al_load_bitmap("res/graphics/race/horse06.png");
				horseFace06 = al_load_bitmap("res/graphics/betting/horseFace06.png");
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
								add * 10 <= balance) {
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
					possibleProfit = (unsigned int)(sumDivided < 0 ? (sumDivided - 0.5) : (sumDivided + 0.5));
					possibleProfit *= bet;
				}

				if (draw) {
					draw = false;

					al_draw_bitmap(bettingBackground, 0, 0, NULL);
					al_draw_bitmap(horseFace01, 30.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace02, 130.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace03, 230.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace04, 330.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace05, 430.0f, 120.0f, NULL);
					al_draw_bitmap(horseFace06, 530.0f, 120.0f, NULL);

					for (i = 0; i < 6; i++) {
						for (j = 0; j < hLevel[i]; j++) {
							al_draw_filled_circle(30.0f + i * 100.0f + j * 10.0f, 190.0f, 3.0f, al_map_rgb(0, 0, 0));	//Show horses' level
						}
					}

					if (betHorse != -1) {
						al_draw_rectangle(28.0f + betHorse * 100, 118.0f, 112.0f + betHorse * 100, 182.0f, al_map_rgb(70, 200, 70), 4);
					}

					al_draw_textf(font1, al_map_rgb(0, 0, 0), 350.0f, 40.0f, ALLEGRO_ALIGN_LEFT, "%u", balance);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 40.0f, 290.0f, ALLEGRO_ALIGN_LEFT, "%u", bet);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 430.0f, 240.0f, ALLEGRO_ALIGN_LEFT, "%u", possibleProfit);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 310.0f, 270.0f, ALLEGRO_ALIGN_CENTER, "%u", add);

					al_flip_display();
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

	//Race layout
		if (layout == 4) {
			bool race = true;
			float hPosX[6], hPosY[6];
			float bPosY[6];

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
							printf("Race stopped\n");
							if (al_show_native_message_box(display, "Stop the race", "Your bet will be lost", "Are you sure you want to stop the race?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1){
								layout = 2;
								printf("Layout: 2\n");
								al_start_timer(timer);
							}
							else {
								al_start_timer(timer);
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

				//Every FPS events
				if (events.type == ALLEGRO_EVENT_TIMER) {
					for (i = 0; i < 6; i++) {
						hPosX[i] += (rand() % 2) * hLevel[i] * 0.02 + (rand() % 2) * 2;	//Horses' movement
					}
		

					draw = true;
				}
					
				j = 1;
				
				if (draw) {
					draw = false;

					

					if (race) {
						al_draw_bitmap(raceBackground, 0, 0, NULL);

						

						al_draw_filled_rectangle(20.0f, 142.0f + 30 * betHorse, 590.0f, 169.0f + 30 * betHorse, al_map_rgb(70, 200, 70));

						al_draw_bitmap(horse01, hPosX[0], hPosY[0], NULL);
						al_draw_bitmap(box1, 20, bPosY[0], NULL);
						al_draw_bitmap(horse02, hPosX[1], hPosY[1], NULL);
						al_draw_bitmap(box2, 20, bPosY[1], NULL);
						al_draw_bitmap(horse03, hPosX[2], hPosY[2], NULL);
						al_draw_bitmap(box3, 20, bPosY[2], NULL);
						al_draw_bitmap(horse04, hPosX[3], hPosY[3], NULL);
						al_draw_bitmap(box4, 20, bPosY[3], NULL);
						al_draw_bitmap(horse05, hPosX[4], hPosY[4], NULL);
						al_draw_bitmap(box5, 20, bPosY[4], NULL);
						al_draw_bitmap(horse06, hPosX[5], hPosY[5], NULL);
						al_draw_bitmap(box6, 20, bPosY[5], NULL);

						//Win event
						for (i = 0; i < 6; i++) {
							if (hPosX[i] >= 590) {
								race = false;

								al_draw_text(font1, al_map_rgb(0, 0, 0), 320.0f, 148.0f + i * 30.0f, ALLEGRO_ALIGN_CENTER, "W I N N E R");

								printf("***Horse0%i wins!***\n", i + 1);
								if (i == betHorse) {
									balance += possibleProfit;
								}
							}
						}
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
	al_destroy_font(font1);
	al_destroy_timer(timer);
	return 0;
}
