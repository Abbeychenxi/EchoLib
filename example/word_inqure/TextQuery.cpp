#include "TextQuery.h"
#include <stdio.h>
#include <echo/Exception.h>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <string.h>
#include <string>
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
            word.erase(new_end, word.end());
            Dic[word]++;
        }
    }

}

string TextQuery::runQuery(const string &word)
{
    string res,tmp;
    auto a = hash_word.find(word);

    if(a != hash_word.end()) {
        set<string> candidate;
        candidate = searchHashWord(word);
        for(auto index = candidate.begin(); index != candidate.end(); ++index) {
            tmp = *index;
            tmp += "\r\n";
            res += tmp;
        }
    }else {
        while(!candidate_.empty()) {
            candidate_.pop();
        }
        for(map<string, int>::iterator it = Dic.begin(); it != Dic.end(); ++it) {
            int res = editDistance(word.c_str(), (it->first).c_str());
            if(res < 2) {
                candidate cand;
                cand.word_ = it->first;
                cand.distance_ = res;
                cand.frequence_ = it->second;
                candidate_.push(cand);
            }
        }
        for(int i = 0; i < 3; ++i) {
            if(candidate_.empty()) {
                break;
            }
            tmp = candidate_.top().word_;
            buildHashWord(word, tmp);
            tmp += "\r\n";
            res += tmp;
            candidate_.pop();
        }
    }

    return res;
}

int TextQuery::editDistance(const char* word, const char* des_word) {
    int memo[100][100];
    int index = strlen(word);
    int des_index = strlen(des_word);
    for(int i = 0; i < index + 1; ++i) {
        memo[i][0] = i;
    }
    for(int i = 0; i < des_index + 1; i++) {
        memo[0][i] = i;
    }
    for(int i = 1; i <= index; ++i) {
        for(int j = 1; j <= des_index; ++j) {
            if(word[i -1] == des_word[j-1]) {
                memo[i][j] = memo[i-1][j-1];
            }else {
                memo[i][j] = (memo[i-1][j] < memo[i][j-1] ? (memo[i-1][j]< memo[i-1][j-1]? memo[i-1][j] : memo[i-1][j-1]) : (memo[i][j-1] < memo[i-1][j-1] ? memo[i][j-1] : memo[i-1][j-1]))+1;
            }
        }
    }
    return memo[index][des_index];
}

void TextQuery::buildHashWord(const string &word, const string &candidate) {
    hash_word[word].insert(candidate);
}

set<string> TextQuery::searchHashWord(const string &word) {
    auto a = hash_word.find(word);
    set<string> candidates;
    if(a != hash_word.end()) {
        candidates = a->second;
    }
    return candidates;
}




