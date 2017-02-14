#include <UI/PreferencesDialog.h>

preferencesDialog::preferencesDialog(magneticPreference &pref) : wxDialog(NULL, wxID_ANY, "Problem Definition")
{
    wxBoxSizer *probTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *lengthSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *freqSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *depthSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *solverPreSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *minAngleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *acSolverSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *footerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxBoxSizer *textSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *optionsSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *intermediateSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxFont *font = new wxFont(8.5, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    
    _magPreference = pref;
    
    probTypeNameArray->Add("Planar");
    probTypeNameArray->Add("Axisymmetric");
   
    lengthUnitsNameArray->Add("Inches");
    lengthUnitsNameArray->Add("Millimeters");
    lengthUnitsNameArray->Add("Centimeters");
    lengthUnitsNameArray->Add("Meters");
    lengthUnitsNameArray->Add("Mils");
    lengthUnitsNameArray->Add("Micrometers");
   
    acSolverNameArray->Add("Succ. Approx");
    acSolverNameArray->Add("Newton");
   
    wxStaticText *probTypeText = new wxStaticText(this, wxID_ANY, "Problem Type:", wxPoint(12, 15), wxSize(90, 13));
    probTypeText->SetFont(*font);
    problemTypeComboBox->Create(this, wxID_ANY, wxEmptyString, wxPoint(98, 12), wxSize(121, 21), *probTypeNameArray);
    problemTypeComboBox->SetFont(*font);
    problemTypeComboBox->SetSelection(0);
//    textSizer->Add(probTypeText);
//    optionsSizer->Add(problemTypeComboBox);
    probTypeSizer->Add(probTypeText, 0, wxCENTER | wxTOP | wxBOTTOM | wxLEFT, 6);
    probTypeSizer->Add(problemTypeComboBox, 0, wxCENTER | wxTOP | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *lengthUnitsText = new wxStaticText(this, wxID_ANY, "Length Units:", wxPoint(12, 42), wxSize(90, 13));
    lengthUnitsText->SetFont(*font);
    lengthUnitsComboBox->Create(this, wxID_ANY, wxEmptyString, wxPoint(98, 39), wxSize(121, 21), *lengthUnitsNameArray);
    lengthUnitsComboBox->SetFont(*font);
    lengthUnitsComboBox->SetSelection(0);
//    textSizer->Add(lengthUnitsText);
//    optionsSizer->Add(lengthUnitsComboBox);
    lengthSizer->Add(lengthUnitsText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    lengthSizer->Add(lengthUnitsComboBox, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *acSolverText = new wxStaticText(this, wxID_ANY, "AC Solver:", wxPoint(12, 69), wxSize(60, 13));
    acSolverText->SetFont(*font);
    acSolverComboBox->Create(this, wxID_ANY, wxEmptyString, wxPoint(98, 65), wxSize(121, 21), *acSolverNameArray);
    acSolverComboBox->SetFont(*font);
    acSolverComboBox->SetSelection(0);
    acSolverSizer->Add(acSolverText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    acSolverSizer->Add(30, 0, 0);
    acSolverSizer->Add(acSolverComboBox, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *freqText = new wxStaticText(this, wxID_ANY, "Frequency (Hz):", wxPoint(12, 95), wxSize(90, 13));
    freqText->SetFont(*font);
    frequencyTextCtrl->Create(this, wxID_ANY, std::to_string(_magPreference.getFrequency()), wxPoint(98, 92), wxSize(121, 20));
    frequencyTextCtrl->SetFont(*font);
    freqSizer->Add(freqText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    freqSizer->Add(8, 0, 0);
    freqSizer->Add(frequencyTextCtrl, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *depthText = new wxStaticText(this, wxID_ANY, "Depth:", wxPoint(12, 121), wxSize(39, 13));
    depthText->SetFont(*font);
    depthTextCtrl->Create(this, wxID_ANY, std::to_string(_magPreference.getDepth()), wxPoint(98, 118), wxSize(121, 20));
    depthTextCtrl->SetFont(*font);
    depthSizer->Add(depthText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    depthSizer->Add(59, 0, 0);
    depthSizer->Add(depthTextCtrl, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *solverPrecisionText = new wxStaticText(this, wxID_ANY, "Solver Precision:", wxPoint(12, 147), wxSize(98, 13));
    solverPrecisionText->SetFont(*font);
    solverPrecisionTextCtrl->Create(this, wxID_ANY, std::to_string(_magPreference.getPrecision()), wxPoint(98, 144), wxSize(121, 20));
    solverPrecisionTextCtrl->SetFont(*font);
    solverPreSizer->Add(solverPrecisionText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    solverPreSizer->Add(solverPrecisionTextCtrl, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
   
    wxStaticText *minAngleText = new wxStaticText(this, wxID_ANY, "Min Angle (deg):", wxPoint(12, 173), wxSize(90, 13));
    minAngleText->SetFont(*font);
    minAngleTextCtrl->Create(this, wxID_ANY, std::to_string(_magPreference.getMinAngle()), wxPoint(98, 170), wxSize(121, 20));
    minAngleTextCtrl->SetFont(*font);
    minAngleSizer->Add(minAngleText, 0, wxCENTER | wxBOTTOM | wxLEFT, 6);
    minAngleSizer->Add(7, 0, 0);
    minAngleSizer->Add(minAngleTextCtrl, 0, wxCENTER | wxBOTTOM | wxRIGHT, 6);
    
    wxStaticBoxSizer *commentSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Comments");
    commentSizer->GetStaticBox()->SetFont(*font);
    commentsTextCtrl->Create(commentSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxPoint(6, 19), wxSize(192, 87), wxTE_MULTILINE);
    commentsTextCtrl->SetFont(*font);
    commentSizer->Add(commentsTextCtrl);
    
    wxButton *okButton = new wxButton(this, wxID_OK, "Ok", wxPoint(63, 330), wxSize(75, 23));
    okButton->SetFont(*font);
    wxButton *cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(144, 330), wxSize(75, 23));
    cancelButton->SetFont(*font);
    footerSizer->Add(okButton, 0, wxTOP | wxBOTTOM | wxRIGHT, 6);
    footerSizer->Add(cancelButton, 0, wxRIGHT | wxBOTTOM | wxTOP, 6);
    
    intermediateSizer->Add(textSizer, 0, wxTOP | wxLEFT | wxRIGHT, 6);
    intermediateSizer->Add(optionsSizer, 0, wxTOP | wxRIGHT, 6);
    topSizer->Add(probTypeSizer);
    topSizer->Add(lengthSizer);
    topSizer->Add(acSolverSizer);
    topSizer->Add(freqSizer);
    topSizer->Add(depthSizer);
    topSizer->Add(solverPreSizer);
    topSizer->Add(minAngleSizer);
    topSizer->Add(commentSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 6);
    
    topSizer->Add(footerSizer, 0, wxALIGN_RIGHT);
    
    SetSizerAndFit(topSizer);
}



preferencesDialog::preferencesDialog(electroStaticPref &pref) : wxDialog(NULL, wxID_ANY, "Problem Definition")
{
    
}





preferencesDialog::~preferencesDialog()
{
    
}