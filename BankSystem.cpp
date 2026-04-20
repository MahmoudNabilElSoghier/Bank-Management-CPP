#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

string LocationClientDataFile = "clientData.txt";
string LocationUserDataFile = "userData.txt";

void showMainManueScreen();
void showTransactionScreen();
void showManageUsersScreen();
void backToHomeScreen();

typedef struct stClient {
    string accountNumber;
    string pinCode;
    string accountName;
    string phone;
    double AccountBalance;
    bool markForDelete = false;
}stClient;


typedef struct stUser {
    string userName;
    string password;
    short accessLevel;
    bool markForDelete = false;
}stUser;

enum enMainManueOptions
{
    eShowClientsList = 1,
    eAddNewClient    = 2,
    eDeleteClient    = 3,
    eUpdateClient    = 4,
    eFindClient      = 5,
    eTransaction     = 6,
    eManageUsers     = 7,
    eLogout          = 8
};
enum enMainManueAccessLevelOptions
{
    eAllAccessLevel = -1,
    eShowClientsListAccessLevel = 1,
    eAddNewClientAccessLevel = 2,
    eDeleteClientAccessLevel = 4,
    eUpdateClientAccessLevel = 8,
    eFindClientAccessLevel = 16,
    eTransactionAccessLevel = 32,
    eManageUsersAccessLevel = 64,
};
enum enTransactionManueOptions
{
    eDeposit         = 1,
    eWithdraw        = 2,
    eTransfer        = 3,
    eTotalBalance    = 4,
    eBackToMainManue = 5
};

enum enManageUsersManueOptions
{
    eShowUserList    = 1,
    eAddNewUser      = 2,
    eDeleteUser      = 3,
    eUpdateUser      = 4,
    eFindUser        = 5,
    eMainManue       = 6
};


stUser currentUser;
short fullAccessLevel = -1;
bool findClientWithAccountNumber(stClient& client, vector<stClient>& vClient, string AccountNumber);
char readChar(string message) {
    cout << message;
    char input;
    cin >> input;
    while (cin.fail()) {
        cout << "Invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << message;
        cin >> input;
    }
    return input;
}

double readDouble(string message) {
    cout << message;
    double input;
    cin >> input;
    while (cin.fail()) {
        cout << "Invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << message;
        cin >> input;
    }
    return input;
}
short readShort(string message) {
    cout << message;
    short input;
    cin >> input;
    while (cin.fail()) {
        cout << "Invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << message;
        cin >> input;
    }
    return input;
}
vector<string> stringSplit(string dataLine, string delimeter = "#//#") {
    vector<string> vString;
    short position = 0;
    string sWord;
    while ((position = dataLine.find(delimeter)) != string::npos) {
        sWord = dataLine.substr(0, position);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        dataLine.erase(0, position + delimeter.length());
    }
    if (dataLine != "") {
        vString.push_back(dataLine);
    }
    return vString;
}

stClient convertDataLineToRecord(string dataLine, string delimeter = "#//#") {
    stClient client;
    vector<string> vClientDataLine = stringSplit(dataLine);
    client.accountNumber = vClientDataLine[0];
    client.pinCode = vClientDataLine[1];
    client.accountName = vClientDataLine[2];
    client.phone = vClientDataLine[3];
    client.AccountBalance = stod(vClientDataLine[4]);
    return client;
}

stUser convertDataLineToRecordUser(string dataLine, string delimeter = "#//#") {
    stUser user;
    vector<string> vUserDataLine = stringSplit(dataLine);
    user.userName = vUserDataLine[0];
    user.password = vUserDataLine[1];
    user.accessLevel = stoi(vUserDataLine[2]);
    return user;
}

string convertRecordToDataLine(stClient client, string delimeter = "#//#") {
    string dataLine = "";
    dataLine += client.accountNumber + delimeter;
    dataLine += client.pinCode + delimeter;
    dataLine += client.accountName + delimeter;
    dataLine += client.phone + delimeter;
    dataLine += to_string(client.AccountBalance);
    return dataLine;
}

string convertRecordToDataLine(stUser user, string delimeter = "#//#") {
    string dataLine = "";
    dataLine += user.userName + delimeter;
    dataLine += user.password + delimeter;
    dataLine += to_string(user.accessLevel);
    return dataLine;
}

vector<stClient> LoadDataToFile(string FileLocation) {
    vector<stClient> vClient;
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::in);
    if (fs.is_open()) {
        while (getline(fs, dataLine)) {
            vClient.push_back(convertDataLineToRecord(dataLine));
        }
        fs.close();
    }
    return vClient;
}

vector<stUser> LoadDataToFileUser(string FileLocation) {
    vector<stUser> vUser;
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::in);
    if (fs.is_open()) {
        while (getline(fs, dataLine)) {
            vUser.push_back(convertDataLineToRecordUser(dataLine));
        }
        fs.close();
    }
    return vUser;
}

bool addDataToFile(stClient client, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out | ios::app);
    if (fs.is_open()) {
        fs << convertRecordToDataLine(client) << endl;
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool addDataToFile(vector<stClient>& vClient, stClient client, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out | ios::app);
    if (fs.is_open()) {
        fs << convertRecordToDataLine(client) << endl;
        vClient.push_back(client);
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool saveDataToFile(vector<stClient> &vClient, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out);
    if (fs.is_open()) {
        for (stClient& c : vClient) {
            if (c.markForDelete == false) {
                fs << convertRecordToDataLine(c) << endl;
            }
        }
        vClient = LoadDataToFile(FileLocation);
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool addDataToFile(stUser user, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out | ios::app);
    if (fs.is_open()) {
        fs << convertRecordToDataLine(user) << endl;
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool addDataToFile(vector<stUser>& vUser, stUser user, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out | ios::app);
    if (fs.is_open()) {
        fs << convertRecordToDataLine(user) << endl;
        vUser.push_back(user);
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool saveDataToFile(vector<stUser>& vUser, string FileLocation) {
    fstream fs;
    string dataLine;
    fs.open(FileLocation, ios::out);
    if (fs.is_open()) {
        for (stUser& c : vUser) {
            if (c.markForDelete == false) {
                fs << convertRecordToDataLine(c) << endl;
            }
        }
        vUser = LoadDataToFileUser(FileLocation);
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

bool checkAccessLevel(enMainManueAccessLevelOptions AccessLevel) {
    if (AccessLevel == enMainManueAccessLevelOptions::eAllAccessLevel) {
        return true;
    }
    if ((AccessLevel & currentUser.accessLevel) == AccessLevel) {
        return true;
    }
    else {
        return false;
    }
}

void showAccessDeniedScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "Access Denied,\n";
    cout << "You dont Have Permission To Do this,\n";
    cout << "Please Contact Your Admin.\n";
    cout << "====================================================================================\n\n\n\n";
}


void printClientRecordCard(stClient &client) {
    cout << "====================================================================================\n";
    cout << "\t\t\t Client Card" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    cout << "Account Number : " << client.accountNumber << '\n';
    cout << "Pin Code       : " << client.pinCode << '\n';
    cout << "Account Name   : " << client.accountName << '\n';
    cout << "Account Phone  : " << client.phone << '\n';
    cout << "Account Balance : " << client.AccountBalance << '\n';
}

void printUserRecordCard(stUser& user) {
    cout << "====================================================================================\n";
    cout << "\t\t\t User Card" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    cout << "User Name      : " << user.userName << '\n';
    cout << "Password       : " << user.password << '\n';
    cout << "Access Level   : " << user.accessLevel << '\n';
}

void printClientRecordLine(stClient &client) {
    cout << "| " << left << setw(6) << client.accountNumber;
    cout << " | " << left << setw(8) << client.pinCode;
    cout << " | " << left << setw(16) << client.accountName;
    cout << " | " << left << setw(16) << client.phone;
    cout << " | " << left << setw(22) << client.AccountBalance << " |\n";
}

void printAllClientRecord(vector<stClient> &vClient) {
    cout << "====================================================================================\n";
    cout << "\t\t\t Total Clients: " << vClient.size() << " (current user : " << currentUser.userName <<")\n";
    cout << "====================================================================================\n";
    cout << "| Number | Pin Code | Name             | Phone            | Balance                |\n";
    cout << "====================================================================================\n";
    if (!vClient.empty()) {
        for (stClient & c : vClient) {
            printClientRecordLine(c);
        }
    }
    else {
        cout << "There's no Clients\n";
    }
    cout << "====================================================================================\n";
}

void printClientRecordLineBalance(stClient& client) {
    cout << "| " << left << setw(6) << client.accountNumber;
    cout << " | " << left << setw(8) << client.pinCode;
    cout << " | " << left << setw(16) << client.accountName;
    cout << " | " << left << setw(41) << client.AccountBalance << " |\n";
}

void printAllClientRecordBalance(vector<stClient>& vClient) {
    long totalBalances = 0;
    cout << "====================================================================================\n";
    cout << "\t\t\t Total Clients: " << vClient.size() << " (current user : " << currentUser.userName <<")\n";
    cout << "====================================================================================\n";
    cout << "| Number | Pin Code | Name             | Balance                                   |\n";
    cout << "====================================================================================\n";
    if (!vClient.empty()) {
        for (stClient& c : vClient) {
            printClientRecordLineBalance(c);
            totalBalances += c.AccountBalance;
        }
    }
    else {
        cout << "There's no Clients\n";
    }
    cout << "====================================================================================\n";
    cout << "   "<< left << setw(21) <<"\t\t\tTotal balances: "<< totalBalances <<" \n";
    cout << "====================================================================================\n";
}


bool checkExistClient(stClient &client) {
    vector<stClient> vClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    if (findClientWithAccountNumber(client, vClient, client.accountNumber)) {
        return true;
    }
    return false;
}

stClient createNewClient() {
    stClient client;
    cout << "____________________________________________________________________________________\n";
    cout << "\t\t\t Create New Accout" << " (current user: " << currentUser.userName << ")\n";
    cout << "____________________________________________________________________________________\n";
    cout << "\nEnter Account Number: ";
    getline(cin >> ws, client.accountNumber);
    while (checkExistClient(client)) {
        cout << "\nAccount Number [" << client.accountNumber << "] is exist.";
        cout << "\nEnter Account Number: ";
        getline(cin >> ws, client.accountNumber);
    }
    
    cout << "Enter Pin Code: ";
    getline(cin >> ws, client.pinCode);
    cout << "Enter Account Name: ";
    getline(cin >> ws, client.accountName);
    cout << "Enter Phone: ";
    getline(cin >> ws, client.phone);
    cout << "Enter Balance: ";
    cin >> client.AccountBalance;
    return client;
}

void showClientsListScreen() {

    if (!checkAccessLevel(enMainManueAccessLevelOptions::eShowClientsListAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }

    cout << "====================================================================================\n";
    cout << "\t\t\t Clients List Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stClient> vClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    printAllClientRecord(vClient);
}

void showAddNewClientScreen() {

    if (!checkAccessLevel(enMainManueAccessLevelOptions::eAddNewClientAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    cout << "====================================================================================\n";
    cout << "\t\t\t Add New Client Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    char answer = 'Y';
    do {
        addDataToFile(createNewClient(), LocationClientDataFile);
        cout << "____________________________________________________________________________________\n";
        answer = readChar("do you wanna add more client? y/n? ");
        cout << "____________________________________________________________________________________\n";
    } while (toupper(answer) == 'Y');
}

bool findClientWithAccountNumber(stClient& client, vector<stClient>& vClient, string AccountNumber) {
    for (stClient &c : vClient) {
        if (c.accountNumber == AccountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

bool markClinetForDelete(stClient &client, vector<stClient> & vClient, string AccountNumber) {
    for (stClient &c : vClient) {
        if (c.accountNumber == AccountNumber) {
            c.markForDelete = true;
            client = c;
            return true;
        }
    }
    return false;
}

bool markUserForDelete(stUser& user, vector<stUser>& vUser, string userName) {
    for (stUser& u : vUser) {
        if (u.userName == userName) {
            if (user.userName == "admin") {
                cout << "you can't delete admin user\n";
            }
            else {
                u.markForDelete = true;
            }
            user = u;
            return true;
        }
    }
    return false;
}

void showDeleteClientScreen() {
    if (!checkAccessLevel(enMainManueAccessLevelOptions::eDeleteClientAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    cout << "====================================================================================\n";
    cout << "\t\t\t Delete Client Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stClient client;
    vector<stClient> vClient;
    string accountNumber;
    char answer = 'N';
    vClient = LoadDataToFile(LocationClientDataFile);
    do {
        cout << "\nTo delete client\n";
        cout << "Enter the Account Number: ";
        getline(cin >> ws, accountNumber);
        if (markClinetForDelete(client, vClient, accountNumber)) {
            printClientRecordCard(client);
            answer = readChar("do you wanna delete this client? y/n? ");
            if (toupper(answer) == 'Y') {
                saveDataToFile(vClient, LocationClientDataFile);
            }
        }
        answer = readChar("do you wanna delete more client? y/n? ");
    } while (toupper(answer) == 'Y');
}

stClient changeClientRecord(stClient &client) {
    cout << "Enter the Pin Code: ";
    getline(cin >> ws, client.pinCode);
    cout << "Enter the Account Name: ";
    getline(cin >> ws, client.accountName);
    cout << "Enter the Phone: ";
    getline(cin >> ws, client.phone);
    client.AccountBalance = readDouble("Enter the Account Balance: ");
    return client;
}

bool UpdateClientWithAccountNumber(stClient &client, vector<stClient>& vClient, string AccountNumber) {
    for (stClient& c : vClient) {
        if (c.accountNumber == AccountNumber) {
            c = changeClientRecord(c);
            client = c;
            return true;
        }
    }
    return false;
}

void showUpdateClientScreen() {
    if (!checkAccessLevel(enMainManueAccessLevelOptions::eUpdateClientAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    cout << "====================================================================================\n";
    cout << "\t\t\t Update Client Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stClient client;
    vector<stClient> vClient;
    string accountNumber;
    char answer = 'N';
    vClient = LoadDataToFile(LocationClientDataFile);

    do {
        cout << "\nTo update client\n";
        cout << "Enter the Account Number: ";
        getline(cin >> ws, accountNumber);
        if (findClientWithAccountNumber(client, vClient, accountNumber)) {

            printClientRecordCard(client);
            
            answer = readChar("do you wanna update this client? y/n? ");
            if (toupper(answer) == 'Y') {
                UpdateClientWithAccountNumber(client ,vClient, accountNumber);
                printClientRecordCard(client);
            }
        }
        answer = readChar("do you wanna update more client? y/n? ");
    } while (toupper(answer) == 'Y');

    answer = readChar("do you wanna save this updates? y/n? ");
    if (toupper(answer) == 'Y') {
        saveDataToFile(vClient, LocationClientDataFile);
    }
}

void showFindClientScreen() {
    if (!checkAccessLevel(enMainManueAccessLevelOptions::eFindClientAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    cout << "====================================================================================\n";
    cout << "\t\t\t Find Client Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stClient client;
    vector<stClient> vClient;
    string accountNumber;
    char answer = 'N';
    vClient = LoadDataToFile(LocationClientDataFile);
    do {
        cout << "\nTo find client\n";
        cout << "Enter the Account Number: ";
        getline(cin >> ws, accountNumber);
        if (findClientWithAccountNumber(client, vClient, accountNumber)) {
            printClientRecordCard(client);
        }
        answer = readChar("do you wanna find more client? y/n? ");
    } while (toupper(answer) == 'Y');

}
int readTransactionOption() {
    short input = 0;
    cout << "Enter your choice from [1] to [5] : ";
    cin >> input;
    while (cin.fail() || input < 1 || input > 5) {
        cout << "invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter your choice from [1] to [5] : ";
        cin >> input;
    }
    return input;
}

int readManageUserOption() {
    short input = 0;
    cout << "Enter your choice from [1] to [6] : ";
    cin >> input;
    while (cin.fail() || input < 1 || input > 6) {
        cout << "invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter your choice from [1] to [6] : ";
        cin >> input;
    }
    return input;
}

void backToHomeScreen() {
    cout << "press any key to leave from this page...";
    system("pause>0");
    showMainManueScreen();
}
void backToTransationScreen() {
    cout << "press any key to leave from this page...";
    system("pause>0");
    showTransactionScreen();
}
void backToManageUserScreen() {
    cout << "press any key to leave from this page...";
    system("pause>0");
    showManageUsersScreen();
}
bool depositClient(stClient& toClient, double value) {

    if (value > 0) {
        toClient.AccountBalance += value;
        cout << "deposit completed" << endl;
        return true;
    }
    cout << "deposit not completed" << endl;
    return false;
}
bool changeBalanceToClient(vector<stClient>& vClient, stClient toClient) {
    for (stClient& client : vClient) {
        if (client.accountNumber == toClient.accountNumber) {
            client = toClient;
            cout << "\nTransfer sent." << endl;
            return true;
        }
    }
    cout << "\nTransfer not sent." << endl;
    return false;
}
vector<string> getDepositClient() {
    string toClientAccounntNumber;
    double value;
    vector<string> transferInfo;
    cout << "\nEnter Account Number to Client: ";
    getline(cin >> ws, toClientAccounntNumber);
    transferInfo.push_back(toClientAccounntNumber);
    cout << "\Enter value to Client " << toClientAccounntNumber << " : ";
    value = readDouble("");
    transferInfo.push_back(to_string(value));
    return transferInfo;
}
void showClientDepositScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Clients Deposit Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stClient> vClient;
    vector<string> depositInfo;
    stClient toClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    char answer = 'N';
    do {
        depositInfo = getDepositClient();
        if (findClientWithAccountNumber(toClient, vClient, depositInfo[0])) {

            // print before
            printClientRecordCard(toClient);

            if (depositClient(toClient, stod(depositInfo[1]))) {

                cout << "====================================================================================\n";
                cout << "\t\t\t Changed to New Values\n";
                cout << "====================================================================================\n";

                // print after
                printClientRecordCard(toClient);

                if (changeBalanceToClient(vClient, toClient)) {
                    answer = readChar("\ndo you wanna save this data? y/n");
                    if (toupper(answer) == 'Y') {
                        saveDataToFile(vClient, LocationClientDataFile);
                        cout << "\ndata saved." << endl;
                    }
                    else {
                        cout << "\ndata not saved." << endl;
                    }

                }

            }
        }
        else {
            cout << "User not Exist." << endl;
        }
        answer = readChar("\ndo you wanna do more deposit? y/n");
    } while (toupper(answer) == 'Y');

}

bool withdrawClient(stClient& toClient, double value) {

    if (value > 0 && ((toClient.AccountBalance - value) >= 0)) {
        toClient.AccountBalance -= value;
        cout << "withdraw completed" << endl;
        return true;
    }
    cout << "withdraw not completed" << endl;
    return false;
}

vector<string> getWithdrawClient() {
    string toClientAccounntNumber;
    double value;
    vector<string> withdrawInfo;
    cout << "\nEnter Account Number to Client: ";
    getline(cin >> ws, toClientAccounntNumber);
    withdrawInfo.push_back(toClientAccounntNumber);
    cout << "\Enter value to Client " << toClientAccounntNumber << " : ";
    value = readDouble("");
    withdrawInfo.push_back(to_string(value));
    return withdrawInfo;
}

void showClientWithdrawScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Clients Withdraw Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stClient> vClient;
    vector<string> withdrawInfo;
    stClient toClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    char answer = 'N';
    do {
        withdrawInfo = getWithdrawClient();
        if (findClientWithAccountNumber(toClient, vClient, withdrawInfo[0])) {

            // print before
            printClientRecordCard(toClient);

            if (withdrawClient(toClient, stod(withdrawInfo[1]))) {

                cout << "====================================================================================\n";
                cout << "\t\t\t Changed to New Values\n";
                cout << "====================================================================================\n";

                // print after
                printClientRecordCard(toClient);

                if (changeBalanceToClient(vClient, toClient)) {
                    answer = readChar("\ndo you wanna save this data? y/n");
                    if (toupper(answer) == 'Y') {
                        saveDataToFile(vClient, LocationClientDataFile);
                        cout << "\ndata saved." << endl;
                    }
                    else {
                        cout << "\ndata not saved." << endl;
                    }
                }

            }
        }
        else {
            cout << "User not Exist." << endl;
        }
        answer = readChar("\ndo you wanna do more withdraw? y/n");
    } while (toupper(answer) == 'Y');

}

bool transferClient(stClient& fromClient, stClient& toClient, double value) {

    if (fromClient.AccountBalance > 0 && fromClient.AccountBalance >= value && value > 0) {
        fromClient.AccountBalance -= value;
        toClient.AccountBalance += value;
        cout << "transfer completed" << endl;
        return true;
    }
    cout << "transfer not completed" << endl;
    return false;
}

vector<string> getTransferClients() {
    string fromClientAccounntNumber, toClientAccounntNumber;
    double value;
    vector<string> transferInfo;
    cout << "\nEnter Account Number From Client: ";
    getline(cin >> ws, fromClientAccounntNumber);
    transferInfo.push_back(fromClientAccounntNumber);
    cout << "\Enter value From Client " << fromClientAccounntNumber << " : ";
    value = readDouble("");
    transferInfo.push_back(to_string(value));
    cout << "\nEnter Account Number To Client: ";
    getline(cin >> ws, toClientAccounntNumber);
    transferInfo.push_back(toClientAccounntNumber);
    return transferInfo;
}

bool changeBalanceFromClient(vector<stClient>& vClient, stClient fromClient, stClient toClient) {
    short counter = 0;
    for (stClient& client : vClient) {
        if (client.accountNumber == fromClient.accountNumber) {
            client = fromClient;
            ++counter;
        }
        if (client.accountNumber == toClient.accountNumber) {
            client = toClient;
            ++counter;
        }
        if (counter == 2) {
            cout << "\nTransfer sent." << endl;
            return true;
        }
    }
    cout << "\nTransfer not sent." << endl;
    return false;
}

void showClientTransferScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Clients Transfer Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stClient> vClient;
    vector<string> transferInfo;
    stClient fromClient, toClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    char answer = 'N';
    do {
        transferInfo = getTransferClients();
        if (   findClientWithAccountNumber(fromClient, vClient, transferInfo[0]) // check from user
            && findClientWithAccountNumber(toClient, vClient, transferInfo[2])  // check to user
            && !(transferInfo[0] == transferInfo[2]) // check not send to same user
            ) {

            // print before
            printClientRecordCard(fromClient);
            printClientRecordCard(toClient);

            if (transferClient(fromClient, toClient, stod(transferInfo[1]))) {

                cout << "====================================================================================\n";
                cout << "\t\t\t Changed to New Values\n";
                cout << "====================================================================================\n";

                // print after
                printClientRecordCard(fromClient);
                printClientRecordCard(toClient);

                if (changeBalanceFromClient(vClient, fromClient, toClient)) {
                    answer = readChar("\ndo you wanna save this data? y/n");
                    if (toupper(answer) == 'Y') {
                        saveDataToFile(vClient, LocationClientDataFile);
                        cout << "\ndata saved." << endl;
                    }
                    else {
                        cout << "\ndata not saved." << endl;
                    }
                }

            }
        }
        else {
            cout << "User not Exist or User are the same." << endl;
        }
        answer = readChar("\ndo you wanna do more transfer? y/n");
    } while (toupper(answer) == 'Y');
}

void showClientTotalBalanceScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Clients Total Balance Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stClient> vClient;
    vClient = LoadDataToFile(LocationClientDataFile);
    printAllClientRecordBalance(vClient);
}

bool isValidUserName(vector<stUser> vUser, string username) {
    for (stUser &user : vUser) {
        if (user.userName == username) {
            return true;
        }
    }
    return false;
}
bool isValidUserName(vector<stUser> vUser, stUser &getuser, string username) {
    for (stUser& user : vUser) {
        if (user.userName == username) {
            getuser = user;
            return true;
        }
    }
    return false;
}
bool isValidPassword(vector<stUser> vUser, string password) {
    for (stUser& user : vUser) {
        if (password == user.password) {
            return true;
        }
    }
    return false;
}

bool isValidUser(vector<stUser> vUser) {
    string userName = "";
    string password = "";
    stUser user;
    cout << "Enter User Name? ";
    getline(cin >> ws, userName);

    if (isValidUserName(vUser, user, userName)) {

        cout << "Enter Password? ";
        getline(cin >> ws, password);

        if (user.password == password) {
            currentUser = user;
            return true;
        }
        else {
            return false;
        }

    }
    else {
        return false;
    }
}

void printUserRecordLine(stUser& user) {
    cout << "| " << left << setw(25) << user.userName;
    cout << " | " << left << setw(26) << user.password;
    cout << " | " << left << setw(23) << user.accessLevel << " |\n";
}

void printAllUserRecord(vector<stUser>& vUser) {
    cout << "====================================================================================\n";
    cout << "\t\t\t Total Users: " << vUser.size() << " (current user : " << currentUser.userName <<")\n";
    cout << "====================================================================================\n";
    cout << "| User Name                 | Password                   | Access Level            |\n";
    cout << "====================================================================================\n";
    if (!vUser.empty()) {
        for (stUser& u : vUser) {
            printUserRecordLine(u);
        }
    }
    else {
        cout << "There's no Clients\n";
    }
    cout << "====================================================================================\n";
}


void showLoginScreen(bool validUser = false) {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Login Screen\n";
    cout << "====================================================================================\n";
    vector<stUser> vUser;
    vUser = LoadDataToFileUser(LocationUserDataFile);
    if (validUser) {
        cout << "Invalid username/password.\n";
    }
    bool ValidUser = isValidUser(vUser);
    if (ValidUser) {
        showMainManueScreen();
    }
    else {
        showLoginScreen(!ValidUser);
    }
}
bool findUserWithUserName(stUser& user, vector<stUser>& vUser, string userName) {
    for (stUser& u : vUser) {
        if (u.userName == userName) {
            user = u;
            return true;
        }
    }
    return false;
}

void showUserListScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Users List Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    vector<stUser> vUser;
    vUser = LoadDataToFileUser(LocationUserDataFile);
    printAllUserRecord(vUser);
}

bool checkExistUser(stUser& user) {
    vector<stUser> vUser;
    vUser = LoadDataToFileUser(LocationUserDataFile);
    if (findUserWithUserName(user, vUser, user.userName)) {
        return true;
    }
    return false;
}

short accessLevelUser() {
    short accessLevel = 0;
    char answer = 'N';
    answer = readChar("Do you want to give full access? y/n? ");
    if (toupper(answer) == 'Y') {
        accessLevel = -1;
    }
    else {
        cout << "Do you want to give access to :\n";
        answer = readChar("Show Client List? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eShowClientsListAccessLevel;
        }
        answer = readChar("Add New Client? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eAddNewClientAccessLevel;
        }
        answer = readChar("Delete Client? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eDeleteClientAccessLevel;
        }
        answer = readChar("Update Client? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eUpdateClientAccessLevel;
        }
        answer = readChar("Find Client? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eFindClientAccessLevel;
        }
        answer = readChar("Transactions? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eTransactionAccessLevel;
        }
        answer = readChar("Manager User? y/n? ");
        if (toupper(answer) == 'Y') {
            accessLevel += enMainManueAccessLevelOptions::eManageUsersAccessLevel;
        }
    }
    return accessLevel;
}

stUser createNewUser() {
    stUser user;
    cout << "____________________________________________________________________________________\n";
    cout << "\t\t\t Create New User" << " (current user: " << currentUser.userName << ")\n";
    cout << "____________________________________________________________________________________\n";
    cout << "\nEnter User Name: ";
    getline(cin >> ws, user.userName);
    while (checkExistUser(user)) {
        cout << "\nUser Name [" << user.userName << "] is exist.";
        cout << "\nEnter User Name: ";
        getline(cin >> ws, user.userName);
    }

    cout << "Enter Password: ";
    getline(cin >> ws, user.password);

    //user.accessLevel = readShort("Enter Access Level: ");
    user.accessLevel = accessLevelUser();
    return user;
}

void showAddNewLUserScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Add New User Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    char answer = 'Y';
    do {
        addDataToFile(createNewUser(), LocationUserDataFile);
        cout << "____________________________________________________________________________________\n";
        answer = readChar("do you wanna add more user? y/n? ");
        cout << "____________________________________________________________________________________\n";
    } while (toupper(answer) == 'Y');
}

void showDeleteUserScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Delete User Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stUser user;
    vector<stUser> vUser;
    string userName;
    char answer = 'N';
    vUser = LoadDataToFileUser(LocationUserDataFile);
    do {
        cout << "\nTo delete user\n";
        cout << "Enter the User Name: ";
        getline(cin >> ws, userName);
        if (markUserForDelete(user, vUser, userName)) {
            printUserRecordCard(user);
            answer = readChar("do you wanna delete this user? y/n? ");
            if (toupper(answer) == 'Y') {
                saveDataToFile(vUser, LocationUserDataFile);
            }
        }
        answer = readChar("do you wanna delete more user? y/n? ");
    } while (toupper(answer) == 'Y');
}

stUser changeUserRecord(stUser& user) {
    cout << "Enter the Password: ";
    getline(cin >> ws, user.password);
    user.accessLevel = readShort("Enter the Access Level: ");
    return user;
}

bool UpdateUserWithUserName(stUser& user, vector<stUser>& vUser, string userName) {
    for (stUser& u : vUser) {
        if (u.userName == userName) {
            u = changeUserRecord(u);
            user = u;
            return true;
        }
    }
    return false;
}

void showUpdateUserScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Update User Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stUser user;
    vector<stUser> vUser;
    string userName;
    char answer = 'N';
    vUser = LoadDataToFileUser(LocationUserDataFile);

    do {
        cout << "\nTo update user\n";
        cout << "Enter the User Name: ";
        getline(cin >> ws, userName);
        if (findUserWithUserName(user, vUser, userName)) {

            printUserRecordCard(user);

            answer = readChar("do you wanna update this user? y/n? ");
            if (toupper(answer) == 'Y') {
                UpdateUserWithUserName(user, vUser, userName);
                printUserRecordCard(user);
                cout << "User updated successfully.";
            }
        }
        answer = readChar("do you wanna update more user? y/n? ");
    } while (toupper(answer) == 'Y');

    answer = readChar("do you wanna save this updates? y/n? ");
    if (toupper(answer) == 'Y') {
        saveDataToFile(vUser, LocationUserDataFile);
    }
}

void showFindUserScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Find User Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    stUser user;
    vector<stUser> vUser;
    string userName;
    char answer = 'N';
    vUser = LoadDataToFileUser(LocationUserDataFile);
    do {
        cout << "\nTo find user\n";
        cout << "Enter the User Name: ";
        getline(cin >> ws, userName);
        if (findUserWithUserName(user, vUser, userName)) {
            printUserRecordCard(user);
        }
        answer = readChar("do you wanna find more user? y/n? ");
    } while (toupper(answer) == 'Y');

}

void performManageUsersManueOptions(enManageUsersManueOptions MainManueOptions) {
    switch (MainManueOptions) {
    case enManageUsersManueOptions::eShowUserList:
        system("cls");
        showUserListScreen();
        backToManageUserScreen();
        break;
    case enManageUsersManueOptions::eAddNewUser:
        system("cls");
        showAddNewLUserScreen();
        backToManageUserScreen();
        break;
    case enManageUsersManueOptions::eDeleteUser:
        system("cls");
        showDeleteUserScreen();
        backToManageUserScreen();
        break;
    case enManageUsersManueOptions::eUpdateUser:
        system("cls");
        showUpdateUserScreen();
        backToManageUserScreen();
        break;
    case enManageUsersManueOptions::eFindUser:
        //system("cls");
        showFindUserScreen();
        backToManageUserScreen();
        break;
    case enManageUsersManueOptions::eMainManue:
        //system("cls");
        backToHomeScreen();
        break;
    }
}

void showManageUsersScreen() {
    if (!checkAccessLevel(enMainManueAccessLevelOptions::eManageUsersAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Manage Users Screen"<< " current user: " << currentUser.userName <<"\n";
    cout << "====================================================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Back To Main Manue.\n";
    cout << "====================================================================================\n";
    performManageUsersManueOptions((enManageUsersManueOptions)readManageUserOption());
}

void performTransationManueOptions(enTransactionManueOptions MainManueOptions) {
    switch (MainManueOptions) {
        case enTransactionManueOptions::eDeposit:
            system("cls");
            showClientDepositScreen();
            backToTransationScreen();
            break;
        case enTransactionManueOptions::eWithdraw:
            system("cls");
            showClientWithdrawScreen();
            backToTransationScreen();
            break;
        case enTransactionManueOptions::eTransfer:
            system("cls");
            showClientTransferScreen();
            backToTransationScreen();
            break;
        case enTransactionManueOptions::eTotalBalance:
            system("cls");
            showClientTotalBalanceScreen();
            backToTransationScreen();
            break;
        case enTransactionManueOptions::eBackToMainManue:
            //system("cls");
            backToHomeScreen();
            break;
    }
}

void showTransactionScreen() {
    if (!checkAccessLevel(enMainManueAccessLevelOptions::eTransactionAccessLevel)) {
        showAccessDeniedScreen();
        backToHomeScreen();
        return;
    }
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Transaction Manue Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Transfer.\n";
    cout << "\t[4] Total Balance.\n";
    cout << "\t[5] Back To Main Manue.\n";
    cout << "====================================================================================\n";
    performTransationManueOptions((enTransactionManueOptions)readTransactionOption());
}


void showExitScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Program exit ;-)\n";
    cout << "====================================================================================\n";
}

void performManueOptions(enMainManueOptions MainManueOptions) {
    switch (MainManueOptions) {
        case enMainManueOptions::eShowClientsList:
            system("cls");
            showClientsListScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eAddNewClient:
            system("cls");
            showAddNewClientScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eDeleteClient:
            system("cls");
            showDeleteClientScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eUpdateClient:
            system("cls");
            showUpdateClientScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eFindClient:
            system("cls");
            showFindClientScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eTransaction:
            showTransactionScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eManageUsers:
            showManageUsersScreen();
            backToHomeScreen();
            break;
        case enMainManueOptions::eLogout:
            showLoginScreen();
            break;
    }
}

int readManueOption() {
    short input = 0;
    cout << "Enter your choice from [1] to [8] : ";
    cin >> input;
    while (cin.fail() || input < 1 || input > 8) {
        cout << "invalid input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter your choice from [1] to [8] : ";
        cin >> input;
    }
    return input;
}

void showMainManueScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Main Manue Screen" << " (current user: " << currentUser.userName << ")\n";
    cout << "====================================================================================\n";
    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "====================================================================================\n";
    performManueOptions((enMainManueOptions)readManueOption());
}

int main()
{
    showLoginScreen();
    system("pause>0");
}