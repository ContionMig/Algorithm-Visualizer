#include "Radix Sort.h"

namespace RadixSort
{
    int getMax(float arr[], int n)
    {
        int mx = arr[0];
        for (int i = 1; i < n; i++)
            if (arr[i] > mx)
                mx = arr[i];
        return mx;
    }

    // A function to do counting sort of arr[] according to 
    // the digit represented by exp. 
    void countSort(float arr[], int n, int exp)
    {
        float output[110]; // output array 
        int i, count[10] = { 0 };

        // Store count of occurrences in count[] 
        for (i = 0; i < n; i++)
            count[((int)arr[i] / exp) % 10]++;

        // Change count[i] so that count[i] now contains actual 
        //  position of this digit in output[] 
        for (i = 1; i < 10; i++)
            count[i] += count[i - 1];

        // Build the output array 
        for (i = n - 1; i >= 0; i--)
        {
            output[count[((int)arr[i] / exp) % 10] - 1] = arr[i];
            count[((int)arr[i] / exp) % 10]--;
            Render::SelectedChange2 = i;
        }

        // Copy the output array to arr[], so that arr[] now 
        // contains sorted numbers according to current digit 
        for (i = 0; i < n; i++)
        {
            arr[i] = output[i];

            // Not much to show since it already stores it in a different array.
            Render::SelectedChange = i;
            Render::SelectedChange2 = i;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));
        }
    }

    // The main function to that sorts arr[] of size n using  
    // Radix Sort 
    void ThreadRadixSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);
        int n = 100;

        // Find the maximum number to know number of digits 
        int m = getMax(arr, n);

        // Do counting sort for every digit. Note that instead 
        // of passing digit number, exp is passed. exp is 10^i 
        // where i is current digit number 
        for (int exp = 1; m / exp > 0; exp *= 10)
            countSort(arr, n, exp);

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}