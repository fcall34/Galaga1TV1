#include <raylib.h>
#include "Game.hpp"


// Estados del juego
enum GameScreen { MENU, GAME, OPTIONS };

int main()
{
    //inicializar color gris
    Color grey = {29, 29,27,255};
    

    //dimensiones de la ven5tana
    int windowHeigth = 750;
    int windowWidth = 700;

    InitWindow(windowWidth, windowHeigth, "Galaga by cocas galacticas");

    GameScreen currentScreen = MENU;

    // Variables para el menú
    int selectedOption = 0;
    const char *menuOptions[] = { "Iniciar partida", "Configuraciones", "Salir" };
    const int optionsCount = sizeof(menuOptions) / sizeof(menuOptions[0]);

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
                game.HandleInput();
                game.Update();
                BeginDrawing();
                ClearBackground(grey);
                game.Draw();
            } break;
            case OPTIONS: {
                // Lógica de configuraciones aquí (placeholder)
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MENU;
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
                DrawText("Score: ", 100 - MeasureText("Score:", 20) / 2, 50, 20, WHITE);
            } break;

            case OPTIONS: {
                DrawText("Options Screen", windowWidth / 2 - MeasureText("Options Screen", 20) / 2, windowHeigth / 2 - 10, 20, WHITE);
                DrawText("Press ESC to go back to Menu", windowWidth / 2 - MeasureText("Press ESC to go back to Menu", 20) / 2, windowHeigth / 2 + 20, 20, GRAY);
            } break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}




