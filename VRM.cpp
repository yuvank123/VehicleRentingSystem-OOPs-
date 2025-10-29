#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
using namespace std;

class showingDetails {
public:
    virtual void show() const { 
        cout << "Showing Details in the while..." << endl; 
    } 
    virtual ~showingDetails() {} 
};

class Model : public showingDetails {
    string modelname, modelnumber, modelcolour;
    double modelprice;
  
public:
    Model(string nameOfModel = "", double priceOfModel = 0.0, string phonenumberOfModel = "", string colourOfModel = "")
        : modelname(nameOfModel), modelprice(priceOfModel), modelnumber(phonenumberOfModel), modelcolour(colourOfModel) {}

    string getModelName() const { return modelname; }
    friend void displayModel(const Model &m);

    void show() const override {
        displayModel(*this);
    }
};

class Company : public showingDetails {
    string companyname;
    vector<Model *> models;

public:
    Company(string nameOfCompany = "") : companyname(nameOfCompany) {}
    vector<Model *> &getModels() { return models; }
    string getCompanyName() const { return companyname; }

    void addModel(Model *m) { models.push_back(m); }

    void show() const override {
        cout << "\n Company: " << companyname << "\n";
        for (auto m : models)
            m->show();
    }

    ~Company() {
        for (auto m : models)
            delete m;
    }
};

class Variety : public showingDetails {
    string vname;
    vector<Company *> companies;
public:
    Variety(string varietyname = "") : vname(varietyname) {};
    vector<Company *> &getCompanies() { return companies; }
    void addCompany(Company *c) { companies.push_back(c); };
    string getVarietyName() const { return vname; }

    void show() const override {
        cout << "\n  Variety: " << vname << "\n";
        for (auto c : companies)
            c->show();
    }

    ~Variety() {
        for (auto c : companies)
            delete c;
    }
};

class Enterprises : public showingDetails {
    string Enterprisename, ownerName, OwnerPhonenumber;
    vector<Company *> companies;
    vector<Variety *> varieties;

public:
    Enterprises(string nameOfEnterprise = "", string nameOfOwner = "", string phoneOfOwner = "")
        : Enterprisename(nameOfEnterprise), ownerName(nameOfOwner), OwnerPhonenumber(phoneOfOwner) {}

    vector<Variety *> &getVarieties() { return varieties; }
    void addCompany(Company *c) { companies.push_back(c); }
    void addVariety(Variety *v) { varieties.push_back(v); }
    string getEnterpriseName() const { return Enterprisename; }

    void show() const override {
        cout << "\nEnterprise: " << Enterprisename
             << "\nOwner: " << ownerName
             << "\nPhone: " << OwnerPhonenumber << "\n";
        for (auto v : varieties)
            v->show();
    }

    ~Enterprises() {
        for (auto c : companies)
            delete c;
    }
};

void displayModel(const Model &m) {
    cout << "      Model: " << m.modelname
         << " | Price: " << m.modelprice
         << " | Number: " << m.modelnumber
         << " | Color: " << m.modelcolour << "\n";
}

vector<Enterprises *> loadData(const string &filename) {
    ifstream file(filename);
    vector<Enterprises *> enterprises;
    if (!file.is_open()) {
        cerr << "Cannot open file\n";
        return enterprises;
    }

    string line;
    Enterprises *currentE = nullptr;
    Variety *currentV = nullptr;
    Company *currentC = nullptr;

    while (getline(file, line)) {
        if (line.rfind("Enterprise:", 0) == 0) {
            string ename, owner, phone, part;
            stringstream ss(line);
            while (getline(ss, part, '|')) {
                if (part.find("Enterprise:") != string::npos)
                    ename = part.substr(11);
                else if (part.find("Owner:") != string::npos)
                    owner = part.substr(6);
                else if (part.find("Phone:") != string::npos)
                    phone = part.substr(6);
            }
            currentE = new Enterprises(ename, owner, phone);
            enterprises.push_back(currentE);
        } else if (line.rfind("Variety:", 0) == 0) {
            string vname = line.substr(8);
            currentV = new Variety(vname);
            if (currentE)
                currentE->addVariety(currentV);
        } else if (line.rfind("Company:", 0) == 0) {
            string cname = line.substr(8);
            currentC = new Company(cname);
            if (currentV)
                currentV->addCompany(currentC);
        } else if (line.rfind("Model:", 0) == 0) {
            string mname, number, color, part;
            double price = 0;
            stringstream ss(line);
            while (getline(ss, part, '|')) {
                if (part.find("Model:") != string::npos)
                    mname = part.substr(6);
                else if (part.find("Price:") != string::npos)
                    price = stod(part.substr(6));
                else if (part.find("Number:") != string::npos)
                    number = part.substr(7);
                else if (part.find("Color:") != string::npos)
                    color = part.substr(6);
            }
            if (currentC)
                currentC->addModel(new Model(mname, price, number, color));
        } else if (line == "END_ENTERPRISE") {
            currentE = nullptr;
            currentV = nullptr;
            currentC = nullptr;
        }
    }
    return enterprises;
}

class RentalManager {
    string filename;
public:
    RentalManager(string fname = "rentalRecords.txt") : filename(fname) {}
    void storeRentalDetails(const string &username, const string &enterprise,
                            const string &variety, const string &company,
                            const string &model) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open rental records file!\n";
            return;
        }

        time_t now = time(0);
        char *dt = ctime(&now);

        file << "User: " << username
             << " | Enterprise: " << enterprise
             << " | Variety: " << variety
             << " | Company: " << company
             << " | Model: " << model
             << " | Date: " << dt;
        file << "---------------------------------------------\n";
        file.close();

        cout << "\nRental Details Saved Successfully!\n";
    }

    void storeRentalDetails(const string &enterprise,
                            const string &variety, const string &company,
                            const string &model) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open rental records file!\n";
            return;
        }

        time_t now = time(0);
        char *dt = ctime(&now);

        file << "User: Guest"
             << " | Enterprise: " << enterprise
             << " | Variety: " << variety
             << " | Company: " << company
             << " | Model: " << model
             << " | Date: " << dt;
        file << "---------------------------------------------\n";
        file.close();

        cout << "\nRental Details Saved for Guest User!\n";
    }

    void processPayment() {
        int paymentChoice;
        cout << "\nChoose Payment Method:\n";
        cout << "1. UPI\n";
        cout << "2. Debit/Credit Card\n";
        cout << "3. Cash on Delivery\n";
        cout << "Enter your choice: ";
        cin >> paymentChoice;

        switch (paymentChoice) {
        case 1:
            cout << "Enter UPI ID: ";
            {
                string upi;
                cin >> upi;
                cout << "Processing UPI Payment...\nPayment Successful!\n";
            }
            break;
        case 2:
            cout << "Enter Card Number: ";
            {
                string card;
                cin >> card;
                cout << "Processing Card Payment...\nPayment Successful!\n";
            }
            break;
        case 3:
            cout << "You selected Cash on Delivery.\nOrder Confirmed!\n";
            break;
        default:
            cout << "Invalid choice! Defaulting to Cash on Delivery.\nOrder Confirmed!\n";
        }
    }
};

class UserDetails {
private:
    string filename;
    string username;
    string password;
    string email;
    string phonenumber;

public:
    UserDetails(string fname = "userDetails.txt") : filename(fname) {}
    
    void UserEnterpriseDetailEntry() {
        vector<Enterprises *> enterprises = loadData("sellerDetails.txt");
        cout << "\nWELCOME TO THE USER PORTAL\n";
        cout << "====================================\n";
        cout << "Available Enterprises:\n";
        for (size_t i = 0; i < enterprises.size(); i++)
            cout << i + 1 << ". " << enterprises[i]->getEnterpriseName() << "\n";

        int eChoice;
        cout << "\nSelect Enterprise number: ";
        cin >> eChoice;

        if (eChoice <= 0 || eChoice > (int)enterprises.size()) {
            cout << "Invalid enterprise choice!\n";
            for (auto e : enterprises) delete e;
            return;
        }

        Enterprises *selectedEnterprise = enterprises[eChoice - 1];
        selectedEnterprise->show();

        cout << "\nSelect Variety:\n";
        vector<Variety *> &varieties = selectedEnterprise->getVarieties();

        if (varieties.empty()) {
            cout << "No varieties available.\n";
            for (auto e : enterprises) delete e;
            return;
        }

        for (size_t i = 0; i < varieties.size(); i++)
            cout << i + 1 << ". " << varieties[i]->getVarietyName() << "\n";

        int vChoice;
        cout << "\nEnter Variety number: ";
        cin >> vChoice;

        if (vChoice <= 0 || vChoice > (int)varieties.size()) {
            cout << "Invalid variety choice!\n";
            for (auto e : enterprises) delete e;
            return;
        }

        Variety *selectedVariety = varieties[vChoice - 1];

        cout << "\nSelect Company:\n";
        vector<Company *> &companies = selectedVariety->getCompanies();

        if (companies.empty()) {
            cout << "No companies available.\n";
            for (auto e : enterprises) delete e;
            return;
        }

        for (size_t i = 0; i < companies.size(); i++)
            cout << i + 1 << ". Company: " << companies[i]->getCompanyName() << "\n";

        int cChoice;
        cout << "\nEnter Company number: ";
        cin >> cChoice;

        if (cChoice <= 0 || cChoice > (int)companies.size()) {
            cout << "Invalid company choice!\n";
            for (auto e : enterprises) delete e;
            return;
        }

        Company *selectedCompany = companies[cChoice - 1];

        cout << "\nSelect Model:\n";
        vector<Model *> &models = selectedCompany->getModels();

        if (models.empty()) {
            cout << "No models available.\n";
            for (auto e : enterprises) delete e;
            return;
        }

        for (size_t i = 0; i < models.size(); i++) {
            cout << i + 1 << ". ";
            displayModel(*models[i]);
        }

        int mChoice;
        cout << "\nEnter Model number: ";
        cin >> mChoice;

        if (mChoice <= 0 || mChoice > (int)models.size()) {
            cout << "Invalid model choice!\n";
            for (auto e : enterprises) delete e;
            return;
        }

        Model *selectedModel = models[mChoice - 1];
        cout << "\nYou Selected:\n";
        displayModel(*selectedModel);

        RentalManager rentalManager;
        string username = this->username;
        if(username.size() == 0){username = "GuestUser";}
        string enterpriseName = selectedEnterprise->getEnterpriseName();
        string varietyName = selectedVariety->getVarietyName();
        string companyName = selectedCompany->getCompanyName();
        string modelName = selectedModel->getModelName();

        if (username == "GuestUser")
            rentalManager.storeRentalDetails(enterpriseName, varietyName, companyName, modelName);
        else
            rentalManager.storeRentalDetails(username, enterpriseName, varietyName, companyName, modelName);

        rentalManager.processPayment();

        for (auto e : enterprises)
            delete e;
    }

    void registerUser() {
        cout << "Keep Format like yuvank_goyal (firstName_lastName)!!\n";
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        bool phoneCheck = false;
        do {
            cout << "Enter phonenumber: ";
            cin >> phonenumber;

            phoneCheck = (phonenumber.size() == 10);
            if (!phoneCheck) {
                cout << "Phone number must be exactly 10 digits.\n";
                continue;
            }
            for (char ch : phonenumber)
                if (!isdigit(ch)) { phoneCheck = false; cout << "Phone number must contain digits only.\n"; break; }

        } while (!phoneCheck);

        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << username << "," << password << "," << phonenumber << "\n";
            file.close();
            cout << "\nRegistration successful!\n";
            cout << "-------------------------------------\n";
            UserEnterpriseDetailEntry();
        } else {
            cout << "Error opening file for registration.\n";
        }
    }

    bool loginUser() {
        for (int attempts = 1; attempts <= 2; attempts++) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            ifstream file(filename);
            string line, uuser, upass;
            while (getline(file, line)) {
                stringstream ss(line);
                getline(ss, uuser, ',');
                getline(ss, upass, ',');
                if (uuser == username && upass == password) {
                    cout << "\nLogin successful!\n";
                    UserEnterpriseDetailEntry();
                    return true;
                }
            }
            cout << "\nInvalid username or password!\n";
        }
        return false;
    }
};

int main() {
    cout << "Welcome to Vehicle Rental System\n";
    cout << "-------------------------------------\n";
    cout << "Which Portal You Want To Access?\n";
    cout << "1. User Portal\n";
    cout << "2. Service Provider Portal\n";
    cout << "Enter your choice (1 or 2): ";
    int portalChoice;
    cin >> portalChoice;

    if (portalChoice != 1 && portalChoice != 2) {
        cout << "Invalid choice! Exiting...\n";
        return 0;
    }

    try {
        if (portalChoice == 1) {
            cout << "You have accessed the User Portal.\n";
            cout << "==============================\n";
            cout << "1. login\n";
            cout << "2. Register\n";
            cout << "Enter your choice (1 or 2): ";
            int userChoice;
            cin >> userChoice;
            if (userChoice == 1) {
                UserDetails user; user.loginUser();
            } else if (userChoice == 2) {
                UserDetails user; user.registerUser();
            } else {
                cout << "Invalid choice! Exiting...\n";
            }
        }
        if (portalChoice == 2) {
            cout << "You have accessed the Service Provider Portal.\n";
        }
    } catch (const exception &e) {
        cerr << e.what() << '\n';
    }
    return 0;
}
