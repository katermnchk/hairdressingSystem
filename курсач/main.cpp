#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <iomanip> 
#include <conio.h> 
#include <fstream>
#include <ctime> //чтобы узнать текущую дату
#include "enters.h"
#include "classes.h"
#include "requests.h"
#include "logins.h"
#include "sorts.h"

using namespace std;

//получаем текущую дату для возраста
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

static bool isLeapYear(int year)//проверка на високосный год
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
static bool isValidDate(int day, int month, int year)//проверка на корректный ввод даты
{
    return !(day < 1 || month < 1 || year < 1 || year < 1900 || month > 12 ||
        ((month == 2 && day > (isLeapYear(year) ? 29 : 28)) ||
            ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) || ((month == 1 || month == 3 || month == 5 ||
                month == 7 || month == 8 || month == 10 || month == 12) && day > 31)));
}
static bool isValidDate2(int day, int month, int year)//проверка на корректный ввод даты на момент 15.04
{
    return !(day < 1 || month < 1 || year < 1 || year < 2024 || month > 12 ||
        ((month == 2 && day > (isLeapYear(year) ? 29 : 28)) ||
            ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) || ((month == 1 || month == 3 || month == 5 ||
                month == 7 || month == 8 || month == 10 || month == 12) && day > 31)||(month <=5 && day<=30 && year<=2024)));
}
static bool isValidTime(int hour, int minute)//проверка на корректный ввод времени
{
    return !(hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

static void viewProfile(const string& login, const vector<Client>& clients);
Client findClientByLogin(const string& login, const vector<Client>& clients);

vector<service> services;//вектор с информацией об услугах
vector<Client> clients;//вектор с информацией о клиентах
vector<Admin> admins;//вектор с информацией об админах
vector<shedule> appointmentSlots;//вектор с информацией о занятых окошках

int main()
{
    loadServicesFromFile(services);
    loadClientsFromFile(clients);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    begin();
}

string encryptPass(const string& password)//кодирование пароля
{
    string encryptedPassword;
    for (char c : password)
    {
        encryptedPassword += c + 10 ;
    }
    return encryptedPassword;
}

string decryptPass(const string& encryptedPassword)//раскодирование пароля
{
    string decryptedPassword;
    for (char c : encryptedPassword)
    {
        decryptedPassword += c - 10;
    }
    return decryptedPassword;
}

static int begin() // самый главный вход (для старшего администратора)
{
    ifstream file("senior_admin_credentials.txt");
    string login;
    if (file.is_open()) {
        if (file.peek() == ifstream::traits_type::eof()) 
        {
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "|                        Добрый день, уважаемый администратор!                               |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "| Перед вами информационная система учета женских процедур в вашем салоне красоты.           |\n";
            cout << "| Пожалуйста, авторизуйтесь в программе.                                                     |\n";
            cout << "| Рекомендуем запомнить или записать свой логин и пароль, т.к. только у вас есть возможность |\n";
            cout << "| раздавать роли пользователям.                                                              |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "Введите логин: ";
            cin >> login;
            cout << "Введите пароль: ";
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
                    if (ch == 13) // клавиша Enter
                        break;
                    else if (ch == 8) // клавиша Backspace
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
                if (size != 6) cout << "\nПароль слишком короткий. Введите новый пароль: ";
            } while (size != 6);
            cout << "\nПодтвердите пароль: ";
            const int size_newpassword = 65;
            char newpassword[size_newpassword] = { 0 };
            char ch_new;
            bool flag = false;
            do {
                int i_new = 0; // сброс счетчика перед повторным вводом
                while (i_new < size_newpassword - 1) {
                    ch_new = _getch();
                    if (ch_new == 13) // клавиша Enter
                        break;
                    else if (ch_new == 8) // клавиша Backspace
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
                    cout << "\nВы превысили количество попыток ввода. Пожалуйста, повторите регистрацию." << endl;
                    break;
                }
                if (!flag) cout << "\nПароли не совпадают! Подтвердите пароль: ";
                attempts++;

            } while (strcmp(password, newpassword) != 0);
            if (attempts < 3) {
                cout << endl << "Вы успешно авторизованы! Пожалуйста, нажмите на любую клавишу для продолжения...";
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

static int menu()//меню
{
    int choice_role;
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|                  Система учета услуг в женском зале парикмахерской                         |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|1 - Первый раз у нас в программе ? Зарегистрироваться                                       |\n";
    cout << "|2 - Уже есть учетная запись ? Войти                                                         |\n";
    cout << "|0 - Выход                                                                                   |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice_role;
    while (cin.fail() || choice_role < 0 || choice_role > 2)
    {
        cout << "Ошибка! Введите корректное значение: ";
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

int registration()//регистрация
{
    string login;
    cout << "+-----------------------------------------------+\n";
    cout << "|                  РЕГИСТРАЦИЯ                  |\n";
    cout << "+-----------------------------------------------+\n";
    bool flag_log = false;
    do
    {
        cout << "Введите логин: ";
        cin >> login;
        if (checkLoginExists(login))
        {
            cout << "\nТакой логин уже существует. Попробуйте другой." << endl;
        }
        else flag_log = true;
    } while (!flag_log);
    writeLoginToFile(login);
    int size = 0;
    const int size_password = 65;
    char password[size_password] = { 0 };
    cout << "Введите пароль (минимум 6 символов): ";
    do
    {
        size = 0;
        char ch;
        int i = 0;
        while (i < size_password - 1)
        {
            ch = _getch();
            if (ch == 13) // клавиша Enter
                break;
            else if (ch == 8) // клавиша Backspace
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
        if (size != 6) cout << "\nПароль слишком короткий. Введите новый пароль: ";
    } while (size != 6);
    cout << "\nПодтвердите пароль: ";
    const int size_newpassword = 65;
    char newpassword[size_newpassword] = { 0 };
    char ch_new;
    bool flag = false;
    int attempts = 0;
    do
    {
        int i_new = 0; // сброс счетчика перед повторным вводом
        while (i_new < size_newpassword - 1)
        {
            ch_new = _getch();
            if (ch_new == 13) // клавиша Enter
                break;
            else if (ch_new == 8) // клавиша Backspace
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
        if (flag) cout << "\nПароли не совпадают! Подтвердите пароль: ";
        flag = true;
        if (strcmp(password, newpassword) == 0) break;
        if (attempts == 3)
        {
            cout << "\nВы превысили количество попыток ввода. Пожалуйста, повторите регистрацию." << endl;
            _getch();
            system("cls");
            return registration();
        }
        if (!flag) cout << "\nПароли не совпадают! Подтвердите пароль: ";
        attempts++;
    } while (strcmp(password, newpassword) != 0);
    int choice;
    cout << "\n+-----------------------------------------------+\n";
    cout << "|1 - зарегистрироваться как обычный пользователь|\n";
    cout << "|2 - зарегистрироваться как администратор       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 2)
    {
        cout << "Ошибка! Введите корректное значение: ";
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
        cout << "Введите ваше имя: ";
        cin >> ws;
        getline(cin, name);
        cout << "Введите вашу фамилию: ";
        getline(cin, surname);
        cout << "Введите ваш номер телефона: +";
        bool validInput = false;
        while (!validInput)
        {
            validInput = true;
            phone = "";
            char c;
            while ((c = cin.get()) != '\n')
            { // считываем символы, пока не будет введен Enter
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
            // Выводим сообщение об ошибке и повторяем ввод, если он некорректен
            if (!validInput)
            {
                cout << "Ошибка! Номер телефона должен содержать ровно 12 цифр. Повторите ввод: +";
            }
        }


        cout << "Введите вашу дату рождения (дд мм гггг): ";
        cin >> birthday.day >> birthday.month >> birthday.year;
        while (!isValidDate(birthday.day, birthday.month, birthday.year))
        {
            cout << "Неверный ввод. Пожалуйста, введите корректную дату: ";
            cin >> birthday.day >> birthday.month >> birthday.year;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        int age = currentYear - birthday.year;
        if (currentYear < birthday.year || (currentYear == birthday.year && (ltm->tm_mon + 1) < birthday.month) ||
            ((ltm->tm_mon + 1) == birthday.month && ltm->tm_mday < birthday.day)) {
            age--; // если еще не был день рождения в этом году
        }

        if (age < 14) 
        {
            cout << "Извините, регистрация доступна только для лиц старше 14 лет." << endl;
            system("pause");
            system("cls");
            return menu();
        }
        Client newClient(login, name, surname, phone, birthday);
        clients.push_back(newClient);
        saveUserCredentials(login, password);
        saveClientsToFile(clients);
        cout << "Пользователь успешно добавлен!\nНажмите на любую клавишу для продолжения...";
        _getch();
        system("cls");
        return user(login);
    }
    case 2:
    {
        string name, surname, phone;
        date birthday;
        cout << "Введите ваше имя: ";
        cin >> ws;
        getline(cin, name);
        cout << "Введите вашу фамилию: ";
        getline(cin, surname);
        cout << "Введите ваш номер телефона: +";
        bool validInput = false;
        while (!validInput)
        {
            validInput = true;
            phone = "";
            char c;
            while ((c = cin.get()) != '\n')
            { // считываем символы, пока не будет введен Enter
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
                cout << "Ошибка! Номер телефона должен содержать ровно 12 цифр. Повторите ввод: +";
            }
        }

    
       
        cout << "Введите вашу дату рождения (дд мм гггг): ";
        cin >> birthday.day >> birthday.month >> birthday.year;
        while (!isValidDate(birthday.day, birthday.month, birthday.year))
        {
            cout << "Неверный ввод. Пожалуйста, введите корректную дату: ";
            cin >> birthday.day >> birthday.month >> birthday.year;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        int age = currentYear - birthday.year;
        if (currentYear < birthday.year || (currentYear == birthday.year && (ltm->tm_mon + 1) < birthday.month) ||
            ((ltm->tm_mon + 1) == birthday.month && ltm->tm_mday < birthday.day)) {
            age--; // если еще не был день рождения в этом году, то вычитаем один год
        }

        if (age < 18)
        {
            cout << "Извините, регистрация администратора доступна только для лиц старше 18 лет." << endl;
            system("pause");
            system("cls");
            return menu();
        }
        Admin newAdmin(login, name, surname, phone, birthday);
        admins.push_back(newAdmin);
        addAdminRequest(login, password, name, surname, phone, birthday);
        cout << "\nВаша заявка отправлена главному администратору на рассмотрение.\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|                               Подсказка                                         |\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|На главном меню вы можете попробовать осуществить вход в качестве администратора.|\n";
        cout << "|Если заявка будет одобрена, то у вас появятся возможности администратора.        |\n";
        cout << "|В противном случае для вас будет открыт функционал обычного пользователя.        |\n";
        cout << "+---------------------------------------------------------------------------------+";
        cout << "\nНажмите на любую клавишу для выхода в главное меню...\n";
        _getch();
        system("cls");
        return menu();
    }
    }
}

void saveUserAppointment(const Client& client, const service& selectedService)//запись в файл информации о юзере
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
        cout << "Выбранное время уже занято. Невозможно сохранить запись.\n";
        return;
    }
    if (file.is_open())
    {
        file << "Логин: " << client.getlogin() << "\n";
        file << "Имя: " << client.getName() << "\n";
        file << "Фамилия: " << client.getSurname() << "\n";
        file << "Номер телефона: " << client.getPhone() << "\n";
        file << "Услуга: " << selectedService.getName() << "\n";
        file << "Мастер: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
        file << "Дата: " << client.appointmentDate << "\n";
        file << "Время: ";
        file << ((client.appointmentTime.hour < 10) ? "0" : "") << client.appointmentTime.hour << ":"
            << ((client.appointmentTime.minute < 10) ? "0" : "") << client.appointmentTime.minute << "\n";
        file << "----------------------------------\n";
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения записи." << endl;
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
            // если найдена строка с логином пользователя
            if (line.find("Логин: " + login) != string::npos)
            {
                //временный файл
                tempFile << line << "\n";
                found = true;

                // пропускаем 5 строк вперед и записываем новую информацию
                for (int i = 0; i < 4; ++i)
                {
                    getline(clientFile, line);
                    tempFile << line << "\n";
                }
                tempFile << "Услуга: " << selectedService.getName() << "\n";
                tempFile << "Мастер: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
                tempFile << "Дата записи: " << client.appointmentDate << "\n";
                tempFile << "Время записи: ";
                tempFile << ((client.appointmentTime.hour < 10) ? "0" : "") << client.appointmentTime.hour << ":"
                    << ((client.appointmentTime.minute < 10) ? "0" : "") << client.appointmentTime.minute << "\n";
                
            }
            else
            {
                // копируем остальные строки из исходного файла во временный
                tempFile << line << "\n";
            }
        }

        if (!found)
        {
            cout << "Логин пользователя не найден в файле.\n";
        }

        
        clientFile.close();
        tempFile.close();

       //удаление старого
        remove("clients.txt");

        // переименовываем временный файл в исходный
        rename("temp_clients.txt", "clients.txt");
    }
    else
    {
        cout << "Ошибка открытия файлов.\n";
    }
}

void displayUserAppointments()
{
    ifstream file("user_appointments.txt");
    string line;
    if (file.is_open())
    {
        cout << "+-----------------------------------------------+\n";
        cout << "|              Записи пользователей             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла с записями пользователей." << endl;
    }
}

static void saveAdmin(const string& login, const string& name, const string& surname, const string& phone, const date& birthday)
{
    ofstream file("admin_contacts.txt", ios::app);
    if (file.is_open())
    {
        file << login << " " << name << " " << surname << " " << phone << " " << birthday.day << " " << birthday.month << " " << birthday.year << endl;
        cout << "\nАдминистратор успешно добавлен!\n";
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения контактной информации администратора." << endl;
    }
}

void editAdminProfile(vector<Admin>& admins)
{
    string name, surname, phone, login;
    date birthday;
    cout << "Введите ваш логин: ";
    cin >> ws;
    getline(cin, login);
    cout << "Введите ваше имя: ";
    getline(cin, name);
    cout << "Введите вашу фамилию: ";
    getline(cin, surname);
    cout << "Введите ваш номер телефона: ";
    getline(cin, phone);
    cout << "Введите вашу дату рождения (дд мм гггг): ";
    cin >> birthday.day >> birthday.month >> birthday.year;

    Admin newAdmin(login, name, surname, phone, birthday);
    admins.push_back(newAdmin);

}
static void editUserProfile(const string& login, vector<Client>& clients)
{
    cout << "\n+-------------------------------------------+\n";
    cout << "|               Изменение профиля           |" << endl;
    cout << "+-------------------------------------------+\n";
    auto it = find_if(clients.begin(), clients.end(), [&login](const Client& client)//поиск в векторе по логину
        {
            return client.getlogin() == login;
        });

    if (it != clients.end())
    {
        int choice;
        cout << "+-------------------------------------------+\n";
        cout << "|Что вы хотите изменить?                    |\n";
        cout << "|1 - Имя                                    |\n";
        cout << "|2 - Фамилию                                |\n";
        cout << "|3 - Номер телефона                         |\n";
        cout << "|0 - Отмена                                 |\n";
        cout << "+-------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        while (cin.fail() || choice < 0|| choice > 3)
        {
            cout << "Ошибка! Введите корректное значение: ";
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
            cout << "Введите ваше новое имя: ";
            cin >> ws;
            getline(cin, name);
            it->setName(name);
            break;
        }
        case 2:
        {
            string surname;
            cout << "+-------------------------------------------+\n";
            cout << "Введите вашу новую фамилию: ";
            cin >> ws;
            getline(cin, surname);
            it->setSurname(surname);
            break;
        }
        case 3:
        {
            string phone;
            cout << "+-------------------------------------------+\n";
            cout << "Введите ваш новый номер телефона: ";
            cin >> ws;
            getline(cin, phone);
            it->setPhone(phone);
            break;
        }
        case 0:
            cout << "+-------------------------------------------+\n";
            cout << "Отмена. Профиль пользователя не изменен.\n";
            cout << "+-------------------------------------------+\n";
            return;
        default:
            cout << "Неверный выбор.\n";
            return;
        }

        // сохранение обновленных данных в файл
        saveClientsToFile(clients);
        cout << "+-------------------------------------------+\n";
        cout << "Профиль пользователя успешно обновлен!\n";
        cout << "+-------------------------------------------+\n";
    }
    else
    {
        cout << "Пользователь с указанным логином не найден.\n";
    }
}

static void viewProfile(const string& login, const vector<Client>& clients)
{
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "Ошибка открытия файла с записями на процедуры.\n";
        return;
    }
    // поиск профиля пользователя по логину
    for (const auto& client : clients) 
    {
        
        if (client.getlogin() == login)
        {
            // вывод информации о пользователе
            cout << "\nИнформация о пользователе:\n";
            cout << "Логин: " << client.getlogin() << endl;
            cout << "Имя: " << client.getName() << endl;
            cout << "Фамилия: " << client.getSurname() << endl;
            cout << "Номер телефона: " << client.getPhone() << endl;
            cout << "Дата рождения: " << client.getBirthday() << endl;
            // проверка на наличие записи
            // Поиск записи о процедуре в файле
            string line;
            bool found = false;
            while (getline(appointmentsFile, line))
            {
                if (line.find("Логин: " + login) != string::npos)
                {
                    // найдена запись 
                    found = true;

                    cout << "\nИнформация о записи на процедуру:\n";

                    // извлечение даты и времени записи из следующих двух строк
                    string date, time, service;
                    
                    getline(appointmentsFile, line); // строка "Имя: ..."
                    getline(appointmentsFile, line); // строка "Фамилия: ..."
                    getline(appointmentsFile, line); //строка "Номер телефона: ..."
                    cout << "Услуга: ";
                    getline(appointmentsFile, line); // строка "Услуга: ..."
                    service = line.substr(8);
                    cout << service << endl;
                    getline(appointmentsFile, line); // строка "Мастер: ..."
                    getline(appointmentsFile, line); // строка "Дата: "
                    date = line.substr(6); // извлекаем дату
                    getline(appointmentsFile, line); // строка "Время: "
                    time = line.substr(7); // извлекаем время
                    cout << "Дата и время записи: ";
                    cout << date << " в " << time << endl;

                    break;
                }
            }

            // Если запись не найдена
            if (!found)
            {
                cout << "У вас нет записей на процедуры.\n";
            }

            // Закрытие файла с записями на процедуры
            appointmentsFile.close();
            return;
        }
    }
    // если профиль не найден
    cout << "\nПрофиль с логином " << login << " не найден." << endl;
            return;
}

static void displayUserReviews()//показать отзывы пользователей
{
    ifstream file("user_reviews.txt");
    string line;
    if (file.is_open()) 
    {
        cout << "\n+-----------------------------------------------+\n";
        cout << "|              Отзывы пользователей             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line)) 
        {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Ошибка открытия файла с отзывами пользователей." << endl;
    }
}

static void saveUserReview(const Client& client, const string& review, const service& selectedService)//запись отзыва в файл
{
    ofstream file("user_reviews.txt", ios::app); 
    if (file.is_open())
    {
        file << "Логин: " << client.getlogin() << "\n";
        file << "Услуга: " << selectedService.getName() << "\n";
        file << "Мастер: " << selectedService.getMaster().name << " " << selectedService.getMaster().surname << "\n";
        file << "Отзыв: " << review << "\n"; 
        file << "----------------------------------\n";
        file.close();
    }
    else 
    {
        cout << "Ошибка сохранения отзыва." << endl;
    }
}

// функция оставления отзыва
static void leaveReview(const Client& client, const vector<service>& services)
{
    cout << "\n+-------------------------------------------+\n";
    cout << "|                    Отзыв                  |\n";
    cout << "+-------------------------------------------+\n";
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "Ошибка открытия файла с записями на процедуры.\n";
        return;
    }

    string login = client.getlogin();
    bool found = false;
    string line, review;
    vector<pair<string, string>> userServices; // список услуг и дат, на которые пользователь был записан

    while (getline(appointmentsFile, line))
    {
        if (line.find("Логин: " + login) != string::npos)
        {
            found = true;
            string serviceName, date, time;

            while (getline(appointmentsFile, line) && !line.empty())
            {
                if (line.find("Услуга: ") != string::npos)
                {
                    serviceName = line.substr(8);
                }
                else if (line.find("Дата: ") != string::npos)
                {
                    date = line.substr(6);
                }
                else if (line.find("Время: ") != string::npos)
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
        cout << "У вас нет записей на процедуры. К сожалению, вы не можете оставить отзыв.\n";
        return;
    }

    cout << "Выберите процедуру, на которую вы хотите оставить отзыв:\n";
    for (int i = 0; i < userServices.size(); ++i)
    {
        cout << i + 1 << ". " << userServices[i].first << " (" << userServices[i].second << ")\n";
    }

    cout << "Введите номер процедуры, на которую хотите оставить отзыв: ";
    int selectedIndex;
    cin >> selectedIndex;

    if (selectedIndex < 1 || selectedIndex > userServices.size())
    {
        cout << "Некорректный номер процедуры.\n";
        return;
    }

    int index = selectedIndex - 1; // Преобразуем в индекс массива
    cout << "Пожалуйста, оставьте отзыв об услуге " << userServices[index].first << " (" << userServices[index].second << ") : ";
    cin.ignore(); // Игнорируем оставшийся символ новой строки после ввода номера
    getline(cin, review);

    // Найти соответствующую услугу по имени
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
        cout << "Услуга не найдена.\n";
        return;
    }

    saveUserReview(client, review, selectedService);
    cout << "\n+-------------------------------------------+\n";
    cout << "Спасибо за ваш отзыв!\n";
    cout << "+-------------------------------------------+\n";
}

static void saveClientsToFile(const vector<Client>& clients) // запись клиентов в файл
{
    ofstream file("clients.txt");
    if (file.is_open())
    {
        for (const auto& client : clients)
        {
            file << "Логин: " << client.getlogin() << "\n";
            file << "Имя: " << client.getName() << "\n";
            file << "Фамилия: " << client.getSurname() << "\n";
            file << "Телефон: " << client.getPhone() << "\n";
            file << "Дата рождения: " << client.getBirthday().day << " " << client.getBirthday().month << " " << client.getBirthday().year << "\n";
            file << "\n";
        }
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения клиентов в файл." << endl;
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
       
        bool flag = false;//флаг для считывания даты рождения
        bool flagService = false;//флаг для считывания услуги
        while (getline(file, line))
        {
            // если считаны все данные для клиента, добавляем его в вектор
            if (!login.empty() && !name.empty() && !surname.empty() && !phone.empty() && flag)
            {
                Client client(login, name, surname, phone, birthday);
                client.setLogin(login); // устанавливаем логин
                client.setName(name); // устанавливаем имя
                client.setSurname(surname); // устанавливаем фамилию
                client.setPhone(phone); // устанавливаем номер телефона
                
                clients.push_back(client);
                
                // сбрасываем данные для следующего клиента
                login.clear();
                name.clear();
                surname.clear();
                phone.clear();
                birthday = date();
               
            }

            if (line.empty())
                continue;

            // считываем данные построчно и извлекаем нужные поля
            if (line.find("Логин: ") != string::npos)
            {
                flag = false;//для нового пользователя
                flagService = false;
                login = line.substr(7);
            }
            else if (line.find("Имя: ") != string::npos)
            {
                name = line.substr(5);
            }
            else if (line.find("Фамилия: ") != string::npos)
            {
                surname = line.substr(9);
            }
            else if (line.find("Телефон: ") != string::npos)
            {
                phone = line.substr(9);
            }
            else if (line.find("Дата рождения: ") != string::npos)
            {
                int day, month, year;
                sscanf_s(line.c_str(), "Дата рождения: %d %d %d", &day, &month, &year);
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
        cout << "Файл с клиентами не найден." << endl;
    }
}

Client findClientByLogin(const string& login, const vector<Client>& clients) //поиск инфо о клиенте по его логину
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

//функции работы с процедурами
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
        cout << "Ошибка сохранения услуг в файл." << endl;
    }
}
static void addNewService(vector<service>& services)
{
    string name, info, masterName, masterSurname;
    int price, duration;
    cout << "+-----------------------------------------------+\n";
    cout << "|                Добавление услуги              |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Введите название услуги: ";
    cin >> ws;
    getline(cin, name);
    cout << "Введите информацию об услуге: ";
    getline(cin, info);
    cout << "Введите стоимость услуги (в бел рублях): ";
    cin >> price;
    while (cin.fail() || price < 0)//стоимость не может быть отрицательной, проверка
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> price;
    }
    cout << "Введите длительность услуги (в минутах): ";
    cin >> duration;
    while (cin.fail() || duration <= 0)//длительность не может быть отрицательной, проверка
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> duration;
    }
    cout << "Введите фамилию мастера: ";
    cin >> masterSurname;
    cout << "Введите имя мастера: ";
    cin >> masterName;
    master newMaster = { masterName, masterSurname };
    service newService(name, info, price, duration, newMaster);
    services.push_back(newService);//добавление процедуры в вектор
    saveServicesToFile(services);
    cout << "\n+-----------------------------------------------+\n";
    cout << "|            Услуга успешно добавлена!          |\n";
    cout << "+-----------------------------------------------+\n";
}
static void displayServices(const vector<service>& services)
{
    cout << "+------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                                         Каталог процедур                                                     |" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    cout << "| №   | " << setw(24) << left << "Название" << "|" << setw(37) << left << "Информация" << "|"
        << setw(10) << left << "Стоимость" << "|" << setw(15) << left << "Длительность" << "|"
        << setw(29) << left << "Мастер" << "|" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i)
    {
        cout << "| " << setw(3) << left << i + 1 << " |" << setw(25) << left << services[i].getName() << "|"
            << setw(37) << left << services[i].getInfo() << "|"
            << setw(6) << left << services[i].getPrice() << "BYN |"
            << setw(11) << left << services[i].getDuration() << "мин |"
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
            line.erase(0, pos + 1);//переход на новую строчку

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
            services.push_back(s);//добавление в вектор
        }
        file.close();
    }
    else
    {
        cout << "Файл с услугами не найден. Создайте услуги с помощью меню администратора." << endl;
    }
}
static void editService(vector<service>& services)
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Изменение услуги              |\n";
    cout << "+-----------------------------------------------+\n";
    int index;
    cout << "Введите номер услуги для изменения: ";
    cin >> index;

    if (index >= 1 && index <= services.size())
    {
        string newName, newInfo, newMasterName, newMasterSurname;
        int newPrice, newDuration;
        int choice;
        cout << "-----------------------------------------------\n";
        cout << "Что вы хотите изменить?\n";
        cout << "1 - Название услуги\n";
        cout << "2 - Информацию об услуге\n";
        cout << "3 - Стоимость услуги\n";
        cout << "4 - Длительность услуги\n";
        cout << "5 - Мастера\n";
        cout << "0 - Отмена\n";
        cout << "-----------------------------------------------\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 5)//проверка
        {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            cout << "-----------------------------------------------\n";
            cout << "Введите новое название услуги: ";
            cin >> ws;
            getline(cin, newName);
            services[index - 1].name = newName;
            break;
        case 2:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую информацию об услуге: ";
            cin >> ws;
            getline(cin, newInfo);
            services[index - 1].info = newInfo;
            break;
        case 3:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую стоимость услуги: ";
            cin >> newPrice;
            while (cin.fail() || newPrice < 0)//стоимость не может быть отрицательной, проверка
            {
                cout << "Ошибка! Введите корректное значение: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newPrice;
            }
            services[index - 1].price = newPrice;
            break;
        case 4:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую длительность услуги (в минутах): ";
            cin >> newDuration;
            while (cin.fail() || newDuration < 0)//длительность не может быть отрицательной, проверка
            {
                cout << "Ошибка! Введите корректное значение: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newDuration;
            }
            services[index - 1].duration = newDuration;
            break;
        case 5:
            cout << "-----------------------------------------------\n";
            cout << "Введите новое имя мастера: ";
            cin >> newMasterName;
            cout << "Введите новую фамилию мастера: ";
            cin >> newMasterSurname;
            services[index - 1].serviceMaster.name = newMasterName;
            services[index - 1].serviceMaster.surname = newMasterSurname;
            break;
        default:
            cout << "-----------------------------------------------\n";
            cout << "Изменение отменено.\n";
            cout << "-----------------------------------------------\n";
            return;
        }

        saveServicesToFile(services);
        cout << "\n+---------------------------------------------+\n";
        cout << "|              Услуга успешно изменена!       |\n";
        cout << "+---------------------------------------------+\n";
    }
    else
    {
        cout << "Неверный номер услуги.\n";
    }
}
static void deleteService(vector<service>& services)
{
    int index;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Удаление услуги               |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Введите номер услуги для удаления: ";
    cin >> index;
    int choice;
    if (index >= 1 && index <= services.size())
    {
        cout << "-------------------------------------------------\n";
        cout << "Вы уверены, что хотите удалить услугу?\n1 - Да\n0 - Отмена\n";
        cout << "-------------------------------------------------\n";
        cout << "Ваш выбор : ";//необратимый процесс, поэтому спрашиваем
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 1)
        {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        if (choice == 1)
        {
            services.erase(services.begin() + index - 1);//удаление из вектора
            saveServicesToFile(services);//перезапись данных в файле
            cout << "-------------------------------------------------\n";
            cout << "Услуга успешно удалена!\n";
            cout << "-------------------------------------------------\n";
        }
        else
        {
            cout << "-------------------------------------------------\n";
            cout << "Удаление услуги отменено!\n";
            cout << "-------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Неверный номер услуги.\n";
    }
}

//функция отклонения заявки админа
static void rejectAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    Client newClient(login, name, surname, phone, birthday);//если заявка отклонена, добавляем ноовго пользователя
    clients.push_back(newClient);
    saveUserCredentials(login, password);
    saveClientsToFile(clients);
    cout << "Заявка отклонена. Добавлен новый пользователь." << endl;
    removeAdminRequest(login, password);//удаление рассмотренной заявки
}

static int main_admin_menu()
{
    int choice;
    while (true)
    {
        cout << "+--------------------------------------------+\n";
        cout << "|         МЕНЮ СТАРШЕГО АДМИНИСТРАТОРА       |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - Просмотреть заявки администраторов      |\n";
        cout << "|2 - Одобрить заявку                         |\n";
        cout << "|3 - Отклонить заявку                        |\n";
        cout << "|0 - Выйти в главное меню                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 3)
        {
            cout << "Ошибка! Введите корректное значение: ";
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
            cout << "Введите логин администратора для одобрения: ";
            cin >> login;
            string password = getAdminPassword(login); // получение пароля из файла
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
            cout << "Введите логин администратора для отклонения: ";
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
            cout << "Неверный выбор." << endl;
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
        // cчитываем данные и подсчитываем количество записей для каждой процедуры
        while (getline(file, line)) 
        {
            if (line.find("Услуга: ") == 0)
            {
                string procedureName = line.substr(8); // имя процедуры начинается с 8-го символа
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

        // сортировка вставками
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
        cout << "|            Топ популярных процедур            |\n";
        cout << "+-----------------------------------------------+\n";
        for (int i = 0; i < min(5, static_cast<int>(procedures.size())); ++i) 
        {
            cout << i + 1 << ". " << procedures[i].name << " - " << procedures[i].count << " записей\n";
        }
        cout << "-------------------------------------------------\n";
    }
    else {
        cout << "Ошибка открытия файла с записями пользователей." << endl;
    }
}

static int admin()
{
    int choice;
    cout << "+--------------------------------------------+\n";
    cout << "|             МЕНЮ АДМИНИСТРАТОРА            |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|1 - Добавить услугу в список                |\n";
    cout << "|2 - Просмотреть все услуги                  |\n";
    cout << "|3 - Изменить услугу                         |\n";
    cout << "|4 - Удалить услугу                          |\n";
    cout << "|5 - Просмотреть записи пользователей        |\n";
    cout << "|6 - Вывести топ популярных процедур         |\n";
    cout << "|7 - Посмотреть отзывы                       |\n";
    cout << "|0 - Выйти в главное меню                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    while (cin.fail() || choice < 0 || choice > 7)
    {
        cout << "Ошибка! Введите корректное значение: ";
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
        saveServicesToFile(services); // cохранение услуг в файл при выходе
        system("cls");
        return menu();
        break;
    default:
        cout << "Неверный выбор. Пожалуйста, выберите снова." << endl;
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

static int user(const string& login) //обычный пользователь
{
    int choice;
    Client client;
    client.setLogin(login);
    cout << "\n+--------------------------------------------+\n";
    cout << "|              МЕНЮ ПОЛЬЗОВАТЕЛЯ             |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|1 - Просмотреть список услуг                |\n";
    cout << "|2 - Записаться к мастеру                    |\n";
    cout << "|3 - Фильтрация данных                       |\n";
    cout << "|4 - Поиск информации о процедуре            |\n";
    cout << "|5 - Изменение профиля                       |\n";
    cout << "|6 - Оставить отзыв                          |\n";
    cout << "|7 - Просмотреть свой профиль                |\n";
    cout << "|0 - Выйти в главное меню                    |\n";
    cout << "+--------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    while (cin.fail() || choice < 0 || choice > 7)
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
        cout << "\n---------------------------------------------";
        cout << "\nЖелаете ли вы отсортировать список услуг?";
        cout << "\n1 - да\n2 - нет";
        cout << "\n---------------------------------------------";
        cout << endl << "Ваш выбор: ";
        int choiceDisp;
        cin >> choiceDisp;
        while (cin.fail() || choice < 1 || choice > 2)
        {
            cout << "Ошибка! Введите корректное значение: ";
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
            cout << "|Выберите тип сортировки:                   |\n";
            cout << "|1 - По цене                                |\n";
            cout << "|2 - По времени                             |\n";
            cout << "|3 - По названию процедуры                  |\n";
            cout << "+-------------------------------------------+\n";
            cout << "Ваш выбор: ";
            cin >> sortChoice;

            while (cin.fail() || sortChoice < 1 || sortChoice > 3)
            {
                cout << "Ошибка! Введите корректное значение: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> sortChoice;
            }
            switch (sortChoice)
            {
            case 1:
                sortByPrice(services);
                cout << "\n+-------------------------------------------+\n";
                cout << "|Список услуг (отсортированный по цене)     |" << endl;
                cout << "+-------------------------------------------+\n";
                displayServices(services);
                break;
            case 2:
                sortByTime(services);
                cout << "\n+-------------------------------------------+\n";
                cout << "|Список услуг (сортировка по длительности)  |" << endl;
                cout << "+-------------------------------------------+\n";
                displayServices(services);
                break;
            case 3:
                sortByName(services);
                cout << "\n+---------------------------------------------+\n";
                cout << "|Список услуг (отсортированный по названию)   |" << endl;
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
        // запрос номера услуги для записи
        cout << "\n+-------------------------------------------+\n";
        cout << "|           Запись на процедуру             |" << endl;
        cout << "+-------------------------------------------+\n";
        cout << "Выберите услугу для записи:" << endl;
        for (int i = 0; i < services.size(); ++i)
        {
            cout << i + 1 << ". " << services[i].getName() << endl;
        }
        cout << "---------------------------------------------\n";
        cout << "Введите номер или нажмите 0 для выхода: ";
        int serviceChoice;
        cin >> serviceChoice;
        // проверка выбранной услуги
        if (serviceChoice >= 1 && serviceChoice <= services.size())
        {
            selectedService = services[serviceChoice - 1];
            cout << "---------------------------------------------\n";
            cout << "Вы выбрали услугу: " << selectedService.getName();
            cout << "\n---------------------------------------------";
            // заполнение даты записи
            date chosenDate;
            cout << "\nВведите дату (дд мм гггг): ";
            cin >> chosenDate.day >> chosenDate.month >> chosenDate.year;

            // проверка корректности введённой даты
            
            while (!isValidDate2 (chosenDate.day, chosenDate.month, chosenDate.year))
            {
                cout << "Неверный ввод. Пожалуйста, введите корректную дату: ";
                cin >> chosenDate.day >> chosenDate.month >> chosenDate.year;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

            // проверяем доступные временные слоты для выбранной даты и услуги
            vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, appointmentSlots);

            if (availableTimes.empty())
            {
                cout << "На выбранную дату нет доступных временных слотов.\n";
                return user(login);
            }

            // вывод доступных временных слотов
            cout << "Доступные временные слоты для выбранной даты:\n";
            for (int i = 0; i < availableTimes.size(); ++i)
            {
                cout << i + 1 << ". " << availableTimes[i].hour << ":" << availableTimes[i].minute << endl;
            }

            // запрос номера временного слота
            int timeChoice;
            cout << "Выберите время (номер): ";
            cin >> timeChoice;

            // проверка выбранного времени
            if (timeChoice >= 1 && timeChoice <= availableTimes.size())
            {
                Time chosenTime = availableTimes[timeChoice - 1];

                // проверяем, занято ли выбранное время для выбранной даты и услуги
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
                    cout << "Выбранное время уже занято. Пожалуйста, выберите другое время.\n";
                    return user(login);
                }
                else
                {
                    // установка даты и времени записи клиента
                    Client client = findClientByLogin(login, clients);
                    client.setAppointmentDate(chosenDate);
                    client.setAppointmentTime(chosenTime);
                    
                    saveUserAppointment(client, selectedService);

                    // вывод информации о записи
                    cout << "---------------------------------------------\n";
                    cout << "Вы успешно записаны на " << selectedService.getName() << " к "
                        << selectedService.getMaster().name << " " << selectedService.getMaster().surname
                        << " на " << chosenDate.day << "/" << chosenDate.month << "/" << chosenDate.year
                        << " в " << chosenTime.hour << ":" << chosenTime.minute << "0" << endl;
                    cout << "---------------------------------------------\n";
                }
            }
            else
            {
                cout << "\nНеверный выбор времени.\n";
            }
        }
        else if (serviceChoice == 0)
        {
            return user(login);
        }
        else
        {
            cout << "\nНеверный выбор услуги!" << endl;
        }
        return user(login);
    }
    case 3:
    {
       cout << "\n+-------------------------------------------+\n";
       cout << "|1 - Фильтрация по стоимости                |\n";
       cout << "|2 - Фильтрация по длительности             |\n";
       cout << "|0 - Отмена                                 |\n";
       cout << "+-------------------------------------------+\n"; 
       cout << "Ваш выбор : ";
       int choiceFiltr;
       cin >> choiceFiltr;
       while (cin.fail() || choiceFiltr < 0 || choiceFiltr > 2)
       {
           cout << "Ошибка! Введите корректное значение: ";
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
           cout << "|          Фильтрация по стоимости          |\n";
           cout << "+-------------------------------------------+\n";
           cout << "Введите минимальную цену (в BYN): ";
           cin >> minPrice;
           while (cin.fail())
           {
               cout << "Ошибка! Введите корректное значение: ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> minPrice;
           }

           cout << "Введите максимальную цену (в BYN): ";
           cin >> maxPrice;
           while (cin.fail() || maxPrice < minPrice)
           {
               cout << "Ошибка! Введите корректное значение: ";
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
               cout << "Нет услуг в заданном диапазоне цен." << endl;
           }
           else
           {
               cout << "\n+-------------------------------------------+\n";
               cout << "Список услуг в заданном диапазоне цен:" << endl;
               cout << "+-------------------------------------------+\n";
               displayServices(filteredServices);
           }

           break;
       }
       case 2:
       {
           cout << "\n+-------------------------------------------+\n";
           cout << "|          Фильтрация по длительности       |\n";
           cout << "+-------------------------------------------+\n";
           int minTime, maxTime;
           cout << "Введите минимальное время (в минутах): ";
           cin >> minTime;
           while (cin.fail() || minTime < 0)
           {
               cout << "Ошибка! Введите корректное время (в минутах): ";
               cin.clear();
               cin.ignore(INT_MAX, '\n');
               cin >> minTime;
           }

           cout << "Введите максимальное время (в минутах): ";
           cin >> maxTime;
           while (cin.fail() || maxTime < minTime)
           {
               cout << "Ошибка! Введите корректное время (в минутах): ";
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
               cout << "Нет услуг в заданном диапазоне времени." << endl;
           }
           else
           {
               cout << "\n+-------------------------------------------+\n";
               cout << "Список услуг в заданном диапазоне времени:" << endl;
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
        cout << "|               Поиск услуг                 |" << endl;
        cout << "+-------------------------------------------+\n";
        cout << "Введите название процедуры для поиска: ";
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
            cout << "Услуги с указанным названием не найдены." << endl;
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
        loadClientsFromFile(clients);//обновляем данные
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