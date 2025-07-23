#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include "iSound.h"
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
screen =11 -> Medium level cleared



*/

#define ROWS 10
#define COLLUMS 20
#define BUET_ROWS 7
#define BUET_COLS 25

// Game State
int screen = 0, currentScreen = -1, score = 0;
int difficultylevel = 0;
bool soundOn = true, musicOn = true;
char userName[100] = "";
int userNameLen = 0;
bool writtingName = false, nameWarning = false;
bool medium_clear = false, easy_clear = false, hard_clear = false;
int pigCounter = 0;

// Bird Physics and States
bool bluevisible = true, redvisible = true, yellowvisible = true, blackvisible = true;
int bluebirdX = 20, bluebirdY = 194;
int redSpriteX = 100, redSpriteY = 194;
int yellowbirdX = 180, yellowbirdY = 200;
float blue_vx = 0, blue_vy = 0;
float red_vx = 0, red_vy = 0;
float yellow_vx = 0, yellow_vy = 0;
bool blueflying = false, redflying = false, yellowflying = false;
bool bluedragging = false, reddragging = false, yellowdragging = false;
float restitution = 0.6f, friction = 0.8f, angularFriction = 0.97f;

int currentBird = 0, selectedBird = -1; // 0-> blue, 1-> red, 2-> yellow
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;
int birdheight = 63, birdwidth = 65;

// pig info's:
const int pigCount = 6;
int pigX[pigCount] = {1070, 1195, 1320, 1445, 1570, 1695};
int pigY[pigCount] = {420, 316, 420, 316, 420, 316};
float pigVX[pigCount] = {}, pigVY[pigCount] = {};
bool pigVisible[pigCount] = {true, true, true, true, true, true};
bool pigFalling[pigCount] = {false, false, false, false, false, false};
int pigheight = 60, pigwidth = 60;

// Cursor
int cursorX = -1, cursorY = -1;
char cursorStr[30];

// Assets
Image bg, gultiback, gultifront, map_block, map_mosaic, map_stone, woodblock, realwoodSq, realwoodRect,
    menuBg, blueImg, redImg, yellowImg, bg1, woodHorizontal, woodVertical, realwoodSt, realiceSq, realiceRect,
    woodHorizontal2, woodVertical2, rock, blackImg, bgHard, settings, previous, realiceSt, nameshowingBar,
    menubutton, levelbutton, scorebutton, pigimage, credit2, creditBack, credit1, cross_button, mediumCleared,
    sound1, sound2, about, faq, instruction, whiteCanvas, about1, about2, next_button, cross_button2;
Image redframes[4];
Sprite redSprite, verticalSprite;

// for medium levels collision
bool mediumblocksVisible[ROWS][COLLUMS];
bool mediumpigsVisible[ROWS][COLLUMS];
bool mediumrockVisible[ROWS][COLLUMS];
void initMediumLevel();

void loadResources()
{
    iLoadImage(&bg, "assets/images/Angry Bird3.jpg");
    iLoadImage(&menuBg, "assets/images/IconFinal.png");
    iLoadImage(&menubutton, "assets/images/5.png");
    iResizeImage(&menubutton, 130, 120);
    iLoadImage(&levelbutton, "assets/images/5.png");
    iResizeImage(&levelbutton, 140, 130);
    iLoadImage(&gultiback, "assets/images/CatapultBackSprite.png");
    iLoadImage(&gultifront, "assets/images/CatapultFrontSprite.png");
    iLoadImage(&blueImg, "assets/images/Blue_angry_bird5.png");
    iResizeImage(&blueImg, 65, 63);
    iLoadImage(&redImg, "assets/images/redAngryBird.png");
    iResizeImage(&redImg, 65, 63);
    iLoadImage(&yellowImg, "assets/images/yellowAngryBird.png");
    iResizeImage(&yellowImg, 65, 63);
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
    iLoadImage(&mediumCleared, "assets/images/win.png");

    // iInitSprite(&redSprite);
    // iLoadFramesFromFolder(redframes, "assets/images/sprites/red_bird");
    // iChangeSpriteFrames(&redSprite, redframes, 4);
    // iSetSpritePosition(&redSprite,redSpriteX, redSpriteY);

    // iInitSprite(&verticalSprite);
    // iChangeSpriteFrames(&verticalSprite, &woodVertical, 1);
    // iSetSpritePosition(&verticalSprite, 1088, 205);
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
int groundY = 205;
float pillarRotation[3] = {0}, pillarAngVelocity[3] = {0};
bool pillarRotating[3] = {false};
float beamPositionsX[3] = {1088 - 46, 1338 - 46, 1588 - 46}, beamVelocitiesX[3] = {0};

void drawMenu()
{

    iShowLoadedImage(0, 0, &menuBg);

    iShowLoadedImage(110, 237, &menubutton);

    iSetColor(255, 255, 255);
    iText(140, 288, "PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(110, 177, &menubutton);
    iText(140, 228, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(110, 110, &menubutton);
    iText(132, 162, "CREDIT", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawLevelSelect()
{

    iShowLoadedImage(0, 0, &menuBg);

    iShowLoadedImage(85, 210, &levelbutton);
    iSetColor(255, 255, 255);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 155, &levelbutton);
    iText(105, 210, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 95, &levelbutton);
    iText(120, 150, "HARD", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawSettings()
{
    // iPlaySound("assets/sounds/thukraKe.wav", true);
    iShowLoadedImage(0, 0, &menuBg);
    iSetTransparentColor(0, 0, 0, 0.5);
    iFilledRectangle(0, 0, 1920, 1080);
    iShowLoadedImage2(677 - 152, 360 - 73, &credit2, 1050, 700);
    iShowLoadedImage2(56, 56, &previous, 50, 50);
}

int map1[ROWS][COLLUMS] = {
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
    int startX = 1220, startY = 425; // base position
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

void checkMapCollision(int birdX, int birdY)
{
    int blockW = 30, blockH = 30;
    int startX = 1220, startY = 425;

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

                    map1[r][c] = 0;
                    for (int r = ROWS - 2; r >= 0; r--)
                    {
                        for (int c = 0; c < COLLUMS; c++)
                        {
                            if ((map1[r][c] == 1 || map1[r][c] == 2 || map1[r][c] == 3) && map1[r + 1][c] == 0)
                            {
                                if (map1[r][c] == 1)
                                {
                                    map1[r + 1][c] = 1;
                                    map1[r][c] = 0;
                                }
                                if (map1[r][c] == 2)
                                {
                                    map1[r + 1][c] = 2;
                                    map1[r][c] = 0;
                                }
                                if (map1[r][c] == 3)
                                {
                                    map1[r + 1][c] = 3;
                                    map1[r][c] = 0;
                                }
                            }
                            if ((map1[r][c] == 1 || map1[r][c] == 2 || map1[r][c] == 3) && map1[r + 1][c] == 2)
                            {
                                if (map1[r][c] == 1)
                                {
                                    map1[r + 1][c] = 1;
                                    map1[r][c] = 0;
                                }
                                if (map1[r][c] == 2)
                                {
                                    map1[r + 1][c] = 0;
                                    map1[r][c] = 0;
                                }
                                if (map1[r][c] == 3)
                                {
                                    map1[r + 1][c] = 3;
                                    map1[r][c] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int map3[20][20] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void display_map3()
{
    int blockSize = 30;
    int startX = 1000 - 50;
    int startY = 500 + 15;

    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            if (map3[row][col] == 1)
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage(posX, posY, &realwoodSq);
            }
            if (map3[row][col] == 2)
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY + 5, &pigimage, 50, 45);
            }
        }
    }
}

int map2[BUET_ROWS][BUET_COLS] = {

    {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 2, 0, 1, 0, 1, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void display_map2()
{
    int blockSize = 32;
    int startX = 1100;
    int startY = 305 - 115;

    for (int row = 0; row < BUET_ROWS; row++)
    {
        for (int col = 0; col < BUET_COLS; col++)
        {
            if (map2[row][col] == 1)
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage(posX, posY, &realwoodSq);
            }
            if (map2[row][col] == 2)
            {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage2(posX, posY, &pigimage, 50, 45);
            }
        }
    }
}

// void checkMap2Collision(int birdX, int birdY)
// {
//     int blockW = 30, blockH = 30;
//     int startX = 1100;
//     int startY = 305 - 115;

//     for (int r = 0; r < BUET_ROWS; r++)
//     {
//         for (int c = 0; c < BUET_COLS; c++)
//         {
//             int x = startX + c * blockW;
//             int y = startY - r * blockH;

//             if (map2[r][c] == 1 || map2[r][c] == 2)
//             {
//                 if (birdX + birdRadius > x && birdX - birdRadius < x + blockW &&
//                     birdY + birdRadius > y && birdY - birdRadius < y + blockH)
//                 {
//                     map2[r][c] = 0;

//                     for (int rr = BUET_ROWS - 2; rr >= 0; rr--)
//                     {
//                         for (int cc = 0; cc < BUET_COLS; cc++)
//                         {
//                             if ((map2[rr][cc] == 1 || map2[rr][cc] == 2) && map2[rr + 1][cc] == 0)
//                             {
//                                 map2[rr + 1][cc] = map2[rr][cc];
//                                 map2[rr][cc] = 0;
//                             }

//                             if ((map2[rr][cc] == 1 || map2[rr][cc] == 2) && map2[rr + 1][cc] == 2)
//                             {
//                                 if (map2[rr][cc] == 1)
//                                 {
//                                     map2[rr + 1][cc] = 1;
//                                     map2[rr][cc] = 0;
//                                 }
//                                 else if (map2[rr][cc] == 2)
//                                 {
//                                     map2[rr + 1][cc] = 0;
//                                     map2[rr][cc] = 0;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void checkMap2Collision(int birdX, int birdY)
{
    int blockW = 30, blockH = 30;
    int startX = 1100, startY = 315 - 115;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLLUMS; c++)
        {
            int x = startX + c * blockW;
            int y = startY - r * blockH;

            if (map2[r][c] == 1 || map2[r][c] == 2 || map2[r][c] == 3)
            {

                if (birdX + birdRadius > x && birdX - birdRadius < x + blockW &&
                    birdY + birdRadius > y && birdY - birdRadius < y + blockH)
                {

                    map2[r][c] = 0;
                    for (int r = ROWS - 2; r >= 0; r--)
                    {
                        for (int c = 0; c < COLLUMS; c++)
                        {
                            if ((map2[r][c] == 1 || map2[r][c] == 2 || map2[r][c] == 3) && map2[r + 1][c] == 0)
                            {
                                if (map2[r][c] == 1)
                                {
                                    map2[r + 1][c] = 1;
                                    map2[r][c] = 0;
                                }
                                if (map2[r][c] == 2)
                                {
                                    map2[r + 1][c] = 2;
                                    map2[r][c] = 0;
                                }
                                if (map2[r][c] == 3)
                                {
                                    map2[r + 1][c] = 3;
                                    map2[r][c] = 0;
                                }
                            }
                            if ((map2[r][c] == 1 || map2[r][c] == 2 || map2[r][c] == 3) && map2[r + 1][c] == 2)
                            {
                                if (map2[r][c] == 1)
                                {
                                    map2[r + 1][c] = 1;
                                    map2[r][c] = 0;
                                }
                                if (map2[r][c] == 2)
                                {
                                    map2[r + 1][c] = 0;
                                    map2[r][c] = 0;
                                }
                                if (map2[r][c] == 3)
                                {
                                    map2[r + 1][c] = 3;
                                    map2[r][c] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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
    float fx = x;
    float fy = y;
    float dt = 0.8f; // Match with your flying logic's timestep

    for (int i = 0; i < 200; i++)
    {
        fx += vx;
        fy += vy;
        vy += g * dt;

        if (fy < groundY)
            break;

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
            pigVisible[i] = false;
        }

        if (pigX[i] < 0 || pigX[i] > 1920 || pigY[i] > 1080)
        {
            pigVisible[i] = false;
            pigFalling[i] = false;
        }
    }
}

void drawBirds_easy()
{

    if (bluevisible)
        iShowLoadedImage(bluebirdX, bluebirdY, &blueImg);

    if (redvisible)
        iShowLoadedImage(redSpriteX, redSpriteY, &redImg);
    // iShowSprite(&redSprite);

    if (yellowvisible)
        iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void drawBirds_medium()
{

    if (bluevisible)
        iShowLoadedImage2(bluebirdX, bluebirdY, &blueImg, 50, 50);

    if (redvisible)
        iShowLoadedImage2(redSpriteX, redSpriteY, &redImg, 50, 50);

    if (blackvisible)
        iShowLoadedImage2(yellowbirdX, yellowbirdY, &blackImg, 50, 50);
}

void drawBirds_hard()
{

    if (bluevisible)
        iShowLoadedImage2(bluebirdX, bluebirdY - 115, &blueImg, 50, 50);

    if (redvisible)
        iShowLoadedImage2(redSpriteX, redSpriteY - 115, &redImg, 50, 50);

    if (yellowvisible)
        iShowLoadedImage2(yellowbirdX, yellowbirdY - 115, &yellowImg, 50, 50);
}

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 &&
            y1 < y2 + h2 && y1 + h1 > y2);
}

void updatePhysics()
{
    // Update pillar rotations
    for (int i = 0; i < 3; i++)
    {
        if (pillarRotating[i])
        {
            pillarRotation[i] += pillarAngVelocity[i];
            pillarAngVelocity[i] *= angularFriction;

            if (fabs(pillarAngVelocity[i]) < 0.5f)
            {
                pillarRotating[i] = false;
            }
        }
    }

    // Update moving beams
    for (int i = 0; i < 3; i++)
    {
        if (fabs(beamVelocitiesX[i]) > 0.1f)
        {
            beamPositionsX[i] += beamVelocitiesX[i];
            beamVelocitiesX[i] *= friction;
        }
    }
    if (screen == 3)
    {
        checkMapCollision(bluebirdX, bluebirdY);
        checkMapCollision(redSpriteX, redSpriteY);
        checkMapCollision(yellowbirdX, yellowbirdY);
    }
    if (screen == 4)
    {
        checkMap2Collision(bluebirdX, bluebirdY - 115);
        checkMap2Collision(redSpriteX, redSpriteY - 115);
        checkMap2Collision(yellowbirdX, yellowbirdY - 115);
    }
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {

        x += vx - 0.4;
        y += vy;
        vy += g;

        float birdVelocity = sqrt(vx * vx + vy * vy) * 0.5;
        float hittingAngle = atan2(vy, vx);

        // // Ground collision
        // if (y <= groundY) {
        //     y = groundY;
        //     vy = -vy * 0.25;
        //     vx *= 0.6;

        //     if (fabs(vx) < 0.6 && fabs(vy) < 0.6) {
        //         flying = false;
        //         visible = false;
        //     }
        //     iPlaySound("assets/sounds/ball_bounce.wav");
        //   x += vx;
        // y += vy;
        // vy += g;

        // Ground collision
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
        if (x < 0 || x > 1919 || y > 10080)
        {
            flying = false;
            visible = false;
            return;
        }

        if (screen == 1)
        {

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
                    iPlaySound("assets/sounds/wood_damage_a1.wav");
                }

                // horizontal
                if (checkCollision(x, y, birdwidth, birdheight,
                                   beamPositionsX[i], 398, 120, 30))
                {
                    beamVelocitiesX[i] += vx * 0.2f;

                    vy = -vy * restitution;
                    vx *= 0.9f;
                    iPlaySound("assets/sounds/wood_damage_a2.wav");
                }
            }

            // Pig collision
            for (int i = 0; i < pigCount; i++)
            {
                if (pigVisible[i] &&
                    checkCollision(x, y, birdwidth, birdheight,
                                   pigX[i], pigY[i], pigwidth, pigheight))
                {

                    pigVX[i] = cos(hittingAngle) * birdVelocity * 0.3f;
                    pigVY[i] = sin(hittingAngle) * birdVelocity * 0.3f;
                    pigFalling[i] = true;
                    score += 100;
                    iPlaySound("assets/sounds/pig_collision_a6.wav");
                }
                updatePigMotion(i);
            }
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

void draweasy()
{

    iShowLoadedImage2(0, 0, &bg);
    iShowLoadedImage2(208, 177, &gultiback);
    iShowSprite(&redSprite);
    iShowSprite(&verticalSprite);

    if (bluedragging)
        drawRubberLines(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines(redSpriteX, redSpriteY);
    if (yellowdragging)
        drawRubberLines(yellowbirdX, yellowbirdY);

    drawBirds_easy();

    if (bluedragging)
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redSpriteX, redSpriteY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

    iShowLoadedImage2(208, 177, &gultifront);

    // for (int i = 0; i < pillarCount; i++)
    // {
    //     int baseX = 1088 + i * 250;
    //     iShowLoadedImage(baseX, pillarY[i], &woodVertical);
    //     iShowLoadedImage(baseX - 46, 398, &woodHorizontal);
    //     iShowLoadedImage(baseX + 111, 294, &woodHorizontal2);
    //     iShowLoadedImage(baseX + 125, pillarY[i] - 4, &woodVertical2);
    // }

    for (int i = 0; i < pillarCount; i++)
    {
        int baseX = 1088 + i * 250;
        // Main vertical pillar (rotated)
        iRotate(baseX, pillarY[i], 30);
        iShowLoadedImage(baseX, pillarY[i], &woodVertical);
        iUnRotate();

        // Horizontal beams (movable)
        iShowLoadedImage(beamPositionsX[i], 398, &woodHorizontal);
        iShowLoadedImage(baseX + 111, 294, &woodHorizontal2);

        // Small vertical pillar
        iShowLoadedImage(baseX + 125, pillarY[i] - 4, &woodVertical2);
    }

    // Draw pigs
    for (int i = 0; i < pigCount; i++)
    {
        if (pigVisible[i])
            iShowLoadedImage(pigX[i], pigY[i], &pigimage);
    }

    // Draw score panel

    iShowLoadedImage2(25, 888 + 50, &scorebutton, 140, 130);

    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55 - 2, 950 + 50, str);
}

void drawmedium()
{
    iShowLoadedImage2(0, 0, &bg1);
    iShowLoadedImage2(265, 140, &gultiback, 200, 200);
    // iShowSprite(&redSprite);
    if (bluedragging)
        drawRubberLines_medium(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines_medium(redSpriteX, redSpriteY);
    if (yellowdragging)
        drawRubberLines_medium(yellowbirdX, yellowbirdY);
    drawBirds_medium();
    display_map1();
    if (bluedragging)
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redSpriteX, redSpriteY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);
    iShowLoadedImage2(265, 140, &gultifront, 200, 200);
    iShowLoadedImage2(25, 888 + 50, &scorebutton, 140, 130);
    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55 - 2, 950 + 50, str);
}

void drawhard()
{
    iShowLoadedImage2(0, 0, &bgHard, 1920, 1080);
    iShowLoadedImage2(208, 177 - 115, &gultiback, 200, 200);

    if (bluedragging)
        drawRubberLines_hard(bluebirdX, bluebirdY - 115);
    if (reddragging)
        drawRubberLines_hard(redSpriteX, redSpriteY - 115);
    if (yellowdragging)
        drawRubberLines_hard(yellowbirdX, yellowbirdY - 115);

    drawBirds_hard();
    display_map2();

    if (bluedragging)
        drawPathway_hard(bluebirdX, bluebirdY - 115, blue_vx, blue_vy);
    if (reddragging)
        drawPathway_hard(redSpriteX, redSpriteY - 115, red_vx, red_vy);
    if (yellowdragging)
        drawPathway_hard(yellowbirdX, yellowbirdY - 115, yellow_vx, yellow_vy);

    iShowLoadedImage2(208, 177 - 115, &gultifront, 200, 200);

    iShowLoadedImage2(25, 888 + 50, &scorebutton, 140, 130);
    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55 - 2, 950 + 50, str);
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

        // iPlaySound("assets/sounds/thukraKe.wav", true, 20);
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
        iSetColor(0, 0, 0);
        iText(755, 730, "(i)SOUND", GLUT_BITMAP_HELVETICA_18);
        if (soundOn)
            iShowLoadedImage2(1115, 715, &sound1, 120, 55);
        else
            iShowLoadedImage2(1115, 715, &sound2, 120, 55);

        iText(755, 600, "(ii)INSTRUCTION", GLUT_BITMAP_HELVETICA_18);
        iShowLoadedImage2(1115, 585, &instruction, 50, 50);

        iText(755, 470, "(iii)ABOUT:", GLUT_BITMAP_HELVETICA_18);
        iShowLoadedImage2(1115, 460, &about, 50, 50);
    }
    else if (screen == 7)
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(56, 56, &previous, 50, 50);
        iShowLoadedImage2(677 - 152, 360 - 73, &whiteCanvas);
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
        iSetTransparentColor(0, 0, 0, 0.6);
        iFilledRectangle(0, 0, 1920, 1080);
        iText(700, 500, "Enter your name:", GLUT_BITMAP_HELVETICA_18);
        iRectangle(700, 460, 300, 30);
        iText(710, 465, userName, GLUT_BITMAP_HELVETICA_18);
        iText(700, 400, "Press ENTER to continue", GLUT_BITMAP_HELVETICA_12);
        if (nameWarning)
        {
            iSetColor(255, 0, 0);
            iText(700, 465, "Please, Enter your name", GLUT_BITMAP_HELVETICA_18);
        }
    }
    else if (screen == 11)
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(100, 100, &mediumCleared);
    }

    else if (screen == 12)
    {
        iShowLoadedImage2(0, 0, &menuBg);
        iSetTransparentColor(0, 0, 0, 0.5);
        iFilledRectangle(0, 0, 1920, 1080);
        iShowLoadedImage2(100, 100, &mediumCleared);
    }

    iSetColor(0, 0, 0);
    iText(1700, 1035, cursorStr, GLUT_BITMAP_HELVETICA_18);
}

void updateBird()
{
    updatePhysics();
    updateSingleBird(bluebirdX, bluebirdY, blue_vx, blue_vy, blueflying, bluevisible);

    updateSingleBird(redSpriteX, redSpriteY, red_vx, red_vy, redflying, redvisible);

    updateSingleBird(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowflying, yellowvisible);

    for (int i = 0; i < pigCount; i++)
    {
        updatePigMotion(i);
    }
}

void iMouseMove(int mx, int my)
{
    cursorX = mx;
    cursorY = my;
    sprintf(cursorStr, "Cursor: (%d, %d)", cursorX, cursorY);
    if (screen == 1 || screen == 3 || screen == 4)
    {
        if (bluedragging)
        {
            iPlaySound("assets/sounds/slingshot.wav", false, 35);
            bluebirdX = mx;
            bluebirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            blue_vx = v * cos(angle) * 0.5;
            blue_vy = v * sin(angle) * 0.5;
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
}

void iMouse(int button, int state, int mx, int my)
{
    if (screen == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 123 && mx <= 224 && my >= 282 && my <= 318) // play button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 10;
            userName[0] = '\0';
            userNameLen = 0;
        }

        else if (mx >= 123 && mx <= 224 && my >= 219 && my <= 258) // exit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            exit(0);
        }

        else if (mx >= 123 && mx <= 224 && my >= 156 && my <= 191) // credit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 5;
        }
        else if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) //  settings button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
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

            difficultylevel = 1;
            screen = 1;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 97 && mx <= 211 && my >= 206 && my <= 241) // medium level
        {

            difficultylevel = 2;
            screen = 3;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 97 && mx <= 211 && my >= 144 && my <= 178) // hard button button
        {
            difficultylevel = 3;
            screen = 4;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
    }

    else if ((screen == 1 || screen == 3) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (selectedBird == -1 && mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70)
        {
            bluedragging = true;
            selectedBird = 0; // blue bird
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
    }
    else if ((screen == 4) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (selectedBird == -1 && mx >= bluebirdX && mx <= bluebirdX + 50 && my >= bluebirdY - 115 && my <= bluebirdY - 115 + 50)
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
            yellowdragging = true;
            selectedBird = 2;
            iPlaySound("assets/sounds/bird_03_select.wav", false);
        }
    }

    else if ((screen == 1 || screen == 3 || screen == 4) && button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        if (bluedragging)
        {
            bluedragging = false;
            blueflying = true;
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
    }
    else if ((screen == 5) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // credit- previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            // iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
    }
    else if (screen == 6 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // settings- previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            // iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
        }
        else if (mx >= 1111 && mx <= 1283 && my >= 718 && my <= 767) // sound button
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            soundOn = !soundOn;
            if (soundOn)
                iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
            else
                iPlaySound(0, 0, 0);
        }
        else if (mx >= 1118 && mx <= 1160 && my >= 593 && my <= 635) // instruction button
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 7;
        }
        else if (mx >= 1118 && mx <= 1160 && my >= 465 && my <= 500) // instruction button
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 8;
        }
    }
    else if ((screen == 7 || screen == 8) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (mx >= 58 && mx <= 101 && my >= 65 && my <= 103) // about & instruction- previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
        }
        else if (mx >= 1264 && mx <= 1307 && my >= 367 && my <= 408) // for going about_2
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 9;
        }
    }
    else if ((screen == 9) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (mx >= 585 && mx <= 626 && my >= 374 && my <= 413) // about_2 previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 8;
        }
        else if (mx >= 1353 && mx <= 1387 && my >= 790 && my <= 830) // about_2 previous
        {
            iStopAllSounds();
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 6;
        }
    }
}

void iKeyboard(unsigned char key, int state)
{
    if (state != 0)
        return;

    if (key == 'q')
        iCloseWindow();

    if (screen == 10)
    {
        if (key == '\r')
        {
            if (Correct_username())

            {
                screen = 2;
            }
            else
                nameWarning = true;
        }
        if (key == '\b')
        {
            if (userNameLen > 0)
            {
                userNameLen--;
                userName[userNameLen] = '\0';
            }
        }
        if ((key >= 32) && (key <= 126))
        {
            if (userNameLen < 99)
            {
                userName[userNameLen++] = key;
                userName[userNameLen] = '\0';
                if (Correct_username())
                {
                    nameWarning = false;
                }
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
// void animate() {
//     iAnimateSprite(&redSprite);
//     iAnimateSprite(&verticalSprite);
// }

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    loadResources();
    iInitializeSound();
    iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
    iSetTimer(100, updateBird);
    // iSetTimer(200, animate);
    iOpenWindow(1920, 1080, "Angry Birds - BUET PROJECT");
    return 0;
}
