#include "Merge Sort.h"

namespace MergeSort
{
    // https://www.geeksforgeeks.org/merge-sort/
    // Merges two subarrays of arr[]. 
    // First subarray is arr[l..m] 
    // Second subarray is arr[m+1..r] 
    void merge(float arr[], int l, int m, int r)
    {
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;

        /* create temp arrays */
        int L[110], R[110];

        /* Copy data to temp arrays L[] and R[] */
        for (i = 0; i < n1; i++)
        {
            L[i] = arr[l + i];
            Render::SelectedChange = i;
            Render::SelectedChange2 = l + i;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }
        for (j = 0; j < n2; j++)
        {
            R[j] = arr[m + 1 + j];
            Render::SelectedChange = j;
            Render::SelectedChange2 = m + 1 + j;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }

        /* Merge the temp arrays back into arr[l..r]*/
        i = 0; // Initial index of first subarray 
        j = 0; // Initial index of second subarray 
        k = l; // Initial index of merged subarray 
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j])
            {
                arr[k] = L[i];
                Render::SelectedChange = k;
                Render::SelectedChange2 = i;

                std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
                i++;
            }
            else
            {
                arr[k] = R[j];
                Render::SelectedChange = k;
                Render::SelectedChange2 = j;

                std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
                j++;
            }
            k++;
        }

        /* Copy the remaining elements of L[], if there
           are any */
        while (i < n1)
        {
            arr[k] = L[i];
            Render::SelectedChange = i;
            Render::SelectedChange2 = k;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
            i++;
            k++;
        }

        /* Copy the remaining elements of R[], if there
           are any */
        while (j < n2)
        {
            arr[k] = R[j];
            Render::SelectedChange = k;
            Render::SelectedChange2 = j;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
            j++;
            k++;
        }
    }

    void mergeSort(float arr[], int l, int r)
    {
        if (l < r)
        {
            // Same as (l+r)/2, but avoids overflow for 
            // large l and h 
            int m = l + (r - l) / 2;

            // Sort first and second halves 
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);

            merge(arr, l, m, r);
        }
    }

    /* l is for left index and r is right index of the
       sub-array of arr to be sorted */
    void ThreadMergeSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);
        mergeSort(arr, 0, 99);

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}