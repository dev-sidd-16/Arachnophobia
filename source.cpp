/* ArachnoPhobia : A 2D graphics game with OpenGL
   Developed by : Siddhant Prakash / Roll : 201201008 / 
   Graphics Assignment 1
 */


/*Header Files*/
#include<iostream>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<time.h>
#include<unistd.h>
#include<vector>
#include<GL/glut.h>
#include<fstream>
#include<string>


using namespace std;

/*Constant defined */
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define MAX 100000
#define TRUE 1
#define FALSE 0



//structure of spider
typedef struct spider {
    float x;
    float y;
    float z;

    int color;

    float width;
    float height;
    float rad;

    float vel;
    bool atBase;

} Spider;

//structure of laser
typedef struct laser {
    float x;
    float y;
    float z;

    float len;

    float vely;
    float velx;

    float phi;
} Laser;

//function declarations
void initializeGlobalVars();
void testLine(float w);
void drawScene();
void drawSpace(float w, float h);
void drawWorld1(float w, float h); 
void drawWorld2(float w, float h); 
void drawWorld3(float w, float h); 
void update(int value);
void drawBox(float w, float h);
void drawCannon(float len);
void drawCannonLine(float len);
void deleteCannonLine(float len);
void drawBasket(float h, float w);
void drawBasketLine(float w, float h); 
void deleteBasketLine(float w, float h); 
void drawLaser(Laser l);
void drawSpider(Spider s);
void drawEyes(float r);
void drawRLegs(float r);
void drawLLegs(float r);
void move();
void moveRed();
void moveGreen();
void moveCannon();
void rotateCannon();
void laserShoot(int value);
void spiderGenerate(int value);
void spiderMove(int value);
void spiderHit(int value);
void spiderScored(int value);
void cannonHit(int value);
void levelImplement(int value);
void runningScore();
void finalScore();
void initRendering(); 
void handleResize(int w,int h);
void handleKeyPress1(unsigned char key, int x, int y);
void handleKeyPress2(int key, int x, int y);    
void handleMouseClick(int button, int state, int x, int y);


//gloabal Variables; Specifications of the game

//box specs
float box_width = 6.0f;
float box_height = 4.0f;

//basket specs
float basket_width ;
float basket_height  ;
float basket_y ;
float basketGreen_x ;                                      //initial position of Green Basket
float basketRed_x ;                                    //initial position of Red Basket
bool blockLGreen ;
bool blockLRed ;
float gBasketLLimit ;
float gBasketRLimit ;
float rBasketLLimit ;
float rBasketRLimit ;

//cannon specs
float cannon_length ;
float cannon_x ;
float cannon_y ;
float theta ;
bool blockLCannon ;
float cannonLLimit ;
float cannonRLimit ;

//laser specs
float laser_length ;
float laser_Vely  ;
float laser_Velx  ;
float laserVel = box_height/5;
int noLaser ;                                                        //no. of active lasers
int count_time ;                                                   //variable to control the time elapsed till next laser fire

Laser lasers[MAX];                                                          //to keep track of the lasers fired
bool laserFire[MAX];                                                    //to check whether the laser is fired or not
bool laserTrail[MAX];                                                   //to check whether the laser is in flight or not


//spider specs
float spider_x ;                                                        //to initialize random x position to spiders
float spider_y ;                                      //to initialize y position of spiders
float spider_width ;
float spider_height ;
float spider_rad ;
float max_vel ;
float min_vel ;

vector<Spider> spiders;                                                 //to keep track of the spiders generated

//random global variables for game control
char selectedObj ;                                                 //to check which object is selected for moving
bool paused ;
int scoreCard ;
int finished ;
int final;
int world = 0;                                                      
int lastMouseX, lastMouseY;
int exit_time ;
int view_Angle ;
int lineNo = 0;
int difficulty = 0;
int timer = 0;

//Main Function
int main(int argc, char **argv) {

    //initializing glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w;
    int windowHeight = h;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2 , (h - windowHeight) / 2);

    glutCreateWindow("Arachnophobia");                                          //setup the window
    initRendering();

    ifstream myFile(argv[1]);

    std::string line;
    if (myFile.is_open())
    {
        while( getline (myFile,line)){
            if(lineNo == 0)
                box_width = ::atof(line.c_str());
            else if(lineNo == 1)
                box_height = ::atof(line.c_str());
            else if(lineNo == 2)
                difficulty = ::atoi(line.c_str());
            else if(lineNo == 3)
                world = ::atoi(line.c_str());

            lineNo++;
        }

        initializeGlobalVars();
    }
    else
    {
        cout<<"File Does Not Exist !!\n";
        exit(0);
    }

    myFile.close();



    if(box_width > 6.0f || box_height > 4.0f)
        view_Angle = 75;




    //Register callbacks
    glutDisplayFunc(drawScene);    
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeyPress1);
    glutSpecialFunc(handleKeyPress2);
    glutMouseFunc(handleMouseClick);
    glutReshapeFunc(handleResize);
    glutTimerFunc(10,laserShoot,0);
    glutTimerFunc(5000,spiderGenerate,0);
    glutTimerFunc(10,spiderMove,0);
    glutTimerFunc(10,spiderHit,0);
    glutTimerFunc(10,spiderScored,0);
    glutTimerFunc(10,levelImplement,0);


    glutMainLoop();    
    return 0; 
}


//Function to Initialize all the Global Variables According to specifications input thorugh file
void initializeGlobalVars(){

    //basket specs
    basket_width = box_width/10;
    basket_height = box_height * 0.15 ;
    basket_y = - (0.95 * box_height)/2;
    basketGreen_x = 0.8 * box_width/2;                                      //initial position of Green Basket
    basketRed_x = - (0.8 * box_width)/2;                                    //initial position of Red Basket
    blockLGreen = FALSE;
    blockLRed = FALSE;
    gBasketLLimit = -box_width/2;
    gBasketRLimit = box_width/2;
    rBasketLLimit = -box_width/2;
    rBasketRLimit = box_width/2;

    //cannon specs
    cannon_length = 0.05 * box_height;
    cannon_x = 0.0f;
    cannon_y = (- (0.95 * box_height) / 2) + 0.1f;
    theta = 0.0f;
    blockLCannon = FALSE;
    cannonLLimit = -box_width/2;
    cannonRLimit = box_width/2;

    //laser specs
    laser_length = 0.1f * box_height;
    laser_Vely = laserVel * (cos(DEG2RAD(theta))) ;
    laser_Velx = laserVel * (-sin(DEG2RAD(theta))) ;
    noLaser = 0;                                                        //no. of active lasers
    count_time = 100;                                                   //variable to control the time elapsed till next laser fire


    //spider specs
    spider_y = 0.9 * box_height;                                      //to initialize y position of spiders
    spider_width = box_width/30;
    spider_height = box_height/20;
    spider_rad = box_width/30;

    //initializing difficulty level
    if(difficulty == 1){
        max_vel = box_height/1000;
        min_vel = box_height/2000;
        laserVel = box_height*100;
    }
    else if(difficulty == 2){
        max_vel = box_height/500;
        min_vel = box_height/1000;
        laserVel = box_height*500 ;
    }
    else if(difficulty == 3){
        max_vel = box_height/50;
        min_vel = box_height/500;
        laserVel = box_height*1000 ;
    }
    else if(difficulty >3){
        max_vel = box_height;
        min_vel = box_height/50;
        laserVel = box_height*2000 ;
    }

    //initializing difficulty according to timer
    if(timer > 100*30)
        difficulty++;
    else if(timer > 100*60)
        difficulty++;
    else if(timer > 100*90)
        difficulty++;
    else if(timer > 100*120)
        difficulty = 4;

    //initialize random global variables for game control
    selectedObj = 'C';                                                 //to check which object is selected for moving
    paused = FALSE;
    scoreCard = 0;
    finished = FALSE;
    exit_time = 0;
    view_Angle = 45;

}

/*=========================================================== Drawing Various Object On Screen =================================================*/

//Main Function to draw objects on the screen
void drawScene(){

    srand(time(NULL));
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    if(!finished)
    {
        //If the game is finished
        glPushMatrix();

        //Draw World
        glTranslatef(0.0f,0.0f, -5.0f);
        glColor3f(0.0f,0.6f,1.0f);
        drawSpace(box_width,box_height);
        drawBox(box_width,box_height);
        if(world == 1)
            drawWorld1(box_width,box_height);
        else if(world == 2)
            drawWorld2(box_width,box_height);
        else if(world == 3)
            drawWorld3(box_width,box_height);



        //draw the Laser
        for(i = 0 ; i < noLaser ; i++) {

            if(laserFire[i] ) {
                if(!laserTrail[i]) {
                    lasers[i].x = cannon_x ;
                    lasers[i].y = cannon_y ;
                    lasers[i].z = 0.0f;
                    lasers[i].phi = theta;
                    lasers[i].len = laser_length;

                    laser_Vely = laserVel * (cos(DEG2RAD(theta))) ;
                    laser_Velx = laserVel * (-sin(DEG2RAD(theta))) ;
                    lasers[i].velx = laser_Velx;
                    lasers[i].vely = laser_Vely;
                }

                lasers[i].vely = 0.01f * (cos(DEG2RAD(lasers[i].phi))) ;
                lasers[i].velx = 0.01f * (-sin(DEG2RAD(lasers[i].phi))) ;

                glPushMatrix();
                glTranslatef(lasers[i].x , lasers[i].y , 0.0f);
                glColor3f(0.6f,0.6f,0.9f);
                //testLine(2.0f);
                drawLaser(lasers[i]);
                glPopMatrix();
                laserTrail[i] = TRUE;

            }

        }
        //draw spiders
        for(unsigned k=0; k < spiders.size() ; k++) {
            glPushMatrix();
            glTranslatef(spiders.at(k).x,spiders.at(k).y,0.0f);
            switch(spiders.at(k).color) {
                case 0: glColor3f(0.5f,0.5f,0.5f);
                        break;
                case 1: glColor3f(1.0f,0.0f,0.0f);
                        break;
                case 2: glColor3f(0.0f,1.0f,0.0f);
                        break;
            }
            drawSpider(spiders.at(k));
            glPopMatrix();
        }

        //draw GREEN Basket
        glPushMatrix();
        glTranslatef(basketGreen_x, basket_y, 0.0f);
        glColor3f(0.0f,1.0f,0.0f);
        drawBasket(basket_width, basket_height);
        if(selectedObj == 'G')
            drawBasketLine(basket_width, basket_height);
        else
            deleteBasketLine(basket_width, basket_height);
        glPopMatrix(); 

        //draw RED Basket
        glPushMatrix();
        glTranslatef(basketRed_x, basket_y, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawBasket(basket_width, basket_height);
        if(selectedObj == 'R')
            drawBasketLine(basket_width, basket_height);
        else
            deleteBasketLine(basket_width, basket_height);
        glPopMatrix(); 




        //draw the Cannon
        glPushMatrix();
        glTranslatef(cannon_x, cannon_y, 0.0f);
        //    testLine(cannon_length,cannon_length);
        glColor3f(0.2f,0.0f,0.9f);
        glRotatef(theta, 0.0f,0.0f, 1.0f);
        drawCannon(cannon_length);
        if(selectedObj == 'C')
            drawCannonLine(cannon_length);
        else
            deleteCannonLine(cannon_length);
        glPopMatrix();

        //ScoreCard

        glPushMatrix();
        glTranslatef(box_width/2+0.1f,0.9*box_height/2,0.0f);
        runningScore();
        glPopMatrix();

        glPopMatrix();
        glutSwapBuffers();
    }
    else
    {
        //If the Game is not finished
        glPushMatrix();

        //Draw World
        glTranslatef(0.0f,0.0f, -5.0f);
        glColor3f(0.0f,0.6f,1.0f);
        drawSpace(box_width,box_height);
        drawBox(box_width,box_height);
        if(world == 2)
            drawWorld1(box_width,box_height);
        else if(world == 3)
            drawWorld2(box_width,box_height);
        else if(world == 1)
            drawWorld3(box_width,box_height);


        //ScoreCard
        glPushMatrix();
        glTranslatef(-box_width/10,0.0f, -5.0f);
        finalScore();
        glPopMatrix();

        glPopMatrix();
        paused = FALSE;
        glutSwapBuffers();
    }

}



//Function to draw the outer Space
void drawSpace(float w, float h){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glColor3f(0.3f,0.3f,0.3f);
    glVertex2f(-10*w,-10*h);
    glVertex2f(-10*w,10*h);
    glVertex2f(10*w,10*h);
    glVertex2f(10*w,-10*h);
    glEnd();
}

//Function to draw world boundary
void drawBox(float w, float h) {

    glLineWidth(10.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.6f,0.9f);
    glVertex2f(-w/2,-h/2);
    glVertex2f(w/2,-h/2);
    glVertex2f(w/2,h/2);
    glVertex2f(-w/2,h/2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(-w/2,-((0.9*h)/2));
    glVertex2f(w/2,-((0.9*h)/2));
    glEnd();
}

//Function to draw world 1 
void drawWorld1(float w, float h) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.2f,0.5f);
    glVertex2f(w/2,h/2);
    glVertex2f(-w/2,h/2);
    glColor3f(0.0f,0.3f,0.0f);
    glVertex2f(-w/2,-h/2);
    glVertex2f(w/2,-h/2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.8f,0.5f);
    glVertex2f(-w/2,-((0.9*h)/2));
    glVertex2f(w/2,-((0.9*h)/2));
    glEnd();
    glLineWidth(1.0f);
}

//Function to draw world 2
void drawWorld2(float w, float h) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.3f,0.9f);
    glVertex2f(w/2,h/2);
    glVertex2f(-w/2,h/2);
    glColor3f(0.4f,0.6f,0.4f);
    glVertex2f(-w/2,-h/2);
    glVertex2f(w/2,-h/2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.8f,0.8f);
    glVertex2f(-w/2,-((0.9*h)/2));
    glVertex2f(w/2,-((0.9*h)/2));
    glEnd();
    glLineWidth(1.0f);
}

//Function to draw world 3
void drawWorld3(float w, float h) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
    glBegin(GL_QUADS);
    glColor3f(0.5f,0.4f,0.1f);
    glVertex2f(w/2,h/2);
    glVertex2f(-w/2,h/2);
    glColor3f(0.5f,0.5f,0.5f);
    glVertex2f(-w/2,-h/2);
    glVertex2f(w/2,-h/2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.8f,0.8f);
    glVertex2f(-w/2,-((0.9*h)/2));
    glVertex2f(w/2,-((0.9*h)/2));
    glEnd();
    glLineWidth(1.0f);
}

//Function to draw cannon
void drawCannon(float len) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(-len/2,len*3/4);
    glVertex2f(len/2, -len/3);
    glVertex2f(len/2,len* 3/4);
    glVertex2f(-len/2, -len/3);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-len/4,len * 5/4);
    glVertex2f(-len/4,len * 0);
    glVertex2f(len/4,len * 0);
    glVertex2f(len/4,len * 5/4);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-len,0.0f);
    glVertex2f(len,0.0f);

    glVertex2f(-len,len/2);
    glVertex2f(-len,-len/2);

    glVertex2f(len,len/2);
    glVertex2f(len,-len/2);
    glEnd();
    glLineWidth(1.0f);
}

//Function to draw Cannon Outline when active
void drawCannonLine(float len) {

    glPushMatrix();
    glColor3f(1.0f,0.4f,0.0f);
    glLineWidth(3.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINES);
    glVertex2f(-len/2,len*3/4);
    glVertex2f(-len/2, -len/3);
    glVertex2f(-len/2, -len/3);
    glVertex2f(len/2, -len/3);
    glVertex2f(len/2, -len/3);
    glVertex2f(len/2,len* 3/4);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-len/4,len * 0);
    glVertex2f(-len/4,len * 5/4);
    glVertex2f(-len/4,len * 5/4);
    glVertex2f(len/4,len * 5/4);
    glVertex2f(len/4,len * 5/4);
    glVertex2f(len/4,len * 0);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPopMatrix();
}

//Function to remove Cannon outline
void deleteCannonLine(float len) {

    glPushMatrix();
    glColor3f(0.3f,0.3f,0.3f);
    glLineWidth(3.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINES);
    glVertex2f(-len/2,len*3/4);
    glVertex2f(-len/2, -len/3);
    glVertex2f(-len/2, -len/3);
    glVertex2f(len/2, -len/3);
    glVertex2f(len/2, -len/3);
    glVertex2f(len/2,len* 3/4);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-len/4,len * 0);
    glVertex2f(-len/4,len * 5/4);
    glVertex2f(-len/4,len * 5/4);
    glVertex2f(len/4,len * 5/4);
    glVertex2f(len/4,len * 5/4);
    glVertex2f(len/4,len * 0);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPopMatrix();
}

//Function to draw basket
void drawBasket(float w, float h) {
    float up_y = h ;
    float left_x = -(w/2);
    float right_x = (w/2);
    float bottom_y = 0.0f;
    float rad = w/10;

    glBegin(GL_TRIANGLE_FAN);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(right_x, bottom_y);
    glVertex2f(left_x,bottom_y);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(left_x,up_y);
    glVertex2f(right_x,up_y);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLineWidth(5.0f);
    glTranslatef(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.8f,0.6f);
    glVertex2f(left_x * 3/4,up_y);
    glVertex2f(right_x * 3/4,up_y);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0f,h,0.0f);
    glColor3f(0.9f,0.9f,0.9f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();



}

//Function to draw basket outline
void drawBasketLine(float w, float h) {
    float up_y = h ;
    float left_x = -(w/2);
    float right_x = (w/2);
    float bottom_y = 0.0f;
    float rad = w/10;

    glPushMatrix();
    glLineWidth(5.0f);
    glColor3f(0.0f,0.7f,1.0f);

    glBegin(GL_LINES);
    glVertex2f(left_x,up_y);
    glVertex2f(left_x,bottom_y);

    glVertex2f(right_x,bottom_y);
    glVertex2f(right_x,up_y);

    glEnd();

    glTranslatef(0.0f,h,0.0f);
    glBegin(GL_LINE_LOOP);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();

    glTranslatef(0.0f,-h,0.0f);
    glBegin(GL_LINES);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();

    glPopMatrix();

}

//Function to remove basket outline
void deleteBasketLine(float w, float h) {
    float up_y = h ;
    float left_x = -(w/2);
    float right_x = (w/2);
    float bottom_y = 0.0f;
    float rad = w/10;

    glPushMatrix();
    glLineWidth(5.0f);
    glColor3f(0.3f,0.3f,0.3f);

    glBegin(GL_LINES);
    glVertex2f(left_x,up_y);
    glVertex2f(left_x,bottom_y);
    glEnd();

    glTranslatef(0.0f,h,0.0f);
    glBegin(GL_LINE_LOOP);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();

    glTranslatef(0.0f,-h,0.0f);
    glBegin(GL_LINES);
    glVertex2f(right_x,up_y);
    glVertex2f(right_x,bottom_y);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for(int i=0 ; i<360 ; i++) {
        glVertex2f(5 * rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();

    glPopMatrix();

}

//Function to draw the laser
void drawLaser(Laser l) {

    float x1 = + ((l.len ) * sin(DEG2RAD(l.phi)));
    float y1 = - ((l.len ) * cos(DEG2RAD(l.phi)));

    float x2 = 0.0f;
    float y2 = 0.0f;

    l.x = -x1;
    l.y = -y1;
    glTranslatef(-x1,-y1,0.0f); 
    //testLine(l.y);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(x2,y2);
    glVertex2f(x1,y1);
    glEnd();

    glLineWidth(1);
}

//Function to draw spiders
void drawSpider(Spider s) {

    float r = s.rad;


    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i<360 ; i++){
        if(i<45 || i>180){
            glVertex2f(r * cos(DEG2RAD(i)), r * sin(DEG2RAD(i)));
        }
        else
        {
            switch(s.color) {
                case 0: glColor3f(0.1f,0.1f,0.1f);
                        break;
                case 1: glColor3f(0.5f,0.0f,0.0f);
                        break;
                case 2: glColor3f(0.0f,0.5f,0.0f);
                        break;
            }
            glVertex2f(r * cos(DEG2RAD(i)), r * sin(DEG2RAD(i)));
        }

    }
    glEnd();

    glPushMatrix();
    glTranslatef(0.0f,-r,0.0f);
    drawEyes(r);
    glPopMatrix();


    switch(s.color) {
        case 0: glColor3f(0.3f,0.3f,0.3f);
                break;
        case 1: glColor3f(0.7f,0.0f,0.0f);
                break;
        case 2: glColor3f(0.0f,0.7f,0.0f);
                break;
    }

    glPushMatrix();        
    glTranslatef(0.0f,r/2,0.0f);
    glRotatef(30.0f,0.0f,0.0f,1.0f);
    drawRLegs(r);
    glRotatef(-60.0f,0.0f,0.0f,1.0f);
    drawLLegs(r);
    glPopMatrix();

    glPushMatrix();        
    glTranslatef(0.0f,-r*4/5,0.0f);
    glRotatef(30.0f,0.0f,0.0f,1.0f);
    drawRLegs(r/2);
    glRotatef(-60.0f,0.0f,0.0f,1.0f);
    drawLLegs(r/2);
    glPopMatrix();


}

//Function to draw Eyes of Spider
void drawEyes(float r){

    glPushMatrix();
    glTranslatef(r/2,0.0f,0.0f);
    glColor3f(1.0f,1.0f,0.6f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i<360 ; i++){
        glVertex2f(r/3 * cos(DEG2RAD(i)), r/3 * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-r/2,0.0f,0.0f);
    glColor3f(1.0f,1.0f,0.6f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i<360 ; i++){
        glVertex2f(r/3 * cos(DEG2RAD(i)), r/3 * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(r*2/5,-r/10,0.0f);
    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i<360 ; i++){
        glVertex2f(r/5 * cos(DEG2RAD(i)), r/5 * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-r*2/5,-r/10,0.0f);
    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i<360 ; i++){
        glVertex2f(r/5 * cos(DEG2RAD(i)), r/5 * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();


}

//Function to draw Spiders legs
void drawRLegs(float r){

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(r/2,0.0f);
    glVertex2f(1.5*r ,0.0f);
    glVertex2f(1.5*r ,0.0f);
    glVertex2f(r/2 + r * 2 * cos(DEG2RAD(15.0f)), r * 2 * sin(DEG2RAD(15.0f)));
    glVertex2f(r/2 + r * 2 * cos(DEG2RAD(15.0f)), r * 2 * sin(DEG2RAD(15.0f)));
    glVertex2f(r/2 + r * 2 * cos(DEG2RAD(-60.0f)), r * 2 * sin(DEG2RAD(-60.0f)));
    glEnd();
}

void drawLLegs(float r){

    glBegin(GL_LINES);
    glVertex2f(-r/2,0.0f);
    glVertex2f(-1.5*r ,0.0f);
    glVertex2f(-1.5*r ,0.0f);
    glVertex2f(-r/2 + r * 2 * cos(DEG2RAD(165.0f)), r * 2 * sin(DEG2RAD(15.0f)));
    glVertex2f(-r/2 + r * 2 * cos(DEG2RAD(165.0f)), r * 2 * sin(DEG2RAD(15.0f)));
    glVertex2f(-r/2 + r * 2 * cos(DEG2RAD(240.0f)), r * 2 * sin(DEG2RAD(-60.0f)));
    glEnd();

}

/*============================================================ Main Game Control Engine ========================================================*/

//Function to generate spiders randomly at a constant interval of 2 seconds
void spiderGenerate(int value) {
    if(!paused){
        //    noSpider++;
        spider s;
        spider_x = (-box_width/2.5) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((box_width/2)-(-box_width/3))));
        s.x = spider_x;
        s.y = spider_y/2;

        float spider_c = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((3.0))));
        if(spider_c < 1.0)
            s.color = 0;
        else if(spider_c < 2.0)
            s.color = 1;
        else
            s.color = 2;

        s.width = spider_width;
        s.height = spider_height;
        s.rad = spider_rad;

        s.atBase = FALSE;
        s.vel = (min_vel) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((max_vel)-(min_vel))));
        spiders.push_back(s);
        //cout<<spiders.size()<<" Spiders Generated"<<"\n";
    }
    glutTimerFunc(5000, spiderGenerate, 0);
}


//Function to update laser position for its motion
void laserShoot(int value) {

    if(!paused){

        int i;

        count_time++;
        for(i = 0 ; i < noLaser ; i++)
        {
            if(laserTrail[i]){

                if(lasers[i].x+lasers[i].velx < -box_width/2 || lasers[i].x+lasers[i].velx > box_width/2 ) {
                    lasers[i].phi *= -1;
                    lasers[i].velx -= 1;                 
                }
                else {
                    //update laser position
                    lasers[i].x += lasers[i].velx; 
                    lasers[i].y += lasers[i].vely; 
                }
                if(lasers[i].y > box_height/2)
                    lasers[i].len = 0;
            }
        }
    }
    glutTimerFunc(10, laserShoot, 0);
}

//Function to move a spider
void spiderMove(int value) {
    if(!paused){
        unsigned int i;
        //draw the spiders
        for(i = 0 ; i < spiders.size() ; i++) {
            //update spider position
            if((spiders.at(i).y - spiders.at(i).vel > -0.85*box_height/2)){
                spiders.at(i).y -= spiders.at(i).vel;
            }
            else if(spiders.at(i).y > -box_height/2){
                spiders.at(i).y;

                if(spiders.at(i).x - spiders.at(i).rad < cannon_x + cannon_length/2 && spiders.at(i).x + spiders.at(i).rad > cannon_x - cannon_length/2){
                    if(!finished)
                        final = scoreCard;
                    finished = TRUE;
                    //cout<<"Final Score : "<<scoreCard<<"\n";
                }
                else{


                    if(!spiders.at(i).atBase){
                        scoreCard -= 3;
                        spiders.at(i).atBase = TRUE;
                    }

                    if( spiders.at(i).x > cannon_x && spiders.at(i).x < cannonRLimit){
                        cannonRLimit = spiders.at(i).x - spiders.at(i).rad;
                        //cout<<"cannoRLimit changed : "<<cannonRLimit<<"\n";
                    }
                    else if( spiders.at(i).x < cannon_x && spiders.at(i).x > cannonLLimit){
                        cannonLLimit = spiders.at(i).x + spiders.at(i).rad;
                        //cout<<"cannoLLimit changed : "<<cannonLLimit<<"\n";
                    }

                    if( spiders.at(i).x - spiders.at(i).rad > (basketGreen_x+(basket_width/2)) && spiders.at(i).x < gBasketRLimit){
                        gBasketRLimit = spiders.at(i).x - spiders.at(i).rad;
                        //cout<<"gBasketRLimit changed : "<<gBasketRLimit<<"\n";
                    }
                    else if( spiders.at(i).x + spiders.at(i).rad < (basketGreen_x-(basket_width/2)) && spiders.at(i).x > gBasketLLimit){
                        gBasketLLimit = spiders.at(i).x + spiders.at(i).rad;
                        //cout<<"gBasketLLimit changed : "<<gBasketLLimit<<"\n";
                    }

                    if( spiders.at(i).x - spiders.at(i).rad > (basketRed_x+(basket_width/2)) && spiders.at(i).x < rBasketRLimit){
                        rBasketRLimit = spiders.at(i).x - spiders.at(i).rad;
                        //cout<<"rBasketRLimit changed : "<<rBasketRLimit<<"\n";
                    }
                    else if( spiders.at(i).x + spiders.at(i).rad < (basketRed_x-(basket_width/2)) && spiders.at(i).x > rBasketLLimit){
                        rBasketLLimit = spiders.at(i).x + spiders.at(i).rad;
                    }
                }
            }
        }
    }
    glutTimerFunc(10, spiderMove, 0);
}

//Function to check whether a spider is hit or nor
//If hit, it removes the laser and the spiider
void spiderHit(int value) {
    if(!paused){

        int j;
        for(unsigned i=0; i < spiders.size(); i++){
            if(spiders.at(i).y > -0.8*box_height/2){
                for(j = 0; laserTrail[j] ; j++) { 
                    if(sqrt((pow(spiders.at(i).x - lasers[j].x,2)) + (pow(spiders.at(i).y - lasers[j].y,2))) <= spiders.at(i).rad ){
                        spiders.at(i).rad = 0;
                        spiders.at(i).x = -box_width;
                        spiders.at(i).y = -2*box_width;
                        lasers[j].x = 200;
                        lasers[j].len = 0;
                        if(spiders.at(i).color == 0){
                            scoreCard += 1;
                            //cout<<"Score : "<<scoreCard<<"Kill Black\n";
                        }
                        //cout<<spiders.size()<<" Spider Destroyed"<<"\n";
                    }
                }
            }
        }
    }
    glutTimerFunc(10,spiderHit,0);
}

//Function to check when a spider is scored
void spiderScored(int value){
    if(!paused){
        for(unsigned i=0; i < spiders.size() ; i++){
            if(spiders.at(i).x + spiders.at(i).rad <= (basketGreen_x)+(basket_width/2) && spiders.at(i).x - spiders.at(i).rad >= (basketGreen_x)-(basket_width/2) && spiders.at(i).color == 2 && spiders.at(i).y <=  ((-0.9* box_height)/2+ basket_height) && spiders.at(i).y >=  (-0.8* box_height)/2){
                spiders.at(i).rad = 0;
                spiders.at(i).x = -box_width;
                spiders.at(i).y = -2*box_width;
                //cout<<spiders.size()<<" Spider Destroyed"<<"\n";
                scoreCard += 1;
                //cout<<"Score : "<<scoreCard<<"Score Green\n";

            }
            else if(spiders.at(i).x + spiders.at(i).rad <= (basketGreen_x)+(basket_width/2) && spiders.at(i).x - spiders.at(i).rad >= (basketGreen_x)-(basket_width/2) && (spiders.at(i).color == 0 || spiders.at(i).color == 1) && spiders.at(i).y <=  ((-0.9* box_height)/2+ basket_height) && spiders.at(i).y >=  (-0.8* box_height)/2){

                spiders.at(i).rad = 0;
                spiders.at(i).x = -box_width;
                spiders.at(i).y = -2*box_width;
                //cout<<spiders.size()<<" Spider Destroyed"<<"\n";
                scoreCard -= 1;
                //cout<<"Score : "<<scoreCard<<"Miss Green\n";
            }


            if(spiders.at(i).x + spiders.at(i).rad <= (basketRed_x)+(basket_width/2) && spiders.at(i).x - spiders.at(i).rad >= (basketRed_x)-(basket_width/2) && spiders.at(i).color == 1 && spiders.at(i).y <=  ((-0.9* box_height)/2+ basket_height) && spiders.at(i).y >=  (-0.8* box_height)/2){
                spiders.at(i).rad = 0;
                spiders.at(i).x = -box_width;
                spiders.at(i).y = -2*box_width;
                //cout<<spiders.size()<<" Spider Destroyed"<<"\n";
                scoreCard += 1;
                //cout<<"Score : "<<scoreCard<<"Score Red\n";
            }
            else if(spiders.at(i).x + spiders.at(i).rad <= (basketRed_x)+(basket_width/2) && spiders.at(i).x - spiders.at(i).rad >= (basketRed_x)-(basket_width/2) && (spiders.at(i).color == 0 || spiders.at(i).color == 2) && spiders.at(i).y <=  ((-0.9* box_height)/2+ basket_height) && spiders.at(i).y >=  (-0.8* box_height)/2){

                spiders.at(i).rad = 0;
                spiders.at(i).x = -box_width;
                spiders.at(i).y = -2*box_width;
                //cout<<spiders.size()<<" Spider Destroyed"<<"\n";
                scoreCard -= 1;
                //cout<<"Score : "<<scoreCard<<"Miss Red\n";

            }

        }
    }
    glutTimerFunc(10,spiderScored,0);

}


//Function to implement difficulty level
void levelImplement(int value){
    timer++;                                                            // to keep track of time

    // to implement difficulty aaccording to levels
    if(difficulty == 1){
        max_vel = box_height/1000;
        min_vel = box_height/2000;
        laserVel = laserVel*100 ;
    }
    else if(difficulty == 2){
        max_vel = box_height/500;
        min_vel = box_height/1000;
        laserVel += difficulty*box_height*500 ;

    }
    else if(difficulty == 3){
        max_vel = box_height/50;
        min_vel = box_height/500;
        laserVel += difficulty*box_height*1000 ;
    }
    else if(difficulty >3){
        max_vel = box_height;
        min_vel = box_height/50;
        laserVel += difficulty*box_height*2000 ;
    }


    // to increase difficulty as time progresses
    if(timer == 100*30)
        difficulty++;
    else if(timer == 100*60)
        difficulty++;
    else if(timer == 100*90)
        difficulty++;
    else if(timer >= 100*120)
        difficulty = 4;

    if(finished)
        exit_time++;

    glutTimerFunc(10,levelImplement,0);
}


/*================================================================= Game Messages =============================================================*/

//Function to maintain a running Score and Show Difficulty level
void runningScore(){
    char text[32];
    sprintf(text, "Score: %d",scoreCard);
    glColor3f(0.0f,0.9f,0.1f);
    glRasterPos2f(0.0f,0.0f);
    for(int i=0 ; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    char diffMess[32];
    sprintf(diffMess, "Level : %d", difficulty);
    glColor3f(1.0f,0.7f,0.1f);
    glRasterPos2f(0.0f,-1.0f);
    for(int i=0 ; diffMess[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, diffMess[i]);
}

//Function to Display the final Score
void finalScore(){
    char text[32];
    sprintf(text, "Final Score : %d ",final);
    glColor3f(1.0f,0.7f,0.1f);
    glRasterPos2f(-0.3f,0.0f);
    for(int i=0 ; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    char exitMess[32];
    sprintf(exitMess, "Press Q to EXIT !! Thank You !!");
    glColor3f(1.0f,0.7f,0.1f);
    glRasterPos2f(-1.0f,-1.0f);
    for(int i=0 ; exitMess[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, exitMess[i]);



}

/*======================================================= Graphics Control & Event Handlers ===================================================*/

//Initialize some openGL 3D rendering options
void initRendering() {
    glEnable(GL_DEPTH_TEST);                                                    //enable objects to be drwan behind one another
    glEnable(GL_COLOR_MATERIAL);                                                //enable coloring
    glClearColor(1.0f,1.0f,1.0f,1.0f);                                          //setup a background color
}

//function called when window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view_Angle, (float)w / (float)h, 0.1,200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//to handle keypress of type 1
void handleKeyPress1(unsigned char key, int x, int y) {

    if(!paused){
        if(key == 27 || key == 81 || key == 113) {
            if(!finished)
                final = scoreCard;
            finished = TRUE;
            //cout<<"Final Score : "<<final<<"\n";
            if(exit_time > 100)
                exit(0);
        }
        else if(key == 82 || key == 114) {
            selectedObj = 'R';
        }
        else if(key == 71 || key == 103) {
            selectedObj = 'G';
        }
        else if(key == 66 || key == 98) {
            selectedObj = 'C';
        }
        else if(key == ' ' ) {
            if(count_time >= 100){
                laserFire[noLaser] = TRUE;
                system("aplay tos-photon-torpedo-1.wav &");
                noLaser++;
                count_time = 0;
            }
        }
    }

    if(key == 'p' || key == 'P') {
        if(paused){
            paused=FALSE;
        }
        else
            paused = TRUE;
    }
}

//to handle keypress of type 2
void handleKeyPress2(int key, int x, int y) {
    if(!paused){
        if(key == GLUT_KEY_LEFT) {


            switch(selectedObj) {

                case 'C' :
                    (cannon_x - cannon_length) < cannonLLimit? cannon_x : cannon_x -= 0.1f;
                    break;
                case 'G' : 
                    if( (basketGreen_x - basket_width/2 - 0.1f ) < gBasketLLimit){
                        basketGreen_x *= 1 ;
                    }
                    else{
                        basketGreen_x -= 0.1f;
                    }
                    break;
                case 'R' :  
                    if( (basketRed_x - basket_width/2 - 0.1f ) < rBasketLLimit){
                        basketRed_x *= 1;
                    }
                    else{
                        basketRed_x -= 0.1f;
                    }
                    break;
            }    
        }
        if(key == GLUT_KEY_RIGHT) {
            switch(selectedObj) {
                case 'C' : (cannon_x + cannon_length) > cannonRLimit? cannon_x : cannon_x += 0.1f;
                           break;
                case 'G' : if( (basketGreen_x + basket_width/2 + 0.1f ) > gBasketRLimit){
                               basketGreen_x *= 1 ;
                           }
                           else {
                               basketGreen_x += 0.1f;
                           }
                           break;
                case 'R' : if( (basketRed_x + basket_width/2 + 0.1f ) > rBasketRLimit){
                               basketRed_x *= 1 ;
                           }
                           else {
                               basketRed_x += 0.1f;
                           }
                           break;
            }        
        }
        if(key == GLUT_KEY_DOWN) {
            (theta - 5) < - 80 ? theta : theta -= 5;
        }
        if(key == GLUT_KEY_UP) {
            (theta + 5) > 80 ? theta : theta += 5;
        }
    }
}

//to handle mouse click events
void handleMouseClick(int button, int state, int x, int y) {

    int dx;
    if(button == GLUT_RIGHT_BUTTON){

        if( state == GLUT_DOWN){
            lastMouseX = x;
        }
        else if(state == GLUT_UP){
            dx = x-lastMouseX;
            if(dx > 0){            
                (theta - 10) < - 80 ? theta : theta -= 10;
            }
            else{
                (theta + 10) > 80 ? theta : theta += 10;
            }
        }
    }
    if (button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            if(count_time >= 100){
                laserFire[noLaser] = TRUE;
                system("aplay tos-photon-torpedo-1.wav &");
                noLaser++;
                count_time = 0;
            }
        }
    }
}


/*============================================================= Testing Function Used  =========================================================*/
//Function for test line        : Used to check the position of the cordinate axis after a translation is not clear

//Purely for CODE TESTING, NO RELATION TO GAME LOGIC
void testLine(float w) {
    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2f(-w/2,0.0f);
    glVertex2f(w/2,0.0f);
    glEnd();
}
/*=====================================================================X========================================================================*/
