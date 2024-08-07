#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <limits>
#include <iomanip> 
#include <conio.h> 
#include <fstream>

string encryptPass(const string& Password);
string decryptPass(const string& encryptedPassword);

static void saveClientsToFile(const vector<Client>& clients);

void saveUserCredentials(const string& login, const string& password);

bool checkSeniorAdminCredentials(const string& login, const string& password);

vector<Request> requests;

string getAdminPassword(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    bool loginFound = false; // переменная для отслеживания наличия логина
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем пароль
                loginFound = true; // флаг, что логин найден
                getline(file, line); // переход к следующей строке (паролю)
                file.close();
                return decryptPass(line.substr(8)); //обрезаем Пароль:  в начале строки
            }
        }
        file.close();
    }
    if (!loginFound)
    {
        // если логин не найден в файле
        return "";
    }
    // если файл закончился, но логин был найден 
    return "";
}

string getAdminName(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем имя
                for (int i = 0; i < 2; ++i) getline(file, line); // переходим к строке с именем
                file.close();
                return line.substr(5); // обрезая Имя: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден 
}

string getSurname(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем фамилию
                for (int i = 0; i < 3; ++i) getline(file, line); // переходим к строке с номером телефона
                file.close();
                return line.substr(9); // обрезаем Фамилия: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден 
}

string getPhone(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                //найден логин, извлекаем номер телефона
                for (int i = 0; i < 4; ++i) getline(file, line); // переход к строке с номером телефона
                file.close();
                return line.substr(9); // обрезаем Телефон: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден
}

date getBirthday(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем дату рождения
                for (int i = 0; i < 5; ++i) getline(file, line); // переход к строке с датой рождения
                file.close();
                date birthday;
                sscanf_s(line.c_str(), "Дата рождения: %d %d %d", &birthday.day, &birthday.month, &birthday.year);
                return birthday;
            }
        }
        file.close();
    }
    return {}; // если логин не найден 
}

void saveAdminCredentials(const string& login, const string& password)
{
    ofstream file("admin_credentials.txt", ios::app); // файл с логинами и паролями админов
    if (file.is_open())
    {
        cout << "Заявка одобрена. Новый администратор добавлен." << endl;
        file << endl << login << " " << encryptPass(password) << endl; // записываем логин и пароль
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

void readAdminRequests(vector<string>& allRequests)
{
    ifstream file("admin_requests.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            allRequests.push_back(line);
        }
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для чтения заявок." << endl;
    }
}

void displayAdminRequests()//вывести на экран список заявок
{
    vector<string> requests;
    readAdminRequests(requests);
    bool has = false;//есть ли заявки
    for (const auto& request : requests)
    {
        if (request.find("Логин: ") != string::npos)
        {
            has = true;
            break;
        }
    }
    if (!has)
    {
        cout << "+---------------------------+\n";
        cout << "|Нет заявок для рассмотрения|" << endl;
        cout << "+---------------------------+\n" << endl;
        return;
    }
    else
    {
        cout << "+-------------------------+\n";
        cout << "|      Список заявок      |\n";
        cout << "+-------------------------+\n";
        for (int i = 0; i < requests.size(); ++i)
        {
            if (requests[i].find("Логин: ") != string::npos)
            {
                string login = requests[i].substr(7); // логин после Логин: 
                string name, surname;

                if (i + 2 < requests.size() && requests[i + 2].find("Имя: ") != string::npos)
                {
                    name = requests[i + 2].substr(5); // имя после Имя: 
                }
                if (i + 3 < requests.size() && requests[i + 3].find("Фамилия: ") != string::npos)
                {
                    surname = requests[i + 3].substr(9); // фамилию после Фамилия: 
                }

                cout << "Логин: " << login << "\nИмя: " << name << "\nФамилия: " << surname << endl;
                cout << "---------------------------\n";
            }
        }
    }
    
}

void removeAdminRequest(const string& login, const string& password)
{
    vector<string> allRequests;
    readAdminRequests(allRequests);

    ofstream file("admin_requests_tmp.txt");
    if (file.is_open())
    {
        bool skipNextLines = false;
        int lines = 0;

        for (int i = 0; i < allRequests.size(); ++i)
        {
            if (!skipNextLines && allRequests[i].find("Логин: " + login) != string::npos)
            {
                // найдена запись, соответствующая логину
                skipNextLines = true;
                lines = 6; // пропустить 6 строк
            }
            if (skipNextLines)
            {
                if (lines > 0)
                {
                    lines--;
                    continue;
                }
                skipNextLines = false;
            }
            file << allRequests[i] << endl;
        }
        file.close();
        remove("admin_requests.txt");
        rename("admin_requests_tmp.txt", "admin_requests.txt");//заменяю старый файл новым
    }
    else
    {
        cout << "Ошибка открытия файла для записи заявок." << endl;
    }
}

void approveAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    // добавляем нового администратора в файл с учетными данными администраторов
    saveAdminCredentials(login, password);
    // удаляем заявку из списка заявок
    removeAdminRequest(login, password);
}

void addAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    ofstream file("admin_requests.txt", ios::app);
    if (file.is_open())
    {
        file << "Логин: " << login << "\n";
        file << "Пароль: " << encryptPass(password) << "\n";
        file << "Имя: " << name << "\n";
        file << "Фамилия: " << surname << "\n";
        file << "Телефон: " << phone << "\n";
        file << "Дата рождения: " << birthday.day << " " << birthday.month << " " << birthday.year << "\n";
        file << "\n"; // пустая строка для разделения заявок
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения заявки." << endl;
    }
}
