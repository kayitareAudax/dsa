#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>

using namespace std;
class InventorySystem
{
private:
    const string FILE_PATH = "C:\\Users\\abc\\Documents\\codes\\NE\\actuals\\dsa\\inventory.csv";
    static bool compareByID(const string &item1, const string &item2)
    {
        InventorySystem inventory;
        string id1 = inventory.tokenizer(item1, ' ')[2]; // Assuming ID is at index 2
        string id2 = inventory.tokenizer(item2, ' ')[2];
        return stoi(id1) < stoi(id2); // Convert to integer and compare
    }

public:
    vector<string> tokenizer(const string &str, char delim)
    {
        size_t start = 0;
        size_t end = str.find(delim);
        vector<string> tokens;

        while (end != string::npos)
        {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delim, start);
        }

        tokens.push_back(str.substr(start));
        return tokens;
    }

    string convertToLowercase(const string &str)
    {
        string lowercaseStr = str;
        string::size_type i = 0;

        while (i < lowercaseStr.length())
        {
            lowercaseStr[i] = tolower(lowercaseStr[i]);
            i++;
        }

        return lowercaseStr;
    }

    void add(int item_id, const string &item_name, int item_quantity, const string &item_registration_date)
    {
        try
        {
            if(item_name.empty() || item_registration_date.empty()){
                cout<<"Please enter all data"<<endl;
                return;
            }
            if(item_registration_date.size()!=10){
                throw runtime_error("Please enter a valid date of format yyyy-mm-dd");
            }
            ofstream outputFile(FILE_PATH.c_str(), ios::app);
            ifstream inFile(FILE_PATH.c_str());
            if (!outputFile)
            {
                throw runtime_error("Unable to open the file");
            }
            // check if item exists
            string line;
            if (!inFile)
            {
                cout << "Unable to open the file" << endl;
                return;
            }
            string item;
            while (getline(inFile, item))
            {
                vector<string> tokens = tokenizer(item, ' ');
                if (tokens.size() >= 1)
                {
                    int existing_id =stoi(tokens[0]);
                    if (existing_id == item_id)
                    {
                        inFile.close();
                        throw runtime_error("Item with the same ID already exists.");
                    }
                }
            }

            inFile.close();
            outputFile << item_id << " " << item_name << " " << to_string(item_quantity) << " " << item_registration_date << endl;
            cout << "Item added to inventory" << endl;
            outputFile.close();
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void listItems()
    {
        try
        {
            ifstream inFile(FILE_PATH.c_str());
            if (!inFile)
            {
                throw runtime_error("Cannot open the file");
            }
            inFile.seekg(0, ios::end);

            if (inFile.tellg() == 0)
            {
                cout << "No Item found in the inventory" << endl;
                return;
            }

            inFile.seekg(0, ios::beg);
            string line;
            vector<string> result;
            while (getline(inFile, line))
            {
                vector<string> tokens = tokenizer(line, ' ');
                if (tokens.size() >= 4)
                {
                    result.push_back("Item ID: " + tokens[0] + "     Item Name: " + tokens[1] + "    Quantity: " + tokens[2] + "     Reg Date: " + tokens[3]);
                }
            }

            inFile.close();

            // Sort the items by ID
            sort(result.begin(), result.end(), compareByID);

            // Display the sorted items
            for (const string &item : result)
            {
                cout << item << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void help()
    {
        cout << "-------------------------------------------------------------" << endl;
        cout << "*                        Command syntaxes                   *" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "                            itemadd <item_id> <item_name> <quantity> <registration_date>" << endl;
        cout << "                            itemslist" << endl;
    }
};

int main()
{
    InventorySystem inventory;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "RCA      INVENTORY           SYSTEM" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout<<endl;
    inventory.help();
    string option;
    do
    {
        cout << "console>";
        getline(cin, option);
        if (inventory.convertToLowercase(inventory.tokenizer(option, ' ')[0]) == "itemadd")
        {
            try
            {
                vector<string> tokens = inventory.tokenizer(option, ' ');
                if(tokens.size()<5){
                    throw runtime_error("Please provide all required arguments");
                }
                int id = stoi(tokens[1]);
                string item_name = tokens[2];
                int item_quantity = stoi(tokens[3]);
                string item_reg_date = tokens[4];
                inventory.add(id, item_name, item_quantity, item_reg_date);
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (inventory.convertToLowercase(inventory.tokenizer(option, ' ')[0]) == "itemslist")
        {
            inventory.listItems();
        }
        else if (inventory.convertToLowercase(option) == "help")
        {
            inventory.help();
        }
        else if(inventory.convertToLowercase(option) == "exit"){
            exit(1);
        }
        else
        {
            cout << "Please enter a valid command." << endl;
            inventory.help();
        }

    } while (inventory.convertToLowercase(option) != "exit");

    return 0;
}
