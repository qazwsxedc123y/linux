#pragma once

#include <iostream>
#include <string>
// 规定报头协议：
// "len"\n"x op y"\n
// "len"\n"result code"\n

const std::string blank_space_sep = " ";
const std::string protocol_sep = "\n";

std::string Encode(std::string &content) // 编码
{

}

bool Decode(std::string &package, std::string *content) // 解码
{

}

class Request
{
public:
    bool Serialize(std::string *out) // 序列化
    {

    }
    bool Deserialize(const std::string &in)    // 反序列化
    {
        // "x op y"

    }
public:
    // x op y
    int x;
    int y;
    char op; // + - * / %
};


class Response
{
public:
    bool Serialize(std::string *out) // 序列化
    {

    }
    bool Deserialize(const std::string &in)    // 反序列化
    {
        // "result code"

    }
public:
    int result;
    int code; // 0，可信，否则!0具体是几，表明对应的错误原因
};