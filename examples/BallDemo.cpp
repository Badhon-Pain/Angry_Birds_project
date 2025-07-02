#include "iGraphics.h"


int x=0, y=0;

int ball_x, ball_y;
int dx = 5, dy = 7;
int ball_radius = 7;
int r=255, g=255, b=255;

bool musicOn =true;


const char* bc[3]= {"assets\\images\\background.jpg","assets\\images\\mmario.png","assets\\images\\rect.png"};
int bcIndex =0;
 const char* golem[12]={"assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_000.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_001.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_002.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_003.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_004.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_005.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_006.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_007.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_008.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_009.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_010.png","assets\\images\\sprites\\Golem_2\\Running\\0_Golem_Running_011.png"};
 int golemIndex =0;
/*
    function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here

    iClear();

    // iSetColor(r,g,b);
    // iFilledCircle(ball_x, ball_y, ball_radius);

    // iSetColor(255, 255, 255);
    // iText(100, 100, "Badhon Pain",GLUT_BITMAP_TIMES_ROMAN_24);
    // iShowImage(0, 0, bc[bcIndex]);
        iShowImage(100, 100,  golem[ golemIndex]);
    
    // iText(100, 100, "Badhon Pain",GLUT_BITMAP_TIMES_ROMAN_24);
}
void updateGolemIndex()
{
    golemIndex= (golemIndex+1) %12;
}

/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
    function iMouseMove() is called automatically when the mouse pointer is in motion
*/
void iMouseMove(int mx, int my)
{
    // place your code here
}
void iMouseWheel(int dir, int mx, int my)
{
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if (key == 'w'){
        // do something with 'q'
        // iPauseTimer(0);
        x+=10;
    }
    if (key == 's')
    {
        // iResumeTimer(0);
        x-=10;
    }
      if (key == 'a')
    {
        // iResumeTimer(0);
        y-=10;
    }
      if (key == 'd')
    {
        // iResumeTimer(0);
        y+=10;
    }
    // place your codes for other keys here
}

/*
    function iSpecialKeyboard() is called whenver user hits special keys like-
    function keys, home, end, pg up, pg down, arraows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_UP)
    {
      if(musicOn)
      {
        musicOn= false;
        PlaySound(0, 0, 0);
      }
      else {
        musicOn= true;
        
    PlaySound("assets\\sounds\\background.wav", NULL, SND_ASYNC | SND_LOOP);
      }
    }
 else if (key == GLUT_KEY_DOWN)
 {
   
 }
 
 else if (key == GLUT_KEY_LEFT)
 {
    
 }
 

}

void ballChange()
{
    /*

    ball_x += dx;
    ball_y += dy;
    r=rand()%255;
    g=rand()%255;
    b=rand()%255;

    if (ball_x + ball_radius > iScreenWidth || ball_x - ball_radius < 0)
    {
        if (ball_x + ball_radius > iScreenWidth)
            ball_x = iScreenWidth - ball_radius;
        else
            ball_x = ball_radius;
        dx = -dx;
    }

    if (ball_y + ball_radius > iScreenHeight || ball_y - ball_radius < 0)
    {
        if (ball_y + ball_radius > iScreenHeight)
            ball_y = iScreenHeight - ball_radius;
        else
            ball_y = ball_radius;
        dy = -dy;
    }
        */
}

void randomizeBallPosition()
{
    /*
    ball_x = rand() % (iScreenWidth - 2 * ball_radius) + ball_radius;
    ball_y = rand() % (iScreenHeight - 2 * ball_radius) + ball_radius;
    */
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    iSetTimer(10, ballChange);
        iSetTimer(100, updateGolemIndex );
    // iSetTimer(2000, randomizeBallPosition);
    if(musicOn)
    PlaySound("assets\\sounds\\background.wav", NULL, SND_ASYNC | SND_LOOP);



     iInitialize(800, 600, "Golem Animation");
    // iInitialize(400, 400, "Angry Bird");

    return 0;
}
