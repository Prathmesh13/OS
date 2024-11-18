#include <iostream>
#include <thread>

using namespace std;

const int MAX = 4;
const int MAX_THREAD = 4;

int matAdd[MAX][MAX];
int matSub[MAX][MAX];
int stepAdd = 0; int stepSub = 0;

void printMatrix(int mat[][MAX])
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void addition(int matA[][MAX], int matB[][MAX])
{
    int index = stepAdd++;

    int start = index * (MAX/4);
    int end = (index+1) * (MAX/4);

    for(int i=start; i<end; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            matAdd[i][j] = matA[i][j] + matB[i][j];
        }
    }
}

void subtraction(int matA[][MAX], int matB[][MAX])
{
    int index = stepSub++;

    int start = index * (MAX/4);
    int end = (index+1) * (MAX/4);

    for(int i=start; i<end; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            matSub[i][j] = matB[i][j] - matA[i][j];
        }
    }
}

int main()
{
    int matA[MAX][MAX] = {{19,27,36,49},{54,66,73,82},{99,110,112,126},{163,143,154,163}};
    int matB[MAX][MAX] = {{22,11,23,42},{55,63,72,81},{90,101,121,122},{123,164,135,166}};

    printMatrix(matA);
    printMatrix(matB);

    thread add_thread[MAX_THREAD];
    thread sub_thread[MAX_THREAD];

    for (int i=0; i<MAX_THREAD; i++)
    {
       add_thread[i] = thread(addition,matA,matB); 
       sub_thread[i] = thread(subtraction,matA,matB); 
    }

    for(int i=0; i<MAX_THREAD; i++)
    {
        add_thread[i].join();
        sub_thread[i].join();
    }

    cout << "Addition of matrix A and B: " << endl;
    printMatrix(matAdd);
    cout << "Subtraction of matrix A and B: " << endl;
    printMatrix(matSub);

    return 0;
}