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
bool bluevisible = true, redvisible = true, yellowvisible = true;
int bluebirdX = 20, bluebirdY = 194;
int redbirdX = 100, redbirdY = 194;
int yellowbirdX = 180, yellowbirdY = 200;
float blue_vx = 0, blue_vy = 0;
float red_vx = 0, red_vy = 0;
float yellow_vx = 0, yellow_vy = 0;
bool blueflying = false, redflying = false, yellowflying = false;
bool bluedragging = false, reddragging = false, yellowdragging = false;

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
const char *bg = "assets/images/Angry Bird3.jpg";
const char *catapultBack = "assets/images/CatapultBackSprite.png";
const char *catapultFront = "assets/images/CatapultFrontSprite.png";
Image menuBg, blueImg, redImg, yellowImg, bg1, woodHorizontal, woodVertical,
    woodHorizontal2, woodVertical2;
Image menubutton, levelbutton, scorebutton, pigimage, credit;

void loadResources()
{
    iLoadImage(&menuBg, "assets/images/IconFinal.png");
    iLoadImage(&menubutton, "assets/images/1.png");
    iResizeImage(&menubutton, 130, 120);
    iLoadImage(&levelbutton, "assets/images/10.png");
    iResizeImage(&levelbutton, 140, 130);
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
    iLoadImage(&credit, "assets/images/terrace.png" );
    iResizeImage(&credit, 1920, 1080);
    iLoadImage(&scorebutton, "assets/images/4.png");
    iResizeImage(&scorebutton, 140, 130);
    iLoadImage(&bg1, "assets/images/BG_03.png");
    iResizeImage(&bg1, 1920, 1080);
    
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
        iPoint(fx, fy, 1);
    }
}

void updatePigMotion(int i) {
    if (pigFalling[i]) {
        pigVY[i] += g * 0.3f; 
        pigX[i] += pigVX[i];
        pigY[i] += pigVY[i];

        
        for (int j = 0; j < pillarCount; j++) {
            bool collX = pigX[i] + pigwidth >= pillarX[j] && pigX[i] <= pillarX[j] + pillarwidth;
            bool collY = pigY[i] + pigheight >= pillarY[j] && pigY[i] <= pillarY[j] + pillarheight;
            if (collX && collY) {
                pigVX[i] = -pigVX[i] * 0.4f;
                pigVY[i] = -pigVY[i] * 0.4f;
            }
        }

      
        if (pigY[i] <= groundY) {
            pigY[i] = groundY;
            pigVX[i] = pigVY[i] = 0;
            pigFalling[i] = false;
            pigVisible[i] = false; 
        }

        
        if (pigX[i] < 0 || pigX[i] > 1920 || pigY[i] > 1080) {
            pigVisible[i] = false;
            pigFalling[i] = false;
        }
    }
}

void drawBirds()
{

    if (bluevisible)
        iShowLoadedImage(bluebirdX, bluebirdY, &blueImg);

    if (redvisible)
        iShowLoadedImage(redbirdX, redbirdY, &redImg);

    if (yellowvisible)
        iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible)
{
    if (flying)
    {
        x += vx;
        y += vy;
        vy += g;

        // Bounce on ground
        if (y <= groundY)
        {
            y = groundY;
            vy = -vy * 0.25;
            vx *= 0.5;

            if (x < 0 || x > 1890 || y < 205 || y > 1000 || fabs(vx) < 0.05)
            {
                flying = false;
                visible = false;
                return;
            }

            iPlaySound("assets/sounds/ball_bounce.wav");
        }

        // Collision with any pillar
        for (int i = 0; i < pillarCount; i++)
        {
            int baseX = 1088 + i * 250;
            if (y + birdheight >= pillarY[i] && y <= pillarY[i] + pillarheight &&
                x + birdwidth >= baseX && x <= baseX + pillarwidth) // with vertical1
            {
                vx = -vx * 0.5;
                pillarHit[i] = true;
            }
            if (y + birdheight >= 398 && y <= 428 &&
                x + birdwidth >= baseX - 46 && x <= baseX - 46 + 120) // with horizontal1
            {
                vy = -vy * 0.5;
                vx *= 1;
            }
            if (y + birdheight >= 294 && y <= 324 &&
                x + birdwidth >= baseX + 111 && x <= baseX + 111 + 60) // with horizontal2
            {
                vy = -vy * 0.5;
                vx *= 0.5;
                if (vx < 0.05)
                {
                    visible = false;
                }
            }
            if (y + birdheight >= pillarY[i] && y <= pillarY[i] + 100 &&
                x + birdwidth >= baseX + 125 && x <= baseX + 125 + 30) // with vertical2
            {
                vx = -vx * 0.5;
            }
        }

        // Collision with any pig
        for (int i = 0; i < pigCount; i++)
        {
            updatePigMotion(i);
           
        }
    }
}



void draweasy()
{
    // Background and slingshot
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    // Draw rubber lines if dragging
    if (bluedragging)
        drawRubberLines(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines(redbirdX, redbirdY);
    if (yellowdragging)
        drawRubberLines(yellowbirdX, yellowbirdY);

    // Draw birds
    drawBirds();

    // Draw projectile pathway while dragging
    if (bluedragging)
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redbirdX, redbirdY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

    // Draw front part of slingshot
    iShowImage(208, 177, catapultFront);

    // Draw 3 pillar systems (each has: vertical long, horizontal long, horizontal short, vertical short)
    for (int i = 0; i < pillarCount; i++)
    {
        int baseX = 1088 + i * 250;
        iShowLoadedImage(baseX, pillarY[i], &woodVertical);            // vertical pillar
        iShowLoadedImage(baseX - 46, 398, &woodHorizontal);            // horizontal top (long)
        iShowLoadedImage(baseX + 111, 294, &woodHorizontal2);          // horizontal mid (short)
        iShowLoadedImage(baseX + 125, pillarY[i] - 4, &woodVertical2); // right short pillar
    }

    // Draw pigs
    for (int i = 0; i < pigCount; i++)
    {
        if (pigVisible[i])
            iShowLoadedImage(pigX[i], pigY[i], &pigimage);
    }

    // Draw score panel
   
    iShowLoadedImage(25, 688, &scorebutton);

    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55, 750, str);
}

void drawmedium()
{
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
    iSetColor(0, 0, 0);
    iText(1700, 1035, cursorStr, GLUT_BITMAP_HELVETICA_18);
}

void updateBird()
{
    updateSingleBird(bluebirdX, bluebirdY, blue_vx, blue_vy, blueflying, bluevisible);

    updateSingleBird(redbirdX, redbirdY, red_vx, red_vy, redflying, redvisible);

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
    cursorX = mx;
    cursorY = my;
    if (screen == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        if (mx >= 123 && mx <= 224 && my >= 282 && my <= 318) // play button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            screen = 2;
        }

        else if (mx >= 123 && mx <= 224 && my >= 219 && my <= 258) // exit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            exit(0);
        }

        else if (mx >= 123 && mx <= 224 && my >= 156 && my <= 191) // credit button
        {
            iPlaySound("assets/sounds/menu_sound.wav", false);
            // iShowLoadedImage2(0, 0, &credit, 1920, 1080);
            iShowLoadedImage(0,0, &credit);
        }
    }

    else if (screen == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
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
        if (selectedBird = -1 && mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70)
        {
            bluedragging = true;
            selectedBird = 0;
            iPlaySound("assets/sounds/bird_01_select.wav", false);
        }
        else if (selectedBird = -1 && mx >= redbirdX && mx <= redbirdX + 70 && my >= redbirdY && my <= redbirdY + 70)
        {
            reddragging = true;
            selectedBird = 1;
            iPlaySound("assets/sounds/bird_02_select.wav", false);
        }
        else if (selectedBird = -1 && mx >= yellowbirdX && mx <= yellowbirdX + 70 && my >= yellowbirdY && my <= yellowbirdY + 70)

        {
            yellowdragging = true;
            selectedBird = 2;
            iPlaySound("assets/sounds/bird_03_select.wav", false);
        }
    }

    else if (screen == 1 && button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
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
}

void iKeyboard(unsigned char key, int state)
{
    if (key == 'r')
    {
        for (int i = 0; i < pigCount; i++)
        {
            pigVisible[i] = true;
            pigFalling[i] = false;
            pigX[i] = 1070 + (i / 2) * 250; // adjust position if needed
            pigY[i] = (i % 2 == 0) ? 420 : 316;
            pigVX[i] = pigVY[i] = 0;
        }
    }
    if (key == 'q')
        iCloseWindow();
    if (key == 'm')
    {
        screen = 0;
        iStopAllSounds();
        iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
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
    iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
    iSetTimer(20, updateBird);
    iOpenWindow(1920, 1080, "Angry Birds - BUET PROJECT");
    return 0;
}
