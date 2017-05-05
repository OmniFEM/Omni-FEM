#ifndef MODELDEFINITION_H_
#define MODELDEFINITION_H_

#include <vector>
#include <algorithm>
#include <string>
#include <math.h>

#include <glew.h>
#include <freeglut.h>

#include <wx/wx.h>
#include <wx/gdicmn.h>

#include <common/ProblemDefinition.h>
#include <common/GridPreferences.h>
#include <common/wxGLString.h>
#include <common/plfcolony.h>

#include <common/GeometryProperties/NodeSettings.h>

#include <UI/GeometryDialog/BlockPropertyDialog.h>
#include <UI/GeometryDialog/NodalSettingDialog.h>
#include <UI/GeometryDialog/SegmentPropertyDialog.h>
#include <UI/GeometryDialog/ArcSegmentDialog.h>
#include <UI/GeometryDialog/EditGroupDialog.h>

#include <UI/ModelDefinition/OGLFT.h>

#include <UI/geometryShapes.h>
#include <UI/GeometryEditor2D.h>
#include <UI/common.h>


//! The model class the handles the dispaying of objects
/*!
    This class is designed to handle the bulk of dispalying the geometry onto a glCanvas.
    This class will handle the drawing of any nodes/arcs/lines/labels. Along
    with any zooming, mouse moving, dragging, snapping to grid, and selecting geometry objects.
    Other functionality in the class include creating copies of selected geometry and moving selected geometry.
    THe modelDefinition inherits from the wxGLCanvas class because the modelDefinition class
    handles all direct calls for openGL.
    Refer to the following link for documentation of wxGLCanvas class:
     
    http://docs.wxwidgets.org/3.1.0/classwx_g_l_canvas.html
*/ 
class modelDefinition : public wxGLCanvas
{
private:

    //! This is the context which will be associated to the class.
    /*!
        A context is used for the glCanvans becuase there needs to be a record
        of the state of an openGL machince. This also servces as a connection
        between openGL and the canvas.
        For further documentation of the wxGLCanvas object, refer to the following link:
        http://docs.wxwidgets.org/3.1.0/classwx_g_l_context.html
    */ 
	wxGLContext *_geometryContext;
    
    //! This is the address for the problem definition contained in the main frame object.
    /*!
        This is simply a local variable for easier access to the state of Omni-FEM.
        This couldn't be a direct copy because the modelDefinition would need to be able to 
        see any changes that the user makes while to program is running. With this being a pointer,
        we are now pointing to the address which will allow the user to make changes to the top level
        and the changes will properaget downward
     */ 
    problemDefinition *_localDefinition;
    
    //! The local variable containing all of the grid preferences.
    /*! 
        For a detailed explanation, refer to the apprioate documentation for the gridPreferences class.
        In short, this variable stores the current state of the grid. The state of if the grid should be drawn,
        if geometry should be snapped to the grid, if block labels should be displayed, etc. All of these 
        status variables are stored in this variable which is access through out the modelDefinition class
    */ 
    gridPreferences _preferences;
    
    //! The local variable containing all of the code for calculating anything related to the geometry.
    /*!
        For a detailed explanation, refer to the documentation for the geometryEditor2D class.
        In short, this variable contains all of the code necessary for calculating any geometric functions.
        If there is an intersection between the arcs or lines or node or block labels.
        This variable also contains the data structures which hold the lists of the geometry shapes.
        Anytime a draw event occurs or the class needs to loop through any combination of the geometry
        shapes, this variable is accessed
     */ 
    geometryEditor2D _editor;
    
    //! The variable that contains the zoom factor in the x direction.
    /*!
        Zooming could be taken as one variable. However, having two variables for zooming
        allows a greater ability for configuring the glViewport especially for the zoom window.
        Still, in many cases in order to convert a two variable zoom factor into one, the 
        program takes the average between the x and the y zoom factors
     */ 
    double _zoomX = 1.0;
    
    //! The variable that contains the zoom factor in the y-direction.
    /*
        See the documentation for the _zoomX variable for a detailed description
     */ 
    double _zoomY = 1.0;
    
    //! The varable that contains the viewport offset in the x-direction.
    /*!
        In order to emulate panning across an infinite grid, the program uses glTranslate function.
        Every time that a paint event is fired, the program will alwys reset itself back to the default position 
        which is the view port being centered at 0,0 with coordinate number 1 above, -1 below, 1 to the right, and -1 to the left.
        Then, the program will translate the viewport _cameraX and _cameraY number of steps to the proper location.
        For more documentation on this matter, refer to the function updateProjection(). THis function implements the idea.
     */ 
    double _cameraX = 0;
    
    //! The varibale that contains the viewport offset in the y-direction.
    /*!
        For a detailed explaniation refer to the documentation for the _cameraX variable
     */ 
    double _cameraY = 0;
    
    //! Holds the pixel value for hte previous location of the mouse in the x-direction
    /*!
        This variable is not used for much except for calculating the _cameraX/_cameraY values 
        when the user pans across the canvas. In the calculation, the program looks to find the difference
        between the current mouse position and the previous mouse position (which is this variable) and from there 
        determines how much to increment the _cameraX/_cameraY values by. For a more detailed description of this,
        refer to the documentation of for the function onMouseMove(wxMouseEvent &event).
        As a side note, this is the pixel value and not the coordiante value.
    */ 
    int _mouseXPixel = 0;
    
    //! Holds the pixel value for the previous location of the mouse in the y-Direction
    /*!
        Refer to the documentation for the _mouseXPixel for a detailed description of this variable
     */ 
    int _mouseYPixel = 0;
    
    //! A status flag to indicate to the program to perform a zoom window
    /*
        When the user clicks on the zooom window, this boolean becomes true. When it is true,
        the program's abaility to create nodes/lines/arcs will be overridden to create a 
        zoom window which is a dotted line. On the up release of the left mouse button,
        the program will check if this boolean is true and if so, resize the glviewport and 
        the projection matrix in order to match the zoom window that the user created.
        After completetion, this variable will be set back to false.
     */ 
    bool _doZoomWindow = false;
    
    //! A status flag to indicate to the program that a mirror operation will be performed
    /*!
        When the user would like perform a mirror operation on selected geometry, this 
        status boolean will be set to true. Like the _doZoomWindow flag, this flag will override the
        program's abaility to create geometry in favor of creating the mirror line.
        On the release of the right mouse button, if this boolean is set to true, the program
        will execute the mirror function and after completetion, will reset this variable back
        to false.
    */ 
    bool _doMirrorLine = false;
    
    //! A status flag to draw a selection box rectangle
    /*!
        When a user wants to select a group of geometry objects, this status flag will be set to true.
        Like the flags _doMirrorLine and _doZoomWindow, this will override the program's ability to
        create geometry objects. On the up release of the right mouse button, the program will
        check if this flag is true and if so, execute the necessary functions in order to select a group
        of geometry objects within the window. This flag will be set back to false upon the completetion 
        of this process
     */ 
    bool _doSelectionWindow = false;
    
    //! This is a status variable which will indicate to the program whether or not the user is creating nodes or block labels
    /*!
        This is the status varaible that the simulator uses in order to determine if the user will be creating nodes or block labels
        This variable does not effect the users abaility to select nodes or block labels. This variable is primarily used when the user
        presses down on the left mouse button. This variable will also effect what geometry is selected when the user performs a left up motion
        on the selection window. The variable is true when the user needs to create nodes and false when the user needs to create block labels
    */ 
    bool _createNodes = true;
    
    //! This is a status variable that will indicate to the program whether or not the user will create lines/arcs
    /*!
        This variable has the same function and purpose as the _createNodes variable except for this variable applies to lines and arcs.
        This variable effects whetehr lines or arcs are selected when the user performs a left down motion when creating a selection window
        This variable is true when the user whats to create lines and false when the user wants to create arcs
     */ 
    bool _createLines = true;
    
    //! A status variable which tells the program that nodes have been selected.
    /*!
        When a user clicks the right mouse button, if the mouse pointer is above a node, then the this variable 
        will be set to true to keep track of which geometry objects are selected. THis is helpful in functions such as
        mirroring geometry, translating geometry, and copying the geometry. This boolean will only be set to false when 
        all nodes are deselected or if another geometry is selected that is different from the nodes.
        Also, the selection window process will effect this variable
     */ 
    bool _nodesAreSelected = false;
    
    //! A status variable that tells the program that the user is selecting or has selected lines
    /*!
        For a detailed description, see the documentation for _nodesAreSelected. The purpose
        and function of this status variable is the same for the status variable _nodesAreSelected
    */ 
    bool _linesAreSelected = false;
    
    //! A status variable that tells the program that the user is selecting or has selected arcs.
    /*!
        For a detailed description, see the documentation for _nodesAreSelected. The purpose
        and function of this status variable is the same for the status variable _nodesAreSelected
    */ 
    bool _arcsAreSelected = false;
    
    //! A status variable that tells the program that the user is selecting or has selected block labels.
    /*!
        For a detailed description, see the documentation for _nodesAreSelected. The purpose
        and function of this status variable is the same for the status variable _nodesAreSelected
    */ 
    bool _labelsAreSelected = false;
    
    //! A status variable that tells the program that a mix of the geometry shapes has been selected.
    /*!
        For a detailed description, see the documentation for _nodesAreSelected. The purpose
        and function of this status variable is the same for the status variable _nodesAreSelected.
        However, this variable is set to true only if a mixture of geometry is selected. This would occur during the
        group selection process where the user creates a window where the endpoint is greater then the start point 
        in the x-direction. When a mix of geometry is selected, this variable becomes true.
    */ 
    bool _geometryGroupIsSelected = false;
    
    //! A status flag that is set to true for if a node is selected
    /*!
        This selection differs from the _nodesAreSelected variable becuase this status flag
        is set to true only when the user selects a node that is to be used for creating lines/arcs
     */ 
    bool _geometryIsSelected = false;
    
    //! This variable specifies the starting point of the a mirror line/zoom window/selection box
    /*!
        This variable is only used when the user would creates a mirror line, a zoom window box, or a selection box.
        This variable is not updated when the mouse moves over the canvas. The variable is first set to a value when the user
        presses down on the left mouse button. After this, the variable is set. When the mouse moves over the canvas,
        the _endPoint variable is updated. After a mirror,zoom window, or selection box function has been executed,
        the variable is reset back to the point (0, 0). The datatype is wxRealPoint becuase this stores the coordinate position
        of the mouse for the graph instead of the pixel coordinate.
     */ 
    wxRealPoint _startPoint;
   
    //! This variable specifices the end point of a mirror line/zoom window/selection box.
    /*!
        The purpose of the variable is to store the end point of a mirror line/zoom window/selection box.
        The variable is updated whenever the mouse is moved across the canvas. At the conclusion of the function
        executing either the mirror line/zoom window box/selection box, this variable is reset back to (0, 0). Again,
        the datatype is a wxRealPoint becuase this stores the coordinate position inside of the variable.
        \sa _startPoint
     */ 
    wxRealPoint _endPoint;
    
    //! The variable that contains all of the related functions for font render
    /*!
        The instance of the font rendering engine OGLFT. This handles all of the importing of the fonts and the 
        actual rendering of the font. For documentation regarding the OGLFT library, refer to the following link:
         
        http://oglft.sourceforge.net/
    */ 
    OGLFT::Grayscale *_fontRender;
    
    //! A function that converts the x pixel coordinate into a cartesian/polar coordinate
    /*!
        This function takes into account the _zoomX parameter and the _cameraX parameter. This fucntion also takes into account the aspect ratio of the screen
        in order to properly size the gird. In simple form, we multiply the pixel value by two and then divide by the screen width. Subtract the result by 1 and multiply
        this result by the aspect ratio and the zoom factor. Last, we add in the camera offset in order to obtain the mappping between the pixel coordinate plane and 
        the cartensian coordinate plane. The reason that this function works with the polar form is that the mapping is exactly the same; hwoever, the meaning of the 
        coordinate value has changed from cartesian to polar coordinate.
        \param xPixel The pixel value that needs to be converted into the cartensian plane. This is for the x-plane and ranges from 0 to the screen width.
        \return Returns the cartensian/polar coordinate of the pixel value.
     */ 
    double convertToXCoordinate(int xPixel)
    {
        return _zoomX * (((2.0 / (double)this->GetSize().GetWidth()) * ((double)xPixel - (double)this->GetSize().GetWidth() / 2.0)) / 1.0) * ((double)this->GetSize().GetWidth() / (double)this->GetSize().GetHeight()) + _cameraX;
    }
    
    //! A function that converts the y pixel coordinate into a cartesian/polar coordinate
    /*!
        This function performs the same way as the convertTOXCoordinate function. Except that this function does not take into account of the 
        aspect ratio since this is already taken into account by the convertToXCoordinate function. The math is a little reversed compared to the math
        for the convertToXCoordinate. Mathmatically speaking, this function will subtact the result of multipling the yPixel value by the half of the
        the canvas height from 1. Then multiply this result by the zoom Y factor and add in the camera Y offset.
        \param yPixel The pixel value that needs to be converted to the cartesian/polar plane. this is for the y-plane and ranges from 0 to the canvas height.
        \return Returns the cartesian/poalr coordiante of the y pixel value.
        \sa convertToXCoordinate
     */
    double convertToYCoordinate(int yPixel)
    {
        return _zoomY * ((-(2.0 / (double)this->GetSize().GetHeight()) * ((double)yPixel - (double)this->GetSize().GetHeight() / 2.0)) / 1.0) + _cameraY;
    }
    
    //! A function that will calculate the tolerance value used for selecting geometry shapes and creating the geometry
    /*!
        The tolerance is an imaginary buffer that gives the user a region in order to select a shape. The tolerance
        is also used in creating geometry. When a geometry shape is created (this primarily pertains to the block labels and nodes) 
        the program needs to make sure the the geomety is not created too close to another geometry in order to avoid convergence issues.
        The simulator uses this function in order to properly determine the "Keep out" area for the block labels and nodes.
        \return The function will return the tolerance value that reprents the width of the area that is either keep out or selectable.
     */
    double getTolerance()
    {
        return ((((_zoomX + _zoomY) / 2.0) / 25.0));
    }
    
    //! A function that will clear the selection of any selected geometries
    /*!
        The function will first check the state of the variables: _createNodes, _labelsAreSelected, _createLines, _arcsAreSelected, and _geometryGroupIsSelected.
        If the _createNodes flag is set to true, then the program will only loop through the entire node list and set the select status of the node to false.
        Same thing appplies to the _labelsAreSelected, _createLines, and _arcsAreSelected. Except they will effect their respectivey geometry shapes.
        If _geometryGroupIsSelected is set to true, then the program will loop through all geometry lists and deselect the geometry shape.
        The function will also set the variables _createNodes, _labelsAreSelected, _createLines, _arcsAreSelected, and _geometryGroupIsSelected to false 
        no matter what flag is set to true at the conclusion of the function
     */ 
    void clearSelection();
    
    //! Function that is called in order to update the user's view
    /*!
        Every time onPaintCanvas is called, this function is executed.
        This function will update the user's view of the scene.
        First, we size the viewport to be equal to the canvas height and width.
        Next, we clear the projection matrix and reset it according to the aspect ratio and the zoom factor.
        Lastly, we clear the modelmatrix and then shift the matrix according to the camera offset
     */ 
    void updateProjection();
    
    //! Function that will draw the grid on the screen. The grid consists of the grid markings, the grid axis, and the center.
    /*!
        This function will draw the grid markings, the axis for the grid along with the cneter crosshair in order to display where (0, 0)
        is for the frid. All of these are controlled by the user preference. For the grid preference, the function checks the variable
        _preferences in order to determine what should be drawn.
      
        Drawing the crosshair for the center point and drawing the axis is quite straight forward. Both function in a similiar manner.
        At the top of the drawGrid function, the coordinate min and max of the screen width and height is computed. Using the 4 numbers,
        the program is able to create the axis line. Where the x-axis line extends from 0 to the width of the canvas and the y-axis line
        extends from 0 to the height of the canvas. The min and max values are effected by the zoom and translation factors. In fact, 
        if you are to recompile the project with the glOrtho2D scaled to double what the code states, you will end up seeing the 
        grid axis move and zoom with the user. The reason the lines are not drawn on the screen when the user pans or zooms past the grid
        lines is that for both the x and y axis lines, neither take the other into account. For example, when the x line is drawn, the y value
        is zero and when the y-axis line is drawn, the x value is zero.
        
        Drawing the grid markings is not as straight forward. First, we check to see if the user has zoomed out toom much. If this is the 
        case, then the grid will be too dense for us to really see anything so don't bother to draw the grid. Next, we create the grid markings
        for the y-axis by calculating the number of times we need to loop through the code. For every 4th iteration, we need to draw the line 
        more bold in order to desginate this as a minor axis line. The program repeats this operation for the x-axis.
 
        \sa _preferences
     */ 
    void drawGrid();

    //! This function will take an x coordinate value and a y coordinate value and round the two values to the nearest grid marking
    /*! For the sake of the explanianation, imagine we are working with
     * a 1D problem and that the point is between two grid markings.
     * First, the program finds the modulus of the coordinate / gridStep.
     * What this tells the program is the "distance" from the lower grid makeing to the point.
     * If the point is beyond the halfway "mark" of two grid markings, the modulus will return a number 
     * greater then gridStep / 2. If the point is below the halfway "mark",
     * the program will simply take the point coordinate and subtract out the modulus.
     * And this is the rounded answer.
     * If the point is beyond the halfway "mark", the program will add the gridstep to the 
     * point and then subtract out the modulus.
     * For example, if a point is at 0.40 and the grid step size is 0.25, then the point 
     * is between 0.25 and 0.5. The modulus of 0.40 % 0.25 will be 0.15. Since this is 
     * greater then 0.25 / 2 = 0.125 (the halfway mark), we need to add 0.25 to 0.40 (0.25 + 0.4 = 0.65)
     * Subtracting the modulus of 0.15 yeilds 0.5. The correct number to round 0.4 up to.
     * The two parameters are passed in by reference.
     * \param xCoordinate The x coordinate of a point that will be rounded to the nearest grid marking
     * \param yCoordinate The y coordinate of a point that will be rounded to the nearest grid marking
     */ 
    void roundToNearestGrid(double &xCoordinate, double &yCoordinate);
    
    //! This is the event that is fired when the canvas is drawn or re-drawn
    /*!
        A very important function. Inside this function is all of the draw calls for the canvas.
        The canvas is redrawn by calling the function this->Refresh(). This will effectively 
        post a paintevent to the event handler which the computer will execute then this function.
        First, the function must set the context. This simply redirects all openGL commands to the canvas.
        Next, we need to create an instance of the wxPaintDC object becuase we are drawing to a canvas inside
        of an event procedure (For more documentation regarding the wxPaintDC class, refer to the following link:
        http://docs.wxwidgets.org/trunk/classwx_paint_d_c.html)
        Next, the program switches to the modelMatrix and deletes everything.
        Then, update the projection and draw the grid. The program will then loop through the entire
        node/line/arc/and label list and call their corresponding draw function to draw them on the canvas.
        For the label name that is displayed, it is required that the program pass the variable _fontRender into the draw call
        so that the function has access to all of the glyphs for drawing the text onto the screen.
        The function that actually draws everything to the screen is SwapBuffers().
        \param event A requirded event datatype needed for the event table to post the event function properyl and to route the event procedure to the correct function.
     */ 
	void onPaintCanvas(wxPaintEvent &event);
    
    //! The function that is called when a resize event occurs
    /*!
        Typically, a resize event occurs when the user resizes the main frame. In this case,
        the canvas needs to be resized with the main frame and in order to maintain consistency.
        This function handles the resize event for the canvas.
        For more documentation regarding the wxSizeEvent class, refer to the following documentation:
        http://docs.wxwidgets.org/3.1.0/classwx_size_event.html
        \param event A requirded event datatype needed for the event table to post the event function properyl and to route the event procedure to the correct function.
     */ 
    void onResize(wxSizeEvent &event);
    
    //! The event procedure that is executed every time the user moves the mouse wheel.
    /*!
        This function will query the GetWheelRotation from the event variable. This will determine if the mouse wheel moved counter-clock-wise or
        clock-wise. Depending on the status of getMouseZoomReverseState, a counter-clockwise motion will cause the canvas to zoom in and a clockwise rotation
        will cause the canvas to zoom out. If getMouseZoomReverseState is true, then the operation is reversed. The program changes the zoom variables (_zoomX and _zoomY)
        by a factor that is computed by 1.2^(GetWheelRotation/150). This value is multuplied by the current zoom factor (for x and y). The program will ensure that
        both zoom factors are equal to each other. If a user zoom windowed to a particular location, when the user zooms in or out of that location, the program 
        will reset the zoom factors back to equal to each other. The program will also calculate a small offset and add/subtract this from the _cameraX and _cameraY
        variables. This allows the program to pan towards/away from the current mouse position as the program is zooming in/out.
        \sa _cameraX, _cameraY, _zoomX, _zoomY
        \param event This variable is required for the event procedure to work properly. When called from the event table, this variable contains the datatype to retrieve how much the user rotated the mouse wheel
     */ 
    void onMouseWheel(wxMouseEvent &event);
    
    //! This is the function that is executed when the user moves the mouse pointer across the canvas
    /*!
        This is executed every time the user moves the mouse pointer across the canvas. THe program will check a series of status flags 
        and execute accordingly. The first flag that it checks is to see if the middel mouse button (the wheel button) is pressed down.
        This indicates that tehe user woudl like to pan across the canvas. Based on the change of pixels from the last event to the next,
        the program will calculate the shift for the _cameraX and _cameraY variables. The variables, _mousePixelX and _mousePixelY, store the
        last position of the mouse (in pixel units). The current position X and Y mouse pixels can be obtained from the function GetX() and GetY() located
        in the wxMouseEvent class. 
        If the middle mouse button is not pressed, the program will check to see if the left mouse button is pressed at the time that the mouse moved
        If so, the program will check if the canvas is to move a draggingNode/label or to update the position of the _endPoint becuase the user
        is drawing a mirror line or a selection window. 
        If the left mouse button is not pressed and the user has the right mouse button down when moving the mouse, the program will update the
        value of _endPoint variable. This occurs only if the program is in the mode to draw a selection window.
        Once the program is finished executing, the program will force the canvas to refresh the screen
        \sa _cameraX, _cameraY, _endPoint
        \param event This variable is required for the event procedure to function properly. The wxMouseEvent class stores the current mouse positon and the information needed to determine what mouse button is pressed at the time that the event is fired.
     */ 
    void onMouseMove(wxMouseEvent &event);
    
    void onMouseLeftDown(wxMouseEvent &event);
    
    void onMouseLeftUp(wxMouseEvent &event);
    
    void onMouseRightDown(wxMouseEvent &event);
    
    void onMouseRightUp(wxMouseEvent &event);
    
    void onKeyDown(wxKeyEvent &event);
    
    void onEnterWindow(wxMouseEvent &event)
    {
      //  this->SetFocus();
    }
    
    void doZoomWindow();
    
public:
    modelDefinition(wxWindow *par, const wxPoint &point, const wxSize &size, problemDefinition &definition);

    void setGridPreferences(gridPreferences &preferences)
    {
        _preferences = preferences;
        this->Refresh();
    }
    
    gridPreferences* getGridPreferences()
    {
        return &_preferences;
    }
    
    void setCreateNodeState(bool state)
    {
        _createNodes = state;
    }
    
    void setCreateLinesState(bool state)
    {
        _createLines = state;
    }
    
    bool getCreateNodeState()
    {
        return _createNodes;
    }
    
    bool getCreateLineState()
    {
        return _createLines;
    }
    
    void setZoomWindow(bool state)
    {
        _doZoomWindow = state;
    }
    
    bool getZoomWindow()
    {
        return _doZoomWindow;
    }
    
    void setMirrorLineState(bool state)
    {
        _doMirrorLine = state;
    }
    
    bool getMirrorLineState()
    {
        return _doMirrorLine;
    }
    
    void zoomIn()
    {
        _zoomX *= pow(1.2, -(300.0) / 150.0);
        _zoomY *= pow(1.2, -(300.0) / 150.0);
        
        if(_zoomX > _zoomY)
            _zoomY = _zoomX;
        else if(_zoomY > _zoomX)
            _zoomX = _zoomY;

        this->Refresh();
    }
    
    void zoomOut()
    {
        _zoomX *= pow(1.2, (300.0) / 150.0);
        _zoomY *= pow(1.2, (300.0) / 150.0);
        
        if(_zoomX > _zoomY)
            _zoomY = _zoomX;
        else if(_zoomY > _zoomX)
            _zoomX = _zoomY;
        
        this->Refresh();
    }
    
    void deleteSelection();
    
    //! This will allow the user to edit the settings for the particular node/label/arc/line. It is in this calss because this class has access to the master settings list
    void editSelection();
    
    //! If a properties name is changes or if the property is deleted, this will reset the properties
    void updateProperties(bool scanConductorProperty, bool scanNodalProperty, bool scanBoundaryProperty, bool scanMaterialProperty, bool scanCircuitProperty);
    
    void selectGroup(EditGeometry geometry, unsigned int groupNumber);
    
    void moveTranslateSelection(double horizontalShift, double verticalShift);
    
    void moveRotateSelection(double angularShift, wxRealPoint aboutPoint);
    
    void scaleSelection(double scalingFactor, wxRealPoint basePoint);
    
    void mirrorSelection(wxRealPoint pointOne, wxRealPoint pointTwo);
    
    void copyTranslateSelection(double horizontalShift, double verticalShift, unsigned int numberOfCopies);
    
    void copyRotateSelection(double angularShift, wxRealPoint aboutPoint, unsigned int numberOfCopies);
    
    void displayDanglingNodes();
    
    void createOpenBoundary(unsigned int numberLayers, double radius, wxRealPoint centerPoint, OpenBoundaryEdge boundaryType);
    
    //! pointOne is the point that for the boundaing box that is the highest left most point. Pointtwo is hte lowest right most point 
    void getBoundingBox(wxRealPoint &pointOne, wxRealPoint &pointTwo);
    
    void createFillet(double filletRadius);
    
private:
    //! This is a macro in order to let wxWidgets understand that there are events withing the class
    wxDECLARE_EVENT_TABLE(); 
};

#endif