#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;

#define FILENAME "ACCOUNTS.DAT"

void clearScreen();

enum TYPE
{
    SAVINGS = 0,
    CURRENT
};

class CAccount
{
private:

    int     acctNo;
    char    holderName[50];
    TYPE    type;
    int     amount;

    int     isDummy;

public:

//======================================================================//
    CAccount()
    {
        isDummy = 1;
    }

//======================================================================//
    CAccount(int _acctNo, char* _holderName, TYPE _type, int _initialAmt)
    {
        type        = SAVINGS;
        acctNo      = _acctNo;
        type        = _type;
        amount      = _initialAmt;
        isDummy     = 0;

        strcpy(holderName, _holderName);

        dump();
    }

//======================================================================//
    CAccount(CAccount &acct)
    {
        acctNo      = acct.acctNo;
        type        = acct.type;
        amount      = acct.amount;
        isDummy     = acct.isDummy;

        strcpy(holderName, acct.holderName);
    }

//======================================================================//
    void update(char* _holderName, TYPE _type, int _initialAmt)
    {
        type        = SAVINGS;
        type        = _type;
        amount      = _initialAmt;
        isDummy     = 0;

        strcpy(holderName, _holderName);

        updateInFile();
    }

//======================================================================//
    int is_Dummy()
    {
        return isDummy;
    }

//======================================================================//
    int getAcctNo()
    {
        return acctNo;
    }

//======================================================================//
    char* getType()
    {
        if(type == SAVINGS) return "Savings";
        else if(type = CURRENT) return "Current";
        else return "Unknown";
    }

//======================================================================//
    void withdraw(int amt)
    {
        if(amount < amt)
        {
            cout << "Insufficient account balance." << endl;
        }
        else
        {
            amount -= amt;

            updateInFile();
            showDetails();
            cin.get();
        }
    }

//======================================================================//
    void deposit(int amt)
    {
        amount += amt;

        updateInFile();
        showDetails();
        cin.get();
    }

//======================================================================//
    void showAsList()
    {
        cout << "  " << acctNo;
        cout << setw(20) << holderName;
        cout << setw(20) << getType();
        cout << setw(16) << amount << endl;
    }

//======================================================================//
    void showDetails()
    {
        cout << "========================================" << endl << endl;
        cout << "     ACCOUNT DETAILS" << endl << endl;
        cout << "   Account No: " << acctNo << endl;
        cout << "   Account Holder Name: " << holderName << endl;
        cout << "   Account Type: " << getType() << endl;
        cout << "   Amount: " << amount << endl << endl;
        cout << "========================================" << endl << endl;

    }

//======================================================================//
    void updateInFile()
    {
        int found = 0;
        CAccount account;
        fstream file(FILENAME, ios::binary | ios::out | ios::in);

        while(file.read((char*)&account, sizeof(CAccount)))
        {
            if(account.acctNo == acctNo)
            {
                int pos = (-1)*sizeof(CAccount);
                file.seekp(pos, ios::cur);
                file.write((char*)&(*this), sizeof(CAccount));
                found = 1;
                break;
            }
        }

        file.close();

        clearScreen();

        if(found)
        {
            cout << "Record updated successfully!" << endl << endl;
        }
        else
        {
            cout << "An error has occurred while updating the record.";
        }
    }

//======================================================================//
    void dump()
    {
        ofstream outfile(FILENAME, ios::binary | ios::app);
        outfile.write((char*)&(*this), sizeof(CAccount));
        outfile.close();
    }

//======================================================================//
    void _delete()
    {
        CAccount account;
        ifstream infile(FILENAME, ios::binary | ios::in);

        ofstream outfile("TEMP.DAT", ios::binary | ios::out);

        infile.seekg(0);
        while(infile.read((char*)&account, sizeof(CAccount)))
        {
            if(account.acctNo != acctNo)
            {
                outfile.write((char*)&account, sizeof(CAccount));
            }
        }

        infile.close();
        outfile.close();

        remove(FILENAME);
        rename("TEMP.DAT", FILENAME);

        cout << "Record with Account No: " << acctNo << " has been successfully deleted!" << endl << endl;
    }
};

//======================================================================//
void clearScreen()
{
    system("cls");
}

//======================================================================//
void showDetails()
{
    cout << "========================================" << endl << endl;
    cout << "     BANKING ADMINISTRATION PANEL" << endl << endl;
    cout << "========================================" << endl << endl;
    cout << "========================================" << endl << endl;
    cout << "   Developed by:      Luckshya Verma" << endl;
    cout << "   Exam. Roll No:     17657115" << endl;
    cout << "   Class:             XII - A" << endl << endl;
    cout << "========================================" << endl << endl;

    cin.get();
}
//=======================================================================//
int getAcctNo()
{
    int acctNo;

    cout << "Enter Account No: ";
    cin >> acctNo;

    return acctNo;
}

//=======================================================================//
void getAcctDetails()
{
    clearScreen();

    int     acctNo;
    char    holderName[50];
    char    s_type;
    int     initialAmt;
    TYPE    type;

    cout << "Enter Account No: ";
    cin >> acctNo;

    cout << "Enter Account holder Name: ";
    cin.ignore();
    cin.getline(holderName, 50);

    cout << "Enter Account Type: S - Savings, C - Current: ";
    cin >> s_type;

    if(s_type == 'S')
    {
        type = SAVINGS;
    }
    else if(s_type == 'C')
    {
        type = CURRENT;
    }

    cout << "Enter Initial Amount: ";
    cin >> initialAmt;

    CAccount account(acctNo, holderName, type, initialAmt);

    cout << endl << "Account created successfully with Account No: " << acctNo << endl << endl;

    cin.get();
}

//=======================================================================//
CAccount getAcct(int acctNo)
{
    int found = 0;
    CAccount account;

    ifstream file(FILENAME, ios::binary | ios::in);

    while(file.read((char*)&account, sizeof(CAccount)))
    {
        if(account.getAcctNo() == acctNo)
        {
            ::clearScreen();
            account.showDetails();
            return account;
        }
    }

    file.close();

    if(!found)
    {
        cout << "No Account found with Account No: " << acctNo << endl;
        CAccount acct;
        account = acct;
        cin.get();
    }

    return account;
}

//=======================================================================//
void showAcctList()
{
    clearScreen();

    int found = 0;
    CAccount account;

    ifstream file(FILENAME, ios::binary | ios::in);

    file.seekg(0);

    cout << "================================================================" << endl;
    cout << "  A/C No.          Name             Type             Balance" << endl;
    cout << "================================================================" << endl;
    while(file.read((char*)&account, sizeof(CAccount)))
    {
        found = 1;
        account.showAsList();
    }

    if(!found)
    {
        cout << "No Record found!";
    }

    file.close();
}

//=======================================================================//
int main()
{
    showDetails();

    int option;

    do
    {
        clearScreen();

		cout<<"\n\n\n\tMAIN MENU";
		cout<<"\n\n\t01. NEW ACCOUNT";
		cout<<"\n\n\t02. DEPOSIT AMOUNT";
		cout<<"\n\n\t03. WITHDRAW AMOUNT";
		cout<<"\n\n\t04. BALANCE ENQUIRY";
		cout<<"\n\n\t05. ALL ACCOUNT HOLDER LIST";
		cout<<"\n\n\t06. CLOSE AN ACCOUNT";
		cout<<"\n\n\t07. MODIFY AN ACCOUNT";
		cout<<"\n\n\t08. EXIT";
		cout<<"\n\n\tSelect Your Option (1-8) ";

        cin >> option;

        switch(option)
        {
        case 1:
            getAcctDetails();
            break;

        case 2:
        {
            clearScreen();

            CAccount account;
            account = getAcct(getAcctNo());

            if(!account.is_Dummy())
            {
                int amount;

                cout << "Enter Deposit Amount: ";
                cin >> amount;

                account.deposit(amount);
            }
        } break;

        case 3:
        {
            clearScreen();

            CAccount account;
            account = getAcct(getAcctNo());

            if(!account.is_Dummy())
            {
                int amount;

                cout << "Enter Withdraw Amount: ";
                cin >> amount;

                account.withdraw(amount);
            }
        } break;

        case 4:
        {
            clearScreen();

            CAccount account;
            account = getAcct(getAcctNo());

            if(!account.is_Dummy())
            {
                cin.get();
            }
        } break;

        case 5:
            showAcctList();
            cin.get();
            break;

        case 6:
        {
            clearScreen();

            CAccount account;
            account = getAcct(getAcctNo());

            if(!account.is_Dummy())
            {
                account._delete();
                cin.get();
            }
        } break;

        case 7:
        {
            clearScreen();

            CAccount account;
            account = getAcct(getAcctNo());

            if(!account.is_Dummy())
            {
                char    holderName[50];
                char    s_type;
                int     initialAmt;
                TYPE    type;

                cout << "Modify Account holder Name: ";
                cin.ignore();
                cin.getline(holderName, 50);

                cout << "Modify Account Type: S - Savings, C - Current: ";
                cin >> s_type;

                if(s_type == 'S')
                {
                    type = SAVINGS;
                }
                else if(s_type == 'C')
                {
                    type = CURRENT;
                }

                cout << "Modify Initial Amount: ";
                cin >> initialAmt;

                account.update(holderName, type, initialAmt);
                account.showDetails();
                cin.get();
            }
        } break;

        case 8:
            return 0;
            break;

        default :
            cout << "Invalid option!";
            break;
        }

        cin.get();

    } while(option != 8);

    return 0;
}
