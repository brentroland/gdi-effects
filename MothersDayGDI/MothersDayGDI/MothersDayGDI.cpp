#include <windows.h>
#include <math.h>
#include <ctime>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    srand((unsigned int)time(NULL)); // seed the random number generator

    HDC hdc = GetDC(0);

    int sw = GetSystemMetrics(SM_CXSCREEN); //gets width of the screen
    int sh = GetSystemMetrics(SM_CYSCREEN); //gets height of the screen

    POINT points[3];

    //sets the first point at the top left of the screen!
    points[0].x = 0;
    points[0].y = 0;

    //sets the second point to a random position withim the screens width and height
    points[1].x = rand() % sw;
    points[1].y = rand() % sh;

    //sets a third point to a random position outside the screens boundaries!
    points[2].x = sw + (rand() % sw);
    points[2].y = rand() % sh;

    float angle = 0.0f;
    float cubeX = (float)sw / 2;
    float cubeY = (float)sh / 2;
    float cubeZ = 0.0f;

    float shapeAngle = 0.0f;
    float shapeX = (float)sw / 2;
    float shapeY = (float)sh / 2;

    HFONT font = CreateFont(48, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
    HGDIOBJ oldFont = SelectObject(hdc, font);

    SetTextColor(hdc, RGB(255, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    while (true)
    {
        PlgBlt(hdc, points, hdc, 0, 0, sw, sh, 0, 0, 0);

        // Draw 3D cube
        float cubeSize = 100.0f;
        float cubePoints[8][3] = {
            {-cubeSize, -cubeSize, -cubeSize},
            {cubeSize, -cubeSize, -cubeSize},
            {cubeSize, cubeSize, -cubeSize},
            {-cubeSize, cubeSize, -cubeSize},
            {-cubeSize, -cubeSize, cubeSize},
            {cubeSize, -cubeSize, cubeSize},
            {cubeSize, cubeSize, cubeSize},
            {-cubeSize, cubeSize, cubeSize}
        };

        for (int i = 0; i < 8; i++)
        {
            float x = cubePoints[i][0] * cos(angle) - cubePoints[i][2] * sin(angle);
            float y = cubePoints[i][1];
            float z = cubePoints[i][0] * sin(angle) + cubePoints[i][2] * cos(angle);

            cubePoints[i][0] = x + cubeX;
            cubePoints[i][1] = y + cubeY;
            cubePoints[i][2] = z + cubeZ;
        }

        for (int i = 0; i < 12; i++)
        {
            int p1 = i * 2;
            int p2 = i * 2 + 1;

            if (p2 >= 8)
                p2 = p2 % 8;

            MoveToEx(hdc, (int)cubePoints[p1][0], (int)cubePoints[p1][1], NULL);
            LineTo(hdc, (int)cubePoints[p2][0], (int)cubePoints[p2][1]);
        }

        angle += 0.01f;

        // Draw rotating shape
        float shapeSize = 50.0f;
        float shapePoints[6][2] = {
            {(float)shapeX, (float)shapeY - shapeSize},
            {(float)shapeX + shapeSize * cos(shapeAngle), (float)shapeY + shapeSize * sin(shapeAngle)},
            {(float)shapeX + shapeSize * cos(shapeAngle + 2 * 3.14159f / 3), (float)shapeY + shapeSize * sin(shapeAngle + 2 * 3.14159f / 3)},
            {(float)shapeX + shapeSize * cos(shapeAngle + 4 * 3.14159f / 3), (float)shapeY + shapeSize * sin(shapeAngle + 4 * 3.14159f / 3)},
            {(float)shapeX + shapeSize * cos(shapeAngle + 6 * 3.14159f / 3), (float)shapeY + shapeSize * sin(shapeAngle + 6 * 3.14159f / 3)},
            {(float)shapeX, (float)shapeY - shapeSize}
        };

        for (int i = 0; i < 6; i++)
        {
            int p1 = i;
            int p2 = (i + 1) % 6;

            MoveToEx(hdc, (int)shapePoints[p1][0], (int)shapePoints[p1][1], NULL);
            LineTo(hdc, (int)shapePoints[p2][0], (int)shapePoints[p2][1]);
        }

        shapeAngle += 0.01f;

        TextOut(hdc, (int)(sw / 2 - 150), (int)(sh / 2 - 200), L"HAPPY MOTHER'S DAY", 20);

        if (GetAsyncKeyState(VK_ESCAPE) < 0)
        {
            break;
        }

        Sleep(50);
    }

    SelectObject(hdc, oldFont);
    DeleteObject(font);
    ReleaseDC(0, hdc); //releases device context!
    return 0;
}