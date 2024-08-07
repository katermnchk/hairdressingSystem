#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <limits>
#include <iomanip> 
#include <conio.h> 

using namespace std;

int menu();
int admin();
int user(const string& login);
int main_admin_menu();

int enter_user();
int enter_admin();
int enter_main_admin();

bool checkAdminCredentials(const string& login, const string& password);
bool checkUserCredentials(const string& login, const string& password);
bool checkSeniorAdminCredentials(const string& login, const string& password);


int enter()
{
    int choice;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Вход в систему                |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "|1 - Войти как обычный пользователь             |\n";
    cout << "|2 - Войти как администратор                    |\n";
    cout << "|3 - Войти как главный администратор            |\n";
    cout << "|4 - Выйти в главное меню                       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 4)
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }
    switch (choice)
    {
    case 1: system("cls"); return enter_user();
    case 2: system("cls"); return enter_admin();
    case 3: system("cls"); return enter_main_admin();
    case 4: system("cls"); return menu();
    }
    cout << "\nНажмите на любую клавишу для продолжения...";
    _getch();
    system("cls");
    return menu();
}

int enter_user()//вход обычного пользователя
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                      Вход                     |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    const int size_password = 65;
    char password[size_password] = { 0 };
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
    }
    while (true)
    {
        if (checkUserCredentials(login, password))
        {
            cout << "\nВы вошли как обычный пользователь!" << endl;
            _getch();
            system("cls");
            user(login);
            break;
        }
        else
        {
            cout << "\nНеверный логин или пароль. Попробуйте еще раз." << endl;
            return menu();
        }
    }
}

int enter_admin() // вход администратора
{
    cout << "+-----------------------------------------------+\n";
    cout << "|              Вход администратора              |\n";
    cout << "+-----------------------------------------------+\n";
    string login, password;
    cout << "Введите логин: ";
    cin >> login;

    if (login == "0") // проверяем, был ли введен 0 для выхода
    {
        cout << endl;
        system("cls");
        return menu();
    }

    cout << "Введите пароль: ";

    const int size_password = 65;
    char inputPassword[size_password] = { 0 };
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
            inputPassword[i] = ch;
            ++i;
            cout << '*';
        }
    }

    inputPassword[i] = '\0'; 
    password = inputPassword;

    if (checkAdminCredentials(login, password))
    {
        cout << "\nУспешно выполнен вход в качестве администратора" << endl;
        cout << "Пожалуйста, нажмите на любую клавишу для входа...";
        _getch();
        system("cls");
        return  admin(); // возвращаемся в главное меню
    }
    else
    {
        cout << "\nНеверный логин или пароль. Попробуйте еще раз или нажмите 0 для выхода." << endl;
        return enter_admin(); // повторный вход для администратора
    }
}

int enter_main_admin()//вход старшего администратора
{
    cout << "+-----------------------------------------------+\n";
    cout << "|         Вход главного администратора          |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    const int size_password = 65;
    char password[size_password] = { 0 };
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
    }
    while (true)
    {
        if (checkSeniorAdminCredentials(login, password))
        {
            cout << "\nВы вошли как старший администратор!" << endl;
            _getch();
            system("cls");
            main_admin_menu();
            break;
        }
        else
        {
            cout << "\nНеверный логин или пароль. Попробуйте еще раз." << endl;
            return menu();
        }
    }
}
