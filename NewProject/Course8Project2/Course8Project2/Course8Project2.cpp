#include <iostream>

#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

const string ClientDataBase = "Clients.txt";

void Login();
void TransactionsMenue();
bool AreUSure();
void PrintNewScreenTitle(string title);

enum  enTransactionsButtons {
    QucikWithdraw = 1, NormalWithdraw, Deposit ,CheckBalance, Logout
};

enum QuickWithdrawoptions {
    twenty=1,fifty,onehundred,twohundreds,fourhundreds,sixhundreds,eighthundreds,onethousand,logout
};

struct sClient {
    string AccountNumber, PinCode, Name, Phone;
    double AccountBalance;
};

sClient CurrentClient;

double ReadWithdrawAmount() {
    double num;
    cout << "Enter an amount multiple of 5s ? ";
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

string Record2Line(const sClient& Client, string Separator = "#//#") {
    return Client.AccountNumber + Separator + Client.PinCode + Separator + Client.Name + Separator + Client.Phone + Separator + to_string(Client.AccountBalance);
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

void SaveClientsToFile(const vector<sClient>& vClients) {
    ofstream myfile(ClientDataBase);
    if (myfile.is_open()) {
        for (const sClient& Client : vClients) {
            myfile << Record2Line(Client) << endl;
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

void PrintClientData(const sClient& Client) {
    cout << "\nThe following are client details:\n\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PinCode        : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

bool IsExist(sClient& foundClient, const string& input) {
    vector<sClient> vClients = ImportFromFile(ClientDataBase);
    for (const sClient& Client : vClients) {
        if (Client.AccountNumber == input) {
            foundClient = Client;
            return true;
        }
    }
    return false;
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

void PrintNewScreenTitle(string title) {
    system("cls");
    cout << "============================================\n";
    cout << "\t\t" << title << "\n";
    cout << "============================================\n";
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    TransactionsMenue();
}

void DepositScreen() {
    system("cls");
    PrintNewScreenTitle("Deposit Screen");
    vector<sClient> clients = ImportFromFile(ClientDataBase);
    double deposit;
    cout << "\nplease enter a positive Deposit ? ";
    cin>>deposit;
    
    char x;

    cout << "\n\nAre you sure you want to perform this transaction ? y/n ? ";
    cin >> x;

    if (toupper(x) == 'Y') {
        CurrentClient.AccountBalance += deposit;
        cout << "\n\nDone Successfull ,New balance is : " << CurrentClient.AccountBalance;
    }
    SaveClientsToFile(clients);
}

void performwithdraw(int amount) {
    if (amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount exceeds the balance , make another choice .\n";
        cout << "perss any key to continue...";
        system("pause");
    }
    else if (AreUSure()) {
        CurrentClient.AccountBalance -= amount;
        cout << "\nDone Successfully. New Balance is " << CurrentClient.AccountBalance << endl;
    }
}

bool AreUSure() {
    char Answer;
    cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        return true;
    else return false;
}

void QuickWithdrawScreen() {
    system("cls");
    short input = 0;
    PrintNewScreenTitle("Withdraw Screen");
    vector<sClient> clients = ImportFromFile(ClientDataBase);
    cout << "\t[1] 20\t [2] 50\n";
    cout << "\t[3] 100\t[4] 200\n";
    cout << "\t[5] 400\t[6] 600\n";
    cout << "\t[7] 800\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "============================================\n";
    cout << "Your balance is " << CurrentClient.AccountBalance;
    cout << "\nChoose what to withdraw from [1] to [8] ? ";
    cin >> input;
    
    switch (input)
    {
    case QuickWithdrawoptions::twenty:
        performwithdraw(20);
        GoBackToTransactionsMenue();
        break;
    
    case QuickWithdrawoptions::fifty:
        performwithdraw(50);
        GoBackToTransactionsMenue();
        break;
        
    case QuickWithdrawoptions::onehundred:
            performwithdraw(100);
        GoBackToTransactionsMenue();
        break;
        
        case QuickWithdrawoptions::twohundreds:
            performwithdraw(200);
        GoBackToTransactionsMenue();
        break;
        
        case QuickWithdrawoptions::fourhundreds:
            performwithdraw(400);
        GoBackToTransactionsMenue();
        break;
        
        case QuickWithdrawoptions::sixhundreds:
            performwithdraw(600);
        GoBackToTransactionsMenue();
        break;
        
        case QuickWithdrawoptions::eighthundreds:
            performwithdraw(800);
        GoBackToTransactionsMenue();
        break;

        case QuickWithdrawoptions::onethousand:
            performwithdraw(1000);
        GoBackToTransactionsMenue();
        break;

    case QuickWithdrawoptions::logout:
        Login();
        break;
    }
}

//„⁄ ‘ €Ì— œÌ
void WithdrawScreen() {
    system("cls");
    PrintNewScreenTitle("Withdraw Screen");
    vector<sClient> clients = ImportFromFile(ClientDataBase);
    int withdraw=0;
    do
    {
        
        if (withdraw > CurrentClient.AccountBalance) {
            system("cls");
            PrintNewScreenTitle("Withdraw Screen");
        }
        withdraw = ReadWithdrawAmount();
        cout << "\n\n";

        if (withdraw % 5 != 0 || withdraw > CurrentClient.AccountBalance) {
            continue;
        }
        performwithdraw(withdraw);
        break;
    } while (true);

    SaveClientsToFile(clients);
}

void CheckBalanceScreen() {
    system("cls");
    PrintNewScreenTitle("Check Balance Screen");
    cout << "Your balance is " << CurrentClient.AccountBalance;
    cout << "\n\n\n";
}

void TransactionsMenue() {
    int input = 0;

    PrintNewScreenTitle("ATM Main Menue Screen");
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "============================================\n";
    cout << "Choose what do you want to do? [1 to 5]? ";
    cin >> input;

    switch (input)
    {
    case enTransactionsButtons::QucikWithdraw:
        QuickWithdrawScreen ();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::NormalWithdraw:
        WithdrawScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::Deposit:
        DepositScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::CheckBalance:
        CheckBalanceScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsButtons::Logout:
        Login();
        break;
    }
}

bool FindUserByUsernameAndPassword(string accountnumber, string pincode,sClient currentclient) {
    vector <sClient> vclients = ImportFromFile(ClientDataBase);
    for (sClient client : vclients)
    {
        if (client.AccountNumber == accountnumber && client.PinCode== pincode)
        {
            currentclient = client;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string accountnumber, string password) {
    if (FindUserByUsernameAndPassword(accountnumber, password,CurrentClient))
        return true;
    else
        return false;
}

void Login() {
    bool LoginFailed = false;

    string accountnumber, pincode;

    do
    {
        PrintNewScreenTitle("Login Screen");
        if (LoginFailed)
            cout << "Invalid account number/pincode!\n";

        cout << "Enter Username? ";
        cin >> accountnumber;
        cout << "Enter Password? ";
        cin >> pincode;

        LoginFailed=!LoadUserInfo(accountnumber, pincode);

    } while (LoginFailed);

    TransactionsMenue();
}

int main() {
    Login();
    return 0;
}

