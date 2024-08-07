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
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

bool checkLoginExists(const string& login)//проверка логина на уникальность
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
        cout << "Ошибка открытия файла для чтения." << endl;
    }

    return exists;
}

bool checkUserCredentials(const string& login, const string& password)
{
    ifstream file("user_credentials.txt"); // файл с логинами и паролями пользователей
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
        cout << "Ошибка открытия файла для чтения." << endl;
    }
    return isCorrect;
}

bool checkAdminCredentials(const string& login, const string& password)
{
    ifstream file("admin_credentials.txt"); // файл с логинами и паролями администраторов
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
        cout << "Ошибка открытия файла для чтения." << endl;
    }

    return isCorrect;
}


// функция для сохранения логина, соли и хеша пароля в файл
void saveSeniorAdminCredentials(const string& login, const string& password) {
   
    ofstream file("senior_admin_credentials.txt", ios::app); // файл с логином и хешем пароля старшего администратора
    if (file.is_open()) {
        file << login << " " << encryptPass(password) << endl; // сохраняем логин
        file.close();
    }
    else {
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

// функция для проверки логина и пароля старшего администратора
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
                return true; // пароль верный
            }
            else {
                file.close();
                return false; // пароль неверный
            }
        }
    }
    if (!found) {
        cout << "Пользователь с таким логином не найден." << endl;
    }
    file.close();
    return false; 
}

void saveUserCredentials(const string& login, const string& password)
{
    ofstream file("user_credentials.txt", ios::app); // файл с логинами и паролями пользователей
    if (file.is_open())
    {
        file << login << " " << encryptPass(password) << endl;
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

