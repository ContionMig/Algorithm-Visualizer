#include "Counting Sort.h"

namespace CountingSort
{
    // Counting sort in C++ programming

#include <iostream>
    using namespace std;

    void countSort(float array[], int size)
    {
        // The size of count must be at least the (max+1) but
        // we cannot assign declare it as int count(max+1) in C++ as
        // it does not support dynamic memory allocation.
        // So, its size is provided statically.
        int output[100];
        int count[100];
        int max = array[0];
        for (int i = 1; i < size; i++)
        {
            if (array[i] > max)
                max = array[i];
        }

        for (int i = 0; i <= max; ++i)
        {
            count[i] = 0;
        }

        for (int i = 0; i < size; i++)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = i;

            count[(int)array[i]]++;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }
        for (int i = 1; i <= max; i++)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = i;

            count[i] += count[i - 1];

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }
        for (int i = size - 1; i >= 0; i--)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = i;
            
            output[count[(int)array[i]] - 1] = array[i];
            count[(int)array[i]]--;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }
        for (int i = 0; i < size; i++)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = i;
            array[i] = output[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness / 4));
        }
    }

    void ThreadCountingSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);

        countSort(arr, 100);

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}