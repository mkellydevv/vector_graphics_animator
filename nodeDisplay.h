#ifndef NODE_DISPLAY_H
#define NODE_DISPLAY_H

#include "display.h"
#include <sstream>
#include <string>

// FLTK
#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Slider.H>
#include <Fl/Fl_Window.H>

class display;		// Forward declaration

class nodeDisplay : public Fl_Window
{
private:
	display* dis;
	node* target;						// The node that a chid will be added to or the node that will be edited
	int displayType;					// Determines if this is a new node display, or an edit node display

	std::vector<gVector3> vertices;		// The following are used to construct a new node, or edit the values of a node
	std::string name;
	int drawMode;
	gVector3 originalColor;
public:
	nodeDisplay(int x, int y, int w, int h, char* c, display* d, int type);		// Constructor: Type - 0: New Node Display, 1: Edit Node Display 
	~nodeDisplay();																// Destructor
	void loadWidgets();															// Instantiates FL Widgets

	//Widgets
	Fl_Input* nodeNameInput;
	Fl_Input* vertexInput;
	Fl_Button* addVertexButton;
	Fl_Box* verticesBox;
	Fl_Color_Chooser* colorChooser;
	Fl_Slider* typeSlider;
	Fl_Button* clearVerticesButton;
	Fl_Input * widthInput;
	Fl_Input* heightInput;
	Fl_Button* addPolyButton;
	Fl_Button* addQuadButton;

	//Callback methods
	static void addVertexCB(Fl_Widget* w, void* data);		// Adds a vertex to this->vertices. Usage - "x y"
	static void colorCB(Fl_Widget* w, void* data);			// Choose original color of the node
	static void sliderCB(Fl_Widget* w, void* data);			// Choose the draw mode of the node
	static void clearVerticesCB(Fl_Widget* w, void* data);	// Clears all vertices from this->vertices
	static void addPolyCB(Fl_Widget* w, void* data);		// Adds or edits a node using this->vertices
	static void addQuadCB(Fl_Widget* w, void* data);		// Adds or edits a node using width and height values
};

#endif