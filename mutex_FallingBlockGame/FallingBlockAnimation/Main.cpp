#include "icb_gui.h"
#include <thread>
#include <mutex>

ICBYTES panel;
int FRM1, BTN;
bool keypressed = false;

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;
const int BOX_WIDTH = 50;
const int BOX_HEIGHT = 40;
const int LASER_WIDTH = 5;
const int LASER_HEIGHT = 20;
const int MAX_BOXES = 5;

std::mutex laserMutex;

struct Box {
    int x, y;
    bool visible;
};

struct Laser {
    int x, y;
    bool active;
};

Box boxes[MAX_BOXES];
Laser laser = { 0, 0, false };

// Random number generator
int GenerateRandom(int min, int max) {
    static unsigned long seed = 123456789; // A fixed seed for simplicity
    seed = (1103515245 * seed + 12345) % 2147483648;
    return min + (seed % (max - min + 1));
}

// Initialize boxes
void InitializeBoxes() {
    for (int i = 0; i < MAX_BOXES; i++) {
        boxes[i].x = GenerateRandom(0, SCREEN_WIDTH - BOX_WIDTH);
        boxes[i].y = GenerateRandom(0, 300);
        boxes[i].visible = true;
    }
}

// Move laser upwards
void MoveLaser() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        laserMutex.lock();
        if (laser.active) {
            laser.y -= 10;
            if (laser.y < 0) {
                laser.active = false;
            }
        }
        laserMutex.unlock();
    }
}

// Check collision between laser and boxes
void CheckCollision() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        laserMutex.lock();
        if (laser.active) {
            for (int i = 0; i < MAX_BOXES; i++) {
                if (boxes[i].visible &&
                    laser.x >= boxes[i].x && laser.x <= boxes[i].x + BOX_WIDTH &&
                    laser.y >= boxes[i].y && laser.y <= boxes[i].y + BOX_HEIGHT) {

                    boxes[i].visible = false;
                    laser.active = false;
                    break;
                }
            }
        }
        laserMutex.unlock();
    }
}

// Move boxes downwards
void MoveBoxes() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        for (int i = 0; i < MAX_BOXES; i++) {
            if (boxes[i].visible) {
                boxes[i].y += 5;
                if (boxes[i].y > SCREEN_HEIGHT) {
                    boxes[i].y = 0;
                    boxes[i].x = GenerateRandom(0, SCREEN_WIDTH - BOX_WIDTH);
                }
            }
        }
    }
}

// Fire laser
void FireLaser(int startX, int startY) {
    if (laserMutex.try_lock()) {
        laser.x = startX;
        laser.y = startY;
        laser.active = true;
        laserMutex.unlock();
    }
}

// Main game loop
void GameLoop(void* v) {
    InitializeBoxes();
    std::thread laserThread(MoveLaser);
    std::thread collisionThread(CheckCollision);
    std::thread boxThread(MoveBoxes);

    while (true) {
        if (keypressed) {
            FireLaser(SCREEN_WIDTH / 2 - LASER_WIDTH / 2, SCREEN_HEIGHT - 50);
            keypressed = false;
        }

        // Clear the screen
        FillRect(panel, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000);

        // Draw boxes
        for (int i = 0; i < MAX_BOXES; i++) {
            if (boxes[i].visible) {
                FillRect(panel, boxes[i].x, boxes[i].y, BOX_WIDTH, BOX_HEIGHT, 0xff0000);
            }
        }

        // Draw laser
        laserMutex.lock();
        if (laser.active) {
            FillRect(panel, laser.x, laser.y, LASER_WIDTH, LASER_HEIGHT, 0x00ff00);
        }
        laserMutex.unlock();

        // Display updated panel
        DisplayImage(FRM1, panel);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    laserThread.join();
    collisionThread.join();
    boxThread.join();
}

// GUI initialization
void ICGUI_Create() {
    ICG_MWTitle("Laser Shooting Game");
    ICG_MWSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    ICG_SetFont(30, 12, "");
}

// Keyboard input handler
void OnKeyPress(int key) {
    if (key == VK_SPACE) {
        keypressed = true;
    }
}

// Main GUI components
void ICGUI_main() {
    CreateImage(panel, SCREEN_WIDTH, SCREEN_HEIGHT, ICB_UINT);
    FRM1 = ICG_FrameMedium(5, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
    FillRect(panel, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    DisplayImage(FRM1, panel);
    BTN = ICG_TButton(575, 5, 100, 50, "START", GameLoop, NULL);
    ICG_SetOnKeyPressed(OnKeyPress);
}
