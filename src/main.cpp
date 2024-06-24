#include <raylib.h>
#include "Game.hpp"
#include <string>
#include <iostream>

std::string formatWithZeros(int number, int width){
    std::string numberText = std::to_string(number);
    int leadingZeros =width-numberText.length();
    return numberText = std::string(leadingZeros, '0')+numberText;


}


// Estados del juego
enum GameScreen { MENU, GAME, OPTIONS, PAUSE};

int main()
{
    //inicializar color gris
    Color grey = {29, 29,27,255};
    Color yellow = {243, 216, 63, 255};
    

    //dimensiones de la ven5tana
    int offset = 50;
    int windowHeigth = 750;
    int windowWidth = 700;

    InitWindow(windowWidth + offset, windowHeigth + 2*offset , "Galaga by cocas galacticas");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0,0);
    Texture2D Spaceshipimage = LoadTexture("Graphics/spaceship.png");

    GameScreen currentScreen = MENU;

    // Variables para el menú
    int selectedOption = 0;
    int selectedDifficulty =0;
    int selectedPause = 0;
    const char *menuOptions[] = { "Iniciar partida", "Configuraciones", "Salir" };
    const char *difficultyOptions[] = {"Facil", "Media", "Dificil", "Infierno"};
    const char *pauseOptions[] = {"Reanudar", "Cambiar Dificultad", "Finalizar juego"};
    const int pauseCount = sizeof(pauseOptions) / sizeof(pauseOptions[0]);
    const int optionsCount = sizeof(menuOptions) / sizeof(menuOptions[0]);
    const int difficultyCount = sizeof(difficultyOptions)/ sizeof(difficultyOptions[0]);

    Game game;

    SetTargetFPS(30);

    

    while (!WindowShouldClose()) {
        // Manejo de la lógica del menú
        switch (currentScreen) {
            case MENU: {
                // Navegar por las opciones del menú
                if (IsKeyPressed(KEY_DOWN)) selectedOption++;
                if (IsKeyPressed(KEY_UP)) selectedOption--;

                if (selectedOption < 0) selectedOption = optionsCount - 1;
                if (selectedOption >= optionsCount) selectedOption = 0;

                if (IsKeyPressed(KEY_ENTER)) {
                    switch (selectedOption) {
                        case 0: currentScreen = GAME; break;  // Ir al juego
                        case 1: currentScreen = OPTIONS; break;  // Ir a configuraciones
                        case 2: CloseWindow(); return 0;  // Salir del juego
                    }
                }
            } break;

            case GAME: {
                // Lógica del juego aquí (placeholder)

                if (IsKeyPressed(KEY_TAB)) {
                    game.pause = !game.pause;
                    currentScreen = game.pause ? PAUSE : GAME;
                }

                if (!game.pause) {
                    UpdateMusicStream(game.music);
                    game.HandleInput();
                    game.Update();
                }
                BeginDrawing();
                ClearBackground(grey);
                DrawRectangleRoundedLines({10,10,730,830}, 0.18f, 20, 2, yellow);
                DrawLineEx({10,780}, {735, 780}, 3, yellow);
                game.run ? DrawTextEx(font, "LEVEL 01", {570, 790}, 34,2, yellow) : DrawTextEx(font, "GAME OVER", {570, 790}, 34,2, yellow);
                float x = 50.0;
                for(int i=0; i<game.lives; i++){
                    DrawTextureV(Spaceshipimage, {x,790}, WHITE);
                    x+=50;
                }
                if(IsKeyPressed(KEY_COMMA)){
                    currentScreen = MENU;
                }
                game.Draw();
            } break;
            case OPTIONS: {
                // Navegar por las opciones del menú
                if (IsKeyPressed(KEY_DOWN)) selectedDifficulty++;
                if (IsKeyPressed(KEY_UP)) selectedDifficulty--;

                if (selectedDifficulty < 0) selectedDifficulty = difficultyCount - 1;
                if (selectedDifficulty >= difficultyCount) selectedDifficulty = 0;

                if (IsKeyPressed(KEY_ENTER)) {
                    game.Difficulty(selectedDifficulty);
                    
                    currentScreen = MENU;
                }
                if(IsKeyPressed(KEY_COMMA)){
                    currentScreen = MENU;
                }
            } break;
            case PAUSE: {
                if (IsKeyPressed(KEY_TAB)) {
                    game.pause = !game.pause;
                    currentScreen = game.pause ? PAUSE : GAME;
                }
                if (IsKeyPressed(KEY_DOWN)) selectedPause++;
                if (IsKeyPressed(KEY_UP)) selectedPause--;

                if (selectedPause < 0) selectedPause = pauseCount - 1;
                if (selectedPause >= pauseCount) selectedPause = 0;
            
            } break;
        }

        // Dibujar en la pantalla
        BeginDrawing();
        ClearBackground(grey);  // Fondo oscuro

        switch (currentScreen) {
            case MENU: {
                DrawText("Galaga by Cocas Galacticas", windowWidth / 2 - MeasureText("Galaga by Cocas Galacticas", 40) / 2, 100, 40, WHITE);
                for (int i = 0; i < optionsCount; i++) {
                    Color color = (i == selectedOption) ? RED : WHITE;  // Opción seleccionada en rojo, las demás en blanco
                    DrawText(menuOptions[i], windowWidth / 2 - MeasureText(menuOptions[i], 20) / 2, 300 + 40 * i, 20, color);
                }
            } break;

            case GAME: {
                DrawTextEx(font, "SCORE: ", {50, 15}, 34, 2, yellow);
                std::string scoreText = formatWithZeros(game.score, 5);
                DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, yellow);

                DrawTextEx(font, "HIGH SCORE", {570, 15}, 34, 2, yellow);
                std::string highscoreText = formatWithZeros(game.highscore, 5);
                DrawTextEx(font, highscoreText.c_str(), {655, 40}, 34, 2, yellow);
            } break;

            case OPTIONS: {
            DrawText("Seleccionar Dificultad", windowWidth / 2 - MeasureText("Seleccionar Dificultad", 40) / 2, 100, 40, WHITE);
            for (int i = 0; i < difficultyCount; i++) {
                Color color = (i == selectedDifficulty) ? RED : WHITE;
                DrawText(difficultyOptions[i], windowWidth / 2 - MeasureText(difficultyOptions[i], 20) / 2, 300 + 40 * i, 20, color);
            }
        } break;
        }

        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();

    return 0;
}



