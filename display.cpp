#include "display.h"

display::display(int x, int y, int w, int h, char* c) : Fl_Window(x, y, w, h, c)
{
	view = new viewPort(10, 10, 500, 500);
	root = new node();
	currentNode = root;
	frames = new std::vector<std::vector<transform>*>;
	frames->push_back(new std::vector<transform>);
	currentFrame = frames->front();
	previousKeyFrame = frames->front();

	loadWidgets();
}

display::~display()
{
	delete view;
	root->deleteNode();							// Delete all children of root
	delete root;
	for (int i = 0; i < frames->size(); i++)	// Delete all frames held by frames
		delete frames->operator[](i);
	delete frames;

	// Delete widgets
	delete newNodeButton;
	delete editNodeButton;
	delete moveNodeUpButton;
	delete moveNodeDownButton;
	delete deleteNodeButton;
	delete openFileButton;
	delete saveFileButton;
	delete fileNameInput;
	delete tree;
	delete currentTreeItem;
	delete colorChooser;
	delete rotationDial;
	delete positionXCounter;
	delete positionYCounter;
	delete translationXCounter;
	delete translationYCounter;
	delete scaleXCounter;
	delete scaleYCounter;
	delete frameSlider;
	delete previousFrameButton;
	delete playFrameButton;
	delete nextFrameButton;
	delete insertFrameButton;
	delete deleteFrameButton;
	delete clearFrameButton;
	delete saveTweenButton;
	delete tweenButton;
	delete frameCounter;
	delete fpsCounter;
}

void
display::loadWidgets()
{
	newNodeButton = new Fl_Button(740, 10, 90, 30, "New Node");
	newNodeButton->box(FL_EMBOSSED_BOX);
	newNodeButton->callback(this->newNodeCB, this);

	editNodeButton = new Fl_Button(740, 50, 90, 30, "Edit Node");
	editNodeButton->box(FL_EMBOSSED_BOX);
	editNodeButton->callback(this->editNodeCB, this);

	moveNodeUpButton = new Fl_Button(740, 90, 90, 30, "Node Up");
	moveNodeUpButton->box(FL_EMBOSSED_BOX);
	moveNodeUpButton->callback(this->moveNodeCB, this);

	moveNodeDownButton = new Fl_Button(740, 130, 90, 30, "Node Down");
	moveNodeDownButton->box(FL_EMBOSSED_BOX);
	moveNodeDownButton->callback(this->moveNodeCB, this);

	deleteNodeButton = new Fl_Button(740, 170, 90, 30, "Delete Node");
	deleteNodeButton->box(FL_EMBOSSED_BOX);
	deleteNodeButton->callback(this->deleteNodeCB, this);

	openFileButton = new Fl_Button(740, 210, 90, 30, "Open File");
	openFileButton->box(FL_EMBOSSED_BOX);
	openFileButton->callback(this->openFileCB, this);

	saveFileButton = new Fl_Button(740, 250, 90, 30, "Save File");
	saveFileButton->box(FL_EMBOSSED_BOX);
	saveFileButton->callback(this->saveFileCB, this);

	fileNameInput = new Fl_Input(740, 290, 90, 30);

	tree = new Fl_Tree(530, 10, 200, 400);
	tree->callback(this->treeCB, this);
	populateTree(root, tree->root());
	tree->select(tree->first()->next());
	tree->showroot(0);
	
	colorChooser = new Fl_Color_Chooser(530, 420, 100, 90);
	colorChooser->callback(this->transformCB, this);
	
	rotationDial = new Fl_Dial(650, 420, 75, 75, "Rotation:");
	rotationDial->callback(this->transformCB, this);
	rotationDial->bounds(-1, 1);

	positionXCounter = new Fl_Simple_Counter(530, 520, 100, 30, "Position: X");
	positionXCounter->step(.5);
	positionXCounter->callback(this->transformCB, this);

	positionYCounter = new Fl_Simple_Counter(640, 520, 100, 30, "Position: Y");
	positionYCounter->step(.5);
	positionYCounter->callback(this->transformCB, this);

	translationXCounter = new Fl_Simple_Counter(530, 570, 100, 30, "Translation: X");
	translationXCounter->step(.5);
	translationXCounter->callback(this->transformCB, this);

	translationYCounter = new Fl_Simple_Counter(640, 570, 100, 30, "Translation: Y");
	translationYCounter->step(.5);
	translationYCounter->callback(this->transformCB, this);

	scaleXCounter = new Fl_Simple_Counter(530, 620, 100, 30, "Scale: X");
	scaleXCounter->step(1.1);
	scaleXCounter->callback(this->transformCB, this);

	scaleYCounter = new Fl_Simple_Counter(640, 620, 100, 30, "Scale: Y");
	scaleYCounter->step(1.1);
	scaleYCounter->callback(this->transformCB, this);

	frameSlider = new Fl_Value_Slider(100, 520, 410, 30);
	frameSlider->type(FL_HOR_NICE_SLIDER);
	frameSlider->step(1);
	frameSlider->minimum(1);
	frameSlider->maximum(1);
	frameSlider->value(1);
	frameSlider->callback(this->frameSliderCB, this);
	frameSlider->do_callback();

	previousFrameButton = new Fl_Button(10, 520, 30, 30, "@|<");
	previousFrameButton->box(FL_EMBOSSED_BOX);
	previousFrameButton->callback(this->previousFrameCB, this);

	playFrameButton = new Fl_Button(40, 520, 30, 30, "@>");
	playFrameButton->box(FL_EMBOSSED_BOX);
	playFrameButton->callback(this->playFrameCB, this);

	nextFrameButton = new Fl_Button(70, 520, 30, 30, "@>|");
	nextFrameButton->box(FL_EMBOSSED_BOX);
	nextFrameButton->callback(this->nextFrameCB, this);

	insertFrameButton = new Fl_Button(10, 550, 30, 30, "@+");
	insertFrameButton->box(FL_EMBOSSED_BOX);
	insertFrameButton->callback(this->insertFrameCB, this);

	deleteFrameButton = new Fl_Button(40, 550, 30, 30, "@+19+");
	deleteFrameButton->box(FL_EMBOSSED_BOX);
	deleteFrameButton->callback(this->deleteFrameCB, this);

	clearFrameButton = new Fl_Button(70, 550, 30, 30, "Clr");
	clearFrameButton->labelfont(FL_BOLD);
	clearFrameButton->box(FL_EMBOSSED_BOX);
	clearFrameButton->callback(this->clearFrameCB, this);

	saveTweenButton = new Fl_Button(100, 550, 30, 30, "+T");
	saveTweenButton->labelfont(FL_BOLD);
	saveTweenButton->box(FL_EMBOSSED_BOX);
	saveTweenButton->callback(this->saveTweenCB, this);

	tweenButton = new Fl_Check_Button(130, 550, 30, 30, "Tween Toggle");
	tweenButton->value(0);
	tweenButton->callback(this->tweenCB, this);

	frameCounter = new Fl_Simple_Counter(10, 620, 55, 30, "# Frames");
	frameCounter->step(1);
	frameCounter->value(1);
	frameCounter->callback(this->frameCounterCB, this);

	fpsCounter = new Fl_Simple_Counter(80, 620, 55, 30, "FPS");
	fpsCounter->step(1);
	fpsCounter->value(fps);
	fpsCounter->callback(this->fpsCB, this);

	playing = false;
	Fl::add_timeout(1 / fps, playCB, this);
}

void
display::playToggle()
{
	playing = !playing;
}

void
display::tweenToggle()
{
	tween = !tween;
}

void
display::populateTree(node* targetNode, Fl_Tree_Item* targetItem)
{
	Fl_Tree_Item* item;
	item = tree->add(targetItem, targetNode->getName().c_str());
	item->user_data((void *)targetNode);
	
	// Recursively add all children
	for (int i = 0; i < targetNode->getChildren()->size(); i++)
		populateTree(targetNode->getChildren()->operator[](i), item);
}

void
display::clearChildrenTransforms(node* nodeTarget)
{
	// Clear transforms from frames that point to this node and its children
	for (int i = 0; i < frames->size(); i++)
	{
		for (int j = 0; j < frames->operator[](i)->size(); j++)
		{
			if (frames->operator[](i)->operator[](j).nodePtr == nodeTarget)
				frames->operator[](i)->erase(frames->operator[](i)->begin() + j);
		}
	}

	// Recursively clear all children
	for (int i = 0; i < nodeTarget->getChildren()->size(); i++)		
		clearChildrenTransforms(nodeTarget->getChildren()->operator[](i));
}

void 
display::balanceKeyFrames(std::vector<transform>* previous, std::vector<transform>* next)
{
	// Check that for every transform in previous key frame, a transform pointing to the same node exists in next key frame
	bool found = false;
	for (int i = 0; i < previous->size(); i++)
	{
		for (int j = 0; j < next->size(); j++)
		{
			if (previous->operator[](i).nodePtr == next->operator[](j).nodePtr)
				found = true;
		}
		if (found == false)
		{
			transform temp;
			temp.nodePtr = previous->operator[](i).nodePtr;
			next->push_back(temp);
		}
		found = false;
	}

	// Check that for every transform in next key frame, a transform pointing to the same node exists in previous key frame
	found = false;
	for (int i = 0; i < next->size(); i++)
	{
		for (int j = 0; j < previous->size(); j++)
		{
			if (next->operator[](i).nodePtr == previous->operator[](j).nodePtr)
				found = true;
		}
		if (found == false)
		{
			transform temp;
			temp.nodePtr = next->operator[](i).nodePtr;
			previous->push_back(temp);
		}
		found = false;
	}
}

int
display::findNextKeyFrame()
{
	// Finds the next key frame after the current frame
	int count = 0;
	bool startCounting = false;

	if (previousKeyFrame == currentFrame)				// Current frame is a key frame, so return 0
		return count;

	for (int i = 0; i < frames->size(); i++)			// Iterate through frames and start counting when previous key frame has been found
	{
		if (startCounting == true)
		{
			count += 1;
			if (frames->operator[](i)->size() > 0)		// Return count when next key frame is found
			{
				nextKeyFrame = frames->operator[](i);
				return count;
			}
		}
		if (frames->operator[](i) == previousKeyFrame)	// Start counting when we have reached previous key frame
			startCounting = true;
	}

	return -1;											// Return -1 if there are no more key frames after the current frame
}

int
display::findFramesSincePreviousKeyFrame()
{
	// Find how many frames after the previous key frame the current frame is
	int count = 0;
	bool startCounting = false;

	if (previousKeyFrame == currentFrame)					// Current frame is a key frame, so return 0
		return count;

	for (int i = 0; i < frames->size(); i++)
	{
		if (startCounting == true)
		{
			count += 1;
			if (frames->operator[](i) == currentFrame)		// Return count when current frame is found
				return count;
		}
		if (frames->operator[](i) == previousKeyFrame)		// Start counting when we have reached previous key frame
			startCounting = true;
	}
	return -1;												// Should not be able to return this
}

void
display::sortFrame()
{
	// Sort a frame's transforms by tree depth.  Lower depth transforms should be applied before higher depth transforms
	// This is a simple bubble sort algorithm
	for (int i = 0; i < currentFrame->size(); i++)
	{
		for (int j = 0; j < currentFrame->size() - 1; j++)
		{
			if (currentFrame->operator[](j).nodePtr->getTreeDepth() < currentFrame->operator[](j + 1).nodePtr->getTreeDepth())
				std::swap(currentFrame->operator[](j), currentFrame->operator[](j + 1));
		}
	}
}

void
display::incrementNodeID()
{
	nodeID += 1;
}

void
display::flattenTree(std::vector<node*>* flatTree, node* target)
{
	if (target != root)
		flatTree->push_back(target);

	// Recursively add all children
	for (int i = 0; i < target->getChildren()->size(); i++)
		flattenTree(flatTree, target->getChildren()->operator[](i));
}

viewPort*
display::getViewPort()
{
	return view;
}

node*
display::getRoot()
{
	return root;
}

node*
display::getCurrentNode()
{
	return currentNode;
}

std::vector<std::vector<transform>*>*
display::getFrames()
{
	return frames;
}

std::vector<transform>*
display::getCurrentFrame()
{
	return currentFrame;
}

std::vector<transform>*
display::getPreviousKeyFrame()
{
	return previousKeyFrame;
}

std::vector<transform>*
display::getNextKeyFrame()
{
	return nextKeyFrame;
}

bool
display::getTween()
{
	return tween;
}

Fl_Tree*
display::getTree()
{
	return tree;
}

int
display::getNodeID()
{
	return nodeID;
}

void
display::setFrames(std::vector<std::vector<transform>*>* f)
{
	frames = f;
}

void
display::setCurrentFrame(std::vector<transform>* f)
{
	currentFrame = f;
}

void
display::setPreviousKeyFrame(std::vector<transform>* f)
{
	previousKeyFrame = f;
}

void
display::newNodeCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	nodeDisplay* nodeDisp = new nodeDisplay(425, 150, 275, 275, "New Node", disp, 0);
	nodeDisp->show();
}

void
display::editNodeCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	if (disp->currentNode->getShape() != Root)
	{
		nodeDisplay* nodeDisp = new nodeDisplay(425, 150, 275, 275, "Edit Node", disp, 1);
		nodeDisp->show();
	}
}

void
display::moveNodeCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	Fl_Widget* widget = w;

	if (disp->currentNode->getShape() != Root)
	{
		node* par = disp->currentNode->getParent();

		if (widget == disp->moveNodeUpButton)
		{
			for (int i = 0; i < par->getChildren()->size(); i++)
			{
				if (par->getChildren()->operator[](i) == disp->currentNode)
				{
					if (i > 0)
						std::swap(par->getChildren()->operator[](i), par->getChildren()->operator[](i - 1));
				}
			}
		}
		else
		{
			for (int i = 0; i < par->getChildren()->size(); i++)
			{
				if (par->getChildren()->operator[](i) == disp->currentNode)
				{
					if (i != par->getChildren()->size() - 1)
					{
						std::swap(par->getChildren()->operator[](i), par->getChildren()->operator[](i + 1));
						i = par->getChildren()->size();		// Exit the for loop or else the node will be pushed to the bottom
					}
				}
			}
		}

		// Repopulate tree
		disp->tree->clear_children(disp->tree->root());
		disp->populateTree(disp->root, disp->tree->root());
		disp->tree->select(disp->tree->root()->next());

		disp->tree->redraw();
		disp->view->redraw();
	}
}

void
display::deleteNodeCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	
	if (disp->currentNode->getShape() != Root)		// Do not operate on root
	{
		// Clear current node's transforms from all frames, as well as its children's
		disp->clearChildrenTransforms(disp->currentNode);

		// Clear scenegraph of node
		disp->currentNode->deleteNode();
		disp->currentNode = disp->root;

		// Repopulate tree
		disp->tree->clear_children(disp->tree->root());
		disp->populateTree(disp->root, disp->tree->root());
		disp->tree->select(disp->tree->root()->next());

		disp->tree->redraw();
		disp->view->redraw();
	}
}

void
display::openFileCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;

	// Create the file chooser and show it
	Fl_File_Chooser fileChooser("./Animations", "*.{txt}", Fl_File_Chooser::SINGLE, "Open File");
	fileChooser.show();

	// Block until user picks something.
	while (fileChooser.shown())
		Fl::wait();

	if (fileChooser.value() != NULL)
	{
		std::string line;
		std::ifstream file(fileChooser.value());

		if (file.is_open())
		{
			// Number of frames in the animation
			std::getline(file, line);
			int numFrames = stoi(line);

			// FPS of the animation
			std::getline(file, line);
			float fps = stof(line);

			// Last used unique node ID
			std::getline(file, line);
			int nodeID = stoi(line);

			// Clear scene graph
			disp->getRoot()->deleteNode();

			// Clear frames
			for (int i = 0; i < disp->getFrames()->size(); i++)
				delete disp->getFrames()->operator[](i);
			delete disp->getFrames();

			// Clear tree
			disp->tree->clear_children(disp->tree->root());

			// Initialize frames
			disp->setFrames(new std::vector<std::vector<transform>*>);
			for (int i = 0; i < numFrames; i++)
				disp->getFrames()->push_back(new std::vector<transform>);
			disp->frameCounter->value(numFrames);
			disp->frameSlider->maximum(numFrames);
			disp->frameSlider->value(1);
			disp->frameSlider->do_callback();
			disp->setCurrentFrame(disp->getFrames()->front());
			disp->setPreviousKeyFrame(disp->getFrames()->front());

			// Initialize FPS
			disp->fps = fps;
			disp->fpsCounter->value(fps);

			// Initialize nodeID
			disp->nodeID = nodeID;

			// Add nodes to a temporary vector
			std::vector<node*> tempNodes;
			std::getline(file, line);
			std::string delimiter = ";";
			size_t pos = 0;
			std::string token;
			while (line != "Transforms")
			{
				// New node's ID
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				int ID = stoi(token);
				line.erase(0, pos + delimiter.length());

				// New node's parent ID
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				int parentID = stoi(token);
				line.erase(0, pos + delimiter.length());

				// New node's color
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				gVector3 color = gVector3(token);
				line.erase(0, pos + delimiter.length());

				// New node's draw mode
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				int drawMode = stoi(token);
				line.erase(0, pos + delimiter.length());

				// New node's name
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				std::string name = token;
				line.erase(0, pos + delimiter.length());

				// New node's vertices
				std::vector<gVector3> vertices;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					gVector3 vertex = gVector3(token);
					vertices.push_back(vertex);
					line.erase(0, pos + delimiter.length());
				}

				// Initialize new node and add it to the temporary vector
				node* newNode = new node(vertices, color, drawMode, name);
				newNode->setID(ID);
				newNode->setParentID(parentID);
				tempNodes.push_back(newNode);

				// Get next line
				std::getline(file, line);
			}

			// Arrange nodes into a scene graph
			for (int i = 0; i < tempNodes.size(); i++)
			{
				for (int j = 0; j < tempNodes.size(); j++)
				{
					if (tempNodes[i]->getParentID() == tempNodes[j]->getID())
						tempNodes[j]->addChild(tempNodes[i]);
				}
				if (tempNodes[i]->getParentID() == 0)
					disp->root->addChild(tempNodes[i]);
			}

			// Add transforms to the frames
			while (std::getline(file, line))
			{
				pos = 0;
				while ((pos = line.find(delimiter)) != std::string::npos)
				{
					transform newTransform;

					// Key frame this transform belongs to
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					int frameNumber = stoi(token) - 1;
					line.erase(0, pos + delimiter.length());

					// The node this transform points to
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					for (int i = 0; i < tempNodes.size(); i++)
					{
						if (tempNodes[i]->getID() == stoi(token))
							newTransform.nodePtr = tempNodes[i];
					}
					line.erase(0, pos + delimiter.length());

					// Translation Row 1
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					gVector3 row1 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Translation Row 2
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					gVector3 row2 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Translation Row 3
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					gVector3 row3 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Apply translation to the new transform
					newTransform.translation = gMatrix3(row1, row2, row3);

					// Rotation Row 1
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row1 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Rotation Row 2
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row2 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Rotation Row 3
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row3 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Apply Rotation to the new transform
					newTransform.rotation = gMatrix3(row1, row2, row3);

					// Scale Row 1
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row1 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Scale Row 2
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row2 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Scale Row 3
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					row3 = gVector3(token);
					line.erase(0, pos + delimiter.length());

					// Apply Scale to the new transform
					newTransform.scale = gMatrix3(row1, row2, row3);

					// Apply Color
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					gVector3 col = gVector3(token);
					line.erase(0, pos + delimiter.length());
					newTransform.color = col;

					disp->frames->operator[](frameNumber)->push_back(newTransform);
				}
			}

			file.close();
		}

		// Cull directory from file name
		std::string fileName = (std::string)fileChooser.value();
		std::string delimiter = "/";
		size_t pos = 0;
		std::string token;
		while ((pos = fileName.find(delimiter)) != std::string::npos) {
			token = fileName.substr(0, pos);
			fileName.erase(0, pos + delimiter.length());
		}

		// Set file name input to the culled file name
		disp->fileNameInput->value(fileName.c_str());

		disp->currentNode = disp->root;

		// Repopulate tree
		disp->populateTree(disp->root, disp->tree->root());
		disp->tree->select(disp->tree->root()->next());

		disp->fileNameInput->redraw();
		disp->view->redraw();
		disp->tree->redraw();
	}
}

void
display::saveFileCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;

	std::string fileName = "./Animations/" + (std::string)disp->fileNameInput->value();

	std::ofstream file(fileName);

	// 1st line - Number of frames in the animation
	file << disp->frameCounter->value() << "\n";

	// 2nd Line - FPS of the animation
	file << disp->fps << "\n";

	// 3rd Line - Unique identifiers used so far by animation
	file << disp->nodeID << "\n";

	// Line .. Line - Nodes stored in a line using the following format - nodeID parentID color drawMode name vertex .. vertex
	if (disp->root->getChildren()->size() > 0)
	{
		std::vector<node*>* flatTree = new std::vector<node*>;
		disp->flattenTree(flatTree, disp->root);

		for (int i = 0; i < flatTree->size(); i++)
		{
			file << flatTree->operator[](i)->getID() << ";" << flatTree->operator[](i)->getParent()->getID() << ";" <<
				flatTree->operator[](i)->getOriginalColor().operator[](0) << "," << flatTree->operator[](i)->getOriginalColor().operator[](1) << "," <<
				flatTree->operator[](i)->getOriginalColor().operator[](2) << ",;" << flatTree->operator[](i)->getDrawMode() << ";" << flatTree->operator[](i)->getName() <<
				";";

			for (int j = 0; j < flatTree->operator[](i)->getVertices().size(); j++)
				file << flatTree->operator[](i)->getVertices().operator[](j).operator[](0) << "," << flatTree->operator[](i)->getVertices().operator[](j).operator[](1) << ",;";

			file << "\n";
		}
		delete flatTree;
	}

	// Marker Line - Simple marker to show that frame transforms will now begin
	file << "Transforms\n";

	// Line .. Line - Frame transforms stored in a line using the following format - frame# nodeID Trow1 Trow2 Trow3 Rrow1 Rrow2 Rrow3 Srow1 Srow2 Srow3 color
	if (disp->frames->size() > 0)
	{
		for (int i = 0; i < disp->frames->size(); i++)
		{
			if (disp->frames->operator[](i)->size() > 0)
			{
				for (int j = 0; j < disp->frames->operator[](i)->size(); j++)
				{
					file << i + 1 << ";" << disp->frames->operator[](i)->operator[](j).nodePtr->getID() << ";" <<
						disp->frames->operator[](i)->operator[](j).translation[0][0] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[0][1] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[0][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).translation[1][0] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[1][1] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[1][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).translation[2][0] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[2][1] << "," <<
						disp->frames->operator[](i)->operator[](j).translation[2][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).rotation[0][0] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[0][1] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[0][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).rotation[1][0] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[1][1] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[1][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).rotation[2][0] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[2][1] << "," <<
						disp->frames->operator[](i)->operator[](j).rotation[2][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).scale[0][0] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[0][1] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[0][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).scale[1][0] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[1][1] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[1][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).scale[2][0] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[2][1] << "," <<
						disp->frames->operator[](i)->operator[](j).scale[2][2] << ",;" <<
						disp->frames->operator[](i)->operator[](j).color[0] << "," <<
						disp->frames->operator[](i)->operator[](j).color[1] << "," <<
						disp->frames->operator[](i)->operator[](j).color[2] << ",;\n";
				}
			}
		}
	}
}

void
display::treeCB(Fl_Widget* w, void* data) 
{
	display* disp = (display*)data;
	Fl_Tree* tree = (Fl_Tree*)w;
	Fl_Tree_Item* item = tree->callback_item();
	if (item && tree->callback_reason() == FL_TREE_REASON_SELECTED)
	{
		// Set current node to tree items associated node
		disp->currentNode = (node*)item->user_data();
		if (disp->currentNode != disp->root)
			disp->colorChooser->rgb(disp->currentNode->getCurrentColor()[0], disp->currentNode->getCurrentColor()[1], disp->currentNode->getCurrentColor()[2]);
	}
}

void
display::transformCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	Fl_Widget* widget = w;

	bool found = false;
	if (disp->currentNode->getShape() != Root)		// Do not operate on root
	{
		// Initialize a new gVector3 if the color is modified, or a new gMatrix3 for transforms
		gVector3 newColor;
		gMatrix3 newTransform;

		if (w == disp->colorChooser)
			newColor = gVector3(disp->colorChooser->r(), disp->colorChooser->g(), disp->colorChooser->b());
		else if (w == disp->rotationDial)
			newTransform = gMatrix3::rotation2D(disp->rotationDial->value());
		else if (w == disp->positionXCounter)
			newTransform = gMatrix3::translation2D(disp->positionXCounter->value(), 0);
		else if (w == disp->positionYCounter)
			newTransform = gMatrix3::translation2D(0, disp->positionYCounter->value());
		else if (w == disp->translationXCounter)
			newTransform = gMatrix3::translation2D(disp->translationXCounter->value(), 0);
		else if (w == disp->translationYCounter)
			newTransform = gMatrix3::translation2D(0, disp->translationYCounter->value());
		else if (w == disp->scaleXCounter)
		{
			if (disp->scaleXCounter->value() < 0)
				disp->scaleXCounter->value(.9);
			newTransform = gMatrix3::scale2D(disp->scaleXCounter->value(), 1);
		}
		else if (w == disp->scaleYCounter)
		{
			if (disp->scaleYCounter->value() < 0)
				disp->scaleYCounter->value(.9);
			newTransform = gMatrix3::scale2D(1, disp->scaleYCounter->value());
		}
		
		// If position is modified, the nodes base position is translated and a new transform is not added to this frame
		if (w == disp->positionXCounter || w == disp->positionYCounter)
		{
			std::vector<gVector3> tempVertices = disp->currentNode->getVertices();
			for (int i = 0; i < tempVertices.size(); i++)
				tempVertices[i] = newTransform * tempVertices[i];
			disp->currentNode->setVertices(tempVertices);
			disp->currentNode->setCentroid(disp->currentNode->findCentroid());
		}
		else
		{
			// If this frame is not a key frame, make it a key frame by inheriting transforms from previous key frame
			if (disp->currentFrame != disp->previousKeyFrame)
			{
				if (disp->previousKeyFrame->size() > 0)
				{
					for (std::vector<transform>::iterator i = disp->previousKeyFrame->begin(); i != disp->previousKeyFrame->end(); i++)
						disp->currentFrame->push_back(*i);
					disp->previousKeyFrame = disp->currentFrame;
				}
			}

			// Iterate through the transforms in this frame to see if the current node has previously been transformed.  Apply new transform if so
			if (disp->currentFrame->size() > 0)
			{
				for (std::vector<transform>::iterator i = disp->currentFrame->begin(); i != disp->currentFrame->end(); i++)
				{
					if (i->nodePtr == disp->currentNode)
					{
						if (w == disp->colorChooser)
							i->color = newColor;
						else if (w == disp->rotationDial)
							i->rotation = i->rotation * newTransform;
						else if (w == disp->translationXCounter || w == disp->translationYCounter)
							i->translation = i->translation * newTransform;
						else if (w == disp->scaleXCounter || w == disp->scaleYCounter)
							i->scale = i->scale * newTransform;

						found = true;
					}
				}
			}

			// Add a new transform to current frame if current node has previously not been transformed
			if (found == false)
			{
				transform temp;
				if (w == disp->colorChooser)
					temp.color = newColor;
				else if (w == disp->rotationDial)
					temp.rotation = newTransform;
				else if (w == disp->translationXCounter || w == disp->translationYCounter)
					temp.translation = newTransform;
				else if (w == disp->scaleXCounter || w == disp->scaleYCounter)
					temp.scale = newTransform;
				temp.nodePtr = disp->currentNode;
				disp->currentFrame->push_back(temp);
			}
		}
		disp->frameSlider->do_callback();

		disp->frameSlider->redraw();
		disp->view->redraw();
	}

	// Reorder current frame transforms
	disp->sortFrame();

	// Reset widgets
	disp->positionXCounter->value(0);
	disp->positionYCounter->value(0);
	disp->translationXCounter->value(0);
	disp->translationYCounter->value(0);
	disp->scaleXCounter->value(0);
	disp->scaleYCounter->value(0);

	// Redraw whichever widget issued this callback
	widget->redraw();
}

void
display::frameSliderCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;

	// Set current frame to frame slider value
	int i = disp->frameSlider->value() - 1;
	disp->currentFrame = disp->getFrames()->operator[](i);

	// Find most recent key frame
	while (disp->getFrames()->operator[](i)->size() == 0 && i != 0)
		i -= 1;
	disp->previousKeyFrame = disp->getFrames()->operator[](i);

	// Color the frame slider selector black if the current frame is a key frame
	if (disp->previousKeyFrame == disp->currentFrame)
	{
		if (disp->currentFrame == disp->getFrames()->front() && disp->currentFrame->size() == 0)
			disp->frameSlider->selection_color(49);
		else
			disp->frameSlider->selection_color(0);
	}
	else
		disp->frameSlider->selection_color(49);

	disp->frameSlider->redraw();
	disp->view->redraw();
}

void
display::previousFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	if (disp->frameSlider->value() > 1)
		disp->frameSlider->value(disp->frameSlider->value() - 1);
	else
		disp->frameSlider->value(disp->frameSlider->maximum());		// Wrap around
	disp->frameSlider->do_callback();
}

void
display::playFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->playToggle();
	if (disp->playing)
		disp->playFrameButton->label("@||");
	else
		disp->playFrameButton->label("@>");
}

void
display::nextFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	if (disp->frameSlider->value() != disp->frameSlider->maximum())
		disp->frameSlider->value(disp->frameSlider->value() + 1);
	else
		disp->frameSlider->value(1);		// Wrap around
	disp->frameSlider->do_callback();
}

void
display::insertFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	int i = disp->frameSlider->value() - 1;
	disp->frameCounter->value(disp->frameCounter->value() + 1);
	disp->frameSlider->maximum(disp->frameCounter->value());
	std::vector<std::vector<transform>*>::iterator it = disp->getFrames()->begin();
	disp->getFrames()->insert(it + i, new std::vector<transform>);

	disp->frameSlider->value(disp->frameSlider->value() + 1);
	disp->frameSlider->do_callback();
}

void
display::deleteFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	if (disp->currentFrame != disp->getFrames()->front())
	{
		int i = disp->frameSlider->value() - 1;
		disp->frameCounter->value(disp->frameCounter->value() - 1);
		if (disp->frameSlider->value() == disp->frameSlider->maximum())
			disp->frameSlider->value(disp->frameSlider->value() - 1);
		disp->frameSlider->maximum(disp->frameCounter->value());
		std::vector<std::vector<transform>*>::iterator it = disp->getFrames()->begin();
		delete disp->getFrames()->operator[](i);
		disp->getFrames()->erase(it + i);
		disp->frameSlider->do_callback();
	}
	else
		printf("Can not delete first frame.\n");
}

void
display::clearFrameCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	int i = disp->frameSlider->value() - 1;
	disp->getFrames()->operator[](i)->clear();
	disp->frameSlider->do_callback();
}

void
display::saveTweenCB(Fl_Widget* w, void* data)
{
	display* disp = (display*)data;

	if (disp->tween = true && disp->currentFrame->size() == 0 && 0 < disp->view->getTweenTransforms()->size())
	{
		for (int i = 0; i < disp->view->getTweenTransforms()->size(); i++)
			disp->currentFrame->push_back(disp->view->getTweenTransforms()->operator[](i));
		
		disp->previousKeyFrame = disp->currentFrame;
		disp->frameSlider->do_callback();
	}
}

void
display::tweenCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->tweenToggle();
	disp->view->redraw();
}

void
display::frameCounterCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;

	// If the final keyFrame is being used to hold transforms, we can not reduce # of keyFrames
	if (disp->getFrames()->back()->size() > 0 && disp->frameCounter->value() < disp->frameSlider->maximum())
	{
		printf("Can not reduce number of frames. Last frame is in use.\n");
		disp->frameCounter->value(disp->frameCounter->value() + 1);
	}
	else
	{
		if (disp->frameCounter->value() >= 1)
		{
			// Adds or deletes a frame to the end of frames
			if (disp->frameSlider->maximum() > disp->frameCounter->value())
				disp->getFrames()->pop_back();  // I'm aware
			else
				disp->getFrames()->push_back(new std::vector<transform>);

			// Change the maximum number of frames
			disp->frameSlider->maximum(disp->frameCounter->value());

			// If the final frame was removed, move the cursor to the new final frame
			if (disp->frameSlider->maximum() == disp->frameSlider->value() - 1)
				disp->frameSlider->value(disp->frameCounter->value());
		}
		// There must always be at least 1 frame
		else
			disp->frameCounter->value(1);
	}
	disp->frameSlider->do_callback();
}

void
display::fpsCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	if (disp->fpsCounter->value() > 0)
		disp->fps = disp->fpsCounter->value();
	else
	{
		disp->fps = 1;
		disp->fpsCounter->value(disp->fps);
	}
}

void
display::playCB(void *data)
{
	display* disp = (display*)data;

	if (disp->playing){
		disp->nextFrameButton->do_callback();
		disp->view->redraw();
	}
	Fl::repeat_timeout(1 / disp->fps, playCB, disp);
}