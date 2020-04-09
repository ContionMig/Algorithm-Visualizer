#include "Selection Sort.h"

namespace SelectionSort
{
    // https://www.geeksforgeeks.org/selection-sort/
    void ThreadSelectionSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);
        int n = 100;

        int i, j, min_idx;

        // One by one move boundary of unsorted subarray  
        for (i = 0; i < n - 1; i++)
        {
            // Find the minimum element in unsorted array  
            min_idx = i;
            for (j = i + 1; j < n; j++)
                if (arr[j] < arr[min_idx])
                    min_idx = j;

            // Swap the found minimum element with the first element  
            Render::SelectedChange = i;
            Render::SelectedChange2 = min_idx;

            float temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));
        }

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}