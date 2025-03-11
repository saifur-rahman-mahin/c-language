#include <windows.h>
#include <stdio.h>

#define ID_BUTTON_CALCULATE 1
#define ID_EDIT_NUM1 2
#define ID_EDIT_NUM2 3
#define ID_COMBO_OPERATOR 4
#define ID_STATIC_RESULT 5

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static HWND hEditNum1, hEditNum2, hComboOperator, hButtonCalculate, hResult;

    switch (msg)
    {
    case WM_CREATE:
        CreateWindow("STATIC", "Number 1:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
        hEditNum1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 20, 150, 25, hwnd, (HMENU)ID_EDIT_NUM1, NULL, NULL);

        CreateWindow("STATIC", "Operator:", WS_VISIBLE | WS_CHILD, 20, 60, 80, 20, hwnd, NULL, NULL, NULL);
        hComboOperator = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 110, 60, 150, 100, hwnd, (HMENU)ID_COMBO_OPERATOR, NULL, NULL);

        SendMessage(hComboOperator, CB_ADDSTRING, 0, (LPARAM) "+");
        SendMessage(hComboOperator, CB_ADDSTRING, 0, (LPARAM) "-");
        SendMessage(hComboOperator, CB_ADDSTRING, 0, (LPARAM) "*");
        SendMessage(hComboOperator, CB_ADDSTRING, 0, (LPARAM) "/");
        SendMessage(hComboOperator, CB_SETCURSEL, 0, 0);

        CreateWindow("STATIC", "Number 2:", WS_VISIBLE | WS_CHILD, 20, 100, 80, 20, hwnd, NULL, NULL, NULL);
        hEditNum2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 100, 150, 25, hwnd, (HMENU)ID_EDIT_NUM2, NULL, NULL);

        hButtonCalculate = CreateWindow("BUTTON", "Calculate", WS_VISIBLE | WS_CHILD, 110, 140, 150, 30, hwnd, (HMENU)ID_BUTTON_CALCULATE, NULL, NULL);
        hResult = CreateWindow("STATIC", "Result: ", WS_VISIBLE | WS_CHILD, 20, 180, 300, 25, hwnd, (HMENU)ID_STATIC_RESULT, NULL, NULL);

        break;

    case WM_COMMAND:
        if (LOWORD(wp) == ID_BUTTON_CALCULATE)
        {
            char num1Text[50], num2Text[50], resultText[100], operatorText[5];
            GetWindowText(hEditNum1, num1Text, 50);
            GetWindowText(hEditNum2, num2Text, 50);
            int index = SendMessage(hComboOperator, CB_GETCURSEL, 0, 0);
            SendMessage(hComboOperator, CB_GETLBTEXT, index, (LPARAM)operatorText);

            double num1 = atof(num1Text);
            double num2 = atof(num2Text);
            double result = 0;

            if (strcmp(operatorText, "+") == 0)
            {
                result = num1 + num2;
            }
            else if (strcmp(operatorText, "-") == 0)
            {
                result = num1 - num2;
            }
            else if (strcmp(operatorText, "*") == 0)
            {
                result = num1 * num2;
            }
            else if (strcmp(operatorText, "/") == 0)
            {
                if (num2 != 0)
                {
                    result = num1 / num2;
                }
                else
                {
                    MessageBox(hwnd, "Division by zero is not allowed!", "Error", MB_OK | MB_ICONERROR);
                    return 0;
                }
            }

            sprintf(resultText, "Result: %.2f", result);
            SetWindowText(hResult, resultText);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CalculatorClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("CalculatorClass", "Simple GUI Calculator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
