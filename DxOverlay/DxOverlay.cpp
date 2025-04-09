#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
#include <thread>
#include <fstream>

#define WINDOW_WIDTH      500
#define WINDOW_HEIGHT     350
#define SHORTCUT_WIDTH    600
#define SHORTCUT_HEIGHT   400
#define WINDOW_TITLE      L"Private User Window"
#define SHORTCUT_TITLE    L"Atalhos Rápidos"
#define HOTKEY_NEW_WINDOW 1
#define HOTKEY_SHORTCUTS  2
#define CARD_COUNT        4

// Estrutura para representar um card
struct Card {
    RECT rect;
    std::wstring title;
    std::wstring description;
    COLORREF color;
    int id;
};

// Variáveis globais
bool isDragging = false;
bool showHelp = false;
POINT dragStartPos;
int transparency = 255;
std::vector<Card> cards;

// Protótipos
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ShortcutWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateTransparency(HWND hwnd, int delta);
void ToggleHelp();
HWND CreateNewWindow(HINSTANCE hInstance);
HWND CreateShortcutWindow(HINSTANCE hInstance);
void InitializeCards(HWND hwnd);
void ExecuteCardAction(int cardId);
void DrawCards(HWND hwnd, HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = CreateNewWindow(hInstance);
    if (!hwnd) return 0;

    // Registrar hotkeys
    if (!RegisterHotKey(hwnd, HOTKEY_NEW_WINDOW, MOD_CONTROL, 'N')) {
        MessageBox(NULL, L"Failed to register new window hotkey", L"Error", MB_OK | MB_ICONERROR);
    }
    if (!RegisterHotKey(hwnd, HOTKEY_SHORTCUTS, MOD_CONTROL, 'S')) {
        MessageBox(NULL, L"Failed to register shortcuts hotkey", L"Error", MB_OK | MB_ICONERROR);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterHotKey(hwnd, HOTKEY_NEW_WINDOW);
    UnregisterHotKey(hwnd, HOTKEY_SHORTCUTS);
    return 0;
}

HWND CreateNewWindow(HINSTANCE hInstance) {
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

    if (!hwnd) return NULL;

    SetLayeredWindowAttributes(hwnd, 0, transparency, LWA_ALPHA);
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

HWND CreateShortcutWindow(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"ShortcutWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = ShortcutWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        SHORTCUT_TITLE,
        WS_POPUP | WS_VISIBLE,
        100, 100, SHORTCUT_WIDTH, SHORTCUT_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return NULL;

    SetLayeredWindowAttributes(hwnd, 0, transparency, LWA_ALPHA);
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

    InitializeCards(hwnd);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

void InitializeCards(HWND hwnd) {
    cards.clear();

    int cardWidth = SHORTCUT_WIDTH / 2 - 30;
    int cardHeight = SHORTCUT_HEIGHT / 2 - 30;
    int margin = 20;

    // Card 1
    cards.push_back({
        { margin, margin, margin + cardWidth, margin + cardHeight },
        L"Abrir Calculadora",
        L"Executa o aplicativo de calculadora do Windows",
        RGB(70, 130, 180),  // SteelBlue
        1
        });

    // Card 2
    cards.push_back({
        { margin + cardWidth + 10, margin, SHORTCUT_WIDTH - margin, margin + cardHeight },
        L"Abrir Bloco de Notas",
        L"Executa o bloco de notas do Windows",
        RGB(60, 179, 113),  // MediumSeaGreen
        2
        });

    // Card 3
    cards.push_back({
        { margin, margin + cardHeight + 10, margin + cardWidth, SHORTCUT_HEIGHT - margin },
        L"Capturar Tela",
        L"Simula a tecla PrintScreen para captura de tela",
        RGB(205, 92, 92),  // IndianRed
        3
        });

    // Card 4
    cards.push_back({
        { margin + cardWidth + 10, margin + cardHeight + 10, SHORTCUT_WIDTH - margin, SHORTCUT_HEIGHT - margin },
        L"Fechar Tudo",
        L"Fecha todas as janelas deste aplicativo",
        RGB(238, 130, 238),  // Violet
        4
        });
}

void ExecuteCardAction(int cardId) {
    switch (cardId) {
    case 1:  // Abrir Calculadora
        ShellExecute(NULL, L"open", L"calc.exe", NULL, NULL, SW_SHOWNORMAL);
        break;

    case 2:  // Abrir Bloco de Notas
        ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);
        break;

    case 3:  // Capturar Tela
        keybd_event(VK_SNAPSHOT, 0, 0, 0);
        keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
        break;

    case 4:  // Fechar Tudo
        PostQuitMessage(0);
        break;
    }
}

void DrawCards(HWND hwnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    HBRUSH bgBrush = CreateSolidBrush(RGB(40, 40, 40));
    FillRect(hdc, &clientRect, bgBrush);
    DeleteObject(bgBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    HFONT hTitleFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    HFONT hDescFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    for (const auto& card : cards) {
        HBRUSH cardBrush = CreateSolidBrush(card.color);
        FillRect(hdc, &card.rect, cardBrush);
        DeleteObject(cardBrush);

        FrameRect(hdc, &card.rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        RECT textRect = card.rect;
        InflateRect(&textRect, -10, -10);

        SelectObject(hdc, hTitleFont);
        DrawTextW(hdc, card.title.c_str(), -1, &textRect, DT_TOP | DT_CENTER);

        SelectObject(hdc, hDescFont);
        textRect.top += 30;
        DrawTextW(hdc, card.description.c_str(), -1, &textRect, DT_WORDBREAK);
    }

    DeleteObject(hTitleFont);
    DeleteObject(hDescFont);
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
    case WM_HOTKEY:
        if (wParam == HOTKEY_NEW_WINDOW) {
            CreateNewWindow((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE));
        }
        else if (wParam == HOTKEY_SHORTCUTS) {
            CreateShortcutWindow((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE));
        }
        break;

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
        else if (wParam == 'H' || wParam == 'h') {
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
                L"- - Decrease opacity\n"
                L"Ctrl+N - New private window\n"
                L"Ctrl+S - Shortcuts window\n\n"
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

LRESULT CALLBACK ShortcutWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawCards(hwnd, hdc);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };

        for (const auto& card : cards) {
            if (PtInRect(&card.rect, pt)) {
                ExecuteCardAction(card.id);
                return 0;
            }
        }

        isDragging = true;
        SetCapture(hwnd);
        dragStartPos = pt;
        break;
    }

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
        break;

    case WM_DESTROY:
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}