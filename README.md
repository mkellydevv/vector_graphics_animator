# Vector Graphics Animator

__Summary :__

In this program, the user can construct and animate scenes using vector graphics and frame tweening.  The scene graph is held in a node tree data structure where each node holds a vector graphic polygon and pointers to its sub-nodes.  Animations are stored as a series of key frames where transforms have been applied to nodes.  During playback, intermediate frames are linearly interpolated between key frames allowing a smooth transition of position, translation, rotation, scale, and color.

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
