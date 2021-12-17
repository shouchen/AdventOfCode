#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

auto hex_to_bin(char c)
{
    const static std::string binary[] =
    {
        "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
    };

    return binary[std::string("0123456789ABCDEF").find(c)];
}

auto read_numeric_field(std::stringstream &ss, int length)
{
    std::string temp;
    while (length--)
    {
        char c;
        if (!(ss >> c))
            return -1;
        temp.push_back(c);
    }

    return stoi(temp, nullptr, 2);
}

auto read_chars_into_string(std::stringstream &ss, int length)
{
    std::string retval;
    char c;

    while (length--)
    {
        ss >> c;
        retval.push_back(c);
    }

    return retval;
}

struct Packet
{
    enum PacketType { Sum, Product, Minimum, Maximum, Literal, GreaterThan, LessThan, EqualTo};
    int version;
    PacketType type;
    long long literal; // only applies if packet type is Literal
    std::vector<Packet *> children;

    int get_version_sum()
    {
        auto sum = version;
        for (auto c : children)
            sum += c->get_version_sum();
        return sum;
    }

    long long evaluate()
    {
        auto sum = 0LL, product = 1LL, minimum = LLONG_MAX, maximum = LLONG_MIN;

        switch (type)
        {
        case Sum:
            for (auto c : children)
                sum += c->evaluate();
            return sum;

        case Product:
            for (auto c : children)
                product *= c->evaluate();
            return product;

        case Minimum:
            for (auto c : children)
            {
                auto temp = c->evaluate();
                minimum = std::min(temp, minimum);
            }
            return minimum;

        case Maximum:
            for (auto c : children)
            {
                auto temp = c->evaluate();
                maximum = std::max(temp, maximum);
            }
            return maximum;

        case Literal:
            return literal;

        case GreaterThan:
            return children[0]->evaluate() > children[1]->evaluate() ? 1 : 0;

        case LessThan:
            return children[0]->evaluate() < children[1]->evaluate() ? 1 : 0;

        case EqualTo:
            return children[0]->evaluate() == children[1]->evaluate() ? 1 : 0;

        default:
            return -1;
        }
    }
};

std::vector<Packet *> read_packets(std::stringstream &ss, bool read_to_end = false)
{
    std::vector<Packet *> retval;

    for (;;)
    {
        auto version = read_numeric_field(ss, 3);
        if (version == -1)
            break;

        Packet *p = new Packet();
        p->version = version;
        p->type = Packet::PacketType(read_numeric_field(ss, 3));

        if (p->type == Packet::Literal)
        {
            auto flag = '1';
            p->literal = 0;

            while (flag == '1')
            {
                ss >> flag;
                auto four_bits = read_numeric_field(ss, 4);
                p->literal = (p->literal << 4) + four_bits;
            }
        }
        else
        {
            char len_type_id;
            ss >> len_type_id;

            if (len_type_id == '0')
            {
                auto sub_packets_length = read_numeric_field(ss, 15);
                auto subpackets = read_chars_into_string(ss, sub_packets_length);

                std::stringstream subss(subpackets);
                p->children = read_packets(subss, true);
            }
            else
            {
                auto num_sub_packets = read_numeric_field(ss, 11);
                for (int i = 0; i < num_sub_packets; i++)
                    p->children.push_back(read_packets(ss)[0]);
            }
        }

        retval.push_back(p);
        if (!read_to_end)
            break;
    }

    return retval;
}

auto process_input(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string line;
    file >> line;

    std::string binary;
    for (auto c : line)
        binary += hex_to_bin(c);

    std::stringstream ss(binary);

    Packet *p = read_packets(ss, false)[0];
    return std::make_pair(p->get_version_sum(), p->evaluate());
}

int main()
{
    auto result = process_input("input.txt");

    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 886);
    assert(result.second == 184487454837);
    return 0;
}
