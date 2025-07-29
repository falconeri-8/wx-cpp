#include "Modules/DataManager.h"
#include <iostream>
#include <wx/wx.h>

using namespace std;

class App : public wxApp
{
public:
    bool OnInit() override;
};

class Frame : public wxFrame
{
    private:
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;
    wxTextCtrl* nameCtrl;
    wxTextCtrl* contactCtrl;
    wxTextCtrl* descriptionCtrl;
    wxPanel* loginPanel;
    wxPanel* profilePanel;
    void OnExit(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
    void OnCreateProfile(wxCommandEvent& event);
    void ShowProfilePanel();

public:
    Frame() : wxFrame(nullptr, wxID_ANY, "Application", wxDefaultPosition, wxSize(500, 500))
    {
        auto* mainSizer = new wxBoxSizer(wxVERTICAL);

        loginPanel = new wxPanel(this);
        profilePanel = new wxPanel(this);
        profilePanel->Hide();

        auto* loginSizer = new wxBoxSizer(wxVERTICAL);
        usernameCtrl = new wxTextCtrl(loginPanel, wxID_ANY, wxEmptyString,
                                     wxDefaultPosition, wxSize(-1, 30));
        passwordCtrl = new wxTextCtrl(loginPanel, wxID_ANY, wxEmptyString,
                                     wxDefaultPosition, wxSize(-1, 30), wxTE_PASSWORD);
        auto* loginButton = new wxButton(loginPanel, wxID_ANY, "Login");

        loginSizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Username:"),
                       0, wxALL | wxEXPAND, 5);
        loginSizer->Add(usernameCtrl, 0, wxALL | wxEXPAND, 5);
        loginSizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Password:"),
                       0, wxALL | wxEXPAND, 5);
        loginSizer->Add(passwordCtrl, 0, wxALL | wxEXPAND, 5);
        loginSizer->Add(loginButton, 0, wxALL | wxEXPAND, 5);

        auto* profileSizer = new wxBoxSizer(wxVERTICAL);
        nameCtrl = new wxTextCtrl(profilePanel, wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxSize(-1, 30));
        contactCtrl = new wxTextCtrl(profilePanel, wxID_ANY, wxEmptyString,
                                    wxDefaultPosition, wxSize(-1, 30));
        descriptionCtrl = new wxTextCtrl(profilePanel, wxID_ANY, wxEmptyString,
                                        wxDefaultPosition, wxSize(-1, 100),
                                        wxTE_MULTILINE);
        auto* createButton = new wxButton(profilePanel, wxID_ANY, "Create Profile");

        profileSizer->Add(new wxStaticText(profilePanel, wxID_ANY, "Name:"),
                         0, wxALL | wxEXPAND, 5);
        profileSizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 5);
        profileSizer->Add(new wxStaticText(profilePanel, wxID_ANY, "Contact:"),
                         0, wxALL | wxEXPAND, 5);
        profileSizer->Add(contactCtrl, 0, wxALL | wxEXPAND, 5);
        profileSizer->Add(new wxStaticText(profilePanel, wxID_ANY, "Description:"),
                         0, wxALL | wxEXPAND, 5);
        profileSizer->Add(descriptionCtrl, 1, wxALL | wxEXPAND, 5);
        profileSizer->Add(createButton, 0, wxALL | wxEXPAND, 5);

        loginPanel->SetSizer(loginSizer);
        profilePanel->SetSizer(profileSizer);

        mainSizer->Add(loginPanel, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(profilePanel, 1, wxEXPAND | wxALL, 10);

        SetSizer(mainSizer);

        loginButton->Bind(wxEVT_BUTTON, &Frame::OnLogin, this);
        createButton->Bind(wxEVT_BUTTON, &Frame::OnCreateProfile, this);

        SetMinSize(wxSize(500, 400));

        Fit();
        Centre();
    }
};


bool App::OnInit()
{
    Frame* frame = new Frame();
    frame->Show();
    return true;
}



void Frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void Frame::OnLogin(wxCommandEvent& event)
{
    if (usernameCtrl->GetValue() == "admin" && passwordCtrl->GetValue() == "password")
    {
        ShowProfilePanel();
    }
    else
    {
        wxMessageBox("Invalid credentials!", "Error", wxOK | wxICON_ERROR);
    }
}

void Frame::OnCreateProfile(wxCommandEvent& event)
{
    string name = nameCtrl->GetValue().ToStdString();
    string contact = contactCtrl->GetValue().ToStdString();
    string description = descriptionCtrl->GetValue().ToStdString();

    Write("profile", name, contact, description);
    wxMessageBox("Profile created successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void Frame::ShowProfilePanel()
{
    loginPanel->Hide();
    profilePanel->Show();
    Layout();
}

wxIMPLEMENT_APP(App);


/*class Class
{
    string variable;
public:
    Class() : variable("") {}
    Class(string variable) : variable(variable) {}
    string& getVariable()
    {
        return variable;
    }
    void setVariable(string variable)
    {
        this->variable = variable;
    }
};

int main()
{

    vector <Class> Objects;
    Class Object;
    Object.setVariable("Hello World");
    Write("data.txt", &Object.getVariable());
    Read("data.txt", &Object.getVariable());

     vector <string> Rows;
     Fetch("data.txt", Rows);
     for (auto& row : Rows)
     {
         Class Object(row);
         Objects.push_back(Object);
         cout << Object.getVariable() << endl;
     }

    return 0;
}*/