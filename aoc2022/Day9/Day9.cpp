#include <iostream>
#include <fstream>
#include <array>
#include <set>
#include <cstdlib>
#include <cassert>

/*
void dump1()
{
    for (int r = -4; r <= 0; r++)
    {
        for (int c = 0; c < 6; c++)
        {
            if (h_rc.first == r && h_rc.second == c)
                cout << "H";
            else if (t_rc.first == r && t_rc.second == c)
                cout << "T";
            else
                cout << ".";
        }
        cout << endl;
    }
    cout << endl;
}
*/

auto do_part1(const std::string &filename)
{
    std::pair<int, int> h_rc = std::make_pair(0, 0), t_rc = std::make_pair(0, 0);
    std::set<std::pair<int, int>> visited;

    std::ifstream file(filename);
    auto dir = ' ';
    auto dist = 0;

    //dump1();

    while (file >> dir >> dist)
    {
        //cout << "== " << dir << " " << dist << " ==" << endl << endl;

        for (int i = 0; i < dist; i++)
        {
            if (dir == 'U')
                --h_rc.first;
            else if (dir == 'D')
                h_rc.first++;
            else if (dir == 'L')
                --h_rc.second;
            else if (dir == 'R')
                h_rc.second++;

            // check tail move if two steps
            if (t_rc.first - h_rc.first == 2 && t_rc.second == h_rc.second)
                t_rc.first--;
            else if (h_rc.first - t_rc.first == 2 && t_rc.second == h_rc.second)
                t_rc.first++;
            else if (t_rc.second - h_rc.second == 2 && t_rc.first == h_rc.first)
                t_rc.second--;
            else if (h_rc.second - t_rc.second == 2 && t_rc.first == h_rc.first)
                t_rc.second++;
            else if (abs(h_rc.first - t_rc.first) == 2 || abs(h_rc.second - t_rc.second) == 2)
            {
                if (h_rc.first < t_rc.first)
                    t_rc.first--;
                if (h_rc.first > t_rc.first)
                    t_rc.first++;
                if (h_rc.second < t_rc.second)
                    t_rc.second--;
                if (h_rc.second > t_rc.second)
                    t_rc.second++;
            }

            //dump1();

            visited.insert(t_rc);
        }
    }

    return visited.size();
}

/*
void dump2()
{
    for (int r = -15; r <= 5; r++)
    {
        for (int c = -11; c <= 14; c++)
        {
            if (pos[0].first == r && pos[0].second == c)
                cout << "H";
            else if (pos[1].first == r && pos[1].second == c)
                cout << "1";
            else if (pos[2].first == r && pos[2].second == c)
                cout << "2";
            else if (pos[3].first == r && pos[3].second == c)
                cout << "3";
            else if (pos[4].first == r && pos[4].second == c)
                cout << "4";
            else if (pos[5].first == r && pos[5].second == c)
                cout << "5";
            else if (pos[6].first == r && pos[6].second == c)
                cout << "6";
            else if (pos[7].first == r && pos[7].second == c)
                cout << "7";
            else if (pos[8].first == r && pos[8].second == c)
                cout << "8";
            else if (pos[9].first == r && pos[9].second == c)
                cout << "9";
            else
                cout << ".";
        }
        cout << endl;
    }
    cout << endl;
}
*/

auto do_part2(const std::string &filename)
{
    std::array<std::pair<int, int>, 10> pos;
    std::set<std::pair<int, int>> visited;

    std::ifstream file(filename);
    auto dir = ' ';
    auto dist = 0;

    //dump2();

    while (file >> dir >> dist)
    {
        //cout << "== " << dir << " " << dist << " ==" << endl << endl;

        for (int i = 0; i < dist; i++)
        {
            // move head
            if (dir == 'U')
                --pos[0].first;
            else if (dir == 'D')
                pos[0].first++;
            else if (dir == 'L')
                --pos[0].second;
            else if (dir == 'R')
                pos[0].second++;

            // check tail move if two steps
            for (int j = 1; j <= 9; j++)
            {
                if (pos[j].first - pos[j - 1].first == 2 && pos[j].second == pos[j - 1].second)
                    pos[j].first--;
                else if (pos[j - 1].first - pos[j].first == 2 && pos[j].second == pos[j - 1].second)
                    pos[j].first++;
                else if (pos[j].second - pos[j - 1].second == 2 && pos[j].first == pos[j - 1].first)
                    pos[j].second--;
                else if (pos[j - 1].second - pos[j].second == 2 && pos[j].first == pos[j - 1].first)
                    pos[j].second++;
                else if (abs(pos[j - 1].first - pos[j].first) == 2 || abs(pos[j - 1].second - pos[j].second) == 2)
                {
                    if (pos[j - 1].first < pos[j].first)
                        pos[j].first--;
                    if (pos[j - 1].first > pos[j].first)
                        pos[j].first++;
                    if (pos[j - 1].second < pos[j].second)
                        pos[j].second--;
                    if (pos[j - 1].second > pos[j].second)
                        pos[j].second++;
                }
            }

            //dump2();
            visited.insert(pos[9]);
        }
    }

    return visited.size();
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6090);
    assert(part2 == 2566);
    return 0;
}
