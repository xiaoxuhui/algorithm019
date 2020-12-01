#include <vector>
#include <set>
using namespace std;

class Solution {
public:
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        set<pair<int,int>> obstacleSet;
        for(vector<int> &obstacle : obstacles){
            obstacleSet.insert(make_pair(obstacle[0],obstacle[1]));
        }
        int x = 0;
        int y = 0;
        int sum = 0;
        int directionX[] = {0,1,0,-1};
        int directionY[] = {1,0,-1,0};
        int direction_I = 0;
        for(int command : commands) {
            switch(command) {
                case -1://  右转90度
                    direction_I = (direction_I + 1) % 4;
                    break;
                case -2://  左转90度
                    direction_I = (direction_I + 4 - 1) % 4;
                    break;
                default:
                    for(int i = 0;i < command;++i) {
                        int nX = x + directionX[direction_I];
                        int nY = y + directionY[direction_I];
                        if(obstacleSet.find(make_pair(nX,nY)) == obstacleSet.end()) {
                            x = nX;
                            y = nY;                            
                        }
                    }
                    sum = max(x * x + y * y,sum);
                    break;
            }
        }
        return sum;
    }
};
