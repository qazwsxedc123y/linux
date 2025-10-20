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
    // 将原有的有效载荷变为报文   
    // 报文格式：
    // "len"\n"有效载荷"\n
    std::string package = std::to_string(content.size());
    package += protocol_sep;
    package += content;
    package += protocol_sep;
    
    return package;
}

bool Decode(std::string &package, std::string *content) // 解码
{
    // "len"\n"有效载荷"\n
    // 将报文的有效载荷提取   package 提取 存到 content
    std::size_t pos = package.find(protocol_sep);
    if(pos == package.npos) return false;

    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str);
    
    std::size_t total_len =  len_str.size() + len + 2;
    if(package.size() < total_len) return false;

    *content = package.substr(pos + 1, len);

    // earse 移除报文 package.erase(0, total_len);
    package.erase(0, total_len);

    return true;
}

class Request
{
public:
    Request()
    {

    }
    Request(int data1, int data2, char oper) :x(data1) ,y(data2) ,op(oper)
    {}
    bool Serialize(std::string *out) // 序列化
    {
        // 序列化：构建报文的有效载荷
        // x op y
        std::string s = std::to_string(x);
        s += blank_space_sep;
        s += op;
        s += blank_space_sep;
        s += std::to_string(y);
        *out = s;
        return true;

    }
    bool Deserialize(const std::string &in)    // 反序列化
    {
        // 将有效载荷 "x op y" 赋值到成员变量中
        std::size_t left = in.find(blank_space_sep);
        if(left == in.npos) return false;
        std::string part_x = in.substr(0, left); // 左开  右闭

        std::size_t right = in.rfind(blank_space_sep);
        if(right == in.npos) return false;
        std::string part_y = in.substr(right+1);

        if (left + 2 != right) return false;

        op = in[left + 1];
        x = std::stoi(part_x);
        y = std::stoi(part_y);

        return true;
    }

    void DebugPrint()
    {
        std::cout << "新请求构建完成:  " << x << op << y << "=?" << std::endl;
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
    Response()
    {}
    Response(int res, int c) : result(res), code(c)
    {}
    bool Serialize(std::string *out) // 序列化
    {
        // 序列化：构建报文的有效载荷
        // "result code"
        std::string s = std::to_string(result);
        s += blank_space_sep;
        s += std::to_string(code);

        *out = s;
        return true;
    }
    bool Deserialize(const std::string &in)    // 反序列化
    {
        // 将有效载荷 "result code" 赋值到成员变量中
        std::size_t pos = in.find(blank_space_sep);
        if(pos == in.npos) return false;

        std::string part_result = in.substr(0, pos);
        std::string part_code = in.substr(pos + 1);

        result = std::stoi(part_result);
        code = std::stoi(part_code);

        return true;
    }

    void DebugPrint()
    {
        std::cout << "结果响应完成, result: " << result << ", code: "<< code << std::endl;
    }
public:
    int result;
    int code; // 0，可信，否则!0具体是几，表明对应的错误原因
};