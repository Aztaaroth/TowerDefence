using System;
using System.Collections.Generic;

public class Node {
    public int X, Y;
    public int G, H, F;
    public Node Parent;

    public Node(int x, int y) {
        X = x; Y = y;
        G = H = F = 0;
        Parent = null;
    }
}

public class AStarPathfinder {
    private const int GRID_SIZE = 20;
    public int[,] grid = new int[GRID_SIZE, GRID_SIZE];

    public AStarPathfinder() {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                grid[y, x] = 0; // Initialize grid with empty spaces
            }
        }
    }

    private int Heuristic(Node a, Node b) {
        return Math.Abs(a.X - b.X) + Math.Abs(a.Y - b.Y);
    }

    public List<Node> FindPath(Node start, Node goal) {
        List<Node> openSet = new List<Node> { start };
        HashSet<(int, int)> closedSet = new HashSet<(int, int)>();

        while (openSet.Count > 0) {
            openSet.Sort((a, b) => a.F.CompareTo(b.F));
            Node current = openSet[0];

            if (current.X == goal.X && current.Y == goal.Y) {
                List<Node> path = new List<Node>();
                while (current != null) {
                    path.Add(current);
                    current = current.Parent;
                }
                path.Reverse();
                return path;
            }

            openSet.Remove(current);
            closedSet.Add((current.X, current.Y));

            int[] dx = { 0, 1, 0, -1 };
            int[] dy = { 1, 0, -1, 0 };

            for (int i = 0; i < 4; i++) {
                int newX = current.X + dx[i];
                int newY = current.Y + dy[i];

                if (newX < 0 || newX >= GRID_SIZE || newY < 0 || newY >= GRID_SIZE)
                    continue;

                if (grid[newY, newX] == 1 || closedSet.Contains((newX, newY)))
                    continue;

                Node neighbor = new Node(newX, newY);
                int tentativeG = current.G + 1;

                if (!openSet.Contains(neighbor) || tentativeG < neighbor.G) {
                    neighbor.G = tentativeG;
                    neighbor.H = Heuristic(neighbor, goal);
                    neighbor.F = neighbor.G + neighbor.H;
                    neighbor.Parent = current;

                    if (!openSet.Contains(neighbor))
                        openSet.Add(neighbor);
                }
            }
        }

        return null; // No path found
    }
}
