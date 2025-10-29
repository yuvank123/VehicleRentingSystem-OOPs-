class EnterpriseSeller
{
private:
    string enterpriseName;
    string ownerName;
    string phone;
    string enterpriseID;

    struct Model
    {
        string name;
        double price;
        string number;
        string color;
    };

    struct Company
    {
        string name;
        vector<Model> models;
    };

    struct Variety
    {
        string name;
        vector<Company> companies;
    };

    vector<Variety> varieties;

    string generateEnterpriseID()
    {
        srand(time(0));
        int num = rand() % 90000 + 10000;
        return "ENT" + to_string(num);
    }

public:
    
    friend istream &operator>>(istream &in, EnterpriseSeller &s)
    {
        cout << "\n=== Set Up New Enterprise ===\n";
        cout << "Enter enterprise name: ";
        in.ignore();
        getline(in, s.enterpriseName);
        cout << "Enter owner name: ";
        getline(in, s.ownerName);
        cout << "Enter phone number: ";
        getline(in, s.phone);

        s.enterpriseID = s.generateEnterpriseID();

        bool addMoreVarieties = true;
        while (addMoreVarieties)
        {
            Variety v;
            cout << "\nEnter vehicle variety (e.g., Car, Bike, Truck): ";
            getline(in, v.name);

            bool addMoreCompanies = true;
            while (addMoreCompanies)
            {
                Company c;
                cout << "Enter company name: ";
                getline(in, c.name);

                bool addMoreModels = true;
                while (addMoreModels)
                {
                    Model m;
                    cout << "Enter model name: ";
                    getline(in, m.name);
                    cout << "Enter model color: ";
                    getline(in, m.color);
                    cout << "Enter model number: ";
                    getline(in, m.number);
                    cout << "Enter model price: ";
                    in >> m.price;
                    in.ignore();
                    c.models.push_back(m);

                    char choice;
                    cout << "Add another model for this company? (y/n): ";
                    in >> choice;
                    in.ignore();
                    if (choice == 'n' || choice == 'N')
                        addMoreModels = false;
                }

                v.companies.push_back(c);

                char choice;
                cout << "Add another company for this variety? (y/n): ";
                in >> choice;
                in.ignore();
                if (choice == 'n' || choice == 'N')
                    addMoreCompanies = false;
            }

            s.varieties.push_back(v);

            char choice;
            cout << "Add another variety? (y/n): ";
            in >> choice;
            in.ignore();
            if (choice == 'n' || choice == 'N')
                addMoreVarieties = false;
        }

        cout << "\n✅ Enterprise successfully created!\n";
        cout << "Your enterprise name: " << s.enterpriseName << "\n";
        cout << "Your unique Enterprise ID (for login): " << s.enterpriseID << "\n";
        cout << "-----------------------------------------\n";
        return in;
    }

    // Operator overloading for output (display)
    friend ostream &operator<<(ostream &out, const EnterpriseSeller &s)
    {
        out << "\nEnterprise: " << s.enterpriseName
            << "\nOwner: " << s.ownerName
            << "\nPhone: " << s.phone
            << "\nEnterprise ID: " << s.enterpriseID << "\n";

        for (const auto &v : s.varieties)
        {
            out << "  Variety: " << v.name << "\n";
            for (const auto &c : v.companies)
            {
                out << "    Company: " << c.name << "\n";
                for (const auto &m : c.models)
                {
                    out << "      Model: " << m.name
                        << " | Price: " << m.price
                        << " | Number: " << m.number
                        << " | Color: " << m.color << "\n";
                }
            }
        }
        return out;
    }

    // Save enterprise details to file
    void saveToFile(string filename)
    {
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file for saving seller details!\n";
            return;
        }

        file << "Enterprise:" << enterpriseName
             << "|Owner:" << ownerName
             << "|Phone:" << phone
             << "|ID:" << enterpriseID << "\n";

        for (auto &v : varieties)
        {
            file << "Variety:" << v.name << "\n";
            for (auto &c : v.companies)
            {
                file << "Company:" << c.name << "\n";
                for (auto &m : c.models)
                {
                    file << "Model:" << m.name
                         << "|Price:" << m.price
                         << "|Number:" << m.number
                         << "|Color:" << m.color << "\n";
                }
            }
        }

        file << "END_ENTERPRISE\n";
        file.close();
    }

    string getEnterpriseID() const { return enterpriseID; }
    string getEnterpriseName() const { return enterpriseName; }
};
