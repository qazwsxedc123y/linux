#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace ns_util
{
    class FileUtil
    {
    public:
        static bool ReadFile(const std::string &file_path, std::string *out)
        {
            // 读取 file_path（一个.html文件） 中的内容  -- 打开文件
            std::ifstream in(file_path, std::ios::in);
            if(!in.is_open())
            {
                std::cerr << "open file" << file_path << "err" << std::endl;
                return false;
            }
            //读取文件内容
            std::string line;
             while(std::getline(in, line))
            {
                *out += line;
            }
            in.close();
            return true;
        }
        static bool ParseTitle(const std::string &file, std::string *title)
        {
            std::size_t begin = file.find("<title>");
            if(begin == std::string::npos)
            {
                return false;
            }

            std::size_t end = file.find("</title>");
            if(end == std::string::npos)
            {
                return false;
            }

            begin += std::string("<title>").size();
            if(begin > end)
            {
                return false;
            }
            *title = file.substr(begin, end - begin);
            return true;
        }
        
        static bool ParseContent(const std::string &file, std::string *content)
        {
            // 去标签，基于一个简易的状态机
            enum status
            {
                LABLE,   // 标签
                CONTENT  // 内容
            };
            enum status s = LABLE;  // 刚开始肯定会碰到 "<" 默认状态为 LABLE
            for(char c : file)
            {
                // 检测状态
                switch(s)
                {
                    case LABLE:
                        if(c == '>') s = CONTENT;
                        break;
                    case CONTENT:
                        if(c == '<') s = LABLE;
                        else
                        {
                            // 我们不想保留原始文件中的\n，因为我们想用\n作为html解析之后的文本的分隔符
                            if(c == '\n') c = ' ';
                            content->push_back(c);
                        }
                        break;
                    default:
                        break;
                }
            }
            return true;
        }
        static bool Parseurl(const std::string &file_path, std::string* url)
        {
            const std::string src_path = "data/input";          // 数据源的路径
            std::string url_head = "https://www.boost.org/doc/libs/1_89_0/doc/html";
            std::string url_tail = file_path.substr(src_path.size());//将data/input截取掉    
            *url = url_head + url_tail;//拼接
            return true;
        }
    };
}