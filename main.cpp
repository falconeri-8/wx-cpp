#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>

class Appointment;
class DataManager;

using namespace std;

class Person
{
    protected:
    string name;
    string id;
    string password;
    public:
    Person() : name(""), id(""), password("") {}
    Person(string name, string id, string password)
    {
        this->name = name;
        this->id = id;
        this->password = password;
    }
    virtual bool authenticate(string name, string password)
    {
        if (name == this->name && password == this->password)
        {
            return true;

        } else {
            return false;
        }
    }
    virtual string getID() const
    {
        return id;
    }
    virtual string getName() const
    {
        return name;
    }
    virtual string getPassword() const
    {
        return password;
    }

    virtual string serialize() const = 0;
    virtual ~Person() {}
};

class Patient : virtual public Person
{
    private:
    string disease;
    string prescription;
    vector<int> appointmentIDs; // Store appointment IDs instead of pointers
    static int patientcount;

    public:
    Patient(string name, string password)
    {
        this->name = name;
        this->password = password;
        this->id = "P" + to_string(patientcount++);
        this->disease = "None";
        this->prescription = "None";
    }

    Patient(string name, string id, string disease, string prescription, string password)
    {
        this->name = name;
        this->password = password;
        this->id = id;
        this->disease = disease;
        this->prescription = prescription;
        patientcount++;
    }

    void setDisease(string disease)
    {
        this->disease = disease;
    }
    void setPrescription(string prescription)
    {
        this->prescription = prescription;
    }
    string getDisease() const
    {
        return disease;
    }
    string getPrescription() const
    {
        return prescription;
    }
    string serialize() const override
    {
        return name + "," + id + "," + disease + "," + prescription + "," + password;
    }
    void addAppointmentID(int appointmentID) {
        appointmentIDs.push_back(appointmentID);
    }
    vector<int>& getAppointmentIDs() {
        return appointmentIDs;
    }
    void displayProfile() const
    {
        cout << "\n[Profile]\n";
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Disease: " << disease << endl;
        cout << "Prescription: " << prescription << "\n" << endl;
    }
    ~Patient() {}
};
int Patient::patientcount = 0;

class Doctor : virtual public Person
{
    private:
    string specialty;
    static int doctorcount;

    public:
    Doctor(string name, string specialty, string password)
    {
        this->name = name;
        this->specialty = specialty;
        this->id = "D" + to_string(doctorcount++);
        this->password = password;
    }
    Doctor(string name, string id, string specialty, string password)
    {
        this->name = name;
        this->password = password;
        this->id = id;
        this->specialty = specialty;
        doctorcount++;
    }
    string serialize() const override
    {
        return name + "," + id + "," + specialty + "," + password;
    }
    string getSpecialty() const
    {
        return specialty;
    }
    void displayProfile() const
    {
        cout << "\n[Profile]\n";
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Specialty: " << specialty << "\n" << endl;
    }
    ~Doctor() {}
};

int Doctor::doctorcount = 0;

class Appointment
{
    private:
    int appointmentID;
    string date;
    string doctorID;
    string patientID;
    static int appointmentCount;

    public:
    Appointment() : appointmentID(++appointmentCount), date(""), doctorID(""), patientID("") {}

    Appointment(string date, string doctorID, string patientID)
        : appointmentID(++appointmentCount), date(date), doctorID(doctorID), patientID(patientID) {}

    // Constructor for loading from file
    Appointment(int id, string date, string doctorID, string patientID)
        : appointmentID(id), date(date), doctorID(doctorID), patientID(patientID)
    {
        if (id >= appointmentCount) {
            appointmentCount = id;
        }
    }

    void setDate(string date) { this->date = date; }
    void setDoctorID(string doctorID) { this->doctorID = doctorID; }
    void setPatientID(string patientID) { this->patientID = patientID; }

    int getAppointmentID() const { return appointmentID; }
    string getDate() const { return date; }
    string getDoctorID() const { return doctorID; }
    string getPatientID() const { return patientID; }

    string serialize() const {
        return to_string(appointmentID) + "," + date + "," + doctorID + "," + patientID;
    }

    void displayAppointment(const vector<Doctor*>& doctors, const vector<Patient*>& patients) const
    {
        cout << "Date: " << date << " | ID: " << appointmentID;

        // Find doctor
        for (const auto& doctor : doctors) {
            if (doctor->getID() == doctorID) {
                cout << " | Doctor: " << doctor->getName() << " (" << doctor->getSpecialty() << ")";
                break;
            }
        }

        // Find patient
        for (const auto& patient : patients) {
            if (patient->getID() == patientID) {
                cout << " | Patient: " << patient->getName();
                break;
            }
        }
        cout << endl;
    }

    ~Appointment() {}
};

int Appointment::appointmentCount = 0;

class Management : virtual public Person
{
public:
    Management()
    {
        this->name = "admin";
        this->password = "admin";
        this->id = "ADMIN001";
    }

    bool authenticate(string name, string password) override
    {
        if (name == "admin" && password == "admin")
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    string serialize() const override
    {
        return name + "," + id + "," + password;
    }
};

class DataManager
{
    const string docFile = "docFile.txt";
    const string patFile = "patFile.txt";
    const string aptFile = "aptFile.txt";
    vector<Patient*> patients;
    vector<Doctor*> doctors;
    vector<Appointment*> appointments;

public:
    void load()
    {
        // Load doctors
        ifstream File1(docFile);
        if (!File1.is_open())
        {
            cout << "Doctor file not found, creating new file\n";
        }
        else
        {
            string line;
            while (getline(File1, line))
            {
                stringstream ss(line);
                string item;
                vector<string> tokens;

                while (getline(ss, item, ',')) {
                    tokens.push_back(item);
                }
                if (tokens.size() >= 4)
                {
                    Doctor* doctor = new Doctor(tokens[0], tokens[1], tokens[2], tokens[3]);
                    doctors.push_back(doctor);
                }
            }
            File1.close();
        }

        // Load patients
        ifstream File2(patFile);
        if (!File2.is_open())
        {
            cout << "Patient file not found, creating new file\n";
        }
        else
        {
            string line;
            while (getline(File2, line))
            {
                stringstream ss(line);
                string item;
                vector<string> tokens;
                while (getline(ss, item, ',')) {
                    tokens.push_back(item);
                }
                if (tokens.size() >= 5)
                {
                    Patient* patient = new Patient(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
                    patients.push_back(patient);
                }
            }
            File2.close();
        }

        // Load appointments
        ifstream File3(aptFile);
        if (!File3.is_open())
        {
            cout << "Appointment file not found, creating new file\n";
        }
        else
        {
            string line;
            while (getline(File3, line))
            {
                stringstream ss(line);
                string item;
                vector<string> tokens;
                while (getline(ss, item, ',')) {
                    tokens.push_back(item);
                }
                if (tokens.size() >= 4)
                {
                    int id = stoi(tokens[0]);
                    Appointment* appointment = new Appointment(id, tokens[1], tokens[2], tokens[3]);
                    appointments.push_back(appointment);

                    // Link appointment to patient
                    for (auto& patient : patients) {
                        if (patient->getID() == tokens[3]) {
                            patient->addAppointmentID(id);
                            break;
                        }
                    }
                }
            }
            File3.close();
        }

        cout << "Data retrieved\n";
    }

    void save()
    {
        // Save doctors
        ofstream File1(docFile);
        for (const auto& doctor : doctors)
        {
            File1 << doctor->serialize() << endl;
        }
        File1.close();

        // Save patients
        ofstream File2(patFile);
        for (const auto& patient : patients)
        {
            File2 << patient->serialize() << endl;
        }
        File2.close();

        // Save appointments
        ofstream File3(aptFile);
        for (const auto& appointment : appointments)
        {
            File3 << appointment->serialize() << endl;
        }
        File3.close();

        cout << "Data saved\n";
    }

    Patient* getPatient(string name, string password)
    {
        for (auto& patient : patients)
        {
            if (patient->authenticate(name, password))
            {
                return patient;
            }
        }
        return nullptr;
    }

    Doctor* getDoctor(string name, string password)
    {
        for (auto& doctor : doctors)
        {
            if (doctor->authenticate(name, password))
            {
                return doctor;
            }
        }
        return nullptr;
    }

    Doctor* getDoctorByID(string id)
    {
        for (auto& doctor : doctors)
        {
            if (doctor->getID() == id)
            {
                return doctor;
            }
        }
        return nullptr;
    }

    vector<Doctor*>& getDoctors() {
        return doctors;
    }
    vector<Patient*>& getPatients() {
        return patients;
    }
    vector<Appointment*>& getAppointments() {
        return appointments;
    }

    void addDoctor(string name, string specialty, string password)
    {
        Doctor* doctor = new Doctor(name, specialty, password);
        doctors.push_back(doctor);
        cout << "Doctor added successfully! ID: " << doctor->getID() << ", Password: " << doctor->getPassword() << endl;
    }

    void bookAppointment(string date, Doctor* doctor, Patient* patient)
    {
        Appointment* newAppointment = new Appointment(date, doctor->getID(), patient->getID());
        appointments.push_back(newAppointment);
        patient->addAppointmentID(newAppointment->getAppointmentID());
        cout << "Appointment booked successfully! Appointment ID: " << newAppointment->getAppointmentID() << endl;
    }

    vector<Appointment*> getPatientAppointments(Patient* patient)
    {
        vector<Appointment*> patientAppointments;
        for (int id : patient->getAppointmentIDs()) {
            for (auto& appointment : appointments) {
                if (appointment->getAppointmentID() == id) {
                    patientAppointments.push_back(appointment);
                    break;
                }
            }
        }
        return patientAppointments;
    }

    vector<Appointment*> getDoctorAppointments(Doctor* doctor)
    {
        vector<Appointment*> doctorAppointments;
        for (auto& appointment : appointments) {
            if (appointment->getDoctorID() == doctor->getID()) {
                doctorAppointments.push_back(appointment);
            }
        }
        return doctorAppointments;
    }

    void displayAllPatients()
    {
        cout << "\n[Patients]\n";
        for (size_t i = 0; i < patients.size(); i++)
        {
            cout << "[" << i + 1 << "] " << patients[i]->getName() << " (ID: " << patients[i]->getID() << ")" << endl;
        }
        cout << "\n";
    }

    void displayAllDoctors()
    {
        cout << "\n[Doctors]\n";
        for (size_t i = 0; i < doctors.size(); i++)
        {
            cout << "[" << i + 1 << "] " << doctors[i]->getName() << " - " << doctors[i]->getSpecialty() << " (ID: " << doctors[i]->getID() << ")" << endl;
        }
        cout << "\n";
    }

    ~DataManager()
    {
        // Clean up memory
        for (auto& patient : patients)
        {
            delete patient;
        }
        for (auto& doctor : doctors)
        {
            delete doctor;
        }
        for (auto& appointment : appointments)
        {
            delete appointment;
        }
    }
};

class Interface
{
    public:
    DataManager dataManager;

    void patientMenu(Patient* patient)
    {
        int choice;
        do {
            cout << "\n[Patient Interface]\n";
            cout << "[1] View Profile\n[2] View Prescriptions\n[3] View Appointments\n[4] Make Appointment\n[5] Log Out\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    patient->displayProfile();
                    break;
                case 2:
                    cout << "\n[Prescriptions]\n";
                    cout << "Prescription: " << patient->getPrescription() << "\n\n";
                    break;
                case 3:
                    {
                        cout << "\n[Appointments]\n";
                        vector<Appointment*> appointments = dataManager.getPatientAppointments(patient);
                        if (appointments.empty())
                        {
                            cout << "No appointments found.\n";
                        }
                        else
                        {
                            for (size_t i = 0; i < appointments.size(); i++)
                            {
                                cout << "[" << i + 1 << "] ";
                                appointments[i]->displayAppointment(dataManager.getDoctors(), dataManager.getPatients());
                            }
                        }
                        cout << "\n";
                    }
                    break;
                case 4:
                    if (dataManager.getDoctors().empty())
                    {
                        cout << "No doctors available.\n";
                    }
                    else
                    {
                        cout << "\n[Select Specialist]\n";
                        for (size_t i = 0; i < dataManager.getDoctors().size(); i++)
                        {
                            Doctor* doctor = dataManager.getDoctors()[i];
                            cout << "[" << i + 1 << "] " << doctor->getName() << " (" << doctor->getSpecialty() << ")" << endl;
                        }
                        cout << "Enter your choice: ";
                        cin >> choice;
                        if (choice > 0 && choice <= static_cast<int>(dataManager.getDoctors().size()))
                        {
                            Doctor* selectedDoctor = dataManager.getDoctors()[choice - 1];
                            string date;
                            cout << "Enter appointment date (DD/MM/YYYY): ";
                            cin.ignore();
                            getline(cin, date);
                            dataManager.bookAppointment(date, selectedDoctor, patient);
                        }
                        else
                        {
                            cout << "Invalid choice!\n";
                        }
                    }
                    break;
                case 5:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice\n";
                    break;
            }
        } while (choice != 5);
    }

    void doctorMenu(Doctor* doctor)
    {
        int choice;
        do {
            cout << "\n[Doctor Interface]\n";
            cout << "[1] View Profile\n[2] View Appointments\n[3] Manage Patients\n[4] Log Out\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    doctor->displayProfile();
                    break;
                case 2:
                    {
                        cout << "\n[Appointments]\n";
                        vector<Appointment*> appointments = dataManager.getDoctorAppointments(doctor);
                        if (appointments.empty())
                        {
                            cout << "No appointments found.\n";
                        }
                        else
                        {
                            for (size_t i = 0; i < appointments.size(); i++)
                            {
                                cout << "[" << i + 1 << "] ";
                                appointments[i]->displayAppointment(dataManager.getDoctors(), dataManager.getPatients());
                            }
                        }
                        cout << "\n";
                    }
                    break;
                case 3:
                    cout << "\n[Manage Patients]\n";
                    dataManager.displayAllPatients();
                    if (!dataManager.getPatients().empty())
                    {
                        cout << "Select patient to manage: ";
                        int patientChoice;
                        cin >> patientChoice;
                        if (patientChoice > 0 && patientChoice <= static_cast<int>(dataManager.getPatients().size()))
                        {
                            Patient* selectedPatient = dataManager.getPatients()[patientChoice - 1];
                            cout << "\n[1] Update Disease\n[2] Update Prescription\n";
                            cout << "\nEnter your choice: ";
                            int updateChoice;
                            cin >> updateChoice;
                            if (updateChoice == 1)
                            {
                                string disease;
                                cout << "Enter new disease: ";
                                cin.ignore();
                                getline(cin, disease);
                                selectedPatient->setDisease(disease);
                                cout << "Disease updated successfully!\n";
                            }
                            else if (updateChoice == 2)
                            {
                                string prescription;
                                cout << "Enter new prescription: ";
                                cin.ignore();
                                getline(cin, prescription);
                                selectedPatient->setPrescription(prescription);
                                cout << "Prescription updated successfully!\n";
                            }
                            else
                            {
                                cout << "Invalid choice!\n";
                            }
                        }
                        else
                        {
                            cout << "Invalid choice!\n";
                        }
                    }
                    break;
                case 4:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice\n";
                    break;
            }
        } while (choice != 4);
    }

    void adminMenu()
    {
        string name, password;
        cout << "Enter admin name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter admin password: ";
        getline(cin, password);

        Management admin;
        if (admin.authenticate(name, password))
        {
            int choice;
            do {
                cout << "\n[Admin Interface]\n";
                cout << "[1] Add Doctor\n[2] View All Patients\n[3] View All Doctors\n[4] View All Appointments\n[5] Log Out\n";
                cout << "\nEnter your choice: ";
                cin >> choice;

                switch (choice)
                {
                    case 1:
                        {
                            string docName, specialty, password;
                            cout << "Enter doctor name: ";
                            cin.ignore();
                            getline(cin, docName);
                            cout << "Enter password: ";
                            //cin.ignore();
                            getline(cin, password);
                            cout << "Enter doctor specialty: ";
                            getline(cin, specialty);
                            dataManager.addDoctor(docName, specialty, password);
                        }
                        break;
                    case 2:
                        dataManager.displayAllPatients();
                        break;
                    case 3:
                        dataManager.displayAllDoctors();
                        break;
                    case 4:
                        {
                            cout << "\n[Appointments]\n";
                            vector<Appointment*>& appointments = dataManager.getAppointments();
                            if (appointments.empty())
                            {
                                cout << "No appointments found.\n";
                            }
                            else
                            {
                                for (size_t i = 0; i < appointments.size(); i++)
                                {
                                    cout << "[" << i + 1 << "] ";
                                    appointments[i]->displayAppointment(dataManager.getDoctors(), dataManager.getPatients());
                                }
                            }
                            cout << "\n";
                        }
                        break;
                    case 5:
                        cout << "Logging out...\n";
                        break;
                    default:
                        cout << "Invalid choice\n";
                        break;
                }
            } while (choice != 5);
        }
        else
        {
            cout << "Invalid admin credentials!\n";
        }
    }

    Interface()
    {
        dataManager.load();
        int choice;
        do {
            cout << "\n[Hospital Interface]\n";
            cout << "[1] Patient Interface\n[2] Doctor Login\n[3] Administrator\n[0] Exit\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 0:
                    cout << "Exiting system...\n";
                    break;
                case 1:
                    {
                        cout << "\n[Patient Interface]";
                        cout << "\n[1] Login\n[2] Register\n";
                        cout << "\nEnter your choice: ";
                        int subChoice;
                        cin >> subChoice;
                        switch (subChoice)
                        {
                            case 1:
                            {
                                string name, password;
                                cout << "Enter your name: ";
                                cin.ignore();
                                getline(cin, name);
                                cout << "Enter your password: ";
                                getline(cin, password);
                                Patient* patient = dataManager.getPatient(name, password);
                                if (patient)
                                {
                                    cout << "\nWelcome " << patient->getName() << "!\n";
                                    patientMenu(patient);
                                } else {
                                    cout << "Invalid credentials!\n";
                                }
                                break;
                            }
                            case 2:
                            {
                                string name, password;
                                cout << "Enter your name: ";
                                cin.ignore();
                                getline(cin, name);
                                cout << "Enter your password: ";
                                getline(cin, password);
                                Patient* patient = new Patient(name, password);
                                dataManager.getPatients().push_back(patient);
                                cout << "Registration successful! Your ID is: " << patient->getID() << endl;
                                break;
                            }
                            default:
                                cout << "Invalid choice!\n";
                                break;
                        }
                        break;
                    }
                case 2:
                    {
                        string name, password;
                        cout << "Enter your name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter your password: ";
                        getline(cin, password);
                        Doctor* doctor = dataManager.getDoctor(name, password);
                        if (doctor)
                        {
                            cout << "\nWelcome Dr. " << doctor->getName() << "!\n";
                            doctorMenu(doctor);
                        } else {
                            cout << "\nInvalid credentials!\n";
                        }
                        break;
                    }
                case 3:
                    adminMenu();
                    break;
                default:
                    cout << "Invalid choice!\n";
                    break;
            }
        } while (choice != 0);

        dataManager.save();
    }
};

int main()
{
    Interface* interface = new Interface();
    delete interface;
    return 0;
}