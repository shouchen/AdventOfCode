#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

struct Token
{
    bool is_num; // true means n is a number
    int n; // can be '[' or ']' or ',' or a number
};

inline bool operator==(const Token &lhs, const Token &rhs)
{
    return lhs.is_num == rhs.is_num && lhs.n == rhs.n;
}

const Token open_bracket{ false, '[' }, comma{ false, ',' }, close_bracket{ false, ']' };

struct Snailfish : std::vector<Token>
{
    Snailfish &operator+=(const Snailfish &rhs)
    {
        insert(begin(), open_bracket);
        push_back(comma);

        for (auto &n : rhs)
            push_back(n);

        push_back(close_bracket);
        reduce();
        return *this;
    }

    auto get_magnitude()
    {
        while (size() > 4)
        {
            iterator it = find_regular_pair();
            if (it == end())
                break;

            // Collapse pair
            auto n1 = (it + 1)->n, n2 = (it + 3)->n;

            it = erase(it, it + 4);
            it->is_num = true;
            it->n = 3 * n1 + 2 * n2;
        }

        return (*this)[0].n;
    }

private:
    iterator find_regular_pair()
    {
        if (size() > 4)
            for (auto it = begin(); it + 4 != end(); it++)
                if (*it == open_bracket && (it + 1)->is_num && *(it + 2) == comma && (it + 3)->is_num && *(it + 4) == close_bracket)
                    return it;

        return end();
    }

    auto find_regular_pair_inside_four_bracket_pairs()
    {
        auto bracket_count = 0;
        for (auto it = begin(); it != end(); it++)
        {
            if (*it == open_bracket)
            {
                if (++bracket_count <= 4)
                    continue;

                if ((it + 1)->is_num && *(it + 2) == comma && (it + 3)->is_num && *(it + 4) == close_bracket)
                    return it;
            }
            else if (*it == close_bracket)
                --bracket_count;
        }

        return end();
    }

    void try_add_to_previous_num(iterator it, int n)
    {
        for (; it != begin(); --it)
            if (it->is_num)
            {
                it->n += n;
                break;
            }
    }

    void try_add_to_next_num(iterator it, int n)
    {
        for (; it != end(); it++)
            if (it->is_num)
            {
                it->n += n;
                break;
            }
    }

    auto try_explode()
    {
        auto it = find_regular_pair_inside_four_bracket_pairs();
        if (it == end()) return false;

        try_add_to_previous_num(it, (it + 1)->n);
        try_add_to_next_num(it + 5, (it + 3)->n);

        // Erase regular pair and leave last bracket for replace
        it = erase(it, it + 4);
        *it = { true, 0 };
        return true;
    }

    auto try_split()
    {
        for (auto it = begin(); it != end(); ++it)
            if (it->is_num && it->n > 9)
            {
                auto n = it->n;
                it = erase(it);
                insert(it, { open_bracket, {true, n / 2}, comma, {true, n / 2 + n % 2}, close_bracket });
                return true;
            }

        return false;
    }

    void reduce()
    {
        while (try_explode() || try_split());
    }
};

std::stringstream &operator>>(std::stringstream &i, Snailfish &sf)
{
    auto c = ' ';

    while (i >> c)
    {
        if (isdigit(c))
            sf.push_back(Token{ true, c - '0' });
        else
            sf.push_back(Token{ false, c });
    }

    return i;
}

auto read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Snailfish> sfs;

    std::string s;
    while (file >> s)
    {
        std::stringstream ss(s);
        Snailfish sf;

        ss >> sf;
        sfs.push_back(sf);
    }

    return sfs;
}

auto do_part1(const std::vector<Snailfish> &sfs)
{
    Snailfish total;
    for (auto sf : sfs)
        if (total.empty())
            std::swap(total, sf);
        else
            total += sf;

    return total.get_magnitude();
}

auto do_part2(std::vector<Snailfish> &sfs)
{
    auto retval = 0;
    for (auto i = 0; i < sfs.size(); i++)
        for (auto j = 0; j < sfs.size(); j++)
            if (i != j)
                retval = std::max(retval, do_part1({ sfs[i], sfs[j] }));

    return retval;
}

int main()
{
    auto sfs = read_input("input.txt");

    auto part1 = do_part1(sfs);
    std::cout << "Part 1: " << part1 << std::endl;
    assert(part1 == 3574);

    auto part2 = do_part2(sfs);
    std::cout << "Part 2: " << part2 << std::endl;
    assert(part2 == 4763);

    return 0;
}
