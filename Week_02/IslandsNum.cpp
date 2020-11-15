#include <vector>
using namespace std;

// 分配足够的空间
static char szNums[312][312] = {0};

class Solution {
private:
    // 扫描一个岛屿
    void scanAIsland(int r, int c) {
        if(szNums[r][c] != '1'){// 扫描终止条件，由于在外面分配了一圈'0',故不再需要进行边界判断
            return;
        }
        szNums[r][c] = '2';//标志为已经扫描
        scanAIsland(r - 1, c);//往上扫描
        scanAIsland(r + 1, c);//往下扫描
        scanAIsland(r, c - 1);//往左扫描
        scanAIsland(r, c + 1);//往右扫描
    }

public:
    int numIslands(vector<vector<char>>& grid) {
        int rowNum = grid.size();
        int colNum = grid[0].size();
        // 将图拷贝到[(1,1)rowNum x colNum]区域，即在外围包一圈'0'，以减少后续的边界判断
        // 实际工程代码中，会预分配好多的代码空间，在输入时已经处理过边界一圈的初始，此处性能可节省
        for(int row = 0;row < rowNum; ++row) {
            for (int col = 0;col < colNum; ++col) {
                szNums[row+1][col+1]=grid[row][col];
            }
        }

        int islandNum = 0;// 岛屿数量
        for (int r = 1; r < rowNum + 1; ++r) {
            for (int c = 1; c < colNum + 1; ++c) {
                if (szNums[r][c] == '1') {// 找到一个岛屿
                    ++islandNum;
                    scanAIsland(r, c);
                }
            }
        }

        return islandNum;
    }
};
