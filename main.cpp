#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <mmsystem.h>
#include <stdlib.h> // For rand()
#include <time.h>   // For srand()
    

/*
====================================================
  Happy Paws - Computer Graphics Group Project

  Screen: Feeding Screen
  Name: Iftekhar Tasnim Md. Asif
  ID: 22-478960-2
  Group: 1
====================================================
*/

// Function declarations
void playFeedingSound();
void mouse(int button, int state, int x, int y);
void foodReappear(int foodIndex);
void drawFeedingBackground();
void drawFeedingTableAndPlates();
void drawCatWithMouthAnimation();
void drawFoodItem(int i);
void drawHintBubble();

void sound() {
   PlaySound("bg.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void playFeedingSound() {
   PlaySound("meow.wav", NULL, SND_ASYNC | SND_FILENAME);
}

void animateFoodToMouth(int value); 

float smileAmount = 0.0f;

// All screens
enum ScreenType {
    INTRO_SCREEN,
    FEEDING_CAT_SCREEN,
    DANCE_SCREEN,
    SLEEP_SCREEN,
    SHOWER_SCREEN,
    FUN_SCREEN,
};
ScreenType currentScreen = INTRO_SCREEN;

void display();
void reshape(int, int);
void update(int); 
void drawWindow();
void drawClock();
void drawTree();
void drawOutsideView();
void drawFeedingCatScreen();
void keyboard(unsigned char key, int x, int y);
void drawIntroScreen();
// New blank screens
void drawDanceScreen();
void drawSleepScreen();
void drawShowerScreen();
void drawFunScreen();

// Cloud positions
float cloud1X = 40.0f;
float cloud2X = 75.0f;
float cloud3X = 110.0f; 

// Tree Animation
float swayAngle = 0.0f;
float swayDirection = 0.05f;

// =================== Food Animation ===================
struct Food {
    float plateX, plateY; // Plate position
    float x, y;          // Current position
    bool isMoving;       
    bool isVisible;      
};

Food foods[3] = {
    { -50, -80, -50, -80, false, true }, // Left plate 
    { 0,   -80, 0,   -80, false, true }, // Middle plate 
    { 50,  -80, 50,  -80, false, true }  // Right plate 
};

int animatingFood = -1; // Index of food being animated, -1 if none
float catMouthX = 0.0f, catMouthY = -0.0f; // mouth position
bool catIsEating = false;
int eatTimer = 0;



// Helper functions for Cat
void drawCircle(float cx, float cy, float r, int numSegments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * 3.1415926f * i / numSegments;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawCloud(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);
    // A cloud is made of a few overlapping white circles
    drawCircle(0.0f, 0.0f, 5.0f, 30, 1.0f, 1.0f, 1.0f);
    drawCircle(6.0f, 2.0f, 4.0f, 30, 1.0f, 1.0f, 1.0f);
    drawCircle(-5.0f, 1.0f, 4.5f, 30, 1.0f, 1.0f, 1.0f);
    drawCircle(2.0f, -3.0f, 3.5f, 30, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void drawTree() {
    // Tree Trunk
    drawRectangle(45.0f, 27.0f, 50.0f, 45.0f, 0.5f, 0.35f, 0.05f);


    glPushMatrix();
    glTranslatef(47.5f, 45.0f, 0.0f);
    glRotatef(swayAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-47.5f, -45.0f, 0.0f);


    drawCircle(47.5f, 55.0f, 12.0f, 30, 0.0f, 0.5f, 0.0f);
    drawCircle(40.5f, 50.0f, 10.0f, 30, 0.0f, 0.6f, 0.0f);
    drawCircle(54.5f, 50.0f, 10.0f, 30, 0.0f, 0.6f, 0.0f);
    glPopMatrix();
}

void drawOutsideView() {
    // Sky Background 
    glBegin(GL_QUADS);
    // Top (deeper blue)
    glColor3f(0.0f, 0.55f, 0.95f);
    glVertex2f(32.0f, 93.0f);
    glVertex2f(83.0f, 93.0f);
    // Bottom (lighter blue )
    glColor3f(0.53f, 0.81f, 0.98f);
    glVertex2f(83.0f, 60.0f);
    glVertex2f(32.0f, 60.0f);
    glEnd();

    // Sun 
    float sunCenterX = 75.0f, sunCenterY = 85.0f, sunRadius = 4.0f;
    int sunTriangles = 40;
    glColor3f(1.0f, 0.95f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sunCenterX, sunCenterY);
    for (int i = 0; i <= sunTriangles; ++i) {
        float angle = 2.0f * 3.1415926f * float(i) / float(sunTriangles);
        float dx = sunRadius * cosf(angle);
        float dy = sunRadius * sinf(angle);
        glVertex2f(sunCenterX + dx, sunCenterY + dy);
    }
    glEnd();

    // Grass 
    glBegin(GL_QUADS);
    // Top (lighter green)
    glColor3f(0.6f, 1.0f, 0.4f);
    glVertex2f(32.0f, 60.0f);
    glVertex2f(83.0f, 60.0f);
    // Bottom (darker green)
    glColor3f(0.2f, 0.7f, 0.1f);
    glVertex2f(83.0f, 27.0f);
    glVertex2f(32.0f, 27.0f);
    glEnd();

    // Draw tree
    drawTree();

    // Clouds
    drawCloud(cloud1X, 80.0f, 0.8f);
    drawCloud(cloud2X, 85.0f, 0.6f);
    drawCloud(cloud3X, 77.0f, 1.0f);
}

void drawWindow() {
    // window frame
    glBegin(GL_QUADS);
    glColor3f(0.95f, 0.65f, 0.85f); // Pink
    glVertex2f(30.0f, 95.0f);
    glVertex2f(85.0f, 95.0f);
    glVertex2f(85.0f, 25.0f);
    glVertex2f(30.0f, 25.0f);
    glEnd();

    // window pane
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glVertex2f(32.0f, 93.0f);
    glVertex2f(83.0f, 93.0f);
    glVertex2f(83.0f, 27.0f);
    glVertex2f(32.0f, 27.0f);
    glEnd();

  
    glEnable(GL_SCISSOR_TEST);
    glScissor(528, 508, 204, 264);

    drawOutsideView();

    glDisable(GL_SCISSOR_TEST);
}

void drawCat(float mouthOpen) {
    // Scale and position the cat
    glPushMatrix();
    glTranslatef(0.0f, -10.0f, 0.0f);  // Adjusted position
    glScalef(100.0f, 100.0f, 1.0f);    // Scaling 


    // Left arm 
    glPushMatrix();
    glTranslatef(-0.25f, -0.15f, 0.0f);
    glRotatef(-50, 0.0f, 0.0f, 1.0f);
    drawRectangle(-0.03f, 0.0f, 0.03f, -0.32f, 0.9f, 0.8f, 0.7f); // Arm
    drawCircle(0.0f, -0.36f, 0.06f, 20, 0.976, 0.941, 0.910);      // Paw
    glPopMatrix();

    // Right arm 
    glPushMatrix();
    glTranslatef(0.25f, -0.15f, 0.0f);
    glRotatef(50, 0.0f, 0.0f, 1.0f);
    drawRectangle(-0.03f, 0.0f, 0.03f, -0.32f, 0.9f, 0.8f, 0.7f); // Arm
    drawCircle(0.0f, -0.36f, 0.06f, 20, 0.976, 0.941, 0.910);      // Paw
    glPopMatrix();

    // Body
    drawCircle(0.0f, -0.4f, 0.4f, 50, 0.9f, 0.8f, 0.7f);
    // Chest
    drawCircle(0.0f, -0.35f, 0.3f, 50, 0.976, 0.941, 0.910);

    // Head
    drawCircle(0.0f, 0.2f, 0.3f, 50, 0.9f, 0.8f, 0.7f);

    //outer Ears
    glColor3f(0.9f, 0.8f, 0.7f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.2f, 0.4f); // Left ear
    glVertex2f(-0.1f, 0.6f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.2f, 0.4f); // Right ear
    glVertex2f(0.1f, 0.6f);
    glVertex2f(0.0f, 0.4f);
    glEnd();

    // Inner ears
    glColor3f(1.0, 0.905, 0.914);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, 0.45f);
    glVertex2f(-0.1f, 0.57f);
    glVertex2f(-0.05f, 0.49f);
    glVertex2f(0.15f, 0.45f);
    glVertex2f(0.1f, 0.57f);
    glVertex2f(0.05f, 0.49f);
    glEnd();

    // Lines on head
    glColor3f(0.37f, 0.18f, 0.16f);
    drawLine(-0.03f, 0.5f, -0.03f, 0.45f, 0.37f, 0.18f, 0.16f);
    drawLine(0.0f, 0.5f, 0.0f, 0.45f, 0.37f, 0.18f, 0.16f);
    drawLine(0.03f, 0.5f, 0.03f, 0.45f, 0.37f, 0.18f, 0.16f);

    // Eyes (blinking while eating)
    bool isBlinking = false;
    if (catIsEating && eatTimer > 0) {
        // Blink for 10 frames in the middle of eating
        if (eatTimer <= 25 && eatTimer >= 15) {
            isBlinking = true;
        }
    }

    if (isBlinking) {
        // Draw closed eyes (lines)
        glColor3f(0.37f, 0.18f, 0.16f);
        glBegin(GL_LINES);
        glVertex2f(-0.13f, 0.25f); glVertex2f(-0.07f, 0.25f);
        glVertex2f(0.07f, 0.25f); glVertex2f(0.13f, 0.25f);
        glEnd();
    } else {
        // Draw open eyes
        drawCircle(-0.1f, 0.25f, 0.05f, 20, 0.37f, 0.18f, 0.16f);
        drawCircle(0.1f, 0.25f, 0.05f, 20, 0.37f, 0.18f, 0.16f);
        drawCircle(-0.1f, 0.25f, 0.02f, 20, 1.0f, 1.0f, 1.0f);
        drawCircle(0.1f, 0.25f, 0.02f, 20, 1.0f, 1.0f, 1.0f);
    }

    // Nose
    glColor3f(1.0f, 0.5f, 0.5f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.02f, 0.2f);
    glVertex2f(0.02f, 0.2f);
    glVertex2f(0.0f, 0.18f);
    glEnd();

    // Mouth
    glColor3f(0.37f, 0.18f, 0.16f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 10; ++i) {
        float theta = 3.1415926f * (i / 10.0f); // 0 to PI (half circle)
        float x = 0.03f * cos(theta);
        float y;
        if (mouthOpen > 0.0f) {
            y = 0.13f - (0.03f + mouthOpen) * sin(theta); // animated during feeding
        } else {
            y = 0.13f - 0.03f * sin(theta); // static when idle
        }
        glVertex2f(x, y);
    }
    glEnd();

    // Whiskers
    drawLine(-0.15f, 0.2f, -0.3f, 0.25f, 0.37f, 0.18f, 0.16f);
    drawLine(-0.15f, 0.18f, -0.3f, 0.18f, 0.37f, 0.18f, 0.16f);
    drawLine(-0.15f, 0.16f, -0.3f, 0.11f, 0.37f, 0.18f, 0.16f);
    drawLine(0.15f, 0.2f, 0.3f, 0.25f, 0.37f, 0.18f, 0.16f);
    drawLine(0.15f, 0.18f, 0.3f, 0.18f, 0.37f, 0.18f, 0.16f);
    drawLine(0.15f, 0.16f, 0.3f, 0.11f, 0.37f, 0.18f, 0.16f);

    // Collar
    drawRectangle(-0.2f, -0.02f, 0.2f, -0.05f, 0.910, 0.278, 0.416);
    drawCircle(0.0f, -0.1f, 0.05f, 30, 1.0, 1.0, 0.624);
    drawLine(-0.02f, -0.1f, 0.02f, -0.1f, 0.0f, 0.0f, 0.0f);

    

    // Legs
    drawRectangle(-0.2f, -0.82f, -0.1f, -0.7f, 0.9f, 0.8f, 0.7f);
    drawRectangle(0.1f, -0.82f, 0.2f, -0.7f, 0.9f, 0.8f, 0.7f);

    // Paws (front)
    drawCircle(-0.15f, -0.85f, 0.07f, 20, 0.976, 0.941, 0.910); // Left paw
    drawCircle(0.15f, -0.85f, 0.07f, 20, 0.976, 0.941, 0.910);  // Right paw

    // Optional: Paws (back, for a sitting pose)
    // drawCircle(-0.25f, -0.75f, 0.06f, 20, 0.976, 0.941, 0.910); // Back left paw
    // drawCircle(0.25f, -0.75f, 0.06f, 20, 0.976, 0.941, 0.910);  // Back right paw

    glPopMatrix();
}

void init() {
    glClearColor(0.69f, 0.88f, 0.90f, 1.0f); // Light blue background color
    // sound(); // Remove this line so music doesn't start at launch
}

void update(int value) {
    // Move clouds
    cloud1X -= 0.05f;
    if (cloud1X < 20.0f) { // cloud reset position
        cloud1X = 90.0f;
    }

    cloud2X -= 0.08f;
    if (cloud2X < 20.0f) {
        cloud2X = 90.0f;
    }

    cloud3X -= 0.03f;
    if (cloud3X < 20.0f) {
        cloud3X = 90.0f;
    }

    // Update tree sway
    swayAngle += swayDirection;
    if (swayAngle > 2.0f || swayAngle < -2.0f) {
        swayDirection = -swayDirection;
    }

    smileAmount = 0.02f + 0.02f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.002f); // oscillates between 0.0 and 0.04

    glutPostRedisplay(); 
    glutTimerFunc(16, update, 0); //refresh at 16ms
}

/*
====================================================
    
  Screen: Feeding Cat Screen
  
====================================================
*/  

void drawFeedingCatScreen() {
    drawFeedingBackground();
    drawClock();
    drawWindow();
    drawCatWithMouthAnimation(); 
    drawFeedingTableAndPlates(); // Draw table and plates after
    drawHintBubble();
    glFlush();
}

void drawFeedingBackground() {
    // Wall
    glBegin(GL_QUADS);
    glColor3f(0.98f, 0.85f, 0.95f); // Soft pink
    glVertex2f(-100.0f, 100.0f);
    glVertex2f(100.0f, 100.0f);
    glVertex2f(100.0f, -20.0f);
    glVertex2f(-100.0f, -20.0f);
    glEnd();
    // Wall design - Horizontal line
    glBegin(GL_QUADS);
    glColor3f(0.95f, 0.75f, 0.90f); // darker pink
    glVertex2f(-100.0f, 30.0f);
    glVertex2f(100.0f, 30.0f);
    glVertex2f(100.0f, 28.0f);
    glVertex2f(-100.0f, 28.0f);
    glEnd();
    // Floor
    glBegin(GL_QUADS);
    glColor3f(0.95f, 0.95f, 0.75f); // Soft yellow
    glVertex2f(-100.0f, -20.0f);
    glVertex2f(100.0f, -20.0f);
    glVertex2f(100.0f, -100.0f);
    glVertex2f(-100.0f, -100.0f);
    glEnd();
    // Floor design - vertical lines
    glColor3f(0.85f, 0.85f, 0.65f); // Lighter yellow
    glBegin(GL_LINES);
    for(float i = -100; i <= 100; i += 13) {
        glVertex2f(i, -20.0f);
        glVertex2f(i, -100.0f);
    }
    glEnd();
    // Floor design - horizontal lines
    glBegin(GL_LINES);
    for(float i = -100; i <= -20; i += 9) {
        glVertex2f(-100.0f, i);
        glVertex2f(100.0f, i);
    }
    glEnd();
}

void drawFeedingTableAndPlates() {
    // Dining Table
    glColor3f(0.7f, 0.9f, 1.0f); // Light blue
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        float x = 0 + 90 * cos(theta);
        float y = -90 + 40 * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
    // Plates and Food
    for (int p = -1; p <= 1; p++) {
        int i = p + 1; // food index: -1->0, 0->1, 1->2
        // Draw plate
        glColor3f(1.0f, 1.0f, 1.0f); // White plate
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++) {
            float theta = j * 3.14159f / 180.0f;
            float x = foods[i].plateX + 18 * cos(theta);
            float y = foods[i].plateY + 10 * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
        // Draw food if visible or moving
        if (foods[i].isVisible || foods[i].isMoving) {
            drawFoodItem(i);
        }
    }
}

void drawCatWithMouthAnimation() {
    float mouthOpen = 0.0f;
    if (catIsEating && eatTimer > 0) {
        mouthOpen = 0.01f * sin((30 - eatTimer) * 0.3f); // animate open/close
    }
    drawCat(mouthOpen);
}

void drawFoodItem(int i) {
    float fx = foods[i].isMoving ? foods[i].x : foods[i].plateX;
    float fy = foods[i].isMoving ? foods[i].y : foods[i].plateY;
    float scale = 1.0f;
    if (catIsEating && animatingFood == i) {
        scale = (float)eatTimer / 30.0f; // shrink as eaten
    }
    if (i == 0) {
        // Fish
        glColor3f(0.6f, 0.8f, 1.0f); // Light blue
        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 10 * scale * cos(theta);
            float y = fy + 4 * scale * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
        // Tail
        glColor3f(0.4f, 0.6f, 0.8f);
        glBegin(GL_TRIANGLES);
        glVertex2f(fx - 10, fy);
        glVertex2f(fx - 16, fy + 4);
        glVertex2f(fx - 16, fy - 4);
        glEnd();
        // Eye
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 6 + 1 * cos(theta);
            float y = fy + 1 * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
    } else if (i == 1) {
        // Meat drumstick
        glColor3f(0.8f, 0.4f, 0.2f);
        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 7 * scale * cos(theta);
            float y = fy + 4 * scale * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
        // Bone
        glColor3f(1.0f, 1.0f, 0.9f);
        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 2 * scale * cos(theta);
            float y = fy + 8 + 2 * scale * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
    } else if (i == 2) {
        // Watermelon
        glColor3f(1.0f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        for (int k = 0; k <= 180; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 8 * scale * cos(theta);
            float y = fy + 8 * scale * sin(theta);
            glVertex2f(x, y);
        }
        glVertex2f(fx + 8, fy);
        glVertex2f(fx - 8, fy);
        glEnd();
        // Green rind
        glColor3f(0.2f, 0.8f, 0.2f);
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k <= 180; k++) {
            float theta = k * 3.14159f / 180.0f;
            float x = fx + 8 * scale * cos(theta);
            float y = fy + 8 * scale * sin(theta);
            glVertex2f(x, y);
        }
        glEnd();
        // Seeds
        glColor3f(0.1f, 0.1f, 0.1f);
        for (int s = -2; s <= 2; s++) {
            glBegin(GL_POLYGON);
            for (int k = 0; k < 360; k++) {
                float theta = k * 3.14159f / 180.0f;
                float x = fx + s * 2 + 0.5 * cos(theta);
                float y = fy + 4 + 0.5 * sin(theta);
                glVertex2f(x, y);
            }
            glEnd();
        }
    }
}

/*
====================================================
  
  Screen: Intro Screen
  
====================================================
*/  


void drawIntroScreen() {
    
    // Gradient sky (deep blue to purple)
    glClearColor(0.10f, 0.08f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.10f, 0.08f, 0.25f); // Deep blue (top)
    glVertex2f(-100.0f, 100.0f);
    glVertex2f(100.0f, 100.0f);
    glColor3f(0.22f, 0.10f, 0.32f); // Deep purple (bottom)
    glVertex2f(100.0f, -100.0f);
    glVertex2f(-100.0f, -100.0f);
    glEnd();
    // Glowing moon with halo (top right)
    float moonX = 60.0f, moonY = 65.0f, moonR = 18.0f;
    // Halo
    glColor4f(1.0f, 1.0f, 0.8f, 0.18f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(moonX, moonY);
    for (int i = 0; i <= 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        glVertex2f(moonX + cos(angle) * (moonR + 10), moonY + sin(angle) * (moonR + 10));
    }
    glEnd();
    // Moon
    glColor3f(1.0f, 1.0f, 0.85f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(moonX, moonY);
    for (int i = 0; i <= 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        glVertex2f(moonX + cos(angle) * moonR, moonY + sin(angle) * moonR);
    }
    glEnd();
    // Stars
    srand(42); // Fixed seed for consistent star placement
    for (int i = 0; i < 30; ++i) {
        float sx = -90.0f + rand() % 180;
        float sy = 10.0f + rand() % 80;
        float starR = 0.8f + (rand() % 10) * 0.12f;
        if (i % 3 == 0)
            glColor3f(1.0f, 1.0f, 0.8f); // yellowish
        else
            glColor3f(1.0f, 1.0f, 1.0f); // white
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int j = 0; j <= 8; ++j) {
            float angle = 2.0f * 3.1415926f * j / 8;
            glVertex2f(sx + cos(angle) * starR, sy + sin(angle) * starR);
        }
        glEnd();
    }
    // Hill (bottom)
    glColor3f(0.05f, 0.05f, 0.10f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; ++i) {
        float theta = 3.1415926f * i / 180.0f;
        float x = 0.0f + 120.0f * cos(theta);
        float y = -60.0f + 40.0f * sin(theta);
        glVertex2f(x, y);
    }
    glVertex2f(120.0f, -100.0f);
    glVertex2f(-120.0f, -100.0f);
    glEnd();
    // Cat silhouette (simple shape, sitting, looking up)
    float catBaseX = -20.0f, catBaseY = -35.0f;
    // Body
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        glVertex2f(catBaseX + cos(angle) * 8.0f, catBaseY + sin(angle) * 13.0f);
    }
    glEnd();
    // Head
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        glVertex2f(catBaseX + cos(angle) * 5.0f, catBaseY + 13.0f + sin(angle) * 5.0f);
    }
    glEnd();
    // Left ear
    glBegin(GL_TRIANGLES);
    glVertex2f(catBaseX - 2.5f, catBaseY + 18.0f);
    glVertex2f(catBaseX - 5.5f, catBaseY + 22.0f);
    glVertex2f(catBaseX, catBaseY + 19.0f);
    glEnd();
    // Right ear
    glBegin(GL_TRIANGLES);
    glVertex2f(catBaseX + 2.5f, catBaseY + 18.0f);
    glVertex2f(catBaseX + 5.5f, catBaseY + 22.0f);
    glVertex2f(catBaseX, catBaseY + 19.0f);
    glEnd();
    // Tail
    glBegin(GL_LINE_STRIP);
    glLineWidth(3.0f);
    for (int i = 0; i <= 20; ++i) {
        float t = i / 20.0f;
        float angle = 3.1415926f * 0.7f * t;
        float tx = catBaseX + 8.0f + 12.0f * t;
        float ty = catBaseY - 8.0f + 8.0f * sin(angle);
        glVertex2f(tx, ty);
    }
    glEnd();

    // Title: Happy Paws (centered, glowing light yellow)
    glColor3f(1.0f, 0.95f, 0.7f); // Glowing light yellow
    const char* gameTitle = "Happy Paws";
    float textWidth = 0.0f;
    for (const char* c = gameTitle; *c != '\0'; c++) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    float worldTextWidth = textWidth * 0.15f;
    float centerX = -worldTextWidth / 2.0f;
    glRasterPos2f(centerX, 90.0f);
    for (const char* c = gameTitle; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    // Cat in the center
    glPushMatrix();
    glTranslatef(0.0f, 30.0f, 0.0f);
    glScalef(0.9f, 0.9f, 1.0f);
    drawCat(0.0f);
    glPopMatrix();
    
    // Draw a soft vertical gradient background for the table
    float tableTopY = 5.0f;
    float tableBottomY = -60.0f;
    float tableLeftX = -65.0f;
    float tableRightX = 75.0f;
    glBegin(GL_QUADS);
    // Top color (soft purple)
    glColor3f(0.85f, 0.80f, 0.95f);
    glVertex2f(tableLeftX, tableTopY);
    glVertex2f(tableRightX, tableTopY);
    // Bottom color (soft pink)
    glColor3f(0.98f, 0.85f, 0.95f);
    glVertex2f(tableRightX, tableBottomY);
    glVertex2f(tableLeftX, tableBottomY);
    glEnd();
    glColor3f(0.1f, 0.2f, 0.3f); // Reset color for text
    // Table text rendering loop follows
    float tableStartY = 0.0f;
    float rowHeight = -10.0f;
    float col1X = -60.0f;
    float col2X = 10.0f;
    int numRows = 6;
    const char* table[6][2] = {
        {"Name", "ID"},
        {"Iftekhar Tasnim Md. Asif", "22-478960-2"},
        {"Md Rubai Hasan Abir", "22-48963-3"},
        {"Afrin Akter Ridika", "22-48986-3"},
        {"Md Warish Ahmmed Tusher", "22-49043-3"},
        {"Shahnewaz Ahmed", "22-48979-3"}
    };
    for (int i = 0; i < numRows; ++i) {
        glColor3f(i == 0 ? 0.2f : 0.1f, 0.2f, 0.3f);
        glRasterPos2f(col1X + 5, tableStartY + i * rowHeight - 2);
        for (const char* c = table[i][0]; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glRasterPos2f(col2X + 5, tableStartY + i * rowHeight - 2);
        for (const char* c = table[i][1]; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    // Game name at the top - centered and bigger
    glColor3f(1.0f, 0.95f, 0.7f); // Glowing light yellow
    // Playful instruction at the bottom
    glColor3f(0.7f, 0.85f, 1.0f); // Soft blue
    glRasterPos2f(-55.0f, -80.0f);
    const char* instr = "Press '2' to Play!";
    for (const char* c = instr; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    switch (currentScreen) {
        case INTRO_SCREEN:
            drawIntroScreen();
            break;
        case FEEDING_CAT_SCREEN:
            drawFeedingCatScreen();
            break;
        case DANCE_SCREEN:
            drawDanceScreen();
            break;
        case SLEEP_SCREEN:
            drawSleepScreen();
            break;
        case SHOWER_SCREEN:
            drawShowerScreen();
            break;
        case FUN_SCREEN:
            drawFunScreen();
            break;
        default:
            break;
    }
}

void drawClock() {
    // Clock outline
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.9f); // Light gray
    for(int i = 0; i < 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(-70.0f + cos(angle) * 12, 60.0f + sin(angle) * 12);
    }
    glEnd();

    // Clock border
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray
    for(int i = 0; i < 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(-70.0f + cos(angle) * 12, 60.0f + sin(angle) * 12);
    }
    glEnd();

    // Clock ticks marks
    glLineWidth(1.0f);
    for(int i = 0; i < 12; i++) {
        float angle = i * 30 * 3.14159f / 180;
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f); // Black
        glVertex2f(-70.0f + cos(angle) * 10, 60.0f + sin(angle) * 10);
        glVertex2f(-70.0f + cos(angle) * 12, 60.0f + sin(angle) * 12);
        glEnd();
    }

    // Clock center point
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex2f(-70.0f, 60.0f);
    glEnd();

    // Hour
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex2f(-70.0f, 60.0f);
    glVertex2f(-70.0f + cos(45 * 3.14159f / 180) * 7, 60.0f + sin(45 * 3.14159f / 180) * 7);
    glEnd();

    // Minute
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray
    glVertex2f(-70.0f, 60.0f);
    glVertex2f(-70.0f + cos(180 * 3.14159f / 180) * 10, 60.0f + sin(180 * 3.14159f / 180) * 10);
    glEnd();
}

void reshape(int w, int h) {
    // Force the window to stay at 800x800
    if (w != 800 || h != 800) {
        glutReshapeWindow(800, 800);
    }

    glViewport(0, 0, 800, 800); // Fixed viewport size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            currentScreen = INTRO_SCREEN;
            PlaySound(NULL, NULL, 0); // Stop background music
            break;
        case '2':
            currentScreen = FEEDING_CAT_SCREEN;
            sound(); // Start background music
            break;
        case '3':
            currentScreen = DANCE_SCREEN;
            PlaySound(NULL, NULL, 0);
            break;
        case '4':
            currentScreen = SLEEP_SCREEN;
            PlaySound(NULL, NULL, 0);
            break;
        case '5':
            currentScreen = SHOWER_SCREEN;
            PlaySound(NULL, NULL, 0);
            break;
        case '6':
            currentScreen = FUN_SCREEN;
            PlaySound(NULL, NULL, 0);
            break;
        // Add more cases for other screens (e.g., '3', '4', ...)
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && animatingFood == -1 && !catIsEating) {
        // Convert mouse x, y to world coordinates
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float wx = (float)x / winWidth * 200.0f - 100.0f;
        float wy = 100.0f - (float)y / winHeight * 200.0f;
        // Check if mouse is over any food
        for (int i = 0; i < 3; ++i) {
            if (foods[i].isVisible && !foods[i].isMoving) {
                float dx = wx - foods[i].x;
                float dy = wy - foods[i].y;
                if (dx * dx + dy * dy < 100.0f) { // Click radius (10 units)
                    animatingFood = i;
                    foods[i].isMoving = true;
                    foods[i].isVisible = false; // Hide on plate during move
                    glutTimerFunc(16, animateFoodToMouth, 0);
                    break;
                }
            }
        }
    }
}

// Add a timer for food reappearance
void foodReappear(int foodIndex) {
    foods[foodIndex].x = foods[foodIndex].plateX;
    foods[foodIndex].y = foods[foodIndex].plateY;
    foods[foodIndex].isVisible = true;
    foods[foodIndex].isMoving = false;
    glutPostRedisplay();
}

// Callback wrapper for glutTimerFunc
void foodReappearCallback(int value) {
    foodReappear(value);
}

void animateFoodToMouth(int value) {
    static int lastFedFood = -1;
    if (animatingFood != -1) {
        // Move food towards cat's mouth
        float& fx = foods[animatingFood].x;
        float& fy = foods[animatingFood].y;
        float dx = catMouthX - fx;
        float dy = catMouthY - fy;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist > 2.0f) {
            fx += dx * 0.1f;
            fy += dy * 0.1f;
            glutPostRedisplay();
            glutTimerFunc(16, animateFoodToMouth, 0);
        } else {
            // Arrived at mouth: start eating animation
            foods[animatingFood].isMoving = false;
            foods[animatingFood].isVisible = false;
            catIsEating = true;
            eatTimer = 30; // Start eating timer
            lastFedFood = animatingFood;
            animatingFood = -1;
            glutPostRedisplay();
            glutTimerFunc(16, animateFoodToMouth, 0); // Start eating animation
        }
    } else if (catIsEating) {
        if (eatTimer == 30) {
            playFeedingSound(); // Play sound once when eating starts
        }
        if (eatTimer > 0) {
            --eatTimer;
            glutPostRedisplay();
            glutTimerFunc(16, animateFoodToMouth, 0);
        } else {
            // Eating animation finished, reappear food
            catIsEating = false;
            if (lastFedFood >= 0 && lastFedFood < 3) {
                foodReappear(lastFedFood);
                lastFedFood = -1;
            }
            glutPostRedisplay();
        }
    }
}

void drawHintBubble() {
    // Position and size
    float boxX = -70.0f, boxY = -7.0f;
    float boxW = 38.0f, boxH = 14.0f;
    float tailX = boxX + boxW * 0.6f;
    float tailY = boxY;
    float tailW = 5.0f, tailH = 6.0f;

    // --- Draw filled yellow rectangle ---
    glColor3f(1.0f, 0.97f, 0.6f); // Light yellow
    glBegin(GL_QUADS);
    glVertex2f(boxX, boxY);
    glVertex2f(boxX + boxW, boxY);
    glVertex2f(boxX + boxW, boxY + boxH);
    glVertex2f(boxX, boxY + boxH);
    glEnd();

    // --- Draw tail (triangle) ---
    glBegin(GL_TRIANGLES);
    glVertex2f(tailX, tailY); // base middle
    glVertex2f(tailX + tailW/2.0f, tailY - tailH);
    glVertex2f(tailX + tailW, tailY);
    glEnd();

    // --- Draw black outline for rectangle ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boxX, boxY);
    glVertex2f(boxX + boxW, boxY);
    glVertex2f(boxX + boxW, boxY + boxH);
    glVertex2f(boxX, boxY + boxH);
    glEnd();
    // --- Draw black outline for tail ---
    glBegin(GL_LINE_LOOP);
    glVertex2f(tailX, tailY);
    glVertex2f(tailX + tailW/2.0f, tailY - tailH);
    glVertex2f(tailX + tailW, tailY);
    glEnd();

    // --- Draw hint text ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(boxX + 4.0f, boxY + boxH/2.0f + 2.5f);
    const char* hintText = "Click on the Food";
    for (const char* c = hintText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Blank screen functions
void drawDanceScreen() {
    glClearColor(0.95f, 0.95f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}
void drawSleepScreen() {
    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}
void drawShowerScreen() {
    glClearColor(0.95f, 1.0f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}
void drawFunScreen() {
    glClearColor(1.0f, 0.98f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(400, 200);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Happy Paws");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    // Do not start music here, only in feeding screen
    init();
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
