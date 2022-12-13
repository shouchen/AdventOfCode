#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

struct Packet;

struct Item
{
    int value;
    Packet *packet;
};

struct Packet
{
    std::vector<Item> items;
};

Packet *parse_packet(std::istream &in)
{
    auto retval = new Packet();

    auto c = '[';
    in >> c;

    for (;;)
    {
        auto v = 0;

        auto peek = in.peek();

        if (peek == '[')
        {
            auto p = parse_packet(in);
            retval->items.push_back(Item{ 0, p });

            in >> c;

            if (c == ']')
                break;
        } else if (peek == ']')
        {
            in >> c;
            break;
        }
        else
        {
            in >> v >> c;
            retval->items.push_back(Item{ v, nullptr });

            if (c == ']')
                break;
        }
    }

    return retval;
}

int compare(const Packet &p1, const Packet &p2);

// -1 if right order, 0 if equal, 1 if wrong order
int compare(const Item &i1, const Item &i2)
{
    if (i1.packet == nullptr && i2.packet == nullptr)
    {
        if (i1.value < i2.value)
            return -1;
        if (i1.value > i2.value)
            return 1;
        return 0;
    }
    else if (i1.packet != nullptr && i2.packet != nullptr)
    {
        return compare(*i1.packet, *i2.packet);
    }
    else if (i1.packet == nullptr && i2.packet != nullptr)
    {
        Packet p;
        p.items.push_back(Item{ i1.value });
        return compare(p, *i2.packet);
    }
    else if (i1.packet != nullptr && i2.packet == nullptr)
    {
        Packet p;
        p.items.push_back(Item{ i2.value });
        return compare(*i1.packet, p);
    }
    assert(false);
    return 0;
}

// -1 if right order, 0 if equal, 1 if wrong order
int compare(const Packet &p1, const Packet &p2)
{
    auto until = std::max(p1.items.size(), p2.items.size());
    for (int i = 0; i < until; i++)
    {
        if (i >= p1.items.size())
            return -1;
        if (i >= p2.items.size())
            return 1;

        auto temp = compare(p1.items[i], p2.items[i]);

        if (temp != 0)
            return temp;
    }

    return 0;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line1, line2;
    auto num = 1;

    auto retval = 0;
    while (getline(file, line1) && getline(file, line2))
    {
        std::stringstream ss(line1);
        auto p1 = parse_packet(ss);

        ss = std::stringstream(line2);
        auto p2 = parse_packet(ss);

        if (compare(*p1, *p2) < 0)
            retval += num;

        getline(file, line1);
        num++;
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Packet *> packets;

    auto retval = 0;
    while (file >> line)
    {
        std::stringstream ss(line);
        auto p = parse_packet(ss);
        packets.push_back(p);
    }

    std::stringstream ss1("[[2]]");
    auto div1 = parse_packet(ss1);
    packets.push_back(div1);

    std::stringstream ss2("[[6]]");
    auto div2 = parse_packet(ss2);
    packets.push_back(div2);

    for (;;)
    {
        bool swapped = false;
        for (int i = 0; i < packets.size() - 1; i++)
        {
            auto temp = compare(*packets[i], *packets[i + 1]);
            if (temp == 1)
            {
                std::swap(packets[i], packets[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    auto index1 = -1, index2 = -1;
    for (int i = 0; i < packets.size(); i++)
    {
        if (compare(*packets[i], *div1) == 0)
            index1 = i;

        if (compare(*packets[i], *div2) == 0)
            index2 = i;
    }
    
    return (index1 + 1) * (index2 + 1);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6072); 
    assert(part2 == 22184);
    return 0;
}
