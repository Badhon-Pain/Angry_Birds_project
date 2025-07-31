#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include <string>
#include "iSound.h"
#include "2405083.h"
#include <vector>
#include <fstream>
#include <algorithm>
// #include "leaderboard.h"
#include "iFont.h"
// Leaderboard structure
struct PlayerScore {
    std::string name;
    int score;
    std::string level;  
};

std::vector<PlayerScore> leaderboard;
/*
screen =0 -> very first screen (menu screen)
screen =1 -> third screen (easy level screen)
screen =2 -> second screen (level select screen)
screen =3 -> medium level screen
screen =4 -> hard level screen
screen =5 -> credit showing
screen =6 -> settings page
screen =7 -> instruction page
screen =8 -> about page
screen =9 -> about_2 page
screen =10 -> Username taking screen
screen =11 -> easyPause
screen =12 ->medium pause
screen =13 -> hard pause


*/
void loadResources();
void updateBird();
void loadLeaderboard(const std::string& filename);
void saveLeaderboard(const std::string& filename);
void addScore(const std::string& name, int score);
int getPlayerRank(const std::string& playerName, int playerScore);
void drawScoreDisplay();
void resetGameState();
void resetBeams();
bool Correct_username();
void drawMenu();
void drawLevelSelect();
void draweasy();
void drawmedium();
void drawhard();
void drawBirds_easy();
void drawBirds_medium();
void drawBirds_hard();
void updatePigMotion(int i);
void updatePhysics();

#define ROWS 15
#define COLLUMS 20
#define BUET_ROWS 7
#define BUET_COLS 25
#define SCREEN_LEADERBOARD 20

// Game State
int screen = 0, currentScreen = -1, score = 0;
int difficultylevel = 0;
bool soundOn = true, musicOn = true;
char userName[100] = "";
int userNameLen = 0;
bool writtingName = false, nameWarning = false;
bool medium_clear = false, easy_clear = false, hard_clear = false;
int pigCounter = 0;
bool pigsRemain = false;
bool enterKeyPressed = false; 
bool scoreSaved = false; 
bool hoverPlay = false, hoverExit = false, hoverCredit = false;
bool hoverEasy = false, hoverMedium = false, hoverHard = false;

// Bird Physics and States
bool bluevisible = true, redvisible = true, yellowvisible = true, blackvisible = true;
bool discovisible= true, pinkvisible = true, greenvisible = true;
int bluebirdX[3] = {20, 20, 20};
int bluebirdY[3] = {194, 194, 194};
float blue_vx[3] = {0, 0, 0};
float blue_vy[3] = {0, 0, 0};
bool blueflying[3] = {false, false, false};
bool bluevisible_arr[3] = {true, false, false}; 
bool blueSplit = false;
int redSpriteX = 100, redSpriteY = 194;
int yellowbirdX = 180, yellowbirdY = 200;


float red_vx = 0, red_vy = 0;
float yellow_vx = 0, yellow_vy = 0;
bool redflying = false, yellowflying = false, discoflying = false, pinkflying = false, greenflying = false,
discodragging = false, pinkdragging = false, greendragging = false;
bool bluedragging = false, reddragging = false, yellowdragging = false;
float restitution = 0.6f, friction = 0.8f, angularFriction = 0.97f;

int currentBird = 0, selectedBird = -1; // 0-> blue, 1-> red, 2-> yellow
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;
int birdheight = 63, birdwidth = 65;

// pig info's:
const int pigCount = 3;
int pigX[pigCount] = {1070, 1320, 1570};
int pigY[pigCount] = {420, 420, 420};
float pigVX[pigCount] = {}, pigVY[pigCount] = {};
bool pigVisible[pigCount] = {true, true, true};
bool pigFalling[pigCount] = {false, false, false};
int pigheight = 60, pigwidth = 60;

// Cursor
int cursorX = -1, cursorY = -1;
char cursorStr[30];

// Assets
Image bg, gultiback, gultifront, map_block, map_mosaic, map_stone, woodblock, realwoodSq, realwoodRect,
    menuBg, blueImg, redImg, yellowImg, bg1, woodHorizontal, woodVertical, realwoodSt, realiceSq, realiceRect,
    woodHorizontal2, woodVertical2, rock, blackImg, bgHard, settings, previous, realiceSt, nameshowingBar,
    menubutton, levelbutton, scorebutton, pigimage, credit2, creditBack, credit1, cross_button, mediumCleared,
    sound1, sound2, about, faq, instruction, whiteCanvas, about1, about2, next_button, cross_button2, pause, pausebox,play,restart,namec,monsterPig,leaderboardimg,win,gameover,nextlevel,mainmenu;
Image redframes[4],
    
     rosco, stella, hall, bluePair;
    
Sprite redSprite, verticalSprite;

// for medium levels collision
bool mediumblocksVisible[ROWS][COLLUMS];
bool mediumpigsVisible[ROWS][COLLUMS];
bool mediumrockVisible[ROWS][COLLUMS];
void initMediumLevel();
bool showWin = false;
bool showGameOver = false;

void loadResources()
{
    iLoadImage(&leaderboardimg, "assets/images/prize.png");
    iLoadImage(&mainmenu, "assets/images/menu.png");
    iLoadImage(&restart, "assets/images/restart.png");
    iLoadImage(&nextlevel, "assets/images/next.png");
    iLoadImage(&win, "assets/images/winbox.png");
    iLoadImage(&gameover, "assets/images/gameover.png");
    iLoadImage(&bg, "assets/images/Angry Bird3.jpg");
    iLoadImage(&pausebox, "assets/images/pausebox.png");
    iLoadImage(&pause, "assets/images/pause.png");
    iLoadImage(&menuBg, "assets/images/IconFinal.png");
    iLoadImage(&menubutton, "assets/images/5.png");
    iResizeImage(&menubutton, 130, 120);
    iLoadImage(&levelbutton, "assets/images/5.png");
    iResizeImage(&levelbutton, 140, 130);
    iLoadImage(&gultiback, "assets/images/CatapultBackSprite.png");
    iLoadImage(&gultifront, "assets/images/CatapultFrontSprite.png");
    iLoadImage(&blueImg, "assets/images/Blue_angry_bird5.png");
    iResizeImage(&blueImg, 57, 57);
    iLoadImage(&redImg, "assets/images/redAngryBird.png");
    iResizeImage(&redImg, 57, 57);
    iLoadImage(&yellowImg, "assets/images/yellowAngryBird.png");
    iResizeImage(&yellowImg, 57, 57);
    iLoadImage(&pigimage, "assets/images/Pig_front.png");
    iResizeImage(&pigimage, 60, 60);
    iLoadImage(&woodHorizontal, "assets/images/Pillar_01.png");
    iResizeImage(&woodHorizontal, 120, 30);
    iLoadImage(&woodVertical, "assets/images/Pillar_02.png");
    iResizeImage(&woodVertical, 30, 200);
    iLoadImage(&woodHorizontal2, "assets/images/Pillar_01.png");
    iResizeImage(&woodHorizontal2, 60, 30);
    iLoadImage(&woodVertical2, "assets/images/Pillar_02.png");
    iResizeImage(&woodVertical2, 30, 100);
    iLoadImage(&scorebutton, "assets/images/18.png");
    iLoadImage(&bg1, "assets/images/BG_03.png");
    iResizeImage(&bg1, 1920, 1080);
    iLoadImage(&map_block, "assets/images/Pillar_05_exs.png");
    iResizeImage(&map_block, 60, 60);
    iLoadImage(&map_mosaic, "assets/images/Tile_21.png");
    iResizeImage(&map_mosaic, 32, 32);
    iLoadImage(&map_stone, "assets/images/Tile_45.png");
    iResizeImage(&map_stone, 32, 32);
    iLoadImage(&woodblock, "assets/images/Wooden_Box.png");
    iResizeImage(&woodblock, 32, 32);
    iLoadImage(&rock, "assets/images/Rock_06.png");
    iLoadImage(&blackImg, "assets/images/Bomb.png");
    iLoadImage(&bgHard, "assets/images/bgHard.jpg");
    iLoadImage(&credit2, "assets/images/credits2.png");
    iLoadImage(&creditBack, "assets/images/creditback.png");
    iLoadImage(&settings, "assets/images/settings.png");
    iLoadImage(&previous, "assets/images/prew.png");
    iLoadImage(&credit1, "assets/images/woodenboard.png");
    iLoadImage(&sound1, "assets/images/96.png"); // sound on
    iLoadImage(&sound2, "assets/images/95.png"); // sound off
    iLoadImage(&about, "assets/images/about.png");
    iLoadImage(&instruction, "assets/images/faq.png");
    iLoadImage(&cross_button, "assets/images/close.png");
    iLoadImage(&whiteCanvas, "assets/images/whiteCanvas.png");
    iLoadImage(&about1, "assets/images/About_1.png");
    iLoadImage(&about2, "assets/images/About_2.png");
    iLoadImage(&next_button, "assets/images/next.png");
    iLoadImage(&cross_button2, "assets/images/close_2.png");
    iLoadImage(&realwoodSq, "assets/images/woodSquare.jpg");
    iLoadImage(&nameshowingBar, "assets/images/93.png");
    iLoadImage(&mediumCleared, "assets/images/93.png");
    iLoadImage(&realwoodRect, "assets/images/woodRectangle.jpg");
    iLoadImage(&realiceSq, "assets/images/iceSquarePlain.jpg");
    iLoadImage(&realiceRect, "assets/images/iceRectangle.jpg");
    iLoadImage(&realiceSt, "assets/images/iceStickPlain.jpg");
    iLoadImage(&realwoodSt, "assets/images/woodStick.jpg");
    iLoadImage(&monsterPig, "assets/images/monsterPig.png");
    iLoadImage(&rosco, "assets/images/Rosco.png");
    iLoadImage(&stella, "assets/images/Stella.png");
    iLoadImage(&hall, "assets/images/Hal.png");
    iLoadImage(&bluePair, "assets/images/BluesSVG.png");
    iLoadImage(&play, "assets/images/play.png");
    
}

// Rubber Position
int leftArmX = 298, leftArmY = 374;
int rightArmX = 336, rightArmY = 380;

// for pillar & ground
const int pillarCount = 3;
int pillarX[pillarCount] = {1088, 1338, 1588};
int pillarY[pillarCount] = {205, 205, 205};
bool pillarHit[pillarCount] = {false, false, false};
float pillarAngle[pillarCount] = {0, 0, 0};
int pillarwidth = 30, pillarheight = 200;
int groundY = 200;
float pillarRotation[3] = {0}, pillarAngVelocity[3] = {0};
bool pillarRotating[3] = {false};
float beamPositionsX[3] = {1088 - 46, 1338 - 46, 1588 - 46}, beamPositionsY[3] = {398, 398, 398},
      beamVelocitiesX[3] = {0}, beamVelocitiesY[3] = {0};

void drawMenu()
{
    showGameOver = false;
    showWin = false;
    iShowLoadedImage(0, 0, &menuBg);

    
    if (hoverPlay)
        iShowLoadedImage2(105, 232, &menubutton, 140, 130); 
    else
        iShowLoadedImage(110, 237, &menubutton);
    iSetColor(255, 255, 255);
    iText(140, 288, "PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    // EXIT
    if (hoverExit)
        iShowLoadedImage2(105, 172, &menubutton, 140, 130);
    else
        iShowLoadedImage(110, 177, &menubutton);
    iText(140, 228, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);

    // CREDIT
    if (hoverCredit)
        iShowLoadedImage2(105, 105, &menubutton, 140, 130);
    else
        iShowLoadedImage(110, 110, &menubutton);
    iText(132, 162, "CREDIT", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage2(1830, 130, &leaderboardimg, 70, 70);
}



void drawLevelSelect()
{
    showGameOver = false;
    showWin = false;
    iShowLoadedImage(0, 0, &menuBg);

    // EASY Button
    if (hoverEasy)
        iShowLoadedImage2(80, 205, &levelbutton, 150, 140);
    else
        iShowLoadedImage(85, 210, &levelbutton);
    iSetColor(255, 255, 255);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    // MEDIUM Button
    if (hoverMedium)
        iShowLoadedImage2(80, 150, &levelbutton, 150, 140);
    else
        iShowLoadedImage(85, 155, &levelbutton);
    iText(105, 210, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    // HARD Button
    if (hoverHard)
        iShowLoadedImage2(80, 90, &levelbutton, 150, 140);
    else
        iShowLoadedImage(85, 95, &levelbutton);
    iText(120, 150, "HARD", GLUT_BITMAP_TIMES_ROMAN_24);
}


void drawSettings()
{
    
    showGameOver = false;
    showWin = false;
    iShowLoadedImage(0, 0, &menuBg);
    iSetTransparentColor(0, 0, 0, 0.5);
    iFilledRectangle(0, 0, 1920, 1080);
    iShowLoadedImage2(677 - 152, 360 - 73, &credit2, 1050, 700);
    iShowLoadedImage2(56, 56, &previous, 50, 50);
}

void drawpause()
{
    // iSetTransparentColor(0, 0, 0, 0.5);
     if (currentScreen == 1)
        iShowLoadedImage2(0, 0, &bg);
    else if (currentScreen == 3)
        iShowLoadedImage(0, 0, &bg1);
    else if (currentScreen == 4)
        iShowLoadedImage2(0, 0, &bgHard, 1920, 1080);

    iShowLoadedImage2(530, 320, &pausebox, 900, 570);
    iShowLoadedImage2(911, 423, &previous, 70, 70);
    iShowLoadedImage2(699, 555, &play, 70, 70);
    iShowLoadedImage2(911, 555, &restart, 70, 70);
    iShowLoadedImage2(1121, 555, &mainmenu, 70, 70);

}

int map1[ROWS][COLLUMS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
    {0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void display_map1()
{
    int startX = 1220, startY = 500;
    int blockW = 30, blockH = 30;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLLUMS; c++)
        {
            int x = startX + c * blockW;
            int y = startY - r * blockH;

            if (map1[r][c] == 1)
                iShowLoadedImage(x, y, &map_block);
            else if (map1[r][c] == 2)
                iShowLoadedImage2(x - 9, y + 18, &pigimage, 50, 45);
            else if (map1[r][c] == 3)
                iShowLoadedImage(x, y, &rock);
        }
    }
}


void checkMap1Collision(int birdX, int birdY)
{
    int blockW = 30, blockH = 30;
    int startX = 1220, startY = 500;
    bool falling = false;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLLUMS; c++)
        {
            int x = startX + c * blockW;
            int y = startY - r * blockH;

            if (map1[r][c] == 1 || map1[r][c] == 2 || map1[r][c] == 3)
            {

                if (birdX + birdRadius > x && birdX - birdRadius < x + blockW &&
                    birdY + birdRadius > y && birdY - birdRadius < y + blockH)
                {
                    
                    if (map1[r][c] == 2) { // Pig
                        score += 500;
                    } else if (map1[r][c] == 1 || map1[r][c] == 3) { // Block or rock
                        score += 100;
                    }
                    map1[r][c] = 0;
                    falling = true;
                }
            }
        }
    }

    
    while (falling)
    {
        falling = false;
        for (int r = ROWS - 2; r >= 0; r--)
        {
            for (int c = 0; c < COLLUMS; c++)
            {
                if ((map1[r][c] == 1 || map1[r][c] == 2 || map1[r][c] == 3) && 
                    map1[r + 1][c] == 0 && r + 1 < ROWS)
                {
                    map1[r + 1][c] = map1[r][c];
                    map1[r][c] = 0;
                    falling = true;
                }
                else if ((map1[r][c] == 1 || map1[r][c] == 3) && map1[r + 1][c] == 2)
                {
                    // Block falling on pig
                    score += 500;
                    map1[r + 1][c] = 0;
                    falling = true;
                }
            }
        }
    }
}

int map3[20][20] = {
    {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
    {0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
    {0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0},
    {0, 0, 4, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0},
    {0, 4, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0},
    {4, 0, 0, 0, 0, 0, 4, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 6, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 2, 6, 0, 0, 0, 0, 2},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void display_map3()
{
    int blockSize = 30;
    int startX = 1100;
    int startY = 620;

    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 20; col++)
        {

            if (map3[row][col] == 1) // real wood square
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY, &realwoodSq);
            }

            if (map3[row][col] == 2) // vertical pillar
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY, &woodVertical);
            }
            if (map3[row][col] == 3) // horizontal beam
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;

                iShowLoadedImage2(posX, posY, &woodHorizontal, 200, 30);
            }

            if (map3[row][col] == 4) // ice square
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY - 7, &realiceSq);
            }
            if (map3[row][col] == 5) // pig image
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;

                iShowLoadedImage2(posX, posY, &pigimage, 50, 45);
            }
            if (map3[row][col] == 6) // monster pig
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY, &monsterPig);
            }
            if (map3[row][col] == 7) // small horizontal beam
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY, &woodHorizontal2, 60, 38);
            }
        }
    }
}
void checkMap3Collision(int birdX, int birdY)
{
    int blockW = 30, blockH = 30;
    int startX = 1100, startY = 620;
    bool falling = false;

    for (int r = 0; r < 20; r++)
    {
        for (int c = 0; c < 20; c++)
        {
            int x = startX + c * blockW;
            int y = startY - r * blockH;

            if (map3[r][c] >= 1 && map3[r][c] <= 7)
            {
                if (birdX + birdRadius > x && birdX - birdRadius < x + blockW &&
                    birdY + birdRadius > y && birdY - birdRadius < y + blockH)
                {
                    map3[r][c] = 0;
                    falling = true;
                }
            }
        }
    }

    while (falling)
    {
        falling = false;
        for (int r = 18; r >= 0; r--)
        {
            for (int c = 0; c < 20; c++)
            {

                if (map3[r][c] >= 1 && map3[r][c] <= 7 && map3[r + 1][c] == 0 && map3[r + 1][c] != 3)
                {
                    map3[r + 1][c] = map3[r][c];
                    map3[r][c] = 0;
                    falling = true;
                }

                else if ((map3[r][c] >= 1 && map3[r][c] <= 4) && (map3[r + 1][c] == 5 || map3[r + 1][c] == 6))
                {
                    map3[r + 1][c] = 0;
                    falling = true;
                }
                else if (map3[r][c] >= 1 && map3[r][c] <= 7 && map3[r + 1][c] == 2)
                {
                    map3[r + 1][c] = 0;
                    map3[r][c] = 0;
                    falling = true;
                }
            }
        }
    }
}


void initEasyLevel() {
   
    pigX[0] = 1070; pigY[0] = 420;
    pigX[1] = 1320; pigY[1] = 420;
    pigX[2] = 1570; pigY[2] = 420;
    
    for (int i = 0; i < pigCount; i++) {
        pigVisible[i] = true;
        pigFalling[i] = false;
        pigVX[i] = pigVY[i] = 0;
    }
    
   
    for (int i = 0; i < pillarCount; i++) {
        pillarX[i] = 1088 + i * 250;
        pillarY[i] = 205;
        pillarRotation[i] = 0;
        pillarAngVelocity[i] = 0;
        pillarRotating[i] = false;
        
        beamPositionsX[i] = 1088 - 46 + i * 250;
        beamPositionsY[i] = 398;
        beamVelocitiesX[i] = beamVelocitiesY[i] = 0;
    }
    
    
    bluebirdX[0] = 20; bluebirdY[0] = 194;
    redSpriteX = 100; redSpriteY = 194;
    yellowbirdX = 180; yellowbirdY = 200;
    
    for (int i = 0; i < 3; i++) {
        blue_vx[i] = blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0);
    }
    red_vx = red_vy = 0; redflying = false;
    yellow_vx = yellow_vy = 0; yellowflying = false;
    
    discovisible = redvisible = yellowvisible = true;
    bluedragging = reddragging = yellowdragging = false;
    selectedBird = -1;
}


void initMediumLevel() {
    
    int initialMap1[ROWS][COLLUMS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    memcpy(map1, initialMap1, sizeof(map1));
    
    // Reset birds
    bluebirdX[0] = 20; bluebirdY[0] = 194;
    redSpriteX = 100; redSpriteY = 194;
    yellowbirdX = 180; yellowbirdY = 200;
    
    for (int i = 0; i < 3; i++) {
        blue_vx[i] = blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0);
    }
    red_vx = red_vy = 0; redflying = false;
    yellow_vx = yellow_vy = 0; yellowflying = false;
    
    greenvisible = blackvisible = true;
    bluedragging = greendragging = yellowdragging = false;
    selectedBird = -1;
}


void initHardLevel() {
    // Full hard level map (20x20)
    int initialMap3[20][20] = {
        {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0},
        {0, 0, 4, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0},
        {0, 4, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0},
        {4, 0, 0, 0, 0, 0, 4, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 6, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 2, 6, 0, 0, 0, 0, 2},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    memcpy(map3, initialMap3, sizeof(map3));
    
    // Reset birds
    bluebirdX[0] = 20; bluebirdY[0] = 194;
    redSpriteX = 100; redSpriteY = 194;
    yellowbirdX = 180; yellowbirdY = 200;
    
    for (int i = 0; i < 3; i++) {
        blue_vx[i] = blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0);
    }
    red_vx = red_vy = 0; redflying = false;
    yellow_vx = yellow_vy = 0; pinkflying = false;
    
    redvisible = pinkvisible = true;
    bluedragging = reddragging = pinkdragging = false;
    selectedBird = -1;
}


void resetLevel() {
    // Reset game state flags
    showWin = false;
    showGameOver = false;
    score = 0;
    blueSplit = false;
    selectedBird = -1;
    greenvisible = true;
blackvisible = true;
pinkvisible=true;

    // Reset all birds
    for (int i = 0; i < 3; i++) {
        bluebirdX[i] = 20;
        bluebirdY[i] = 194;
        blue_vx[i] = 0;
        blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0); 
    }
    
    redSpriteX = 100;
    redSpriteY = 194;
    red_vx = red_vy = 0;
    redflying = false;
    redvisible = true;
    
    yellowbirdX = 180;
    yellowbirdY = 200;
    yellow_vx = yellow_vy = 0;
    yellowflying = false;
    yellowvisible = true;
    blackvisible = true;

    // Reset dragging states
    bluedragging = false;
    reddragging = false;
    yellowdragging = false;

    // Level-specific resets
    if (screen == 1) { // Easy Level
        // Reset pigs
        for (int i = 0; i < pigCount; i++) {
            pigVisible[i] = true;
            pigFalling[i] = false;
            pigX[i] = 1070 + i * 250;
            pigY[i] = 420;
            pigVX[i] = pigVY[i] = 0;
        }
        
        // Reset pillars
        resetBeams();
        for (int i = 0; i < pillarCount; i++) {
            pillarRotation[i] = 0;
            pillarAngVelocity[i] = 0;
            pillarRotating[i] = false;
        }
    }
    else if (screen == 3) { // Medium Level
        
        int initialMap1[ROWS][COLLUMS] = {
            // Your original map1 initialization data here
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            // rest,,,,,, eta sudhu bojhar jonne
        };
        memcpy(map1, initialMap1, sizeof(map1));
    }
    else if (screen == 4) { // Hard Level
        
        int initialMap3[20][20] = {
            // Your original map3 initialization data here
            {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
            // bojhar jnne
        };
        memcpy(map3, initialMap3, sizeof(map3));}
}





   
void drawRubberLines(int x, int y)
{
    iSetColor(0, 0, 0);
    iLine(leftArmX, leftArmY, x + birdRadius / 2, y + birdRadius / 2);
    iLine(rightArmX, rightArmY, x + birdRadius / 2, y + birdRadius / 2);
}

void drawRubberLines_hard(int x, int y)
{
    iSetColor(0, 0, 0);
    iLine(274, 217, x + birdRadius / 2, y + birdRadius / 2);
    iLine(313, 223, x + birdRadius / 2, y + birdRadius / 2);
}

void drawRubberLines_medium(int x, int y)
{
    iSetColor(0, 0, 0);
    iLine(323, 293, x + birdRadius / 2, y + birdRadius / 2);
    iLine(369, 300, x + birdRadius / 2, y + birdRadius / 2);
}

void drawPathway(int x, int y, float vx, float vy)
{
    float fx = x + birdRadius / 2;
    float fy = y + birdRadius / 2;
    float dt = 0.8;
    for (int i = 0; i < 200; i++)
    {
        fx += vx * dt;
        fy += vy * dt;
        vy += g * dt;
      
        iSetColor(0, 0, 0);
        iFilledCircle(fx, fy, 2);
    }
}

void drawPathway_hard(int x, int y, float vx, float vy)
{
    float fx = x + birdRadius / 2;
    float fy = y + birdRadius / 2;
    float dt = 0.1f;
    for (int i = 0; i < 200; i++)
    {
        fx += vx * dt;
        fy += vy * dt;
        vy += g * dt;
        if (fy < 0)
            break;
        iSetColor(0, 0, 0);
        iPoint(fx, fy, 1);
    }
}

void updatePigMotion(int i)
{
    if (pigFalling[i])
    {
        pigVY[i] += g * 0.3f;
        pigX[i] += pigVX[i];
        pigY[i] += pigVY[i];

        for (int j = 0; j < pillarCount; j++)
        {
            bool collX = pigX[i] + pigwidth >= pillarX[j] && pigX[i] <= pillarX[j] + pillarwidth;
            bool collY = pigY[i] + pigheight >= pillarY[j] && pigY[i] <= pillarY[j] + pillarheight;
            if (collX && collY)
            {
                pigVX[i] = -pigVX[i] * 0.4f;
                pigVY[i] = -pigVY[i] * 0.4f;
            }
        }

        if (pigY[i] <= groundY)
        {
            pigY[i] = groundY;
            pigVX[i] = pigVY[i] = 0;
            pigFalling[i] = false;
           
            if (pigVisible[i]) {
                score += 500; // 
                pigVisible[i] = false;
            }
        }

        if (pigX[i] < 0 || pigX[i] > 1920 || pigY[i] > 1080)
        {
          
           if (pigVisible[i]) {
                score += 500; 
                pigVisible[i] = false;
            }
            pigFalling[i] = false;
        }
    }
}

void drawBirds_easy()
{

    if (discovisible)
        iShowLoadedImage2(bluebirdX[0], bluebirdY[0], &rosco, 60, 60);
    if (redvisible)
        iShowLoadedImage(redSpriteX, redSpriteY, &redImg);
    

    if (yellowvisible)
        iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void drawBirds_medium()
{

    for (int i = 0; i < 3; i++)
        if (bluevisible_arr[i])
            iShowLoadedImage2(bluebirdX[i], bluebirdY[i], &blueImg, 50, 50);

    if (greenvisible)
        iShowLoadedImage2(redSpriteX, redSpriteY, &hall, 50, 50);

    if (blackvisible)
        iShowLoadedImage2(yellowbirdX, yellowbirdY, &blackImg, 50, 50);
}

void drawBirds_hard()
{

        for (int i = 0; i < 3; i++)
        if (bluevisible_arr[i])
            iShowLoadedImage2(bluebirdX[i], bluebirdY[i]-115, &blueImg, 50, 50);

    if (redvisible)
        iShowLoadedImage2(redSpriteX, redSpriteY - 115, &redImg, 50, 50);

    if (pinkvisible)
        iShowLoadedImage2(yellowbirdX, yellowbirdY - 115, &stella, 60, 60);
}

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 &&
            y1 < y2 + h2 && y1 + h1 > y2);
}

void updatePhysics()
{
    
    for (int i = 0; i < 3; i++)
    {
        if (pillarRotating[i])
        {

            pillarRotation[i] += pillarAngVelocity[i];
            pillarAngVelocity[i] *= angularFriction;
            if ((pillarAngVelocity[i] > 0 && pillarRotation[i] >= 90.0f) ||
                (pillarAngVelocity[i] < 0 && pillarRotation[i] <= -90.0f))
            {
                pillarRotation[i] = (pillarAngVelocity[i] > 0) ? 90.0f : -90.0f;
                pillarRotating[i] = false;
                pillarAngVelocity[i] = 0;
            }
        }
    }

    
    for (int i = 0; i < 3; i++)
    {

        if (fabs(beamVelocitiesX[i]) > 0.1f || fabs(beamVelocitiesY[i]) > 0.1f)
        {
            beamVelocitiesY[i] += g * 0.5f;
            beamPositionsX[i] += beamVelocitiesX[i];
            beamPositionsY[i] += beamVelocitiesY[i];
            beamVelocitiesX[i] *= friction;
            if (beamPositionsY[i] <= groundY)
            {
                beamPositionsY[i] = groundY;
                beamVelocitiesY[i] = 0;
                beamVelocitiesX[i] = 0;
            }
        }
    }
    if (screen == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            checkMap1Collision(bluebirdX[i], bluebirdY[i]);
        }

        checkMap1Collision(redSpriteX, redSpriteY);
        checkMap1Collision(yellowbirdX, yellowbirdY);
    }
    if (screen == 4)
    {
        for (int i = 0; i < 3; i++)
        {
            checkMap3Collision(bluebirdX[i], bluebirdY[i] - 115);
        }

        checkMap3Collision(redSpriteX, redSpriteY - 115);
        checkMap3Collision(yellowbirdX, yellowbirdY - 115);
    }
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {
        vy += g * 0.8;
        x += vx;
        y += vy;

        float birdVelocity = sqrt(vx * vx + vy * vy);
        float hittingAngle = atan2(vy, vx);

        if (y <= groundY)
        {
            y = groundY;
            vy = -vy * restitution * 0.6;
            vx *= friction;

            if (fabs(vx) < 0.8)
            {
                flying = false;
                visible = false;
            }
            iPlaySound("assets/sounds/ball_bounce.wav");
        }
        if (x < 0 || x > 1919 || y > 1080)
        {
            flying = false;
            visible = false;
            return;
        }
      
        

        // Collision with pillars
        for (int i = 0; i < pillarCount; i++)
        {
            int baseX = 1088 + i * 250;

            // vertical
            if (checkCollision(x, y, birdwidth, birdheight, baseX, pillarY[i], pillarwidth, pillarheight))
            {
                
                vx = -vx * 0.5;
                vy *= 0.8;
                pillarHit[i] = true;
                pillarRotating[i] = true;
                
                pillarAngVelocity[i] = -10.0f;
                iPlaySound("assets/sounds/wood_damage_a1.wav");
            }
          

            // horizontal
            if (checkCollision(x, y, birdwidth, birdheight,
                               beamPositionsX[i], beamPositionsY[i], 120, 30))
            {
                beamVelocitiesX[i] += vx * 0.2f;
                beamVelocitiesY[i] += vy * 0.2f;
                vy = -vy * restitution;
                vx *= 0.9f;
                iPlaySound("assets/sounds/wood_damage_a2.wav");
            }
        }

        
        for (int i = 0; i < pigCount; i++)
        {
            if (pigVisible[i] &&
                checkCollision(x, y, birdwidth, birdheight,
                               pigX[i], pigY[i], pigwidth, pigheight))
            {

                bool onPillar = false;
                for (int j = 0; j < pillarCount; j++)
                {
                    int pillarTop = pillarY[j] + pillarheight;

                    if (pigX[i] + pigwidth > pillarX[j] && pigX[i] < pillarX[j] + pillarwidth)
                    {

                        if (fabs(pillarRotation[j]) < 45.0f)
                        {

                            if (abs((pigY[i]) - (pillarY[j] + pillarheight)) < 10)
                            {
                                onPillar = true;
                                break;
                            }
                        }
                    }
                }

                if (!onPillar)
                {
                    pigFalling[i] = true;
                }

                pigVX[i] = cos(hittingAngle) * birdVelocity * 0.3f;
                pigVY[i] = sin(hittingAngle) * birdVelocity * 0.3f;
                score += 100;
                iPlaySound("assets/sounds/pig_collision_a6.wav");
            }
            updatePigMotion(i);
        }
    }
}
void updateSingleBird_medium(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {
        vy += g * 0.8;
        x += vx;
        y += vy;



        if (y <= groundY)
        {
            y = groundY;
            vy = -vy * restitution * 0.6;
            vx *= friction;

            if (fabs(vx) < 0.8)
            {
                flying = false;
                visible = false;
            }
            iPlaySound("assets/sounds/ball_bounce.wav");
        }
        if (x < 0 || x > 1919 || y > 1080)
        {
            flying = false;
            visible = false;
            return;
        }
    }
}
void updateSingleBird_hard(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {
        vy += g * 0.8;
        x += vx;
        y += vy;

        float birdVelocity = sqrt(vx * vx + vy * vy);
        float hittingAngle = atan2(vy, vx);

        if (y <= groundY)
        {
            y = groundY;
            vy = -vy * restitution * 0.6;
            vx *= friction;

            if (fabs(vx) < 0.8)
            {
                flying = false;
                visible = false;
            }
            iPlaySound("assets/sounds/ball_bounce.wav");
        }
        if (x < 0 || x > 1919 || y > 1080)
        {
            flying = false;
            visible = false;
            return;
        }
    }
}

bool Correct_username()
{
    for (int i = 0; i < userNameLen; i++)
    {
        if (userName[i] != ' ')
            return true; // sob input space hole false return korbe
    }
    return false;
}

void addScore(const std::string& name, int score) {
    
    char debugMsg[100];
    sprintf(debugMsg, "Adding: %s - %d", name.c_str(), score);
    iText(100, 120, debugMsg, GLUT_BITMAP_HELVETICA_12); 

    PlayerScore newScore;
    newScore.name = name;
    newScore.score = score;
    
  
    if (screen == 1) {
        newScore.level = "Easy";
    } 
    else if (screen == 3) {
        newScore.level = "Medium";
    } 
    else if (screen == 4) {
        newScore.level = "Hard";
    }
    else {
        newScore.level = "Unknown"; 
    }
    
    leaderboard.push_back(newScore);
    
   
    std::sort(leaderboard.begin(), leaderboard.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score;
    });
    
    // Keep only top 10 scores
    if (leaderboard.size() > 10) {
        leaderboard.resize(10);
    }
    
    // Debug: Show what was saved
    char saveMsg[100];
    sprintf(saveMsg, "Saved: %s - %d (%s)", newScore.name.c_str(), newScore.score, newScore.level.c_str());
    iText(100, 140, saveMsg, GLUT_BITMAP_HELVETICA_12); // Temporary
    
    // Immediately save to file
    saveLeaderboard("leaderboard.txt");
}


void loadLeaderboard(const std::string& filename) {
    leaderboard.clear();
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name, level;
        int score;
        while (file >> name >> score >> level) {
            PlayerScore ps;
            ps.name = name;
            ps.score = score;
            ps.level = level;
            leaderboard.push_back(ps);
        }
        file.close();
    }
    
    std::sort(leaderboard.begin(), leaderboard.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score;
    });
}



void saveLeaderboard(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "w");
    if (file) {
        for (const auto& ps : leaderboard) {
            fprintf(file, "%s %d\n", ps.name.c_str(), ps.score);
            
            
            char saveMsg[100];
            sprintf(saveMsg, "Saved: %s - %d", ps.name.c_str(), ps.score);
            iText(100, 140, saveMsg, GLUT_BITMAP_HELVETICA_12); // Temporary
        }
        fclose(file);
    } else {
        iText(100, 160, "ERROR: Can't open leaderboard file!", GLUT_BITMAP_HELVETICA_12);
    }
}

int getPlayerRank(const std::string& playerName, int playerScore)
{
    for (int i = 0; i < leaderboard.size(); i++)
    {
        if (leaderboard[i].name == playerName && leaderboard[i].score == playerScore)
        {
            return i + 1; 
        }
    }
    return -1; 
}

void drawScoreDisplay()
{
   
    iSetColor(0, 0, 0);
    iFilledRectangle(20, 1000, 200, 60);
    iSetColor(255, 255, 255);
    iRectangle(20, 1000, 200, 60);
    
    
    iSetColor(255, 215, 0); // Gold color
    char scoreText[50];
    sprintf(scoreText, "SCORE: %d", score);
    iTextBold(30, 1040, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
    
    
    iSetColor(255, 255, 255);
    char playerText[100];
    sprintf(playerText, "Player: %s", userName);
    iTextBold(30, 1010, playerText, GLUT_BITMAP_TIMES_ROMAN_24);
}

void resetBeams()
{
    for (int i = 0; i < 3; i++)
    {
        beamPositionsX[i] = 1088 - 46 + i * 250;
        beamPositionsY[i] = 398;
        beamVelocitiesX[i] = 0;
        beamVelocitiesY[i] = 0;
    }
}

void draweasy()
{

    iShowLoadedImage2(0, 0, &bg);
    iShowLoadedImage2(1850, 960, &pause, 70, 70);
    iShowLoadedImage2(208, 177, &gultiback);
    iShowSprite(&redSprite);
    iShowSprite(&verticalSprite);

    if (discodragging)
        drawRubberLines(bluebirdX[0], bluebirdY[0]);
    if (reddragging)
        drawRubberLines(redSpriteX, redSpriteY);
    if (yellowdragging)
        drawRubberLines(yellowbirdX, yellowbirdY);

    drawBirds_easy();

   
     if (discodragging)
        drawPathway(bluebirdX[0], bluebirdY[0], blue_vx[0], blue_vy[0]);
    if (reddragging)
        drawPathway(redSpriteX, redSpriteY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

    iShowLoadedImage2(208, 177, &gultifront);

    for (int i = 0; i < pillarCount; i++)
    {
        int baseX = 1088 + i * 250;

        iRotate(baseX + pillarwidth / 2, pillarY[i], pillarRotation[i]);
        iShowLoadedImage(baseX, pillarY[i], &woodVertical);
        iUnRotate();

      
        iShowLoadedImage(beamPositionsX[i], beamPositionsY[i], &woodHorizontal);
    }

    // Draw pigs
    for (int i = 0; i < pigCount; i++)
    {
        if (pigVisible[i])
            iShowLoadedImage(pigX[i], pigY[i], &pigimage);
    }

     // Draw new score display
    drawScoreDisplay();

}

void drawmedium()
{

    iShowLoadedImage(0, 0, &bg1);
    iShowLoadedImage2(265, 140, &gultiback, 200, 200);
    iShowLoadedImage2(1850, 960, &pause, 70, 70);
    
    
    if (bluedragging)
        drawRubberLines_medium(bluebirdX[0], bluebirdY[0]);
    if (greendragging)
        drawRubberLines_medium(redSpriteX, redSpriteY);
    if (yellowdragging)
        drawRubberLines_medium(yellowbirdX, yellowbirdY);
    drawBirds_medium();
    display_map1();
    if (bluedragging)
        drawPathway(bluebirdX[0], bluebirdY[0], blue_vx[0], blue_vy[0]);
    if (greendragging)
        drawPathway(redSpriteX, redSpriteY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);
    iShowLoadedImage2(265, 140, &gultifront, 200, 200);
    
    drawScoreDisplay();
}

void drawhard(){

    iShowLoadedImage2(0, 0, &bgHard, 1920, 1080);
    iShowLoadedImage2(208, 177 - 115, &gultiback, 200, 200);
    iShowLoadedImage2(1850, 960, &pause, 70, 70);

    if (bluedragging)
        drawRubberLines_hard(bluebirdX[0], bluebirdY[0] - 115);
    if (reddragging)
        drawRubberLines_hard(redSpriteX, redSpriteY - 115);
    if (pinkdragging)
        drawRubberLines_hard(yellowbirdX, yellowbirdY - 115);

    drawBirds_hard();
    
    display_map3();

    if (bluedragging)
        drawPathway_hard(bluebirdX[0], bluebirdY[0] - 115, blue_vx[0], blue_vy[0]);
    if (reddragging)
        drawPathway_hard(redSpriteX, redSpriteY - 115, red_vx, red_vy);
    if (pinkdragging)
        drawPathway_hard(yellowbirdX, yellowbirdY - 115, yellow_vx, yellow_vy);

    iShowLoadedImage2(208, 177 - 115, &gultifront, 200, 200);
    drawScoreDisplay();
}

void iDraw()
{

    iClear();
  
    if (!soundOn)
    {
        iStopAllSounds();
    }
    if (!isFullScreen)
        iToggleFullscreen();

    if (screen == 0)
    {
        drawMenu();
        iShowLoadedImage2(56, 56, &settings, 50, 50);
        iShowLoadedImage2(1840, 56, &cross_button, 50, 50);
    }
    else if (screen == 1)
    {
        draweasy();
        iShowLoadedImage2(740, 1030, &nameshowingBar, 350, 50);
        char usrnm[200];
        sprintf(usrnm, "%s is playing...", userName);
        iText(788, 1050, usrnm, GLUT_BITMAP_HELVETICA_18);
    }
    else if (screen == 2)
    {
        drawLevelSelect();
    }
    else if (screen == 3)
    {
        drawmedium();
        iShowLoadedImage2(740, 1030, &nameshowingBar, 350, 50);
        char usrnm[200];
        sprintf(usrnm, "%s is playing...", userName);
        iText(788, 1050, usrnm, GLUT_BITMAP_HELVETICA_18);
    }
    else if (screen == 4)
    {
        drawhard();
        iShowLoadedImage2(740, 1030, &nameshowingBar, 350, 50);
        char usrnm[200];
        sprintf(usrnm, "%s is playing...", userName);
        iText(788, 1050, usrnm, GLUT_BITMAP_HELVETICA_18);
    }
    else if (screen == 5)
    {
        iShowLoadedImage(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(677 - 152, 360 - 73, &credit2, 1050, 700);
        iShowLoadedImage2(56, 56, &previous, 50, 50);
    }
    else if (screen == 6) // in settings
    {   
        
        
        
        
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(677 - 152, 360 - 73, &credit1, 1050, 700);
        iShowLoadedImage2(56, 56, &previous, 50, 50);
        iSetColor(153, 204, 0);
        iShowText(755, 730, "SOUND", "assets/fonts/RubikDoodleShadow-Regular.ttf", 30);

        if (soundOn)
            iShowLoadedImage2(1115, 715, &sound1, 120, 55);
        else
            iShowLoadedImage2(1115, 715, &sound2, 120, 55);

        iShowText(755, 600, "INSTRUCTION", "assets/fonts/RubikDoodleShadow-Regular.ttf", 30);
        iShowLoadedImage2(1115, 585, &instruction, 50, 50);

        iShowText(755, 470, "ABOUT", "assets/fonts/RubikDoodleShadow-Regular.ttf", 30);
        iShowLoadedImage2(1115, 460, &about, 50, 50);
       
    }
    else if (screen == 7) // instructions
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(56, 56, &previous, 50, 50);
        iShowLoadedImage2(677 - 152, 360 - 73, &whiteCanvas);
        
        iTextBold(600, 711, "Press 'm' for going to Main-menu", GLUT_BITMAP_TIMES_ROMAN_24);
        iTextBold(600, 675, "Press 'q' to close the Application", GLUT_BITMAP_TIMES_ROMAN_24);
        iTextBold(600, 639, "Press 'r' to reset the Game state", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(600, 550, &bluePair, 80, 80);
        iSetColor(0,0,255);
        iTextBold(700, 605, "(Jay,Jake & Jim) ->Availabe in medium and hard level", GLUT_BITMAP_TIMES_ROMAN_24);
        iTextBold(700, 575, "Initially, Jay will be with us but after right-clicking,", GLUT_BITMAP_TIMES_ROMAN_24);
        iTextBold(700, 545, "Jake & Jim will also join us", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(600, 475, &redImg, 60, 60);
        iSetColor(255, 0 ,0);
        iTextBold(700, 498, "(Red) ->The main protagonist", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(600, 405, &rosco, 60, 60);
        iSetColor(153,51,102);
        iTextBold(700, 430, "(Rosco) ->The KHILADI 786 ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(600, 335, &stella, 60, 60);
        iSetColor(255,153,204);
        iTextBold(700, 362, "(Stella) ->The Pookie ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(1015, 476, &hall, 60, 60);
        iSetColor(0,255,0);
        iTextBold(1083, 500, "(Hal) ->Boomerang Guy ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(1015, 408, &yellowImg, 60, 60);
        iSetColor(255,255,0);
        iTextBold(1083, 430, "(Chuck) ->Speedy Guy ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowLoadedImage2(1015, 340, &blackImg, 60, 60);
        iSetColor(0,0,0);
        iTextBold(1083, 360, "(Bomb) ->The Exploder ", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else if (screen == 8)
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.7);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(56, 56, &previous, 50, 50);
        iShowLoadedImage2(677 - 152, 360 - 73, &about1);
        iShowLoadedImage2(1260, 360, &next_button, 50, 50);
    }
    else if (screen == 9)
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.6);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(677 - 152, 360 - 73, &about2);
        iShowLoadedImage2(582, 367, &previous, 50, 50);
        iShowLoadedImage2(1350, 780, &cross_button2, 50, 50);
    }
    else if (screen == 10)
    {
      iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.4);
        iFilledRectangle(0, 0, 1920, 1080);
        // iShowLoadedImage2(600, 250, &nameinput);
        iSetColor(0, 0,0);
        iShowText(700, 500, "Enter your name:",  "assets/fonts/DancingScript-Medium.ttf",  48);
        iRectangle(700, 460, 380, 30);
        iSetColor(255, 153, 204);
        iText(710, 465, userName, GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0, 0, 0);
        iTextBold(700, 400, "Press ENTER to continue",GLUT_BITMAP_TIMES_ROMAN_24);
        if (nameWarning)
        {
            iSetColor(255, 0, 0);
            iTextBold(700, 465, "Please, Enter your name", GLUT_BITMAP_TIMES_ROMAN_24);
        }

    }
    else if (screen == 11)
    {
        drawpause();
    }
   else if (screen == SCREEN_LEADERBOARD)
{
    iShowLoadedImage(0, 0, &menuBg);
    iSetTransparentColor(0, 0, 0, 0.6);
    iFilledRectangle(0, 0, 1920, 1080);
   
   
    iShowLoadedImage2(560, 200, &whiteCanvas, 800, 600);
    
   
    iSetColor(204, 153, 255); // Gold color for title
    iShowText(760, 690, "LEADERBOARD","assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf",38);
    
    
    iSetColor(0, 0, 0);
    iText(650, 650, "RANK", GLUT_BITMAP_HELVETICA_18);
    iText(750, 650, "PLAYER", GLUT_BITMAP_HELVETICA_18);
    iText(950, 650, "LEVEL", GLUT_BITMAP_HELVETICA_18);  // New column
    iText(1100, 650, "SCORE", GLUT_BITMAP_HELVETICA_18); // Moved score to the right
    
  
    iSetColor(100, 100, 100);
    iLine(600, 630, 1200, 630);

    
    int y = 600;
    for (int i = 0; i < leaderboard.size() && i < 10; i++) {
        // Alternate row colors
        if (i % 2 == 0) {
            iSetColor(240, 240, 240);
            iFilledRectangle(600, y - 15, 600, 30);
        }
        
        
        if (i == 0) iSetColor(255, 215, 0);      // Gold
        else if (i == 1) iSetColor(192, 192, 192); // Silver  
        else if (i == 2) iSetColor(205, 127, 50);  // Bronze
        else iSetColor(0, 0, 0);                   // Black
        
        char rankStr[10], scoreStr[20];
        sprintf(rankStr, "%d", i + 1);
        sprintf(scoreStr, "%d", leaderboard[i].score);
        
        iText(670, y, rankStr, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(750, y, leaderboard[i].name.c_str(), GLUT_BITMAP_TIMES_ROMAN_24);
        iText(950, y, leaderboard[i].level.c_str(), GLUT_BITMAP_TIMES_ROMAN_24); // New level display
        iText(1100, y, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
        y -= 40;
    
    }
    
   
    if (leaderboard.size() == 0) {
        iSetColor(100, 100, 100);
        iText(850, 500, "No scores yet! Play to set a record!", GLUT_BITMAP_HELVETICA_18);
    }
    
    // Back button
    iShowLoadedImage2(56, 56, &previous, 50, 50);
}
    if ((screen == 1 || screen == 3 || screen == 4) && showWin) {
       iSetTransparentColor(0,0,0,0.5);
        iFilledRectangle(0,0,1920,1080); 
    iShowLoadedImage2(530, 320, &win, 950, 650);
    // iStopAllSounds();
    iPlaySound("assets/sounds/winningTrack.wav", true);
    discovisible = false;
    redvisible = false;
    yellowvisible = false;
     char levelText[20];
    if (screen == 1) strcpy(levelText, "Easy Level");
    else if (screen == 3) strcpy(levelText, "Medium Level");
    else strcpy(levelText, "Hard Level");
    iSetColor(255, 255, 255); // White color for text
    iText(900, 580, levelText, GLUT_BITMAP_TIMES_ROMAN_24);
     
     
    iShowLoadedImage2(683, 491, &restart, 70, 70);
    iShowLoadedImage2(945, 491, &nextlevel, 70, 70);
    iShowLoadedImage2(1180, 491, &mainmenu, 70, 70);
   
    iSetColor(0, 0, 0); // Gold color
        char scoreText[50];
        sprintf(scoreText, "Your Score: %d", score);
        iText(900, 650, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
        // Show leaderboard position
    int rank = getPlayerRank(std::string(userName), score);
    if (rank != -1) {
        char rankText[100];
        if (rank <= 10) {
            sprintf(rankText, "Leaderboard Position: #%d", rank);
            iSetColor(255, 215, 0); // Gold color
        } else {
            sprintf(rankText, "Not in Top 10");
            iSetColor(100, 100, 100); // Gray color
        }
        iText(850, 600, rankText, GLUT_BITMAP_HELVETICA_18);
    }
      
}
if ((screen == 1 || screen == 3 || screen == 4) && showGameOver) {
    iShowLoadedImage2(530, 320, &gameover, 950, 650);
    iPlaySound("assets/sounds/failureTrack.wav", true);
    
    iShowLoadedImage2(683, 491, &restart, 70, 70);
    iShowLoadedImage2(945, 491, &nextlevel, 70, 70);
    iShowLoadedImage2(1180, 491, &mainmenu, 70, 70);
    if (screen == 1) 
    {
        discovisible = false;
        redvisible = false;
        yellowvisible = false;
    }
    else if (screen == 3) 
    {
        bluevisible_arr[0] = false;
        greenvisible = false;
        blackvisible = false;
    }
    else if (screen == 4)
    {
        bluevisible_arr[0] = false;
        pinkvisible = false;
        redvisible = false;
    }
    

 char levelText[20];
    if (screen == 1) strcpy(levelText, "Easy Level");
    else if (screen == 3) strcpy(levelText, "Medium Level");
    else strcpy(levelText, "Hard Level");
    iSetColor(255, 255, 255); // White color for text
    iText(900, 580, levelText, GLUT_BITMAP_TIMES_ROMAN_24);
   
         iSetColor(0, 0, 0);
    char scoreText[50];
    sprintf(scoreText, "Your Score: %d", score);
    iText(900, 650, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
   
    int rank = getPlayerRank(std::string(userName), score);
    if (rank != -1) {
        char rankText[100];
        if (rank <= 10) {
            sprintf(rankText, "Leaderboard Position: #%d", rank);
            iSetColor(255, 215, 0); // Gold color
        } else {
            sprintf(rankText, "Not in Top 10");
            iSetColor(100, 100, 100); // Gray color
        }
        iText(850, 600, rankText, GLUT_BITMAP_HELVETICA_18);
    }
    }
    
     if ((screen == 1 || screen == 3 || screen == 4) && (showWin || showGameOver)) {
        // Semi-transparent overlay
        iSetTransparentColor(0, 0, 0, 0.7);
        iFilledRectangle(0, 0, 1920, 1080);
        
        if (showWin) {
            iShowLoadedImage2(530, 320, &win, 950, 650);
            iSetColor(255, 215, 0); // Gold
            iTextBold(930, 700, "VICTORY!", GLUT_BITMAP_TIMES_ROMAN_24);
        } else {
            iShowLoadedImage2(530, 320, &gameover, 950, 650);
            iSetColor(255, 0, 0); // Red
            iTextBold(930, 700, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
        }

        // Score display
        iSetColor(255, 255, 255);
        char scoreText[50];
        sprintf(scoreText, "Score: %d", score);
        iTextBold(930, 650, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

        // Level info
        char levelText[50];
        if (screen == 1) strcpy(levelText, "Easy Level");
        else if (screen == 3) strcpy(levelText, "Medium Level");
        else strcpy(levelText, "Hard Level");
        iTextBold(1200, 700, levelText, GLUT_BITMAP_TIMES_ROMAN_24);

       
        iShowLoadedImage2(683, 491, &restart, 70, 70);    // Restart
        iShowLoadedImage2(945, 491, &nextlevel, 70, 70);  // Next level
        iShowLoadedImage2(1180, 491, &mainmenu, 70, 70);  // Main menu

       
        int rank = getPlayerRank(std::string(userName), score);
        if (rank != -1) {
            char rankText[100];
            if (rank <= 10) {
                sprintf(rankText, "Rank: #%d", rank);
                iSetColor(255, 215, 0); // Gold
            } else {
                sprintf(rankText, "Rank: Top %d%%", (rank * 100) / leaderboard.size());
                iSetColor(200, 200, 200); // Silver
            }
            iTextBold(1200, 650, rankText, GLUT_BITMAP_TIMES_ROMAN_24);
        }
    }
   
}


void updateBird() {
   
    updatePhysics();

    
    if (screen == 1) { // Easy Level
    
    updateSingleBird(bluebirdX[0], bluebirdY[0], blue_vx[0], blue_vy[0], discoflying, discovisible);
    updateSingleBird(redSpriteX, redSpriteY, red_vx, red_vy, redflying, redvisible);
    updateSingleBird(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowflying, yellowvisible);

    
    bool allPigsGone = true;
    bool birdsFinished = true;

   
    for (int i = 0; i < pigCount; i++) {
        if (pigVisible[i]) {
            allPigsGone = false;
            break;
        }
    }

    
    if ((discovisible || discoflying) || 
        (redvisible || redflying) || 
        (yellowvisible || yellowflying)) {
        birdsFinished = false;
    }

    
    if (allPigsGone && !showWin && !showGameOver) {
        showWin = true;
        showGameOver = false;
        score += 1000; 
        if (!scoreSaved) {
            addScore(std::string(userName), score);
            saveLeaderboard("leaderboard.txt");
            scoreSaved = true;
        }
    }
    // Handle loss condition
    else if (birdsFinished && !allPigsGone && !showGameOver && !showWin) {
        showGameOver = true;
        showWin = false;
        if (!scoreSaved) {
            addScore(std::string(userName), score);
            saveLeaderboard("leaderboard.txt");
            scoreSaved = true;
        }
    }
}
    else if (screen == 3) { // Medium Level
        for (int i = 0; i < 3; i++) {
            if (blueflying[i] && bluevisible_arr[i]) {
                updateSingleBird_medium(bluebirdX[i], bluebirdY[i], blue_vx[i], blue_vy[i], blueflying[i], bluevisible_arr[i]);
            }
        }
        updateSingleBird_medium(redSpriteX, redSpriteY, red_vx, red_vy, greenflying, greenvisible);
        updateSingleBird_medium(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowflying, blackvisible);

       
        bool allPigsGone = true;
        bool birdsFinished = true;

        
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLLUMS; c++) {
                if (map1[r][c] == 2) { 
                    allPigsGone = false;
                    break;
                }
            }
            if (!allPigsGone) break;
        }

       
        for (int i = 0; i < 3; i++) {
            if (bluevisible_arr[i] || blueflying[i]) {
                birdsFinished = false;
                break;
            }
        }
        if (greenvisible || greenflying) birdsFinished = false;
        if (blackvisible || yellowflying) birdsFinished = false;

       
        if (allPigsGone && !showWin && !showGameOver) {
            showWin = true;
            showGameOver = false;
            if (!scoreSaved) {
                addScore(std::string(userName), score);
                saveLeaderboard("leaderboard.txt");
                scoreSaved = true;
            }
        }
        else if (birdsFinished && !allPigsGone && !showGameOver && !showWin) {
            showGameOver = true;
            showWin = false;
            if (!scoreSaved) {
                addScore(std::string(userName), score);
                saveLeaderboard("leaderboard.txt");
                scoreSaved = true;
            }
        }
    }
    else if (screen == 4) { // Hard Level
        for (int i = 0; i < 3; i++) {
            if (blueflying[i] && bluevisible_arr[i]) {
                updateSingleBird_hard(bluebirdX[i], bluebirdY[i], blue_vx[i], blue_vy[i], blueflying[i], bluevisible_arr[i]);
            }
        }
        updateSingleBird_hard(redSpriteX, redSpriteY, red_vx, red_vy, redflying, redvisible);
        updateSingleBird_hard(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, pinkflying, pinkvisible);

        
        bool allPigsGone = true;
        bool birdsFinished = true;

       
        int blockSize = 30;
        int startX = 1100;
        int startY = 620;

        for (int r = 0; r < 20; r++) {
            for (int c = 0; c < 20; c++) {
                int x = startX + c * blockSize;
                int y = startY - r * blockSize;

               
                bool hitByBird = false;
                for (int i = 0; i < 3; i++) {
                    if (blueflying[i] && checkCollision(bluebirdX[i], bluebirdY[i], birdwidth, birdheight, x, y, blockSize, blockSize)) {
                        hitByBird = true;
                        break;
                    }
                }
                if (!hitByBird && redflying && checkCollision(redSpriteX, redSpriteY, birdwidth, birdheight, x, y, blockSize, blockSize)) {
                    hitByBird = true;
                }
                if (!hitByBird && pinkflying && checkCollision(yellowbirdX, yellowbirdY, birdwidth, birdheight, x, y, blockSize, blockSize)) {
                    hitByBird = true;
                }

                if (hitByBird) {
                    
                    if (map3[r][c] == 5) { 
                        score += 500;
                        map3[r][c] = 0; 
                    }
                    else if (map3[r][c] == 6) { 
                        score += 700;
                        map3[r][c] = 0; 
                    }
                    else if (map3[r][c] >= 1 && map3[r][c] <= 4 || map3[r][c] == 7) { 
                        score += 100;
                        map3[r][c] = 0; 
                    }
                }

                if (map3[r][c] == 5 || map3[r][c] == 6) {
                    allPigsGone = false;
                }
            }
        }

       
        for (int i = 0; i < 3; i++) {
            if (bluevisible_arr[i] || blueflying[i]) {
                birdsFinished = false;
                break;
            }
        }
        if (redvisible || redflying) birdsFinished = false;
        if (pinkvisible || pinkflying) birdsFinished = false;

        
        if (allPigsGone && !showWin && !showGameOver) {
            showWin = true;
            showGameOver = false;
            score += 2000; 
            if (!scoreSaved) {
                addScore(std::string(userName), score);
                saveLeaderboard("leaderboard.txt");
                scoreSaved = true;
            }
        }
        else if (birdsFinished && !allPigsGone && !showGameOver && !showWin) {
            showGameOver = true;
            showWin = false;
            if (!scoreSaved) {
                addScore(std::string(userName), score);
                saveLeaderboard("leaderboard.txt");
                scoreSaved = true;
            }
        }
    }

    
    for (int i = 0; i < pigCount; i++) {
        updatePigMotion(i);
    }

}



void iMouseMove(int mx, int my)
{ if (screen == 0) { 
        hoverPlay   = (mx >= 110 && mx <= 240 && my >= 237 && my <= 237 + 120);
        hoverExit   = (mx >= 110 && mx <= 240 && my >= 177 && my <= 177 + 120);
        hoverCredit = (mx >= 110 && mx <= 240 && my >= 110 && my <= 110 + 120);
    }
    if (screen == 2) { 
    hoverEasy   = (mx >= 85 && mx <= 225 && my >= 210 && my <= 340);
    hoverMedium = (mx >= 85 && mx <= 225 && my >= 155 && my <= 285);
    hoverHard   = (mx >= 85 && mx <= 225 && my >= 95  && my <= 225);
}

    cursorX = mx;
    cursorY = my;
    sprintf(cursorStr, "Cursor: (%d, %d)", cursorX, cursorY);
    if (screen == 1 )
    {
        if (discodragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            bluebirdX[0] = mx;
            bluebirdY[0] = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            blue_vx[0] = v * cos(angle) * 0.5;
            blue_vy[0] = v * sin(angle) * 0.5;
        }
        if (reddragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            redSpriteX = mx;
            redSpriteY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            red_vx = v * cos(angle) * 0.5;
            red_vy = v * sin(angle) * 0.5;
        }
        if (yellowdragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            yellowbirdX = mx;
            yellowbirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            yellow_vx = v * cos(angle) * 0.5;
            yellow_vy = v * sin(angle) * 0.5;
        }
    }
    else if ( screen == 3)
    {
        if (bluedragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            bluebirdX[0] = mx;
            bluebirdY[0] = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            blue_vx[0] = v * cos(angle) * 0.5;
            blue_vy[0] = v * sin(angle) * 0.5;
        }
        if (greendragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            redSpriteX = mx;
            redSpriteY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            red_vx = v * cos(angle) * 0.5;
            red_vy = v * sin(angle) * 0.5;
        }
        if (yellowdragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            yellowbirdX = mx;
            yellowbirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            yellow_vx = v * cos(angle) * 0.5;
            yellow_vy = v * sin(angle) * 0.5;
        }
    }
       if (screen == 1 || screen == 3 || screen == 4)
    {
        if (bluedragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            bluebirdX[0] = mx;
            bluebirdY[0] = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            blue_vx[0] = v * cos(angle) * 0.5;
            blue_vy[0] = v * sin(angle) * 0.5;
        }
        if (reddragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            redSpriteX = mx;
            redSpriteY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            red_vx = v * cos(angle) * 0.5;
            red_vy = v * sin(angle) * 0.5;
        }
        if (pinkdragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            yellowbirdX = mx;
            yellowbirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            yellow_vx = v * cos(angle) * 0.5;
            yellow_vy = v * sin(angle) * 0.5;
        }
    }
}


void resetEasyLevel() {
    score = 0;
    scoreSaved = false;
    showWin = false;
    showGameOver = false;
    
    selectedBird = -1;

    // Reset all blue birds
    
        bluebirdX[0] = 20;
        bluebirdY[0] = 194;
        blue_vx[0] = blue_vy[0] = 0;
        discovisible = true;
    discoflying = false;
    discodragging = false;
        
     

    // Reset red bird
    redSpriteX = 100;
    redSpriteY = 194;
    red_vx = red_vy = 0;
    redflying = false;
    redvisible = true;

    // Reset yellow bird
    yellowbirdX = 180;
    yellowbirdY = 200;
    yellow_vx = yellow_vy = 0;
    yellowflying = false;
    yellowvisible = true;

    discodragging = reddragging = yellowdragging = false;

    // Reset pigs
    for (int i = 0; i < pigCount; i++) {
        pigVisible[i] = true;
        pigFalling[i] = false;
        pigX[i] = 1070 + i * 250;
        pigY[i] = 420;
        pigVX[i] = pigVY[i] = 0;
    }

    // Reset pillars and beams
    resetBeams();
    for (int i = 0; i < pillarCount; i++) {
        pillarRotation[i] = pillarAngVelocity[i] = 0;
        pillarRotating[i] = false;
    }
}

void resetMediumLevel() {
    score = 0;
    scoreSaved = false;
    showWin = false;
    showGameOver = false;
    blueSplit = false;
    selectedBird = -1;

   
    for (int i = 0; i < 3; i++) {
        bluebirdX[i] = 20;
        bluebirdY[i] = 194;
        blue_vx[i] = blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0); 
    }

    
    redSpriteX = 100;
    redSpriteY = 194;
    red_vx = red_vy = 0;
    greenflying = false;
    greenvisible = true;
    greendragging = false;

    
    yellowbirdX = 180;
    yellowbirdY = 200;
    yellow_vx = yellow_vy = 0;
    yellowflying = false;
    blackvisible = true;
    yellowdragging = false;

    
    bluedragging = false;

    
    int initialMap1[ROWS][COLLUMS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    memcpy(map1, initialMap1, sizeof(map1));
}

void resetHardLevel() {
    score = 0;
    scoreSaved = false;
    showWin = false;
    showGameOver = false;
    blueSplit = false;
    selectedBird = -1;

    
    for (int i = 0; i < 3; i++) {
        bluebirdX[i] = 20;
        bluebirdY[i] = 194;
        blue_vx[i] = blue_vy[i] = 0;
        blueflying[i] = false;
        bluevisible_arr[i] = (i == 0); 
    }

   
    redSpriteX = 100;
    redSpriteY = 194;
    red_vx = red_vy = 0;
    redflying = false;
    redvisible = true;
    reddragging = false;

    
    yellowbirdX = 180;
    yellowbirdY = 200;
    yellow_vx = yellow_vy = 0;
    pinkflying = false;
    pinkvisible = true;
    pinkdragging = false;

    
    bluedragging = false;

    
    int initialMap3[20][20] = {
        {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 7, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0},
        {0, 0, 4, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0},
        {0, 4, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0},
        {4, 0, 0, 0, 0, 0, 4, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 6, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 2, 6, 0, 0, 0, 0, 2},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    memcpy(map3, initialMap3, sizeof(map3));
}
void iMouse(int button, int state, int mx, int my)
{
    if (screen == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 123 && mx <= 224 && my >= 282 && my <= 318) 
        {
            showWin = false;
            showGameOver = false;
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 10;
            userName[0] = '\0';
            userNameLen = 0;
            
    if (currentScreen == 1) resetEasyLevel();
    else if (currentScreen == 3) resetMediumLevel();
    else if (currentScreen == 4) resetHardLevel();

        }
        else if (mx >= 1830 && mx <= 1830 + 70 && my >= 130 && my <= 130 + 70) 
        {
            showWin = false;
            showGameOver = false;
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = SCREEN_LEADERBOARD; // or screen = 20;
            iStopAllSounds();
            iPlaySound("assets/sounds/settingsSound.wav", true);
        }

        else if (mx >= 123 && mx <= 224 && my >= 219 && my <= 258) //play button
        {

            iPlaySound("assets/sounds/menu_sound.wav", false);
            exit(0);
        }

        else if (mx >= 123 && mx <= 224 && my >= 156 && my <= 191) // credit button
        {
            showWin = false;
            showGameOver = false;
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 5;
            iStopAllSounds();
            iPlaySound("assets/sounds/Aura.wav", true);
        }
        else if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) //  settings button
        {
            showWin = false;
            showGameOver = false;
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
            iStopAllSounds();
            iPlaySound("assets/sounds/settingsSound.wav", true);
        }
        else if (mx >= 1842 && mx <= 1885 && my >= 65 && my <= 103) // exit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            iCloseWindow();
        }
    }

    else if (screen == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 97 && mx <= 211 && my >= 258 && my <= 294) // easy level
        {
            showWin = false;
            showGameOver = false;
            difficultylevel = 1;
            screen = 1;
        
            resetEasyLevel(); // Explicitly reset the level
            resetBeams();
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 97 && mx <= 211 && my >= 206 && my <= 241) // medium level
        {
            showWin = false;
            showGameOver = false;
            difficultylevel = 2;
            screen = 3;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 97 && mx <= 211 && my >= 144 && my <= 178) // hard button button
        {
            showWin = false;
            showGameOver = false;
            difficultylevel = 3;
            screen = 4;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
    }

    else if (( screen == 3 || screen == 4) && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // Blue bird split
        if (blueflying[0] && !blueSplit)
        {
            blueSplit = true;
            float angle = atan2(blue_vy[0], blue_vx[0]);
            float speed = sqrt(blue_vx[0] * blue_vx[0] + blue_vy[0] * blue_vy[0]);
            for (int i = 0; i < 3; i++)
            {
                bluebirdX[i] = bluebirdX[0];
                bluebirdY[i] = bluebirdY[0];
                blueflying[i] = true;
                bluevisible_arr[i] = true;
            }
            blue_vx[0] = speed * cos(angle);
            blue_vy[0] = speed * sin(angle);
            blue_vx[1] = speed * cos(angle + 0.15f);
            blue_vy[1] = speed * sin(angle + 0.15f);
            blue_vx[2] = speed * cos(angle - 0.15f);
            blue_vy[2] = speed * sin(angle - 0.15f);
        }
    }

   
   
    

    else if ((screen == 1) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (selectedBird == -1 && mx >= bluebirdX[0] && mx <= bluebirdX[0] + 70 && my >= bluebirdY[0] && my <= bluebirdY[0] + 70)
        {
            discodragging = true;
            selectedBird = 0; // rosco
      
            iPlaySound("assets/sounds/bird_01_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= redSpriteX && mx <= redSpriteX + 70 && my >= redSpriteY && my <= redSpriteY + 70)
        {
            reddragging = true;
            selectedBird = 1; // red bird
            iPlaySound("assets/sounds/bird_02_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= yellowbirdX && mx <= yellowbirdX + 70 && my >= yellowbirdY && my <= yellowbirdY + 70)

        {
            yellowdragging = true;
            selectedBird = 2; // yellow bird
            iPlaySound("assets/sounds/bird_03_select.wav", false);
        }
        else if (mx >= 1850 && mx <= 1850 + 70 && my >= 960 && my <= 960 + 70)
        {   currentScreen=screen;
            screen = 11;
        }
    }
        else if ((screen == 3) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (selectedBird == -1 && mx >= bluebirdX[0] && mx <= bluebirdX[0] + 50 && my >= bluebirdY[0] && my <= bluebirdY[0]  + 50)
        {
            bluedragging = true;
            selectedBird = 0;
            iPlaySound("assets/sounds/bird_01_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= redSpriteX && mx <= redSpriteX + 50 && my >= redSpriteY&& my <= redSpriteY + 50)
        {
            greendragging = true;
            selectedBird = 1;
            iPlaySound("assets/sounds/bird_02_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= yellowbirdX && mx <= yellowbirdX + 50 && my >= yellowbirdY && my <= yellowbirdY + 50)

        {
            yellowdragging = true;
            selectedBird = 2;
            iPlaySound("assets/sounds/bird_03_select.wav", false);
        }
          else if (mx >= 1850 && mx <= 1850 + 70 && my >= 960 && my <= 960 + 70) // Pause button
        {
            currentScreen = screen; // Remember current screen
            screen = 11; // Go to pause screen
            iPlaySound("assets/sounds/menu_sound.wav", false);
        }
    }
    else if ((screen == 4) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (selectedBird == -1 && mx >= bluebirdX[0] && mx <= bluebirdX[0] + 50 && my >= bluebirdY[0] - 115 && my <= bluebirdY[0] - 115 + 50)
        {
            bluedragging = true;
            selectedBird = 0;
            iPlaySound("assets/sounds/bird_01_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= redSpriteX && mx <= redSpriteX + 50 && my >= redSpriteY - 115 && my <= redSpriteY - 115 + 50)
        {
            reddragging = true;
            selectedBird = 1;
            iPlaySound("assets/sounds/bird_02_select.wav", false);
        }
        else if (selectedBird == -1 && mx >= yellowbirdX && mx <= yellowbirdX + 50 && my >= yellowbirdY - 115 && my <= yellowbirdY - 115 + 50)

        {
            pinkdragging = true;
            selectedBird = 2;
            iPlaySound("assets/sounds/bird_03_select.wav", false);
        }
         else if (mx >= 1850 && mx <= 1850 + 70 && my >= 960 && my <= 960 + 70)
        {   currentScreen=screen;
            screen = 11;
        }
    }

    else if ((screen == 1 || screen == 3 || screen == 4) && button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        if (bluedragging)
        {
            bluedragging = false;
            blueflying[0] = true;
            iPlaySound("assets/sounds/bird_01_flying.wav", false);
            selectedBird = -1;
        }
        else if (reddragging)
        {
            reddragging = false;
            redflying = true;
            iPlaySound("assets/sounds/bird_02_flying.wav", false);
            selectedBird = -1;
        }
        else if (yellowdragging)
        {
            yellowdragging = false;
            yellowflying = true;
            iPlaySound("assets/sounds/bird_03_flying.wav", false);
            selectedBird = -1;
        }
          else if (discodragging)
        {
            discodragging = false;
            discoflying = true;
            iPlaySound("assets/sounds/bird_03_flying.wav", false);
            selectedBird = -1;
        }
          else if (greendragging)
        {
            greendragging = false;
            greenflying = true;
            iPlaySound("assets/sounds/bird_03_flying.wav", false);
            selectedBird = -1;
        }
          else if (pinkdragging)
        {
            pinkdragging = false;
            pinkflying = true;
            iPlaySound("assets/sounds/bird_03_flying.wav", false);
            selectedBird = -1;
        }
    }
    else if ((screen == 5) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // credit- previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            showWin = false;
            showGameOver = false;
            // iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
    }
    else if (screen == 6 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // settings- previous
        {
            showWin = false;
            showGameOver = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
        else if (mx >= 1111 && mx <= 1283 && my >= 718 && my <= 767) // sound button
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            soundOn = !soundOn;
            if (soundOn)
                iPlaySound("assets/sounds/settingsSound.wav", true);/////////////////
            else
                iPlaySound(0, 0, 0);
            showWin = false;
            showGameOver = false;
        }
        else if (mx >= 1118 && mx <= 1160 && my >= 593 && my <= 635) // instruction button
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 7;
            showWin = false;
            showGameOver = false;
        }
        else if (mx >= 1118 && mx <= 1160 && my >= 465 && my <= 500) 
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 8;
            showWin = false;
            showGameOver = false;
        }
    }
    else if ((screen == 7 || screen == 8) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // about & instruction- previous
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
            showWin = false;
            showGameOver = false;
        }
        else if (mx >= 1264 && mx <= 1307 && my >= 367 && my <= 408) // for going about_2
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 9;
            showWin = false;
            showGameOver = false;
        }
    }
    else if ((screen == 9) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (mx >= 585 && mx <= 626 && my >= 374 && my <= 413) // about_2 previous
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 8;
            showWin = false;
            showGameOver = false;
        }
        else if (mx >= 1353 && mx <= 1387 && my >= 790 && my <= 830) // about_2 previous
        {
            
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
            showWin = false;
            showGameOver = false;
        }
    }
 
else if (screen == 11 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
   
    if (mx >= 699 && mx <= 699 + 70 && my >= 555 && my <= 555 + 70) {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        screen = currentScreen; 
    }
    
   if (mx >= 911 && mx <= 911 + 70 && my >= 555 && my <= 555 + 70) {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        if (currentScreen == 1) resetEasyLevel();
        else if (currentScreen == 3) resetMediumLevel();
        else if (currentScreen == 4) resetHardLevel();
        screen = currentScreen;
        showWin = false;
        showGameOver = false;
    }
   
    else if (mx >= 1121 && mx <= 1121 + 70 && my >= 555 && my <= 555 + 70) {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        screen = 0; 
        
    if (currentScreen == 1) resetEasyLevel();
    else if (currentScreen == 3) resetMediumLevel();
    else if (currentScreen == 4) resetHardLevel();
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true, 20); 
    }
}
   
else if ((screen == 1 || screen == 3 || screen == 4) && (showWin || showGameOver) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
{
    if (mx >= 683 && mx <= 683 + 70 && my >= 491 && my <= 491 + 70) 
    {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        
        if (screen == 1) // Easy level
        {
            resetEasyLevel();
        }
        else if (screen == 3) // Medium level
        {
            resetMediumLevel();
        }
        else if (screen == 4) // Hard level
        {
            resetHardLevel();
        }
       
        showWin = false;
        showGameOver = false;
        blueSplit = false;
        selectedBird = -1;
        
      
        for (int i = 0; i < 3; i++) 
        {
            bluevisible_arr[i] = (i == 0); 
            blueflying[i] = false;
        }
        redvisible = true;
        redflying = false;
        yellowvisible = true; ///////////////
        yellowflying = false;
        
        if (musicOn)
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
    }
    else if (mx >= 945 && mx <= 945 + 70 && my >= 491 && my <= 491 + 70) // Next level button
    {
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        
    }
    else if (mx >= 1180 && mx <= 1180 + 70 && my >= 491 && my <= 491 + 70) // Main menu button
    {
        // proto 
    }
}
  else if (screen == SCREEN_LEADERBOARD && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
{
    if (mx >= 56 && mx <= 106 && my >= 56 && my <= 106) // back button
    {
        
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0; // back to menu
            showWin = false;
            showGameOver = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true); // Play menu music
        }
}
    else if (showWin && mx >= 1180 && mx <= 1180 + 70 && my >= 491 && my <= 491 + 70 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        screen = 0; // Go to Menu Page
        showWin = false;
        showGameOver = false;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
    }
    else if (showGameOver && mx >= 800 && mx <= 1000 && my >= 330 && my <= 380 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        screen = 0; // Go to Menu Page
        showWin = false;
        showGameOver = false;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
    }
    else if (!(screen == 0 || screen == 2 || screen == 5 || screen == 6 || screen == 7 || screen == 8 || screen == 9 || screen == 10 || screen == 11))
    {
        showWin = false;
        showGameOver = false;
    }

   
    else if (showWin && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // Restart Button (Left)
        if (mx >= 683 && mx <= 753 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            score = 0; // Reset score
            if (screen == 1) resetEasyLevel();
            else if (screen == 3) resetMediumLevel();
            else if (screen == 4) resetHardLevel();
            showWin = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
  
else if (mx >= 945 && mx <= 1015 && my >= 491 && my <= 561) { // Next level button
    iPlaySound("assets/sounds/menu_sound.wav", false);

    showWin = false;
    showGameOver = false;
    blueSplit = false;
    selectedBird = -1;
    score = 0;
    scoreSaved = false;
    iStopAllSounds();
    if (screen == 1) { //  easy to medium
        screen = 3;
        resetMediumLevel();
    }
    else if (screen == 3) { //  medium to hard
        screen = 4;
        resetHardLevel();
    }
    else if (screen == 4) { // hard back to easy
        screen = 1;
        resetEasyLevel();
    }
    
    
    
    // Reset all birds
    for (int i = 0; i < 3; i++) {
        bluevisible_arr[i] = (i == 0);
        blueflying[i] = false;
    }
    redvisible = true;
    redflying = false;
    yellowvisible = true;
    yellowflying = false;
    
    iStopAllSounds();
    iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
}

        // Main Menu Button (Right)
        else if (mx >= 1180 && mx <= 1250 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            score = 0; // Reset score
            showWin = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
    }
    
    else if (showGameOver && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // Restart Button (Left)
        if (mx >= 683 && mx <= 753 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            score = 0; // Reset score
            if (screen == 1) resetEasyLevel();
            else if (screen == 3) resetMediumLevel();
            else if (screen == 4) resetHardLevel();
            showGameOver = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
        // Next Level Button (Middle)
          if (mx >= 945 && mx <= 1015 && my >= 491 && my <= 561) {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        score = 0; // Reset score for the next level
        if (screen == 1) {
            screen = 3; 
            resetMediumLevel();
        }
        else if (screen == 3) {
            screen = 4; 
            resetHardLevel(); 
        }
        else if (screen == 4) {
            screen = 1; 
            resetEasyLevel();
        }
        showWin = false;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
    }
        
        else if (mx >= 1180 && mx <= 1250 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            score = 0; // Reset score
            showGameOver = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
    }
        else if (!(screen ==0 || screen ==2 || screen ==5 || screen ==6 || screen ==7 || screen ==8 || screen ==9 || screen ==10 || screen ==11 ))
          {
            showWin = false;
            showGameOver = false;
          }
         if ((showWin || showGameOver) && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // Restart Button (Left)
        if (mx >= 683 && mx <= 753 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            if (screen == 1) resetEasyLevel();
            else if (screen == 3) resetMediumLevel();
            else if (screen == 4) resetHardLevel();
            showWin = showGameOver = false;
        }
        // Next Level Button (Middle)
        else if (mx >= 945 && mx <= 1015 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            if (screen == 1) screen = 3; // Easy → Medium
            else if (screen == 3) screen = 4; // Medium → Hard
            else if (screen == 4) screen = 1; // Hard → Easy (loop)
            if (screen == 1) resetEasyLevel();
            else if (screen == 3) resetMediumLevel();
            else if (screen == 4) resetHardLevel();
            showWin = showGameOver = false;
        }
        // Main Menu Button (Right)
        else if (mx >= 1180 && mx <= 1250 && my >= 491 && my <= 561) {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            showWin = showGameOver = false;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
    }
    else if (screen == 4 && (showWin || showGameOver) && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    if (mx >= 683 && mx <= 753 && my >= 491 && my <= 561) { // Restart button
        iPlaySound("assets/sounds/menu_sound.wav", false);
        resetHardLevel();
        showWin = false;
        showGameOver = false;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
    }
}

   



}
   

void iKeyboard(unsigned char key, int state)
{
    if (state != 0)
        return;

    if (key == 'r' && screen!=10)
    {
        iPlaySound("assets/sounds/menu_sound.wav", false);
        if (currentScreen == 1) resetEasyLevel();
        else if (currentScreen == 3) resetMediumLevel();
        else if (currentScreen == 4) resetHardLevel();
        screen = currentScreen;
        showWin = false;
        showGameOver = false;
    }
    if (key == 'q' && screen!=10)
        iCloseWindow();

    if (key == 'm' && screen!=10)
    {
        screen = 0;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        showWin = false;
        showGameOver = false;
    }
   if (screen == 10) {
    if (key == '\r' || key == '\n') { // Enter key
        if (Correct_username()) {
            screen = 2;
        } else {
            nameWarning = true;
        }
    }
    else if (key == '\b') { // Backspace
        if (userNameLen > 0) {
            userName[--userNameLen] = '\0';
        }
    }
    else if ((key >= 32) && (key <= 126)) { // Printable characters
        if (userNameLen < 99) {
            userName[userNameLen++] = key;
            userName[userNameLen] = '\0';
            nameWarning = false;
        }
    }
}
}
void iSpecialKeyboard(unsigned char key, int state)
{
    if (key == GLUT_KEY_END)
        exit(0);
}

void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

int main(int argc, char *argv[])
{   
     
    glutInit(&argc, argv);
    loadResources();
    loadLeaderboard("leaderboard.txt"); 
    iInitializeSound();
    iPlaySound("assets/sounds/angry_birds_2.wav", true);
    iSetTimer(100, updateBird);
    iOpenWindow(1920, 1080, "Angry Birds - BUET PROJECT");
    return 0;
}
