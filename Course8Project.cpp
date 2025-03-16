#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

const string filename = "Clients.txt";
const string filename2 = "Users.txt";
string LoginUserName;
void Login();
void TransactionsMenue();
void ManageUserScreen();
void MainScreen();
void PrintNewScreenTitle(string title);

enum enMainScreenButtons {
    ShowClientList = 1, AddNewclient, DeleteClient, UpdateClient, Findclient, Transactions, ManageUsers,Logout
};

enum enTransactionsButtons {
    Deposit = 1, Withdraw, TotalBalances, MainMenue
};

enum enManageUsersScreen {
    eListUsers=1,eAddNewUser,eDeleteUser,eUpdateUser,eFindUser,eMainMenue
};

struct sClient {
    string AccountNumber, PinCode, Name, Phone;
    double AccountBalance;
};

struct sUser {
    string UserName;
    int Password;
    short Permissions;
};

string ReadData() {
    string input;
    cout << "Please enter account number ? "; cin >> input;
    return input;
}

double ReadNum(string x) {
    double num;
    cout << "Please Enter " << x << " Amount ? ";
    cin >> num;
    return num;
}

vector<string> Split(string line, string separator) {
    vector<string> vstring;
    size_t pos = 0;
    string token;
    while ((pos = line.find(separator)) != string::npos) {
        token = line.substr(0, pos);
        if (!token.empty()) {
            vstring.push_back(token);
        }
        line.erase(0, pos + separator.length());
    }
    if (!line.empty()) {
        vstring.push_back(line);
    }
    return vstring;
}

sClient Line2Record(string Line, string Separator = "#//#") {
    sClient Client;
    vector<string> vClientData = Split(Line, Separator);
    if (vClientData.size() == 5) {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }
    return Client;
}

sUser LineToRecord(string Line, string Separator = "#//#") {
    sUser User;
    vector<string> vUserData = Split(Line, Separator);
    if (vUserData.size() == 3) {
        User.UserName = vUserData[0];
        User.Password= stoi(vUserData[1]);
        User.Permissions= stoi(vUserData[2]);
    }
    return User;
}

string Record2Line(const sClient& Client, string Separator = "#//#") {
    return Client.AccountNumber + Separator + Client.PinCode + Separator + Client.Name + Separator + Client.Phone + Separator + to_string(Client.AccountBalance);
}

string Record2Line(const sUser& user, string Separator = "#//#") {
    return user.UserName + Separator + to_string(user.Password)+ Separator + to_string(user.Permissions);
}

vector <sClient> ImportFromFile(const string& filename) {
    vector<sClient> vClients;
    ifstream myfile(filename);
    if (myfile.is_open()) {
        string Line;
        while (getline(myfile, Line)) {
            sClient Client = Line2Record(Line);
            vClients.push_back(Client);
        }
        myfile.close();
    }
    return vClients;
}

vector <sUser> ImportFromFile2(const string& filename) {
    vector<sUser> vUsers;
    ifstream myfile(filename2);
    if (myfile.is_open()) {
        string Line;
        while (getline(myfile, Line)) {
            sUser User= LineToRecord(Line);
            vUsers.push_back(User);
        }
        myfile.close();
    }
    return vUsers;
}

void SaveClientsToFile(const vector<sClient>& vClients) {
    ofstream myfile(filename);
    if (myfile.is_open()) {
        for (const sClient& Client : vClients) {
            myfile << Record2Line(Client) << endl;
        }
        myfile.close();
    }
}

void SaveUsersToFile(const vector<sUser>& vUsers) {
    ofstream myfile(filename2);
    if (myfile.is_open()) {
        for (const sUser& user : vUsers) {
            myfile << Record2Line(user) << endl;
        }
        myfile.close();
    }
}

void PrintClientRecord(sClient Client)
{
    cout << "|" << left << setw(15) << Client.AccountNumber;
    cout << "|" << left << setw(15) << Client.PinCode;
    cout << "|" << left << setw(35) << Client.Name;
    cout << "|" << left << setw(15) << Client.Phone;
    cout << "|" << left << setw(15) << Client.AccountBalance;
}

void PrintUserRecord(sUser User)
{
    cout << "|" << left << setw(30) << User.UserName;
    cout << "|" << left << setw(15) << User.Password;
    cout << "|" << left << setw(15) << User.Permissions;
}

void PrintClientRecordB(sClient Client)
{
    cout << "|" << left << setw(15) << Client.AccountNumber;
    cout << "|" << left << setw(35) << Client.Name;
    cout << "|" << left << setw(15) << Client.AccountBalance;
}

void PrintClientData(const sClient& Client) {
    cout << "\nThe following are client details:\n\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PinCode        : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

void PrintUserData(const sUser& user) {
    cout << "\nThe following are user details:\n\n";
    cout << "============================================\n";   
    cout << "Username    : " << user.UserName << endl;
    cout << "Password    : " << user.Password << endl;
    cout << "Permissions : " << user.Permissions<< endl;
    cout << "============================================\n";
}

bool IsExist(sClient& foundClient, const string& input) {
    vector<sClient> vClients = ImportFromFile(filename);
    for (const sClient& Client : vClients) {
        if (Client.AccountNumber == input) {
            foundClient = Client;
            return true;
        }
    }
    return false;
}

bool IsExist( const string& username) {
    vector<sUser> vUsers = ImportFromFile2(filename2);
    for (const sUser& User: vUsers) {
        if (User.UserName == username) {
            
            return true;
        }
    }
    return false;
}

void UpdateClientData(sClient& Client) {
    cout << "Enter new PinCode: ";
    cin >> Client.PinCode;
    cout << "Enter new Name: ";
    cin.ignore();
    getline(cin, Client.Name);
    cout << "Enter new Phone: ";
    cin >> Client.Phone;
    cout << "Enter new Account Balance: ";
    cin >> Client.AccountBalance;
    cout << "\nClient data updated successfully!\n\n";
}

short AskForPermissions() {
    short Permissions = 0;
    char YesOrNo='n';
    cout << "\n\nDo you want to give access to : \n";
    cout << "\nShow Client List? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 0);
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 1);
    }

    cout << "\nDelete Client? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions|= (1 << 2);
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 3);
    }

    cout << "\nFind Client? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 4);
    }

    cout << "\nTransactions? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 5);
    }

    cout << "\nManage Users? y/n? ";
    cin >> YesOrNo;
    if (toupper(YesOrNo) == 'Y') {
        Permissions |= (1 << 6);
    }

    return Permissions;
}

sUser UpdateUserData(sUser& user) {
    cout << "\nEnter Password? ";
    cin >> user.Password;
    char input;
    cout << "\nDo you want to give full access? y/n? ";
    cin >> input;
    if (toupper(input) == 'Y') {
        user.Permissions = -1;
    }
    else {
        user.Permissions = AskForPermissions();
    }
    cout << "\nClient data updated successfully!\n\n";
    return user;
}

void DeleteClientData() {
    vector<sClient> vClients = ImportFromFile(filename);
    vector<sClient> UpdatedClients;
    char x;
    short counter = 0;
    PrintNewScreenTitle("Delete Client Screen");
    string AccountNumber = ReadData();
    // «·«Õ ›«Ÿ »«·⁄„·«¡ €Ì— «·„—«œ Õ–›Â„
    for (const auto& client : vClients) {
        if (client.AccountNumber != AccountNumber) {
            UpdatedClients.push_back(client);
            counter++;

        }
        else if (client.AccountNumber == AccountNumber) {
            PrintClientData(client);
            cout << "\nAre you sure you want to delete this client ? \n";
            cin >> x;
            if (toupper(x) == 'Y') {
                SaveClientsToFile(UpdatedClients);
                cout << "\nClient deleted successfully.\n\n";
                return;
            }
        }
    }
    if (counter == vClients.size()) {
        cout << "\nThe Client With Account Number \"" << AccountNumber << "\" Doesnt exist!\n\n";
    }
}

void PrintClientList() {
    vector<sClient> x = ImportFromFile(filename);
    system("cls");
    cout << "\t\t\t\tClient List {" << x.size() << "} Client (s).\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(15) << "Account Number ";
    cout << "|" << left << setw(15) << "Pin Code ";
    cout << "|" << left << setw(35) << "Client Name ";
    cout << "|" << left << setw(15) << "Phone ";
    cout << "|" << left << setw(15) << "Balance" << endl;
    cout << "--------------------------------------------------------------------------------------------\n";
    for (sClient& client : x) {
        PrintClientRecord(client);
        cout << endl;
    }
    cout << "\n\n--------------------------------------------------------------------------------------------\n";
}

void PrintClientBalances() {
    vector<sClient> x = ImportFromFile(filename);
    system("cls");
    cout << "\t\t\t\tClient List {" << x.size() << "} Client (s).\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(15) << "Account Number ";
    cout << "|" << left << setw(35) << "Client Name ";
    cout << "|" << left << setw(15) << "Balance" << endl;
    cout << "--------------------------------------------------------------------------------------------\n";
    double sum = 0;
    for (sClient& client : x) {
        PrintClientRecordB(client);
        sum += client.AccountBalance;
        cout << endl;
    }
    cout << "\n\n--------------------------------------------------------------------------------------------\n";
    cout << "\t\t\tTotal Balances = " << sum << endl;
}

sClient IsExist(const vector<sClient>& clients, const string& accountNumber) {
    for (const auto& client : clients) {
        if (client.AccountNumber == accountNumber) {
            return  client;// ≈‰‘«¡ ‰”Œ… ÃœÌœ… „‰ »Ì«‰«  «·⁄„Ì·
        }
    }
    //„Â„ «·”ÿ— œ«
    return { "", "", "", "", -1 }; // „ƒ‘— ≈·Ï √‰ «·Õ”«» €Ì— „ÊÃÊœ
}
 
sUser IsExist(const vector<sUser>& users, const string& username) {
    for (const auto& user : users) {
        if (user.UserName == username) {
            return  user;// ≈‰‘«¡ ‰”Œ… ÃœÌœ… „‰ »Ì«‰«  «·⁄„Ì·
        }
    }
    //„Â„ «·”ÿ— œ«
    return { "", -1, -1 }; // „ƒ‘— ≈·Ï √‰ «·Õ”«» €Ì— „ÊÃÊœ
}

sClient ReadClientData() {
    sClient newClient;

    cout << "Enter Account Number: ";
    cin >> newClient.AccountNumber;
    while (IsExist(newClient, newClient.AccountNumber))
    {
        cout << "\nThe Account Number You Entered is already exists ,Try again :\n";
        cout << "Enter Account Number: ";
        cin >> newClient.AccountNumber;
    }

    cout << "Enter PinCode: ";
    cin >> newClient.PinCode;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newClient.Name);
    cout << "Enter Phone: ";
    cin >> newClient.Phone;
    cout << "Enter Account Balance: ";
    cin >> newClient.AccountBalance;
    return newClient;
}

sUser ReadUserData() {
    sUser newuser;
    char input;

    cout << "Adding New User:\n\n";
    cout << "Enter UserName? ";
    getline(cin >> ws, newuser.UserName);
    while (IsExist(newuser.UserName))
    {
        cout << "\nUser with ["<<newuser.UserName<< "] already exists ,Enter another UserName? ";
        getline(cin >> ws, newuser.UserName);
    }
    cout << "Enter Password? ";
    cin >> newuser.Password;
    cout << "\nDo you want to give full access? y/n? ";
    cin >> input;
    if (toupper(input) == 'Y') {
        newuser.Permissions = -1;
    }
    else {
        newuser.Permissions=AskForPermissions();
    }
    return newuser;
}

void AddNewClient() {
    char x = 'n';
    PrintNewScreenTitle("Add New Client Screen");

    sClient newClient;
    do
    {
        newClient = ReadClientData();
        cout << "\nDo You Want to add more Clients? y/n ? \n";
        cin >> x;

    } while (toupper(x) == 'Y');

    vector<sClient> clients = ImportFromFile(filename);
    clients.push_back(newClient);
    SaveClientsToFile(clients);
    cout << "\nNew client added successfully!\n\n";
}

void FindClient() {
    system("cls");
    vector<sClient> clients = ImportFromFile(filename);
    
    string input = ReadData();

    sClient client = IsExist(clients, input);

    if (IsExist(client, input)) {
        PrintClientData(client);
        cout << endl;
    }
    else {
        cout << "\nClient with account number (" << input << ") does not exist !\n" << endl;
    }
}

bool IsValidateUser(string username,int password) {
    vector<sUser> vUsers = ImportFromFile2(filename2);
    for (auto& user : vUsers) {
        if (user.UserName == username) {
            if (user.Password == password)
                return true;
        }
    }
    return false;
}

short GetPermission(string username) {
    vector<sUser> users = ImportFromFile2(filename2);
    sUser user=IsExist(users, username);
    return user.Permissions;
}

void EndTheProgram() {
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t  Program Ends :-)\t\n";
    cout << "--------------------------------------------\n";
}

void UpdatingClientmethod() {
    sClient client;
    system("cls");
    char x = 'n';
    string accnum = ReadData();
    if (IsExist(client, accnum)) {
        PrintClientData(client);
        cout << "\nAre You Sure You Want To Update ? y/n ?\n";
        cin >> x;
        if (toupper(x) == 'Y') {
            UpdateClientData(client);
            vector<sClient> clients = ImportFromFile(filename);
            for (auto& c : clients) {
                if (c.AccountNumber == client.AccountNumber) {
                    c = client;
                    break;
                }
            }
            SaveClientsToFile(clients);
        }
    }
    else {
        cout << "\nClient not found!\n";
    }
}

void UpdateUserScreen() {
    sUser user;
    PrintNewScreenTitle("Update User Screen");
    vector<sUser> users = ImportFromFile2(filename2);
    char x = 'n';
    cout << "\n\nPlease enter username? ";
    getline(cin >> ws, user.UserName);
    if (IsExist(user.UserName)) {
        PrintUserData(IsExist(users,user.UserName));
        cout << "\nAre You Sure You Want To Update ? y/n ?\n";
        cin >> x;
        if (toupper(x) == 'Y') {

            for (auto& currentuser : users) {
                if (currentuser.UserName== user.UserName) {
                    currentuser = UpdateUserData(user);;
                    break;
                }
            }
            SaveUsersToFile(users);
        }
    }
    else {
        cout << "\nUser not found!\n";
    }
}

void PrintNewScreenTitle(string title) {
    system("cls");
    cout << "============================================\n";
    cout << "\t\t"<<title<<"\n";
    cout << "============================================\n";
}

void PrintUsersList() {
    system("cls");
    vector<sUser> vUsers = ImportFromFile2(filename2);
    cout << "\t\t\t\tUser List {" << vUsers.size() << "} User (s).\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(30) << "User Name";
    cout << "|" << left << setw(15) << "Password";
    cout << "|" << left << setw(15) << "Permissions\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    for (sUser& User: vUsers) {
        PrintUserRecord(User);
        cout << endl;
    }
    cout << "\n\n--------------------------------------------------------------------------------------------\n";
}

void AddUserScreen() {
    char x = 'n';
    sUser newUser;

    PrintNewScreenTitle("Add New User Screen");
    do
    {
        newUser = ReadUserData();
        cout << "\nUser added successfully, Do You Want to add more users? y/n ? \n";
        cin >> x;

    } while (toupper(x) == 'Y');

    vector<sUser> users = ImportFromFile2(filename2);
    users.push_back(newUser);
    SaveUsersToFile(users);
}

void DeleteUserScreen() {
    vector<sUser> vUsers = ImportFromFile2(filename2);
    vector<sUser> UpdatedUsers;
    char x;
    string username;
    short counter = 0;
    PrintNewScreenTitle("Delete User Screen");
    cout << "\n\nPlease enter username? ";
    getline(cin >> ws, username);
    // «·«Õ ›«Ÿ »«·⁄„·«¡ €Ì— «·„—«œ Õ–›Â„
    for (const auto& user : vUsers) {
        if (user.UserName!= username) {
            UpdatedUsers.push_back(user);
            counter++;

        }
        else if (user.UserName == username) {
            if (user.Permissions == -1) {
                cout << "\n\nYou Cannot delete this user.\n";
                counter++;
                break;
            }
            PrintUserData(user);
            cout << "\nAre you sure you want to delete this client ? \n";
            cin >> x;
            if (toupper(x) == 'Y') {
                SaveUsersToFile(UpdatedUsers);
                cout << "\nClient deleted successfully.\n\n";
                return;
            }
        }
    }
    if (counter == vUsers.size()) {
        cout << "\nThe User With UserName \"" << username << "\" Doesnt exist!\n\n";
    }
}

void GoBackToManageUserScreen() {
    cout << "\n\npress any key to go back to Manage Users Screen...";
    system("pause>0");
    ManageUserScreen();
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    MainScreen();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    TransactionsMenue();
}

void FindUserScreen() {
    string username;
    PrintNewScreenTitle("Find User Screen");

    cout << "\n\nPlease enter username? ";
    getline(cin >> ws, username);
    vector<sUser> users = ImportFromFile2(filename2);

    if (IsExist(username)) {
        PrintUserData(IsExist(users, username));
    }
    else cout << "\nThe user you entered doesNOT exist\n\n\n";
    
}

void ManageUserScreen() {
    short input = 6;
    PrintNewScreenTitle("Manage User Screen");
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "============================================\n";
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> input;
    switch (input) {
    case enManageUsersScreen::eListUsers:
        PrintUsersList();
        GoBackToManageUserScreen();
        break;
    case enManageUsersScreen::eAddNewUser:
        AddUserScreen();
        GoBackToManageUserScreen();
        break;
    case enManageUsersScreen::eDeleteUser:
        DeleteUserScreen();
        GoBackToManageUserScreen();
        break;
    case enManageUsersScreen::eUpdateUser:
        UpdateUserScreen();
        GoBackToManageUserScreen();
        break;
    case enManageUsersScreen::eFindUser:
        FindUserScreen();
        GoBackToManageUserScreen();
        break;
    case enManageUsersScreen::eMainMenue:
        MainScreen();
        break;
    }
}

bool IsYouPermit(short input,short permissions) {
    
    switch (input)
    {
    case enMainScreenButtons::ShowClientList:
        ((permissions & (1 << 0)) ? true : false);
        break;
    case enMainScreenButtons::AddNewclient:
        ((permissions & (1 << 1)) ? true : false);
        break;
    case enMainScreenButtons::DeleteClient:
        ((permissions & (1 << 2)) ? true : false);
        break;
    case enMainScreenButtons::UpdateClient:
        ((permissions & (1 << 3)) ? true : false);
        break;
    case enMainScreenButtons::Findclient:
        ((permissions & (1 << 4)) ? true : false);
        break;
    case enMainScreenButtons::Transactions:
        ((permissions & (1 << 5)) ? true : false);
        break;
    case enMainScreenButtons::ManageUsers:
        ((permissions & (1 << 6)) ? true : false);
        break;
    default :
        return true;
        break;
    }
}

void AccessDeniedMessage() {
    cout << "============================================\n";
    cout << "Access denied ,\nYou dont have permissions to do this,\nplease contact your admin,";
    cout << "\n============================================\n\n";
}

void PerformMainScreen(short input) {
    
    short permissions = GetPermission(LoginUserName);
    switch (input)
    {
    case enMainScreenButtons::ShowClientList:
        if (IsYouPermit(enMainScreenButtons::ShowClientList,permissions))
        {
            PrintClientList();
        }
        else { AccessDeniedMessage(); }
        GoBackToMainMenue();
        break;
    case enMainScreenButtons::AddNewclient:
        if (IsYouPermit(enMainScreenButtons::AddNewclient, permissions))
        {
            AddNewClient();
        }
        else { AccessDeniedMessage(); }
        GoBackToMainMenue();
        break;
    case enMainScreenButtons::DeleteClient:
        if (IsYouPermit(enMainScreenButtons::DeleteClient, permissions))
        {
            DeleteClientData();
        }
        else { AccessDeniedMessage(); }
        GoBackToMainMenue();
        break;
    case enMainScreenButtons::UpdateClient:
        if (IsYouPermit(enMainScreenButtons::UpdateClient, permissions))
        {
            UpdatingClientmethod();
        }
        else { AccessDeniedMessage(); }
        GoBackToMainMenue();
        break;
    case enMainScreenButtons::Findclient:
        if (IsYouPermit(enMainScreenButtons::Findclient, permissions))
        {
            FindClient();
        }
        else { AccessDeniedMessage(); }
        GoBackToMainMenue();
        break;
    case enMainScreenButtons::Transactions:
        if (IsYouPermit(enMainScreenButtons::Transactions, permissions))
        {
            TransactionsMenue();
        }
        else {
            AccessDeniedMessage();
            GoBackToMainMenue();
        }
        break;
    case enMainScreenButtons::ManageUsers:
        if (IsYouPermit(enMainScreenButtons::ManageUsers, permissions))
        {
            ManageUserScreen();
        }
        else {
            AccessDeniedMessage();
            GoBackToMainMenue();
        }
        break;
    case enMainScreenButtons::Logout:
        Login();
        break;
    }
}

void MainScreen() {
    short input = 8;
    PrintNewScreenTitle("Main Menue Screen");
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "============================================\n";
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> input;

    PerformMainScreen( input);
}

void DepositScreen() {
    PrintNewScreenTitle("Deposit Screen");
    vector<sClient> clients = ImportFromFile(filename);

    sClient newClient;
    cout << "Enter Account Number: ";
    cin >> newClient.AccountNumber;
    while (!IsExist(newClient, newClient.AccountNumber))
    {
        cout << "\nThe Account Number You Entered is already exists ,Try again :\n";
        cout << "Enter Account Number: ";
        cin >> newClient.AccountNumber;
    }
    double deposit = ReadNum("Deopsit");
    char x;
    cout << "\nAre you sure you want to perform this transaction ? y/n ? ";
    cin >> x;
    if (toupper(x) == 'Y') {
        for (auto& client : clients) {
            if (client.AccountNumber == newClient.AccountNumber) {
                client.AccountBalance += deposit;
                break;
            }
        }
    }
    SaveClientsToFile(clients);
}

void WithdrawScreen() {
    PrintNewScreenTitle("Withdraw Screen");
    vector<sClient> clients = ImportFromFile(filename);

    sClient newClient;
    cout << "Enter Account Number: ";
    cin >> newClient.AccountNumber;
    while (!IsExist(newClient, newClient.AccountNumber))
    {
        cout << "\nThe Account Number You Entered is already exists ,Try again :\n";
        cout << "Enter Account Number: ";
        cin >> newClient.AccountNumber;
    }
    sClient client2 = IsExist(clients, newClient.AccountNumber);
    double withdraw = ReadNum("Withdraw");
    while (withdraw > newClient.AccountBalance) {
        cout << "Amount exceeds the balance , Enter another acount lower than " << newClient.AccountBalance << endl;
        withdraw = ReadNum("Withdraw");
    }
    char x;
    cout << "\nAre you sure you want to perform this transaction ? y/n ? ";
    cin >> x;
    if (toupper(x) == 'Y') {

        if (toupper(x) == 'Y') {
            for (auto& client : clients) {
                if (client.AccountNumber == newClient.AccountNumber) {
                    client.AccountBalance += withdraw;
                    break;
                }
            }
        }
    }
    SaveClientsToFile(clients);
}

void TransactionsMenue() {
    int input2 = 4;
    PrintNewScreenTitle("Transactions Menue Screen");
    cout << "\t[1] Deposit\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "============================================\n";
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> input2;

    switch (input2)
    {
    case enTransactionsButtons::Deposit:
        DepositScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::Withdraw:
        WithdrawScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::TotalBalances:
        PrintClientBalances();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::MainMenue:
        MainScreen();
        break;
    }
}

void Login() {
    sUser User;
    string UserName="";
    int Password = 0;
    PrintNewScreenTitle("Login Screen");
    cout << "Enter UserName? ";
    getline(cin >> ws, UserName);
    cout << "Enter Password? ";
    cin >> Password;

    while (!IsValidateUser(UserName, Password)) {
        PrintNewScreenTitle("Login Screen");
        cout << "\nInvalid UserName or Password!\n\n";
        cout << "Enter UserName? ";
        getline(cin >> ws, UserName);
        cout << "Enter Password? ";
        cin >> Password;
    } 
    
    LoginUserName = UserName;

    MainScreen();
}

int main() {
    Login();
    return 0;
}
