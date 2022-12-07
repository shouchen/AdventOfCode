#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>

struct File
{
    long long size;
    std::string name;
};

struct Folder
{
    std::string name;
    Folder *parent;
    std::vector<Folder> children;
    std::vector<File> files;
    long long size = 0;
};

//void Dump(Folder *folder, int indent)
//{
//    for (int i = 0; i < indent; i++)
//        std::cout << "  ";
//
//    std::cout << "-" << folder->name << " (dir)" << std::endl;
//    for (Folder &f : folder->children)
//    {
//        Dump(&f, indent + 2);
//    }
//    for (File &file : folder->files)
//    {
//        for (int i = 0; i <= indent; i++)
//            std::cout << "  ";
//        std::cout << file.name << " (file, size=" << file.size << ")" << std::endl;
//    }
//}

long long FillInFolderSizes(Folder *folder)
{
    for (Folder &f : folder->children)
        folder->size += FillInFolderSizes(&f);

    for (File &file : folder->files)
        folder->size += file.size;

    return folder->size;
}

void SumFoldersUnderLimit(Folder *folder, long long limit, long long &sum)
{
    if (folder->size <= limit)
        sum += folder->size;

    for (Folder &f : folder->children)
        SumFoldersUnderLimit(&f, limit, sum);
}

void FindSmallestFolderAtLeastAmount(Folder *folder, long long amount, long long &smallest)
{
    if (folder->size >= amount && folder->size < smallest)
        smallest = folder->size;

    for (Folder &f : folder->children)
        FindSmallestFolderAtLeastAmount(&f, amount, smallest);
}

Folder root{ "/", nullptr };
Folder *curr = &root;

auto read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        if (line[0] == '$')
        {
HERE:
            std::stringstream ss(line);
            std::string dollar, cmd, param;

            ss >> dollar >> cmd;
            if (cmd == "cd")
            {
                ss >> param;
                if (param == "/") continue;
                if (param == "..") curr = curr->parent;
                for (Folder &f : curr->children)
                    if (f.name == param)
                    {
                        curr = &f;
                        break;
                    }
            }
            else if (cmd == "ls")
            {
                while (getline(file, line))
                {
                    if (line[0] == '$') goto HERE;

                    std::stringstream ss(line);
                    std::string a;
                    std::string b;
                    ss >> a >> b;

                    if (a == "dir")
                    {
                        curr->children.push_back(Folder{ b, curr });
                    }
                    else
                    {
                        curr->files.push_back(File{ atoi(a.c_str()), b });
                    }
                }
            }
        }
    }

    //Dump(&root, 0);
    FillInFolderSizes(&root);
}

auto do_part1()
{
    long long part1 = 0;
    SumFoldersUnderLimit(&root, 100000, part1);
    return part1;
}

auto do_part2()
{
    long long total_space = 70000000;
    long long need_unused = 30000000;
    long long have_unused = total_space - root.size;
    long long size_to_free_up = need_unused - have_unused;

    auto retval = LLONG_MAX;
    FindSmallestFolderAtLeastAmount(&root, size_to_free_up, retval);
    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1086293);
    assert(part2 == 366028);
    return 0;
}
