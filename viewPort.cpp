#include "viewPort.h"
#include "display.h"

#define PI 3.14159265

viewPort::viewPort(int x, int y, int w, int h) : Fl_Gl_Window(x, y, w, h)
{
	tweenTransforms = new std::vector<transform>;
}

viewPort::~viewPort()
{
	delete tweenTransforms;
}

void
viewPort::draw()
{
	if (!valid())
	{
		valid(1);
		glViewport(0, 0, w(), h());
		glOrtho(-10, 10, -10, 10, -1, 1);
	}

	// Get a pointer to the FL display window
	Fl_Group* group = this->parent();
	display* disp = (display*)group;

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Current vertices must be reset before they are tweened
	disp->getRoot()->normalizeVertices();

	// If tweening is active, calculate tween transforms between key frames and apply them to the scene graph
	if (disp->getTween() == true)
	{
		// Returns -1 if there are no key frames after the current frame
		// Returns 0 if current frame is a key frame
		int framesBetweenKeyFrames = disp->findNextKeyFrame();

		// Current frame should be tweened if it is not a key frame and it exists between two key frames
		if (framesBetweenKeyFrames != -1 && framesBetweenKeyFrames != 0)
		{
			int framesSincePreviousKeyFrame = disp->findFramesSincePreviousKeyFrame();

			// Calculate the tween transforms for the current frame
			calculateTweenTransforms(framesBetweenKeyFrames, framesSincePreviousKeyFrame);

			// Apply tween transforms to the scene graph
			for (int i = 0; i < tweenTransforms->size(); i++)
				tweenTransforms->operator[](i).nodePtr->applyTransforms(tweenTransforms->operator[](i), tweenTransforms->operator[](i).nodePtr->getCentroid(), true);
			
		}
		// Use most recent key frame's transforms to apply transforms to scene graph
		else
		{
			for (int i = 0; i < disp->getPreviousKeyFrame()->size(); i++)
				disp->getPreviousKeyFrame()->operator[](i).nodePtr->applyTransforms(disp->getPreviousKeyFrame()->operator[](i), disp->getPreviousKeyFrame()->operator[](i).nodePtr->getCentroid(), true);
		}
	}
	// If tween is false, use most recent key frame's transforms to apply transforms to scene graph
	else
	{
		for (int i = 0; i < disp->getPreviousKeyFrame()->size(); i++)
			disp->getPreviousKeyFrame()->operator[](i).nodePtr->applyTransforms(disp->getPreviousKeyFrame()->operator[](i), disp->getPreviousKeyFrame()->operator[](i).nodePtr->getCentroid(), true);
	}
	
	// Recursively draw each node in the tree starting at root
	node* root = disp->getRoot();
	drawTree(root);
}

void
viewPort::drawTree(node* targetNode)
{
	if (targetNode->getShape() != Root)
	{
		// Draw color
		glColor3f(targetNode->getCurrentColor()[0], targetNode->getCurrentColor()[1], targetNode->getCurrentColor()[2]);

		// Draw mode
		if (targetNode->getDrawMode() == 1)
			glBegin(GL_POLYGON);					// Draw a filled polygon
		else if (targetNode->getDrawMode() == 2)
			glBegin(GL_LINE_LOOP);					// Draw a wireframe polygon
		else
			glBegin(GL_LINES);						// Draw a polyline

		for (int i = 0; i < targetNode->getCurrentVertices().size(); i++)
		{
			gVector3 vertex = targetNode->getCurrentVertices()[i];

			// Filled polygons and wireframe polygons are drawn here
			if (targetNode->getDrawMode() != 3)
				glVertex2f(vertex[0], vertex[1]);

			// Polylines are drawn here
			else
			{
				if (i == 0 || i == targetNode->getCurrentVertices().size())
					glVertex2f(vertex[0], vertex[1]);
				else
				{
					glVertex2f(vertex[0], vertex[1]);
					glVertex2f(vertex[0], vertex[1]);
				}
			}
		}

		glEnd();
	}
	
	// Recursively draw all children
	for (int i = 0; i < targetNode->getChildren()->size(); i++)
		drawTree(targetNode->getChildren()->operator[](i));

	// Reset the current color of the node after it has been drawn
	targetNode->setCurrentColor(targetNode->getOriginalColor());
}

void
viewPort::calculateTweenTransforms(int i, int j)
{
	int framesBetweenKeyFrames = i;
	int framesSincePreviousKeyFrame = j;

	// Get a pointer to the FL display window
	Fl_Group* group = this->parent();
	display* disp = (display*)group;

	// Clear previous tween transforms
	tweenTransforms->clear();

	std::vector<transform>* prev = disp->getPreviousKeyFrame();
	std::vector<transform>* next = disp->getNextKeyFrame();

	// Previous key frame and next key frame need to have transforms pointing to the same node in each frame to perform calculations
	disp->balanceKeyFrames(prev, next);

	// Populate this frame with tween transforms
	for (int i = 0; i < prev->size(); i++)
	{
		for (int j = 0; j < next->size(); j++)
		{
			// Calculate an intermediary transform between transforms in prev and next that point to the same node
			if (prev->operator[](i).nodePtr == next->operator[](j).nodePtr)
			{
				transform trans;
				trans.nodePtr = prev->operator[](i).nodePtr;

				// Translation intermediary
				float pX = prev->operator[](i).translation[0][2];
				float pY = prev->operator[](i).translation[1][2];
				float nX = next->operator[](j).translation[0][2];
				float nY = next->operator[](j).translation[1][2];
				float newX = pX + (framesSincePreviousKeyFrame * ((nX - pX) / framesBetweenKeyFrames));
				float newY = pY + (framesSincePreviousKeyFrame * ((nY - pY) / framesBetweenKeyFrames));

				trans.translation = gMatrix3::translation2D(newX, newY);

				// Rotation intermediary
				float previousAngle;
				float nextAngle;
				float newAngle;

				float cosDegrees = (acos(prev->operator[](i).rotation[0][0]) * 180) / PI;
				float sinDegrees = (asin(prev->operator[](i).rotation[1][0]) * 180) / PI;
				if (sinDegrees < 0)
					previousAngle = 360 - cosDegrees;
				else
					previousAngle = cosDegrees;

				cosDegrees = (acos(next->operator[](j).rotation[0][0]) * 180) / PI;
				sinDegrees = (asin(next->operator[](j).rotation[1][0]) * 180) / PI;
				if (sinDegrees < 0)
					nextAngle = 360 - cosDegrees;
				else
					nextAngle = cosDegrees;

				if (previousAngle < nextAngle)
				{
					if ((previousAngle + 180) < nextAngle)
					{
						previousAngle = previousAngle + 360;
						newAngle = previousAngle - (framesSincePreviousKeyFrame * ((previousAngle - nextAngle) / framesBetweenKeyFrames));
					}
					else
						newAngle = previousAngle + (framesSincePreviousKeyFrame * ((nextAngle - previousAngle) / framesBetweenKeyFrames));
				}
				else
				{
					if ((previousAngle - 180) > nextAngle)
					{
						nextAngle = nextAngle + 360;
						newAngle = previousAngle + (framesSincePreviousKeyFrame * ((nextAngle - previousAngle) / framesBetweenKeyFrames));
					}
					else
						newAngle = previousAngle - (framesSincePreviousKeyFrame * ((previousAngle - nextAngle) / framesBetweenKeyFrames));
				}

				trans.rotation = gMatrix3::rotation2D(newAngle);

				// Scale intermediary
				pX = prev->operator[](i).scale[0][0];
				pY = prev->operator[](i).scale[1][1];
				nX = next->operator[](j).scale[0][0];
				nY = next->operator[](j).scale[1][1];
				newX = pX + (framesSincePreviousKeyFrame * ((nX - pX) / framesBetweenKeyFrames));
				newY = pY + (framesSincePreviousKeyFrame * ((nY - pY) / framesBetweenKeyFrames));

				trans.scale = gMatrix3::scale2D(newX, newY);

				// Color intermediary
				gVector3 prevColor;
				gVector3 nextColor;
				gVector3 newColor;
				if (prev->operator[](i).color == gVector3::nullVector())
					prevColor = prev->operator[](i).nodePtr->getOriginalColor();
				else
					prevColor = prev->operator[](i).color;
				if (next->operator[](j).color == gVector3::nullVector())
					nextColor = next->operator[](j).nodePtr->getOriginalColor();
				else
					nextColor = next->operator[](j).color;
				newColor[0] = prevColor[0] + (framesSincePreviousKeyFrame * ((nextColor[0] - prevColor[0]) / framesBetweenKeyFrames));
				newColor[1] = prevColor[1] + (framesSincePreviousKeyFrame * ((nextColor[1] - prevColor[1]) / framesBetweenKeyFrames));
				newColor[2] = prevColor[2] + (framesSincePreviousKeyFrame * ((nextColor[2] - prevColor[2]) / framesBetweenKeyFrames));

				trans.color = newColor;

				// Add the new transform to tweenTransforms
				tweenTransforms->push_back(trans);
			}
		}
	}

	// Sort a tween transforms by tree depth.  Lower depth transforms should be applied before higher depth transforms
	// This is a simple bubble sort algorithm
	for (int i = 0; i < tweenTransforms->size(); i++)
	{
		for (int j = 0; j < tweenTransforms->size() - 1; j++)
		{
			if (tweenTransforms->operator[](j).nodePtr->getTreeDepth() < tweenTransforms->operator[](j + 1).nodePtr->getTreeDepth())
				std::swap(tweenTransforms->operator[](j), tweenTransforms->operator[](j + 1));
		}
	}
}

std::vector<transform>*
viewPort::getTweenTransforms()
{
	return tweenTransforms;
}