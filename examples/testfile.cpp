#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include "iSound.h"
/*
screen =0 -> very first screen (menu screen)
screen =1 -> third screen (main game screen)
screen =2 -> second screen (level select screen)
screen =3 -> medium level screen 
screen =4 -> hard level screen



*/

#define ROWS 10
#define COLLUMS 20
#define BUET_ROWS 7
#define BUET_COLS 25

// Game State
int screen = 0, currentScreen = -1, score = 0;
int difficultylevel = 0;

// Bird Physics and States
bool bluevisible = true, redvisible = true, yellowvisible = true, blackvisible= true;
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
Image bg, gultiback, gultifront,map_block, map_mosaic, map_stone, woodblock,
    menuBg, blueImg, redImg, yellowImg, bg1, woodHorizontal, woodVertical,
    woodHorizontal2, woodVertical2, rock, blackImg,bgHard,
    menubutton, levelbutton, scorebutton, pigimage, credit;
    Image redframes[4];
    Sprite redSprite;

//for medium levels collision
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
    iLoadImage(&credit, "assets/images/terrace.png");
    iResizeImage(&credit, 1920, 1080);
    iLoadImage(&scorebutton, "assets/images/18.png");
    iLoadImage(&bg1, "assets/images/BG_03.png");
    iResizeImage(&bg1, 1920, 1080);
    iLoadImage(&map_block,"assets/images/Pillar_05_exs.png");
    iResizeImage(&map_block, 60, 60);
    iLoadImage(&map_mosaic, "assets/images/Tile_21.png");
    iResizeImage(&map_mosaic, 32, 32);
    iLoadImage(&map_stone, "assets/images/Tile_45.png");
    iResizeImage(&map_stone, 32, 32);
    iLoadImage(&woodblock, "assets/images/Wooden_Box.png" );
    iResizeImage(&woodblock, 32, 32);
    iLoadImage(&rock,"assets/images/Rock_06.png" );
    iLoadImage(&blackImg,"assets/images/Bomb.png" );
    iLoadImage(&bgHard,"assets/images/bgHard.jpg" );



    // iInitSprite(&redSprite); 
    // iLoadFramesFromFolder(redframes, "assets/images/sprites/red_bird"); 
    // iChangeSpriteFrames(&redSprite, redframes, 4); 
    // iSetSpritePosition(&redSprite, 75, 160);
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
    iText(120, 150, "HARD", GLUT_BITMAP_TIMES_ROMAN_24);
}

 

int map1[ROWS][COLLUMS] = {
    {0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0}, // top pig
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, // top horizontal
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1}, // pig
    {1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2, 1}, // mid horizontal
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1}, // vertical pillar
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1}, // vertical pillar
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1}, // bottom pig
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1}, // ground horizontal
    {1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};



void display_map1(){
    int startX = 1220, startY = 425; // base position
    int blockW = 30, blockH = 30;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLLUMS; c++)
        {
            int x = startX + c * blockW;
            int y = startY - r * blockH;

            if (map1[r][c] == 1 )
                iShowLoadedImage(x, y, &map_block); 
            else if (map1[r][c] == 2 )
                iShowLoadedImage2(x-9, y+18, &pigimage,50 ,45);
            else if (map1[r][c] == 3)
                iShowLoadedImage(x, y, &rock); 
        }
    }
}



int map2[BUET_ROWS][BUET_COLS] = {
    
    {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,1,1,1,1, 0,0,0,0,0}, 
    {1,0,0,1,0, 1,0,0,1,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0}, 
    {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 0,0,1,0,0, 0,0,0,0,0}, 
    {1,0,0,1,0, 1,0,0,1,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0}, 
    {1,1,1,0,0, 1,1,1,1,0, 1,1,1,0,0, 0,0,1,0,0, 0,0,0,0,0}, 
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}, 
    {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}  
};

void display_map2() {
    int blockSize = 32; 
    int startX = 930;    
    int startY = 305-115;    

    for (int row = 0; row < BUET_ROWS; row++) {
        for (int col = 0; col < BUET_COLS; col++) {
            if (map2[row][col] == 1) {
                int posX = startX + col * blockSize;
                int posY = startY - row * blockSize;
                iShowLoadedImage(posX, posY, &map_block);
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
        iShowLoadedImage(redbirdX, redbirdY, &redImg);

    if (yellowvisible)
        iShowLoadedImage(yellowbirdX, yellowbirdY, &yellowImg);
}

void drawBirds_medium()
{

    if (bluevisible)
        iShowLoadedImage2(bluebirdX, bluebirdY, &blueImg, 50, 50);

    if (redvisible)
        iShowLoadedImage2(redbirdX, redbirdY, &redImg, 50, 50);

    if (blackvisible)
        iShowLoadedImage2(yellowbirdX, yellowbirdY, &blackImg, 50, 50);
}

void drawBirds_hard()
{

    if (bluevisible)
        iShowLoadedImage(bluebirdX, bluebirdY-115, &blueImg);

    if (redvisible)
        iShowLoadedImage(redbirdX, redbirdY-115, &redImg);

    if (yellowvisible)
        iShowLoadedImage(yellowbirdX, yellowbirdY-115, &yellowImg);

}

void updateSingleBird(int &x, int &y, float &vx, float &vy, bool &flying, bool &visible) {
    if (flying) {
        x += vx;
        y += vy;
        vy += g;

        // Ground collision
        if (y <= groundY) {
            y = groundY;
            vy = -vy * 0.25;
            vx *= 0.5;
            
            if (fabs(vx) < 0.5 && fabs(vy) < 0.5) {
                flying = false;
                visible = false;
            }
            iPlaySound("assets/sounds/ball_bounce.wav");
        }
         if (x < 0 || x > 1890 || y < 205 || y > 1000 || vx < 0.05)
            {
                flying = false;
                visible = false;
                return;
            }

        // Collision with pillars
        for (int i = 0; i < pillarCount; i++) {
            int baseX = 1088 + i * 250;
            
            // Main vertical pillar collision
            if (x + birdwidth > baseX && x < baseX + pillarwidth &&
                y + birdheight > pillarY[i] && y < pillarY[i] + pillarheight) {
                
                vx = -vx * 0.5;
                vy *= 0.8;
                pillarHit[i] = true;
                iPlaySound("assets/sounds/wood_hit.wav");
            }
            
            //horizontal1
            if (x + birdwidth > baseX - 46 && x < baseX - 46 + 120 &&
                y + birdheight > 398 && y < 398 + 30) {
                
                vy = -vy * 0.25;
                vx *= 0.8;
                iPlaySound("assets/sounds/wood_hit.wav");
            }
            
            //horizontal2
            if (x + birdwidth > baseX + 111 && x < baseX + 111 + 60 &&
                y + birdheight > 294 && y < 294 + 30) {
                
                vy = -vy * 0.5;
                vx *= 0.8;
                iPlaySound("assets/sounds/wood_hit.wav");
            }
            
            // Small vertical pillar
            if (x + birdwidth > baseX + 125 && x < baseX + 125 + 30 &&
                y + birdheight > pillarY[i] && y < pillarY[i] + 100) {
                
                vx = -vx * 0.5;
                vy *= 0.8;
                iPlaySound("assets/sounds/wood_hit.wav");
            }
        }

        // Pig collision
        for (int i = 0; i < pigCount; i++) {
            if (pigVisible[i] && 
                x + birdwidth > pigX[i] && x < pigX[i] + pigwidth &&
                y + birdheight > pigY[i] && y < pigY[i] + pigheight) {
                
                pigFalling[i] = true;
                pigVX[i] = vx * 0.5;
                pigVY[i] = vy * 0.5;
                score += 100;
                iPlaySound("assets/sounds/pig_hit.wav");
            }
            updatePigMotion(i);
        }
    }
}

void draweasy()
{
   
    iShowLoadedImage2(0,0,&bg);
    iShowLoadedImage2(208,177, &gultiback);

    
    if (bluedragging)
        drawRubberLines(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines(redbirdX, redbirdY);
    if (yellowdragging)
        drawRubberLines(yellowbirdX, yellowbirdY);

   
    drawBirds_easy();

    
    if (bluedragging)
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redbirdX, redbirdY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);

   
    iShowLoadedImage2(208, 177, &gultifront);

   
    for (int i = 0; i < pillarCount; i++)
    {
        int baseX = 1088 + i * 250;
        iShowLoadedImage(baseX, pillarY[i], &woodVertical);        
        iShowLoadedImage(baseX - 46, 398, &woodHorizontal);            
        iShowLoadedImage(baseX + 111, 294, &woodHorizontal2);          
        iShowLoadedImage(baseX + 125, pillarY[i] - 4, &woodVertical2); 
    }

    // Draw pigs
    for (int i = 0; i < pigCount; i++)
    {
        if (pigVisible[i])
            iShowLoadedImage(pigX[i], pigY[i], &pigimage);
    }

    // Draw score panel

    iShowLoadedImage2(25, 888+50, &scorebutton, 140, 130);

    char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55-2, 950+50, str);
}

void drawmedium()
{
    iShowLoadedImage(0, 0, &bg1);
    iShowLoadedImage2(265, 140, &gultiback, 200, 200);
    // iShowSprite(&redSprite);
        if (bluedragging)
        drawRubberLines_medium(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines_medium(redbirdX, redbirdY);
    if (yellowdragging)
        drawRubberLines_medium(yellowbirdX, yellowbirdY);
   drawBirds_medium();
    display_map1();
        if (bluedragging)
        drawPathway(bluebirdX, bluebirdY, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redbirdX, redbirdY, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY, yellow_vx, yellow_vy);
        iShowLoadedImage2(265, 140, &gultifront, 200, 200);
        iShowLoadedImage2(25, 888+50, &scorebutton, 140, 130);
          char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55-2, 950+50, str);
}

void drawhard()
{
        iShowLoadedImage2(0,0, &bgHard, 1920, 1080 );
    iShowLoadedImage2(208, 177-115, &gultiback, 200, 200);

    
    if (bluedragging)
        drawRubberLines(bluebirdX, bluebirdY);
    if (reddragging)
        drawRubberLines(redbirdX, redbirdY);
    if (yellowdragging)
        drawRubberLines(yellowbirdX, yellowbirdY);

   
    drawBirds_hard();
    display_map2();

    
    if (bluedragging)
        drawPathway(bluebirdX, bluebirdY-115, blue_vx, blue_vy);
    if (reddragging)
        drawPathway(redbirdX, redbirdY-115, red_vx, red_vy);
    if (yellowdragging)
        drawPathway(yellowbirdX, yellowbirdY-115, yellow_vx, yellow_vy);

   iShowLoadedImage2(208, 177-115, &gultifront, 200, 200);
   
    
    iShowLoadedImage2(25, 888+50, &scorebutton, 140, 130);
      char str[20];
    sprintf(str, "SCORE: %d", score);
    iSetColor(0, 0, 0);
    iText(55-2, 950+50, str);





}

void iDraw()
{
    
    iClear();
    if (!isFullScreen) iToggleFullscreen();
    

    if (screen == 0)
        drawMenu();
    else if (screen == 1)
        draweasy();
    else if (screen == 2)
        drawLevelSelect();
    else if (screen == 3)
        drawmedium();
    else if (screen ==4)
        drawhard();
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
    if (screen == 1 || screen ==3 || screen ==4)
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
            iShowLoadedImage(0, 0, &credit);
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

        else if (mx >= 97&& mx <= 211 && my >= 144 && my <= 178) // hard button button
        {
            difficultylevel = 3;
            screen = 4;
            iStopAllSounds();
            iPlaySound("assets/sounds/angry_birds_intro_music.wav", true);
        }
    }

    else if ((screen == 1 || screen==3 || screen==4) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
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

    else if ((screen == 1 || screen ==3 || screen ==4) && button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
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
// void animate() {
//     iAnimateSprite(&redSprite); 
// }

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    loadResources();
    iInitializeSound();
    iPlaySound("assets/sounds/angry_birds_2.wav", true, 20);
    iSetTimer(20, updateBird);
    // iSetTimer(200, animate); 
    iOpenWindow(1920, 1080, "Angry Birds - BUET PROJECT");
    return 0;
}
