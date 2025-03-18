#pragma once
#include <iostream>
#include "Protocol.hpp"

enum
{
    Div_Zero = 1,
    Mod_Zero,
    Other_Oper
};

class ServerCal
{
public:
    ServerCal()
    {
    }
    Response CalculatorHelper(const Request &req)
    {
        Response resp(0, 0);
        switch (req.op)
        {
        case '+':
            resp.result = req.x + req.y;
            break;

        case '-':
            resp.result = req.x - req.y;
            break;

        case '*':
            resp.result = req.x * req.y;
            break;

        case '/':
        {
            if (req.y == 0)
                resp.code = Div_Zero;
            else
                resp.result = req.x / req.y;
        }
        break;

        case '%':
        {
            if(req.y == 0)
                resp.code = Mod_Zero;
            else
                resp.result = req.x % req.y;
        }
        break;

        default:
            resp.code = Other_Oper;
        break;
        }
        return resp;
    }
    // 调用该函数，然后进行返回
    std::string Calculator(std::string &package)
    {
        std::string content;
        // 先解码
        // 反序列化
        // 计算
        // 序列化
        // 编码
        // 才可以返回
        bool r = Decode(package, &content); // "len"\n"10 + 20"\n
        if(!r) 
            return "";
        // "10 + 20"
        Request req;
        r = req.Deserialize(content); // "10 + 20" ->x=10 op=+ y=20
        if (!r)
            return "";
        
        content = "";
        Response resp = CalculatorHelper(req); // result=30 code=0;
        // resp.DebugPrint();

        resp.Serialize(&content);  // "30 0"
        content = Encode(content); // "len"\n"30 0

        // std::cout << content << std::endl;
        return content;
    }
    ~ServerCal()
    {
    }
};