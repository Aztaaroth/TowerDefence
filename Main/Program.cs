using System;
using System.Collections.Generic;
using Raylib_cs;

public class Program {
    public static void Main() {
        Raylib.InitWindow(600, 600, "Raylib-cs Pathfinding");
        Raylib.SetTargetFPS(60);

        while (!Raylib.WindowShouldClose()) {
            Update();
            Draw();
        }

        Raylib.CloseWindow();
    }

    private static void Update() {
        if (editingMode) {
            HandleObstacleDrawing();
            if (Raylib.IsKeyPressed(KeyboardKey.KEY_ENTER)) {
                StartPathfinding();
            }
        } else {
            MoveEnemy();
        }
    }

    private static void HandleObstacleDrawing() {
        if (Raylib.IsMouseButtonDown(MouseButton.MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = Raylib.GetMousePosition();
            int gridX = (int)(mousePos.X / CELL_SIZE);
            int gridY = (int)(mousePos.Y / CELL_SIZE);

            if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
                pathfinder.grid[gridY, gridX] = 1; // Set obstacle
            }
        }
    }

    private static void StartPathfinding() {
        editingMode = false;
        path = pathfinder.FindPath(new Node(0, 0), new Node(19, 19));
        enemyIndex = 0;
    }

    private static void MoveEnemy() {
        timer += Raylib.GetFrameTime();
        if (timer > 0.3f && enemyIndex < path.Count - 1) {
            enemyIndex++;
            timer = 0;
        }
    }

    private static void Draw() {
        Raylib.BeginDrawing();
        Raylib.ClearBackground(Color.DARKGRAY);

        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                Color cellColor = pathfinder.grid[y, x] == 1 ? Color.GRAY : Color.LIGHTGRAY;
                Raylib.DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellColor);
                Raylib.DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, Color.BLACK);
            }
        }

        if (path != null) {
            foreach (Node node in path) {
                Raylib.DrawRectangle(node.X * CELL_SIZE, node.Y * CELL_SIZE, CELL_SIZE, CELL_SIZE, new Color(0, 0, 255, 100));
            }

            if (enemyIndex < path.Count) {
                Node enemy = path[enemyIndex];
                Raylib.DrawRectangle(enemy.X * CELL_SIZE, enemy.Y * CELL_SIZE, CELL_SIZE, CELL_SIZE, Color.RED);
            }
        }

        if (editingMode) {
            Raylib.DrawText("Draw Obstacles (LMB) - Press ENTER to Start", 10, 10, 20, Color.WHITE);
        }

        Raylib.EndDrawing();
    }
}
