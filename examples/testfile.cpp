#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include <windows.h>

// Game State
int screen = 0, currentScreen = -1, score = 0;
int difficultyLevel = 0;

// Bird Physics and States
int bluebirdX = 20, bluebirdY = 194;
int redbirdX = 100, redbirdY = 194;
int yellowbirdX = 180, yellowbirdY = 200;
float blue_vx = 0, blue_vy = 0;
float red_vx = 0, red_vy = 0;
float yellow_vx = 0, yellow_vy = 0;
bool blueFlying = false, redFlying = false, yellowFlying = false;
bool blueDragging = false, redDragging = false, yellowDragging = false;
int currentBird = 0; // 0: blue, 1: red, 2: yellow
float g = -9.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;

// Pig Position
int pig1X = 1070, pig1Y = 431;
int pigheight = 75, pigwidth = 75;
bool pigvisible = true;

// Cursor
int cursorX = -1, cursorY = -1;

// Assets
const char *bg = "assets/images/Angry Bird3.jpg";
const char *catapultBack = "assets/images/CatapultBackSprite.png";
const char *catapultFront = "assets/images/CatapultFrontSprite.png";
Image blueImg, redImg, yellowImg;
const char *menuBg = "assets/images/Icon.jpg";
Image menubutton, levelbutton, scorebutton, pigimage;

// Rubber Position
int leftArmX = 298, leftArmY = 374;
int rightArmX = 336, rightArmY = 380;

void drawMenu() {
    iShowImage(-480, -234, menuBg);

    iLoadImage(&menubutton, "assets/images/1.png");
    iResizeImage(&menubutton, 130, 120);

    iShowLoadedImage(110, 237, &menubutton);
    iSetColor(0, 0, 0);
    iText(140, 288, "PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(110, 177, &menubutton);
    iText(140, 228, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(110, 110, &menubutton);
    iText(132, 162, "CREDIT", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawRubberLines(int x, int y) {
    iSetColor(0, 0, 0);
    iLine(leftArmX, leftArmY, x + birdRadius / 2, y + birdRadius / 2);
    iLine(rightArmX, rightArmY, x + birdRadius / 2, y + birdRadius / 2);
}

void drawPathway(int x, int y, float vx, float vy) {
    float fx = x + birdRadius / 2;
    float fy = y + birdRadius / 2;
    float dt = 0.1f;
    for (int i = 0; i < 200; i++) {
        fx += vx * dt;
        fy += vy * dt;
        vy += g * dt;
        if (fy < 0) break;
        iSetColor(0, 0, 0);
        iFilledCircle(fx, fy, 2);
    }
}

void drawBirds() {
    iLoadImage(&blueImg, "assets/images/Blue_angry_bird5.png");
    iResizeImage(&blueImg, 65, 63);
    iShowLoadedImage(bluebirdX, bluebirdY, &blueImg);

    iLoadImage(&redImg, "assets/images/redAngryBird.png");
    iResizeImage(&redImg, 65, 63);
    iShowLoadedImage(redbirdX, redbirdY, &redImg);

    iLoadImage(&yellowImg, "assets/images/yellowAngryBird.png");
    iResizeImage(&yellowImg, 65, 63);
    iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying) {
    if (flying) {
        x += vx;
        y += vy;
        vy += g;
        if (y < 0) {
            flying = false;
            vx = vy = 0;
            currentBird++;
        }
        if (pigvisible) {
            int birdheight = 70, birdwidth = 70;
            bool collisionX = x + birdwidth >= pig1X && x <= pig1X + pigwidth;
            bool collisionY = y + birdheight >= pig1Y && y <= pig1Y + pigheight;
            if (collisionX && collisionY) {
                pigvisible = false;
                score += 100;
                PlaySound("assets/sounds/bird_01_collision_a1.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
        }
    }
}

void drawGame() {
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    if (blueDragging) drawRubberLines(bluebirdX, bluebirdY);
    else if (redDragging) drawRubberLines(redbirdX, redbirdY);
    else if (yellowDragging) drawRubberLines(yellowbirdX, yellowbirdY);

    drawBirds();

    if (blueDragging || blueFlying) drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    else if (redDragging || redFlying) drawPathway(redbirdX, redbirdY, red_vx, red_vy);
    else if (yellowDragging || yellowFlying) drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

    iShowImage(208, 177, catapultFront);
    if (pigvisible) {
        iLoadImage(&pigimage, "assets/images/Pig_front.png");
        iResizeImage(&pigimage, 65, 63);
        iShowLoadedImage(pig1X, pig1Y, &pigimage);
    }

    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(255, 255, 255);
    iText(50, 750, str);
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
}

void updateBird() {
    if (currentBird == 0) updateSingleBird(bluebirdX, bluebirdY, blue_vx, blue_vy, blueFlying);
    else if (currentBird == 1) updateSingleBird(redbirdX, redbirdY, red_vx, red_vy, redFlying);
    else if (currentBird == 2) updateSingleBird(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowFlying);
}

void iMouseMove(int mx, int my) {
    cursorX = mx;
    cursorY = my;
    if (screen == 1) {
        if (blueDragging) {
            bluebirdX = mx;
            bluebirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            blue_vx = v * cos(angle) * 0.5;
            blue_vy = v * sin(angle) * 0.5;
        }
        if (redDragging) {
            redbirdX = mx;
            redbirdY = my;
            float dx = slingX - mx;
            float dy = slingY - my;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            red_vx = v * cos(angle) * 0.5;
            red_vy = v * sin(angle) * 0.5;
        }
        if (yellowDragging) {
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

void iMouse(int button, int state, int mx, int my) {
    if (screen == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentBird == 0 && mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70)
            blueDragging = true;
        else if (currentBird == 1 && mx >= redbirdX && mx <= redbirdX + 70 && my >= redbirdY && my <= redbirdY + 70)
            redDragging = true;
        else if (currentBird == 2 && mx >= yellowbirdX && mx <= yellowbirdX + 70 && my >= yellowbirdY && my <= yellowbirdY + 70)
            yellowDragging = true;
    }
    if (screen == 1 && button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        if (currentBird == 0) {
            blueDragging = false;
            blueFlying = true;
        } else if (currentBird == 1) {
            redDragging = false;
            redFlying = true;
        } else if (currentBird == 2) {
            yellowDragging = false;
            yellowFlying = true;
        }
    }
}

void iKeyboard(unsigned char key) {
    if (key == 'r') {
        bluebirdX = 20; bluebirdY = 194; blue_vx = blue_vy = 0; blueFlying = blueDragging = false;
        redbirdX = 100; redbirdY = 194; red_vx = red_vy = 0; redFlying = redDragging = false;
        yellowbirdX = 180; yellowbirdY = 200; yellow_vx = yellow_vy = 0; yellowFlying = yellowDragging = false;
        currentBird = 0;
        pigvisible = true;
        score = 0;
    }
    if (key == 'q') exit(0);
    if (key == 'm') screen = 0;
    if (key == 's') score += 10;
    if (key == '1') screen = 1;
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_END) exit(0);
}

void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iSetTimer(20, updateBird);
    iInitialize(800, 1000, "Angry Birds - Multi Bird Version");
    return 0;
}
