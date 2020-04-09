#include "Render.h"

namespace Render
{
    ID3D11Device* g_pd3dDevice = NULL;
    ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
    IDXGISwapChain* g_pSwapChain = NULL;
    ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

    HANDLE hRenderThread = NULL;
    DWORD RenderThreadID = NULL;

    int Length = 100;
    float arr[100] = { 60 };

    int SelectedChange = -1;
    int SelectedChange2 = -1;

    bool Status = false;
    int Slowness = 30;

    void RenderThread()
    {
        // Create application window
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, Render::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Algorithm Visualizer", NULL };
        ::RegisterClassEx(&wc);
        HWND hwnd = ::CreateWindow(wc.lpszClassName, L"Algorithm Visualizer", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

        // Initialize Direct3D
        if (!Render::CreateDeviceD3D(hwnd))
        {
            Render::CleanupDeviceD3D();
            ::UnregisterClass(wc.lpszClassName, wc.hInstance);
            return;
        }

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiCol_PopupBg;

        // Fill Array
        for (int i = 0; i < 100; i++)
            arr[i] = (i + 1);

        // Show the window
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ExtasyHostingTheme();

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(Render::g_pd3dDevice, Render::g_pd3dDeviceContext);

        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        DragAcceptFiles(hwnd, TRUE);
        while (msg.message != WM_QUIT)
        {
            if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                continue;
            }

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(-1, 0));
            ImGui::SetNextWindowSize(ImVec2(1267, 760));
            if (ImGui::Begin("Main", NULL, window_flags))
            {
                if (ImGui::BeginChild("##Options", ImVec2(1250, 95), true))
                {
                    static int AlgorithmUsed = 0;

                    ImGui::SetNextItemWidth(1187);
                    ImGui::Combo("Display Mode", &AlgorithmUsed, "Quick Sort\0Bubble Sort\0Selection Sort\0Merge Sort\0Heap Sort\0Radix Sort\0Counting Sort\0Shell Sort\0");

                    if (!Status)
                    {
                        if (ImGui::Button("Scramble", ImVec2(1230, 0)))
                            Scramble();

                        if (ImGui::Button("Start", ImVec2(1230, 0)))
                        {
                            switch (AlgorithmUsed)
                            {
                            case 0:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)QuickSort::ThreadQuickSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 1:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BubbleSort::ThreadBubbleSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 2:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SelectionSort::ThreadSelectionSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 3:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MergeSort::ThreadMergeSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 4:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HeapSort::ThreadHeapSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 5:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RadixSort::ThreadRadixSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 6:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CountingSort::ThreadCountingSort, (LPVOID)arr, NULL, NULL);
                                break;
                            case 7:
                                Status = true;
                                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ShellSort::ThreadShellSort, (LPVOID)arr, NULL, NULL);
                                break;
                            default:
                                break;
                            }
                        }
                    }
                    ImGui::SetNextItemWidth(1180);
                    ImGui::SliderInt("Slowness", &Slowness, 10, 1000);
                    ImGui::EndChild();
                }
                
                ImGui::PlotHistogram("##Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0, 100, ImVec2(1250, 730 - 105), sizeof(float), SelectedChange, SelectedChange2);
                ImGui::End();
            }


            ImGui::Render();
            Render::g_pd3dDeviceContext->OMSetRenderTargets(1, &Render::g_mainRenderTargetView, NULL);
            Render::g_pd3dDeviceContext->ClearRenderTargetView(Render::g_mainRenderTargetView, (float*)&ImVec4(0.45f, 0.55f, 0.60f, 1.00f));
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            Render::g_pSwapChain->Present(1, 0);
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        Render::CleanupDeviceD3D();
        ::DestroyWindow(hwnd);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

    void Scramble()
    {
        SelectedChange = -1;
        SelectedChange2 = -1;

        // Re-do the array just in case
        for (int i = 0; i < 100; i++)
            arr[i] = (i + 1);

        srand(time(NULL));
        for (int i = 99; i > 0; i--)
        {
            int j = rand() % (i + 1);

            float temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }

    bool CreateDeviceD3D(HWND hWnd)
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    }

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void ExtasyHostingTheme()
    {
        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding = ImVec2(6, 4);
        style->WindowRounding = 0.0f;
        style->FramePadding = ImVec2(5, 2);
        style->FrameRounding = 8.f;
        style->ItemSpacing = ImVec2(7, 3);
        style->ItemInnerSpacing = ImVec2(1, 1);
        style->TouchExtraPadding = ImVec2(0, 0);
        style->IndentSpacing = 6.0f;
        style->ScrollbarSize = 12.0f;
        style->ScrollbarRounding = 16.0f;
        style->GrabMinSize = 20.0f;
        style->GrabRounding = 8.f;
        style->ChildRounding = 8.f;
        style->WindowTitleAlign.x = 0.50f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 1.0f;

        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.09f, 0.22f, 1.00f);
        style->Colors[ImGuiCol_TabHovered] = ImVec4(0.15f, 0.09f, 0.17f, 1.00f);
        style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
        style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    }

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            exit(0);
            return 0;
        }

        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}