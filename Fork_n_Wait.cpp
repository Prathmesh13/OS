#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void printArray(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void bubbleSort(int arr[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selectionSort(int arr[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for(int j = i + 1; j < n; j++)
        {
            if(arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        if(minIndex != i)
        {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

int main()
{
    int n;
    cout << "Enter number of integers: ";
    cin >> n;
    
    int arr[n];
    cout << "Enter integers: ";
    for(int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    
    pid_t pid = fork();
    
    if(pid == 0)
    {
        cout << "Child process (PID: " << getpid() << ") is sorting integers using bubble sort..\n";
        int childArray[n];
        for(int i = 0; i < n; i++)
        {
            childArray[i] = arr[i];
        }
        
        bubbleSort(childArray, n);
        cout << "Child process sorted array: ";
        printArray(childArray, n);
        
        cout << "Child process exiting..\n";
        exit(0);
    }
    else if(pid > 0)
    {
        cout << "Parent process (PID: " << getpid() << ") is sorting integers using selection sort..\n";
        int parentArray[n];
        for(int i = 0; i < n; i++)
        {
            parentArray[i] = arr[i];
        }
        
        selectionSort(parentArray, n);
        cout << "Parent process sorted array: ";
        printArray(parentArray, n);
        
        cout << "Parent process waiting for child process to finish..\n";
        wait(NULL);
        cout << "Child process completed. Parent process resuming..\n";
        
        cout << "Parent process sleeping to demonstrate zombie state..\n";
        sleep(5);
        
        cout << "Parent process exiting..\n";
    }
    else
    {
        cout << "Fork failed.";
        return 1;
    }
    return 0;
}
