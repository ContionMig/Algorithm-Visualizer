#include "Quick Sort.h"

namespace QuickSort
{
    //https://www.geeksforgeeks.org/cpp-program-for-quicksort/
    /* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
    int partition(float arr[], int low, int high)
    {
        int pivot = arr[high];    // pivot 
        int i = (low - 1);  // Index of smaller element 

        for (int j = low; j <= high - 1; j++)
        {
            // If current element is smaller than or 
            // equal to pivot 
            if (arr[j] <= pivot)
            {
                i++;    // increment index of smaller element 

                Render::SelectedChange = i;
                Render::SelectedChange2 = j;
                
                float temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
                std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));
            }
        }

        Render::SelectedChange = i + 1;
        Render::SelectedChange2 = high;
       
        float temp = arr[high];
        arr[high] = arr[i + 1];
        arr[i + 1] = temp;
        std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));
        return (i + 1);
    }

    /* The main function that implements QuickSort
     arr[] --> Array to be sorted,
      low  --> Starting index,
      high  --> Ending index */
    void quickSort(float arr[], int low, int high)
    {
        if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
               at right place */
            int pi = partition(arr, low, high);

            // Separately sort elements before 
            // partition and after partition 
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    void ThreadQuickSort(LPVOID pOptions)
    {
        float* buf2 = reinterpret_cast<float*>(pOptions);
        quickSort(buf2, 0, 99);
        
        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}