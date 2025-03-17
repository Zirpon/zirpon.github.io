#include <iostream>
#include <string>
#define NEXT_SIZE 100
using namespace std;

void getNext();

void nextPrintf();

int kmp();

void getNext(string s, int *next)
{
    next[0] = -1;
    for (int k = 0; k < s.size(); k++)
    {
        int j = k;
        next[k + 1] = 0;
        for (int i = 0; i < k; i++)
        {
            //   cout << s.substr(0,i + 1) << "===";
            //   cout <<  s.substr(j,i+1)  << "\n";
            if (s.substr(0, i + 1) == s.substr(j, i + 1))
            {
                next[k + 1] = i + 1;
            }
            j--;
        }
    }
}
int kmp(string search, string inputString, int *next)
{
    int i = 0;
    int j = 0;
    int searchIndex = 0;
    int searchSize = search.size();
    int inputSize = inputString.size();
    while (j < searchSize || i == inputSize - 1)
    {
        if (j == -1 || inputString[i] == search[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    if (j >= searchSize)
    {
        searchIndex = i - searchSize;
    }
    cout << searchIndex;
    return searchIndex;
}
void nextPrintf(int *next, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << next[i];
    }
}
int main()
{
    string m = "ABA";
    string s = "ABCDDDEABAJJJJ";
    int next[NEXT_SIZE];
    getNext(m, next);
    // nextPrintf(next,m.size());
    kmp(m, s, next);
}