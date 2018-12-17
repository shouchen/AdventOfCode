#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;
struct unit {
    int x, y;
    int hp;
    int cp;
    char type;
    bool moved;
};
const int MAXN = 500;

string s;

vector <string> map;
vector <unit> units;

int n, m;

int d[MAXN][MAXN];
bool bio[MAXN][MAXN];

queue<pair<int, int> > q;

int killed;

int smjx[] = { -1, 0, 1, 0 };
int smjy[] = { 0, 1, 0, -1 };

bool cmp(unit a, unit b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

int whom_to_attack(int x) {
    int minhp = 201;
    int target = -1;
    for (int i = 0; i < (int)units.size(); i++) {
        if (i == x ||
            units[i].hp < 0) {
            continue;
        }

        for (int j = 0; j < 4; j++) {
            if (units[x].x + smjx[j] == units[i].x &&
                units[x].y + smjy[j] == units[i].y &&
                units[x].type != units[i].type) {
                if (units[i].hp < minhp) {
                    target = i;
                    minhp = units[i].hp;
                }
                else if (units[i].hp == minhp) {
                    if (make_pair(units[i].x, units[i].y) < make_pair(units[target].x, units[target].y)) {
                        target = i;
                    }
                }
            }
        }
    }

    return target;
}

void reconstruct_path(pair<int, int> cur, pair<int, int> prev, pair<int, int> goal, vector<pair<int, int> >& result) {
    if (cur == goal) {
        result.push_back(prev);
        return;
    }

    int dist = d[cur.first][cur.second];

    for (int i = 0; i < 4; i++) {
        pair<int, int> ncur = make_pair(cur.first + smjx[i], cur.second + smjy[i]);

        if (ncur.first >= 0 &&
            ncur.first < n &&
            ncur.second >= 0 &&
            ncur.second < m &&
            d[ncur.first][ncur.second] == dist - 1) {
            reconstruct_path(ncur, cur, goal, result);
        }
    }

    return;
}

bool turn(int x) {
    bool e = false;
    for (int i = 0; i < (int)units.size(); i++) {
        e |= (units[i].type != units[x].type);
    }

    if (!e) {
        return false;
    }

    int y = whom_to_attack(x);
    if (y != -1) {
        units[y].hp -= units[x].cp;

        if (units[y].hp <= 0) {
            killed = y;
        }

        return true;
    }

    memset(d, 0x3f3f3f, sizeof(d));
    memset(bio, 0, sizeof(bio));

    q.push(make_pair(units[x].x, units[x].y));
    d[units[x].x][units[x].y] = 0;

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();

        if (bio[cur.first][cur.second]) {
            continue;
        }

        bio[cur.first][cur.second] = true;

        for (int i = 0; i < 4; i++) {
            pair<int, int> ncur = make_pair(cur.first + smjx[i], cur.second + smjy[i]);

            if (ncur.first >= 0 &&
                ncur.first < n &&
                ncur.second >= 0 &&
                ncur.second < m &&
                map[ncur.first][ncur.second] == '.' &&
                !bio[ncur.first][ncur.second]) {
                d[ncur.first][ncur.second] = d[cur.first][cur.second] + 1;
                q.push(ncur);
            }
        }
    }

    int mind = 4 * MAXN;
    pair<int, int> target = make_pair(MAXN, MAXN);
    for (int i = 0; i < (int)units.size(); i++) {
        if (units[i].type == units[x].type) {
            continue;
        }

        for (int j = 0; j < 4; j++) {
            pair<int, int> cur = make_pair(units[i].x + smjx[j], units[i].y + smjy[j]);

            if (cur.first >= 0 &&
                cur.first < n &&
                cur.second >= 0 &&
                cur.second < m &&
                bio[cur.first][cur.second]) {
                if (d[cur.first][cur.second] < mind) {
                    mind = d[cur.first][cur.second];
                    target = cur;
                }
                else if (d[cur.first][cur.second] == mind) {
                    if (cur < target) {
                        target = cur;
                    }
                }
            }
        }
    }

    if (mind == 4 * MAXN) {
        return true;
    }

    vector<pair<int, int> > nexts;
    nexts.clear();

    reconstruct_path(target, make_pair(-1, -1), make_pair(units[x].x, units[x].y), nexts);

    sort(nexts.begin(), nexts.end());

    map[nexts[0].first][nexts[0].second] = units[x].type;
    map[units[x].x][units[x].y] = '.';

    units[x].x = nexts[0].first;
    units[x].y = nexts[0].second;

    y = whom_to_attack(x);
    if (y != -1) {
        units[y].hp -= units[x].cp;

        if (units[y].hp <= 0) {
            killed = y;
        }
    }

    return true;
}

int main(void) {
    std::ifstream file("temp-input.txt");

    for (int x = 0; file >> s; x++) {
        map.push_back(s);
    }

    n = map.size();
    m = map[0].size();

    vector <string> map2 = map;

    bool f = true;
    int t;
    for (int c = 3; f; c++) {
        map = map2;
        f = false;

        units.clear();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (map[i][j] == 'G' || map[i][j] == 'E') {
                    unit tmp;
                    tmp.x = i;
                    tmp.y = j;
                    tmp.hp = 200;
                    tmp.cp = (map[i][j] == 'E' ? c : 3);
                    tmp.type = map[i][j];

                    units.push_back(tmp);
                }
            }
        }

        t = 0;
        bool e = true;
        for (t = 0;; t++) {
            for (int i = 0; i < (int)units.size(); i++) {
                units[i].moved = false;
            }

            sort(units.begin(), units.end(), cmp);

            for (int i = 0; i < (int)units.size(); i++) {
                if (units[i].moved) {
                    continue;
                }

                killed = -1;
                if (!turn(i)) {
                    e = false;
                    break;
                }

                units[i].moved = true;

                if (killed != -1) {
                    if (units[killed].type == 'E') {
                        f = true;
                        e = false;
                        break;
                    }
                    vector<unit> tmp;

                    for (int j = 0; j < (int)units.size(); j++) {
                        if (j != killed) {
                            tmp.push_back(units[j]);
                        }
                        else {
                            map[units[j].x][units[j].y] = '.';
                        }
                    }

                    units = tmp;
                    i = -1;
                }
            }

            if (!e) {
                break;
            }
        }
    }

    int s = 0;
    for (int i = 0; i < (int)units.size(); i++) {
        s += units[i].hp;
    }

    cout << t * s << endl;
    return 0;
}
