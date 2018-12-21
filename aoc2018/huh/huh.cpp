#include "stdafx.h"
#include <string>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <array>
#include <set>
#include <cassert>

using namespace std::string_literals;

static constexpr size_t PART2_DEPTH = 1000;

struct Room {
    //std::array<Room *, 4> door;
    //Room() : door() { }
    //Room(int dir, Room *prev) : door() { door[dir ^ 1] = prev; }
    //Room * walk(int dir) {
    //    if (!door[dir]) {
    //        door[dir] = new Room(dir, this);
    //    }
    //    return door[dir];
    //}

    Room *north = nullptr;
    Room *south = nullptr;
    Room *east = nullptr;
    Room *west = nullptr;
    Room() {}
    Room *walk_north() { if (!north) { north = new Room(); north->south = this; }; return north; }
    Room *walk_south() { if (!south) { south = new Room(); south->north = this; }; return south; }
    Room *walk_east() { if (!east) { east = new Room(); east->west = this; }; return east; }
    Room *walk_west() { if (!west) { west = new Room(); west->east = this; }; return west; }

};

int main() {
    Room *R0 = new Room();
    std::vector< std::vector<Room *> > S = { { R0 } };

    auto TOP = [&S]() -> auto& { return S.back(); };
    auto SAVED = [&S]() -> auto& { return *(S.rbegin() + 1); };
    auto NEXT = [&S]() -> auto& { return *(S.rbegin() + 2); };

    // Build the map from the regex
    for (int ch = getchar(); ch != EOF; ch = getchar()) {
        switch (ch) {
        case 'N':
            for (auto&& room : TOP())
                room = room->walk_north();
            break;
        case 'S':
            for (auto&& room : TOP())
                room = room->walk_south();
            break;
        case 'E':
            for (auto&& room : TOP())
                room = room->walk_east();
            break;
        case 'W':
            for (auto&& room : TOP())
                room = room->walk_west();
            break;
        case '(':
            S.push_back(std::move(TOP()));
            S.push_back(TOP());
            break;
        case '|': case ')':
            NEXT().insert(NEXT().end(),
                TOP().begin(), TOP().end());
            if (ch == '|') {
                TOP() = SAVED();
            }
            else {
                S.pop_back();
                S.pop_back();
                // deduplicate
                std::sort(TOP().begin(), TOP().end());
                TOP().erase(
                    std::unique(TOP().begin(), TOP().end()),
                    TOP().end());
            }
            break;
        }
    }

    // Breadth-first search to get room counts at each depth
    std::set<Room *> frontier{ R0 }, closed{ R0 }, next;
    std::vector<int> counts;
#if 0
    while (!frontier.empty()) {
        counts.push_back(closed.size());
        for (auto&& r : frontier) {
            for (auto&& rn : r->door) {
                if (rn && closed.insert(rn).second) {
                    next.insert(rn);
                }
            }
        }
        frontier.swap(next);
        next.clear();
    }
#endif
    int part1 = counts.size() - 1;
    int part2 = (counts.size() < PART2_DEPTH) ? 0 :
        counts.back() - counts[PART2_DEPTH - 1];

    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2);

    assert(part1 == 4501);
    assert(part2 == 8623);

    return 0;
}
