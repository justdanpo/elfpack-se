#include "..\include\types.h"

int sin(int i);
int cos(int i);

void rotate(const int az, int &x, int &y);
void rotate3d(const int ax, const int ay, int &x, int &y, int& z);
void translatept(const POINT* frame, POINT& ret);
int getangle(int i);
