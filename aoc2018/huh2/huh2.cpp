#include "stdafx.h"
#include <iostream>
#include <queue>
#include <map>
#include <functional>

using namespace std;
typedef pair<int, int> pii;

int level[1001][1001];
int grid[1001][1001];

int main() {
    int depth = 5913, targetX = 8, targetY = 701;
    for (int i = 0; i <= 1000; i++) {
        for (int j = 0; j <= 1000; j++) {
            int geoIndex;
            if (i == 0) {
                geoIndex = j * 48271;
            }
            else if (j == 0) {
                geoIndex = i * 16807;
            }
            else if (i == 0 && j == 0) {
                geoIndex = 0;
            }
            else if (i == targetX && j == targetY) {
                geoIndex = 0;
            }
            else {
                geoIndex = level[i - 1][j] * level[i][j - 1];
            }
            int eroLevel = (geoIndex + depth) % 20183;
            level[i][j] = eroLevel;
            int type = eroLevel % 3;
            grid[i][j] = type;
        }
    }
    // state: {{x, y}, equipment}
    map<pair<pii, int>, int> dist;
    // this horrible template is to sort in decreasing order
    priority_queue<pair<int, pair<pii, int>>, vector<pair<int, pair<pii, int>>>, greater<pair<int, pair<pii, int>>>> dijk;

    // torch = 0, gear = 1, neither = 2
    dijk.push({ 0,{ { 0, 0 }, 0 } });
    dist[{ {0, 0}, 0}] = 0;

    while (!dijk.empty()) {
        auto curr = dijk.top(); dijk.pop();
        int currDist = curr.first;
        int x = curr.second.first.first;
        int y = curr.second.first.second;
        int equip = curr.second.second;
        if (currDist > dist[curr.second]) continue;
        if (curr.second == make_pair(pii(targetX, targetY), 0)) {
            cout << dist[curr.second] << endl;
            return 0;
        }
        vector<pii> nextLoc = { { x - 1, y },{ x + 1, y },{ x, y - 1 },{ x, y + 1 } };
        for (pii next : nextLoc) {
            int nextX = next.first;
            int nextY = next.second;
            if (nextX < 0) continue;
            if (nextY < 0) continue;
            if (equip == 0) {
                if (grid[nextX][nextY] == 1) continue;
            }
            else if (equip == 1) {
                if (grid[nextX][nextY] == 2) continue;
            }
            else {
                if (grid[nextX][nextY] == 0) continue;
            }
            pair<pii, int> nextNode = { next, equip };
            if (!dist.count(nextNode) || dist[nextNode] > dist[curr.second] + 1) {
                dist[nextNode] = dist[curr.second] + 1;
                dijk.push({ dist[curr.second] + 1, nextNode });
            }
        }
        for (int i = 0; i < 3; i++) {
            if (i == equip) continue;
            // only transition to valid equipment for region
            if (grid[x][y] == 0) {
                if (i == 2) continue;
            }
            else if (grid[x][y] == 1) {
                if (i == 0) continue;
            }
            else {
                if (i == 1) continue;
            }
            pair<pii, int> nextNode = { { x, y }, i };
            if (!dist.count(nextNode) || dist[nextNode] > dist[curr.second] + 7) {
                dist[nextNode] = dist[curr.second] + 7;
                dijk.push({ dist[curr.second] + 7, nextNode });
            }
        }
    }

    return 0;
}