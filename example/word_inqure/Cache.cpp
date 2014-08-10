#include "Cache.h"

using namespace std;

void Cache::buildHashWord(const string &word, const string &candidate) {
    hash_word[word].insert(candidate);
}

set<string> Cache::searchHashWord(const string &word) {
    auto a = hash_word.find(word);
    set<string> candidates;
    candidates = a->second;
    return candidates;
}

int main(int argc, const char *argv[])
{
    Cache ch;
    ch.buildHashWord("zhangsan", "zhangsa");
    set<string> candidates;
    candidates = ch.searchHashWord("zhangsan");
    for(set<string>::iterator it = candidates.begin(); it != candidates.end(); ++it)
    {
        cout << *it << endl;
    }

    return 0;
}
