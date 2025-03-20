#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"

#define GRID_SIZE 20
#define CELL_SIZE 30

typedef struct Node {
    int x, y;
    int g, h, f;
    struct Node* parent;
} Node;

int grid[GRID_SIZE][GRID_SIZE] = {0}; // 0 = walkable, 1 = obstacle
Node* path[GRID_SIZE * GRID_SIZE];
int pathLength = 0;
bool editingMode = true;
int enemyIndex = 0;
float timer = 0.0f;

// A* Helper Functions
int Heuristic(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

Node* CreateNode(int x, int y, Node* parent, int g, int h) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = parent;
    return newNode;
}

void FindPath(int startX, int startY, int goalX, int goalY) {
    pathLength = 0;

    Node* openSet[GRID_SIZE * GRID_SIZE];
    int openCount = 0;
    bool closedSet[GRID_SIZE][GRID_SIZE] = { false };

    Node* start = CreateNode(startX, startY, NULL, 0, Heuristic(&(Node){startX, startY}, &(Node){goalX, goalY}));
    openSet[openCount++] = start;

    while (openCount > 0) {
        int lowestIndex = 0;
        for (int i = 1; i < openCount; i++) {
            if (openSet[i]->f < openSet[lowestIndex]->f) {
                lowestIndex = i;
            }
        }

        Node* current = openSet[lowestIndex];
        if (current->x == goalX && current->y == goalY) {
            while (current != NULL) {
                path[pathLength++] = current;
                current = current->parent;
            }
            return;
        }

        openSet[lowestIndex] = openSet[--openCount];
        closedSet[current->y][current->x] = true;

        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};

        for (int i = 0; i < 4; i++) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (newX < 0 || newX >= GRID_SIZE || newY < 0 || newY >= GRID_SIZE || grid[newY][newX] == 1 || closedSet[newY][newX]) {
                continue;
            }

            int g = current->g + 1;
            int h = Heuristic(&(Node){newX, newY}, &(Node){goalX, goalY});
            Node* neighbor = CreateNode(newX, newY, current, g, h);

            bool inOpenSet = false;
            for (int j = 0; j < openCount; j++) {
                if (openSet[j]->x == newX && openSet[j]->y == newY) {
                    if (g < openSet[j]->g) {
                        openSet[j]->g = g;
                        openSet[j]->f = g + h;
                        openSet[j]->parent = current;
                    }
                    inOpenSet = true;
                    break;
                }
            }

            if (!inOpenSet) {
                openSet[openCount++] = neighbor;
            }
        }
    }
}

// Handle Mouse Input to Place Obstacles
void HandleObstacleDrawing() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int gridX = mousePos.x / CELL_SIZE;
        int gridY = mousePos.y / CELL_SIZE;
        if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
            grid[gridY][gridX] = 1; // Set obstacle
        }
    }
}

// Move the Enemy Along the Path
void MoveEnemy() {
    timer += GetFrameTime();
    if (timer > 0.3f && enemyIndex < pathLength - 1) {
        enemyIndex++;
        timer = 0;
    }
}

// Draw the Grid and Path
void Draw() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            Color cellColor = (grid[y][x] == 1) ? GRAY : LIGHTGRAY;
            DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellColor);
            DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }

    for (int i = 0; i < pathLength; i++) {
        DrawRectangle(path[i]->x * CELL_SIZE, path[i]->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, (Color){0, 0, 255, 100});
    }

    if (enemyIndex < pathLength) {
        DrawRectangle(path[enemyIndex]->x * CELL_SIZE, path[enemyIndex]->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
    }

    if (editingMode) {
        DrawText("Draw Obstacles (LMB) - Press ENTER to Start", 10, 10, 20, WHITE);
    }

    EndDrawing();
}

int main() {
    InitWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE, "Raylib C - A* Pathfinding");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (editingMode) {
            HandleObstacleDrawing();
            if (IsKeyPressed(KEY_ENTER)) {
                editingMode = false;
                FindPath(0, 0, 19, 19);
                enemyIndex = 0;
            }
        } else {
            MoveEnemy();
        }

        Draw();
    }

    CloseWindow();
    return 0;
}
