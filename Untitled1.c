#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;


const float PI = 3.14159265358979323846f;
float pathWidth = 0.25f;
struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Player {
    Vec2 pos{0.0f, 0.0f};
    float r = 0.05f;
    int health = 3;
};

struct Virus {
    Vec2 pos;
    float r = 0.05f;
    float speed = 0.002f;
    int type = 0;
};

struct Spray {
    Vec2 pos;
    Vec2 vel;
    float r = 0.01f;
    float life = 1.5f;
};

struct Vaccine {
    Vec2 pos;
    float r = 0.02f;
    float speed = 0.005f;
};

struct Flower {
    Vec2 pos;
    float size;
    Flower() : pos(0.0f, 0.0f), size(0.05f) {}
    Flower(float x, float y, float s) : pos(x, y), size(s) {}
};

// Function to generate a random number between a and b
float randRange(float a, float b) {
    return a + (b - a) * (rand() / (float)RAND_MAX);
}


struct Bush {
    Vec2 pos;
    float size;
    Bush() : pos(0.0f, 0.0f), size(0.10f) {}
    Bush(float x, float y, float s) : pos(x, y), size(s) {}
};

struct Raindrop {
    float x, y;   // Position of raindrop
    float speed;  // Speed of falling
};
// Define a fixed-size array for raindrops
Raindrop raindrops[100];  // Max 100 raindrops
int numRaindrops = 0;  // Count of active raindrops

struct Snowflake
{
    float x, y;  // Position
    float speed;
      // Speed of falling
};
  // You can change this value as needed
Snowflake snowflakes[100];
int numSnowflakes = 0;

enum GameState {
    PLAYING,
    GAME_OVER,
    HIGH_SCORES,
    LEVEL_COMPLETE
};
enum Weather { SUNNY, WINTER, RAIN, NIGHT };
Weather currentWeather = SUNNY;



void drawText(float x, float y, const char* text, void* font=GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void spawnRaindrops(int count) {
    numRaindrops = count;
    for (int i = 0; i < numRaindrops; i++) {
        raindrops[i].x = rand() % 160 - 80;  // Random x position
        raindrops[i].y = rand() % 90 + 90;  // Start from top of the screen
        raindrops[i].speed = static_cast<float>((rand() % 10 + 12) * 0.1);  // Random speed
    }
}



void updateRaindrops() {
    for (int i = 0; i < numRaindrops; i++) {
        raindrops[i].y -= raindrops[i].speed;
        if (raindrops[i].y < -1.0f) {
            raindrops[i].y = rand() % 90 + 90;  // Reset raindrop position
            raindrops[i].x = rand() % 160 - 80;
        }
    }
}

void drawRaindrops() {
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue for raindrops
    glLineWidth(1.5);

    glBegin(GL_LINES);
    for (int i = 0; i < numRaindrops; i++) {
        glVertex2f(raindrops[i].x / 80.0f, raindrops[i].y / 90.0f);  // Convert to -1 to 1 range
        glVertex2f(raindrops[i].x / 80.0f, (raindrops[i].y - 2.0f) / 90.0f);
    }
    glEnd();
}



void spawnSnowflakes(int count) {
    numSnowflakes = count;
    for (int i = 0; i < numSnowflakes; i++) {
        snowflakes[i].x = rand() % 160 - 80;  // Random x position (centered)
        snowflakes[i].y = rand() % 90 + 90;   // Start from top of the screen
        snowflakes[i].speed = static_cast<float>((rand() % 10 + 12) * 0.1);  // Random speed

    }
}

void updateSnowflakes() {
    for (int i = 0; i < numSnowflakes; i++) {
        // Snowflakes fall down
        snowflakes[i].y -= snowflakes[i].speed;



        // If snowflake goes below the screen, reset its position
        if (snowflakes[i].y < -1.0f) {
            snowflakes[i].y = rand() % 90 + 90;  // Reset Y position (above the screen)
            snowflakes[i].x = rand() % 160 - 80; // Reset X position (random horizontal position)
        }
    }
}


void drawSnowflakes() {
    glColor3f(1.0f, 1.0f, 1.0f);  // White for snowflakes
    glPointSize(5.0f);  // Make the snowflakes larger (like a dot)

    glBegin(GL_POINTS);  // Draw points (snowflakes)
    for (int i = 0; i < numSnowflakes; i++) {
        glVertex2f(snowflakes[i].x / 80.0f, snowflakes[i].y / 90.0f);  // Convert to -1 to 1 range
    }
    glEnd();
}




// Function to draw snowflakes







void drawWeather() {
    // Based on level, show the corresponding weather
    if (currentWeather == SUNNY) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for sunny weather
        drawText(-0.8f, 0.8f, "Weather: Sunny");
    }
    else if (currentWeather == WINTER) {
        glClearColor(0.8f, 0.8f, 1.0f, 1.0f);// Gray for overcast weather
        drawText(-0.8f, 0.8f, "Weather: Winter");
        drawSnowflakes();
    }
    else if (currentWeather == RAIN) {
        glColor3f(0.0f, 0.0f, 1.0f); // Blue for rain
        drawText(-0.8f, 0.8f, "Weather: Rain");
        drawRaindrops(); // Draw raindrops when it's raining
    }
    else if (currentWeather == NIGHT) {
        glColor3f(0.1f, 0.1f, 0.1f); // Dark for night
        drawText(-0.8f, 0.8f, "Weather: Night");
    }
}










Player player;
Virus viruses[50];
Spray sprays[100];
Vaccine vaccines[50];
Flower flowers[10];
Bush bushes[10];

int score = 0;
int level = 1;
int virusesKilled = 0;
int virusHits = 0;
bool keyLeft = false;
bool keyRight = false;
bool keyUp = false;
bool keyDown = false;
bool sprayActive = false;
GameState gameState = PLAYING;

const int MAX_HIGH_SCORES = 5;
int highScores[MAX_HIGH_SCORES] = {0, 0, 0, 0, 0};
bool scoreAdded = false;

const float DT = 0.016f;



float rand01() {
    return rand() / (float)RAND_MAX;
}


float calculateDistance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
float clampf(float v, float a, float b) {
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

void drawCircle(float cx, float cy, float r, int segments = 16) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}


bool checkCollision(Vec2 pos1, float r1, Vec2 pos2, float r2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distanceSquared = dx * dx + dy * dy;
    float minDistance = r1 + r2;
    return distanceSquared <= minDistance * minDistance;
}

// ----- park elements -----
/*void drawFlower(float x, float y, float size) {
    const int petals = 5;
    float petRadius = size * 0.45f;
    float petSize = size * 0.35f;

    // Petals
    glColor3f(1.0f, 0.2f, 0.6f);
    for (int i = 0; i < petals; i++) {
        float a = (2.0f * PI * i) / petals;
        drawCircle(x + cos(a) * petRadius, y + sin(a) * petRadius, petSize, 20);
    }

    // Center
    glColor3f(1.0f, 0.85f, 0.1f);
    drawCircle(x, y, size * 0.28f, 20);
}
*/
void drawPetal() {
    // Circle 1
    float radius2 = calculateDistance(0.338617705895f, -0.5821255268663f,
                                      0.3523274169595f, -0.5816574629887f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.338617705895f, -0.5821255268663f, radius2, 30);

    // Circle 2
    float radius3 = calculateDistance(0.3651201325412f, -0.5867578594816f,
                                      0.3788298436057f, -0.586289795604f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.3651201325412f, -0.5867578594816f, radius3, 30);

    // Circle 3
    float radius4 = calculateDistance(0.3286726692214f, -0.5560371436218f,
                                      0.3423823802859f, -0.5555690797442f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.3286726692214f, -0.5560371436218f, radius4, 30);

    // Circle 4
    float radius5 = calculateDistance(0.3460604498878f, -0.5353340524989f,
                                      0.3597701609523f, -0.5348659886213f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.3460604498878f, -0.5353340524989f, radius5, 30);

    // Circle 5
    float radius6 = calculateDistance(0.3724765012847f, -0.5400089440428f,
                                      0.3861862123492f, -0.5395408801652f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.3724765012847f, -0.5400089440428f, radius6, 30);

    // Circle 6
    float radius7 = calculateDistance(0.3826751034063f, -0.5657208475341f,
                                      0.3963848144708f, -0.5652527836565f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.3826751034063f, -0.5657208475341f, radius7, 30);

    // Center circle (yellow)
    float radius8 = calculateDistance(0.3554231010158f, -0.5610459559903f,
                                      0.3691328120803f, -0.5605778921127f);
    glColor3f(1.0f, 0.85f, 0.1f);
    drawCircle(0.3554231010158f, -0.5610459559903f, radius8, 30);

    float radius_missing = calculateDistance(0.630492358258f, -0.7693384261897f,
                                            0.6442020693225f, -0.7688703623121f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.630492358258f, -0.7693384261897f, radius_missing, 30);

    float radius10 = calculateDistance(0.6569947849042f, -0.773970758805f,
                                      0.6707044959687f, -0.7735026949274f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.6569947849042f, -0.773970758805f, radius10, 30);

    float radius11 = calculateDistance(0.6379351022508f, -0.7225469518223f,
                                      0.6516448133153f, -0.7220788879447f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.6379351022508f, -0.7225469518223f, radius11, 30);

    float radius12 = calculateDistance(0.6643511536477f, -0.7272218433662f,
                                      0.6780608647122f, -0.7267537794886f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.6643511536477f, -0.7272218433662f, radius12, 30);

    float radius13 = calculateDistance(0.6745497557693f, -0.7529337468575f,
                                      0.6882594668338f, -0.7524656829799f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.6745497557693f, -0.7529337468575f, radius13, 30);

    float radius14 = calculateDistance(0.6205473215844f, -0.7432500429452f,
                                      0.6342570326489f, -0.7427819790676f);
    glColor3f(1.0f, 0.2f, 0.6f);
    drawCircle(0.6205473215844f, -0.7432500429452f, radius14, 30);

    float radius15 = calculateDistance(0.6481226761523f, -0.7486707498129f,
                                      0.6618323872168f, -0.7482026859353f);
    glColor3f(1.0f, 0.85f, 0.1f);
    drawCircle(0.6481226761523f, -0.7486707498129f, radius15, 30);

    float radius16 = calculateDistance(0.3554231010158f, -0.5610459559903f,
                                      0.3691328120803f, -0.5605778921127f);
    glColor3f(1.0f, 0.85f, 0.1f);
    drawCircle(0.3554231010158f, -0.5610459559903f, radius16, 30);


}
void drawFlower() {

    glPushMatrix();
    drawPetal();
    glPopMatrix();
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    drawPetal();
    glPopMatrix();


    glPushMatrix();
    glScalef(-1.0f, 1.0f, 1.0f);
    drawPetal();
    glPopMatrix();

    glPushMatrix();
    glScalef(-1.0f, -1.0f, 1.0f);
    drawPetal();
    glPopMatrix();

}

void drawBush() {


  float radius1 = calculateDistance(0.7594437632372f, 0.5396866331079f,
                                  0.9286715083089f, 0.5560515308613f);
glColor3f(0.05f, 0.45f, 0.05f);
drawCircle(0.7594437632372f, 0.5396866331079f, radius1, 30);

float radius2 = calculateDistance(0.7882359096862f, 0.5444787288553f,
                                  0.9287682185758f, 0.5529185366733f);
glColor3f(0.08f, 0.55f, 0.08f);
drawCircle(0.7882359096862f, 0.5444787288553f, radius2, 30);

float radius3 = calculateDistance(0.811629528676f, 0.538488609171f,
                                  0.9288959148598f, 0.5492629410359f);
glColor3f(0.12f, 0.68f, 0.12f);
drawCircle(0.811629528676f, 0.538488609171f, radius3, 30);

   float radius4 = calculateDistance(0.3472411253204f, 0.6309925910361f,
                                  0.5164688703921f, 0.6473574887895f);
glColor3f(0.05f, 0.45f, 0.05f);
drawCircle(0.3472411253204f, 0.6309925910361f, radius4, 30);

float radius5 = calculateDistance(0.3760332717694f, 0.6357846867835f,
                                  0.516565580659f, 0.6442244946015f);
glColor3f(0.08f, 0.55f, 0.08f);
drawCircle(0.3760332717694f, 0.6357846867835f, radius5, 30);

float radius6 = calculateDistance(0.3994268907592f, 0.6297945670992f,
                                  0.516693276943f, 0.6405688989641f);
glColor3f(0.12f, 0.68f, 0.12f);
drawCircle(0.3994268907592f, 0.6297945670992f, radius6, 30);

// Continuing with the new color pattern
float radius10 = calculateDistance(-0.3333603437943f, -0.6221650567835f,
                                   -0.1641325987226f, -0.60580015903f);
glColor3f(0.05f, 0.45f, 0.05f);   // First color in new set
drawCircle(-0.3333603437943f, -0.6221650567835f, radius10, 30);

float radius11 = calculateDistance(-0.3045681973453f, -0.6173729610361f,
                                   -0.1640358884556f, -0.6089331532181f);
glColor3f(0.08f, 0.55f, 0.08f);   // Second color in new set
drawCircle(-0.3045681973453f, -0.6173729610361f, radius11, 30);

float radius12 = calculateDistance(-0.2874738915871f, -0.6233630807203f,
                                   -0.1641555834073f, -0.6169239262008f);
glColor3f(0.12f, 0.68f, 0.12f);   // Third color in new set
drawCircle(-0.2874738915871f, -0.6233630807203f, radius12, 30);
float radius7 = calculateDistance(-0.7637762397985f, 0.4900480651081f,
                                  -0.5945484947268f, 0.5064129628615f);
glColor3f(0.05f, 0.45f, 0.05f);
drawCircle(-0.7637762397985f, 0.4900480651081f, radius7, 30);

float radius9 = calculateDistance(-0.7349840933495f, 0.4948401608555f,
                                  -0.5944517844598f, 0.5032799686735f);
glColor3f(0.08f, 0.55f, 0.08f);
drawCircle(-0.7349840933495f, 0.4948401608555f, radius9, 30);

float radius13 = calculateDistance(-0.7115904743597f, 0.4888500411712f,
                                   -0.5943240881758f, 0.4996243730361f);
glColor3f(0.12f, 0.68f, 0.12f);
drawCircle(-0.7115904743597f, 0.4888500411712f, radius13, 30);
float radius14 = calculateDistance(0.7522584779297f, -0.4325814004564f,
                                   0.9214862230014f, -0.416216502703f);
glColor3f(0.05f, 0.45f, 0.05f);
drawCircle(0.7522584779297f, -0.4325814004564f, radius14, 30);

float radius15 = calculateDistance(0.7810506243787f, -0.427789304709f,
                                   0.9215829332683f, -0.419349496891f);
glColor3f(0.08f, 0.55f, 0.08f);
drawCircle(0.7810506243787f, -0.427789304709f, radius15, 30);

float radius16 = calculateDistance(0.8044442433685f, -0.4337794243933f,
                                   0.9217106295523f, -0.4230050925284f);
glColor3f(0.12f, 0.68f, 0.12f);
drawCircle(0.8044442433685f, -0.4337794243933f, radius16, 30);


}
void drawParkBackground() {
    // Grass background
    glBegin(GL_QUADS);
    glColor3f(0.08f, 0.45f, 0.08f);
    glVertex2f(-1.0f, -1.0f);
    glColor3f(0.20f, 0.72f, 0.20f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(0.08f, 0.45f, 0.08f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
 // Vertical path
    /*glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f, 1.0f);
    glVertex2f(0.13f, 1.0f);
    glVertex2f(0.13f, -1.0f);
    glVertex2f(-0.12f, -1.0f);
    glEnd();

    // Horizontal path
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.14f);
    glVertex2f(1.0f, 0.14f);
    glVertex2f(1.0f, -0.12f);
    glVertex2f(-1.0f, -0.12f);
    glEnd();*/
    // Vertical path - use pathWidth variable
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(-pathWidth/2, 1.0f);
    glVertex2f(pathWidth/2, 1.0f);
    glVertex2f(pathWidth/2, -1.0f);
    glVertex2f(-pathWidth/2, -1.0f);
    glEnd();

    // Horizontal path - use pathWidth variable
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, pathWidth/2);
    glVertex2f(1.0f, pathWidth/2);
    glVertex2f(1.0f, -pathWidth/2);
    glVertex2f(-1.0f, -pathWidth/2);
    glEnd();


}
void initParkObjects() {
    //flowers.clear();
   // bushes.clear();

   // flowers.push_back(Flower(-0.65f, 0.75f, 0.06f));
    //flowers.push_back(Flower(0.65f, 0.72f, 0.05f));
    //flowers.push_back(Flower(-0.6f, -0.75f, 0.055f));
    //flowers.push_back(Flower(0.55f, -0.78f, 0.05f));

    //bushes.push_back(Bush(-0.85f, 0.45f, 0.12f));
    //bushes.push_back(Bush(0.80f, 0.52f, 0.11f));
    //bushes.push_back(Bush(-0.25f, -0.65f, 0.14f));
   // bushes.push_back(Bush(0.30f, 0.60f, 0.10f));
}
void drawCartoonHuman(Vec2 pos, float scale = 0.1f) {
    // --- Head ---
    glColor3f(0.95f, 0.85f, 0.7f); // skin color
    drawCircle(pos.x, pos.y + scale * 0.6f, scale * 0.4f, 40);

    // --- Eyes ---
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(pos.x - scale * 0.15f, pos.y + scale * 0.65f, scale * 0.07f, 20);
    drawCircle(pos.x + scale * 0.15f, pos.y + scale * 0.65f, scale * 0.07f, 20);

    // Add white highlights to eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(pos.x - scale * 0.13f, pos.y + scale * 0.67f, scale * 0.02f, 10);
    drawCircle(pos.x + scale * 0.17f, pos.y + scale * 0.67f, scale * 0.02f, 10);

    // --- Smile ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(pos.x - scale * 0.2f, pos.y + scale * 0.55f);
    glVertex2f(pos.x - scale * 0.1f, pos.y + scale * 0.5f);
    glVertex2f(pos.x, pos.y + scale * 0.48f);
    glVertex2f(pos.x + scale * 0.1f, pos.y + scale * 0.5f);
    glVertex2f(pos.x + scale * 0.2f, pos.y + scale * 0.55f);
    glEnd();

    // --- Body (shirt) ---
    glColor3f(0.2f, 0.4f, 0.9f); // blue shirt
    glBegin(GL_QUADS);
    glVertex2f(pos.x - scale * 0.25f, pos.y + scale * 0.3f);
    glVertex2f(pos.x + scale * 0.25f, pos.y + scale * 0.3f);
    glVertex2f(pos.x + scale * 0.25f, pos.y - scale * 0.2f);
    glVertex2f(pos.x - scale * 0.25f, pos.y - scale * 0.2f);
    glEnd();

    // --- Awesome Patterned Tie ---
    // Main tie body
    glColor3f(0.8f, 0.0f, 0.0f); // red base
    glBegin(GL_TRIANGLES);
    glVertex2f(pos.x, pos.y + scale * 0.3f); // top
    glVertex2f(pos.x - scale * 0.08f, pos.y - scale * 0.1f);
    glVertex2f(pos.x + scale * 0.08f, pos.y - scale * 0.1f);
    glEnd();

    // Tie pattern - polka dots
    glColor3f(1.0f, 1.0f, 0.0f); // yellow dots
    drawCircle(pos.x, pos.y + scale * 0.15f, scale * 0.02f, 10);
    drawCircle(pos.x - scale * 0.03f, pos.y + scale * 0.05f, scale * 0.02f, 10);
    drawCircle(pos.x + scale * 0.03f, pos.y - scale * 0.02f, scale * 0.02f, 10);

    // Tie bottom
    glColor3f(0.8f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);
    glVertex2f(pos.x - scale * 0.05f, pos.y - scale * 0.1f);
    glVertex2f(pos.x + scale * 0.05f, pos.y - scale * 0.1f);
    glVertex2f(pos.x + scale * 0.03f, pos.y - scale * 0.25f);
    glVertex2f(pos.x - scale * 0.03f, pos.y - scale * 0.25f);
    glEnd();

    // --- Arms ---
    glColor3f(0.95f, 0.85f, 0.7f);
    glBegin(GL_QUADS);
    // Left arm
    glVertex2f(pos.x - scale * 0.25f, pos.y + scale * 0.2f);
    glVertex2f(pos.x - scale * 0.45f, pos.y + scale * 0.2f);
    glVertex2f(pos.x - scale * 0.45f, pos.y - scale * 0.1f);
    glVertex2f(pos.x - scale * 0.25f, pos.y - scale * 0.1f);
    // Right arm
    glVertex2f(pos.x + scale * 0.25f, pos.y + scale * 0.2f);
    glVertex2f(pos.x + scale * 0.45f, pos.y + scale * 0.2f);
    glVertex2f(pos.x + scale * 0.45f, pos.y - scale * 0.1f);
    glVertex2f(pos.x + scale * 0.25f, pos.y - scale * 0.1f);
    glEnd();

    // --- Stylish Pants ---
    // Main pants
    glColor3f(0.1f, 0.1f, 0.4f); // dark blue pants
    glBegin(GL_QUADS);
    glVertex2f(pos.x - scale * 0.25f, pos.y - scale * 0.2f);
    glVertex2f(pos.x + scale * 0.25f, pos.y - scale * 0.2f);
    glVertex2f(pos.x + scale * 0.25f, pos.y - scale * 0.4f);
    glVertex2f(pos.x - scale * 0.25f, pos.y - scale * 0.4f);
    glEnd();

    // Pants crease
    glColor3f(0.05f, 0.05f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(pos.x - scale * 0.1f, pos.y - scale * 0.2f);
    glVertex2f(pos.x - scale * 0.1f, pos.y - scale * 0.4f);
    glVertex2f(pos.x + scale * 0.1f, pos.y - scale * 0.2f);
    glVertex2f(pos.x + scale * 0.1f, pos.y - scale * 0.4f);
    glEnd();

    // --- Legs ---
    glColor3f(0.1f, 0.1f, 0.4f); // same as pants
    glBegin(GL_QUADS);
    // Left leg
    glVertex2f(pos.x - scale * 0.15f, pos.y - scale * 0.4f);
    glVertex2f(pos.x - scale * 0.05f, pos.y - scale * 0.4f);
    glVertex2f(pos.x - scale * 0.05f, pos.y - scale * 0.6f);
    glVertex2f(pos.x - scale * 0.15f, pos.y - scale * 0.6f);
    // Right leg
    glVertex2f(pos.x + scale * 0.05f, pos.y - scale * 0.4f);
    glVertex2f(pos.x + scale * 0.15f, pos.y - scale * 0.4f);
    glVertex2f(pos.x + scale * 0.15f, pos.y - scale * 0.6f);
    glVertex2f(pos.x + scale * 0.05f, pos.y - scale * 0.6f);
    glEnd();

    // --- Nice Shoes ---
    // Left shoe
    glColor3f(0.2f, 0.2f, 0.2f); // dark gray
    glBegin(GL_POLYGON);
    glVertex2f(pos.x - scale * 0.17f, pos.y - scale * 0.6f);
    glVertex2f(pos.x - scale * 0.03f, pos.y - scale * 0.6f);
    glVertex2f(pos.x - scale * 0.03f, pos.y - scale * 0.65f);
    glVertex2f(pos.x - scale * 0.17f, pos.y - scale * 0.65f);
    glEnd();

    // Right shoe
    glBegin(GL_POLYGON);
    glVertex2f(pos.x + scale * 0.03f, pos.y - scale * 0.6f);
    glVertex2f(pos.x + scale * 0.17f, pos.y - scale * 0.6f);
    glVertex2f(pos.x + scale * 0.17f, pos.y - scale * 0.65f);
    glVertex2f(pos.x + scale * 0.03f, pos.y - scale * 0.65f);
    glEnd();

    // Shoe details
    glColor3f(0.4f, 0.4f, 0.4f); // lighter gray
    glBegin(GL_LINES);
    // Left shoe detail
    glVertex2f(pos.x - scale * 0.15f, pos.y - scale * 0.6f);
    glVertex2f(pos.x - scale * 0.15f, pos.y - scale * 0.65f);
    // Right shoe detail
    glVertex2f(pos.x + scale * 0.15f, pos.y - scale * 0.6f);
    glVertex2f(pos.x + scale * 0.15f, pos.y - scale * 0.65f);
    glEnd();

    // --- Cap ---
    glColor3f(0.9f, 0.1f, 0.1f); // red cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(pos.x, pos.y + scale * 0.9f); // top
    glVertex2f(pos.x - scale * 0.45f, pos.y + scale * 0.6f);
    glVertex2f(pos.x - scale * 0.4f, pos.y + scale * 0.6f);
    glVertex2f(pos.x - scale * 0.35f, pos.y + scale * 0.62f);
    glVertex2f(pos.x - scale * 0.3f, pos.y + scale * 0.65f);
    glVertex2f(pos.x - scale * 0.2f, pos.y + scale * 0.7f);
    glVertex2f(pos.x, pos.y + scale * 0.75f);
    glVertex2f(pos.x + scale * 0.2f, pos.y + scale * 0.7f);
    glVertex2f(pos.x + scale * 0.3f, pos.y + scale * 0.65f);
    glVertex2f(pos.x + scale * 0.35f, pos.y + scale * 0.62f);
    glVertex2f(pos.x + scale * 0.4f, pos.y + scale * 0.6f);
    glVertex2f(pos.x + scale * 0.45f, pos.y + scale * 0.6f);
    glEnd();

    // --- Spray bottle ---
    glColor3f(0.2f, 0.6f, 1.0f);
    drawCircle(pos.x - 0.08f, pos.y, 0.015f, 16);
}


void drawVirus(const Virus& virus) {

    glColor3f(0.7f, 0.1f, 0.1f);
    drawCircle(virus.pos.x, virus.pos.y, virus.r, 20);


    glColor3f(1.0f, 0.5f, 0.5f);


    glBegin(GL_LINES);

    glVertex2f(virus.pos.x + 0.8f * virus.r, virus.pos.y);
    glVertex2f(virus.pos.x + 1.4f * virus.r, virus.pos.y);

    glVertex2f(virus.pos.x + 0.693f * virus.r, virus.pos.y + 0.4f * virus.r);
    glVertex2f(virus.pos.x + 1.212f * virus.r, virus.pos.y + 0.7f * virus.r);

    glVertex2f(virus.pos.x + 0.4f * virus.r, virus.pos.y + 0.693f * virus.r);
    glVertex2f(virus.pos.x + 0.7f * virus.r, virus.pos.y + 1.212f * virus.r);

    glVertex2f(virus.pos.x, virus.pos.y + 0.8f * virus.r);
    glVertex2f(virus.pos.x, virus.pos.y + 1.4f * virus.r);

    glVertex2f(virus.pos.x - 0.4f * virus.r, virus.pos.y + 0.693f * virus.r);
    glVertex2f(virus.pos.x - 0.7f * virus.r, virus.pos.y + 1.212f * virus.r);

    glVertex2f(virus.pos.x - 0.693f * virus.r, virus.pos.y + 0.4f * virus.r);
    glVertex2f(virus.pos.x - 1.212f * virus.r, virus.pos.y + 0.7f * virus.r);

    glVertex2f(virus.pos.x - 0.8f * virus.r, virus.pos.y);
    glVertex2f(virus.pos.x - 1.4f * virus.r, virus.pos.y);

    glVertex2f(virus.pos.x - 0.693f * virus.r, virus.pos.y - 0.4f * virus.r);
    glVertex2f(virus.pos.x - 1.212f * virus.r, virus.pos.y - 0.7f * virus.r);

    glVertex2f(virus.pos.x - 0.4f * virus.r, virus.pos.y - 0.693f * virus.r);
    glVertex2f(virus.pos.x - 0.7f * virus.r, virus.pos.y - 1.212f * virus.r);

    glVertex2f(virus.pos.x, virus.pos.y - 0.8f * virus.r);
    glVertex2f(virus.pos.x, virus.pos.y - 1.4f * virus.r);

    glVertex2f(virus.pos.x + 0.4f * virus.r, virus.pos.y - 0.693f * virus.r);
    glVertex2f(virus.pos.x + 0.7f * virus.r, virus.pos.y - 1.212f * virus.r);

    glVertex2f(virus.pos.x + 0.693f * virus.r, virus.pos.y - 0.4f * virus.r);
    glVertex2f(virus.pos.x + 1.212f * virus.r, virus.pos.y - 0.7f * virus.r);
    glEnd();


    drawCircle(virus.pos.x + 1.4f * virus.r, virus.pos.y, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x + 1.212f * virus.r, virus.pos.y + 0.7f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x + 0.7f * virus.r, virus.pos.y + 1.212f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x, virus.pos.y + 1.4f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x - 0.7f * virus.r, virus.pos.y + 1.212f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x - 1.212f * virus.r, virus.pos.y + 0.7f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x - 1.4f * virus.r, virus.pos.y, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x - 1.212f * virus.r, virus.pos.y - 0.7f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x - 0.7f * virus.r, virus.pos.y - 1.212f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x, virus.pos.y - 1.4f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x + 0.7f * virus.r, virus.pos.y - 1.212f * virus.r, virus.r * 0.15f, 10);
    drawCircle(virus.pos.x + 1.212f * virus.r, virus.pos.y - 0.7f * virus.r, virus.r * 0.15f, 10);
}


void drawSpray() {
    glColor3f(0.0f, 0.5f, 1.0f);
    for (int i = 0; i < 100 && sprays[i].life > 0; i++) {
        drawCircle(sprays[i].pos.x, sprays[i].pos.y, sprays[i].r, 12);
    }
}

void drawVaccine(const Vaccine& vaccine) {

    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(vaccine.pos.x - vaccine.r, vaccine.pos.y - vaccine.r * 0.5f);
    glVertex2f(vaccine.pos.x + vaccine.r, vaccine.pos.y - vaccine.r * 0.5f);
    glVertex2f(vaccine.pos.x + vaccine.r, vaccine.pos.y + vaccine.r * 0.5f);
    glVertex2f(vaccine.pos.x - vaccine.r, vaccine.pos.y + vaccine.r * 0.5f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex2f(vaccine.pos.x + vaccine.r, vaccine.pos.y - vaccine.r * 0.3f);
    glVertex2f(vaccine.pos.x + vaccine.r * 1.5f, vaccine.pos.y);
    glVertex2f(vaccine.pos.x + vaccine.r, vaccine.pos.y + vaccine.r * 0.3f);
    glEnd();


    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(vaccine.pos.x - vaccine.r * 1.2f, vaccine.pos.y - vaccine.r * 0.3f);
    glVertex2f(vaccine.pos.x - vaccine.r * 0.8f, vaccine.pos.y - vaccine.r * 0.3f);
    glVertex2f(vaccine.pos.x - vaccine.r * 0.8f, vaccine.pos.y + vaccine.r * 0.3f);
    glVertex2f(vaccine.pos.x - vaccine.r * 1.2f, vaccine.pos.y + vaccine.r * 0.3f);
    glEnd();
}


void spawnVirus() {
    int virusesToSpawn = level;

    for (int i = 0; i < virusesToSpawn; i++) {
        Virus virus;
        int side = rand() % 4;

        if (side == 0) {
            virus.pos.x = -1.0f;
            virus.pos.y = randRange(-0.8f, 0.8f);
        } else if (side == 1) {
            virus.pos.x = 1.0f;
            virus.pos.y = randRange(-0.8f, 0.8f);
        } else if (side == 2) {
            virus.pos.x = randRange(-0.8f, 0.8f);
            virus.pos.y = 1.0f;
        } else {
            virus.pos.x = randRange(-0.8f, 0.8f);
            virus.pos.y = -1.0f;
        }


        virus.type = rand() % 3;


        virus.speed = randRange(0.001f, 0.003f);

        viruses[i] = virus;
    }
}

void spawnSprayBurst() {
    const float nx = player.pos.x - 0.08f;
    const float ny = player.pos.y;

    for (int i = 0; i < 8; i++) {
        Spray s;
        s.pos = { nx + randRange(-0.005f, 0.005f), ny + randRange(-0.005f, 0.005f) };

        float angle = PI + randRange(-PI/8.0f, PI/8.0f);
        float speed = randRange(0.015f, 0.03f);
        s.vel = { cosf(angle) * speed, sinf(angle) * speed };

        s.r = 0.01f;
        s.life = randRange(0.5f, 1.2f);
        sprays[i] = s;
    }
}

void spawnVaccine() {
    if (rand() % 100 < 2) {
        for (int i = 0; i < 50; i++) {
            if (vaccines[i].speed == 0) {
                Vaccine v;

                int corner = rand() % 4;

                if (corner == 0) {
                    v.pos = { randRange(-0.9f, -0.2f), randRange(0.2f, 0.9f) };
                }
                else if (corner == 1) {
                    v.pos = { randRange(0.2f, 0.9f), randRange(0.2f, 0.9f) };
                }
                else if (corner == 2) {
                    v.pos = { randRange(-0.9f, -0.2f), randRange(-0.9f, -0.2f) };
                }
                else {
                    v.pos = { randRange(0.2f, 0.9f), randRange(-0.9f, -0.2f) };
                }

                v.speed = 0.005f;
                vaccines[i] = v;
                break;
            }
        }
    }
}
void updateHighScores(int newScore) {
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (newScore > highScores[i]) {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
                highScores[j] = highScores[j - 1];
            }
            highScores[i] = newScore;
            scoreAdded = true;
            break;
        }
    }
}

// ----- movement system -----
void specialKeys(int key, int x, int y) {
    float step = 0.05f;
    float halfW = pathWidth * 0.5f;

    float nextX = player.pos.x;
    float nextY = player.pos.y;

    switch (key) {
        case GLUT_KEY_UP:    nextY += step; break;
        case GLUT_KEY_DOWN:  nextY -= step; break;
        case GLUT_KEY_LEFT:  nextX -= step; break;
        case GLUT_KEY_RIGHT: nextX += step; break;
    }


    bool insideVertical = fabs(nextX) <= halfW && nextY >= -1.0f && nextY <= 1.0f;
    bool insideHorizontal = fabs(nextY) <= halfW && nextX >= -1.0f && nextX <= 1.0f;

    if (insideVertical || insideHorizontal) {
        player.pos.x = nextX;
        player.pos.y = nextY;
    }

    glutPostRedisplay();
}


void renderHUD() {
    drawWeather();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.95f, 0.95f, "Arrow keys to move, SPACE to spray");

    char buf[64];
    snprintf(buf, sizeof(buf), "Health: %d", player.health);
    drawText(-0.95f, 0.90f, buf);

    snprintf(buf, sizeof(buf), "Score: %d", score);
    drawText(-0.95f, 0.85f, buf);

    snprintf(buf, sizeof(buf), "Level: %d", level);
    drawText(-0.95f, 0.80f, buf);

    int target = level * 5;
    snprintf(buf, sizeof(buf), "Viruses: %d/%d", virusesKilled, target);
    drawText(-0.95f, 0.75f, buf);
}

void renderGameOver() {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawText(-0.2f, 0.2f, "GAME OVER");

    glColor3f(1.0f, 1.0f, 1.0f);
    char buf[64];
    snprintf(buf, sizeof(buf), "Final Score: %d", score);
    drawText(-0.2f, 0.1f, buf);

    drawText(-0.4f, -0.1f, "Press ENTER to view High Scores");
    drawText(-0.4f, -0.2f, "Press ESC to quit");
}

void renderHighScores() {
    glColor3f(0.0f, 1.0f, 0.0f);
    drawText(-0.3f, 0.8f, "HIGH SCORES");

    glColor3f(1.0f, 1.0f, 1.0f);
    char buf[64];
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        snprintf(buf, sizeof(buf), "%d. %d", i + 1, highScores[i]);
        drawText(-0.4f, 0.6f - (i * 0.1f), buf);
    }

    snprintf(buf, sizeof(buf), "Your Score: %d", score);
    drawText(-0.4f, 0.0f, buf);

    if (scoreAdded) {
        drawText(-0.4f, -0.1f, "New High Score!");
    }

    drawText(-0.4f, -0.2f, "Press R to Restart");
    drawText(-0.4f, -0.3f, "Press ESC to quit");
}

void renderLevelComplete() {
    glColor3f(0.0f, 1.0f, 0.0f);
    drawText(-0.3f, 0.2f, "LEVEL COMPLETE!");

    glColor3f(1.0f, 1.0f, 1.0f);
    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %d", score);
    drawText(-0.2f, 0.1f, buf);

    drawText(-0.4f, -0.1f, "Press SPACE to continue");
}



void updateGame() {
    if (gameState != PLAYING) return;

    if (level == 2) {
        pathWidth = 0.20f;
    } else if (level == 3) {
        pathWidth = 0.15f;
    } else if (level == 4) {
        pathWidth = 0.10f;
    } else if (level >= 5) {
        pathWidth = 0.08f;
    }



    // Update level and weather based on level
    if (level == 1) {
        currentWeather = SUNNY;

    } else if (level == 2) {
        currentWeather = RAIN;
    } else if (level == 3) {
        currentWeather = WINTER;
        spawnRaindrops(100);  // Trigger raindrops for rain
    } else if (level == 4) {
        currentWeather = NIGHT;
    } else if (level > 4) {
        currentWeather = SUNNY;  // Loop back to sunny or you can define more conditions
    }

         if (currentWeather == WINTER) {
         updateSnowflakes();
    }

    // Weather-specific updates (e.g., rain)
    if (currentWeather == RAIN) {
        updateRaindrops();
    }


   for (int i = 0; i < 100; i++) {
        if (sprays[i].life <= 0.0f) continue;

        sprays[i].pos.x += sprays[i].vel.x;
        sprays[i].pos.y += sprays[i].vel.y;
        sprays[i].life -= DT;

        if (sprays[i].life <= 0.0f ||
            sprays[i].pos.x < -1.1f || sprays[i].pos.x > 1.1f ||
            sprays[i].pos.y < -1.1f || sprays[i].pos.y > 1.1f) {
            sprays[i] = sprays[99];
            sprays[99] = Spray();
        }
    }


    if (sprayActive) {
        spawnSprayBurst();
        score++;
    }


for (int i = 0; i < 50; i++) {
    if (viruses[i].type == 0) continue;

    float dx = player.pos.x - viruses[i].pos.x;
    float dy = player.pos.y - viruses[i].pos.y;
    float dist = sqrt(dx*dx + dy*dy);

    if (dist > 0.01f) {
        viruses[i].pos.x += (dx / dist) * viruses[i].speed;
        viruses[i].pos.y += (dy / dist) * viruses[i].speed;
    }
}



for (int i = 0; i < 50; i++) {
    if (viruses[i].type == 0) continue;

    bool hit = false;
    for (int j = 0; j < 100; j++) {
        if (sprays[j].life <= 0.0f) continue;

        if (checkCollision(sprays[j].pos, sprays[j].r, viruses[i].pos, viruses[i].r)) {
            hit = true;
            score += 10;
            virusesKilled++;
            break;
        }
    }

    if (hit) {
        viruses[i] = viruses[49];
        viruses[49] = Virus();
    }
}


    // Check virus hitting player
    for (int i = 0; i < 50; i++) {
        if (viruses[i].type == 0) continue;

        if (checkCollision(player.pos, player.r, viruses[i].pos, viruses[i].r)) {
            player.health--;
            viruses[i] = viruses[49];
            viruses[49] = Virus();
            if (player.health <= 0) {
                gameState = GAME_OVER;
                updateHighScores(score);
                break;
            }
        }
    }

   // Update vaccines
for (int i = 0; i < 50; i++) {
    if (vaccines[i].speed == 0) continue;

    vaccines[i].pos.y -= vaccines[i].speed;

    // Check if vaccine collected
    if (checkCollision(player.pos, player.r, vaccines[i].pos, vaccines[i].r)) {
        player.health = min(3, player.health + 1);
        score += 25;
        vaccines[i].speed = 0;
    }

    // Remove vaccines that fall off screen
    else if (vaccines[i].pos.y < -1.1f) {
        vaccines[i].speed = 0;
    }
}

    // Check level completion
    int target = level * 5;
    if (virusesKilled >= target) {
        if (level < 5) {
            level++;
            virusesKilled = 0;
            spawnVirus();
            gameState = LEVEL_COMPLETE;
        } else {
            // Game completed
            gameState = GAME_OVER;
            updateHighScores(score);
        }
    }

    // Spawn new viruses if needed
    if (viruses[0].type == 0) spawnVirus();

    // Randomly spawn vaccines
    spawnVaccine();
}

// ----- input handling -----
void keyboardDown(unsigned char key, int, int) {
    if (gameState == PLAYING) {
        if (key == ' ') sprayActive = true;
    } else if (gameState == GAME_OVER && key == 13) { // Enter key
        gameState = HIGH_SCORES;
    } else if (gameState == HIGH_SCORES && (key == 'r' || key == 'R')) {
        // Restart game
        gameState = PLAYING;
        player.health = 3;
        score = 0;
        virusesKilled = 0;
        level = 1;
        scoreAdded = false;
        player.pos = {0.0f, 0.0f};
        spawnVirus();
    } else if (gameState == LEVEL_COMPLETE && key == ' ') {
        gameState = PLAYING;
    }

    if (key == 27) exit(0); // ESC key
}

void keyboardUp(unsigned char key, int, int) {
    if (key == ' ') sprayActive = false;
}

// ----- main rendering -----
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
// Draw all active viruses


    drawParkBackground();
    drawFlower();
    drawBush();
for (int i = 0; i < 50 && viruses[i].type != 0; i++) {
    drawVirus(viruses[i]);
}
   if (gameState == PLAYING || gameState == LEVEL_COMPLETE) {
    drawCartoonHuman(player.pos);

    for (int i = 0; i < 50; i++) {
        if (viruses[i].type != 0) {
            drawVirus(viruses[i]);
        }
    }


    for (int i = 0; i < 50; i++) {
        if (vaccines[i].speed != 0) {
            drawVaccine(vaccines[i]);
        }
    }


    drawSpray();
     drawRaindrops();
     drawSnowflakes();

    renderHUD();
}
    if (gameState == GAME_OVER) {
        renderGameOver();
    } else if (gameState == HIGH_SCORES) {
        renderHighScores();
    } else if (gameState == LEVEL_COMPLETE) {
        renderLevelComplete();
    }

    glutSwapBuffers();
}

void timer(int) {
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}
void sound()

{

    //PlaySound("a.wav", NULL, SND_ASYNC|SND_FILENAME);

    PlaySound("bensound-realworld (online-audio-converter.com).wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);

}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutCreateWindow("COVID-19 Park Game");

    glClearColor(0, 0, 0, 1);

    initParkObjects();
    spawnVirus();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, timer, 0);
    sound();
    spawnRaindrops(100);
    spawnSnowflakes(100);
    glutMainLoop();
    return 0;
}
