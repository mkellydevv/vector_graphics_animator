#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <fstream>
#include <string>

#include "viewPort.h"
#include "node.h"
#include "nodeDisplay.h"

// FLTK
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Value_Slider.H>
#include <Fl/Fl_Window.H>

class display : public Fl_Window
{
private:
	viewPort* view;										// FLGL Window
	node* root;											// Root node of the scene graph
	node* currentNode;									// Currently selected node
	std::vector<std::vector<transform>*>* frames;		// Each frame points to a vector of transforms
	std::vector<transform>* currentFrame;				// Currently select frame
	std::vector<transform>* previousKeyFrame;			// A frame is a key frame when it has transforms
	std::vector<transform>* nextKeyFrame;
	float fps = 60;										// Frames per second
	int nodeID = 0;										// Nodes are given a unique ID for identification	

	bool playing = false;								// Toggle for playback
	bool tween = false;									// Toggle for tweening

	// Widgets
	Fl_Button* newNodeButton;
	Fl_Button* editNodeButton;
	Fl_Button* moveNodeUpButton;
	Fl_Button* moveNodeDownButton;
	Fl_Button* deleteNodeButton;
	Fl_Button* openFileButton;
	Fl_Button* saveFileButton;
	Fl_Input* fileNameInput;
	Fl_Tree* tree;
	Fl_Tree_Item* currentTreeItem;
	Fl_Color_Chooser* colorChooser;
	Fl_Dial* rotationDial;
	Fl_Simple_Counter* positionXCounter;
	Fl_Simple_Counter* positionYCounter;
	Fl_Simple_Counter* translationXCounter;
	Fl_Simple_Counter* translationYCounter;
	Fl_Simple_Counter* scaleXCounter;
	Fl_Simple_Counter* scaleYCounter;
	Fl_Value_Slider* frameSlider;
	Fl_Button* previousFrameButton;
	Fl_Button* playFrameButton;
	Fl_Button* nextFrameButton;
	Fl_Button* insertFrameButton;
	Fl_Button* deleteFrameButton;
	Fl_Button* clearFrameButton;
	Fl_Button* saveTweenButton;
	Fl_Check_Button* tweenButton;
	Fl_Simple_Counter* frameCounter;
	Fl_Simple_Counter* fpsCounter;

public:
	display(int x, int y, int w, int h, char* c);	// Constructor
	~display();										// Destructor
	void loadWidgets();								// Instantiates FL Widgets

	// Methods //
	void playToggle();																		// Toggles playing
	void tweenToggle();																		// Toggles tween
	void populateTree(node* targetNode, Fl_Tree_Item* targetItem);							// Translates scene graph onto the tree widget
	void clearChildrenTransforms(node* targetNode);											// Clears all frames of a node and its childrens transforms
	void balanceKeyFrames(std::vector<transform>* previous, std::vector<transform>* next);  // Previous key frame and next key frame need to have transforms 
																							//  pointing to the same node in each frame to perform calculations
	int findNextKeyFrame();																	// Finds the next key frame after the current frame
	int findFramesSincePreviousKeyFrame();													// Find how many frames after the previous key frame the
																							//  current frame is
	void sortFrame();																		// Sorts a frame's transforms by tree depth
	void incrementNodeID();																	// Increments nodeID
	void flattenTree(std::vector<node*>* flatTree, node* target);							// Recursively populates target vector with all the nodes in the scene graph

	// Getters
	viewPort* getViewPort();
	node* getRoot();
	node* getCurrentNode();
	std::vector<std::vector<transform>*>* getFrames();
	std::vector<transform>* getCurrentFrame();	
	std::vector<transform>* getPreviousKeyFrame();
	std::vector<transform>* getNextKeyFrame();
	bool getTween();
	Fl_Tree* getTree();
	int getNodeID();

	// Setters
	void setFrames(std::vector<std::vector<transform>*>* f);
	void setCurrentFrame(std::vector<transform>* f);
	void setPreviousKeyFrame(std::vector<transform>* f);

	// Callbacks
	static void newNodeCB(Fl_Widget* w, void* data);			// Add a new node to the current node in a new display
	static void editNodeCB(Fl_Widget* w, void* data);			// Edit current node in a new display
	static void moveNodeCB(Fl_Widget* w, void* data);			// Moves node up or down in parent's children vector
	static void deleteNodeCB(Fl_Widget* w, void* data);			// Delete current node and all children from the scene graph
	static void openFileCB(Fl_Widget* w, void* data);			// Load a saved animation
	static void saveFileCB(Fl_Widget* w, void* data);			// Save an animation to the file name given in the input box
	static void treeCB(Fl_Widget* w, void* data);				// Select a tree item and node through tree navigation
	static void transformCB(Fl_Widget* w, void* data);			// Transform the current node or modify its position
	static void frameSliderCB(Fl_Widget* w, void* data);		// Select frame by slider
	static void previousFrameCB(Fl_Widget* w, void* data);		// Set current frame to the one before this
	static void playFrameCB(Fl_Widget* w, void* data);			// Toggle playing the animation
	static void nextFrameCB(Fl_Widget* w, void* data);			// Set current frame to the one after this
	static void insertFrameCB(Fl_Widget* w, void* data);		// Insert a new frame to the left of the current frame
	static void deleteFrameCB(Fl_Widget* w, void* data);		// Delete current frame
	static void clearFrameCB(Fl_Widget* w, void* data);			// Clear frame of all transformations
	static void saveTweenCB(Fl_Widget* w, void* data);			// Saves the current frames tween transforms as a key frame
	static void tweenCB(Fl_Widget* w, void* data);				// Toggle tweening between frames
	static void frameCounterCB(Fl_Widget* w, void* data);		// Modify the number of frames in the animation
	static void fpsCB(Fl_Widget* w, void* data);				// Modify the fps
	static void playCB(void* data);								// Plays the animation
};

#endif