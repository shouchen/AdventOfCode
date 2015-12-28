// Password.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

typedef char Password[8];

bool HasIncreasingStraight(Password password)
{
    for (int i = 0; i <= sizeof(Password) - 3; i++)
        if ((password[i + 1] == password[i] + 1) && (password[i + 2] == password[i] + 2))
            return true;

    return false;
}

bool HasForbiddenLetters(Password password)
{
    for (int i = 0; i < sizeof(Password); i++)
        if (password[i] == 'i' || password[i] == 'o' || password[i] == 'l')
            return true;

    return false;
}

bool HasAtLeastTwoLetterPairs(Password password)
{
    int numLetterPairs = 0;

    for (int i = 0; i < sizeof(Password) - 1; i++)
    {
        if (password[i] == password[i + 1])
        {
            if (++numLetterPairs == 2)
                return true;

            i++;
        }
    }

    return false;
}

bool IsValidPassword(Password password)
{
    return 
        HasIncreasingStraight(password) &&
        !HasForbiddenLetters(password) &&
        HasAtLeastTwoLetterPairs(password);
}

void IncrementPassword(Password password)
{
    for (auto place = &password[sizeof(Password) - 1]; place >= password; --place)
    {
        if (*place < 'z')
        {
            (*place)++;
            break;
        }

        *place = 'a';
    }
}

const char *FindNextPassword(Password password)
{
    do
    {
        IncrementPassword(password);
    } while (!IsValidPassword(password));

    return password;
}

void _tmain(int argc, _TCHAR *argv[])
{
    char password[] { "hxbxwxba" };

    std::cout << "part 1: " << FindNextPassword(password) << std::endl;
    std::cout << "part 2: " << FindNextPassword(password) << std::endl;
}
