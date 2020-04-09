#include "Bubble Sort.h"

namespace BubbleSort
{
    // https://www.geeksforgeeks.org/bubble-sort/
    void swap(float* xp, float* yp)
    {
        float temp = *xp;
        *xp = *yp;
        *yp = temp;
    }

    void ThreadBubbleSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);
        int n = 100;

        int i, j;
        for (i = 0; i < n - 1; i++)
        {
            // Last i elements are already in place  
            for (j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    Render::SelectedChange = j;
                    Render::SelectedChange2 = j + 1;

                    swap(&arr[j], &arr[j + 1]);

                    std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));
                }
            }
        }

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}