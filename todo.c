#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_TASK_LENGTH 256

// টাস্ক অ্যারে এবং প্রয়োজনীয় ভেরিয়েবল
char tasks[MAX_TASKS][MAX_TASK_LENGTH];
int taskCount = 0;
int selectedTask = -1;

// ফাংশন প্রোটোটাইপ
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddTask(HWND hwnd);
void EditTask(HWND hwnd);
void DeleteTask(HWND hwnd);
void trim(char *str);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    // উইন্ডো ক্লাস রেজিস্টার করা
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "ToDoAppClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return 0;
    }

    // উইন্ডো তৈরি করা
    HWND hwnd = CreateWindow("ToDoAppClass", "To-Do List", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInst, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
        return 0;
    }

    // ইনপুট বক্স
    CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 30, 30, 300, 30, hwnd, (HMENU)1, NULL, NULL);

    // "Add Task" বাটন
    CreateWindow("BUTTON", "Add Task", WS_VISIBLE | WS_CHILD, 350, 30, 100, 30, hwnd, (HMENU)2, NULL, NULL);

    // লিস্টবক্স (টাস্ক দেখানোর জন্য)
    CreateWindow("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY, 30, 80, 420, 250, hwnd, (HMENU)3, NULL, NULL);

    // "Edit Task" বাটন
    CreateWindow("BUTTON", "Edit Task", WS_VISIBLE | WS_CHILD, 30, 350, 100, 30, hwnd, (HMENU)4, NULL, NULL);

    // "Delete Task" বাটন
    CreateWindow("BUTTON", "Delete Task", WS_VISIBLE | WS_CHILD, 150, 350, 100, 30, hwnd, (HMENU)5, NULL, NULL);

    // উইন্ডো দেখানো
    ShowWindow(hwnd, ncmdshow);
    UpdateWindow(hwnd);

    // মেসেজ লুপ
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// উইন্ডো ইভেন্ট হ্যান্ডলিং
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case 2:
            AddTask(hwnd);
            break; // Add Task বাটন
        case 4:
            EditTask(hwnd);
            break; // Edit Task বাটন
        case 5:
            DeleteTask(hwnd);
            break; // Delete Task বাটন
        case 3:
            if (HIWORD(wp) == LBN_SELCHANGE)
            {
                selectedTask = SendMessage(GetDlgItem(hwnd, 3), LB_GETCURSEL, 0, 0);
                if (selectedTask != -1)
                {
                    SetWindowText(GetDlgItem(hwnd, 1), tasks[selectedTask]);
                }
            }
            break;
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

// টাস্ক যোগ করার ফাংশন
void AddTask(HWND hwnd)
{
    if (taskCount >= MAX_TASKS)
    {
        MessageBox(hwnd, "Task limit reached!", "Error", MB_ICONERROR);
        return;
    }

    HWND hwndEdit = GetDlgItem(hwnd, 1);
    HWND hwndList = GetDlgItem(hwnd, 3);

    char task[MAX_TASK_LENGTH];
    GetWindowText(hwndEdit, task, MAX_TASK_LENGTH);
    trim(task); // Remove leading and trailing spaces

    if (strlen(task) > 0)
    {
        strcpy(tasks[taskCount], task);
        taskCount++;

        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)task);
        SetWindowText(hwndEdit, "");
    }
    else
    {
        MessageBox(hwnd, "Task cannot be empty!", "Error", MB_ICONERROR);
    }
}

// টাস্ক এডিট করার ফাংশন
void EditTask(HWND hwnd)
{
    if (selectedTask == -1 || selectedTask >= taskCount)
    {
        MessageBox(hwnd, "No task selected!", "Error", MB_ICONERROR);
        return;
    }

    HWND hwndEdit = GetDlgItem(hwnd, 1);
    HWND hwndList = GetDlgItem(hwnd, 3);

    char newTask[MAX_TASK_LENGTH];
    GetWindowText(hwndEdit, newTask, MAX_TASK_LENGTH);
    trim(newTask); // Remove leading and trailing spaces

    if (strlen(newTask) > 0)
    {
        strcpy(tasks[selectedTask], newTask);

        // লিস্টবক্সে টাস্ক আপডেট করা
        SendMessage(hwndList, LB_DELETESTRING, selectedTask, 0);
        SendMessage(hwndList, LB_INSERTSTRING, selectedTask, (LPARAM)newTask);

        SendMessage(hwndList, LB_SETCURSEL, selectedTask, 0); // নতুন টাস্ককে সিলেক্ট করা
        SetWindowText(hwndEdit, "");                          // ইনপুট বক্স খালি করা
    }
    else
    {
        MessageBox(hwnd, "Task cannot be empty!", "Error", MB_ICONERROR);
    }
}

// টাস্ক ডিলিট করার ফাংশন
void DeleteTask(HWND hwnd)
{
    if (selectedTask == -1 || selectedTask >= taskCount)
    {
        MessageBox(hwnd, "No task selected!", "Error", MB_ICONERROR);
        return;
    }

    HWND hwndList = GetDlgItem(hwnd, 3);

    // টাস্ক অ্যারে থেকে ডিলিট করা
    for (int i = selectedTask; i < taskCount - 1; i++)
    {
        strcpy(tasks[i], tasks[i + 1]);
    }
    taskCount--;

    // লিস্টবক্স থেকে টাস্ক ডিলিট করা
    SendMessage(hwndList, LB_DELETESTRING, selectedTask, 0);
    selectedTask = -1;
}

// স্ট্রিং থেকে leading এবং trailing স্পেস ট্রিম করার ফাংশন
void trim(char *str)
{
    int i = strlen(str) - 1;
    while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
    {
        str[i] = '\0';
        i--;
    }
}