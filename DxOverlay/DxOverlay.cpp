#include <windows.h>
#include <tchar.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 350  // Aumentado para acomodar o texto adicional
#define WINDOW_TITLE  L"Private User Window"

// Variáveis globais
bool isDragging = false;
bool showHelp = false;  // Novo estado para controlar a exibição do help
POINT dragStartPos;
int transparency = 255;

// Protótipos
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateTransparency(HWND hwnd, int delta);
void ToggleHelp();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"PrivateWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        WINDOW_TITLE,
        WS_POPUP | WS_VISIBLE,
        100, 100, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    SetLayeredWindowAttributes(hwnd, 0, transparency, LWA_ALPHA);
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void UpdateTransparency(HWND hwnd, int delta) {
    transparency += delta;
    if (transparency > 255) transparency = 255;
    if (transparency < 30) transparency = 30;
    SetLayeredWindowAttributes(hwnd, 0, transparency, LWA_ALPHA);
    InvalidateRect(hwnd, NULL, TRUE);
}

void ToggleHelp() {
    showHelp = !showHelp;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        else if (wParam == VK_ADD || wParam == VK_OEM_PLUS) {
            UpdateTransparency(hwnd, -25);
        }
        else if (wParam == VK_SUBTRACT || wParam == VK_OEM_MINUS) {
            UpdateTransparency(hwnd, 25);
        }
        else if (wParam == 'H' || wParam == 'h') {  // Tecla H para help
            ToggleHelp();
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_LBUTTONDOWN:
        isDragging = true;
        SetCapture(hwnd);
        dragStartPos.x = LOWORD(lParam);
        dragStartPos.y = HIWORD(lParam);
        break;

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;

    case WM_MOUSEMOVE:
        if (isDragging) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            SetWindowPos(
                hwnd, NULL,
                cursorPos.x - dragStartPos.x,
                cursorPos.y - dragStartPos.y,
                0, 0, SWP_NOSIZE | SWP_NOZORDER
            );
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH bgBrush = CreateSolidBrush(RGB(50, 50, 50));
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));

        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SelectObject(hdc, hFont);

        if (showHelp) {
            const wchar_t* helpText =
                L"=== Key Bindings ===\n"
                L"H - Toggle this help\n"
                L"ESC - Close window\n"
                L"+ - Increase opacity\n"
                L"- - Decrease opacity\n\n"
                L"=== Mouse Controls ===\n"
                L"Drag - Move window\n\n"
                L"Current transparency: %d%%";

            wchar_t buffer[512];
            swprintf(buffer, 512, helpText, (int)((transparency / 255.0) * 100));
            DrawTextW(hdc, buffer, -1, &rect, DT_CENTER | DT_VCENTER);
        }
        else {
            const wchar_t* mainText =
                L"Private Window - Not visible in screen sharing\n\n"
                L"Press H for help\n"
                L"Current transparency: %d%%";

            wchar_t buffer[256];
            swprintf(buffer, 256, mainText, (int)((transparency / 255.0) * 100));
            DrawTextW(hdc, buffer, -1, &rect, DT_CENTER | DT_VCENTER);
        }

        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}