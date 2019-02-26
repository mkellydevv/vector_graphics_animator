#include "display.h"

int main(int argc, char * const argv[]) {
	display* disp = new display(300, 0, 850, 700, "Vector Graphics Animator");		// Create a FL Window
	disp->show();
	return Fl::run();
}