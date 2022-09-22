#include<iostream>
#include<cstdio>
#include<queue>
#include<fstream>
#include<stack>
#include<list>
#include<windows.h>
#include<fstream>
#include<string.h>
#define goToMainMenu getchar();cout << "\nPRESS ENTER to go into the main menu...";getchar();system("CLS")
#define serveNext getchar();cout << "PRESS ENTER to serve next customer\n"; getchar();system("CLS")
#define searching cout << "Searching"; Sleep(700); cout << "."; Sleep(700); cout << "."; Sleep(700); cout << "."; Sleep(700); cout << ".\nSearch finished\n\n"
using namespace std;

//Structure having three members (Medicine's names, price & amount)
typedef struct medicine{
    string name;
    int price;
    int amount;
}medicine;

//Declaring list which will store all data from file
list<medicine> store;
list<medicine>::iterator it;

//File to save the earnings list from individuals
FILE * fp;

/*FUNCTIONS*/
void storeDataFromFile(); // Store data from file
void servingCustomer();  //  Serving customers by maintaining queue
void storeEarnings();    //  Check store earnings
void searchMedicine();   //  Search for specific medicine in the store
void checkStock();       //  Check medicine stock

int earnings = 0; //We will keep total income of the store in this variable

int main(){
    cout << "\t\t\tMEDICAL STORE\n\n";
    cout << "\t\t";
  //  cout << "\t\t  **********************";

    fp = fopen ("earningsList.txt", "w");
    
    fprintf(fp, "Customer\tItem\t  Quantity\tPrice\n");
    fprintf(fp, "--------\t----\t  --------\t------\n");
    fclose(fp);

    storeDataFromFile();

    int num;
    while(1){
       // cout << "   ------------------------------------------------------------\n";
        cout << "   | Press,  1 for searching a specific medicine in the store |\n";
        cout << "   |         2 for checking medicine stock                    |\n";
        cout << "   |         3 to show medical store total earnings           |\n";
        cout << "   |         4 for serving people in the queue                |\n";
        cout << "   |         5 to EXIT the program                            |\n";
      //  cout << "   ------------------------------------------------------------\n\n";

        cout << "Enter your choice : ";
        cin >> num;
        putchar('\n');

        if(num == 1) searchMedicine();
        else if(num == 2) checkStock();
        else if(num == 3) storeEarnings();
        else if(num == 4) servingCustomer();
        else if(num == 5) return 0;
        else{
            cout << "WRONG CHOICE!!\n";
            Sleep(2500);
            system("CLS");
        }
    }

    return 0;
}

void storeDataFromFile(){
    medicine temp;

    string s;
    int pri, am;

    //Variable declaration to input from files
    ifstream name;
    ifstream price;
    ifstream amount;

    //Opening file
    name.open("name.dat");
    price.open("price.dat");
    amount.open("amount.txt");

    //Taking data from file and store it into list (Linked list)
    while(name >> s){
        price >> pri;
        amount >> am;
        temp.name = s, temp.price = pri, temp.amount = am;
        store.push_back(temp);
    }
}

void searchMedicine(){
    bool flaG = false;
    string namE;
    cout << "Enter the name of your desired medicine : ";
    cin >> namE;

    //Check the availability of desired medicine
    for(it=store.begin(); it!=store.end(); it++){
        if(it->name == namE){
            cout << namE << " is available in the store.\n";
            flaG = true;
            break;
        }
    }
    if(flaG == false)  cout << "Sorry, " << namE << " is not available in the store.\n";

    goToMainMenu;
}

void checkStock(){
    cout << "Medicine left in the store : \n\n";

    cout << "NAME\t\t\tSTOCK(unit of medicine)\n";
    cout << "----\t\t\t-----------------------\n";
    //Show stock status
    for(it=store.begin(); it!=store.end(); it++){
        cout << it->name << "\t\t\t" << it->amount << endl;
    }
    goToMainMenu;
}

void storeEarnings(){
    if(earnings > 0) cout << "After selling medicine(s), total earnings of the store is : " << earnings << " taka.\n";
    else cout << "No medicine was sold yet.\n";

    goToMainMenu;
}

void servingCustomer(){
    //Declaring stack & queue
    stack<string> medNameList;
    queue<string> line;

    int N;
    cout << "How many customers in the queue : ";
    cin >> N;

    string customerName;
    cout << "Please, enter the name of the " << N << " customers : ";
    //Enqueue the name(s) of the customers
    for(int i=1; i<= N; i++){
        cin >> customerName;
        line.push(customerName);
    }
    putchar('\n');

    while(!line.empty()){
        cout << "Serving " << line.front() << "\n---------------\n";
        int medNum;
        string medName;

        cout << "How many medicine : ";
        cin >> medNum;

        cout << "ENTER the name of medicine(s) in CAPITAL form : ";
        for(int i=0; i<medNum; i++){
            cin >> medName;
            //Push the requested medicine(s) name into stack
            medNameList.push(medName);
        }

        putchar('\n');
        searching;

        while(!medNameList.empty()){
            bool flag = false;
            for(it=store.begin(); it!=store.end(); it++){
                if(medNameList.top() == it->name){
                    int unit;
                    cout << it->name << " is FOUND in the store\nHow many unit of " << it->name << " needed? : ";
                    cin >> unit;
                    it->amount -= unit;

                    //Checking if the medicine is IN STOCK or, OUT oF STOCK
                    if(it->amount >= 0){
                            fp = fopen ("earningsList.txt", "a"); //Opening file for storing earnings
                            fprintf(fp, "%s\t\t%s\t    %d\t\t %d\n",line.front().c_str(), it->name.c_str(), unit, unit*(it->price));
                            fclose(fp);

                            cout << it->name << " is in stock & served.\n";
                            earnings += unit * it->price;
                    }
                    else{
                        cout << "Sorry, " << it->name << " is out of stock.\n";
                        it->amount += unit;
                    }

                    flag = true;
                    break;
                }
            }
            if(flag == false) cout << medNameList.top() << " is NOT FOUND in the store.\n";

            //After serving, pop the medicine from stack
            medNameList.pop();
        }

        cout << "\nTHANK YOU " << line.front() << ", COME AGAIN.\n";

        if(line.front() == line.back()) {goToMainMenu;break;}
        else serveNext;

        //After serving n-th customer, dequeue that customer from queue
        line.pop();
    }

    //update total incomes in file
    fp = fopen ("EarningsList.txt", "a");
    fprintf(fp, "\nTotal earnings after serving that customer(s) : %d tk\n\n", earnings);
    fclose(fp);
}
