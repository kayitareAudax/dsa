#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <regex>
using namespace std;

class InventorySystem
{
private:
    const string FILE_PATH = "C:\\Users\\abc\\Documents\\codes\\NE\\actuals\\dsa\\inventory.csv";

    // Comparison function for sorting by name
    static bool compare_by_name(const string &item1, const string &item2)
    {
        InventorySystem inventorySystem;
        vector<string> tokens1 = inventorySystem.tokenizer(item1, ',');
        vector<string> tokens2 = inventorySystem.tokenizer(item2, ' ,');

        if (tokens1.size() >= 4 && tokens2.size() >= 4)
        {
            return inventorySystem.convert_to_lowercase(tokens1[1]) < inventorySystem.convert_to_lowercase(tokens2[1]); // Compare item names
        }

        return false; // Default comparison
    }

public:
    vector<string> tokenizer(const string &str, char delim)
    {
        // Tokenize the input string based on the delimiter
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

    string convert_to_lowercase(const string &str)
    {
        // Convert a string to lowercase
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
            // Add an item to the inventory
            if (item_registration_date.size() != 10)
            {
                throw runtime_error("Please enter a valid date of format yyyy-mm-dd");
            }

            ofstream outputFile(FILE_PATH.c_str(), ios::app);
            ifstream in_file(FILE_PATH.c_str());

            if (!outputFile)
            {
                throw runtime_error("Unable to open the file");
            }

            // Check if item exists
            string line;

            if (!in_file)
            {
                cout << "Unable to open the file" << endl;
                return;
            }

            string item;
            while (getline(in_file, item))
            {
                vector<string> tokens = tokenizer(item, ',');

                if (tokens.size() >= 1)
                {
                    int existing_id = stoi(tokens[0]);
                    if (existing_id == item_id)
                    {
                        in_file.close();
                        throw runtime_error("Item with the same ID already exists.");
                    }
                }
            }

            in_file.close();
            outputFile << item_id << "," << item_name << "," << to_string(item_quantity) << "," << item_registration_date << endl;
            cout << "Item added to inventory" << endl;
            outputFile.close();
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    bool is_valid_date(const string &date)
    {
        // Regular expression pattern for "yyyy-mm-dd" format
        regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");

        // Check if the date matches the pattern
        return regex_match(date, pattern);
    }
    void listItems()
    {
        try
        {
            // List all items in the inventory
            ifstream in_file(FILE_PATH.c_str());

            if (!in_file)
            {
                throw runtime_error("Cannot open the file");
            }

            in_file.seekg(0, ios::end);

            if (in_file.tellg() == 0)
            {
                cout << "No Item found in the inventory" << endl;
                return;
            }

            in_file.seekg(0, ios::beg);
            string line;
            vector<string> result;

            while (getline(in_file, line))
            {
                vector<string> tokens = tokenizer(line, ',');

                if (tokens.size() >= 4)
                {
                    string formattedItem = tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3];
                    result.push_back(formattedItem);
                }
            }

            in_file.close();

            // Sort the items by name
            sort(result.begin(), result.end(), compare_by_name);

            // Display the sorted items
            for (const string &item : result)
            {
                vector<string> tokens = tokenizer(item, ',');
                cout << "Item ID: " << setw(8) << left << tokens[0]
                     << "Item Name: " << setw(15) << left << tokens[1]
                     << "Quantity: " << setw(8) << left << tokens[2]
                     << "Reg Date: " << tokens[3] << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    bool is_number(const string &str)
    {
        for (char c : str)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return true;
    }
    void help()
    {
        // Display available command syntaxes
        cout << "-------------------------------------------------------------" << endl;
        cout << "*                        Command syntaxes                   *" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "                         itemadd <item_id> <item_name> <quantity> <registration_date>" << endl;
        cout << "                         itemslist" << endl;
    }
};

int main()
{
    InventorySystem inventory;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "RCA      INVENTORY           SYSTEM" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl;
    inventory.help();
    string option;

    do
    {
        cout << "console>";
        getline(cin, option);

        if (inventory.convert_to_lowercase(inventory.tokenizer(option, ' ')[0]) == "itemadd")
        {
            try
            {
                vector<string> tokens = inventory.tokenizer(option, ' ');

                if (tokens.size() < 5)
                {
                    throw runtime_error("Please provide all required arguments");
                }
                if (!inventory.is_number(tokens[1]))
                {
                    throw runtime_error("Item_id should be a number");
                }
                if (!inventory.is_number(tokens[3]))
                {
                    throw runtime_error("Item_quantity should a number");
                }
                if (!inventory.is_valid_date(tokens[4]))
                {
                    throw runtime_error("please enter date in format yyyy-mm-dd");
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
        else if (inventory.convert_to_lowercase(inventory.tokenizer(option, ' ')[0]) == "itemslist")
        {
            inventory.listItems();
        }
        else if (inventory.convert_to_lowercase(option) == "help")
        {
            inventory.help();
        }
        else if (inventory.convert_to_lowercase(option) == "exit")
        {
            exit(1);
        }
        else
        {
            cout << "Please enter a valid command as from this list below!!." << endl;
            inventory.help();
        }

    } while (inventory.convert_to_lowercase(option) != "exit");

    return 0;
}
