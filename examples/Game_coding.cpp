#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include "iSound.h"
/*
screen =0 -> very first screen (menu screen)
screen =1 -> third screen (main game screen) 
screen =2 -> second screen (level select screen) 



*/

// Game State
int screen = 0, currentScreen = -1, score = 0;
int difficultylevel = 0;

// Bird Physics and States
bool bluevisible= true, redvisible = true, yellowvisible = true;
int bluebirdX = 20, bluebirdY = 194;
int redbirdX = 100, redbirdY = 194;
int yellowbirdX = 180, yellowbirdY = 200;
float blue_vx = 0, blue_vy = 0;
float red_vx = 0, red_vy = 0;
float yellow_vx = 0, yellow_vy = 0;
bool blueflying = false, redflying = false, yellowflying = false;
bool bluedragging = false, reddragging = false, yellowdragging = false;

int currentBird = 0; // 0-> blue, 1-> red, 2-> yellow
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;
int birdheight = 63, birdwidth = 65;

// Pig Position
int pig1X = 1070, pig1Y = 420;
int pigheight = 63, pigwidth = 65;
float pig_vx = 0, pig_vy = 0;
bool pigfalling = false;
bool pigvisible = true;

// Cursor
int cursorX = -1, cursorY = -1;

// Assets
const char *bg = "assets/images/Angry Bird3.jpg";
const char *catapultBack = "assets/images/CatapultBackSprite.png";
const char *catapultFront = "assets/images/CatapultFrontSprite.png";
Image  menuBg, blueImg, redImg, yellowImg, bg1, woodHorizontal, woodVertical;
Image menubutton, levelbutton, scorebutton, pigimage;

void loadResources()
{
iLoadImage(&woodHorizontal,"assets/images/Pillar_01.png");
iResizeImage(&woodHorizontal,120, 30);
iLoadImage(&woodVertical,"assets/images/Pillar_02.png");
iResizeImage(&woodVertical,30, 200);

}
// Rubber Position
int leftArmX = 298, leftArmY = 374;
int rightArmX = 336, rightArmY = 380;

// for pillar & ground
int pillarX = 1088, pillarY = 205;
int pillarwidth = 30, pillarheight = 200;
int groundY = 205;

// void loadResources()
// {
// 	iInitSprite(&redbirdspriteS);
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
    // iShowImage(-480, -234, menuBg);
    iLoadImage(&menuBg,"assets/images/IconFinal.png" );
    // iResizeImage(&menuBg, 1920, 1440);
    iShowLoadedImage(0,0,&menuBg);
    iLoadImage(&menubutton, "assets/images/1.png");
    iResizeImage(&menubutton, 130, 120);
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
    iLoadImage(&levelbutton, "assets/images/10.png");
    iResizeImage(&levelbutton, 140, 130);
    iShowLoadedImage(0,0,&menuBg);

    iShowLoadedImage(85, 210, &levelbutton);
    iSetColor(255, 255, 255);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 155, &levelbutton);
    iText(105, 210, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 95, &levelbutton);
    iText(120, 150, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawRubberLines(int x, int y) 
{
    iSetColor(0, 0, 0);
    iLine(leftArmX, leftArmY, x + birdRadius / 2, y + birdRadius / 2);
    iLine(rightArmX, rightArmY, x + birdRadius / 2, y + birdRadius / 2);
    
}

void drawPathway(int x, int y, float vx, float vy)
{
    float fx = x + birdRadius / 2;
    float fy = y + birdRadius / 2;
    float dt = 0.1f;
    for (int i = 0; i < 200; i++)
    {
        fx += vx * dt;
        fy += vy * dt;
        vy += g * dt;
        if (fy < groundY)
            break;
        iSetColor(0, 0, 0);
        iFilledCircle(fx, fy, 2);
    }
}

void drawBirds()
{
    iLoadImage(&blueImg, "assets/images/Blue_angry_bird5.png");

    iResizeImage(&blueImg, 65, 63);
    if(bluevisible)
    iShowLoadedImage(bluebirdX, bluebirdY, &blueImg);

    iLoadImage(&redImg, "assets/images/redAngryBird.png");
    iResizeImage(&redImg, 65, 63);
    if(redvisible)
    iShowLoadedImage(redbirdX, redbirdY, &redImg);

    iLoadImage(&yellowImg, "assets/images/yellowAngryBird.png");
    iResizeImage(&yellowImg, 65, 63);
    if(yellowvisible)
    iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {

        x += vx;
        y += vy;
        vy += g;
        
        if (y <= groundY)
        {
            y = groundY;
            vy = -vy * 0.25;
            vx = vx * 0.5;
             if(x<0 || x> 2000 || y< 205 || y>1200 || vx<0.05)
        {
            flying= false;
            visible= false;
            return;
        }
            
            iPlaySound("assets/sounds/ball_bounce.wav");
           
        }
        if (y >= pillarY && y <= pillarY + pillarheight &&
            x + birdwidth >= pillarX && x <= pillarX + pillarwidth)
        {
            vx = -vx * 0.5;
            
        }
    }
   
    if (pigvisible)
    {

        bool collisionX = x + birdwidth >= pig1X && x <= pig1X + pigwidth;
        bool collisionY = y + birdheight >= pig1Y && y <= pig1Y + pigheight;
        if (collisionX && collisionY)
        {
            pigfalling = true;

            iPlaySound("assets/sounds/bird_01_collision_a1.wav", NULL, SND_FILENAME | SND_ASYNC);
        }
        if (pigfalling)
        {
            pig_vy += -1;
            pig1Y += pig_vy;
            if (pig1Y <= groundY)
            {
                pig1Y = groundY;
                pigfalling = false;
                pigvisible = false;
            }
        }
    }
}

void draweasy()
{
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    if (bluedragging)

        drawRubberLines(bluebirdX, bluebirdY);

    if (reddragging)

        drawRubberLines(redbirdX, redbirdY);

    if (yellowdragging)

        drawRubberLines(yellowbirdX, yellowbirdY);

    drawBirds();

    if (bluedragging)
    {
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
        
    }

    if (reddragging)
    {
        drawPathway(redbirdX, redbirdY, red_vx, red_vy);
        
    }
    if (yellowdragging)
    {
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);
        
    }

    iShowImage(208, 177, catapultFront);
    iSetColor(192, 192, 192);
    // iFilledRectangle(pillarX, pillarY, pillarwidth, pillarheight); // vertical
    iShowLoadedImage(pillarX, pillarY, &woodVertical);
    iSetColor(192, 192, 192);
    // iFilledRectangle(1042, 402, 120, 30); // horizontal
    iShowLoadedImage(1042,398, &woodHorizontal );
    if (pigvisible)
    {
        iLoadImage(&pigimage, "assets/images/Pig_front.png");
        iResizeImage(&pigimage, 65, 63);
        iShowLoadedImage(pig1X, pig1Y, &pigimage);
    }

    iLoadImage(&scorebutton, "assets/images/4.png");
    iResizeImage(&scorebutton, 140, 130);
    iShowLoadedImage(25, 688, &scorebutton);

    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55, 750, str);
}

void drawmedium()
{
    iLoadImage(&bg1, "assets/images/bg_1.png");
    iResizeImage(&bg1, 2560, 1440);
    iShowLoadedImage(0, 0, &bg1);
}

void iDraw()
{
    iClear();
   
    if (screen == 0)
        drawMenu();
    else if (screen == 1)
        draweasy();
    else if (screen == 2)
        drawLevelSelect();
    else if (screen == 3)
        drawmedium();
}

void updateBird()
{
    updateSingleBird(bluebirdX, bluebirdY, blue_vx, blue_vy, blueflying, bluevisible);

    updateSingleBird(redbirdX, redbirdY, red_vx, red_vy, redflying, redvisible);

    updateSingleBird(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowflying, yellowvisible);
}

void iMouseMove(int mx, int my)
{
    cursorX = mx;
    cursorY = my;
    if (screen == 1)
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
            redbirdX = mx;
            redbirdY = my;
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

    if (screen == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        if (mx >= 110 && mx <= 240 && my >= 237 && my <= 357) // play button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 2;
            // iStopAllSounds();
            // iPlaySound("assets/sounds/angry_birds_2.wav", true);
        }

        else if (mx >= 110 && mx <= 240 && my >= 177 && my <= 297) // exit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            exit(0);
        }

        else if (mx >= 110 && mx <= 240 && my >= 110 && my <= 230) // credit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
        }
    }

    else if (screen == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        if (mx >= 85 && mx <= 225 && my >= 210 && my <= 340) // easy level
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            difficultylevel = 1;
            screen = 1;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 85 && mx <= 225 && my >= 155 && my <= 285) // medium level
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            difficultylevel = 2;
            screen = 3;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }

        else if (mx >= 85 && mx <= 225 && my >= 95 && my <= 225) // back button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 0;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_2.wav", true);
        }
    }

    else if (screen == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70)
            bluedragging = true;
            iPlaySound("assets/sounds/bird_01_select.wav", false);
        if (mx >= redbirdX && mx <= redbirdX + 70 && my >= redbirdY && my <= redbirdY + 70)
            reddragging = true;
            iPlaySound("assets/sounds/bird_02_select.wav", false);
        if (mx >= yellowbirdX && mx <= yellowbirdX + 70 && my >= yellowbirdY && my <= yellowbirdY + 70)
            yellowdragging = true;
            iPlaySound("assets/sounds/bird_03_select.wav", false);
    }

    else if (screen == 1 && button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        if (bluedragging)
        {
            bluedragging = false;
            blueflying = true;
            iPlaySound("assets/sounds/bird_01_flying.wav", false);
        }
        if (reddragging)
        {
            reddragging = false;
            redflying = true;
            iPlaySound("assets/sounds/bird_02_flying.wav", false);
        }
        if (yellowdragging)
        {
            yellowdragging = false;
            yellowflying = true;
            iPlaySound("assets/sounds/bird_03_flying.wav", false);
        }
    }
}

void iKeyboard(unsigned char key, int state)
{
    if (key == 'r')
    {
        bluebirdX = 20;
        bluebirdY = 194;
        blue_vx = blue_vy = 0;
        blueflying = bluedragging = false;
        redbirdX = 100;
        redbirdY = 194;
        red_vx = red_vy = 0;
        redflying = reddragging = false;
        yellowbirdX = 180;
        yellowbirdY = 200;
        yellow_vx = yellow_vy = 0;
        yellowflying = yellowdragging = false;
        pigvisible = true;
        score = 0;
    }
    if (key == 'q')
        exit(0);
    if (key == 'm')
    {
        screen = 0;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true,20);
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
    iInitializeSound();
    iPlaySound("assets/sounds/angry_birds_2.wav", true,20);
    // loadResources();
    // iSetTimer(100, iAnim);

    iSetTimer(20, updateBird);
    iOpenWindow(1920, 1080, "Angry Birds - BUET PROJECT");
    return 0;
}
