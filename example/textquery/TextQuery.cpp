#include "TextQuery.h"
#include <stdio.h>
#include <echo/Exception.h>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
using namespace std;

TextQuery::TextQuery(const string &filename)
{
    readFile(filename);
}

void TextQuery::readFile(const string &filename)
{
    ifstream in;
    in.open(filename.c_str());
    if(!in)
        throw Exception("open file fail");
    string line;
    while(getline(in, line))
    {
        lines_.push_back(line);        
    }
    in.close();

    for(vector<string>::size_type ix = 0;
            ix != lines_.size(); ++ix)
    {
        istringstream iss(lines_[ix]);
        string word;
        while(iss >> word)
        {
            for(string::iterator it = word.begin(); it != word.end(); ++it) {
                if (isupper(*it))
                    tolower(*it);
                if (ispunct(*it))
                    *it = ' ';
            }
            string::iterator new_end = remove_if(word.begin(), word.end(), bind2nd(equal_to<char>(), ' '));
            wordMap_[word].insert(ix);
        }
    }

}

string TextQuery::runQuery(const string &word) const
{
    map<string, set<LineNo> >::const_iterator it;
    it = wordMap_.find(word);
    if(it == wordMap_.end())
        return printResult(word, set<LineNo>());
    else
        return printResult(word, it->second);
}

string TextQuery::printResult(const string &word, const set<LineNo> &results) const
{
    string ret;
    size_t cnt = results.size();
    char buf[16];
    snprintf(buf, sizeof buf, "%u", cnt);
    ret = word + " occurs " + buf + " times\n";

    for(set<LineNo>::iterator it = results.begin();
            it != results.end(); ++it)
    {
        snprintf(buf, sizeof buf, "%u", *it + 1);
        ret += string("line ") + buf + ") ";
        ret += lines_[*it] + "\n";
    }

    return ret;
}







