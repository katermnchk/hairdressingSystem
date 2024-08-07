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
    cout << "|                 ���� � �������                |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "|1 - ����� ��� ������� ������������             |\n";
    cout << "|2 - ����� ��� �������������                    |\n";
    cout << "|3 - ����� ��� ������� �������������            |\n";
    cout << "|4 - ����� � ������� ����                       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 4)
    {
        cout << "������! ������� ���������� ��������: ";
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
    cout << "\n������� �� ����� ������� ��� �����������...";
    _getch();
    system("cls");
    return menu();
}

int enter_user()//���� �������� ������������
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                      ����                     |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "������� �����: ";
    cin >> login;
    cout << "������� ������: ";
    const int size_password = 65;
    char password[size_password] = { 0 };
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
    }
    while (true)
    {
        if (checkUserCredentials(login, password))
        {
            cout << "\n�� ����� ��� ������� ������������!" << endl;
            _getch();
            system("cls");
            user(login);
            break;
        }
        else
        {
            cout << "\n�������� ����� ��� ������. ���������� ��� ���." << endl;
            return menu();
        }
    }
}

int enter_admin() // ���� ��������������
{
    cout << "+-----------------------------------------------+\n";
    cout << "|              ���� ��������������              |\n";
    cout << "+-----------------------------------------------+\n";
    string login, password;
    cout << "������� �����: ";
    cin >> login;

    if (login == "0") // ���������, ��� �� ������ 0 ��� ������
    {
        cout << endl;
        system("cls");
        return menu();
    }

    cout << "������� ������: ";

    const int size_password = 65;
    char inputPassword[size_password] = { 0 };
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
            inputPassword[i] = ch;
            ++i;
            cout << '*';
        }
    }

    inputPassword[i] = '\0'; 
    password = inputPassword;

    if (checkAdminCredentials(login, password))
    {
        cout << "\n������� �������� ���� � �������� ��������������" << endl;
        cout << "����������, ������� �� ����� ������� ��� �����...";
        _getch();
        system("cls");
        return  admin(); // ������������ � ������� ����
    }
    else
    {
        cout << "\n�������� ����� ��� ������. ���������� ��� ��� ��� ������� 0 ��� ������." << endl;
        return enter_admin(); // ��������� ���� ��� ��������������
    }
}

int enter_main_admin()//���� �������� ��������������
{
    cout << "+-----------------------------------------------+\n";
    cout << "|         ���� �������� ��������������          |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "������� �����: ";
    cin >> login;
    cout << "������� ������: ";
    const int size_password = 65;
    char password[size_password] = { 0 };
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
    }
    while (true)
    {
        if (checkSeniorAdminCredentials(login, password))
        {
            cout << "\n�� ����� ��� ������� �������������!" << endl;
            _getch();
            system("cls");
            main_admin_menu();
            break;
        }
        else
        {
            cout << "\n�������� ����� ��� ������. ���������� ��� ���." << endl;
            return menu();
        }
    }
}
