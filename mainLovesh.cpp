#include <iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
#include <stdlib.h>
using namespace std;

#define FILENAME "Bank Records.dat"

//===============================================
//FUNCTIONS
void Intro(); //Introduction function for various Details.
void Menu(); //To display all possible operations.
void Get_Account(); //Get Account details from records file using account number.
void Open_Account(); //To get details from the user to open a new account.
void Deposit_Amt(int); //To Deposit Money into Account.
void Display_BE(int); //To Display Balance Enquiry.
void Withdraw_Amt(int); //To Withdraw Money from Account.
void Display_All(int); //To Display all Account Holders and their Details.
void Delete_Acc(int); //To Close Bank Account.
void Edit_Acc(int); //To Modify Bank Account Details.
//===============================================


//===============================================
//CLASSES
class Account
{
    int AccNo;
    char AccHolderName[80];
    int Balance;
    char AccType[20];

public:
    void Create_Account(); //Storing Account Details to Bank Records File.
    int ReturnAccNo();//Return Account Number.
    void Deposit(int); //Deposit Money.
    void Show_AccDetails(); //Show Account Details for Balance Enquiry.
    void Withdraw(int); //Withdraw Money.
    void ShowLists(); //Show all Account Holder and their details.
    void EditAccDetails(); //Input New Account details to Modify.
    int RetBalance(); // To return Balance.
};

int Account::RetBalance()
{
    return Balance;
}

int Account::ReturnAccNo()
{
    return AccNo;
}

void Account::EditAccDetails()
{
    cout<<"\nAccount No. : "<<AccNo;
	cout<<"\nNew Account Holder Name : ";
	cin.ignore();
	cin.getline(AccHolderName,80);
	cout<<"\nNew Type of Account : ";
	cin>>AccType;
	cout<<"\nNew Balance Amount : ";
	cin>>Balance;
}

void Account::ShowLists()
{
    cout<<"   "<<AccNo;
    cout<< setw(16) <<AccHolderName;
    cout<< setw(15) <<AccType;
    cout<< setw(14) <<Balance;
    cout<<endl;
    cin.get();
}

void Account::Show_AccDetails()
{
	cout<<"\nAccount No. : "<<AccNo;
	cout<<"\nAccount Holder Name : "<<AccHolderName;
	cout<<"\nAccount Type : "<<AccType;
	cout<<"\nBalance Amount : "<<Balance;
	cin.get();
}

void Account::Deposit(int x)
{
    Balance+=x;
}
void Account::Withdraw(int x)
{
    Balance-=x;
}

void Account::Create_Account()
{
    cout<<"\nEnter Account No. : ";
	cin>>AccNo;
	cout<<"\n\nEnter Account Holder Name : ";
	cin.ignore();
	cin.getline(AccHolderName,80);
	cout<<"\n\nEnter Account Type (Current or Savings): ";
	cin>>AccType;
	cout<<"\n\nEnter The Initial Deposit amount(>=1000 for Saving and >=2000 for current ) : ";
	cin>>Balance;
	cout<<"\n\n\nYour Account has been created successfully with Account No: "<<AccNo<<". Thanks for using our banking system."<<endl;
	cin.get();
}


//===============================================


int main()
{
    Intro();
    Menu();

}

void Edit_Acc(int GAccNo)
{
    Account Acc;
    bool RecordFound = false;
    fstream AccFile;
    AccFile.open(FILENAME, ios::binary|ios::in|ios::out);
    if(!AccFile)
    {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        cin.get();
        return;
    }
    while((AccFile.read((char*)&Acc, sizeof(Acc))) && RecordFound==false)
    {
        if(Acc.ReturnAccNo()==GAccNo)
        {
            Acc.Show_AccDetails();
            cout<<"\n\nEnter New Details of Account"<<endl;
            Acc.EditAccDetails();
            int pos=(-1)*sizeof(Acc);
            AccFile.seekp(pos,ios::cur);
            AccFile.write((char*)&Acc, sizeof(Acc));
            cout<<"\n\nDone!. Your details have been corrected. Please check them using Balance Enquiry.";
            cin.get();
            RecordFound = true;
            break;
        }
    }
    AccFile.close();
    if(RecordFound==false)
        {
		cout<<"\n\nSorry! Account doesnt exist. Contact Admin.";
		cin.get();
		}
}

void Delete_Acc(int GAccNo)
{
    Account Acc;
    fstream AccFile(FILENAME, ios::binary|ios::in);
    fstream TempFile("Temp.dat", ios::binary|ios::out);
    if(!AccFile)
    {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        cin.get();
        return;
    }
    AccFile.seekg(0,ios::beg);
    while(AccFile.read((char*)&Acc, sizeof(Acc)))
    {
        if(Acc.ReturnAccNo()!=GAccNo)
        {
        TempFile.write((char*)&Acc, sizeof(Acc));
        }
    }
    AccFile.close();
    TempFile.close();
    remove(FILENAME);
	rename("Temp.dat",FILENAME);
	cout<<"\n\n\tSorry to hear you are closing your account. Your Account has been deleted. Thank you for using our services."<<endl;
	cin.get();
}

void Display_All()
{
    Account Acc;
    fstream AccFile;
    AccFile.open(FILENAME, ios::binary|ios::in|ios::out);
    if(!AccFile)
    {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        cin.get();
        return;
    }

    cout<<"\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout<<"====================================================\n";
	cout<<"A/C No.        NAME          Type          Balance\n";
	cout<<"====================================================\n";
	while(AccFile.read((char*)&Acc, sizeof(Acc)))
    {
        Acc.ShowLists();
    }
    AccFile.close();
    cin.get();
}

void Display_BE(int GAccNo)
{
        Account Acc;
        bool RecordFound = false;
        fstream AccFile;
        AccFile.open(FILENAME, ios::binary|ios::in|ios::out);
        if(!AccFile)
        {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        cin.get();
        return;
        }

        cout<<"\nBALANCE DETAILS\n";

        while(AccFile.read((char*)&Acc, sizeof(Acc)) && RecordFound == false)
        {
            if(Acc.ReturnAccNo() == GAccNo)
            {
                Acc.Show_AccDetails();
                RecordFound=true;
            }
        }
        AccFile.close();
        if(RecordFound==false) {
		cout<<"\n\nSorry! Account doesnt exist. Contact Admin.";
		cin.get();
        }


}

void Open_Account()
{
    Account Acc;
    ofstream Accfile;
    Accfile.open(FILENAME, ios::binary|ios::app);
    Acc.Create_Account();
    Accfile.write((char*)&Acc, sizeof(Acc));
    Accfile.close();
}

void Withdraw_Amt(int GAccNo)
{
    int Amt;
    bool RecordFound = false;
    Account Acc;
    fstream AccFile;
    AccFile.open(FILENAME, ios::binary|ios::in|ios::out);
    if(!AccFile)
    {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        return;
    }
    while(AccFile.read((char*)&Acc, sizeof(Acc)))
    {
        if(Acc.ReturnAccNo()==GAccNo)
        {
            Acc.Show_AccDetails();
            cout<<"\n\n\SECTION: WITHDRAW AMOUNT ";
            cout<<"\n\nEnter The amount to withdraw: ";
            cin>>Amt;
                if(Amt<=Acc.RetBalance())
                {
                    Acc.Withdraw(Amt);
                    int pos=(-1)*sizeof(Acc);
                    AccFile.seekp(pos, ios::cur);
                    AccFile.write((char*)&Acc, sizeof(Acc));
                    cout<<"\n\nDone!. Amount Withdrawn. Please check your balance using Balance Enquiry.";
                    cin.get();
                }
                else
                {
                    cout<<"Insufficient Balance"<<endl;
                    cin.get();
                }
                RecordFound=true;
        }

    }
    AccFile.close();
    if (RecordFound == false) cout<<"\n\n Sorry! No Record Found. Contact Admin. "<<endl;

}

void Deposit_Amt(int GAccNo)
{
    int Amt;
    bool RecordFound = false;
    Account Acc;
    fstream AccFile;
    AccFile.open(FILENAME, ios::binary|ios::in|ios::out);
    if(!AccFile)
    {
        cout<<"ERROR: 404BMS File not found, Contact Administrator. Press any key to continue."<<endl;
        return;
    }

    while((AccFile.read((char*)&Acc, sizeof(Acc))) && RecordFound==false)
	{
            if(Acc.ReturnAccNo() == GAccNo)
            {
                Acc.Show_AccDetails();
                cout<<"\n\n\SECTION: DEPOSIT AMOUNT ";
				cout<<"\n\nEnter The amount to be deposited: ";
				cin>>Amt;
				Acc.Deposit(Amt);
            }
            int pos=(-1)*sizeof(Acc);
			AccFile.seekp(pos,ios::cur);
			AccFile.write((char*)&Acc, sizeof(Acc));
			cout<<"\n\nDone!. Amount Withdrawn. Please check your balance using Balance Enquiry.";
			cin.get();
			RecordFound=true;
    }
    AccFile.close();
    if (RecordFound == false) cout<<"\n\nSorry! No Record Found. Contact Admin"<<endl;

}

void Intro()
{
    cout<<"\n==========================================";
    cout<<"\n\n\n\t\t   BANKING";
    cout<<"\n\n\t\tADMINISTRATION";
    cout<<"\n\n\t\t    PANEL";
    cout<<"\n\n\n==========================================\n"<<endl;
    cout<<" SUBMITTED BY: LOVESH VERMA"<<endl;
    cout<<" SUBMITTED TO: MAM PREETI ANEJA"<<endl;
    cout<<" ROLL NUMBER: 17657113"<<endl;
    cout<<" CLASS: XII - A "<<endl;
    cout<<" SCHOOL: ST. THERESA'S CONVENT SCHOOL"<<endl;
    cout<<"\n\nPress any key to continue"<<endl;
    cin.get();
}

void Menu()
{
    int option;
    int EAccNo;

    do {

      system("cls");
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
		cin>>option;
      system("cls");

      switch(option)
      {
      case 1:
          Open_Account();
          break;
      case 2:
          cout<<"\n\n\tEnter Account No. : "; cin>>EAccNo;
          Deposit_Amt(EAccNo);
          break;
      case 3:
          cout<<"\n\n\tEnter Account No. : "; cin>>EAccNo;
          Withdraw_Amt(EAccNo);
          break;
      case 4:
          cout<<"\n\n\tEnter Account No. : "; cin>>EAccNo;
          Display_BE(EAccNo);
          break;
      case 5:
          Display_All();
          break;
      case 6:
          cout<<"\n\n\tEnter Account No. : "; cin>>EAccNo;
			Delete_Acc(EAccNo);
          break;
      case 7:
         cout<<"\n\n\tEnter Account No. : "; cin>>EAccNo;
			Edit_Acc(EAccNo);
          break;
      case 8:
          cout<<"Exiting! Thanks for using bank management system"<<endl;
          break;
      default: cout<<"Select Correct Option!!"<<endl;
      }
      cin.get();
    } while(option!=8) ;


}


