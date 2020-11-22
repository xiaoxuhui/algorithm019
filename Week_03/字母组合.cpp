#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> combinations;
        if (digits.empty()) {
            return combinations;
        }
        unordered_map<char, string> phoneMap{
            {'2', "abc"},
            {'3', "def"},
            {'4', "ghi"},
            {'5', "jkl"},
            {'6', "mno"},
            {'7', "pqrs"},
            {'8', "tuv"},
            {'9', "wxyz"}
        };
        string combination;
        backtrack(combinations, phoneMap, digits, 0, combination);
        return combinations;
    }

    void backtrack(vector<string>& combinations, const unordered_map<char, string>& phoneMap, const string& digits, int index, string& combination) {
        if (index == digits.length()) {
            combinations.push_back(combination);
        } else {
            char digit = digits[index];
            const string& letters = phoneMap.at(digit);
            for (const char& letter: letters) {
                combination.push_back(letter);
                backtrack(combinations, phoneMap, digits, index + 1, combination);
                combination.pop_back();
            }
        }
    }
};

// class Solution {
// public:
//     vector<string> letterCombinations(string digits) {
//         vector<string> resultList;
//         if(digits.empty()) {
//             return resultList;
//         }

//         map<char, string> phoneMap{
//             {'2',"abc"},
//             {'3',"def"},
//             {'4',"ghi"},
//             {'5',"jkl"},
//             {'6',"mno"},
//             {'7',"pqrs"},
//             {'8',"tuv"},
//             {'9',"wxyz"}
//         };
//         string result;
//         backtrack(resultList,phoneMap,digits,0,result);
//         return resultList;
//     }

//     void backtrack(vector<string> &resultList, const map<char, string> &phoneMap, const string &digits, int pos, string &result){
//         if(pos == digits.length()) {
//             resultList.push_back(result);
//             return;
//         }
//         const string &letters = phoneMap.at(digits[pos]);
//         for ( const char & letter: letters) {
//             result.push_back(letter);
//             backtrack(resultList,phoneMap,digits,pos+1,result);
//             result.pop_back();
//         }
//     }
// };
