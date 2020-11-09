#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

//
// This is a starter file to help frame your ideas around
// Lab #9.  The reading of each line in the file is completed
// for you.
//

const string FILENAME = "ft_chicago.csv";  // Input file (CSV)
const int NFIELDS = 3;                     // Number of fields in
                                           // each line of the input file

// This holds a single food truck entry.  Each line in the file should
// correspond to one of these.
struct foodtruck {
    string business_name;
    string zip;
    string license_type;
    foodtruck *next;
};

// Starting size, adjust as needed -- should be prime.
// This strikes a good balance without having too many empty buckets.
const int HASH_SIZE = 53;

// Hash table for all of the food trucks -- static so it's zeroed
static foodtruck *truckHash[HASH_SIZE];

// Reads a single truck, filling in the
// fields (name, etc.) passed by the caller.
void readSingleTruck(const string &str,
        foodtruck *newFT) {
    istringstream istr(str);
    string fields[NFIELDS];
    string tmp;
    int i = 0;

    while (getline(istr, tmp, ',') && i < NFIELDS) {
        fields[i++] = tmp;
    }

    // Fill in the newly allocated entry.
    // A pointer to it was passed to us.
    newFT->business_name = fields[0];
    newFT->zip = fields[1];
    newFT->license_type = fields[2];
}

// Generate a hash key, given a string (this would come from the
// string the user typed to find).  your hash function goes here.
//
// Don't use this one, it performs poorly!
unsigned int genHashKey(string key) {
    unsigned int sum = 0;

    // use the first letter of the key
    for (int i = 0; i < key.size(); i++){
        sum += (int)key[i];
    }

    // for debugging -- add a statement like this to
    // see the hash key generated for an entry.
    //
    // cout << "name: " << key
    //     << " hash key: " << sum % HASH_SIZE << endl;

    return (sum % HASH_SIZE);
}

// Insert a new truck into the hash table
void truckInsert(foodtruck *newFT) {

    //
    // Implement this function.  
    //
    // (These are example parameters)
    //
    // Accepts a new food truck entry, finds the location in the
    // hash table where it should go, and inserts.
    //
    int hashIndex = genHashKey(newFT->business_name);
    newFT->next = 0;
    struct foodtruck *entp;
    entp = truckHash[hashIndex];
    
    if (!entp) {
        truckHash[hashIndex] = newFT;
        return;
    }

    // OK, it's not empty.
    // Find the last entry.
    while (entp->next) {
        entp = entp->next;
    }
    // ...and make it point to the new one.
    entp->next = newFT;

}

// This function accepts a string name and a reference
// to an empty foodtruck.
//
// Upon return,
// - 'foundFT' will be filled-in with what was found.
// - The function will return 'true' if something was found, 'false' otherwise.
//
bool truckFind(const string &name, foodtruck &foundFT, int &ncmp) {
    int key = genHashKey(name);
    int numComparisons = 0;
    foodtruck *ftSearch;
    ftSearch = truckHash[key];
    while(ftSearch) {
        if (ftSearch->business_name == name) {
            foundFT = *ftSearch;
            numComparisons += 1;
            ncmp = numComparisons;
            return (true);
        }
        
        ftSearch = ftSearch->next;
        numComparisons += 1;
        ncmp = numComparisons;
    }

    // Not found
    return (false);
    
    //
    // Implement this function.  
    //
    // (These are example parameters)
    //
    //
    // Accepts a key, a reference to a found entry, and reference to
    // number of comparisons.  Fill-in the values of the 'foundFT' with
    // the values from the entry found in the hash table.
    //

}

int main() {
    ifstream inFile(FILENAME);
    string inputLine, inputStr;
    foodtruck foundFoodTruck;
    bool foundIt;
    int numComparisons;
    int linesRead = 0;

    // Discard the first header line
    getline(inFile, inputLine);

    // Read in each food truck entry
    while (getline(inFile, inputLine)) {

        // Dynamically allocate a new struct
        foodtruck *ftptr = new foodtruck;

        // Read the next line from the file,
        // filling in the new truck
        // just allocated.
        readSingleTruck(inputLine, ftptr);

        // Hash it and insert into the table where needed.
        truckInsert(ftptr);

        // Keep a counter of read lines.
        linesRead++;

        // (for debugging)
        // Output the number of lines read every so often.
        // if (linesRead % 25 == 0)
        //    cout << "Inserted " << linesRead << " entries"
        //         << endl;
    }

    // Handle errors and/or summarize the read
    if (linesRead == 0) {
        cerr << "Read failed." << endl;
        return (-1);
    } else {
        cout << "Read " << linesRead << " food trucks." << endl;
        cout << fixed << setprecision(2) << endl;
    }

    // (example) Forever loop until the user requests an exit
    for (;;) {
        //
        // Your input loop goes here.
        //
        cout << "\n";
        cout << "Enter a business name (-1 to quit):\n";
        getline(cin, inputStr);
        if (inputStr == "-1") {
            cout << "Goodbye!\n";
            exit(0);
        } else {
            foundIt = truckFind(inputStr, foundFoodTruck, numComparisons);
            if (foundIt) {
                cout << "\n";
                cout << "Comparisons: " << numComparisons << endl;
                cout << "Business Name: " << foundFoodTruck.business_name << endl;
                cout << "ZIP: " << foundFoodTruck.zip << endl;
                cout << "License type: " << foundFoodTruck.license_type << endl;
            } else {
                cout << "Not found in the database.\n";
                cout << "Comparisons: " << numComparisons << endl;
            }
            
        }
        
    }
    return (0);
}
