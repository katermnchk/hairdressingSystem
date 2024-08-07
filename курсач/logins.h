#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <limits>
#include <iomanip> 
#include <conio.h>
#include <fstream>

using namespace std;

void writeLoginToFile(const string& login)
{
    ofstream file("logins.txt", ios::app);
    if (file.is_open())
    {
        file << login << endl;
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

bool checkLoginExists(const string& login)//�������� ������ �� ������������
{
    ifstream file("logins.txt");
    string line;
    bool exists = false;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line == login)
            {
                exists = true;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }

    return exists;
}

bool checkUserCredentials(const string& login, const string& password)
{
    ifstream file("user_credentials.txt"); // ���� � �������� � �������� �������������
    string storedLogin, storedPassword;
    bool isCorrect = false;
    if (file.is_open())
    {
        while (file >> storedLogin >> storedPassword)
        {
            if (storedLogin == login && decryptPass(storedPassword) == password)
            {
                isCorrect = true;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }
    return isCorrect;
}

bool checkAdminCredentials(const string& login, const string& password)
{
    ifstream file("admin_credentials.txt"); // ���� � �������� � �������� ���������������
    string storedLogin, storedPassword;
    bool isCorrect = false;

    if (file.is_open())
    {
        while (file >> storedLogin >> storedPassword)
        {
            if (storedLogin == login && decryptPass(storedPassword)  == password)
            {
                isCorrect = true;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }

    return isCorrect;
}


// ������� ��� ���������� ������, ���� � ���� ������ � ����
void saveSeniorAdminCredentials(const string& login, const string& password) {
   
    ofstream file("senior_admin_credentials.txt", ios::app); // ���� � ������� � ����� ������ �������� ��������������
    if (file.is_open()) {
        file << login << " " << encryptPass(password) << endl; // ��������� �����
        file.close();
    }
    else {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

// ������� ��� �������� ������ � ������ �������� ��������������
bool checkSeniorAdminCredentials(const string& login, const string& enteredPassword) 
{
    ifstream file("senior_admin_credentials.txt");
    string savedLogin, savedPassword;
    bool found = false;
    while (file >> savedLogin >>  savedPassword) 
    {
        if (savedLogin == login) {
            found = true;
           
            if (enteredPassword == decryptPass(savedPassword))
            {
                file.close();
                return true; // ������ ������
            }
            else {
                file.close();
                return false; // ������ ��������
            }
        }
    }
    if (!found) {
        cout << "������������ � ����� ������� �� ������." << endl;
    }
    file.close();
    return false; 
}

void saveUserCredentials(const string& login, const string& password)
{
    ofstream file("user_credentials.txt", ios::app); // ���� � �������� � �������� �������������
    if (file.is_open())
    {
        file << login << " " << encryptPass(password) << endl;
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

