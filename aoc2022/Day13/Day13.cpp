#include <iostream>
#include <fstream>
#include <sstream>
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
    auto c = ' ';

    while (in >> c && c != ']')
    {
        if (in.peek() == '[')
        {
            retval->items.push_back(Item{ 0, parse_packet(in) });
        }
        else if (in.peek() != ']')
        {
            auto v = 0;
            in >> v;
            retval->items.push_back(Item{ v, nullptr });
        }
    }

    return retval;
}

int compare(const Packet &p1, const Packet &p2);

// -1 if left < right, 0 if left == right, 1 if left > right
int compare(const Item &i1, const Item &i2)
{
    if (i1.packet == nullptr && i2.packet == nullptr)
    {
        if (i1.value < i2.value) return -1;
        if (i1.value > i2.value) return 1;
        return 0;
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

    return compare(*i1.packet, *i2.packet);
}

// -1 if left < right, 0 if left == right, 1 if left > right
int compare(const Packet &p1, const Packet &p2)
{
    auto c = 0;

    for (auto i1 = p1.items.begin(), i2 = p2.items.begin();
         !c && (i1 != p1.items.end() || i2 != p2.items.end());
         i1++, i2++)
    {
        if (i1 == p1.items.end()) return -1;
        if (i2 == p2.items.end()) return 1;

        c = compare(*i1, *i2);
    }

    return c;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line1, line2;
    auto retval = 0;

    for (auto n = 1; (file >> line1) && (file >> line2); n++)
    {
        std::stringstream ss(line1);
        auto p1 = parse_packet(ss);

        ss = std::stringstream(line2);
        auto p2 = parse_packet(ss);

        if (compare(*p1, *p2) < 0)
            retval += n;
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Packet *> packets;

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

    std::sort(
        packets.begin(), packets.end(),
        [](const Packet *lhs, const Packet *rhs) -> bool { return compare(*lhs, *rhs) < 0; });

    auto index1 = -1, index2 = -1;
    for (int i = 0; i < packets.size(); i++)
    {
        if (compare(*packets[i], *div1) == 0) index1 = i;
        if (compare(*packets[i], *div2) == 0) index2 = i;
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
