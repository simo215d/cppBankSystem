#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class account{
    private:
    int code;
    int money;
    public:
    account(){}
    account(int c, int m){
        code=c;
        money=m;
        cout << "\nA new account was instantiated with code: "<<code<<" and money: "<<money<<"!\n";
    }
    void setMoney(int m){
        money=m;
    }
    int getMoney(){
        return money;
    }
    int getCode(){
        return code;
    }
};

vector<account> accountsv;
int currentUser;

void loadAccounts(){
    //read data.txt for accounts
    string database;
    string line;
    ifstream myfile ("data.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            database += line;
        }
        cout << database;
        myfile.close();
    } else cout << "Unable to open file\n"; 
    //scan database for accounts
    int totalAccounts = 0;
    for (int i = 0; i < database.length(); i++)
    {
        if(database.at(i)=='A'){
            //koden er altid 4 lang, så vi kan bruge substring let nok
            int code = stoi(database.substr(i+3,i+4));
            //money kan være på en ubestemt længde, så vi loope indtil vi finder et underscore_
            string moneyStr;
            for (int j = i+8; j < database.length(); j++)
            {
                if(database.at(j)=='_'){
                    break;
                }
                moneyStr+=database.at(j);
            }
            int money = stoi(moneyStr);
            account acc(code, money);
            //add to vector
            accountsv.push_back(acc);
        }
    }
}

int getCurrentUser(){
    for (int i = 0; i < accountsv.size(); i++)
    {
        if(currentUser==accountsv.at(i).getCode()){
            return i;
        }
    }
    return -1;
}

void withdraw(){
    cout << "You now have "<<accountsv.at(getCurrentUser()).getMoney()<<" dollars\n";
    cout << "enter money to withdraw\n";
    int money;
    cin >> money;
    //find the currentUsers account and withdraw money
    accountsv.at(getCurrentUser()).setMoney(accountsv.at(getCurrentUser()).getMoney()-money);
    cout << "Money has been withdrawn. you now have "<<accountsv.at(getCurrentUser()).getMoney()<<" dollars\n";
}

void deposit(){
    cout << "You now have "<<accountsv.at(getCurrentUser()).getMoney()<<" dollars\n";
    cout << "enter money to deposit\n";
    int money;
    cin >> money;
    //find the currentUsers account and deposit money
    accountsv.at(getCurrentUser()).setMoney(accountsv.at(getCurrentUser()).getMoney()+money);
    cout << "Money has been deposited. you now have "<<accountsv.at(getCurrentUser()).getMoney()<<" dollars\n";
}

void beginsession(int code){
    currentUser = code;
    while(true){
        cout<<"1. Withdraw\n2. Deposit\n3. Exit\n";
        int choice;
        cin >> choice;
        switch (choice)
        {
            case 1: withdraw(); break;
            case 2: deposit(); break;
            case 3: return; break;
        }
    }
}

void login(){
    cout <<"type in ur card code\n";
    int code;
    cin >> code;
    bool wasCorrect = false;
    for (int i = 0; i < accountsv.size(); i++)
    {
        if(code==accountsv.at(i).getCode()){
            cout<<"\nThe code was correct!\n";
            beginsession(code);
            wasCorrect=true;
            break;
        }
    }
    if(!wasCorrect){
        cout<<"INCORRECT\n";
        login();
    }
}

void save(){
    string newData;
    for (int i = 0; i < accountsv.size(); i++)
    {
        int currentUserIndex = i+1;
        int code = accountsv.at(i).getCode();
        int money = accountsv.at(i).getMoney();
        ostringstream oss;
        oss <<"_A"<<currentUserIndex<<"_"<<code<<"_"<<money; 
        newData+=oss.str();
    }
    cout << "trying to save dis: "<<newData;
    ofstream myfile;
    myfile.open ("data.txt");
    myfile << newData;
    myfile.close();
}

int main()
{
    loadAccounts();
    cout <<"-banking system-\n1. Login\n2. Exit\n";
    int selected;
    cin >>selected;
    switch (selected)
    {
        case 1: login(); break;
        case 2: return 0;
    }
    //SAVE - skriv til data.txt
    save();
}