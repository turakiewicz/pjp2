#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

int i, j;

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
	al_init_acodec_addon();
	
//Installing
	if (!al_install_keyboard()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install keyboard!", NULL, NULL);
	}
	if (!al_install_mouse()) {
		al_show_native_message_box(NULL, NULL, NULL, "Cannot install mouse!", NULL, NULL);
	}
	al_install_audio();
//Main variables and functions
	int yon();
	int pom();
	void makeHorseSound(ALLEGRO_SAMPLE *horseSound);
	void exitGame(ALLEGRO_DISPLAY *display, int *layout, bool *gameDone);
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
	bool cheatMode = false, resetBettingLayout = false;
	int cheatBalance;
	bool bankruptcy = false;
	int *layoutPtr = &layout;
	bool *gameDonePtr = &gameDone;

	unsigned int statTotalRaces, statHighestBet, statMoneyWon, statMoneyLost, statHighestWin, statHighestLoss;
	float statAccuracy, statRacesWon, statRacesLost;
//Allegro pointers
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT events;
	ALLEGRO_DISPLAY *display = al_create_display(640, 360);
	al_set_window_title(display, "Gamble Ride");
	ALLEGRO_TRANSFORM camera;
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *h00Timer = al_create_timer(1.0 / hFPS[0]); //Horse00 animation timer
	ALLEGRO_TIMER *h01Timer = al_create_timer(1.0 / hFPS[1]); //Horse00 animation timer
	ALLEGRO_TIMER *h02Timer = al_create_timer(1.0 / hFPS[2]); //Horse00 animation timer
	ALLEGRO_TIMER *h03Timer = al_create_timer(1.0 / hFPS[3]); //Horse00 animation timer
	ALLEGRO_TIMER *h04Timer = al_create_timer(1.0 / hFPS[4]); //Horse00 animation timer
	ALLEGRO_TIMER *h05Timer = al_create_timer(1.0 / hFPS[5]); //Horse00 animation timer
	ALLEGRO_FONT *font18 = al_load_font("res/fonts/AmericanCaptain.ttf", 18, NULL);
	ALLEGRO_FONT *font20 = al_load_font("res/fonts/AmericanCaptain.ttf", 20, NULL);
	ALLEGRO_FONT *font25 = al_load_font("res/fonts/AmericanCaptain.ttf", 25, NULL);
	ALLEGRO_FONT *font30 = al_load_font("res/fonts/AmericanCaptain.ttf", 30, NULL);
	ALLEGRO_FONT *font60 = al_load_font("res/fonts/AmericanCaptain.ttf", 60, NULL);
	ALLEGRO_FONT *secondFont = al_load_font("res/fonts/DejaVuSerifBold.ttf", 15, NULL);
	ALLEGRO_FONT *signFont = al_load_font("res/fonts/MoonFlowerBold.ttf", 30, NULL);
	//SOUNDS
	ALLEGRO_SAMPLE *click = al_load_sample("res/sounds/click.wav");
	ALLEGRO_SAMPLE *gallop = al_load_sample("res/sounds/gallop.wav");
	ALLEGRO_SAMPLE *shoot = al_load_sample("res/sounds/shoot.wav");
	ALLEGRO_SAMPLE *click2 = al_load_sample("res/sounds/click2.wav");
	ALLEGRO_SAMPLE *click3 = al_load_sample("res/sounds/click3.wav");
	ALLEGRO_SAMPLE *horseSound = NULL;
	ALLEGRO_SAMPLE *raceSound = al_load_sample("res/sounds/race.wav");
	ALLEGRO_SAMPLE *cashSound = al_load_sample("res/sounds/cash.wav");
	ALLEGRO_SAMPLE *cheer = al_load_sample("res/sounds/cheer.wav");
	ALLEGRO_SAMPLE *failSound = al_load_sample("res/sounds/fail.wav");

	al_reserve_samples(8);

	//Start layout graphics
	ALLEGRO_BITMAP *startBackground = al_load_bitmap("res/graphics/start/background.png");
	ALLEGRO_BITMAP *highlightBig = al_load_bitmap("res/graphics/start/highlightBig.png");
	ALLEGRO_BITMAP *highlightSmall = al_load_bitmap("res/graphics/start/highlightSmall.png");
	//Betting layout graphics
	ALLEGRO_BITMAP *bettingBackground = al_load_bitmap("res/graphics/betting/background.png");
	ALLEGRO_BITMAP *raceGreyOut = al_load_bitmap("res/graphics/betting/raceGreyOut.png");
	ALLEGRO_BITMAP *takeMoneyPopupBackground = al_load_bitmap("res/graphics/betting/popup.png");
	ALLEGRO_BITMAP *blackout = al_load_bitmap("res/graphics/betting/blackout.png");
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
	//End Career layout
	ALLEGRO_BITMAP *endCareerBackground = al_load_bitmap("res/graphics/endCareer/background.png");
//Registering event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
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
		printf("scoreList succesfully loaded.\n", NULL);
	}
	//Creating scoreList.txt if it doesn't exist
	else {
		printf("Cannot load scoreList.txt\n", NULL);
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
		printf("scoreList.txt template generated\n", NULL);
	}
	//Divide scoreList[100] into smaller arrays and integers
	char podiumName1[10];
	char podiumName2[10];
	char podiumName3[10];
	char podiumName4[10];
	char podiumName5[10];
	
	for (i = 0; i < 10; i++) {
		if (scoreList[i] == '&') {
			podiumName1[i] = ' ';
		} else {
			podiumName1[i] = scoreList[i];
		}

		if (scoreList[i + 20] == '&') {
			podiumName2[i] = ' ';
		} else {
			podiumName2[i] = scoreList[i + 20];
		}

		if (scoreList[i + 40] == '&') {
			podiumName3[i] = ' ';
		} else {
			podiumName3[i] = scoreList[i + 40];
		}

		if (scoreList[i + 60] == '&') {
			podiumName4[i] = ' ';
		} else {
			podiumName4[i] = scoreList[i + 60];
		}

		if (scoreList[i + 80] == '&') {
			podiumName5[i] = ' ';
		} else {
			podiumName5[i] = scoreList[i + 80];
		}
	}

	unsigned long long int k;
	unsigned long long int podiumScore[] = {0, 0, 0, 0, 0, 0, 0};	//podiumScore[0] isn't used in program
	j = 0;
	k = 1;
	while (j < 10) {
		podiumScore[1] += (scoreList[0 * 20 + 10 + j] - 48) * k;
		podiumScore[2] += (scoreList[1 * 20 + 10 + j] - 48) * k;
		podiumScore[3] += (scoreList[2 * 20 + 10 + j] - 48) * k;
		podiumScore[4] += (scoreList[3 * 20 + 10 + j] - 48) * k;
		podiumScore[5] += (scoreList[4 * 20 + 10 + j] - 48) * k;
		j += 1;
		k *= 10;
	}

	

//Game loop
	while (!gameDone) {



	//Start screen layout
		if (layout == 1) {
			balance = 1000;
			add = 100;
			bankruptcy = false;
			statTotalRaces = 0;
			statHighestBet = 0;
			statAccuracy = 0;
			statMoneyWon = 0;
			statMoneyLost = 0;
			statRacesWon = 0;
			statRacesLost = 0;
			statHighestWin = 0;
			statHighestLoss = 0;

			bool highlightExitDraw = false, highlightStartDraw = false, highlightInstructionsDraw = false;

			//CHEAT MODE ADDITION
			if (resetBettingLayout) {
				resetBettingLayout = false;
				balance = cheatBalance;
				layout = 2;
			}

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
				else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					exitGame(display, layoutPtr, gameDonePtr);
				}
				else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					//Exit button
					if (events.mouse.button & 1 && events.mouse.x >= 40 && events.mouse.x <= 160 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						layout = -1;
						gameDone = true;
					}
					//Start button
					if (events.mouse.button & 1 && events.mouse.x >= 200 && events.mouse.x <= 440 && events.mouse.y >= 220 && events.mouse.y <= 320) {
						layout = 2;
						printf("Layout: 2\n", NULL);
						al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					//Instructions button
					if (events.mouse.button & 1 && events.mouse.x >= 480 && events.mouse.x <= 600 && events.mouse.y >= 240 && events.mouse.y <= 300) {
						layout = 3;
						printf("Layout: 3\n", NULL);
						al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
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
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 64.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore[1]);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 88.0f, ALLEGRO_ALIGN_RIGHT, podiumName2);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 88.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore[2]);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 112.0f, ALLEGRO_ALIGN_RIGHT, podiumName3);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 112.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore[3]);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 136.0f, ALLEGRO_ALIGN_RIGHT, podiumName4);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 136.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore[4]);
					al_draw_text(font20, al_map_rgb(0, 0, 0), 310.0f, 160.0f, ALLEGRO_ALIGN_RIGHT, podiumName5);
					al_draw_textf(font20, al_map_rgb(0, 0, 0), 330.0f, 160.0f, ALLEGRO_ALIGN_LEFT, "%llu", podiumScore[5]);

					

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
			bool takeMoneyPopup = false, drawTakeMoneyPopup = false, proceed = false, saveScore = false, drawSingPlace = false;
			int playerPodiumPosition = 6;
			char playerName[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
			int currentLetter = 0;
			enum writingDirectionEnum { forward, backward };
			enum writingDirectionEnum writingDirection = forward;
			bool letterClicked = false;
			char letter;

			for (i = 0; i < 6; i++) {
				hLevel[i] = rand() % 6 + 1;	//Setting random horse levels
				//printf(">horse0%i - %i\n", i + 1, hLevel[i]);
			}

			//Calculate player podium position
			for (i = 5; i >= 1; i--) {
				if (balance + bet > podiumScore[i]) {
					playerPodiumPosition = i;
				}
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

					if (takeMoneyPopup) {
						drawTakeMoneyPopup = true;
					}
				}

				al_wait_for_event(event_queue, &events);
				if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
					//Exit
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						if (al_show_native_message_box(display, "END CAREER", "Your progress will be lost", "End career?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1){
							layout = 1;
							al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
						}
					}

					//CHEAT MODE ('M' to add 1000 to balance, 'R' to restart random horses)
					if (!cheatMode) {
						if (events.keyboard.keycode == ALLEGRO_KEY_TILDE) {
							cheatMode = true;
						}
					}
					else {
						
						

						if (events.keyboard.keycode == ALLEGRO_KEY_TILDE) {
							cheatMode = false;
						}
						else if (events.keyboard.keycode == ALLEGRO_KEY_M) {
							balance += 1000;
						}
						else if (events.keyboard.keycode == ALLEGRO_KEY_R) {
							cheatBalance = balance;
							resetBettingLayout = true;
							layout = 1;
						}
						
						


						for (i = 5; i >= 1; i--) {
							if (balance + bet > podiumScore[i]) {
								playerPodiumPosition = i;
							}
						}
					}

					//Keyboard
					if (drawSingPlace && takeMoneyPopup) {
						if (currentLetter < 10) {
							switch (events.keyboard.keycode) {
							case ALLEGRO_KEY_A: letter = 'A';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_B: letter = 'B';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_C: letter = 'C';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_D: letter = 'D';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_E: letter = 'E';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_F: letter = 'F';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_G: letter = 'G';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_H: letter = 'H';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_I: letter = 'I';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_J: letter = 'J';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_K: letter = 'K';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_L: letter = 'L';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_M: letter = 'M';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_N: letter = 'N';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_O: letter = 'O';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_P: letter = 'P';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_Q: letter = 'Q';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_R: letter = 'R';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_S: letter = 'S';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_T: letter = 'T';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_U: letter = 'U';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_V: letter = 'V';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_W: letter = 'W';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_X: letter = 'X';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_Y: letter = 'Y';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_Z: letter = 'Z';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_0: letter = '0';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_1: letter = '1';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_2: letter = '2';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_3: letter = '3';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_4: letter = '4';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_5: letter = '5';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_6: letter = '6';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_7: letter = '7';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_8: letter = '8';
								letterClicked = true;
								break;
							case ALLEGRO_KEY_9: letter = '9';
								letterClicked = true;
								break;
							default:
								break;
							}

							if (letterClicked) {
								letterClicked = false;
								printf("Letter clicked\n", NULL);

								if (currentLetter <= 9) {
									if (writingDirection == forward) {
										playerName[currentLetter] = letter;
										currentLetter++;
									}
									else if (writingDirection == backward) {
										playerName[currentLetter + 1] = letter;
										writingDirection = forward;
										currentLetter += 2;
									}
								}

								
								
							}
						}
						if (currentLetter >= 0 && takeMoneyPopup) {
							if (events.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {

								if (currentLetter >= 0) {
									if (writingDirection == backward) {
										playerName[currentLetter] = ' ';
										currentLetter--;
									}
									else if (writingDirection == forward) {
										playerName[currentLetter - 1] = ' ';
										writingDirection = backward;
										currentLetter -= 2;
									}
								}


								
							}
						}
						printf("Current letter: %i\n", currentLetter);
					}
				}
				else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					exitGame(display, layoutPtr, gameDonePtr);
				}
				else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					//Left mouse button clicked
					if (events.mouse.button & 1) {
						if (!takeMoneyPopup) {
							//Begin race button
							if (events.mouse.x >= 430 && events.mouse.x <= 620 && events.mouse.y >= 260 && events.mouse.y <= 340 &&
								bet > 0 && betHorse != -1) {
								layout = 4;
								al_play_sample(raceSound, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								printf("Layout: 4\n", NULL);
							}
							//Change bet addition
							if (events.mouse.y >= 270 && events.mouse.y <= 290) {
								//Multiple by 10
								if (events.mouse.x >= 340 && events.mouse.x <= 360 &&
									add * 10 <= balance + bet) {
									add *= 10;
									al_play_sample(click3, 1.5f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								}
								//Divide by 10
								if (events.mouse.x >= 260 && events.mouse.x <= 280 &&
									add > 1) {
									add /= 10;
									al_play_sample(click3, 1.5f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								}
							}
							//Change bet
							if (events.mouse.x >= 290 && events.mouse.x <= 330) {
								//Add
								if (events.mouse.y >= 290 && events.mouse.y <= 310 &&
									balance >= add) {
									bet += add;
									balance -= add;
									al_play_sample(click2, 2.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								}
								//Subtract
								if (events.mouse.y >= 310 && events.mouse.y <= 330 &&
									bet >= add) {
									bet -= add;
									balance += add;
									al_play_sample(click2, 2.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								}
							}

							//Horse choose buttons
							if (events.mouse.y >= 120 && events.mouse.y <= 180) {
								//Horse 1
								if (events.mouse.x >= 30 && events.mouse.x <= 110) {
									betHorse = 0;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
								//Horse 2
								if (events.mouse.x >= 130 && events.mouse.x <= 210) {
									betHorse = 1;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
								//Horse 3
								if (events.mouse.x >= 230 && events.mouse.x <= 310) {
									betHorse = 2;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
								//Horse 4
								if (events.mouse.x >= 330 && events.mouse.x <= 410) {
									betHorse = 3;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
								//Horse 5
								if (events.mouse.x >= 430 && events.mouse.x <= 510) {
									betHorse = 4;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
								//Horse 6
								if (events.mouse.x >= 530 && events.mouse.x <= 610) {
									betHorse = 5;
									printf("horse0%i choosen\n", betHorse + 1);
									makeHorseSound(horseSound);
								}
							}
						}

						

						//Take money button
						if (events.mouse.x >= 340 && events.mouse.x <= 580 && events.mouse.y >= 80 && events.mouse.y <= 100) {
							takeMoneyPopup = true;
							proceed = false;
							al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

							
							printf("Player's podium position: %i\n", playerPodiumPosition);

							if (playerPodiumPosition <= 5) {
								drawSingPlace = true;
								saveScore = true;
							}
						}
						if (takeMoneyPopup) {

							//Close button
							if (events.mouse.x >= 420 && events.mouse.x <= 440 && events.mouse.y >= 120 && events.mouse.y <= 140) {
								takeMoneyPopup = false;
							}
							//Proceed button
							else if ((!saveScore && (events.mouse.x >= 260 && events.mouse.x <= 380 && events.mouse.y >= 210 && events.mouse.y <= 240)) ||
								(playerName[0] != ' ' && (events.mouse.x >= 260 && events.mouse.x <= 380 && events.mouse.y >= 210 && events.mouse.y <= 240))) {
								proceed = true;
								takeMoneyPopup = false;

								if (saveScore) {
									saveScore = false;
									al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
									printf("SCORE SAVE\n", NULL);
									switch (playerPodiumPosition) {
									case 5:
										podiumScore[5] = balance + bet;
										for (i = 0; i < 10; i++) {
											podiumName5[i] = playerName[i];
										}
										break;
									case 4:
										podiumScore[5] = podiumScore[4];
										podiumScore[4] = balance + bet;
										for (i = 0; i < 10; i++) {
											podiumName5[i] = podiumName4[i];
											podiumName4[i] = playerName[i];
										}
										break;
									case 3:
										podiumScore[5] = podiumScore[4];
										podiumScore[4] = podiumScore[3];
										podiumScore[3] = balance + bet;
										for (i = 0; i < 10; i++) {
											podiumName5[i] = podiumName4[i];
											podiumName4[i] = podiumName3[i];
											podiumName3[i] = playerName[i];
										}
										break;
									case 2:
										podiumScore[5] = podiumScore[4];
										podiumScore[4] = podiumScore[3];
										podiumScore[3] = podiumScore[2];
										podiumScore[2] = balance + bet;
										for (i = 0; i < 10; i++) {
											podiumName5[i] = podiumName4[i];
											podiumName4[i] = podiumName3[i];
											podiumName3[i] = podiumName2[i];
											podiumName2[i] = playerName[i];
										}
										break;
									case 1:
										podiumScore[5] = podiumScore[4];
										podiumScore[4] = podiumScore[3];
										podiumScore[3] = podiumScore[2];
										podiumScore[2] = podiumScore[1];
										podiumScore[1] = balance + bet;
										for (i = 0; i < 10; i++) {
											podiumName5[i] = podiumName4[i];
											podiumName4[i] = podiumName3[i];
											podiumName3[i] = podiumName2[i];
											podiumName2[i] = podiumName1[i];
											podiumName1[i] = playerName[i];
										}
										break;
									default:
										break;
									}

									FILE *fp1;
									fp1 = fopen("res/saves/scoreList.txt", "w");

									char temporaryScore[11];
									char temporaryScoreReversed[10];
									int temporaryScoreLength;
									
									for (i = 0; i < 5; i++) {
										sprintf(temporaryScore, "%lli", podiumScore[i + 1]);
										temporaryScoreLength = numberOfDigits(podiumScore[i + 1]);
										//printf("TemScLen[%i] = %i\n", i, temporaryScoreLength);

										for (j = 0; j < 10; j++) {
											temporaryScoreReversed[j] = temporaryScore[temporaryScoreLength - j - 1];
										}
										for (j = temporaryScoreLength + 1; j < 10; j++) {
											temporaryScoreReversed[j] = '0';
										}
										for (j = 0; j < 10; j++) {
											//scoreList[i * 20 + 10 + j] = temporaryScoreReversed[j];
											switch (temporaryScoreReversed[j]) {
											case '0': scoreList[i * 20 + 10 + j] = '0';
												break;
											case '1': scoreList[i * 20 + 10 + j] = '1';
												break;
											case '2': scoreList[i * 20 + 10 + j] = '2';
												break;
											case '3': scoreList[i * 20 + 10 + j] = '3';
												break;
											case '4': scoreList[i * 20 + 10 + j] = '4';
												break;
											case '5': scoreList[i * 20 + 10 + j] = '5';
												break;
											case '6': scoreList[i * 20 + 10 + j] = '6';
												break;
											case '7': scoreList[i * 20 + 10 + j] = '7';
												break;
											case '8': scoreList[i * 20 + 10 + j] = '8';
												break;
											case '9': scoreList[i * 20 + 10 + j] = '9';
												break;
											}
											
											printf("%c", temporaryScoreReversed[j]);
										}

									}

									for (i = 0; i < 10; i++) {
										if (podiumName1[i] != ' ') {
											scoreList[i] = podiumName1[i];
										} else {
											scoreList[i] = '&';
										}

										if (podiumName2[i] != ' ') {
											scoreList[i + 20] = podiumName2[i];
										} else {
											scoreList[i + 20] = '&';
										}

										if (podiumName3[i] != ' ') {
											scoreList[i + 40] = podiumName3[i];
										} else {
											scoreList[i + 40] = '&';
										}

										if (podiumName4[i] != ' ') {
											scoreList[i + 60] = podiumName4[i];
										} else {
											scoreList[i + 60] = '&';
										}

										if (podiumName5[i] != ' ') {
											scoreList[i + 80] = podiumName5[i];
										} else {
											scoreList[i + 80] = '&';
										}
										
									}

									
									
									for (i = 0; i < 100; i++) {
										fprintf(fp1, "%c", scoreList[i]);
									}
									fclose(fp1);

									layout = 5;
								}
								else {
									layout = 5;
									al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
								}
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

					if (playerPodiumPosition <= 5) {
						al_draw_textf(font20, al_map_rgb(5, 30, 17), 590.0f, 83.0f, ALLEGRO_ALIGN_CENTER, "%i.", playerPodiumPosition);
					}

					if (cheatMode) {
						al_draw_text(font20, al_map_rgb(255, 0, 0), 10.0f, 335.0f, ALLEGRO_ALIGN_LEFT, "CHEAT MODE IS ACTIVE");
					}

					if (drawTakeMoneyPopup) {
						drawTakeMoneyPopup = false;

						al_draw_bitmap(blackout, 0, 0, NULL);
						al_draw_bitmap(takeMoneyPopupBackground, al_get_display_width(display) / 2 - al_get_bitmap_width(takeMoneyPopupBackground) / 2, al_get_display_height(display) / 2 - al_get_bitmap_height(takeMoneyPopupBackground) / 2, NULL);
					
						if (drawSingPlace) {
							al_draw_text(secondFont, al_map_rgb(123, 112, 96), 310.0f, 120.0f, ALLEGRO_ALIGN_CENTER, "Change money into glory");
							al_draw_text(font25, al_map_rgb(0, 0, 0), 260.0f, 145.0f, ALLEGRO_ALIGN_CENTER, "SIGN AND GET");
							al_draw_textf(font25, al_map_rgb(174, 0, 0), 325.0f, 145.0f, ALLEGRO_ALIGN_CENTER, "%i.", playerPodiumPosition);
							al_draw_text(font25, al_map_rgb(0, 0, 0), 385.0f, 145.0f, ALLEGRO_ALIGN_CENTER, "HIGH SCORE!");

							al_draw_text(font20, al_map_rgb(0, 0, 0), 315.0f, 190.0f, ALLEGRO_ALIGN_CENTER, ". . . . . . . . . . . . . . . . . . . .");
							al_draw_text(signFont, al_map_rgb(0, 0, 0), 265.0f, 175.0f, ALLEGRO_ALIGN_LEFT, playerName);
						}
						else {
							al_draw_text(secondFont, al_map_rgb(123, 112, 96), 320.0f, 120.0f, ALLEGRO_ALIGN_CENTER, "Oops!");
							al_draw_text(font18, al_map_rgb(0, 0, 0), 320.0f, 142.0f, ALLEGRO_ALIGN_CENTER, "NOT ENOUGH MONEY");
							al_draw_text(font18, al_map_rgb(0, 0, 0), 320.0f, 161.0f, ALLEGRO_ALIGN_CENTER, "TO GET TO THE TOP LIST");
							al_draw_text(font30, al_map_rgb(174, 0, 0), 320.0f, 181.0f, ALLEGRO_ALIGN_CENTER, "E N D   G A M E ?");
						}
					}


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
					al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
					printf("Layout: 1\n", NULL);
				}
			}
			else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				exitGame(display, layoutPtr, gameDonePtr);
			}
			else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && events.mouse.button & 1) {
				layout = 1;
				al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
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

			statTotalRaces++;
			if (bet > statHighestBet) {
				statHighestBet = bet;
			}

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
							printf("Layout: 2\n", NULL);
						}
						else {
							al_stop_timer(timer);
							al_stop_timer(h00Timer);
							al_stop_timer(h01Timer);
							al_stop_timer(h02Timer);
							al_stop_timer(h03Timer);
							al_stop_timer(h04Timer);
							al_stop_timer(h05Timer);
							printf("Race stopped\n", NULL);
							if (al_show_native_message_box(display, "Stop the race", "Your bet will be lost", "Are you sure you want to stop the race?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1){
								layout = 2;
								printf("Layout: 2\n", NULL);
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
								printf("Race started\n", NULL);
							}
						}
					}
				}
				else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					exitGame(display, layoutPtr, gameDonePtr);
				}
				else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					if (events.mouse.button & 1 && !race) {
						if (!bankruptcy) {
							layout = 2;
						}
						else {
							layout = 5;
						}
					}
				}

				if (events.type == ALLEGRO_EVENT_TIMER) {
					if (race) {
						//Every FPS events
						if (events.timer.source == timer) {
							for (i = 0; i < 6; i++) {
								hVelocity[i] += yon() * hLevel[i] * 0.003 + pom() * 0.03 + pom() * 0.03;	//Horses movement

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

								al_stop_samples();

								al_draw_filled_rectangle(offset, 140.0f + 30 * i, 640 + offset, 170.0f + 30 * i, al_map_rgb(172, 129, 25));
								al_draw_text(font25, al_map_rgb(248, 187, 40), 320.0f + offset, 145.0f + i * 30.0f, ALLEGRO_ALIGN_CENTER, "W I N N E R");

								al_draw_text(font20, al_map_rgb(32, 75, 31), 320.0f + offset, 333.0f, ALLEGRO_ALIGN_CENTER, "Click anywhere to continue...");

								printf("***Horse0%i wins!***\n", i + 1);
								if (i == betHorse) {
									statMoneyWon += possibleProfit;
									statRacesWon++;
									if (possibleProfit > statHighestWin) {
										statHighestWin = possibleProfit;
									}

									balance += possibleProfit;
									
									//al_play_sample(cheer, 2.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
									al_play_sample(cashSound, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

									al_draw_text(font60, al_map_rgb(174, 0, 0), 120.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "YOU WON");
									al_draw_textf(font60, al_map_rgb(172, 129, 25), 310.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "%u", possibleProfit);
								}
								else {
									al_draw_text(font60, al_map_rgb(174, 0, 0), 120.0f + offset, 27.0f, ALLEGRO_ALIGN_LEFT, "YOU LOST");
									al_play_sample(failSound, 0.9f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

									statMoneyLost += bet;
									statRacesLost++;
									if (bet > statHighestLoss) {
										statHighestLoss = bet;
									}

									if (balance <= 0) {
										bankruptcy = true;
									}
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
						int animSpeedVariable, animSpeedConstant;
						animSpeedConstant = 5;
						animSpeedVariable = 9;

						//Horse01 animation
						al_draw_bitmap_region(horse01, hSourceX[0], hSourceY[0], 40, 40, hPosX[0], hPosY[0], NULL);
						hFPS[0] = animSpeedConstant + hVelocity[0] * animSpeedVariable;

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
						hFPS[1] = animSpeedConstant + hVelocity[1] * animSpeedVariable;

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
						hFPS[2] = animSpeedConstant + hVelocity[2] * animSpeedVariable;

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
						hFPS[3] = animSpeedConstant + hVelocity[3] * animSpeedVariable;

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
						hFPS[4] = animSpeedConstant + hVelocity[4] * animSpeedVariable;

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
						hFPS[5] = animSpeedConstant + hVelocity[5] * animSpeedVariable;

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
					
						if (bankruptcy) {
							puts("Bank");
							al_draw_bitmap(blackout, 0 + offset, 0, NULL);
							al_draw_bitmap(blackout, 0 + offset, 0, NULL);
							al_draw_text(font60, al_map_rgb(100, 0, 0), 320.0f + offset + 2, 160.0f + 2, ALLEGRO_ALIGN_CENTER, "BANKRUPTCY!");
							al_draw_text(font60, al_map_rgb(255, 0, 0), 320.0f + offset, 160.0f, ALLEGRO_ALIGN_CENTER, "BANKRUPTCY!");
						}

					}

					al_flip_display();
				}				
			}
		}

	//End Career layout
		if (layout == 5) {
			puts(">LAYOUT 5\n");

			if (statRacesWon + statRacesLost > 0) {
				statAccuracy = statRacesWon * 100 / (statRacesWon + statRacesLost);
			}
			else {
				statAccuracy = 0;
			}

			//Set camera to default position
			al_identity_transform(&camera);
			al_translate_transform(&camera, 0, 0);
			al_use_transform(&camera);

			while (layout == 5) {
				if (events.type == ALLEGRO_EVENT_TIMER) {
					draw = true;
				}

				al_wait_for_event(event_queue, &events);
				if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						layout = 1;
						al_play_sample(click, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}
				else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					exitGame(display, layoutPtr, gameDonePtr);
				}

				if (draw) {
					draw = false;

					al_draw_bitmap(endCareerBackground, 0, 0, NULL);
					al_draw_textf(font60, al_map_rgb(152, 109, 5), 450.0f + 2, 46.0f + 2, ALLEGRO_ALIGN_CENTER, "%u", balance);
					al_draw_textf(font60, al_map_rgb(172, 129, 25), 450.0f, 46.0f, ALLEGRO_ALIGN_CENTER, "%u", balance);

					al_draw_textf(font30, al_map_rgb(0, 0, 0), 170.0f, 147.0f, ALLEGRO_ALIGN_CENTER, "%u", statTotalRaces);
					al_draw_textf(font30, al_map_rgb(0, 0, 0), 360.0f, 147.0f, ALLEGRO_ALIGN_CENTER, "%u", statHighestBet);
					al_draw_textf(font30, al_map_rgb(0, 0, 0), 510.0f, 147.0f, ALLEGRO_ALIGN_CENTER, "%.1f", statAccuracy);

					al_draw_textf(font25, al_map_rgb(68, 124, 50), 190.0f, 203.0f, ALLEGRO_ALIGN_CENTER, "%u", statMoneyWon);
					al_draw_textf(font25, al_map_rgb(121, 0, 0), 450.0f, 203.0f, ALLEGRO_ALIGN_CENTER, "%u", statMoneyLost);

					al_draw_textf(font20, al_map_rgb(68, 124, 50), 190.0f, 246.0f, ALLEGRO_ALIGN_CENTER, "%.0f", statRacesWon);
					al_draw_textf(font20, al_map_rgb(121, 0, 0), 450.0f, 246.0f, ALLEGRO_ALIGN_CENTER, "%.0f", statRacesLost);

					al_draw_textf(font18, al_map_rgb(68, 124, 50), 190.0f, 284.0f, ALLEGRO_ALIGN_CENTER, "%u", statHighestWin);
					al_draw_textf(font18, al_map_rgb(121, 0, 0), 450.0f, 284.0f, ALLEGRO_ALIGN_CENTER, "%u", statHighestLoss);

					al_draw_text(font20, al_map_rgb(63, 115, 87), 320.0f, 328.0f, ALLEGRO_ALIGN_CENTER, "Click esc to continue...");
					

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
	al_destroy_bitmap(takeMoneyPopupBackground);
	al_destroy_bitmap(blackout);
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
	al_destroy_font(font18);
	al_destroy_font(font20);
	al_destroy_font(font25);
	al_destroy_font(font30);
	al_destroy_font(font60);
	al_destroy_font(secondFont);
	al_destroy_font(signFont);
	al_destroy_sample(click);
	al_destroy_sample(click2);
	al_destroy_sample(click3);
	al_destroy_sample(gallop);
	al_destroy_sample(shoot);
	al_destroy_sample(horseSound);
	al_destroy_sample(raceSound);
	al_destroy_sample(cashSound);
	al_destroy_sample(cheer);
	al_destroy_sample(failSound);
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

int numberOfDigits(long long int i)
{
	return i > 0 ? (int)log10((double)i) + 1 : 1;
}

void makeHorseSound(ALLEGRO_SAMPLE *horseSound) {
	int i;
	i = rand() % 4;

	switch (i) {
	case 0: horseSound = al_load_sample("res/sounds/horse1.wav");
		break;
	case 1: horseSound = al_load_sample("res/sounds/horse2.wav");
		break;
	case 2: horseSound = al_load_sample("res/sounds/horse3.wav");
		break;
	case 3: horseSound = al_load_sample("res/sounds/horse4.wav");
		break;
	}

	al_play_sample(horseSound, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
}

void exitGame(ALLEGRO_DISPLAY *display, int *layout, bool *gameDone) {
	if (al_show_native_message_box(display, "EXIT GAME", "You are about to exit the game", "Are you sure?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1){
		*layout = -1;
		*gameDone = true;
		puts("Exit true");
	}
}
