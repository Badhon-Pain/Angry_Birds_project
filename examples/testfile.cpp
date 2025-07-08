
#include "iGraphics.h"
#include <math.h>
#include<string.h>

// for game State
int screen = 0, currentScreen = -1, score = 0;
int difficultyLevel = 0;


// for bird & Physics
int bluebirdX = 20, bluebirdY = 194;
int redbirdX = 100, redbirdY = 194;
int yellowbirdX= 180, yellowbirdY = 200;
bool isDragging = false, isFlying = false;
float vx = 0, vy = 0;
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;

// for pig position
int pig1X=1070,  pig1Y= 431;
int pigheight= 75, pigwidth= 75;
bool pigvisible= true;

// for Cursor
int cursorX = -1, cursorY = -1;

// for assets
const char *bg = "assets/images/Angry Bird3.jpg";
const char *catapultBack = "assets/images/CatapultBackSprite.png";
const char *catapultFront = "assets/images/CatapultFrontSprite.png";
Image bluebird, redbird, yellowbird;
// Sprite redbirdsprite;
const char *menuBg = "assets/images/Icon.jpg";
Image menubutton, levelbutton, scorebutton,pigimage;
// iLoadImage(&pigimage, "assets/images/Pig_front.png");

// for rubber position
int leftArmX = 298, leftArmY = 374;
int rightArmX = 336, rightArmY = 380;

// for brick data
// #define MAX_BRICKS 5
// int brickX[MAX_BRICKS] = {500, 560, 720, 780, 840};
// int brickY[MAX_BRICKS] = {300, 300, 300, 300, 300};
// int brickW = 60, brickH = 30;
// bool brickVisible[MAX_BRICKS] = {true, true, true, true, true};
// void loadResources()
// {
// 	iInitSprite(&redbirdsprite, -1);
// 	iLoadFramesFromFolder(redbird, "assets/images/sprites/red_bird");
// 	iChangeSpriteFrames(&redbirdsprite, redbird, 24);
// 	iSetSpritePosition(&redbirdsprite, redbirdX, redbirdY);
// }

// void iAnim()
// {
// 	iAnimateSprite(&redbirdsprite);
// }

void drawMenu()
{
    iShowImage(-480, -234, menuBg);
   

    iSetColor(255, 215, 0);
    iLoadImage(&menubutton, "assets/images/1.png");

    iResizeImage(&menubutton, 130, 120);

    iShowLoadedImage(110, 237, &menubutton);

    iSetColor(0, 0, 0);
    iText(140, 288, "PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(220, 20, 60);

    iShowLoadedImage(110, 177, &menubutton);

    iSetColor(0, 0, 0);
    iText(140, 228, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(30, 144, 255);

    iShowLoadedImage(110, 110, &menubutton);

    iSetColor(0, 0, 0);
    iText(132, 162, "CREDIT", GLUT_BITMAP_TIMES_ROMAN_24);
}
void drawLevelSelect()
{
    iLoadImage(&levelbutton, "assets/images/10.png");
    iResizeImage(&levelbutton, 140, 130);
    iShowImage(-480, -234, menuBg);
    

    iShowLoadedImage(85, 210, &levelbutton);
    iSetColor(0, 0, 0);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 155, &levelbutton);
    iSetColor(0, 0, 0);
    iText(105, 210, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 95, &levelbutton);
    iSetColor(0, 0, 0);
    iText(120, 150, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawRubberLines()
{
    iSetColor(0, 0, 0);
    iLine(leftArmX, leftArmY, bluebirdX + birdRadius / 2, bluebirdY + birdRadius / 2);
    iLine(rightArmX, rightArmY, bluebirdX + birdRadius / 2, bluebirdY + birdRadius / 2);
}

void drawpathway()
{
    float fx = bluebirdX + birdRadius / 2;
    float fy = bluebirdY + birdRadius / 2;
    float tempVx = vx, tempVy = vy;
    float dt = 0.1f;
    for (int i = 0; i < 200; i++)
    {
        fx += tempVx * dt;
        fy += tempVy * dt;
        tempVy += g * dt;
        if (fy < 0)
            break;
        iSetColor(0, 0, 0);
        iFilledCircle(fx, fy, 2);
    }
}

// void drawBricks()
// {
//     for (int i = 0; i < MAX_BRICKS; i++)
//     {
//         if (brickVisible[i])
//         {

//             iSetColor(192, 192, 192);
//             iFilledRectangle(brickX[i], brickY[i], brickW, brickH + 20);

//             iSetColor(105, 105, 105);
//             iRectangle(brickX[i], brickY[i], brickW, brickH + 20);
//         }
//     }
// }

void drawGame()
{
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    if (isDragging)
        drawRubberLines();

    iLoadImage(&bluebird, "assets/images/Blue_angry_bird5.png");
    iResizeImage(&bluebird, 65, 63);
    iShowLoadedImage(bluebirdX, bluebirdY, &bluebird);
    // iShowSprite(&redbirdsprite);
     iLoadImage(&redbird, "assets/images/redAngryBird.png");
     iResizeImage(&redbird, 65, 63);
    iShowLoadedImage(redbirdX, redbirdY, &redbird);
     iLoadImage(&yellowbird, "assets/images/yellowAngryBird.png");
     iResizeImage(&yellowbird, 65, 63);
    iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowbird);

    iSetColor(192,192,192);
    iFilledRectangle(1088, 209, 30, 200); //vertical
    if(pigvisible)
    {
        iLoadImage(&pigimage,"assets/images/Pig_front.png");
        iResizeImage(&pigimage, 65, 63);
        iShowLoadedImage(pig1X, pig1Y, &pigimage);
       
    }
    iSetColor(192,192,192);
    iFilledRectangle(1042, 402, 120, 30); // horizontal


    iShowImage(208, 177, catapultFront);

    if (isDragging || isFlying)
        drawpathway();
    
    // drawBricks();

    iLoadImage(&scorebutton, "assets/images/4.png");
    iResizeImage(&scorebutton, 140, 130);
    iShowLoadedImage(25, 688, &scorebutton);
    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55, 750, str);

    if (cursorX != -1 && cursorY != -1)
    {
        char str[50];
        sprintf(str, "X: %d, Y: %d", cursorX, cursorY);
        iSetColor(0, 0, 0);
        iText(cursorX, cursorY, str, GLUT_BITMAP_8_BY_13);
    }
}

void updateBird()
{
    if (isFlying)
    {
        bluebirdX += vx;
        bluebirdY += vy;
        vy += g;

        //    brick collision
        // for (int i = 0; i < MAX_BRICKS; i++)
        // {
        //     if (brickVisible[i])
        //     {
        //         if (birdX + birdRadius > brickX[i] && birdX < brickX[i] + brickW &&
        //             birdY + birdRadius > brickY[i] && birdY < brickY[i] + brickH + 20)
        //         {
        //             brickVisible[i] = false;
                    
        //         }
        //     }
        // }

        if (bluebirdY < 0)
        {
            isFlying = false;
            vx = vy = 0;
        }
    }
    if(pigvisible)
    {
        int birdheight= 70, birdwidth= 70;
         bool collisionX = bluebirdX + birdwidth >= pig1X && bluebirdX <= pig1X + pigwidth;
    bool collisionY = bluebirdY + birdheight >= pig1Y && bluebirdY <= pig1Y + pigheight;
      if (collisionX && collisionY) {
        pigvisible = false;
        score += 100;
        PlaySound("assets/sounds/bird_01_collision_a1.wav", NULL, SND_FILENAME | SND_ASYNC); 
      }

    }
}

void iDraw()
{
    iClear();
    if (screen != currentScreen)
    {
        currentScreen = screen;
        if (screen == 0)
            PlaySound("assets/sounds/angry_birds_2.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else if (screen == 1)
            PlaySound("assets/sounds/angry_birds_intro_music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    if (screen == 0)
        drawMenu();
    else if (screen == 1)
        drawGame();
    else if (screen == 2)
        drawLevelSelect();
}

void iMouseMove(int mx, int my)
{
    cursorX = mx;
    cursorY = my;
    if (isDragging && screen == 1)
    {
        bluebirdX = mx;
        bluebirdY = my;

        float dx = slingX - bluebirdX;
        float dy = slingY - bluebirdY;
        float v = sqrt(dx * dx + dy * dy);
        float angle = atan2(dy, dx);
        vx = v * cos(angle) * 0.5;
        vy = v * sin(angle) * 0.5;
    }
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (screen == 0)
        {
            // Main menu
            if (mx >= 140 && mx <= 260 && my >= 270 && my <= 310)
                screen = 2;
            else if (mx >= 140 && mx <= 260 && my >= 210 && my <= 250)
                exit(0);
            else if (mx >= 140 && mx <= 260 && my >= 150 && my <= 190)
                score = 999;
        }
        else if (screen == 2)
        {
            // Level selection
            if (mx >= 110 && mx <= 230 && my >= 250 && my <= 290)
            {
                difficultyLevel = 1;
                screen = 1;
            }
            else if (mx >= 110 && mx <= 250 && my >= 190 && my <= 230)
            {
                difficultyLevel = 2;
                screen = 1;
            }
            else if (mx >= 110 && mx <= 230 && my >= 130 && my <= 170)
            {
                screen = 0;
            }
        }

        else if (screen == 1)
        {
            if (mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70)
                isDragging = true;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && screen == 1)
    {
        isDragging = false;
        isFlying = true;
    }
}

void iKeyboard(unsigned char key)
{
    if (key == 'm')
        screen = 0;
    else if (key == 'r')
    {
        bluebirdX = 40;
        bluebirdY = 194;
        vx = vy = 0;
        isFlying = false;
    }

    else if (key == 'q')
        exit(0);
}

void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_END)
        exit(0);
}

void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // loadResources();
	// iSetTimer(100, iAnim);
    iSetTimer(20, updateBird);
    iInitialize(800, 1000, "Angry Birds - BUET Project");
    return 0;
}
