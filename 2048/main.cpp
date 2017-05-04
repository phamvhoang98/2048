#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <sstream>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <string>

using namespace std ;
int boardGame[4][4] = { 0 };
int gameScore;
bool replayGame = false;
bool MUTE = false ;

TTF_Font* comic ;
SDL_Window* windows;
SDL_Renderer* renderer;

// *** tao am thanh cho game

Mix_Music *startSound ;
Mix_Chunk *buttonSound ;

void loadSound()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	startSound = Mix_LoadMUS("Sound/startSound.wav");
	buttonSound = Mix_LoadWAV("Sound/button.wav");
}
void playStartSound()
{
	if(Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(startSound, -1);
	}
}
void playButtonSound()
{
	Mix_PlayChannel(-1,buttonSound,0);
}

void quitSound()
{
	Mix_FreeMusic(startSound);
    Mix_FreeChunk(buttonSound);
    Mix_CloseAudio();
}
//***

//*** dua anh vao trong game
SDL_Rect Screen = {0, 0, 484, 700};
SDL_Rect Replay = {282,159,56,58} ;
SDL_Rect Music = {380,159,58,58};
SDL_Rect gameOver = {0,238,494,462};

SDL_Rect BoxRect[4][4];

SDL_Texture* Board;
SDL_Texture* Number_2 ;
SDL_Texture* Number_4 ;
SDL_Texture* Number_8 ;
SDL_Texture* Number_16 ;
SDL_Texture* Number_32 ;
SDL_Texture* Number_64 ;
SDL_Texture* Number_128 ;
SDL_Texture* Number_256 ;
SDL_Texture* Number_512 ;
SDL_Texture* Number_1024 ;
SDL_Texture* Number_2048 ;
SDL_Texture* Number_4096 ;
SDL_Texture* Number_8192 ;
SDL_Texture* Number_16384 ;
SDL_Texture* Number_32768 ;
SDL_Texture* Number_65536 ;

SDL_Texture* Lose ;
SDL_Texture* MusicSound ;


void quitImage()
{
	SDL_DestroyTexture(Board);
	SDL_DestroyTexture(Number_2);
	SDL_DestroyTexture(Number_4);
	SDL_DestroyTexture(Number_8);
	SDL_DestroyTexture(Number_16);
	SDL_DestroyTexture(Number_32);
	SDL_DestroyTexture(Number_64);
	SDL_DestroyTexture(Number_128);
	SDL_DestroyTexture(Number_256);
	SDL_DestroyTexture(Number_512);
	SDL_DestroyTexture(Number_1024);
	SDL_DestroyTexture(Number_2048);
	SDL_DestroyTexture(Number_4096);
	SDL_DestroyTexture(Number_8192);
	SDL_DestroyTexture(Number_16384);
	SDL_DestroyTexture(Number_32768);
	SDL_DestroyTexture(Number_65536);
	SDL_DestroyTexture(Lose);
	SDL_DestroyTexture(MusicSound);
	IMG_Quit();

}



SDL_Texture* LoadTexture(const std::string &str)
{
	SDL_Surface* surface = IMG_Load(str.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void Init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_VideoInit(NULL);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	windows = SDL_CreateWindow("Game 2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 485, 700, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);
	comic = TTF_OpenFont("Font/comic.ttf", 200);
	if (comic == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERRR", SDL_GetError(), windows);
	}
}

void Quit()
{
	quitImage();
	quitSound();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windows);
	renderer = NULL;
	windows = NULL;
	SDL_Quit();
	TTF_CloseFont(comic);
	TTF_Quit();
}



void loadImage()
{
	MusicSound = LoadTexture("Image/mute.png");
	Lose = LoadTexture("Image/gameOver.png");
	Board = LoadTexture("Image/game2048.png");
	Number_2 = LoadTexture("Image/2.png");
	Number_4 = LoadTexture("Image/4.png");
	Number_8 = LoadTexture("Image/8.png");
	Number_16 = LoadTexture("Image/16.png");
	Number_32 = LoadTexture("Image/32.png");
	Number_64 = LoadTexture("Image/64.png");
	Number_128 = LoadTexture("Image/128.png");
	Number_256 = LoadTexture("Image/256.png");
	Number_512 = LoadTexture("Image/512.png");
	Number_1024 = LoadTexture("Image/1024.png");
	Number_2048 = LoadTexture("Image/2048.png");
	Number_4096 = LoadTexture("Image/4096.png");
	Number_8192 = LoadTexture("Image/8192.png");
	Number_16384 = LoadTexture("Image/16384.png");
	Number_32768 = LoadTexture("Image/32768.png");
	Number_65536 = LoadTexture("Image/65536.png");

}
void init_gameBoard()
{
	loadImage();
	SDL_RenderCopy(renderer,Board,NULL,&Screen);
	gameScore = 0;
	for (int i = 0; i < 4; i++)
        {
		for (int j = 0; j < 4; j++)
		{
			boardGame[i][j] = 0;
		}
	}

	srand(time(NULL));
	int newTile[4];

	newTile[0] = rand() % 4;
	newTile[1] = rand() % 4;

	do {
		newTile[2] = rand() % 4;
		newTile[3] = rand() % 4;
	} while (newTile[0] == newTile[2] || newTile[1] == newTile[3]);

	// Tao 2 gia tri ban dau
	boardGame[newTile[0]][newTile[1]] = 2;
	boardGame[newTile[2]][newTile[3]] = 2;
}
// *** cac ham thay doi gia tri cua cac o trong game
void turnLeft()
{
	// Dua cac gia tri sang trai

	for (int j = 0; j < 4; j++)
        {
		int t = 0;
		for (int i = 0; i < 4; i++)
		 {
			if (boardGame[i][j] != 0)
            {
				boardGame[t][j] = boardGame[i][j];
				t++;
            }
		}
		for (int i = t; i < 4; i++) boardGame[i][j] = 0;
	}

	for (int j = 0; j < 4; j++)
        {
		int t = 0;
		for (int i = 0; i < 4; i++)
		{
			if (boardGame != 0)
			{
				if (boardGame[i][j] == boardGame[i + 1][j])
				 {
					boardGame[t][j] = 2 * boardGame[i][j];
					gameScore += boardGame[t][j];
					t++;
					i++;
				}
				else {
					boardGame[t][j] = boardGame[i][j];
					t++;
                    }
			}
		}
		for (int i = t; i < 4; i++) boardGame[i][j] = 0;
	}

}

void turnRight()
{
	// Dua cac gia tri sang phai

	for (int j = 0; j < 4; j++)
        {
		int t = 3;
		for (int i = 3; i >= 0; i--)
		{
			if (boardGame[i][j] != 0)
			{
				boardGame[t][j] = boardGame[i][j];
				t--;
			}
		}
		for (int i = t; i >= 0; i--) boardGame[i][j] = 0;
	}

	for (int j = 0; j < 4; j++)
        {
		int t = 3;
		for (int i = 3; i >= 0; i--)
		{
			if (boardGame != 0)
			{
				if (boardGame[i][j] == boardGame[i - 1][j])
				{
					boardGame[t][j] = 2 * boardGame[i][j];
					gameScore += boardGame[t][j];
					t--;
					i--;
				}
				else {
					boardGame[t][j] = boardGame[i][j];
					t--;
                    }
			}
		}
		for (int i = t; i >= 0; i--) boardGame[i][j] = 0;
	}

}

void goUp()
{
	// Dua cac gia tri len tren

	for (int i = 0; i < 4; i++)
        {
		int t = 0;
		for (int j = 0; j < 4; j++)
		{
			if (boardGame[i][j] != 0)
            {
				boardGame[i][t] = boardGame[i][j];
				t++;
			}
		}
		for (int j = t; j < 4; j++) boardGame[i][j] = 0;
	}

	for (int i = 0; i < 4; i++)
        {
		int t = 0;
		for (int j = 0; j < 4; j++)
		{
			if (boardGame[i][j] != 0)
			 {
				if (boardGame[i][j] == boardGame[i][j + 1])
				 {
					boardGame[i][t] = 2 * boardGame[i][j];
					gameScore += boardGame[i][t];
					j++;
					t++;
				}
				else {
					boardGame[i][t] = boardGame[i][j];
					t++;
                    }
			}
		}
		for (int j = t; j < 4; j++) boardGame[i][j] = 0;
	}

}

void goDown()
{
	// Dua cac gia tri xuong duoi

	for (int i = 0; i < 4; i++)
        {
		int t = 3;
		for (int j = 3; j >= 0; j--)
		 {
			if (boardGame[i][j] != 0)
			{
				boardGame[i][t] = boardGame[i][j];
				t--;
			}
		}
		for (int j = t; j >= 0; j--) boardGame[i][j] = 0;
	}

	for (int i = 0; i < 4; i++)
        {
		int t = 3;
		for (int j = 3; j >= 0; j--)
		 {
			if (boardGame[i][j] != 0)
			{
				if (boardGame[i][j] == boardGame[i][j - 1])
				{
					boardGame[i][t] = 2 * boardGame[i][j];
					gameScore += boardGame[i][t];
					j--;
					t--;
				}
				else {
					boardGame[i][t] = boardGame[i][j];
					t--;
                    }
			}
		}
		for (int j = t; j >= 0; j--) boardGame[i][j] = 0;
	}
}
// *** ket thuc cac ham thay doi gia tri cua cac o trong game

// Ghi diem cao vao file
void write_bestScore(int score)
{
	fstream fileScore;
	fileScore.open("score.txt", ios::out);
	fileScore << score;
	fileScore.close();
}

int bestScore()
{
	int score;
	fstream fileScore;
	fileScore.open("score.txt", ios::in);
	fileScore >> score;
	fileScore.close();
	return score;
}
// su dung chuot , ban phim
void waitEvent()
{
	SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
		case SDL_QUIT:
			{
				Quit();
				exit(0);
				break;
			}
		case SDL_KEYDOWN:
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE :
					{
						Quit();
						break;
					}
				case SDLK_LEFT:
					{
						turnLeft();
						break;
					}
				case SDLK_DOWN:
					{
						goDown();
						break;
					}
				case SDLK_RIGHT:
					{
						turnRight();
						break;
					}
				case SDLK_UP:
					{
						goUp();
						break;
					}
				default :
					break;
				}
				break;
			}

		case SDL_MOUSEBUTTONDOWN:
			{
				int mouseClickX = event.motion.x;
                int mouseClickY = event.motion.y;
                if((mouseClickX >= Replay.x) && (mouseClickX <= Replay.x + Replay.w) && (mouseClickY >= Replay.y) && (mouseClickY <= Replay.y + Replay.h))
				{
					if (MUTE == false)
					{
						playButtonSound();
					}
					replayGame = true ;
				}
				else if((mouseClickX >= Music.x) && (mouseClickX <= Music.x + Music.w) && (mouseClickY >= Music.y) && (mouseClickY <= Music.y + Music.h))
				{
					if (MUTE == false )
					{
						playButtonSound();
						MUTE = true ;
					}
					else  MUTE = false ;

				}
				break ;
			}
		default:
			break;
        }
    }
}
// hien thi diem, diem cao
void loadScore()
{
	std::stringstream score1, score2;
    score1 << gameScore ;
    std::string strA = score1.str();
    score2 << bestScore();
    std::string strB = score2.str();
	SDL_Texture *result_text[2];
    SDL_Color WHITE = {255,255,255};
    SDL_Surface* ScoreSurA = TTF_RenderText_Solid(comic, strA.c_str(), WHITE);
    SDL_Surface* ScoreSurB = TTF_RenderText_Solid(comic, strB.c_str(), WHITE);
	SDL_Rect Score = {284,51,52,65};
	SDL_Rect Best  = {383,51,52,65};
    result_text[0] = SDL_CreateTextureFromSurface(renderer,ScoreSurA);
    SDL_RenderCopy(renderer, result_text[0], NULL, &Score);
	SDL_FreeSurface(ScoreSurA);
    result_text[1] = SDL_CreateTextureFromSurface(renderer, ScoreSurB);
    SDL_RenderCopy(renderer, result_text[1], NULL, &Best);
    SDL_FreeSurface(ScoreSurB);
    SDL_DestroyTexture(result_text[1]);
    SDL_DestroyTexture(result_text[0]);
}

//*** dua anh vao cac o thich hop
void loadRectangle()
{
	for (int i = 0 ; i<4;i++ )
	{
		for (int j=0;j<4;j++)
		{

			BoxRect[i][j].x = 40+103*i;
			BoxRect[i][j].y = 254+103*j ;
			BoxRect[i][j].w = 89;
			BoxRect[i][j].h = 89;
		}
	}
}
void setImage()
{

	SDL_RenderCopy(renderer,Board,NULL,&Screen);
	for (int i = 0 ; i < 4 ; i++)
	{
		for (int j = 0 ; j < 4 ; j++)
		{
			if (boardGame[i][j] == 2)
			{
				SDL_RenderCopy(renderer,Number_2,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 4)
			{
				SDL_RenderCopy(renderer,Number_4,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 8)
			{
				SDL_RenderCopy(renderer,Number_8,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 16)
			{
				SDL_RenderCopy(renderer,Number_16,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 32)
			{
				SDL_RenderCopy(renderer,Number_32,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 64)
			{
				SDL_RenderCopy(renderer,Number_64,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 128 )
			{
				SDL_RenderCopy(renderer,Number_128,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 256)
			{
				SDL_RenderCopy(renderer,Number_256,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 512)
			{
				SDL_RenderCopy(renderer,Number_512,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 1024)
			{
				SDL_RenderCopy(renderer,Number_1024,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 2048)
			{
				SDL_RenderCopy(renderer,Number_2048,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 4096)
			{
				SDL_RenderCopy(renderer,Number_4096,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 8192)
			{
				SDL_RenderCopy(renderer,Number_8192,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 16384)
			{
				SDL_RenderCopy(renderer,Number_16384,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 32768)
			{
				SDL_RenderCopy(renderer,Number_32768,NULL,&BoxRect[i][j]);
			}
			else if (boardGame[i][j] == 65536)
			{
				SDL_RenderCopy(renderer,Number_65536,NULL,&BoxRect[i][j]);
			}
		}
	}
}
//*** ket thuc viec dua anh vao cac o thich hop

// Kiem tra con vi tri trong hay khong
bool checkZero()
{
	for (int i = 0; i < 4; i++)
        {
		for (int j = 0; j < 4; j++)
		{
			if (boardGame[i][j] == 0)
            {
				return true;
			}
		}
	}
	return false;
}

// sinh them gia tri moi
void addTile()
{
	if (checkZero() == false) return;
	int x, y;
	srand(time(NULL));

	do {
		x = rand() % 4;
		y = rand() % 4;
	} while (boardGame[x][y] != 0);

	int s = rand() % 10;
	if (s > 8) boardGame[x][y] = 4;
	else boardGame[x][y] = 2;
}

int boardCheck[4][4];

void creCheck()
{
	for (int i = 0; i < 4; i++)
        {
		for (int j = 0; j < 4; j++)
        {
			boardCheck[i][j] = boardGame[i][j];
		}
	}
}

bool checkMove()
{
	for (int i = 0; i < 4; i++)
        {
		for (int j = 0; j < 4; j++)
        {
			if (boardCheck[i][j] != boardGame[i][j]) return true;
		}
	}
	return false;
}
// kiem tra gameover
bool checkGameOver()
{
	if (checkZero() == false)
        {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			 {
				if (boardGame[i][j] == boardGame[i][j + 1] || boardGame[i][j] == boardGame[i + 1][j])
                    return false;
			}
		}
	}
	else if (checkZero() == true) return false;
	return true;
}

int main(int argc, char* argv[])
{
	Init();
	loadRectangle();
	loadImage();
	loadSound();
	init_gameBoard();
	while(true)
	{
		playStartSound();
		if (replayGame == true)
        {
			init_gameBoard();
			replayGame = false;
		}

		creCheck();

		SDL_RenderClear(renderer);
		waitEvent();

		setImage();
		if ( MUTE == true)
		{
			SDL_RenderCopy(renderer,MusicSound,NULL,&Music);
			Mix_PauseMusic();
		}
		else Mix_ResumeMusic();
		loadScore();
		if (checkGameOver() == true)
		{
			SDL_RenderCopy(renderer,Lose,NULL,&gameOver);
		}

		if (gameScore > bestScore())
		{
			write_bestScore(gameScore);
		}
		SDL_RenderPresent(renderer);
		if (checkMove() == false)
		{
			continue;
		}
		else addTile();

	}
		return 0;
}
