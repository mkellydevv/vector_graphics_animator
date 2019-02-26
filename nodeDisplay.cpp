#include "nodeDisplay.h"

nodeDisplay::nodeDisplay(int x, int y, int w, int h, char* c, display* d, int type) : Fl_Window(x, y, w, h, c)
{
	dis = d;
	target = d->getCurrentNode();
	displayType = type;
	// If this is an add node display
	if (displayType == 0)
	{
		drawMode = 1;
		originalColor = gVector3(1, 1, 1);		// Default color is white
	}
	// If this is an edit node display
	else if (displayType == 1)
	{
		vertices = target->getVertices();
		name = target->getName();
		drawMode = target->getDrawMode();
		originalColor = target->getOriginalColor();
	}

	loadWidgets();
}

nodeDisplay::~nodeDisplay()
{
	delete nodeNameInput;
	delete vertexInput;
	delete addVertexButton;
	delete verticesBox;
	delete colorChooser;
	delete typeSlider;
	delete clearVerticesButton;
	delete addPolyButton;
	delete widthInput;
	delete heightInput;
	delete addQuadButton;
}

void
nodeDisplay::loadWidgets()
{
	nodeNameInput = new Fl_Input(50, 10, 200, 30, "Name");
	if (displayType == 1)
		nodeNameInput->value(name.c_str());

	vertexInput = new Fl_Input(10, 50, 70, 30);

	addVertexButton = new Fl_Button(90, 50, 90, 30, "Add Vertex");
	addVertexButton->box(FL_EMBOSSED_BOX);
	addVertexButton->callback(this->addVertexCB, this);

	verticesBox = new Fl_Box(180, 50, 90, 30, "Vertices: 0");
	if (displayType == 1)
	{
		std::string tmp = "Vertices: " + std::to_string(vertices.size());
		verticesBox->copy_label(tmp.c_str());
	}

	colorChooser = new Fl_Color_Chooser(10, 90, 100, 90);
	if (displayType == 1)
		colorChooser->rgb(originalColor[0], originalColor[1], originalColor[2]);
	else
		colorChooser->rgb(1, 1, 1);
	colorChooser->changed();
	colorChooser->callback(this->colorCB, this);

	typeSlider = new Fl_Slider(150, 90, 100, 30, "Shaded");
	typeSlider->type(FL_HORIZONTAL);
	typeSlider->bounds(1, 3);
	typeSlider->step(1);
	if (displayType == 1)
		typeSlider->value(drawMode);
	typeSlider->callback(this->sliderCB, this);

	clearVerticesButton = new Fl_Button(150, 150, 100, 30, "Clear Vertices");
	clearVerticesButton->box(FL_EMBOSSED_BOX);
	clearVerticesButton->callback(this->clearVerticesCB, this);

	if (displayType == 0)
		addPolyButton = new Fl_Button(150, 190, 100, 30, "Add Poly");
	else
		addPolyButton = new Fl_Button(150, 190, 100, 30, "Confirm Poly");
	addPolyButton->box(FL_EMBOSSED_BOX);
	addPolyButton->callback(this->addPolyCB, this);

	widthInput = new Fl_Input(50, 190, 40, 30, "Width ");

	heightInput = new Fl_Input(50, 230, 40, 30, "Height");

	if (displayType == 0)
		addQuadButton = new Fl_Button(150, 230, 100, 30, "Add Quad");
	else
		addQuadButton = new Fl_Button(150, 230, 100, 30, "Comfirm Quad");
	addQuadButton->box(FL_EMBOSSED_BOX);
	addQuadButton->callback(this->addQuadCB, this);
}

void
nodeDisplay::addVertexCB(Fl_Widget* w, void* data)
{
	nodeDisplay* disp = (nodeDisplay*)data;

	// Parse the string
	std::string temp = disp->vertexInput->value();
	std::stringstream ss(temp);
	std::string buffer;
	std::vector<std::string> tokens;
	while (ss >> buffer)
		tokens.push_back(buffer);
	gVector3 vertex = gVector3(std::stof(tokens.front()), std::stof(tokens.back()), 1);

	// Add the vertex to this->vertices
	disp->vertices.push_back(vertex);

	// Update label with number of vertices
	std::string newLabel = "Vertices: ";
	newLabel = newLabel + std::to_string(disp->vertices.size());
	const char* arr = newLabel.c_str();
	disp->verticesBox->copy_label(arr);
	disp->vertexInput->value("");

	// Redraw label
	disp->verticesBox->redraw();
}

void
nodeDisplay::colorCB(Fl_Widget* w, void* data)
{
	nodeDisplay* disp = (nodeDisplay*)data;
	disp->originalColor = gVector3(disp->colorChooser->r(), disp->colorChooser->g(), disp->colorChooser->b());
	disp->colorChooser->redraw();
}

void
nodeDisplay::sliderCB(Fl_Widget* w, void* data)
{
	nodeDisplay* disp = (nodeDisplay*)data;
	std::string st;
	if (disp->typeSlider->value() == 1)
	{
		st = "Shaded";
		disp->drawMode = 1;
	}
	else if (disp->typeSlider->value() == 2)
	{
		st = "Outlined";
		disp->drawMode = 2;
	}
	else if (disp->typeSlider->value() == 3)
	{
		st = "Polyline";
		disp->drawMode = 3;
	}
	const char* arr = st.c_str();
	disp->typeSlider->copy_label(arr);

	disp->typeSlider->redraw();
}

void
nodeDisplay::clearVerticesCB(Fl_Widget* w, void* data)
{
	nodeDisplay* disp = (nodeDisplay*)data;
	
	disp->vertices.clear();

	std::string newLabel = "Vertices: 0";
	const char* arr = newLabel.c_str();
	disp->verticesBox->copy_label(arr);
	disp->vertexInput->value("");

	disp->verticesBox->redraw();
}

void
nodeDisplay::addPolyCB(Fl_Widget* w, void* data)
{
	nodeDisplay* disp = (nodeDisplay*)data;

	if (disp->vertices.size() > 1)
	{
		// New Node Display
		if (disp->displayType == 0)
		{
			node* newNode = new node(disp->vertices, disp->originalColor, disp->drawMode, disp->nodeNameInput->value());
			disp->dis->incrementNodeID();
			newNode->setID(disp->dis->getNodeID());
			disp->target->addChild(newNode);
		}
		// Edit Node Display
		else
		{
			disp->target->setVertices(disp->vertices);
			disp->target->setName(disp->nodeNameInput->value());
			disp->target->setDrawMode(disp->drawMode);
			disp->target->setOriginalColor(disp->originalColor);
		}

		// Repopulate tree
		disp->dis->getTree()->clear_children(disp->dis->getTree()->root());
		disp->dis->populateTree(disp->dis->getRoot(), disp->dis->getTree()->root());
		disp->dis->getTree()->select(disp->dis->getTree()->first()->next());

		// Redraw relevant widgets
		disp->dis->getTree()->redraw();
		disp->dis->getViewPort()->redraw();

		// Delete this display
		delete disp;
	}
	else
		printf("Nodes must have at least two vertices.\n");
}

void
nodeDisplay::addQuadCB(Fl_Widget *w, void *data)
{
	nodeDisplay* disp = (nodeDisplay*)data;

	float width = std::stof(disp->widthInput->value());
	float height = std::stof(disp->heightInput->value());

	if (disp->widthInput->value() > 0 && disp->heightInput->value() > 0)
	{
		// Add quad vertices based on width and height
		disp->vertices.clear();
		disp->vertices.push_back(gVector3(0, 0, 1));
		disp->vertices.push_back(gVector3(0, height, 1));
		disp->vertices.push_back(gVector3(width, height, 1));
		disp->vertices.push_back(gVector3(width, 0, 1));

		// New Node Display
		if (disp->displayType == 0)
		{
			node* newNode = new node(disp->vertices, disp->originalColor, disp->drawMode, disp->nodeNameInput->value());
			disp->dis->incrementNodeID();
			newNode->setID(disp->dis->getNodeID());
			disp->target->addChild(newNode);
		}
		// Edit Node Display
		else
		{
			disp->target->setVertices(disp->vertices);
			disp->target->setName(disp->nodeNameInput->value());
			disp->target->setDrawMode(disp->drawMode);
			disp->target->setOriginalColor(disp->originalColor);
		}

		// Repopulate tree
		disp->dis->getTree()->clear_children(disp->dis->getTree()->root());
		disp->dis->populateTree(disp->dis->getRoot(), disp->dis->getTree()->root());
		disp->dis->getTree()->select(disp->dis->getTree()->first()->next());

		// Redraw relevant widgets
		disp->dis->getTree()->redraw();
		disp->dis->getViewPort()->redraw();

		// Delete this display
		delete disp;
	}
	else
	{
		disp->widthInput->value("");
		disp->heightInput->value("");
		printf("Height and Width inputs must be greater than 0.\n");
	}	
}