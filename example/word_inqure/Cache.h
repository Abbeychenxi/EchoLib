#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include "TextQuery.h"

class Cache {
    friend class TextQuery;
    public:
        typedef std::unordered_map<std::string, std::set<std::string> > HashWord;
        void buildHashWord(const std::string &word, const std::string &candidate);
        std::set<std::string> searchHashWord(const std::string &word);


    private:
        HashWord hash_word;

};
