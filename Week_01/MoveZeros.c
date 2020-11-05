void moveZeroes(int *nums, int numsSize)
{
    int nozeroCount = 0;
    // 将非0值全部移至头部
    for (int i = 0; i < numsSize; ++i)
    {
        if (nums[i] != 0)
        {
            nums[nozeroCount] = nums[i];
            // if (i != nozeroCount)
            // {
            //     nums[i] = 0;
            // }
            nozeroCount++;
        }
    }
    // 尾部全部填充为0
    memset(nums + nozeroCount, 0, (numsSize - nozeroCount) * sizeof(int));
}
