#include "node.h"

#define PI 3.14159265

node::node()
{
	nodeShape = Root;
	name = "Root";
	children = new std::vector<node*>;
	treeDepth = 0;
}

node::node(std::vector<gVector3> verts, gVector3 col, int mode)
{
	children = new std::vector<node*>;
	vertices = verts;
	currentVertices = verts;
	centroid = findCentroid();
	if (vertices.size() == 3)
	{
		nodeShape = Tri;
		name = "Tri";
	}
	else if (vertices.size() == 4)
	{
		nodeShape = Quad;
		name = "Quad";
	}
	else
	{
		nodeShape = Poly; 
		name = "Poly";
	}
	drawMode = mode;
	originalColor = col;
	currentColor = col;
}

node::node(std::vector<gVector3> verts, gVector3 col, int mode, std::string n)
{
	children = new std::vector<node*>;
	vertices = verts;
	currentVertices = verts;
	centroid = findCentroid();
	name = n;
	if (vertices.size() == 3)
		nodeShape = Tri;
	else if (vertices.size() == 4)
		nodeShape = Quad;
	else
		nodeShape = Poly;
	drawMode = mode;
	originalColor = col;
	currentColor = col;
}

node::~node()
{
	delete children;
}

gVector3
node::findCentroid()
{
	// Formula for centroid of a polygon from en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
	gVector3 newCentroid;
	float partialArea; 
	float signedArea = 0;

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		partialArea = (vertices[i][0] * vertices[i + 1][1]) - (vertices[i + 1][0] * vertices[i][1]);
		newCentroid[0] += (vertices[i][0] + vertices[i + 1][0]) * partialArea;
		newCentroid[1] += (vertices[i][1] + vertices[i + 1][1]) * partialArea;
		signedArea += partialArea;
	}

	int i = vertices.size() - 1;
	partialArea = (vertices[i][0] * vertices[0][1]) - (vertices[0][0] * vertices[i][1]);
	newCentroid[0] += (vertices[i][0] + vertices[0][0]) * partialArea;
	newCentroid[1] += (vertices[i][1] + vertices[0][1]) * partialArea;
	signedArea += partialArea;

	signedArea /= 2;
	newCentroid[0] /= signedArea * 6;
	newCentroid[1] /= signedArea * 6;

	return newCentroid;
}

void
node::normalizeVertices()
{
	currentVertices = vertices;

	for (int i = 0; i < children->size(); i++)
		children->operator[](i)->normalizeVertices();
}

void
node::applyTransforms(transform tran, gVector3 cent, bool first)
{
	// Clear current vertices
	std::vector<gVector3> tempVertices = currentVertices;
	currentVertices.clear();
	
	// Transform each vertex and readd it to currentVertices
	for (int i = 0; i < tempVertices.size(); i++)
	{
		gVector3 vertex = tempVertices[i];
		vertex = tran.scale * vertex;		// Scale the vertex
		vertex[0] = vertex[0] - cent[0];	// Translate vertex to make sure rotation happens around polygon's center
		vertex[1] = vertex[1] - cent[1];
		vertex = tran.rotation * vertex;	// Rotate the vertex
		vertex[0] = vertex[0] + cent[0];	// Translate vertex back to its original position
		vertex[1] = vertex[1] + cent[1];
		vertex = tran.translation * vertex;	// Translate the vertex

		currentVertices.push_back(vertex);
	}

	// Color transformation not applied to children
	if (tran.color != gVector3::nullVector() && first == true)
		currentColor = tran.color;

	// Recursively transform all children on this section of the tree
	for (int i = 0; i < children->size(); i++)
		children->operator[](i)->applyTransforms(tran, cent, false);
}

void
node::addChild(node* child)
{
	children->push_back(child);
	child->setParent(this);
	child->setTreeDepth(this->getTreeDepth() + 1);
}

void
node::deleteNode()
{
	if (nodeShape != Root)
	{
		// Erase this child from its parent
		for (int i = 0; i < parent->children->size(); i++)
		{
			if (parent->children->operator[](i) == this)
				parent->children->erase(parent->children->begin() + i);
		}

		// Recursively delete all children on this section of the tree
		while (children->size() > 0)
			children->front()->deleteNode();
		
		delete this;
	}
	else
	{
		// Recursively delete all children starting at root
		while (children->size() > 0)
			children->front()->deleteNode();
	}
}

node*
node::getParent()
{
	return parent;
}

std::vector<node*>*
node::getChildren()
{
	return children;
}

std::vector<gVector3>
node::getVertices()
{
	return vertices;
}

std::vector<gVector3>
node::getCurrentVertices()
{
	return currentVertices;
}

gVector3
node::getCentroid()
{
	return centroid;
}

std::string
node::getName()
{
	return name;
}

int
node::getID()
{
	return ID;
}

int
node::getParentID()
{
	return parentID;
}

shape
node::getShape()
{
	return nodeShape;
}

int
node::getDrawMode()
{
	return drawMode;
}

int
node::getTreeDepth()
{
	return treeDepth;
}

gVector3
node::getOriginalColor()
{
	return originalColor;
}

gVector3
node::getCurrentColor()
{
	return currentColor;
}

void
node::setParent(node* par)
{
	parent = par;
}

void
node::setVertices(std::vector<gVector3> verts)
{
	vertices = verts;
}

void
node::setCentroid(gVector3 newCentroid)
{
	centroid = newCentroid;
}

void
node::setName(std::string n)
{
	name = n;
}

void
node::setID(int i)
{
	ID = i;
}

void
node::setParentID(int i)
{
	parentID = i;
}

void
node::setDrawMode(int m)
{
	drawMode = m;
}

void
node::setTreeDepth(int d)
{
	treeDepth = d;
}

void
node::setOriginalColor(gVector3 col)
{
	originalColor = col;
}

void
node::setCurrentColor(gVector3 col)
{
	currentColor = col;
}