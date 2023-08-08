#include <windows.h>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

char alphabetLetters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char symbols[29] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '-', '+', '=', '{', '}', '[', ']', '|', ':', ';', '"', '\'', '<', '>', ',', '.', '/', '?'};
std::string generatedPassword = {};
int upperRange;
HWND button;
HWND textLabel;

void randomLetter()
{
    upperRange = 26;
    int randomLetter = rand() % upperRange;
    upperRange = 2;
    int capsOrNot = rand() % upperRange;

    if (capsOrNot == 0)
    {
        char uppercaseLetter = toupper(alphabetLetters[randomLetter]);
        generatedPassword.push_back(uppercaseLetter);
    }
    else
    {
        generatedPassword.push_back(alphabetLetters[randomLetter]);
    }
}

void randomNumber()
{
    upperRange = 10;
    int randomNumber = rand() % upperRange;
    std::string randomNumberChar = std::to_string(randomNumber);
    generatedPassword += randomNumberChar;
}

void randomSymbol()
{
    upperRange = 29;
    int randomSymbol = rand() % upperRange;
    generatedPassword.push_back(symbols[randomSymbol]);
}

string generatePassword()
{
    generatedPassword = {};
    srand((unsigned)time(NULL));

    for (int i = 0; i < 20; i++)
    {
        upperRange = 3;
        int choice = rand() % upperRange;

        if (choice == 0)
        {
            randomLetter();
        }
        else if (choice == 1)
        {
            randomNumber();
        }
        else
        {
            randomSymbol();
        }
    }

    return generatedPassword;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (reinterpret_cast<HWND>(lParam) == button && HIWORD(wParam) == BN_CLICKED)
        {
            generatedPassword = generatePassword(); // Generate the password and set it as the text of the text label
            SetWindowTextA(textLabel, generatedPassword.c_str());
            ShowWindow(textLabel, SW_SHOW); // Show the text label when the button is clicked
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
        SendMessage(textLabel, EM_SETSEL, 0, -1); // Set the selection to the entire text
        return 0;

    case WM_LBUTTONUP:
        SendMessage(textLabel, EM_SETSEL, -1, 0); // Clear the selection
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    // Window size
    int windowWidth = 300;
    int windowHeight = 300;

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "CPP Password Generator 3",
        WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), // Adjust window style here
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    button = CreateWindowExA( // Create the button
        0,
        "BUTTON",
        "Generate Password",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 200, 50, // Position and size of the button
        hwnd,
        reinterpret_cast<HMENU>(1), // Assign an identifier to the button
        hInstance,
        NULL);

    if (button == NULL)
    {
        return 0;
    }

    textLabel = CreateWindowExA( // Create the text label
        0,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        0, 0, 200, 20, // Position and size of the text label
        hwnd,
        NULL,
        hInstance,
        NULL);

    // Calculate the centered position for the button and text label
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;

    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (clientWidth - buttonWidth) / 2;
    int buttonY = (clientHeight - buttonHeight - 30) / 2;

    int textLabelWidth = 200;
    int textLabelHeight = 20;
    int textLabelX = (clientWidth - textLabelWidth) / 2;
    int textLabelY = buttonY + buttonHeight;

    // Set the position and size of the button and text label
    SetWindowPos(button, NULL, buttonX, buttonY, buttonWidth, buttonHeight, SWP_NOZORDER);
    SetWindowPos(textLabel, NULL, textLabelX, textLabelY, textLabelWidth, textLabelHeight, SWP_NOZORDER);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}