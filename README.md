# Vector Graphics Animator

__Summary :__

This program allows the user to construct a scene graph and create animations with said scene graph.  The scene graph is tree data structure consisting of nodes which represent polygons.  Any transformation applied to a parent is applied to all its children in its section of the tree.  If a transformation is applied to the scene graph, the currently selected frame becomes a key frame.  The animation uses the most recent key frame to display the scene graph or calculates tween intermediaries and applies them to the scene graph if the option is selected.

__Libraries & Technologies Used :__
* FLTK - Fast Light Toolkit is a cross-platform C++ GUI toolkit.

__Controls :__
* New Node - Add a new node to the scene graph
* Edit Node - Edit a node from the scene graph
* Node Up/Down - Changes the draw order for nodes in the scene graph
* Delete Node - Deletes a node from the scene graph and its transforms from frames
* Open File - Load a saved animation
* Save File - Save an animation to a .txt file
* Translation/Rotation/Scale/Color Widgets - Apply a transform to a node at a specific frame
* Position Widget - Change the absolute position of a node
* Frame Slider - Select a frame
* Previous/Next Frame Buttons - Go to previous/next frame
* Play Button - Plays the animation
* + Button - Insert a frame to the left of current frame
* X Button - Delete currently selected frame
* Clr Button - Clear all transforms in the currently selected frame
* +T Button - Save current tween transforms as a key frame
* Tween Toggle - Tweens between key frames
* Frames/FPS Counters - Modify # of frames and/or FPS of the animation
