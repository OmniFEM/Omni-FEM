#ifndef OMNIFEMFrame_H_
#define OMNIFEMFrame_H_

#include <string.h>

#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/stdpaths.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

//#include <UI/problemDefinition.h>
#include <UI/geometryEditor2D.h>
#include <UI/common.h>
#include <UI/PropertiesDialog.h>
#include <UI/MainFrameAbstraction.h>
#include <UI/MaterialsDialog/MaterialDialog.h>
#include <UI/MainFrameController.h>

#include <common/BoundaryConditions.h>
#include <common/enums.h>



// For documenting code, see: https://www.stack.nl/~dimitri/doxygen/manual/docblocks.html

class OmniFEMApp : public wxApp
{
	private:
	
		wxSize minSize = wxSize(450, 340);
	
    public:
        virtual bool OnInit();
};






/*! \class OmniFEMMainFrame
 *  \brief The main class for everything
 * 
 *  This class is also the presentation layer of the main frame
 */
class OmniFEMMainFrame : public wxFrame
{
public:
    OmniFEMMainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
private:
	/***********************************
	* Prototypes for creating the menu *
	************************************/
	
    /* This section is for the File menu */
    void onNewFile(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void onSaveAs(wxCommandEvent &event);
	void onOpenFile(wxCommandEvent &event);
    
    /* This section is for the Edit menu */
	void onLuaRun(wxCommandEvent &event);
    void onPreferences(wxCommandEvent &event);
	
	/* This section is for the View Menu */
	void onViewResults(wxCommandEvent &event);
	
	/* This section is for the Problem Menu */
	void onPrecision(wxCommandEvent &event);
	
    /* This section is for the Grid Menu */
    void onDispGrid(wxCommandEvent &event);
    void onSnapGrid(wxCommandEvent &event);
    void onSetGrid(wxCommandEvent &event);
    
    /* Thos section is for the Properties Menu */
    void onMaterials(wxCommandEvent &event);
    void onBoundary(wxCommandEvent &event);
    void onPointProperty(wxCommandEvent &event);
    void onCircuitsConductor(wxCommandEvent &event);
    void onMatLibrary(wxCommandEvent &event);
    
	/* This section is for the Mesh menu */
	void onCreateMesh(wxCommandEvent &event);
	void onShowMesh(wxCommandEvent &event);
	void onDeleteMesh(wxCommandEvent &event);
    
    /* This section is for the Help menu */
	
	//! Event called to view the manual
    void onManual(wxCommandEvent &event);
	
	//! Event called to view the manual
    void onLicense(wxCommandEvent &event);
	
	//! Event called to view the manual
    void OnAbout(wxCommandEvent &event);
    
	//! Event called to view the manual
    void OnExit(wxCommandEvent &event);
	

 	/*****************************
	* Prototypes for client area *
	******************************/   
    
	//! Function that is called to begin the creating a new simulation
	/*
		This will be called in order for the user to choose the dimesnion of the simulation
	*/
    void createDimensionClient();
	
	
	//! Function that is called to create the initial client area
	void createInitialStartupClient();
	
	
	//! This function is called when the user is ready to choose their physics simulation
	void createProblemChoosingClient();
	
	
	//! This function is called to create the client for defining the physics problem
	/*
		Defining would be creating the geometry, creating the mesh, determining boundary conditions, etc.
	*/
	void createModelDefiningClient();
	
	
	//! This function is called when the user would like to view the results.
	/*
		When the view wants to view results, this function is called in order to create the client frame for viewing
		the results
	*/
	void createResultsViewingClient();
	
	/*************************
	* Prototypes for toolbar *
	**************************/
	
	//! Function called that will create the toolbar for the main frame
	void createTopToolBar();
	
    /*************************
	* Prototypes for buttons *
	**************************/
    
	//! Function called when the user chooses the two dim button
    void onTwoDimButton(wxCommandEvent &event);
	
	//! Function called when the back button is pressed 
	void onBackButton(wxCommandEvent &event);
	
	void onFinishButton(wxCommandEvent &event);
	
    /********************
	* Prototypes Others *
	*********************/	
	
	//! Event called when a resize event occurs
	void onResize(wxSizeEvent &event);	
	
	//! This is a function that will be for the initial state of Omni-FEM. With items on the toolbar greyed out and menus not accessible
	void enableToolMenuBar(bool enable);
	
	//! This is the function that is called when the combox box is clicked for choosing a physics problem
	void physicsProblemComboBox(wxCommandEvent &event);
	
	/************
	* Variables *
	*************/
	
	//! Stores the client size of the main window in the x direction
	int clientSizeWidth;
	
	//! Stores the client size of the main window in the y direction
	int clientSizeLength;
	
	//! The menu bar for the main window
	wxMenuBar *menuBar = new wxMenuBar;
	
	wxBoxSizer *groupOneSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vertBoxSizer = new wxBoxSizer(wxVERTICAL);
	
	//! This would be the file menu in the menu bar
    wxMenu *menuFile = new wxMenu;
	
	//! This would be the edit menu in the menu bar
    wxMenu *menuEdit = new wxMenu;
	
	//! This would be the view menu in the menu bar
    wxMenu *menuView = new wxMenu;
	
	//! This would be the problem menu in the menu bar
    wxMenu *menuProblem = new wxMenu;
	
    //! The Grid menu in the menu bar
    wxMenu *menuGrid = new wxMenu;
    
    //! The properties menu in the menu bar
    wxMenu *menuProperties = new wxMenu;
    
    //! This would be the mesh menu in the menu bar
    wxMenu *menuMesh = new wxMenu;
    
	//! This would be the help menu in the menu bar
    wxMenu *menuHelp = new wxMenu;
	
	//! This is the object for the toolbar of the main window
	wxToolBar *mainFrameToolBar = new wxToolBar();
	
	//! Creates the panel for the first screen
	wxPanel *initialStartPanel = new wxPanel();
	
	//! Panel for selecting the dimension
	wxPanel *dimSelectPanel;
	
	//! Panel for selecting the physics problem
	wxPanel *problemSelectPanel;
	
	//! This panel will be used in the model builder window. The purpose is to display status messages
	wxPanel *statusInfoPanel;
	
	//! This panel will be used in the problem defining state
	/*
		This object will be a list listing commonly acessed parameters. Such as listing
		the materials used, the geometry along with the different simualtions associated with teh project
	*/
	wxPanel *modelBuilderTreePanel;
	
	//! This panel will be used to draw the geometry on 
	wxPanel *geometryBuilderPanel;
	
	//! This panel will be used to dispaly settings that are selected
	wxPanel *settingsPanel;
	
	wxPanel *viewResultsPanel;
	
	//! Sets the mininimum size that the window for OMni-FEM is allowed to have
	wxSize minSize = wxSize(450, 340);
	
	//! The string of the physics problems that Omni-FEM can simulate
	wxArrayString arrayPhysicsProblem;
	
	
	OmniFEMMainFrameController controller;
	
	wxTreeCtrl *modelbuilderTreeCtrl;
	
	geometryEditor2DPresentation *twoDimGeometryEditor;
	
    wxDECLARE_EVENT_TABLE();
};












#endif /* OMNIFEM_H_ */