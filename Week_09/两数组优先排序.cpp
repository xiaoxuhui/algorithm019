#include  <vector>
#include <map>
using namespace std;

class Solution {
public:
    vector<int> relativeSortArray(vector<int>& arr1, vector<int>& arr2) {
        map<int,int> numCountMap;
        for(int num:arr1){
            numCountMap[num]++;
        }
        vector<int> result;
        for(int num_2:arr2){
            auto itNum = numCountMap.find(num_2);
            if(itNum == numCountMap.end()) {
                continue;
            }
            int count = itNum->second;
            for(int i = 0;i < count;++i) {
                result.push_back(itNum->first);
            }
            numCountMap.erase(itNum);
        }
        for(auto itNum = numCountMap.begin();itNum != numCountMap.end();itNum++) {
            int count = itNum->second;
            for(int i = 0;i < count;++i) {
                result.push_back(itNum->first);
            }
        }
         return result;
    }
};
