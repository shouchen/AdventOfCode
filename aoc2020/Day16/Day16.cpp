#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

using Ticket = std::vector<int>;

struct Category
{
    std::string name;
    std::pair<int, int> range1, range2;
    std::set<int> candidate_fields;

    void initialize_candidates(int fields)
    {
        for (int i = 0; i < fields; i++)
            candidate_fields.insert(i);
    }

    auto allows_value(int value)
    {
        return
            (value >= range1.first && value <= range1.second) ||
            (value >= range2.first && value <= range2.second);
    }
};

std::vector<Category> categories;

void read_categories(std::istream &file)
{
    std::string s;

    while (getline(file, s) && !s.empty())
    {
        categories.push_back(Category());
        auto &category = categories.back();

        auto pos = s.find(':');
        category.name = s.substr(0, pos);

        std::stringstream ss(s.substr(pos + 1));
        auto dash = '-';
        std::string or_word;

        ss >> category.range1.first >> dash >> category.range1.second >> or_word;
        ss >> category.range2.first >> dash >> category.range2.second;
    }
}

auto read_ticket(const std::string &s)
{
    std::stringstream ss(s);
    Ticket ticket;
    auto val = 0;

    while (ss >> val)
    {
        ticket.push_back(val);

        auto comma = ',';
        ss >> comma;
    }

    return ticket;
}

auto is_valid(Ticket &ticket, long long &accum_invalid_fields)
{
    auto retval = true;

    for (auto value : ticket)
    {
        auto valid_field = false;
        for (auto &category : categories)
            if (category.allows_value(value))
            {
                valid_field = true;
                break;
            }

        if (!valid_field)
        {
            retval = false;
            accum_invalid_fields += value;
        }
    }

    return retval;
}

void remove_candidate(Category &category, int val)
{
    if (category.candidate_fields.erase(val) == 1 && category.candidate_fields.size() == 1)
        for (auto &other : categories)
            if (other.candidate_fields.size() > 1)
                remove_candidate(other, *category.candidate_fields.begin());
}

auto do_both_parts(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string s, your_ticket_colon, nearby_tickets_colon;

    read_categories(file);

    getline(file, your_ticket_colon);
    getline(file, s);
    auto my_ticket = read_ticket(s);

    for (auto &category : categories)
        category.initialize_candidates(static_cast<int>(my_ticket.size()));

    getline(file, s);
    getline(file, nearby_tickets_colon);

    auto retval = std::make_pair(0LL, 1LL);

    while (getline(file, s))
    {
        auto ticket = read_ticket(s);

        if (is_valid(ticket, retval.first))
            for (auto i = 0; i < ticket.size(); i++)
                for (auto &category : categories)
                    if (!category.allows_value(ticket[i]))
                        remove_candidate(category, i);
    }

    for (auto &category : categories)
        if (category.name.substr(0, 10) == "departure ")
            retval.second *= my_ticket[*category.candidate_fields.begin()];

    return retval;
}

int main()
{
    auto result = do_both_parts("input.txt");

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 26869);
    assert(result.second == 855275529001);
    return 0;
}
