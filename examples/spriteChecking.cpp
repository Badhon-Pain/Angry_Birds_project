#include "iGraphics.h"

Image redFrames[4];       
Sprite redBird;           

void loadResources() {
    iInitSprite(&redBird); 
    iLoadFramesFromFolder(redFrames, "assets/images/sprites/red_bird"); 
    iChangeSpriteFrames(&redBird, redFrames, 4); 
    iSetSpritePosition(&redBird, 300, 300);
}

void iDraw() {
    iClear();
    iSetColor(0, 100, 200);
    iFilledRectangle(0, 0, 800, 600);

    iShowSprite(&redBird);

    iSetColor(255, 255, 255);
    iText(20, 20, "Red Bird Animation Sprite", GLUT_BITMAP_TIMES_ROMAN_24);
}

void iKeyboard(unsigned char key) {
    if (key == 'x') exit(0);
}

void iSpecialKeyboard(unsigned char key) {}
void iMouse(int button, int state, int mx, int my) {}
void iMouseMove(int mx, int my) {}
void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}

void animate() {
    iAnimateSprite(&redBird); 
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    loadResources();
    iSetTimer(500, animate); 
    iOpenWindow(800, 600, "Red Bird Sprite");
    return 0;
}
