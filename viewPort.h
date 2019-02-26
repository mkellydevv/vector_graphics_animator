#ifndef VIEW_PORT_H
#define VIEW_PORT_H

#include "node.h"

// FLTK
#include <FL/Fl.H>
#include <FL/Gl.H>
#include <Fl/Fl_Gl_Window.H>
#include <GL/glu.h>

class viewPort : public Fl_Gl_Window
{
private:
	std::vector<transform>* tweenTransforms;	// Current frame's calculated tween transforms

	// Methods //
	void draw();								// Calls draw on the Graphics Window
	void drawTree(node* targetNode);			// Called inside draw. Recursively draws each node

public:
	viewPort(int x, int y, int w, int h);		// Constructor
	~viewPort();								// Destructor

	// Methods //
	void calculateTweenTransforms(int i, int j);	// Calculate the tween transforms for the current frame using the following
													// i: framesBetweenKeyFrames, j: framesSincePreviousKeyFrame
	// Getters
	std::vector<transform>* getTweenTransforms();
};

#endif