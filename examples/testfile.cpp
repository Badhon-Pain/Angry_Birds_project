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
int selectedBird = -1; // -1 means no bird selected
float g = -0.8;
int slingX = 312, slingY = 358;
int birdRadius = 30;

// Pig Position
int pig1X = 1070, pig1Y = 431;
float pigVy = 0;
bool pigFalling = false;
bool pigvisible = true;
int pigheight = 75, pigwidth = 75;

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

// Pillar
int pillarX = 1088, pillarY = 209;
int pillarW = 30, pillarH = 200;

// Ground level
int groundY = 0;

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

void drawLevelSelect() {
    iLoadImage(&levelbutton, "assets/images/10.png");
    iResizeImage(&levelbutton, 140, 130);
    iShowImage(-480, -234, menuBg);

    iShowLoadedImage(85, 210, &levelbutton);
    iSetColor(0, 0, 0);
    iText(120, 265, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 155, &levelbutton);
    iText(105, 210, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(85, 95, &levelbutton);
    iText(120, 150, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawRubberLines(int x, int y) {
    iSetColor(0, 0, 0);
    iLine(leftArmX, leftArmY, x + birdRadius / 2, y + birdRadius / 2);
    iLine(rightArmX, rightArmY, x + birdRadius / 2, y + birdRadius / 2);
}

// This function draws the parabolic flight path properly
void drawPathway(int x, int y, float vx, float vy) {
    float fx = x + birdRadius / 2;
    float fy = y + birdRadius / 2;
    float tempVx = vx;
    float tempVy = vy;
    float dt = 0.1f;

    for (int i = 0; i < 200; i++) {
        fx += tempVx * dt;
        fy += tempVy * dt;
        tempVy += g * dt; // gravity effect

        if (fy < 0) break; // stop if below ground

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

// Bounce logic for bird hitting ground or pillar
void updateBounce(int &x, int &y, float &vy, int w, int h) {
    if (y <= groundY) {
        y = groundY;
        vy = -vy * 0.6f; // bounce with damping
    }
    // Check collision with pillar from below
    if (x + w > pillarX && x < pillarX + pillarW && y <= pillarY + pillarH && y + h >= pillarY) {
        y = pillarY + pillarH;
        vy = -vy * 0.6f;
    }
}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying) {
    if (flying) {
        x += vx;
        y += vy;
        vy += g;

        updateBounce(x, y, vy, 70, 70);

        if (pigvisible) {
            bool collisionX = x + 70 >= pig1X && x <= pig1X + pigwidth;
            bool collisionY = y + 70 >= pig1Y && y <= pig1Y + pigheight;
            if (collisionX && collisionY) {
                pigFalling = true;
                PlaySound("assets/sounds/bird_01_collision_a1.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
        }
    }
}

void updatePig() {
    if (pigFalling) {
        pigVy += g;
        pig1Y += pigVy;
        if (pig1Y <= groundY) {
            pig1Y = groundY;
            pigvisible = false;
            pigFalling = false;
        }
    }
}

void updateBird() {
    updateSingleBird(bluebirdX, bluebirdY, blue_vx, blue_vy, blueFlying);
    updateSingleBird(redbirdX, redbirdY, red_vx, red_vy, redFlying);
    updateSingleBird(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy, yellowFlying);
    updatePig();
}

void drawGame() {
    iShowImage(0, 0, bg);
    iShowImage(208, 177, catapultBack);

    if (selectedBird == 0 && (blueDragging || blueFlying))
        drawRubberLines(bluebirdX, bluebirdY);
    else if (selectedBird == 1 && (redDragging || redFlying))
        drawRubberLines(redbirdX, redbirdY);
    else if (selectedBird == 2 && (yellowDragging || yellowFlying))
        drawRubberLines(yellowbirdX, yellowbirdY);

    drawBirds();

    iSetColor(192,192,192);
    iFilledRectangle(pillarX, pillarY, pillarW, pillarH);
    iFilledRectangle(1042, 402, 120, 30);

    if (pigvisible) {
        iLoadImage(&pigimage, "assets/images/Pig_front.png");
        iResizeImage(&pigimage, 65, 63);
        iShowLoadedImage(pig1X, pig1Y, &pigimage);
    }

    iShowImage(208, 177, catapultFront);

    if (selectedBird == 0 && (blueDragging || blueFlying))
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    else if (selectedBird == 1 && (redDragging || redFlying))
        drawPathway(redbirdX, redbirdY, red_vx, red_vy);
    else if (selectedBird == 2 && (yellowDragging || yellowFlying))
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

    iLoadImage(&scorebutton, "assets/images/4.png");
    iResizeImage(&scorebutton, 140, 130);
    iShowLoadedImage(25, 688, &scorebutton);
    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55, 750, str);

    if (cursorX != -1 && cursorY != -1) {
        char str[50];
        sprintf(str, "X: %d, Y: %d", cursorX, cursorY);
        iSetColor(0, 0, 0);
        iText(cursorX, cursorY, str, GLUT_BITMAP_8_BY_13);
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
    if (screen == 0)
        drawMenu();
    else if (screen == 1)
        drawGame();
    else if (screen == 2)
        drawLevelSelect();
}

void iMouseMove(int mx, int my) {
    cursorX = mx;
    cursorY = my;
    if (screen == 1) {
        int *x = nullptr, *y = nullptr;
        float *vx = nullptr, *vy = nullptr;
        bool *dragging = nullptr;

        if (selectedBird == 0) {
            x = &bluebirdX; y = &bluebirdY; vx = &blue_vx; vy = &blue_vy; dragging = &blueDragging;
        } else if (selectedBird == 1) {
            x = &redbirdX; y = &redbirdY; vx = &red_vx; vy = &red_vy; dragging = &redDragging;
        } else if (selectedBird == 2) {
            x = &yellowbirdX; y = &yellowbirdY; vx = &yellow_vx; vy = &yellow_vy; dragging = &yellowDragging;
        }

        if (dragging && *dragging) {
            *x = mx;
            *y = my;

            float dx = slingX - *x;
            float dy = slingY - *y;
            float v = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx);
            *vx = v * cos(angle) * 0.5f;
            *vy = v * sin(angle) * 0.5f;
        }
    }
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (screen == 0) {
            // Menu Buttons
            if (mx >= 110 && mx <= 240 && my >= 237 && my <= 357) {
                screen = 2; // Go to level select
            } else if (mx >= 110 && mx <= 240 && my >= 177 && my <= 297) {
                exit(0); // Exit game
            }
        } else if (screen == 2) {
            // Level Select Buttons
            if (mx >= 85 && mx <= 225 && my >= 210 && my <= 340) {
                difficultyLevel = 1; // Easy
                screen = 1;
            } else if (mx >= 85 && mx <= 225 && my >= 155 && my <= 285) {
                difficultyLevel = 2; // Medium
                screen = 1;
            } else if (mx >= 85 && mx <= 225 && my >= 95 && my <= 225) {
                screen = 0; // Back to menu
            }
        } else if (screen == 1) {
            // Bird selection for dragging
            if (mx >= bluebirdX && mx <= bluebirdX + 70 && my >= bluebirdY && my <= bluebirdY + 70) {
                blueDragging = true; selectedBird = 0;
            } else if (mx >= redbirdX && mx <= redbirdX + 70 && my >= redbirdY && my <= redbirdY + 70) {
                redDragging = true; selectedBird = 1;
            } else if (mx >= yellowbirdX && mx <= yellowbirdX + 70 && my >= yellowbirdY && my <= yellowbirdY + 70) {
                yellowDragging = true; selectedBird = 2;
            }
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && screen == 1) {
        // Release bird to fly
        if (selectedBird == 0 && blueDragging) {
            blueFlying = true; blueDragging = false;
        } else if (selectedBird == 1 && redDragging) {
            redFlying = true; redDragging = false;
        } else if (selectedBird == 2 && yellowDragging) {
            yellowFlying = true; yellowDragging = false;
        }
    }
}

void iKeyboard(unsigned char key) {
    if (key == 'm') screen = 0;
    else if (key == 'r') {
        // Reset positions and states
        bluebirdX = 20; bluebirdY = 194; blue_vx = blue_vy = 0; blueFlying = false; blueDragging = false;
        redbirdX = 100; redbirdY = 194; red_vx = red_vy = 0; redFlying = false; redDragging = false;
        yellowbirdX = 180; yellowbirdY = 200; yellow_vx = yellow_vy = 0; yellowFlying = false; yellowDragging = false;
        pig1Y = 431; pigvisible = true; pigFalling = false; pigVy = 0;
        selectedBird = -1;
        score = 0;
    } else if (key == 'q') exit(0);
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_END) exit(0);
}

void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iSetTimer(20, updateBird);
    iInitialize(800, 1000, "Angry Birds - Multi Bird Version with Bounce and Proper Path");
    return 0;
}

