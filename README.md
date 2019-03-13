# Vector Graphics Animator

__Summary :__

This program allows the user to construct a scene using vector graphics and to animate the scene using a set of key frames and tweening.  The scene graph is stored in a node tree structure where each node stores a vector graphic polgyon and pointers to children nodes.  Animations are stored as a series of key frames where transforms have been applied to nodes.  The most recent key frame is displayed until reaching another key frame unless tweening is turned on.  Tweening interpolates between the transforms of key frames allowing a smooth transition of position, translation, rotation, scale, and color.

__Demo Instructions :__
1.  To run the demo, you must have the Microsoft Visual C++ Redistributable for Visual Studio 2017 installed.
2.  Download the "Vector Graphics Animator - Demo.exe" file and the "Animations" folder from this project.
3.  Run the Demo.exe.

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
