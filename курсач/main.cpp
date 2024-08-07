#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <iomanip> 
#include <conio.h> 
#include <fstream>
#include <ctime> //����� ������ ������� ����
#include "enters.h"
#include "classes.h"
#include "requests.h"
#include "logins.h"
#include "sorts.h"

using namespace std;

//�������� ������� ���� ��� ��������
time_t now = time(0);
tm* ltm = localtime(&now);
int currentYear = 1900 + ltm->tm_year;

static int begin();
int menu();
int registration();

int user(const string& login);
int admin();

static void editAdminProfile(vector<Admin>& admins);

static void saveClientsToFile(const vector<Client>& clients);
static void loadClientsFromFile(vector<Client>& clients);
static void loadServicesFromFile(vector<service>& services);

void saveUserAppointment(const Client& client, const service& selectedService);
void displayUserAppointments();

static bool isLeapYear(int year)//�������� �� ���������� ���
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
static bool isValidDate(int day, int month, int year)//�������� �� ���������� ���� ����
{
    return !(day < 1 || month < 1 || year < 1 || year < 1900 || month > 12 ||
        ((month == 2 && day > (isLeapYear(year) ? 29 : 28)) ||
            ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) || ((month == 1 || month == 3 || month == 5 ||
                month == 7 || month == 8 || month == 10 || month == 12) && day > 31)));
}
static bool isValidDate2(int day, int month, int year)//�������� �� ���������� ���� ���� �� ������ 15.04
{
    return !(day < 1 || month < 1 || year < 1 || year < 2024 || month > 12 ||
        ((month == 2 && day > (isLeapYear(year) ? 29 : 28)) ||
            ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) || ((month == 1 || month == 3 || month == 5 ||
                month == 7 || month == 8 || month == 10 || month == 12) && day > 31)||(month <=5 && day<=30 && year<=2024)));
}
static bool isValidTime(int hour, int minute)//�������� �� ���������� ���� �������
{
    return !(hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

static void viewProfile(const string& login, const vector<Client>& clients);
Client findClientByLogin(const string& login, const vector<Client>& clients);

vector<service> services;//������ � ����������� �� �������
vector<Client> clients;//������ � ����������� � ��������
vector<Admin> admins;//������ � ����������� �� �������
vector<shedule> appointmentSlots;//������ � ����������� � ������� �������

int main()
{
    loadServicesFromFile(services);
    loadClientsFromFile(clients);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    begin();
}

string encryptPass(const string& password)//����������� ������
{
    string encryptedPassword;
    for (char c : password)
    {
        encryptedPassword += c + 10 ;
    }
    return encryptedPassword;
}

string decryptPass(const string& encryptedPassword)//�������������� ������
{
    string decryptedPassword;
    for (char c : encryptedPassword)
    {
        decryptedPassword += c - 10;
    }
    return decryptedPassword;
}

static int begin() // ����� ������� ���� (��� �������� ��������������)
{
    ifstream file("senior_admin_credentials.txt");
    string login;
    if (file.is_open()) {
        if (file.peek() == ifstream::traits_type::eof()) 
        {
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "|                        ������ ����, ��������� �������������!                               |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "| ����� ���� �������������� ������� ����� ������� �������� � ����� ������ �������.           |\n";
            cout << "| ����������, ������������� � ���������.                                                     |\n";
            cout << "| ����������� ��������� ��� �������� ���� ����� � ������, �.�. ������ � ��� ���� ����������� |\n";
            cout << "| ��������� ���� �������������.                                                              |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "������� �����: ";
            cin >> login;
            cout << "������� ������: ";
            const int size_password = 65;
            char password[size_password] = { 0 };
            char ch;
            int i = 0;
            int attempts = 0;
            int size = 0;
            do {
                size = 0;
                while (i < size_password - 1) 
                {
                    ch = _getch();
                    if (ch == 13) // ������� Enter
                        break;
                    else if (ch == 8) // ������� Backspace
                    {
                        if (i > 0) {
                            cout << "\b \b";
                            --i;
                        }
                    }
                    else {
                        password[i] = ch;
                        ++i;
                        cout << '*';
                    }
                    size++;
                }
                if (size != 6) cout << "\n������ ������� ��������. ������� ����� ������: ";
            } while (size != 6);
            cout << "\n����������� ������: ";
            const int size_newpassword = 65;
            char newpassword[size_newpassword] = { 0 };
            char ch_new;
            bool flag = false;
            do {
                int i_new = 0; // ����� �������� ����� ��������� ������
                while (i_new < size_newpassword - 1) {
                    ch_new = _getch();
                    if (ch_new == 13) // ������� Enter
                        break;
                    else if (ch_new == 8) // ������� Backspace
                    {
                        if (i_new > 0) {
                            cout << "\b \b";
                            --i_new;
                        }
                    }
                    else {
                        newpassword[i_new] = ch_new;
                        ++i_new;
                        cout << '*';
                    }
                }
                newpassword[i_new] = '\0'; 
                if (strcmp(password, newpassword) == 0) break;
                if (attempts == 3) {
                    cout << "\n�� ��������� ���������� ������� �����. ����������, ��������� �����������." << endl;
                    break;
                }
                if (!flag) cout << "\n������ �� ���������! ����������� ������: ";
                attempts++;

            } while (strcmp(password, newpassword) != 0);
            if (attempts < 3) {
                cout << endl << "�� ������� ������������! ����������, ������� �� ����� ������� ��� �����������...";
                saveSeniorAdminCredentials(login, password);
            }
            else {
                _getch();
                system("cls");
                return begin();
            }

        }
        else {
            return menu();
        }
        file.close();
        _getch();
        system("cls");
        return menu();
    }
    else return 0;
}

static int menu()//����
{
    int choice_role;
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|                  ������� ����� ����� � ������� ���� ��������������                         |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|1 - ������ ��� � ��� � ��������� ? ������������������                                       |\n";
    cout << "|2 - ��� ���� ������� ������ ? �����                                                         |\n";
    cout << "|0 - �����                                                                                   |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice_role;
    while (cin.fail() || choice_role < 0 || choice_role > 2)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice_role;
    }
    system("cls");
    switch (choice_role)
    {
    case 1: return registration();
    case 2: return enter();
    case 0: return 0;
    }
}

int registration()//�����������
{
    string login;
    cout << "+-----------------------------------------------+\n";
    cout << "|                  �����������                  |\n";
    cout << "+-----------------------------------------------+\n";
    bool flag_log = false;
    do
    {
        cout << "������� �����: ";
        cin >> login;
        if (checkLoginExists(login))
        {
            cout << "\n����� ����� ��� ����������. ���������� ������." << endl;
        }
        else flag_log = true;
    } while (!flag_log);
    writeLoginToFile(login);
    int size = 0;
    const int size_password = 65;
    char password[size_password] = { 0 };
    cout << "������� ������ (������� 6 ��������): ";
    do
    {
        size = 0;
        char ch;
        int i = 0;
        while (i < size_password - 1)
        {
            ch = _getch();
            if (ch == 13) // ������� Enter
                break;
            else if (ch == 8) // ������� Backspace
            {
                if (i > 0)
                {
                    cout << "\b \b";
                    --i;
                }
            }
            else
            {
                password[i] = ch;
                ++i;
                cout << '*';
            }
            size++;
            
        }
        if (size != 6) cout << "\n������ ������� ��������. ������� ����� ������: ";
    } while (size != 6);
    cout << "\n����������� ������: ";
    const int size_newpassword = 65;
    char newpassword[size_newpassword] = { 0 };
    char ch_new;
    bool flag = false;
    int attempts = 0;
    do
    {
        int i_new = 0; // ����� �������� ����� ��������� ������
        while (i_new < size_newpassword - 1)
        {
            ch_new = _getch();
            if (ch_new == 13) // ������� Enter
                break;
            else if (ch_new == 8) // ������� Backspace
            {
                if (i_new > 0)
                {
                    cout << "\b \b";
                    --i_new;
                }
            }
            else
            {
                newpassword[i_new] = ch_new;
                ++i_new;
                cout << '*';
            }
        }
        newpassword[i_new] = '\0'; 
        if (strcmp(password, newpassword) == 0) break;
        if (flag) cout << "\n������ �� ���������! ����������� ������: ";
        flag = true;
        if (strcmp(password, newpassword) == 0) break;
        if (attempts == 3)
        {
            cout << "\n�� ��������� ���������� ������� �����. ����������, ��������� �����������." << endl;
            _getch();
            system("cls");
            return registration();
        }
        if (!flag) cout << "\n������ �� ���������! ����������� ������: ";
        attempts++;
    } while (strcmp(password, newpassword) != 0);
    int choice;
    cout << "\n+-----------------------------------------------+\n";
    cout << "|1 - ������������������ ��� ������� ������������|\n";
    cout << "|2 - ������������������ ��� �������������       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 2)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        string name, surname, phone;
        date birthday;
        cout << "������� ���� ���: ";
        cin >> ws;
        getline(cin, name);
        cout << "������� ���� �������: ";
        getline(cin, surname);
        cout << "������� ��� ����� ��������: +";
        bool validInput = false;
        while (!validInput)
        {
            validInput = true;
            phone = "";
            char c;
            while ((c = cin.get()) != '\n')
            { // ��������� �������, ���� �� ����� ������ Enter
                if (c >=0 || c<=9)
                {
                    phone += c;
                }
                else if (cin.fail()||isalpha(c)||c < 0||c>9)
                {
                    
                    validInput = false;
                    
                }
                else
                {
                    validInput = false;
                }
               
            }
            cout << endl;

            if (phone.length() != 12)
            {
                validInput = false;
            }
            // ������� ��������� �� ������ � ��������� ����, ���� �� �����������
            if (!validInput)
            {
                cout << "������! ����� �������� ������ ��������� ����� 12 ����. ��������� ����: +";
            }
        }


        cout << "������� ���� ���� �������� (�� �� ����): ";
        cin >> birthday.day >> birthday.month >> birthday.year;
        while (!isValidDate(birthday.day, birthday.month, birthday.year))
        {
            cout << "�������� ����. ����������, ������� ���������� ����: ";
            cin >> birthday.day >> birthday.month >> birthday.year;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        int age = currentYear - birthday.year;
        if (currentYear < birthday.year || (currentYear == birthday.year && (ltm->tm_mon + 1) < birthday.month) ||
            ((ltm->tm_mon + 1) == birthday.month && ltm->tm_mday < birthday.day)) {
            age--; // ���� ��� �� ��� ���� �������� � ���� ����
        }

        if (age < 14) 
        {
            cout << "��������, ����������� �������� ������ ��� ��� ������ 14 ���." << endl;
            system("pause");
            system("cls");
            return menu();
        }
        Client newClient(login, name, surname, phone, birthday);
        clients.push_back(newClient);
        saveUserCredentials(login, password);
        saveClientsToFile(clients);
        cout << "������������ ������� ��������!\n������� �� ����� ������� ��� �����������...";
        _getch();
        system("cls");
        return user(login);
    }
    case 2:
    {
        string name, surname, phone;
        date birthday;
        cout << "������� ���� ���: ";
        cin >> ws;
        getline(cin, name);
        cout << "������� ���� �������: ";
        getline(cin, surname);
        cout << "������� ��� ����� ��������: +";
        bool validInput = false;
        while (!validInput)
        {
            validInput = true;
            phone = "";
            char c;
            while ((c = cin.get()) != '\n')
            { // ��������� �������, ���� �� ����� ������ Enter
                if (c >= 0 || c <= 9)
                {
                    phone += c;
                }
                else if (cin.fail() || isalpha(c) || c < 0 || c>9)
                {

                    validInput = false;

                }
                else
                {
                    validInput = false;
                }

            }
            cout << endl;

            if (phone.length() != 12)
            {
                validInput = false;
            }
           
            if (!validInput)
            {
                cout << "������! ����� �������� ������ ��������� ����� 12 ����. ��������� ����: +";
            }
        }

    
       
        cout << "������� ���� ���� �������� (�� �� ����): ";
        cin >> birthday.day >> birthday.month >> birthday.year;
        while (!isValidDate(birthday.day, birthday.month, birthday.year))
        {
            cout << "�������� ����. ����������, ������� ���������� ����: ";
            cin >> birthday.day >> birthday.month >> birthday.year;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        int age = currentYear - birthday.year;
        if (currentYear < birthday.year || (currentYear == birthday.year && (ltm->tm_mon + 1) < birthday.month) ||
            ((ltm->tm_mon + 1) == birthday.month && ltm->tm_mday < birthday.day)) {
            age--; // ���� ��� �� ��� ���� �������� � ���� ����, �� �������� ���� ���
        }

        if (age < 18)
        {
            cout << "��������, ����������� �������������� �������� ������ ��� ��� ������ 18 ���." << endl;
            system("pause");
            system("cls");
            return menu();
        }
        Admin newAdmin(login, name, surname, phone, birthday);
        admins.push_back(newAdmin);
        addAdminRequest(login, password, name, surname, phone, birthday);
        cout << "\n���� ������ ���������� �������� �������������� �� ������������.\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|                               ���������                                         |\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|�� ������� ���� �� ������ ����������� ����������� ���� � �������� ��������������.|\n";
        cout << "|���� ������ ����� ��������, �� � ��� �������� ����������� ��������������.        |\n";
        cout << "|� ��������� ������ ��� ��� ����� ������ ���������� �������� ������������.        |\n";
        cout << "+---------------------------------------------------------------------------------+";
        cout << "\n������� �� ����� ������� ��� ������ � ������� ����...\n";
        _getch();
        system("cls");
        return menu();
    }
    }
}

void saveUserAppointment(const Client& client, const service& selectedService)//������ � ���� ���������� � �����
{

    ofstream file("user_appointments.txt", ios::app);
    bool isTimeAvailable = true;
    for (const auto& appointment : appointmentSlots)
    {
        if (appointment.appointmentDate == client.getAppointmentDate() &&
            appointment.appointmentTime == client.getAppointmentTime() &&
            appointment.isBooked &&
            appointment.selectedService.getName() == selectedService.getName())
        {
            isTimeAvailable = false;
            break;
        }
    }

    if (!isTimeAvailable)
    {
        cout << "��������� ����� ��� ������. ���������� ��������� ������.\n";
        return;
    }
    if (file.is_open())
    {
        file << "�����: " << client.getlogin() << "\n";
        file << "���: " << client.getName() << "\n";
        file << "�������: " << client.getSurname() << "\n";
        file << "����� ��������: " << client.getPhone() << "\n";
        file << "������: " << selectedService.getName() << "\n";
        file << "������: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
        file << "����: " << client.appointmentDate << "\n";
        file << "�����: ";
        file << ((client.appointmentTime.hour < 10) ? "0" : "") << client.appointmentTime.hour << ":"
            << ((client.appointmentTime.minute < 10) ? "0" : "") << client.appointmentTime.minute << "\n";
        file << "----------------------------------\n";
        file.close();
    }
    else
    {
        cout << "������ ���������� ������." << endl;
    }
    fstream clientFile("clients.txt", ios::in | ios::out);
    ofstream tempFile("temp_clients.txt");

    string line;
    string login = client.getlogin();
    bool found = false;

    if (clientFile.is_open() && tempFile.is_open())
    {
        while (getline(clientFile, line))
        {
            // ���� ������� ������ � ������� ������������
            if (line.find("�����: " + login) != string::npos)
            {
                //��������� ����
                tempFile << line << "\n";
                found = true;

                // ���������� 5 ����� ������ � ���������� ����� ����������
                for (int i = 0; i < 4; ++i)
                {
                    getline(clientFile, line);
                    tempFile << line << "\n";
                }
                tempFile << "������: " << selectedService.getName() << "\n";
                tempFile << "������: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
                tempFile << "���� ������: " << client.appointmentDate << "\n";
                tempFile << "����� ������: ";
                tempFile << ((client.appointmentTime.hour < 10) ? "0" : "") << client.appointmentTime.hour << ":"
                    << ((client.appointmentTime.minute < 10) ? "0" : "") << client.appointmentTime.minute << "\n";
                
            }
            else
            {
                // �������� ��������� ������ �� ��������� ����� �� ���������
                tempFile << line << "\n";
            }
        }

        if (!found)
        {
            cout << "����� ������������ �� ������ � �����.\n";
        }

        
        clientFile.close();
        tempFile.close();

       //�������� �������
        remove("clients.txt");

        // ��������������� ��������� ���� � ��������
        rename("temp_clients.txt", "clients.txt");
    }
    else
    {
        cout << "������ �������� ������.\n";
    }
}

void displayUserAppointments()
{
    ifstream file("user_appointments.txt");
    string line;
    if (file.is_open())
    {
        cout << "+-----------------------------------------------+\n";
        cout << "|              ������ �������������             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

static void saveAdmin(const string& login, const string& name, const string& surname, const string& phone, const date& birthday)
{
    ofstream file("admin_contacts.txt", ios::app);
    if (file.is_open())
    {
        file << login << " " << name << " " << surname << " " << phone << " " << birthday.day << " " << birthday.month << " " << birthday.year << endl;
        cout << "\n������������� ������� ��������!\n";
        file.close();
    }
    else
    {
        cout << "������ ���������� ���������� ���������� ��������������." << endl;
    }
}

void editAdminProfile(vector<Admin>& admins)
{
    string name, surname, phone, login;
    date birthday;
    cout << "������� ��� �����: ";
    cin >> ws;
    getline(cin, login);
    cout << "������� ���� ���: ";
    getline(cin, name);
    cout << "������� ���� �������: ";
    getline(cin, surname);
    cout << "������� ��� ����� ��������: ";
    getline(cin, phone);
    cout << "������� ���� ���� �������� (�� �� ����): ";
    cin >> birthday.day >> birthday.month >> birthday.year;

    Admin newAdmin(login, name, surname, phone, birthday);
    admins.push_back(newAdmin);

}
static void editUserProfile(const string& login, vector<Client>& clients)
{
    cout << "\n+-------------------------------------------+\n";
    cout << "|               ��������� �������           |" << endl;
    cout << "+-------------------------------------------+\n";
    auto it = find_if(clients.begin(), clients.end(), [&login](const Client& client)//����� � ������� �� ������
        {
            return client.getlogin() == login;
        });

    if (it != clients.end())
    {
        int choice;
        cout << "+-------------------------------------------+\n";
        cout << "|��� �� ������ ��������?                    |\n";
        cout << "|1 - ���                                    |\n";
        cout << "|2 - �������                                |\n";
        cout << "|3 - ����� ��������                         |\n";
        cout << "|0 - ������                                 |\n";
        cout << "+-------------------------------------------+\n";
        cout << "��� �����: ";
        cin >> choice;
        while (cin.fail() || choice < 0|| choice > 3)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "+-------------------------------------------+\n";
            cout << "������� ���� ����� ���: ";
            cin >> ws;
            getline(cin, name);
            it->setName(name);
            break;
        }
        case 2:
        {
            string surname;
            cout << "+-------------------------------------------+\n";
            cout << "������� ���� ����� �������: ";
            cin >> ws;
            getline(cin, surname);
            it->setSurname(surname);
            break;
        }
        case 3:
        {
            string phone;
            cout << "+-------------------------------------------+\n";
            cout << "������� ��� ����� ����� ��������: ";
            cin >> ws;
            getline(cin, phone);
            it->setPhone(phone);
            break;
        }
        case 0:
            cout << "+-------------------------------------------+\n";
            cout << "������. ������� ������������ �� �������.\n";
            cout << "+-------------------------------------------+\n";
            return;
        default:
            cout << "�������� �����.\n";
            return;
        }

        // ���������� ����������� ������ � ����
        saveClientsToFile(clients);
        cout << "+-------------------------------------------+\n";
        cout << "������� ������������ ������� ��������!\n";
        cout << "+-------------------------------------------+\n";
    }
    else
    {
        cout << "������������ � ��������� ������� �� ������.\n";
    }
}

static void viewProfile(const string& login, const vector<Client>& clients)
{
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "������ �������� ����� � �������� �� ���������.\n";
        return;
    }
    // ����� ������� ������������ �� ������
    for (const auto& client : clients) 
    {
        
        if (client.getlogin() == login)
        {
            // ����� ���������� � ������������
            cout << "\n���������� � ������������:\n";
            cout << "�����: " << client.getlogin() << endl;
            cout << "���: " << client.getName() << endl;
            cout << "�������: " << client.getSurname() << endl;
            cout << "����� ��������: " << client.getPhone() << endl;
            cout << "���� ��������: " << client.getBirthday() << endl;
            // �������� �� ������� ������
            // ����� ������ � ��������� � �����
            string line;
            bool found = false;
            while (getline(appointmentsFile, line))
            {
                if (line.find("�����: " + login) != string::npos)
                {
                    // ������� ������ 
                    found = true;

                    cout << "\n���������� � ������ �� ���������:\n";

                    // ���������� ���� � ������� ������ �� ��������� ���� �����
                    string date, time, service;
                    
                    getline(appointmentsFile, line); // ������ "���: ..."
                    getline(appointmentsFile, line); // ������ "�������: ..."
                    getline(appointmentsFile, line); //������ "����� ��������: ..."
                    cout << "������: ";
                    getline(appointmentsFile, line); // ������ "������: ..."
                    service = line.substr(8);
                    cout << service << endl;
                    getline(appointmentsFile, line); // ������ "������: ..."
                    getline(appointmentsFile, line); // ������ "����: "
                    date = line.substr(6); // ��������� ����
                    getline(appointmentsFile, line); // ������ "�����: "
                    time = line.substr(7); // ��������� �����
                    cout << "���� � ����� ������: ";
                    cout << date << " � " << time << endl;

                    break;
                }
            }

            // ���� ������ �� �������
            if (!found)
            {
                cout << "� ��� ��� ������� �� ���������.\n";
            }

            // �������� ����� � �������� �� ���������
            appointmentsFile.close();
            return;
        }
    }
    // ���� ������� �� ������
    cout << "\n������� � ������� " << login << " �� ������." << endl;
            return;
}

static void displayUserReviews()//�������� ������ �������������
{
    ifstream file("user_reviews.txt");
    string line;
    if (file.is_open()) 
    {
        cout << "\n+-----------------------------------------------+\n";
        cout << "|              ������ �������������             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line)) 
        {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

static void saveUserReview(const Client& client, const string& review, const service& selectedService)//������ ������ � ����
{
    ofstream file("user_reviews.txt", ios::app); 
    if (file.is_open())
    {
        file << "�����: " << client.getlogin() << "\n";
        file << "������: " << selectedService.getName() << "\n";
        file << "������: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
        file << "�����: " << review << "\n"; 
        file << "----------------------------------\n";
        file.close();
    }
    else 
    {
        cout << "������ ���������� ������." << endl;
    }
}

// ������� ���������� ������
static void leaveReview(const Client& client, const vector<service>& services)
{
    cout << "\n+-------------------------------------------+\n";
    cout << "|                    �����                  |\n";
    cout << "+-------------------------------------------+\n";
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "������ �������� ����� � �������� �� ���������.\n";
        return;
    }

    string login = client.getlogin();
    bool found = false;
    string line, review;
    vector<pair<string, string>> userServices; // ������ ����� � ���, �� ������� ������������ ��� �������

    while (getline(appointmentsFile, line))
    {
        if (line.find("�����: " + login) != string::npos)
        {
            found = true;
            string serviceName, date, time;

            while (getline(appointmentsFile, line) && !line.empty())
            {
                if (line.find("������: ") != string::npos)
                {
                    serviceName = line.substr(8);
                }
                else if (line.find("����: ") != string::npos)
                {
                    date = line.substr(6);
                }
                else if (line.find("�����: ") != string::npos)
                {
                    time = line.substr(7);
                    userServices.push_back(make_pair(serviceName, date + " " + time));
                    break;
                }
            }
        }
    }

    appointmentsFile.close();

    if (!found)
    {
        cout << "� ��� ��� ������� �� ���������. � ���������, �� �� ������ �������� �����.\n";
        return;
    }

    cout << "�������� ���������, �� ������� �� ������ �������� �����:\n";
    for (int i = 0; i < userServices.size(); ++i)
    {
        cout << i + 1 << ". " << userServices[i].first << " (" << userServices[i].second << ")\n";
    }

    cout << "������� ����� ���������, �� ������� ������ �������� �����: ";
    int selectedIndex;
    cin >> selectedIndex;

    if (selectedIndex < 1 || selectedIndex > userServices.size())
    {
        cout << "������������ ����� ���������.\n";
        return;
    }

    int index = selectedIndex - 1; // ����������� � ������ �������
    cout << "����������, �������� ����� �� ������ " << userServices[index].first << " (" << userServices[index].second << ") : ";
    cin.ignore(); // ���������� ���������� ������ ����� ������ ����� ����� ������
    getline(cin, review);

    // ����� ��������������� ������ �� �����
    service selectedService;
    bool serviceFound = false;
    for (const auto& srv : services)
    {
        if (srv.getName() == userServices[index].first)
        {
            selectedService = srv;
            serviceFound = true;
            break;
        }
    }

    if (!serviceFound)
    {
        cout << "������ �� �������.\n";
        return;
    }

    saveUserReview(client, review, selectedService);
    cout << "\n+-------------------------------------------+\n";
    cout << "������� �� ��� �����!\n";
    cout << "+-------------------------------------------+\n";
}

static void saveClientsToFile(const vector<Client>& clients) // ������ �������� � ����
{
    ofstream file("clients.txt");
    if (file.is_open())
    {
        for (const auto& client : clients)
        {
            file << "�����: " << client.getlogin() << "\n";
            file << "���: " << client.getName() << "\n";
            file << "�������: " << client.getSurname() << "\n";
            file << "�������: " << client.getPhone() << "\n";
            file << "���� ��������: " << client.getBirthday().day << " " << client.getBirthday().month << " " << client.getBirthday().year << "\n";
            file << "\n";
        }
        file.close();
    }
    else
    {
        cout << "������ ���������� �������� � ����." << endl;
    }
}

void loadClientsFromFile(vector<Client>& clients)
{
    ifstream file("clients.txt");
    if (file.is_open())
    {
        clients.clear();
        string line;
        string login, name, surname, phone;
        master mast;
        date birthday;
       
        bool flag = false;//���� ��� ���������� ���� ��������
        bool flagService = false;//���� ��� ���������� ������
        while (getline(file, line))
        {
            // ���� ������� ��� ������ ��� �������, ��������� ��� � ������
            if (!login.empty() && !name.empty() && !surname.empty() && !phone.empty() && flag)
            {
                Client client(login, name, surname, phone, birthday);
                client.setLogin(login); // ������������� �����
                client.setName(name); // ������������� ���
                client.setSurname(surname); // ������������� �������
                client.setPhone(phone); // ������������� ����� ��������
                
                clients.push_back(client);
                
                // ���������� ������ ��� ���������� �������
                login.clear();
                name.clear();
                surname.clear();
                phone.clear();
                birthday = date();
               
            }

            if (line.empty())
                continue;

            // ��������� ������ ��������� � ��������� ������ ����
            if (line.find("�����: ") != string::npos)
            {
                flag = false;//��� ������ ������������
                flagService = false;
                login = line.substr(7);
            }
            else if (line.find("���: ") != string::npos)
            {
                name = line.substr(5);
            }
            else if (line.find("�������: ") != string::npos)
            {
                surname = line.substr(9);
            }
            else if (line.find("�������: ") != string::npos)
            {
                phone = line.substr(9);
            }
            else if (line.find("���� ��������: ") != string::npos)
            {
                int day, month, year;
                sscanf_s(line.c_str(), "���� ��������: %d %d %d", &day, &month, &year);
                birthday.day = day;
                birthday.month = month;
                birthday.year = year;
                flag = true;
            }
        }

        file.close();
    }
    else
    {
        cout << "���� � ��������� �� ������." << endl;
    }
}

Client findClientByLogin(const string& login, const vector<Client>& clients) //����� ���� � ������� �� ��� ������
{
    for (const auto& client : clients)
    {
        if (client.getlogin() == login)
        {
            return client;
        }
    }
    return Client();
}

//������� ������ � �����������
static void saveServicesToFile(const vector<service>& services)
{
    ofstream file("services.txt");
    if (file.is_open())
    {
        for (const auto& service : services)
        {
            file << service.getName() << "," << service.getInfo() << "," << service.getPrice() << "," << service.getDuration() << ","
                << service.getMaster().name << "," << service.getMaster().surname << endl;
        }
        file.close();
    }
    else
    {
        cout << "������ ���������� ����� � ����." << endl;
    }
}
static void addNewService(vector<service>& services)
{
    string name, info, masterName, masterSurname;
    int price, duration;
    cout << "+-----------------------------------------------+\n";
    cout << "|                ���������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "������� �������� ������: ";
    cin >> ws;
    getline(cin, name);
    cout << "������� ���������� �� ������: ";
    getline(cin, info);
    cout << "������� ��������� ������ (� ��� ������): ";
    cin >> price;
    while (cin.fail() || price < 0)//��������� �� ����� ���� �������������, ��������
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> price;
    }
    cout << "������� ������������ ������ (� �������): ";
    cin >> duration;
    while (cin.fail() || duration <= 0)//������������ �� ����� ���� �������������, ��������
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> duration;
    }
    cout << "������� ������� �������: ";
    cin >> masterSurname;
    cout << "������� ��� �������: ";
    cin >> masterName;
    master newMaster = { masterName, masterSurname };
    service newService(name, info, price, duration, newMaster);
    services.push_back(newService);//���������� ��������� � ������
    saveServicesToFile(services);
    cout << "\n+-----------------------------------------------+\n";
    cout << "|            ������ ������� ���������!          |\n";
    cout << "+-----------------------------------------------+\n";
}
static void displayServices(const vector<service>& services)
{
    cout << "+------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                                         ������� ��������                                                     |" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    cout << "| �   | " << setw(24) << left << "��������" << "|" << setw(37) << left << "����������" << "|"
        << setw(10) << left << "���������" << "|" << setw(15) << left << "������������" << "|"
        << setw(29) << left << "������" << "|" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i)
    {
        cout << "| " << setw(3) << left << i + 1 << " |" << setw(25) << left << services[i].getName() << "|"
            << setw(37) << left << services[i].getInfo() << "|"
            << setw(6) << left << services[i].getPrice() << "BYN |"
            << setw(11) << left << services[i].getDuration() << "��� |"
            << setw(15) << left << services[i].getMaster().name << " " << setw(13) << left << services[i].getMaster().surname << "|" << endl;
        cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    }
}
static void loadServicesFromFile(vector<service>& services)
{
    ifstream file("services.txt");
    if (file.is_open())
    {
        services.clear();
        string line;
        while (getline(file, line))
        {
            string name, info, masterName, masterSurname;
            int price, duration;

            int pos = line.find(",");
            name = line.substr(0, pos);
            line.erase(0, pos + 1);//������� �� ����� �������

            pos = line.find(",");
            info = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            price = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(",");
            duration = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(",");
            masterName = line.substr(0, pos);
            masterSurname = line.substr(pos + 1);

            master m = { masterName, masterSurname};
            service s(name, info, price, duration, m);
            services.push_back(s);//���������� � ������
        }
        file.close();
    }
    else
    {
        cout << "���� � �������� �� ������. �������� ������ � ������� ���� ��������������." << endl;
    }
}
static void editService(vector<service>& services)
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                 ��������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    int index;
    cout << "������� ����� ������ ��� ���������: ";
    cin >> index;

    if (index >= 1 && index <= services.size())
    {
        string newName, newInfo, newMasterName, newMasterSurname;
        int newPrice, newDuration;
        int choice;
        cout << "-----------------------------------------------\n";
        cout << "��� �� ������ ��������?\n";
        cout << "1 - �������� ������\n";
        cout << "2 - ���������� �� ������\n";
        cout << "3 - ��������� ������\n";
        cout << "4 - ������������ ������\n";
        cout << "5 - �������\n";
        cout << "0 - ������\n";
        cout << "-----------------------------------------------\n";
        cout << "��� �����: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 5)//��������
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� �������� ������: ";
            cin >> ws;
            getline(cin, newName);
            services[index - 1].name = newName;
            break;
        case 2:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ���������� �� ������: ";
            cin >> ws;
            getline(cin, newInfo);
            services[index - 1].info = newInfo;
            break;
        case 3:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ��������� ������: ";
            cin >> newPrice;
            while (cin.fail() || newPrice < 0)//��������� �� ����� ���� �������������, ��������
            {
                cout << "������! ������� ���������� ��������: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newPrice;
            }
            services[index - 1].price = newPrice;
            break;
        case 4:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ������������ ������ (� �������): ";
            cin >> newDuration;
            while (cin.fail() || newDuration < 0)//������������ �� ����� ���� �������������, ��������
            {
                cout << "������! ������� ���������� ��������: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newDuration;
            }
            services[index - 1].duration = newDuration;
            break;
        case 5:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ��� �������: ";
            cin >> newMasterName;
            cout << "������� ����� ������� �������: ";
            cin >> newMasterSurname;
            services[index - 1].serviceMaster.name = newMasterName;
            services[index - 1].serviceMaster.surname = newMasterSurname;
            break;
        default:
            cout << "-----------------------------------------------\n";
            cout << "��������� ��������.\n";
            cout << "-----------------------------------------------\n";
            return;
        }

        saveServicesToFile(services);
        cout << "\n+---------------------------------------------+\n";
        cout << "|              ������ ������� ��������!       |\n";
        cout << "+---------------------------------------------+\n";
    }
    else
    {
        cout << "�������� ����� ������.\n";
    }
}
static void deleteService(vector<service>& services)
{
    int index;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 �������� ������               |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "������� ����� ������ ��� ��������: ";
    cin >> index;
    int choice;
    if (index >= 1 && index <= services.size())
    {
        cout << "-------------------------------------------------\n";
        cout << "�� �������, ��� ������ ������� ������?\n1 - ��\n0 - ������\n";
        cout << "-------------------------------------------------\n";
        cout << "��� ����� : ";//����������� �������, ������� ����������
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 1)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        if (choice == 1)
        {
            services.erase(services.begin() + index - 1);//�������� �� �������
            saveServicesToFile(services);//���������� ������ � �����
            cout << "-------------------------------------------------\n";
            cout << "������ ������� �������!\n";
            cout << "-------------------------------------------------\n";
        }
        else
        {
            cout << "-------------------------------------------------\n";
            cout << "�������� ������ ��������!\n";
            cout << "-------------------------------------------------\n";
        }
    }
    else
    {
        cout << "�������� ����� ������.\n";
    }
}

//������� ���������� ������ ������
static void rejectAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    Client newClient(login, name, surname, phone, birthday);//���� ������ ���������, ��������� ������ ������������
    clients.push_back(newClient);
    saveUserCredentials(login, password);
    saveClientsToFile(clients);
    cout << "������ ���������. �������� ����� ������������." << endl;
    removeAdminRequest(login, password);//�������� ������������� ������
}

static int main_admin_menu()
{
    int choice;
    while (true)
    {
        cout << "+--------------------------------------------+\n";
        cout << "|         ���� �������� ��������������       |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - ����������� ������ ���������������      |\n";
        cout << "|2 - �������� ������                         |\n";
        cout << "|3 - ��������� ������                        |\n";
        cout << "|0 - ����� � ������� ����                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "��� �����: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 3)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        switch (choice)
        {
        case 1:
            displayAdminRequests();
            break;
        case 2:
        {
            string login;
            cout << "---------------------------\n";
            cout << "������� ����� �������������� ��� ���������: ";
            cin >> login;
            string password = getAdminPassword(login); // ��������� ������ �� �����
            string name = getAdminName(login);
            string surname = getSurname(login);
            string phone = getPhone(login);
            date birthday = getBirthday(login);
            approveAdminRequest(login, password, name, surname, phone, birthday);
            break;
        }
        case 3:
        {
            string login;
            cout << "---------------------------\n";
            cout << "������� ����� �������������� ��� ����������: ";
            cin >> login;
            string password = getAdminPassword(login);
            string name = getAdminName(login);
            string surname = getSurname(login);
            string phone = getPhone(login);
            date birthday = getBirthday(login);
            rejectAdminRequest(login, password, name, surname, phone, birthday);
            break;
        }
        case 0:
            system("cls");
            return menu();
            break;
        default:
            cout << "�������� �����." << endl;
        }
    }
}

static void displayTopProcedures() 
{
    ifstream file("user_appointments.txt");
    string line;
    vector<ProcedureInfo> procedures;
    if (file.is_open())
    {
        // c�������� ������ � ������������ ���������� ������� ��� ������ ���������
        while (getline(file, line)) 
        {
            if (line.find("������: ") == 0)
            {
                string procedureName = line.substr(8); // ��� ��������� ���������� � 8-�� �������
                bool found = false;
                for (auto& procedure : procedures)
                {
                    if (procedure.name == procedureName) 
                    {
                        procedure.count++;
                        found = true;
                        break;
                    }
                }
                if (!found) 
                {
                    procedures.push_back({ procedureName, 1 });
                }
            }
        }
        file.close();

        // ���������� ���������
        for (int i = 1; i < procedures.size(); ++i)
        {
            ProcedureInfo key = procedures[i];
            int j = i - 1;
            while (j >= 0 && procedures[j].count < key.count) 
            {
                procedures[j + 1] = procedures[j];
                j = j - 1;
            }
            procedures[j + 1] = key;
        }
        cout << "+-----------------------------------------------+\n";
        cout << "|            ��� ���������� ��������            |\n";
        cout << "+-----------------------------------------------+\n";
        for (int i = 0; i < min(5, static_cast<int>(procedures.size())); ++i) 
        {
            cout << i + 1 << ". " << procedures[i].name << " - " << procedures[i].count << " �������\n";
        }
        cout << "-------------------------------------------------\n";
    }
    else {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

static int admin()
{
    int choice;
    cout << "+--------------------------------------------+\n";
    cout << "|             ���� ��������������            |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|1 - �������� ������ � ������                |\n";
    cout << "|2 - ����������� ��� ������                  |\n";
    cout << "|3 - �������� ������                         |\n";
    cout << "|4 - ������� ������                          |\n";
    cout << "|5 - ����������� ������ �������������        |\n";
    cout << "|6 - ������� ��� ���������� ��������         |\n";
    cout << "|7 - ���������� ������                       |\n";
    cout << "|0 - ����� � ������� ����                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 0 || choice > 7)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        addNewService(services);
        break;
    }
    case 2:
    {
        displayServices(services);
        break;
    }
    case 3:
    {
        editService(services);
        break;
    }
    case 4:
    {
        displayServices(services);
        cout << endl;
        deleteService(services);
        break;
    }
    case 5:
        displayUserAppointments();
        break;
    case 6:
        displayTopProcedures();
        break;
    case 7:
        displayUserReviews();
        return admin();
    case 0:
        saveServicesToFile(services); // c��������� ����� � ���� ��� ������
        system("cls");
        return menu();
        break;
    default:
        cout << "�������� �����. ����������, �������� �����." << endl;
    }
    return admin();
}

vector<Time> findAvailableTimes(const date& chosenDate, const service& selectedService, const vector<shedule>& appointmentSlots)
{
    vector<Time> availableTimes;

    for (int hour = 9; hour < 21; hour += 3) 
    {
        availableTimes.push_back({ hour, 0 }); 
    }

    for (const auto& appointment : appointmentSlots)
    {
        if (appointment.appointmentDate   == chosenDate &&
            appointment.isBooked && appointment.selectedService.getName() == selectedService.getName()) 
        {
            Time bookedTime = appointment.appointmentTime;
            auto it = find(availableTimes.begin(), availableTimes.end(), bookedTime);
            if (it != availableTimes.end())
            {
                availableTimes.erase(it);
            }
        }
    }
    return availableTimes;
}
service selectedService;

static int user(const string& login) //������� ������������
{
    int choice;
    Client client;
    client.setLogin(login);
    cout << "\n+--------------------------------------------+\n";
    cout << "|              ���� ������������             |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|1 - ����������� ������ �����                |\n";
    cout << "|2 - ���������� � �������                    |\n";
    cout << "|3 - ���������� ������                       |\n";
    cout << "|4 - ����� ���������� � ���������            |\n";
    cout << "|5 - ��������� �������                       |\n";
    cout << "|6 - �������� �����                          |\n";
    cout << "|7 - ����������� ���� �������                |\n";
    cout << "|0 - ����� � ������� ����                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 0 || choice > 7)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
        cout << "\n---------------------------------------------";
        cout << "\n������� �� �� ������������� ������ �����?";
        cout << "\n1 - ��\n2 - ���";
        cout << "\n---------------------------------------------";
        cout << endl << "��� �����: ";
        int choiceDisp;
        cin >> choiceDisp;
        while (cin.fail() || choice < 1 || choice > 2)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choiceDisp;
        }
        switch (choiceDisp)
        {
        case 1:
        {
            int sortChoice;
            cout << "\n+-------------------------------------------+\n";
            cout << "|�������� ��� ����������:                   |\n";
            cout << "|1 - �� ����                                |\n";
            cout << "|2 - �� �������                             |\n";
            cout << "|3 - �� �������� ���������                  |\n";
            cout << "+-------------------------------------------+\n";
            cout << "��� �����: ";
            cin >> sortChoice;

            while (cin.fail() || sortChoice < 1 || sortChoice > 3)
            {
                cout << "������! ������� ���������� ��������: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> sortChoice;
            }
            switch (sortChoice)
            {
            case 1:
                sortByPrice(services);
                cout << "\n+-------------------------------------------+\n";
                cout << "|������ ����� (��������������� �� ����)     |" << endl;
                cout << "+-------------------------------------------+\n";
                displayServices(services);
                break;
            case 2:
                sortByTime(services);
                cout << "\n+-------------------------------------------+\n";
                cout << "|������ ����� (���������� �� ������������)  |" << endl;
                cout << "+-------------------------------------------+\n";
                displayServices(services);
                break;
            case 3:
                sortByName(services);
                cout << "\n+---------------------------------------------+\n";
                cout << "|������ ����� (��������������� �� ��������)   |" << endl;
                cout << "+---------------------------------------------+\n";
                displayServices(services);
                break;
            }
            return user(login);
        }
        case 2: displayServices(services);
            return user(login);
        }
        break;
    case 2:
    {
        // ������ ������ ������ ��� ������
        cout << "\n+-------------------------------------------+\n";
        cout << "|           ������ �� ���������             |" << endl;
        cout << "+-------------------------------------------+\n";
        cout << "�������� ������ ��� ������:" << endl;
        for (int i = 0; i < services.size(); ++i)
        {
            cout << i + 1 << ". " << services[i].getName() << endl;
        }
        cout << "---------------------------------------------\n";
        cout << "������� ����� ��� ������� 0 ��� ������: ";
        int serviceChoice;
        cin >> serviceChoice;
        // �������� ��������� ������
        if (serviceChoice >= 1 && serviceChoice <= services.size())
        {
            selectedService = services[serviceChoice - 1];
            cout << "---------------------------------------------\n";
            cout << "�� ������� ������: " << selectedService.getName();
            cout << "\n---------------------------------------------";
            // ���������� ���� ������
            date chosenDate;
            cout << "\n������� ���� (�� �� ����): ";
            cin >> chosenDate.day >> chosenDate.month >> chosenDate.year;

            // �������� ������������ �������� ����
            
            while (!isValidDate2 (chosenDate.day, chosenDate.month, chosenDate.year))
            {
                cout << "�������� ����. ����������, ������� ���������� ����: ";
                cin >> chosenDate.day >> chosenDate.month >> chosenDate.year;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

            // ��������� ��������� ��������� ����� ��� ��������� ���� � ������
            vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, appointmentSlots);

            if (availableTimes.empty())
            {
                cout << "�� ��������� ���� ��� ��������� ��������� ������.\n";
                return user(login);
            }

            // ����� ��������� ��������� ������
            cout << "��������� ��������� ����� ��� ��������� ����:\n";
            for (int i = 0; i < availableTimes.size(); ++i)
            {
                cout << i + 1 << ". " << availableTimes[i].hour << ":" << availableTimes[i].minute << endl;
            }

            // ������ ������ ���������� �����
            int timeChoice;
            cout << "�������� ����� (�����): ";
            cin >> timeChoice;

            // �������� ���������� �������
            if (timeChoice >= 1 && timeChoice <= availableTimes.size())
            {
                Time chosenTime = availableTimes[timeChoice - 1];

                // ���������, ������ �� ��������� ����� ��� ��������� ���� � ������
                bool isTimeAvailable = true;
                for (const auto& appointment : appointmentSlots)
                {
                    if (appointment.appointmentDate == chosenDate &&
                        appointment.appointmentTime == chosenTime &&
                        appointment.isBooked &&
                        appointment.selectedService.getName() == selectedService.getName())
                    {
                        isTimeAvailable = false;
                        break;
                    }
                }

                if (!isTimeAvailable)
                {
                    cout << "��������� ����� ��� ������. ����������, �������� ������ �����.\n";
                    return user(login);
                }
                else
                {
                    // ��������� ���� � ������� ������ �������
                    Client client = findClientByLogin(login, clients);
                    client.setAppointmentDate(chosenDate);
                    client.setAppointmentTime(chosenTime);
                    
                    saveUserAppointment(client, selectedService);

                    // ����� ���������� � ������
                    cout << "---------------------------------------------\n";
                    cout << "�� ������� �������� �� " << selectedService.getName() << " � "
                        << selectedService.getMaster().name << " " << selectedService.getMaster().surname
                        << " �� " << chosenDate.day << "/" << chosenDate.month << "/" << chosenDate.year
                        << " � " << chosenTime.hour << ":" << chosenTime.minute << "0" << endl;
                    cout << "---------------------------------------------\n";
                }
            }
            else
            {
                cout << "\n�������� ����� �������.\n";
            }
        }
        else if (serviceChoice == 0)
        {
            return user(login);
        }
        else
        {
            cout << "\n�������� ����� ������!" << endl;
        }
        return user(login);
    }
    case 3:
    {
       cout << "\n+-------------------------------------------+\n";
       cout << "|1 - ���������� �� ���������                |\n";
       cout << "|2 - ���������� �� ������������             |\n";
       cout << "|0 - ������                                 |\n";
       cout << "+-------------------------------------------+\n"; 
       cout << "��� ����� : ";
       int choiceFiltr;
       cin >> choiceFiltr;
       while (cin.fail() || choiceFiltr < 0 || choiceFiltr > 2)
       {
           cout << "������! ������� ���������� ��������: ";
           cin.clear();
           cin.ignore(INT_MAX, '\n');
           cin >> choiceFiltr;
       }
       double minPrice, maxPrice;
       switch (choiceFiltr)
       {
       case 1:
       {
           cout << "\n+-------------------------------------------+\n";
           cout << "|          ���������� �� ���������          |\n";
           cout << "+-------------------------------------------+\n";
           cout << "������� ����������� ���� (� BYN): ";
           cin >> minPrice;
           while (cin.fail())
           {
               cout << "������! ������� ���������� ��������: ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> minPrice;
           }

           cout << "������� ������������ ���� (� BYN): ";
           cin >> maxPrice;
           while (cin.fail() || maxPrice < minPrice)
           {
               cout << "������! ������� ���������� ��������: ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> maxPrice;
           }

           vector<service> filteredServices;
           for (const auto& serv : services)
           {
               if (serv.getPrice() >= minPrice && serv.getPrice() <= maxPrice)
               {
                   filteredServices.push_back(serv);
               }
           }

           if (filteredServices.empty())
           {
               cout << "��� ����� � �������� ��������� ���." << endl;
           }
           else
           {
               cout << "\n+-------------------------------------------+\n";
               cout << "������ ����� � �������� ��������� ���:" << endl;
               cout << "+-------------------------------------------+\n";
               displayServices(filteredServices);
           }

           break;
       }
       case 2:
       {
           cout << "\n+-------------------------------------------+\n";
           cout << "|          ���������� �� ������������       |\n";
           cout << "+-------------------------------------------+\n";
           int minTime, maxTime;
           cout << "������� ����������� ����� (� �������): ";
           cin >> minTime;
           while (cin.fail() || minTime < 0)
           {
               cout << "������! ������� ���������� ����� (� �������): ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> minTime;
           }

           cout << "������� ������������ ����� (� �������): ";
           cin >> maxTime;
           while (cin.fail() || maxTime < minTime)
           {
               cout << "������! ������� ���������� ����� (� �������): ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> maxTime;
           }
           vector<service> filteredServices;
           for (const auto& serv : services)
           {
               if (serv.getDuration() >= minTime && serv.getDuration() <= maxTime)
               {
                   filteredServices.push_back(serv);
               }
           }

           if (filteredServices.empty())
           {
               cout << "��� ����� � �������� ��������� �������." << endl;
           }
           else
           {
               cout << "\n+-------------------------------------------+\n";
               cout << "������ ����� � �������� ��������� �������:" << endl;
               cout << "+-------------------------------------------+\n";
               displayServices(filteredServices);
           }
           break;
       }
       case 3: return user(login);
       }
      
       return user(login);
    }
    case 4:
    {
        string searchQuery;
        cout << "\n+-------------------------------------------+\n";
        cout << "|               ����� �����                 |" << endl;
        cout << "+-------------------------------------------+\n";
        cout << "������� �������� ��������� ��� ������: ";
        cin.ignore();
        getline(cin, searchQuery); 

        vector<service> foundServices;
        for (const auto& serv : services)
        {
            if (serv.getName().find(searchQuery) != string::npos)
            {
                foundServices.push_back(serv);
            }
        }

        if (foundServices.empty())
        {
            cout << "������ � ��������� ��������� �� �������." << endl;
        }
        else
        {
            displayServices(foundServices);
        }

        return user(login);
    }

    case 5:
    {
        editUserProfile(login, clients);
        loadClientsFromFile(clients);//��������� ������
        return user(login);
    }
    case 6:
    {
        leaveReview(client, services);
        return user(login);
    }
    case 7:
    {
        viewProfile(login, clients);
        return user(login);
    }
    case 0:
    {
        system("cls");
        return menu();
    }
    }
}