using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Program
{
    public static void Main()
    {
        string[] map = File.ReadAllLines("day15.txt");

        Console.WriteLine(new Game(map, 3).RunGame(false));

        for (int attackPower = 4; ; attackPower++)
        {
            int? outcome = new Game(map, attackPower).RunGame(true);
            if (outcome.HasValue)
            {
                Console.WriteLine(outcome);
                break;
            }
        }
    }
}

public class Game
{
    private readonly string[] _map;
    private List<Unit> _units = new List<Unit>();
    public Game(string[] initialMap, int elfAttackPower)
    {
        for (int y = 0; y < initialMap.Length; y++)
        {
            for (int x = 0; x < initialMap[y].Length; x++)
            {
                if (initialMap[y][x] == 'G')
                    _units.Add(new Unit { X = x, Y = y, IsGoblin = true, Health = 200, AttackPower = 3 });
                else if (initialMap[y][x] == 'E')
                    _units.Add(new Unit { X = x, Y = y, IsGoblin = false, Health = 200, AttackPower = elfAttackPower });
            }
        }

        _map = initialMap.Select(l => l.Replace('G', '.').Replace('E', '.')).ToArray();
    }

    // Returns outcome of game.
    public int? RunGame(bool failOnElfDeath)
    {
        for (int rounds = 0; ; rounds++)
        {
            _units = _units.OrderBy(u => u.Y).ThenBy(u => u.X).ToList();
            for (int i = 0; i < _units.Count; i++)
            {
                Unit u = _units[i];
                List<Unit> targets = _units.Where(t => t.IsGoblin != u.IsGoblin).ToList();
                if (targets.Count == 0)
                    return rounds * _units.Sum(ru => ru.Health);

                if (!targets.Any(t => IsAdjacent(u, t)))
                    TryMove(u, targets);

                Unit bestAdjacent =
                    targets
                    .Where(t => IsAdjacent(u, t))
                    .OrderBy(t => t.Health)
                    .ThenBy(t => t.Y)
                    .ThenBy(t => t.X)
                    .FirstOrDefault();

                if (bestAdjacent == null)
                    continue;

                bestAdjacent.Health -= u.AttackPower;
                if (bestAdjacent.Health > 0)
                    continue;

                if (failOnElfDeath && !bestAdjacent.IsGoblin)
                    return null;

                int index = _units.IndexOf(bestAdjacent);
                _units.RemoveAt(index);
                if (index < i)
                    i--;
            }
        }
    }

    // Important: ordered in reading order
    private static readonly (int dx, int dy)[] s_neis = { (0, -1), (-1, 0), (1, 0), (0, 1) };
    private void TryMove(Unit u, List<Unit> targets)
    {
        HashSet<(int x, int y)> inRange = new HashSet<(int x, int y)>();
        foreach (Unit target in targets)
        {
            foreach ((int dx, int dy) in s_neis)
            {
                (int nx, int ny) = (target.X + dx, target.Y + dy);
                if (IsOpen(nx, ny))
                    inRange.Add((nx, ny));
            }
        }

        Queue<(int x, int y)> queue = new Queue<(int x, int y)>();
        Dictionary<(int x, int y), (int px, int py)> prevs = new Dictionary<(int x, int y), (int px, int py)>();
        queue.Enqueue((u.X, u.Y));
        prevs.Add((u.X, u.Y), (-1, -1));
        while (queue.Count > 0)
        {
            (int x, int y) = queue.Dequeue();
            foreach ((int dx, int dy) in s_neis)
            {
                (int x, int y) nei = (x + dx, y + dy);
                if (prevs.ContainsKey(nei) || !IsOpen(nei.x, nei.y))
                    continue;

                queue.Enqueue(nei);
                prevs.Add(nei, (x, y));
            }
        }

        List<(int x, int y)> getPath(int destX, int destY)
        {
            if (!prevs.ContainsKey((destX, destY)))
                return null;
            List<(int x, int y)> path = new List<(int x, int y)>();
            (int x, int y) = (destX, destY);
            while (x != u.X || y != u.Y)
            {
                path.Add((x, y));
                (x, y) = prevs[(x, y)];
            }

            path.Reverse();
            return path;
        }

        List<(int tx, int ty, List<(int x, int y)> path)> paths =
            inRange
            .Select(t => (t.x, t.y, path: getPath(t.x, t.y)))
            .Where(t => t.path != null)
            .OrderBy(t => t.path.Count)
            .ThenBy(t => t.y)
            .ThenBy(t => t.x)
            .ToList();

        List<(int x, int y)> bestPath = paths.FirstOrDefault().path;
        if (bestPath != null)
            (u.X, u.Y) = bestPath[0];
    }

    private bool IsOpen(int x, int y) => _map[y][x] == '.' && _units.All(u => u.X != x || u.Y != y);
    private bool IsAdjacent(Unit u1, Unit u2) => Math.Abs(u1.X - u2.X) + Math.Abs(u1.Y - u2.Y) == 1;

    private class Unit
    {
        public int X, Y;
        public bool IsGoblin;
        public int Health = 200;
        public int AttackPower;
    }
}