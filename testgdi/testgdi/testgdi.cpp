#define _WIN32_WINNT 0x0400
#pragma warning(disable: 4996)

#include <windows.h>
#include <math.h>
#include <mmsystem.h>
#include <fstream>

#pragma comment(lib, "winmm.lib")

int main() {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    // Check if the file exists
    std::ifstream file("C:\\virus.wav");
    if (!file.good()) {
        MessageBoxA(NULL, "The file does not exist", "Error", MB_OK);
        return 1;
    }
    file.close();

    // Play the audio file in the background
    char command[256];
    sprintf_s(command, 256, "open \"%s\" type waveaudio alias music", "C:\\virus.wav");
    int error = mciSendStringA(command, NULL, 0, NULL);
    if (error != 0) {
        MessageBoxA(NULL, "Error opening audio file", "Error", MB_OK);
        return 1;
    }

    error = mciSendStringA("play music", NULL, 0, NULL);
    if (error != 0) {
        MessageBoxA(NULL, "Error playing audio file", "Error", MB_OK);
        return 1;
    }

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

        for (int i = 0; i < x + y; i++) {
            BitBlt(hdc, 0, i, x, 1, mdc, (int)(sin(angle) * 40), i, SRCCOPY);

            angle += 3.14159 / 80;
        }

        Sleep(10);

        for (int i = 0; i < x + y; i++) {
            BitBlt(hdc, i, 0, 1, y, mdc, i, (int)(sin(angle) * 40), SRCCOPY);

            angle += 3.14159 / 80;
        }

        Sleep(10);

        // Check for escape key press
        if (GetAsyncKeyState(VK_ESCAPE) < 0) {
            break;
        }
    }

    // Stop the audio file
    mciSendStringA("stop music", NULL, 0, NULL);
    mciSendStringA("close music", NULL, 0, NULL);

    DeleteDC(mdc);
    ReleaseDC(0, hdc);
    return 0;
}