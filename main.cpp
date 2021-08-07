#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <sstream>
using namespace std;
vector<string> ls(const char *path) {
   struct dirent *entry;
   vector<string> files;
   DIR *dir = opendir(path);
   
   if (dir == NULL) {
      throw invalid_argument("Fail!");
   }
   while ((entry = readdir(dir)) != NULL) {
     files.push_back(entry->d_name);
   }
   closedir(dir);
   return files;
}
void sort(vector<int>& v){
     /*Bubble sort algorithm*/
     for (int i = 0; i < v.size(); i++){
          for (int j = 1; j < v.size() - i; j++){
               if(v[j - 1] > v[j]){
                    int n = v[j - 1];
                    v[j - 1] = v[j];
                    v[j] = n;
               }
          }
     }
}
void print_vector(vector<int>& v){
     for (int i = 0; i < v.size(); i++){
          cout << v[i] << endl;
     }
}

vector<int> clients(vector<string>& fils){
     vector<int> n_clients;
     for (int i = 0; i < fils.size(); i++){
          string n_client = "";
          for (int j = 0; j < fils[i].size(); j++){
               if(fils[i][j] == '.') break;
               n_client += fils[i][j];
          }
          try{
               n_clients.push_back(stoi(n_client));
          }
          catch (exception){}
     }
     return n_clients;
}

float round_dec(float value, unsigned char dec_places)
{
  float pow_10 = pow(10.0f, (float)dec_places);
  return round(value * pow_10) / pow_10;
}
bool name_in_clients(vector<int> c, string name){
     int cond = 0;
     for (int i = 0; i < c.size(); i++){
          ifstream f;
          char file[c[i] + 4];
          string filename = to_string(c[i]) + ".txt";
          f.open(filename);
          string client_name;
          getline(f, client_name);
          string letters = "";
          for (int j = 0; j < client_name.size(); j++){
               if(client_name[j] == ':'){
                    letters = "";
               }
               letters += client_name[j];
          }
          letters.replace(letters.find(":"), 2, "");
          if(!name.compare(letters)) cond++;

     }
     if(cond > 0)
          return true;
     return false;
}
string client_file(vector<int> c, string name){
     string filename;
     for (int i = 0; i < c.size(); i++){
          ifstream f;
          char file[c[i] + 4];
          string filename = to_string(c[i]) + ".txt";
          f.open(filename);
          string client_name;
          getline(f, client_name);
          string letters = "";
          for (int j = 0; j < client_name.size(); j++){
               if(client_name[j] == ':'){
                    letters = "";
               }
               letters += client_name[j];
          }
          letters.replace(letters.find(":"), 2, "");
          if(name == letters){
               return to_string(c[i]) + ".txt";
          }
     }
     return "";
}
class Client{
     public:
          Client(string name = "", float amount = 0){ //Constructor
               user = name;
               capital = round_dec(amount, 2);
          }
          string get_user(){
               return user;
          }
          float get_capital(){
               return capital;
          }
          vector<float> get_records(){
               return records;
          }
          void deposit_capital(float new_capital){
               capital += new_capital;
               records.push_back(new_capital);
          }
          void withdraw_capital(float new_capital){
               if(new_capital <= capital){
                    capital -= new_capital;
                    records.push_back(-new_capital);
               }
               else
                    cout << "The capital isn't enough to withdraw!" << endl;
          }
          void change_name(string new_name){
               user = new_name;
          }
     private:
          string user;
          float capital;
          vector<float> records;
};

int search_client_index(vector<Client> v_clients, string name){
     Client c;
     for (int i = 0; i < v_clients.size(); i++){
          if(v_clients[i].get_user() == name)
               return i;
     }
     return 0;
}

class Bank{
     public:
          void new_account(Client * client, vector<string>& files, int order){
               ofstream file;
               
               file.open(to_string(order + 1) + ".txt", ios::app);
               file << "Client: " + client->get_user();
               file << endl;

               if(file.is_open())
                    file.close();
               
               
          }
          void update_client(Client * client,vector<string> files, int order){
               auto filename = to_string(order + 1) + ".txt";
               ofstream file(filename);
               auto records = client->get_records();
               file << "Client: " + client->get_user() << endl;
               for (int i = 0; i < client->get_records().size(); i++){
                    stringstream s_str;
                    s_str << records[i];
                    file << "$: " + s_str.str() << endl;
               }
               stringstream capital;
               capital << client->get_capital();
               file << "TOTAL{$}:" + capital.str() << endl;
               if(file.is_open())
                    file.close();
          }
          void show_account(Client * client){
               auto records = client->get_records();
               for (int i = 0; i < records.size(); i++){
                    cout <<to_string(i + 1) + ".    $:" << records[i] << endl;
               }
               cout << "The total capital is " << client->get_capital() << "$";
          }
          void close_account(vector<string>& files, string name){
               auto filename = client_file(clients(files), name);
               char file[filename.size()];
               strcpy(file, filename.c_str());
               int status = remove(file);
          }
          void all_accounts(vector<string> files){
               auto c = clients(files);
               for (int i = 0; i < c.size(); i++){
                    ifstream f;
                    char file[c[i] + 4];
                    string filename = to_string(c[i]) + ".txt";
                    f.open(filename);
                    string client_name;
                    getline(f, client_name);
                    string name = "";
                    for (int j = 0; j < client_name.size(); j++){
                         if(client_name[j] == ':'){
                              name = "";
                         }
                         name += client_name[j];
                    }
                    name.replace(name.find(":"), 2, "");
                    cout << to_string(i + 1) + ". " <<  name << endl;
               }
          }
          void clear_clients(vector<string>& files){
               auto n_clients = clients(files);
               for (int i = 0; i < n_clients.size(); i++){
                    char file[n_clients[i] + 4];
                    string filename = to_string(n_clients[i]) + ".txt";
                    strcpy(file, filename.c_str());
                    int status = remove(file);
               }
          }
};


int main(){
     Bank bank;
     vector<string> options;
     vector<Client> v_clients;

     options.push_back("NEW ACCOUNT");
     options.push_back("DEPOSIT AMOUNT");
     options.push_back("WITHDRAW AMOUNT"); //Take out the money
     options.push_back("BALANCE ENQUIRY"); //Look the total quantity of money
     options.push_back("ALL ACCOUNT HOLDER LIST");
     options.push_back("CLOSE AN ACCOUNT");
     options.push_back("MODIFY AN ACCOUNT");
     options.push_back("EXIT");
     cout << "MAIN MENU" << endl;
     for (int i = 0; i < options.size(); i++){
          cout << "0" + to_string(i + 1) + ". " + options[i] << endl;
     }
     unsigned option;
     auto files = ls(".\\");
     bank.clear_clients(files);
     do{
          cout << "Select Your Option <1-8> ";
          cin >> option;
          
          
          auto files = ls(".\\");
          cout << "The user has chose: " + options[option - 1] << endl;
          if(option == 1){
               string name;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)){
                    Client c(name);
                    
                    bank.new_account(&c,files, v_clients.size());
                    v_clients.push_back(c);
               }
               else
                    cout << "The client's name exists, choose another one!" << endl;
          }
          else if(option == 2){
               string name;
               float amount;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)) cout << "The client doesn't exist, try again!" << endl;
               else{
                    cout << "How much cash's amount do you want to deposit? ";
                    cin >> amount;
                    auto index = search_client_index(v_clients, name);
                    v_clients[index].deposit_capital(amount);
                    Client c;
                    c = v_clients[index];
                    bank.update_client(&c, files, index);
                    
               }
          }
          else if(option == 3){
               string name;
               float amount;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)) cout << "The client doesn't exist, try again!" << endl;
               else{
                    cout << "How much cash's amount do you want to withdraw? ";
                    cin >> amount;
                    
                    auto index = search_client_index(v_clients, name);
                    v_clients[index].withdraw_capital(amount);
                    Client c;
                    c = v_clients[index];
                    bank.update_client(&c, files, index);
               }
          }
          else if(option == 4){
               string name;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)) cout << "The client doesn't exist, try again!" << endl;
               else{
                    cout << "The balance enquiry is..." << endl;
                    
                    auto index = search_client_index(v_clients, name);
                    Client c;
                    c = v_clients[index];
                    bank.show_account(&c);
               }
          }
          else if(option == 5){
               bank.all_accounts(files);
          }
          else if(option == 6){
               string name;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)) cout << "The client doesn't exist, try again!" << endl;
               else{                    
                    auto index = search_client_index(v_clients, name);
                    Client c;
                    c = v_clients[index];
                    bank.close_account(files, name);
               }
          }
          else if(option == 7){
               string name, new_name;
               float amount;
               cout << "Write your account's name: ";
               cin >> name;
               if (!name_in_clients(clients(files), name)) cout << "The client doesn't exist, try again!" << endl;
               else{
                    int option7 = 0;
                    cout << "Change name(1); ";
                    cout << "Deposit amount(2); ";
                    cout << "Withdraw amount(3): ";
                    cin >> option7;
                    auto index = search_client_index(v_clients, name);
                    Client c;
                    
                    if(option7 == 1){
                         cout << "Write a new name: ";
                         cin >> new_name;
                         v_clients[index].change_name(new_name);
                         c = v_clients[index];
                         
                    }
                    if(option7 == 2){
                         cout << "How much cash's amount do you want to deposit? ";
                         cin >> amount;
                         v_clients[index].deposit_capital(amount);
                         c = v_clients[index];
                    }
                    if(option7 == 3){
                         cout << "How much cash's amount do you want to withdraw? ";
                         cin >> amount;
                         v_clients[index].withdraw_capital(amount);
                         c = v_clients[index];
                         

                    }
                    if(name_in_clients(clients(files), c.get_user())) bank.update_client(&c, files, index);
                    
               }
          }

          cout << endl;
     } while (option != 8);
     return 0;
}