#ifndef TEXT_QUERY_H
#define TEXT_QUERY_H 

#include <string>
#include <set>
#include <map>
#include <vector>


class TextQuery
{
    public:
        typedef std::vector<std::string>::size_type LineNo;
    
        TextQuery(const std::string &filename);
        void readFile(const std::string &filename);
        std::string runQuery(const std::string &word) const;
        std::string printResult(const std::string &word, const std::set<LineNo> &results) const;

    private:
        std::vector<std::string> lines_; //存放每行的内容
        std::map<std::string, std::set<LineNo> > wordMap_;  //单词以及他们出现的行号;


};



#endif  /*TEXT_QUERY_H*/
