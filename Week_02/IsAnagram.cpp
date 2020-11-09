#include <string>
using namespace std;

class Solution
{
public:
    bool isAnagram(string s, string t)
    {
        if (s.length() != t.length())
        {
            return false;
        }
        const char *szS = s.c_str();
        const char *szT = t.c_str();
        int zeroCount[26] = {0};
        int difCount[26] = {0};
        for (int i = 0; i < s.length(); ++i)
        {
            difCount[szS[i] - 'a']++;
            difCount[szT[i] - 'a']--;
        }
        return (memcmp(zeroCount, difCount, 26 * sizeof(int)) == 0);
        // sort(s.begin(),s.end());
        // sort(t.begin(),t.end());
        // return (s==t);
    }
};
