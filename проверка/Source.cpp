#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <iomanip> 
#include <conio.h> 
#include <fstream>

using namespace std;

static int menu()//����
{
    int choice_role;
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|                  ������� ����� ����� � ������� ���� ��������������                         |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|1 - ������ ��� � ��� � ��������� ? ������������������                                       |\n";
    cout << "|2 - ��� ���� ������� ������ ? �����                                                         |\n";
    cout << "|3 - �����                                                                                   |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice_role;
    while (cin.fail() || choice_role < 1 || choice_role > 3)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice_role;
    }
    system("cls");
    switch (choice_role)
    {
    case 1: return 1;
    case 2: return 2;
    case 3: return 0;
    }
}

struct date
{
public:
    int day, month, year;
    date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}
    void setDate(int d, int m, int y)
    {
        day = d;
        month = m;
        year = y;
    }
    void getDate() const
    {
        cout << ((day < 10) ? "0" : "") << day << "." << ((month < 10) ? "0" : "") << month << ".";
        if (year < 10) cout << "200";
        else if (year > 10 && year < 100) cout << "20";
        cout << year;
    }
};

struct Time
{
public: int hour, minute;
      Time(int h = 0, int m = 0, int s = 0) : hour(h), minute(m) {}
      void setTime(int h, int m, int s)
      {
          hour = (h >= 0 && h < 24) ? h : 0;
          minute = (m >= 0 && m < 60) ? m : 0;
      }
      void getTime() const
      {
          cout << ((hour < 10) ? "0" : "") << hour << ":" << ((minute < 10) ? "0" : "") << minute;
      }
};

class Person
{

public:
    string name;
    string surname;
    string phone;
    Person() {};
    Person(const string& name, const string& surname, const string& phone) :
        name(name), surname(surname), phone(phone) {}

    const string& getName() const { return name; }
    void setName(const string& newName) { name = newName; }

    const string& getSurname() const { return surname; }
    void setSurname(const string& newSurname) { surname = newSurname; }

    const string& getPhone() const { return phone; }
    void setPhone(const string& newPhone) { phone = newPhone; }
};

class Admin : public Person
{
private:

public:
    string login;
    string password;
    Admin() {};
    Admin(const string& login, const string& password,
        const string& name, const string& surname, const string& phone) :
        Person(name, surname, phone), login(login), password(password) {}

    const string& getLogin() const { return login; }
    void setLogin(const string& newLogin) { login = newLogin; }

    const string& getPassword() const { return password; }
    void setPassword(const string& newPassword) { password = newPassword; }
};

class Client : public Person
{
private:

public:
    date appointmentDate;  // ���� ������
    Time appointmentTime;  // ����� ������
    string login;
    string password;
    date birthday;
    Client() {};
    Client(const string& login, const string& password,
        const string& name, const string& surname, const string& phone,
        const date& birthday, const date& appointmentDate, const Time& appointmentTime) :
        Person(name, surname, phone), login(login), password(password),
        birthday(birthday), appointmentDate(appointmentDate), appointmentTime(appointmentTime) {}

    const string& getlogin() const { return login; }
    void setLogin(const string& newLogin) { login = newLogin; }

    const string& getPassword() const { return password; }
    void setPassword(const string& newPassword) { password = newPassword; }

    const date& getBirthday() const { return birthday; }
    void setBirthday(const date& newBirthday) { birthday = newBirthday; }

    const date& getAppointmentDate() const { return appointmentDate; }
    void setAppointmentDate(const date& newDate) { appointmentDate = newDate; }

    const Time& getAppointmentTime() const { return appointmentTime; }
    void setAppointmentTime(const Time& newTime) { appointmentTime = newTime; }
};

struct shedule
{
public:
    date date;
    Time time;
};

struct master
{
public:
    string name;
    string surname;
    shedule shedule;
};

class service
{
private:
    string name;
    string info;
    int price;
    int duration;
    master serviceMaster;
public:
    service() {}
    service(const string& n, const string& i, int p, int d, const master& m) :
        name(n), info(i), price(p), duration(d), serviceMaster(m) {}
    void setName(const string& n) { name = n; }
    const string& getName() const { return name; }
    void setInfo(const string& i) { info = i; }
    const string& getInfo() const { return info; }
    void setPrice(int p) { price = p; }
    int getPrice() const { return price; }
    void setDuration(int d) { duration = d; }
    int getDuration() const { return duration; }
    void setMaster(const master& m) { serviceMaster = m; }
    const master& getMaster() const { return serviceMaster; }
    void displayInfo() const
    {
        // cout << "���������� � ���������:" << endl;
        cout << "+------------------------------+------------------------------+" << endl;
        cout << "| " << setw(28) << left << name << " |" << setw(29) << left << info << " |" << setw(20) << left << price << " |" << endl;
        // cout << "+-----------------------------+-------------------------+" << endl;
        /* cout << "| " << setw(20) << left << "����������" << " | " << setw(25) << left << info << " |" << endl;
         cout << "+----------------------+-------------------------+" << endl;
         cout << "| " << setw(20) << left << "���������" << " | " << setw(25) << left << price << " |" << endl;
         cout << "+----------------------+-------------------------+" << endl;
         cout << "| " << setw(20) << left << "������������" << " | " << setw(20) << left << duration << " ��� |" << endl;
         cout << "+----------------------+-------------------------+" << endl;
         cout << "| " << setw(20) << left << "������" << " | " << setw(25) << left << serviceMaster.name << " " << serviceMaster.surname << " |" << endl;
         cout << "+----------------------+-------------------------+" << endl;*/
    }
};
vector<service> services;
bool isValidDate(int day, int month, int year)
{
    return !(day < 1 || month < 1 || year < 1 || (year >= 2024 && month >= 3 && day > 26) || year < 1980 || month > 12 ||
        ((month == 2 && day > (isLeapYear(year) ? 29 : 28)) ||
            ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) || ((month == 1 || month == 3 || month == 5 ||
                month == 7 || month == 8 || month == 10 || month == 12) && day > 31)));
}
bool isValidTime(int hour, int minute)
{
    return !(hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}
void saveUserAppointment(const Client& client, const service& selectedService)
{
    ofstream file("user_appointments.txt", ios::app);
    if (file.is_open())
    {
        file << "������������: " << client.getlogin() << "\n";
        file << "������: " << selectedService.getName() << "\n";
        file << "������: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
        file << "����: ";
        client.appointmentDate.getDate();
        file << "\n�����: ";
        client.appointmentTime.getTime();
        file << "\n----------------------------------\n";
        file.close();
    }
    else
    {
        cout << "������ ���������� ������." << endl;
    }
}
bool isLeapYear(int year)//�������� �� ���������� ���
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int user() //������� ������������
{
    int choice;
    Client client;
    bool hasProfileInfo = false; // ���������� ��� ������������ ������� ���������� � �������
    cout << "\n+--------------------------------------------+\n";
    cout << "|              ���� ������������             |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|1 - ����������� ������ �����                |\n";
    cout << "|2 - ���������� � �������                    |\n";
    cout << "|3 - ���������� ������                       |\n";
    cout << "|4 - ��������� �������                       |\n";
    cout << "|5 - �������� �����                          |\n";
    cout << "|6 - ����� � ������� ����                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 6)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    // ��������� ������� ���������� � �������
    if (!client.getName().empty() && !client.getSurname().empty() && !client.getPhone().empty())
    {
        hasProfileInfo = true;
    }
    switch (choice)
    {
    case 1:
        cout << "\n������ �����:" << endl;
        cout << "+-------------------------------------------------------------------------------------------------+" << endl;
        cout << "|                                      ������� ��������                                           |" << endl;
        cout << "+------------------------------+------------------------------+-----------------------------------+" << endl;
        cout << "| " << setw(28) << left << "��������" << " | " << setw(28) << left << "���������� � ���������" << " |" << setw(28) << "���������" << " |" << endl;
        cout << "+------------------------------+--------------------------------------------------------------------+" << endl;
        cout << "|                              |                                                                    |" << endl;
        for (const auto& service : services)
        {
            service.displayInfo();
        }
        cout << "+------------------------------+------------------------------+------------------------------+" << endl;
        return user();
    case 2:
    {
        cout << "\n�������� ������ ��� ������ (������� �����):" << endl;
        for (size_t i = 0; i < services.size(); ++i)
        {
            cout << i + 1 << ". " << services[i].getName() << endl;
        }
        int serviceChoice;
        cin >> serviceChoice;
        if (serviceChoice >= 1 && serviceChoice <= services.size())
        {
            service selectedService = services[serviceChoice - 1];
            cout << "\n�� ������� ������: " << selectedService.getName() << endl;

            date chosenDate;
            Time chosenTime;

            if (!hasProfileInfo)
            {
                cout << "������� ���� ���: ";
                cin >> client.name;
                cout << "������� ���� �������: ";
                cin >> client.surname;
                cout << "������� ��� ����� ��������: ";
                cin >> client.phone;
            }

            do
            {
                cout << "\n������� ���� (�� �� ����): ";
                cin >> chosenDate.day >> chosenDate.month >> chosenDate.year;

                if (!isValidDate(chosenDate.day, chosenDate.month, chosenDate.year))
                {
                    cout << "������! ������� ���������� ����." << endl;
                }
            } while (!isValidDate(chosenDate.day, chosenDate.month, chosenDate.year));

            do
            {
                cout << "������� ����� (�� ��): ";
                cin >> chosenTime.hour >> chosenTime.minute;

                if (!isValidTime(chosenTime.hour, chosenTime.minute))
                {
                    cout << "������! ������� ���������� �����." << endl;
                }
            } while (!isValidTime(chosenTime.hour, chosenTime.minute));

            client.setAppointmentDate(chosenDate);
            client.setAppointmentTime(chosenTime);
            saveUserAppointment(client, selectedService);

            cout << "\n�� ������� �������� �� " << selectedService.getName() << " � "
                << selectedService.getMaster().name << " " << selectedService.getMaster().surname
                << " �� ";
            chosenDate.getDate();
            cout << " � ";
            chosenTime.getTime();
        }
        else
        {
            cout << "\n�������� ����� ������!" << endl;
        }
        return user();
    }
    case 3:
    {
        int sortChoice;
        cout << "\n�������� ��� ����������:\n";
        cout << "1 - �� ����\n";
        cout << "2 - �� �������\n";
        cin >> sortChoice;

        while (cin.fail() || sortChoice < 1 || sortChoice > 2)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> sortChoice;
        }

        switch (sortChoice)
        {
        case 1:
            //sortByPrice(services);
            cout << "\n������ ����� (��������������� �� ����):" << endl;
            for (const auto& service : services)
            {
                service.displayInfo();
                cout << "-----------------------------------------" << endl;
            }
            break;

        case 2:
           // sortByTime(services);
            cout << "\n������ ����� (��������������� �� �������):" << endl;
            for (const auto& service : services)
            {
                service.displayInfo();
                cout << "-----------------------------------------" << endl;
            }
            break;
        }

        return user();
    }
    case 4:
    {
        //editProfile(client);
        return user();
    }
    case 5:
    {
        cout << "\n� ����������";
        _getch;
        return user();
        break;
    }
    case 6:
    {
        system("cls");
        return menu();
    }
    }
}

int admin()
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
    cout << "|6 - ����� � ������� ����                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 6)
    {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
        addNewService(services);
        break;
    case 2:
        // hairSalon.displayServices();
        // break;
        cout << "\n������ �����:" << endl;
        for (const auto& service : services)
        {
            service.displayInfo();
            cout << "-----------------------------------------" << endl;
        }
        break;
    case 3:
        editService(services);
        break;
    case 4:
        deleteService(services);
        break;
    case 5:
        cout << "\n������ �������������:" << endl;
        displayUserAppointments();
        break;
    case 6:
        saveServicesToFile(services); // ���������� ����� � ���� ��� ������
        system("cls");
        return menu();
        break;
    default:
        cout << "�������� �����. ����������, �������� �����." << endl;
    }
    return admin();
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    cout << "1 - ����� 2 - ������������: ";
    cin >> choice;
    if (choice == 1) admin();
    else user();
}