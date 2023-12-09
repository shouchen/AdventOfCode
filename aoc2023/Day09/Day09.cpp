    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <vector>
    #include <algorithm>
    #include <cassert>

    int predict(const std::vector<int> &curr)
    {
        std::vector<int> next;
        for (auto i = 1; i < curr.size(); i++)
            next.push_back(curr[i] - curr[i - 1]);

        return (next.size() == 1) ? next.front() : (curr.back() + predict(next));
    }

    auto solve(const std::string &filename)
    {
        std::ifstream file(filename);
        std::string line;
        auto retval = std::make_pair(0, 0);

        while (std::getline(file, line))
        {
            std::istringstream is(line);
            std::vector<int> series;
            auto n = 0;

            while (is >> n)
                series.push_back(n);

            retval.first += predict(series);

            std::reverse(series.begin(), series.end());
            retval.second += predict(series);
        }

        return retval;
    }

    int main()
    {
        auto answer = solve("input.txt");
        std::cout << "Part One: " << answer.first << std::endl;
        std::cout << "Part Two: " << answer.second << std::endl;

        assert(answer.first == 1819125966);
        assert(answer.second == 1140);
        return 0;
    }

