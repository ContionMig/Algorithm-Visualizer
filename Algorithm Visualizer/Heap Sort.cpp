#include "Heap Sort.h"

namespace HeapSort
{
    void heapify(float arr[], int n, int i)
    {
        int largest = i; // Initialize largest as root 
        int l = 2 * i + 1; // left = 2*i + 1 
        int r = 2 * i + 2; // right = 2*i + 2 

        // If left child is larger than root 
        if (l < n && arr[l] > arr[largest])
            largest = l;

        // If right child is larger than largest so far 
        if (r < n && arr[r] > arr[largest])
            largest = r;

        // If largest is not root 
        if (largest != i)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = largest;

            float temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));

            // Recursively heapify the affected sub-tree 
            heapify(arr, n, largest);
        }
    }

    // main function to do heap sort 
    void heapSort(float arr[], int n)
    {
        // Build heap (rearrange array) 
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        // One by one extract an element from heap 
        for (int i = n - 1; i > 0; i--)
        {
            Render::SelectedChange = i;
            Render::SelectedChange2 = 0;

            float temp = arr[i];
            arr[i] = arr[0];
            arr[0] = temp;

            std::this_thread::sleep_for(std::chrono::milliseconds(Render::Slowness));

            // call max heapify on the reduced heap 
            heapify(arr, i, 0);
        }
    }

    void ThreadHeapSort(LPVOID pOptions)
    {
        float* arr = reinterpret_cast<float*>(pOptions);
        heapSort(arr, 100);

        Render::SelectedChange = -1;
        Render::SelectedChange2 = -1;

        Render::Status = false;
    }
}