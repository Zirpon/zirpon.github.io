#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <list>
#include <algorithm>
#include <cstring>
#include <vector>
#include <functional>

#include <ctime>
#include <chrono>
#include <iomanip>

class crashObj : public std::exception {
public:
    crashObj () {};
    ~ crashObj () {
        printf("hello destructor\n"); //int a = 1/0;
    };
};

void doCrashObj () {
    printf("hello docrash\n");
    int* b = nullptr;
    *b = 9;
}

void worker () {
    crashObj test;

    printf("hello world\n");
    
    doCrashObj();

    printf("hello kitty\n");

}

void show(int v) {
    std::cout << v << ' ';
}

enum class dd {
    big,
    small
};

enum class cc {
    big,
    small,
};

int main (void)
{
    if ( '\0' == 0 ) {
        printf("EQUAL\n");
    } else {
        printf("unequal\n");
    }

/*
    try
    {
        worker();
        printf("hello try\n");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        printf("hello catch\n");
        exit(-1);
    }
    catch (...)
    {
        printf("hello anycatch\n");
        exit(-1);
    }
    
    printf("hello end\n");
    */

    /*
    int arr[10]{4,5,4,2,2,3,4,8,1,4};
    std::list<int> lb(arr, arr+10);
    std::list<int>::iterator last;
    for_each(lb.begin(), lb.end(), show);
    std::cout << std::endl;

    last = std::remove(lb.begin(), lb.end(), 5);
    for_each(lb.begin(), lb.end(), show);
    std::cout << std::endl << "ddd " << *last << " " << *(lb.end()) << std::endl;

    lb.erase(last, lb.end());
    for_each(lb.begin(), lb.end(), show);
    std::cout << std::endl;

    */

    /*
    xxxxx-xxxx=33333
    std::function<void (int,int,std::vector<int>&,std::vector<int>&)> dfs = 
    [=,&dfs](int ori_end, int sub_index, std::vector<int>& arr_num, std::vector<int>& substractor)->void {
        for (int j = 0; j <= ori_end; j++)
        {
            substractor[sub_index] = arr_num[j];
            arr_num[j] = arr_num[ori_end];
            arr_num[ori_end] = substractor[sub_index];
            //ori_end--;
            //sub_index++;

            if (ori_end == 0)
            {
                int sub_head = 0;
                for (int m = 0; m < 5; m++)
                {
                    sub_head = sub_head*10+substractor[m];
                }

                int sub_end = 0;
                for (int n = 5; n < 9; n++)
                {
                    sub_end = sub_end*10+substractor[n];
                }

                if (sub_head - sub_end == 33333)
                {
                    std::cout << sub_head << "-" << sub_end << "=33333" << std::endl;
                    return;
                }

                //std::cout << sub_head << "-" << sub_end << "!!!!=33333" << std::endl;

                return;
            } else {
                dfs(ori_end-1, sub_index+1, arr_num, substractor);
            }

            arr_num[ori_end] = arr_num[j];
            arr_num[j] = substractor[sub_index];
        }
    };

    
    //c++98
    clock_t start = clock();
    dfs(ori_end,sub_index, stat_arr_num, substractor);
    clock_t end = clock();
    std::cout << "花费了" << (double)(end - start) / CLOCKS_PER_SEC << "秒" << std::endl;
    

    //C++11
    //using namespace std::chrono;

    std::vector<int> stat_arr_num{1,2,3,4,5,6,7,8,9};
    //c++11: 初始化 9个0
    std::vector<int> substractor(9,0);
    int sub_index = 0;
    int ori_end = 8;
    dfs(ori_end,sub_index, stat_arr_num, substractor);

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout <<  "花费了" 
        << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den 
        << "秒" << std::endl;
    */

    /*
    std::vector<std::vector<int>> grid{
        { +0, -1, -2, -2, -2, -2, -1, -2}, 
        { -1, -2, -1, -2, -2, -1, +0, -1}, 
        { +0, -1, +0, -1, -1, +0, -1, +0}, 
        { -1, -2, -1, +0, -1, -1, -2, -1}, 
        { -1, -2, -2, -1, -2, -2, -2, -2}, 
        { +0, -1, -2, -2, -2, -2, -1, -2}, 
        { -1, -1, -2, -1, -2, -1, +0, -1}, 
        { -1, +0, -1, +0, -1, -2, -1, +0}, 
    };

    std::vector<std::vector<int>> coordinate;

    std::function<void (void)> dump = [=,&grid,&coordinate](void)->void {        
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (grid[i][j] <= -1)
                {
                    std::cout << "-\t";   
                } else {
                    std::cout << grid[i][j] << "\t";
                }
            }

            std::cout << std::endl;   
        }        
    };

    clock_t start = clock();
    std::vector<int> pick_rowroomnums(8,0);
    const std::vector<int> const_rowroomnums{1,2,1,2,1,1,2,2};
    std::vector<int> pick_columnroomnums(8,0);
    const std::vector<int> const_columnroomnums{3,1,2,1,1,1,1,2};
    
    std::function<void (int)> dfs = 
    [=,&dfs,&grid,&start,&coordinate,&pick_rowroomnums,&pick_columnroomnums](int step)->void {

        if (coordinate.size() == 12)//(step == 12)
        {
            clock_t end = clock();
            double frame = (double)(end - start) / CLOCKS_PER_SEC;

            std::cout <<  "花费了" 
            << frame
            << "秒" << std::endl;
            dump();
            exit(0);
        }
        
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (grid[i][j] == -1)
                {
                    auto checkdot = [&grid,&i,&j](int x, int y)->bool {
                        if (i+x < 0 || j+y < 0 || i+x > 7 || j+y > 7)
                        {
                            return true;
                        }

                        int* dot1 = &grid[i+x][j+y];
                        
                        if (*dot1 == 1)
                        {
                            return false;
                        }

                        return true;
                    };
                    if (checkdot(-1,-1) == false ) continue;
                    if (checkdot(-1,+1) == false ) continue;
                    if (checkdot(-1,+0) == false ) continue;

                    if (checkdot(+0,-1) == false ) continue;
                    if (checkdot(+0,+1) == false ) continue;

                    if (checkdot(+1,-1) == false ) continue;
                    if (checkdot(+1,+0) == false ) continue;
                    if (checkdot(+1,+1) == false ) continue;

                    if (pick_rowroomnums[i] == const_rowroomnums[i]) continue;
                    if (pick_columnroomnums[j] == const_columnroomnums[j]) continue;

                    grid[i][j] = 1;
                    coordinate.push_back(std::vector<int>{i,j});
                    pick_rowroomnums[i]++;
                    pick_columnroomnums[j]++;
                    dfs(step++);
                    coordinate.pop_back();
                    pick_rowroomnums[i]--;
                    pick_columnroomnums[j]--;
                    grid[i][j] = -1;
                }
            }
        }

        return;
    };
    dump();
    dfs(0);
    */

    return 0;
}