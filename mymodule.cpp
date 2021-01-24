#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <Windows.h>

HANDLE ready;
HINSTANCE hinst;
HWND hwnd[4];

void ui_thread() {
    HCURSOR hcursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
    WNDCLASSA wnd_class = {CS_OWNDC, DefWindowProc, 0, 0, hinst, NULL, hcursor, NULL, NULL, "glwindow"};

    RegisterClassA(&wnd_class);

    int style = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    RECT rect = {};
    rect.right = 640;
    rect.bottom = 360;

    AdjustWindowRect(&rect, style, false);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    int x = (sw - width) / 2;
    int y = (sh - height) / 2;

    hwnd[0] = CreateWindowA("glwindow", "Window 1", style, x - 325, y - 200, width, height, NULL, NULL, hinst, NULL);
    hwnd[1] = CreateWindowA("glwindow", "Window 2", style, x + 325, y - 200, width, height, NULL, NULL, hinst, NULL);
    hwnd[2] = CreateWindowA("glwindow", "Window 3", style, x - 325, y + 200, width, height, NULL, NULL, hinst, NULL);
    hwnd[3] = CreateWindowA("glwindow", "Window 4", style, x + 325, y + 200, width, height, NULL, NULL, hinst, NULL);

    SetEvent(ready);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

PyObject * meth_init(PyObject * self, PyObject * vargs, PyObject * kwargs) {
    ready = CreateEvent(NULL, true, false, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ui_thread, NULL, 0, NULL);
    WaitForSingleObject(ready, INFINITE);
    return Py_BuildValue("nnnn", hwnd[0], hwnd[1], hwnd[2], hwnd[3]);
}

PyMethodDef module_methods[] = {
    {"init", (PyCFunction)meth_init, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mymodule", NULL, -1, module_methods};

extern "C" PyObject * PyInit_mymodule() {
    PyObject * module = PyModule_Create(&module_def);
    return module;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        hinst = hinstDLL;
    }
    return true;
}
