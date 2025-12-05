#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include "util.hpp"

namespace ns_index
{
    struct DocInfo // 正排索引结构体  表示文档信息
    {
        std::string title;    // 文档标题
        std::string content;  // 文档内容
        std::string url;      // 文档的url
        uint64_t doc_id;      //文档的ID
    };

    // 一个【关键字】可能出现在 无数个 【文档】中 ，我们需要根据权重判断 文档的重要顺序
    // 该单个结构体对象标识只对应一个倒排索引
    struct InvertedElem // 倒排对应的文档结构信息
    {
        std::string word;     // 关键字（通过关键字可以找到对应的ID）
        uint64_t doc_id;      // 文档ID
        int weight;           // 权重（用于排序）
    };

    // 倒排拉链  -- 一个关键字 可能存在于多个文档中，所以一个关键字对应了一组文档
    typedef std::vector<InvertedElem> InvertedList;

    class Index
    {
    private:
        // 正排索引采用数组的形式，数组的下标就是文档的ID
        std::vector<DocInfo> forward_index;

        // 一个关键字和一组（或者一个）InvertedElem对应，关键字和倒排拉链的映射关系
        std::unordered_map<std::string, InvertedList> inverted_index;
    
    public:
        Index()
        {}
        ~Index()
        {}

    public:
        // 根据doc_id找到正排索引对应doc_id的文档信息
        DocInfo* GetForwardIndex(uint64_t doc_id)
        {
            // 如果这个doc_id已经大于正排索引的元素个数，则索引失败
            if(doc_id > forward_index.size())
            {
                std::cerr << "doc_id out range, error!" << std::endl;
                return nullptr;
            }
            // 否则返回相对应下标的文档信息
            return &forward_index[doc_id];
        }
        
        // 根据倒排索引的关键字word，获得倒排拉链
        InvertedList* GetInvertedList(const std::string &word)
        {
            // word关键字不是在 unordered_map 中，直接去里面找对应的倒排拉链即可
            auto iter = inverted_index.find(word);
            // 不在则返回空
            if(iter == inverted_index.end())
            {
                std::cerr << "have no InvertedList" << std::endl;
                return nullptr;
            }
            // 否则返回 unordered_map 中的第二个元素--- 倒排拉链
            return &(iter->second);
        }
        
        // 根据去标签，格式化后的文档，构建正排和倒排索引                                                                                                              
        // 将数据源的路径：data/raw_html/raw.txt传给input即可，这个函数用来构建索引
        bool BuildIndex(const std::string &input)
        {
            // 先打开文件
            std::ifstream in(input, std::ios::in | std::ios::binary);
            if(!in.is_open())
            {
                std::cerr << input << "open err" << std::endl;
                return false;
            }

            int count = 0;
            std::string line;
            while(getline(in,line))
            {
                // 根据读取的一个文档信息构建索引
                
                // 先构建正排索引
                DocInfo* doc = BuildForwardIndex(line);//构建正排索引
                if(doc == nullptr)
                {
                    std::cerr << "build " << line << " error" << std::endl;
                    continue;
                }
                // 再构建倒排索引
                BuildInvertedIndex(*doc);//有了正排索引才能构建倒排索引

                count++;    
                if(count % 50 == 0)    
                {    
                    std::cout << "当前已经建立的索引文档：" << count << "个" << std::endl;     
                }
            } 
            return true;
        }

    private:
        // 构建正排索引 将拿到的一行html文件传输进来（其也就是单个文档信息），进行解析
        // 大致：文档信息填充一个DocInfo结构体变量中，然后将其插入正排索引的vector中即可
        DocInfo* BuildForwardIndex(const std::string &line)
        {
            // 先进行解析line -- 进行字符串切分
            // 切分为三部分 string(title, content, url)
            std::vector<std::string> results; // 将切分后结果存放到此
            std::string sep = "\3"; //行内分隔符
            ns_util::StringUtil::Splist(line, &results, sep);//字符串切分 
            if(results.size() != 3)
            {
                std::cerr << "build forward index --> Splist" << std::endl;
                return nullptr;
            }
            // 切分完后将其填充到DocInfo类型的变量中
            
            // 将其变量插入到正排索引的vector中
        }

        bool BuildInvertedIndex(const DocInfo &doc)
        {

        }
    };
}