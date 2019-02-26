#ifndef NODE_H
#define NODE_H

#include "gMatrix3.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

class node;		// Forward declaration

// Shape is used to name nodes if a name is not given as a parameter
enum shape {
	Tri, Quad, Poly, Root
};

// Transform holds the transforms for a node at a specific frame
struct transform
{
	gMatrix3 translation = gMatrix3::identity();
	gMatrix3 rotation = gMatrix3::identity();
	gMatrix3 scale = gMatrix3::identity();
	gVector3 color = gVector3::nullVector();
	node* nodePtr;
};

class node
{
private:
	node* parent;							// Nodes are stored in a tree structure, they contain pointers to their parent and children
	std::vector<node*>* children;
	std::vector<gVector3> vertices;			// Each node represents a polygon which is stored as a vector of vertices(gVector3)
	std::vector<gVector3> currentVertices;	// Polygon vertices after they have been transformed
	gVector3 centroid;						// Centroid is used to translate a polygon so that it rotates around the polygon's geometric center
	std::string name;						// Nodes have names to help navigate the tree in the tree widget
	int ID;									// Unique identifier for load and save operations
	int parentID;							// Parent's unique identifier
	shape nodeShape;						// Every node will fall into the categories of Tri, Quad, Poly, or Root
	int drawMode;							// Draw mode determines how openGL draws the polygon. 1: GL_POLYGON, 2: GL_LINE_LOOP, 3: GL_LINES
	int treeDepth;							// Depth of the node in the tree structure.  Used in determining transform application order
	gVector3 originalColor;					// The node's original color that it will always remember
	gVector3 currentColor;					// The node's current color used at drawtime

public:
	node();																		// Constructor for a root node
	node(std::vector<gVector3> verts, gVector3 col, int mode);					// Constructor for children nodes without names
	node(std::vector<gVector3> verts, gVector3 col, int mode, std::string n);	// Constructor for children nodes with names
	~node();																	// Destructor
	
	// Methods //
	gVector3 findCentroid();											// Calculates the centroid for this node
	void normalizeVertices();											// Sets currentVertices to the original untransformed vertices
	void applyTransforms(transform tran, gVector3 cent, bool first);	// Applies a transform struct to this node's current transforms and all its childrens
	void addChild(node* child);											// Add a child to this node
	void deleteNode();													// Deletes this node and all its children. If node is root, only deletes children

	// Getters
	node* getParent();
	std::vector<node*>* getChildren();
	std::vector<gVector3> getVertices();
	std::vector<gVector3> getCurrentVertices();
	gVector3 getCentroid();
	std::string getName();
	int getID();
	int getParentID();
	shape getShape();
	int getDrawMode();
	int getTreeDepth();
	gVector3 getOriginalColor();
	gVector3 getCurrentColor();
	
	// Setters
	void setParent(node* par);
	void setVertices(std::vector<gVector3> verts);
	void setCentroid(gVector3 newCentroid);
	void setName(std::string n);
	void setID(int i);
	void setParentID(int i);
	void setDrawMode(int m);
	void setTreeDepth(int d);
	void setOriginalColor(gVector3 col);
	void setCurrentColor(gVector3 col);
};

#endif