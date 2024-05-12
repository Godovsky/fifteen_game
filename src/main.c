#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#ifdef __WINDOWS__
#define MAIN WinMain
#elif __LINUX__
#define MAIN main
#endif

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
SDL_Texture * fifteenTexture = NULL;
SDL_Texture * backTexture = NULL;
SDL_Texture * upTexture = NULL;
SDL_Rect renRect = {0,0,100,100};
SDL_Rect fifteenRect = {0,0,240,240};

#define WID 420
#define HEI 420

int MAIN (int argc, char * argv[])
{
	if (SDL_Init (SDL_INIT_VIDEO) == -1)
	{
		printf ("Could not initialize SDL: %s.\n", SDL_GetError ());
		return 1;
	}
	if (IMG_Init (IMG_INIT_PNG) == -1)
	{
		SDL_Quit ();
		printf ("Could not initialize SDL image: %s.\n", SDL_GetError ());
		return 1;
	}
	if (TTF_Init () == -1)
	{
		SDL_Quit ();
		IMG_Quit ();
		printf ("Could not initialize SDL_ttf: %s.\n", SDL_GetError ());
		return 1;
	}
	if (Mix_Init (MIX_INIT_MP3) == -1)
	{
		SDL_Quit ();
		IMG_Quit ();
		TTF_Quit ();
		printf ("Could not initialize SDL mixer: %s.\n", SDL_GetError ());
		return 1;
	}
	
	/* Сreating a window */
	window = SDL_CreateWindow ("15", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								WID, HEI, SDL_WINDOW_SHOWN);
	if (NULL == window)
	{
		printf ("Failed to create a window!\n\n");
		SDL_Quit ();
		IMG_Quit ();
		system ("pause");
		return 1;
	}
	
	/* Сreating a renderer */
	renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
	if (NULL == renderer)
	{
		printf ("Failed to create a renderer!\n\n");
		SDL_DestroyWindow (window);
		SDL_Quit ();
		IMG_Quit ();
		system ("pause");
		return 1;
	}
	
	/* Сreating a texture */
	fifteenTexture = IMG_LoadTexture (renderer, "textures/1-15.png");
	backTexture = IMG_LoadTexture (renderer, "textures/back.png");
	upTexture = IMG_LoadTexture (renderer, "textures/up.png");
	if (NULL == fifteenTexture || NULL == backTexture || NULL == upTexture)
	{
		printf ("Failed to create a texture!\n\n");
		SDL_DestroyRenderer (renderer);
		SDL_DestroyWindow (window);
		SDL_Quit ();
		IMG_Quit ();
		system ("pause");
		return 1;
	}
	Mix_Music * sounds[3] = {NULL, NULL, NULL};
	if (Mix_OpenAudio (MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		printf ("Failed to open audio!\n\n");
	}
	else
	{
		Mix_VolumeMusic(16);
		sounds[0] = Mix_LoadMUS("sounds/1.mp3");
		sounds[1] = Mix_LoadMUS("sounds/2.mp3");
		sounds[2] = Mix_LoadMUS("sounds/3.mp3");
		for (int i = 0; i < 3; i++)
			if (sounds[i] == NULL) printf ("Failed to open sound %d!\n\n", i);
	}
	
	SDL_Surface * textSurface = NULL;
	SDL_Texture * textTexture = NULL;
	SDL_Rect textRect = {0};
	TTF_Font * font = TTF_OpenFont("fonts/arialregular.ttf", 90);
	SDL_Color color = {0xFF,0x00,0x00,0xFF};
	textSurface = TTF_RenderText_Solid(font, "DONE!", color);
	textTexture = SDL_CreateTextureFromSurface (renderer, textSurface);
	if (NULL == font || NULL == textTexture || NULL == textSurface)
		printf ("Font error!\n\n");
	SDL_GetClipRect(textSurface, &textRect);
	SDL_FreeSurface(textSurface);
	textRect.x = WID/2 - textRect.w/2;
	textRect.y = HEI/2 - textRect.h/2;
	// printf ("%d %d\n", textRect.x, textRect.y);
	uint8_t doneFlag = 0;
	
	uint8_t breakWhile1 = 1;
	uint8_t breakWhile2 = 1;
	while (breakWhile1)
	{
		uint8_t arr[4][4] = {0};
		srand (time (NULL));
		uint8_t value = 0;
		uint8_t index = rand () % 16;
		uint8_t row = -1, col = -1;
		doneFlag = 0;
		for (int i = 0; i < 16; i++)
		{
			
			if(((uint8_t *)arr)[index] == 0)
				((uint8_t *)arr)[index] = value++;
			else 
			{
				index = rand() % 16;
				i--;
			}
		}
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (arr[i][j] == 0)
				{
					row = i;
					col = j;
				}
			}
		}

		SDL_Event event;
		
		SDL_RenderCopy (renderer, upTexture, NULL, NULL);
		SDL_RenderPresent (renderer);
		SDL_WaitEvent (&event);
		switch (event.type)
		{
			case SDL_QUIT:
				breakWhile1 = 0;
				breakWhile2 = 0;
				continue;
			case SDL_KEYDOWN:
				continue;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					breakWhile1 = 0;
					breakWhile2 = 0;
					continue;
				}
				else if (event.key.keysym.sym == 13)
				{
					breakWhile1 = 1;
					breakWhile2 = 1;
				}
				break;
			default:
				SDL_Delay (50);
				continue;
		}
		while(breakWhile2)
		{
			SDL_RenderCopy (renderer, backTexture, NULL, NULL);
			for (int i = 0, k = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++, k++)
				{
					switch (arr[i][j])
					{
						case 1:
							fifteenRect.x = 0;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 2:
							fifteenRect.x = 240;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 3:
							fifteenRect.x = 240*2;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 4:
							fifteenRect.x = 240*3;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 5:
							fifteenRect.x = 240*4;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 6:
							fifteenRect.x = 240*5;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 7:
							fifteenRect.x = 240*6;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 8:
							fifteenRect.x = 240*7;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 9:
							fifteenRect.x = 240*8;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 10:
							fifteenRect.x = 240*9;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 11:
							fifteenRect.x = 240*10;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 12:
							fifteenRect.x = 240*11;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 13:
							fifteenRect.x = 240*12;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 14:
							fifteenRect.x = 240*13;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 15:
							fifteenRect.x = 240*14;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
						case 0:
							fifteenRect.x = 240*15;
							renRect.x = 100*j + 10;
							renRect.y = 100*i + 10;
							break;
					}
					SDL_RenderCopy (renderer, fifteenTexture, &fifteenRect, &renRect);
				}
			}
			if (doneFlag == 1) SDL_RenderCopy (renderer, textTexture, NULL, &textRect);
			SDL_RenderPresent (renderer);
			
			uint8_t tmp = 0;
			SDL_WaitEvent (&event);
			switch (event.type)
			{
				case SDL_QUIT:
					breakWhile1 = 0;
					breakWhile2 = 0;
					continue;
				case SDL_KEYDOWN:
					continue;
				case SDL_KEYUP:
					tmp = 0;
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						breakWhile2 = 0;
						continue;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (row-1 >= 0)
						{
							tmp = arr[row][col];
							arr[row][col] = arr[row-1][col];
							arr[row-1][col] =tmp;
							row--;
							Mix_PlayMusic (sounds[rand () % 3], 1);
						}
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						if (row+1 <= 3)
						{
							tmp = arr[row][col];
							arr[row][col] = arr[row+1][col];
							arr[row+1][col] =tmp;
							row++;
							Mix_PlayMusic (sounds[rand () % 3], 1);
						}
					}
					else if (event.key.keysym.sym == SDLK_LEFT)
					{
						if (col+1 <= 3)
						{
							tmp = arr[row][col];
							arr[row][col] = arr[row][col+1];
							arr[row][col+1] =tmp;
							col++;
							Mix_PlayMusic (sounds[rand () % 3], 1);
						}
					}
					else if (event.key.keysym.sym == SDLK_RIGHT)
					{
						if (col-1 >= 0)
						{
							tmp = arr[row][col];
							arr[row][col] = arr[row][col-1];
							arr[row][col-1] =tmp;
							col--;
							Mix_PlayMusic (sounds[rand () % 3], 1);
						}
					}
					break;
					// break;
				// case SDL_MOUSEBUTTONDOWN:
					// break;
				// case SDL_MOUSEBUTTONUP:
					// break;
				// case SDL_MOUSEMOTION:
					// break;
				default:
					SDL_Delay (50);
					continue;
			}

			int8_t l = 1;
			for (int i = 0; i < 15; i++)
			{
				if (l != ((uint8_t *)arr)[i])
				{
					break;
				}
				else l++;
				printf ("l %d %d\n", l, ((uint8_t *)arr)[i]);
			}
			int8_t k = 1;
			for (int i = 3; i >= 0; i--)
			{
				int j;
				for (j = 0; j < 4; j++)
				{
					// printf ("%d %d\n", k, arr[j][i]);
					if (k != arr[j][i])
					{
						break;
					}
					else k++;
				}
				// printf ("k %d %d\n", k, arr[j][i]);
			}
			if (k == 16 || l == 16)
			{
				doneFlag = 1;
				// SDL_RenderCopy (renderer, textTexture, NULL, &textRect);
				// SDL_RenderPresent (renderer);
				//SDL_Delay (5000);
				// printf( "Our congratulation!!!\n" );
			}
			SDL_Delay (50);
		}
	}
	SDL_RenderCopy (renderer, upTexture, NULL, NULL);
	SDL_RenderPresent (renderer);
	
	SDL_Delay (500);
	for (int i = 0; i < 3; i++)
		Mix_FreeMusic(sounds[i]);
	Mix_CloseAudio();
	SDL_DestroyTexture (textTexture);
	SDL_DestroyTexture (fifteenTexture);
	SDL_DestroyTexture (backTexture);
	SDL_DestroyTexture (upTexture);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	
	SDL_Quit ();
	IMG_Quit ();
	Mix_Quit();
	TTF_Quit ();

	return 0;
}