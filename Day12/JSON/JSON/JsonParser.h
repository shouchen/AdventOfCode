#pragma once

#include <map>
#include <vector>
#include <cctype>

class JsonObject;
class JsonString;
class JsonNumber;
class JsonArray;
class JsonValue;

class JsonObject
{
public:
    std::map<std::string, JsonValue *> nameValue;
};

class JsonString
{
public:
    JsonString(std::string value) : m_value(value) {}

    std::string m_value;
};

class JsonNumber
{
public:
    JsonNumber(int value) : m_value(value) {}

    int m_value;
};

class JsonArray
{
public:
    void AppendValue(JsonValue *value) { values.push_back(value); }

    std::vector<JsonValue *> values;
};

class JsonValue
{
public:
    enum class Type
    {
        String, Number, Object, Array, True, False, Null
    };

    static JsonValue *CreateStringValue(JsonString *value)
    {
        return new JsonValue{ Type::String, value, nullptr, nullptr, nullptr };
    }

    static JsonValue *CreateNumberValue(JsonNumber *number)
    {
        return new JsonValue{ Type::Number, nullptr, number, nullptr, nullptr };
    }

    static JsonValue *CreateObjectValue(JsonObject *object)
    {
        return new JsonValue{ Type::Object, nullptr, nullptr, object, nullptr };
    }

    static JsonValue *CreateArrayValue(JsonArray *array)
    {
        return new JsonValue{ Type::Array, nullptr, nullptr, nullptr, array };
    }

    static JsonValue *CreateTrueValue()
    {
        return new JsonValue{ Type::True, nullptr, nullptr, nullptr, nullptr };
    }

    static JsonValue *CreateFalseValue()
    {
        return new JsonValue{ Type::False, nullptr, nullptr, nullptr, nullptr };
    }

    static JsonValue *CreateNullValue()
    {
        return new JsonValue{ Type::Null, nullptr, nullptr, nullptr, nullptr };
    }

    Type m_type;
    JsonString *m_string;
    JsonNumber *m_number;
    JsonObject *m_object;
    JsonArray *m_array;
};

JsonObject *ParseObject(const char *&input);
JsonArray *ParseArray(const char *&input);
JsonValue *ParseValue(const char *&input);
JsonString *ParseString(const char *&input);
JsonNumber *ParseNumber(const char *&input);
