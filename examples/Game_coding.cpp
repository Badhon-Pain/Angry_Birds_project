#include "iGraphics.h"
#include <math.h>

//for game State
int screen = 0, currentScreen = -1, score = 0;
bool isDragging = false, isFlying = false;
int difficultyLevel = 0; 


//for bird & Physics
int birdX = 40, birdY = 194;
float vx = 0, vy = 0;
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30; 

// for Cursor
int cursorX = -1, cursorY = -1;

//for assets
const char *bg = "assets/images/Angry Bird3.jpg";
const char *catapultBack = "assets/images/CatapultBackSprite.png";
const char *catapultFront = "assets/images/CatapultFrontSprite.png";
const char *birdSprite = "assets/images/Blue_angry_bird5.png";
const char *menuBg = "assets/images/Icon.jpg";
const char *play = "assets\\images\\playButton2.bmp";
const char *pig="assets\\images\\Pig_front.png";

//for rubber position
int leftArmX = 298, leftArmY = 374;
int rightArmX = 336, rightArmY = 380;

//for brick data
#define MAX_BRICKS 5
int brickX[MAX_BRICKS] = {500, 560, 720, 780, 840};
int brickY[MAX_BRICKS] = {300, 300, 300, 300, 300};
int brickW = 60, brickH = 30;
bool brickVisible[MAX_BRICKS] = {true, true, true, true, true};


void drawMenu()
{
    iShowImage(-480, -234, menuBg);

    iSetColor(255, 215, 0);

    iShowImage(110, 270, play);
    iSetColor(0, 0, 0);
    iText(140, 285, "PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(220, 20, 60);

    iShowImage(110, 210, play);
    iSetColor(0, 0, 0);
    iText(140, 225, "Exit", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(30, 144, 255);

    iShowImage(110, 150, play);
    iSetColor(0, 0, 0);
    iText(140, 165, "Score", GLUT_BITMAP_TIMES_ROMAN_24);
}
void drawLevelSelect()
{
     iShowImage(-480, -234, menuBg);
    iSetColor(255, 255, 255);
    iText(140, 350, "Choose a Level", GLUT_BITMAP_TIMES_ROMAN_24);

   
    iShowImage(110,250, play);
    iSetColor(0, 0, 0);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    
    iShowImage(110,190, play);
    iSetColor(0, 0, 0);
    iText(120, 205, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    
    iShowImage(110,130, play);
    iSetColor(0, 0, 0);
    iText(120, 145, "Back", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawRubberLines() {
    iSetColor(100, 200, 0);
    iLine(leftArmX, leftArmY, birdX + birdRadius/2, birdY + birdRadius/2);
    iLine(rightArmX, rightArmY, birdX + birdRadius/2, birdY + birdRadius/2);
}

void drawpathway() {
    float fx = birdX + birdRadius / 2;
    float fy = birdY + birdRadius / 2;
    float tempVx = vx, tempVy = vy;
    float dt = 0.1f;
    for (int i = 0; i < 200; i++) {
        fx += tempVx * dt;
        fy += tempVy * dt;
        tempVy += g * dt;
        if (fy < 0) break;
        iSetColor(0, 0, 0);
        iFilledCircle(fx, fy, 2);
    }
}


void drawBricks() {
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (brickVisible[i]) {
            
            iSetColor(192, 192, 192); 
            iFilledRectangle(brickX[i], brickY[i], brickW, brickH + 20); 

            
            iSetColor(105, 105, 105); 
            iRectangle(brickX[i], brickY[i], brickW, brickH + 20);
        }
    }
}


void drawGame() {
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    if (isDragging)
        drawRubberLines();

    iShowImage(birdX, birdY, birdSprite);
    iShowImage(208, 177, catapultFront);

    if (isDragging || isFlying)
        drawpathway();
     //drawPigs();
    drawBricks();

    char str[20];
    sprintf(str, "Score: %d", score);
    iSetColor(0, 0, 0);
    iText(20, 570, str);

    if (cursorX != -1 && cursorY != -1) {
        char str[50];
        sprintf(str, "X: %d, Y: %d", cursorX, cursorY);
        iSetColor(0, 0, 0);
        iText(cursorX, cursorY, str, GLUT_BITMAP_8_BY_13);
    }
}

void updateBird() {
    if (isFlying) {
        birdX += vx;
        birdY += vy;
        vy += g;

    //    brick collision
        for (int i = 0; i < MAX_BRICKS; i++) {
            if (brickVisible[i]) {
                if (birdX + birdRadius > brickX[i] && birdX < brickX[i] + brickW &&
                    birdY + birdRadius > brickY[i] && birdY < brickY[i] + brickH+20) {
                    brickVisible[i] = false;
                    // isFlying = false;
                    // vx = vy = 0;
                }
            }
        }
      


        if (birdY < 0) {
            isFlying = false;
            vx = vy = 0;
        }
    }
}

void iDraw() {
    iClear();
    if (screen != currentScreen) {
        currentScreen = screen;
        if (screen == 0)
            PlaySound("assets/sounds/angry_birds_2.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else if (screen == 1)
            PlaySound("assets/sounds/angry_birds_intro_music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    if (screen == 0) drawMenu();
else if (screen == 1) drawGame();
else if (screen == 2) drawLevelSelect();

}

void iMouseMove(int mx, int my) {
    cursorX = mx; cursorY = my;
    if (isDragging && screen == 1) {
        birdX = mx;
        birdY = my;

        float dx = slingX - birdX;
        float dy = slingY - birdY;
        float v = sqrt(dx * dx + dy * dy);
        float angle = atan2(dy, dx);
        vx = v * cos(angle) * 0.5;
        vy = v * sin(angle) * 0.5;
    }
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
       if (screen == 0) {
    // Main menu 
    if (mx >= 140 && mx <= 260 && my >= 270 && my <= 310)
        screen = 2; 
    else if (mx >= 140 && mx <= 260 && my >= 210 && my <= 250)
        exit(0);
    else if (mx >= 140 && mx <= 260 && my >= 150 && my <= 190)
        score = 999;
}
else if (screen == 2) {
    // Level selection
    if (mx >= 110 && mx <= 230 && my >= 250 && my <= 290) {
        difficultyLevel = 1;  
        screen = 1;           
    }
    else if (mx >= 110 && mx <= 250 && my >= 190 && my <= 230) {
        difficultyLevel = 2;  
        screen = 1;         
    }
    else if (mx >= 110 && mx <= 230 && my >= 130 && my <= 170) {
        screen = 0;           
    }
}

        else if (screen == 1) {
            if (mx >= birdX && mx <= birdX + 70 && my >= birdY && my <= birdY + 70)
                isDragging = true;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && screen == 1) {
        isDragging = false;
        isFlying = true;
    }
}

void iKeyboard(unsigned char key) {
    if (key == 'm') screen = 0;
    else if (key == 'r') {
        birdX = 40; birdY = 194; vx = vy = 0; isFlying = false;
        // for (int i = 0; i < MAX_BRICKS; i++) brickVisible[i] = true;
    }
    else if (key == 's') score += 10;
    else if (key == 'q') exit(0);
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_END) exit(0);
}

void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iSetTimer(20, updateBird);
    iInitialize(800, 1000, "Angry Birds - BUET Project");
    return 0;
}