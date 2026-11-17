#include <iostream>
#include <raylib.h>

int main() {
    // 1. Window Settings
    const int screenWidth = 900;
    const int screenHeight = 600;

    // 2. Player Car Settings
    int carWidth = 70;
    int carHeight = 110;
    int carX = 415;
    int carY = 450;
    int carSpeed = 7;

    // 3. Road Settings
    int roadDivision1X = 300;
    int roadDivision2X = 600;
    int roadOffset = 0;
    int roadSpeed = 6;

    // 4. Opponent Car Settings (5 total slots; 4 & 5 activate on Medium/Hard)
    int opponentCar1Width = 70, opponentCar1Height = 110;
    int opponentCar1X = 415, opponentCar1Y = -120;
    int opponentCar1Speed = 2;

    int opponentCar2Width = 70, opponentCar2Height = 110;
    int opponentCar2X = 150, opponentCar2Y = -300;
    int opponentCar2Speed = 3;

    int opponentCar3Width = 70, opponentCar3Height = 110;
    int opponentCar3X = 680, opponentCar3Y = -480;
    int opponentCar3Speed = 4;

    int opponentCar4Width = 70, opponentCar4Height = 110;
    int opponentCar4X = 280, opponentCar4Y = -660;
    int opponentCar4Speed = 3;

    int opponentCar5Width = 70, opponentCar5Height = 110;
    int opponentCar5X = 550, opponentCar5Y = -840;
    int opponentCar5Speed = 4;

    int pad = 12;
    int score = 0;
    int count = 0;

    // 5. Game State & Difficulty (0 = Menu, 1 = Easy, 2 = Medium, 3 = Hard)
    int difficulty = 0;
    bool isGameOver = false;

    // 6. Initialize Window, Audio & Load Assets
    InitWindow(screenWidth, screenHeight, "Full Throttle");
    InitAudioDevice();
    SetTargetFPS(60);

    Sound crashSound = LoadSound("assets/carCrash.mp3");
    Sound engineSound = LoadSound("assets/carRunning.mp3");

    Image imgPlayer = LoadImage("assets/player_car.png.png");
    ImageFormat(&imgPlayer, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&imgPlayer, WHITE, BLANK);
    ImageResize(&imgPlayer, carWidth, carHeight);
    Texture2D playerCarTex = LoadTextureFromImage(imgPlayer);
    UnloadImage(imgPlayer);

    Image imgEnemy1 = LoadImage("assets/enemy1.png.png");
    ImageFormat(&imgEnemy1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&imgEnemy1, WHITE, BLANK);
    ImageResize(&imgEnemy1, opponentCar1Width, opponentCar1Height);
    ImageRotate(&imgEnemy1, 180);
    Texture2D enemy1Tex = LoadTextureFromImage(imgEnemy1);
    UnloadImage(imgEnemy1);

    Image imgEnemy2 = LoadImage("assets/enemy2.png.png");
    ImageFormat(&imgEnemy2, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&imgEnemy2, WHITE, BLANK);
    ImageResize(&imgEnemy2, opponentCar2Width, opponentCar2Height);
    ImageRotate(&imgEnemy2, 180);
    Texture2D enemy2Tex = LoadTextureFromImage(imgEnemy2);
    UnloadImage(imgEnemy2);

    Image imgEnemy3 = LoadImage("assets/enemy3.png.png");
    ImageFormat(&imgEnemy3, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&imgEnemy3, WHITE, BLANK);
    ImageResize(&imgEnemy3, opponentCar3Width, opponentCar3Height);
    ImageRotate(&imgEnemy3, 180);
    Texture2D enemy3Tex = LoadTextureFromImage(imgEnemy3);
    UnloadImage(imgEnemy3);

    // 7. Main Game Loop
    while (!WindowShouldClose()) {

        // --- DIFFICULTY SELECTION MENU ---
        if (difficulty == 0) {
            if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                difficulty = 1; // Easy: 1x speed, 3 cars
                roadSpeed = 6;
                carSpeed = 7;
                opponentCar1Speed = 2;
                opponentCar2Speed = 3;
                opponentCar3Speed = 4;
            }
            else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                difficulty = 2; // Medium: 1.5x speed, 4 cars
                roadSpeed = 9;
                carSpeed = 9;
                opponentCar1Speed = 3;
                opponentCar2Speed = 4;
                opponentCar3Speed = 6;
                opponentCar4Speed = 5;
            }
            else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                difficulty = 3; // Hard: 2x speed, 5 cars
                roadSpeed = 12;
                carSpeed = 11;
                opponentCar1Speed = 4;
                opponentCar2Speed = 6;
                opponentCar3Speed = 8;
                opponentCar4Speed = 6;
                opponentCar5Speed = 7;
            }
        }
        // --- ACTIVE GAMEPLAY ---
        else if (!isGameOver) {
            // Keep engine sound looping while driving
            if (!IsSoundPlaying(engineSound)) {
                PlaySound(engineSound);
            }

            count++;
            score = count / 60;

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) carX -= carSpeed;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) carX += carSpeed;

            if (carX < 0) carX = 0;
            if (carX > screenWidth - carWidth) carX = screenWidth - carWidth;

            roadOffset += roadSpeed;
            if (roadOffset >= 100) {
                roadOffset = 0;
            }

            // Always update Cars 1, 2, 3
            opponentCar1Y += (roadSpeed - opponentCar1Speed);
            opponentCar2Y += (roadSpeed - opponentCar2Speed);
            opponentCar3Y += (roadSpeed - opponentCar3Speed);

            if (opponentCar1Y > screenHeight) {
                opponentCar1Y = -opponentCar1Height;
                opponentCar1X = GetRandomValue(50, screenWidth - opponentCar1Width - 50);
            }
            if (opponentCar2Y > screenHeight) {
                opponentCar2Y = -opponentCar2Height;
                opponentCar2X = GetRandomValue(50, screenWidth - opponentCar2Width - 50);
            }
            if (opponentCar3Y > screenHeight) {
                opponentCar3Y = -opponentCar3Height;
                opponentCar3X = GetRandomValue(50, screenWidth - opponentCar3Width - 50);
            }

            // Update Car 4 if Medium or Hard
            if (difficulty >= 2) {
                opponentCar4Y += (roadSpeed - opponentCar4Speed);
                if (opponentCar4Y > screenHeight) {
                    opponentCar4Y = -opponentCar4Height;
                    opponentCar4X = GetRandomValue(50, screenWidth - opponentCar4Width - 50);
                }
            }

            // Update Car 5 if Hard
            if (difficulty == 3) {
                opponentCar5Y += (roadSpeed - opponentCar5Speed);
                if (opponentCar5Y > screenHeight) {
                    opponentCar5Y = -opponentCar5Height;
                    opponentCar5X = GetRandomValue(50, screenWidth - opponentCar5Width - 50);
                }
            }

            bool hit = false;

            // Collision Check for Cars 1, 2, 3
            if ((carX + pad < opponentCar1X + opponentCar1Width - pad && carX + carWidth - pad > opponentCar1X + pad &&
                carY + pad < opponentCar1Y + opponentCar1Height - pad && carY + carHeight - pad > opponentCar1Y + pad) ||
                (carX + pad < opponentCar2X + opponentCar2Width - pad && carX + carWidth - pad > opponentCar2X + pad &&
                    carY + pad < opponentCar2Y + opponentCar2Height - pad && carY + carHeight - pad > opponentCar2Y + pad) ||
                (carX + pad < opponentCar3X + opponentCar3Width - pad && carX + carWidth - pad > opponentCar3X + pad &&
                    carY + pad < opponentCar3Y + opponentCar3Height - pad && carY + carHeight - pad > opponentCar3Y + pad)) {
                hit = true;
            }

            // Collision Check for Car 4 (Medium & Hard)
            if (difficulty >= 2) {
                if (carX + pad < opponentCar4X + opponentCar4Width - pad && carX + carWidth - pad > opponentCar4X + pad &&
                    carY + pad < opponentCar4Y + opponentCar4Height - pad && carY + carHeight - pad > opponentCar4Y + pad) {
                    hit = true;
                }
            }

            // Collision Check for Car 5 (Hard only)
            if (difficulty == 3) {
                if (carX + pad < opponentCar5X + opponentCar5Width - pad && carX + carWidth - pad > opponentCar5X + pad &&
                    carY + pad < opponentCar5Y + opponentCar5Height - pad && carY + carHeight - pad > opponentCar5Y + pad) {
                    hit = true;
                }
            }

            if (hit) {
                isGameOver = true;
                StopSound(engineSound);
                PlaySound(crashSound);
            }
        }
        // --- GAME OVER STATE ---
        else {
            if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_M)) {
                StopSound(crashSound);
                isGameOver = false;
                carX = 415;
                opponentCar1Y = -120;
                opponentCar2Y = -300;
                opponentCar3Y = -480;
                opponentCar4Y = -660;
                opponentCar5Y = -840;
                count = 0;
                score = 0;

                if (IsKeyPressed(KEY_M)) {
                    difficulty = 0; // Return to difficulty menu
                }
            }
        }

        // --- DRAWING PHASE ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Draw Scrolling Road Dividers
        for (int y = -100; y < screenHeight; y += 100) {
            DrawRectangle(roadDivision1X, y + roadOffset, 10, 50, WHITE);
            DrawRectangle(roadDivision2X, y + roadOffset, 10, 50, WHITE);
        }

        if (difficulty == 0) {
            // Draw Start Menu
            DrawText("FULL THROTTLE", 270, 150, 45, YELLOW);
            DrawText("Select Difficulty:", 320, 230, 25, WHITE);
            DrawText("Press [1] - EASY   (Normal Speed, 3 Cars)", 230, 280, 20, GREEN);
            DrawText("Press [2] - MEDIUM (1.5x Speed, 4 Cars)", 230, 320, 20, ORANGE);
            DrawText("Press [3] - HARD   (2x Speed, 5 Cars)", 230, 360, 20, RED);
        }
        else {
            // Draw Player Car
            DrawTexture(playerCarTex, carX, carY, WHITE);

            // Draw Active Opponent Cars
            DrawTexture(enemy1Tex, opponentCar1X, opponentCar1Y, WHITE);
            DrawTexture(enemy2Tex, opponentCar2X, opponentCar2Y, WHITE);
            DrawTexture(enemy3Tex, opponentCar3X, opponentCar3Y, WHITE);

            if (difficulty >= 2) {
                DrawTexture(enemy1Tex, opponentCar4X, opponentCar4Y, WHITE);
            }
            if (difficulty == 3) {
                DrawTexture(enemy2Tex, opponentCar5X, opponentCar5Y, WHITE);
            }

            // Draw HUD
            DrawText(TextFormat("Score: %d", score), 10, 40, 20, WHITE);

            if (difficulty == 1) DrawText("Mode: EASY", 10, 70, 20, GREEN);
            if (difficulty == 2) DrawText("Mode: MEDIUM", 10, 70, 20, ORANGE);
            if (difficulty == 3) DrawText("Mode: HARD", 10, 70, 20, RED);

            if (isGameOver) {
                DrawText("GAME OVER! Press [R] to Retry or [M] for Menu", 10, 10, 22, RED);
            }
        }

        EndDrawing();
    }

    // 8. Cleanup
    UnloadSound(crashSound);
    UnloadSound(engineSound);
    CloseAudioDevice();

    UnloadTexture(playerCarTex);
    UnloadTexture(enemy1Tex);
    UnloadTexture(enemy2Tex);
    UnloadTexture(enemy3Tex);
    CloseWindow();
    return 0;
}