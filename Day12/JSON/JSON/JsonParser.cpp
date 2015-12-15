#include "stdafx.h"
#include <map>
#include <vector>
#include <cctype>
#include "JsonParser.h"

/*
- [1, 2, 3] and{ "a":2, "b" : 4 } both have a sum of 6.
- [[[3]]] and{ "a":{"b":4}, "c" : -1 } both have a sum of 3.
- {"a":[-1, 1]} and[-1, { "a":1 }] both have a sum of 0.
- [] and{} both have a sum of 0.

object, array, value, string, number
http://json.org/
*/

JsonValue *ParseValue(const char *&input)
{
    auto curr = input;

    while (*curr == ' ') curr++;

    if (*curr == '"')
    {
        JsonString *str = ParseString(curr);
        if (!str) return nullptr;
        input = curr;
        return JsonValue::CreateStringValue(str);
    }
    else if (isdigit(*curr) || *curr == '-')
    {
        JsonNumber *number = ParseNumber(curr);
        if (!number) return nullptr;
        input = curr;
        return JsonValue::CreateNumberValue(number);
    }
    else if (*curr == '{')
    {
        JsonObject *object = ParseObject(curr);
        if (!object) return nullptr;
        input = curr;
        return JsonValue::CreateObjectValue(object);
    }
    else if (*curr == '[')
    {
        JsonArray *array = ParseArray(curr);
        if (!array) return nullptr;
        input = curr;
        return JsonValue::CreateArrayValue(array);
    }
    else if (strcmp(curr, "true") == 0)
    {
        curr += 4;
        input = curr;
        return JsonValue::CreateTrueValue();
    }
    else if (strcmp(curr, "false") == 0)
    {
        curr += 5;
        input = curr;
        return JsonValue::CreateFalseValue();
    }
    else if (strcmp(curr, "null") == 0)
    {
        curr += 4;
        input = curr;
        return JsonValue::CreateNullValue();
    }
    else
    {
        return nullptr;
    }
}

JsonString *ParseString(const char *&input)
{
    std::string value;

    input++; // skip open quote

    while (*input != '"')
    {
        if (*input == '\\')
        {
            ++input;
            switch (*input)
            {
            case '"':
                value += '"';
                break;
            case '\\':
                value += '\\';
                break;
            case '/':
                value += '/';
                break;
            case 'b':
                value += '\b';
                break;
            case 'f':
                value += '\f';
                break;
            case 'n':
                value += '\n';
                break;
            case 'r':
                value += '\r';
                break;
            case 't':
                value += '\t';
                break;
            case 'u':
                input += 3; // TODO: parse
                value += "\\u0000"; // TODO: use parsed value
                break;
            }
        }
        else
        {
            value += *input;
        }

        input++;
    }

    return new JsonString(value);
}

JsonNumber *ParseNumber(const char *&input)
{
    auto curr = input;

    int value = 0;
    if (sscanf_s(curr, "%d", &value) == EOF)
        return nullptr;

    // skip what we just scanned
    while (isdigit(*curr) || *curr == '-')
        curr++;

    input = curr;
    return new JsonNumber(value);


    /*
    bool isNegative = false;
    std::string value;

    if (*curr == '-')
    {
    isNegative = true;
    curr++;
    }

    if (*curr >= '1' && *curr <= '9')
    {
    value += curr++;
    while (isdigit(*curr++))
    value += curr;
    }
    else if (*curr != 0)
    {
    return nullptr;
    }

    if (*curr == '.')
    {
    value += *curr++;
    while (isdigit(*curr++))
    value += curr;
    }

    if (*curr == 'e' || *curr == 'E')
    {
    curr++;

    bool isExponentNegative = false;
    if (*curr == '-')
    {
    curr++;
    isExponentNegative = true;
    }
    else if (*curr == '+')
    {
    curr++;
    }

    while (isdigit(*curr++))
    value += curr;
    }
    */

}

JsonObject *ParseObject(const char *&input)
{
    auto curr = input;

    JsonObject *obj = new JsonObject();

    if (*curr++ != '{')
        return nullptr;

    while (*curr != '}')
    {
        JsonString *name = ParseString(curr);
        if (!name) return nullptr;

        if (*curr++ != ':')
            return nullptr;

        JsonValue *value = ParseValue(curr);
        if (!value) return nullptr;

        obj->nameValue[name->m_value] = value;

        if (*curr == ',')
            curr++;
    }

    curr++;
    return obj;
}

JsonArray *ParseArray(const char *&input)
{
    auto curr = input;

    JsonArray *arr = new JsonArray();

    if (*curr++ != '[')
        return nullptr;

    while (*curr != ']')
    {
        JsonValue *value = ParseValue(curr);
        if (!value) return nullptr;

        arr->values.push_back(value);

        if (*curr == ',')
            curr++;
    }

    curr++;
    input = curr;
    return arr;
}
