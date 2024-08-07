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
    bool loginFound = false; // ���������� ��� ������������ ������� ������
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� ������
                loginFound = true; // ����, ��� ����� ������
                getline(file, line); // ������� � ��������� ������ (������)
                file.close();
                return decryptPass(line.substr(8)); //�������� ������:  � ������ ������
            }
        }
        file.close();
    }
    if (!loginFound)
    {
        // ���� ����� �� ������ � �����
        return "";
    }
    // ���� ���� ����������, �� ����� ��� ������ 
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
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� ���
                for (int i = 0; i < 2; ++i) getline(file, line); // ��������� � ������ � ������
                file.close();
                return line.substr(5); // ������� ���: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������ 
}

string getSurname(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� �������
                for (int i = 0; i < 3; ++i) getline(file, line); // ��������� � ������ � ������� ��������
                file.close();
                return line.substr(9); // �������� �������: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������ 
}

string getPhone(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                //������ �����, ��������� ����� ��������
                for (int i = 0; i < 4; ++i) getline(file, line); // ������� � ������ � ������� ��������
                file.close();
                return line.substr(9); // �������� �������: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������
}

date getBirthday(const string& login)
{
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� ���� ��������
                for (int i = 0; i < 5; ++i) getline(file, line); // ������� � ������ � ����� ��������
                file.close();
                date birthday;
                sscanf_s(line.c_str(), "���� ��������: %d %d %d", &birthday.day, &birthday.month, &birthday.year);
                return birthday;
            }
        }
        file.close();
    }
    return {}; // ���� ����� �� ������ 
}

void saveAdminCredentials(const string& login, const string& password)
{
    ofstream file("admin_credentials.txt", ios::app); // ���� � �������� � �������� �������
    if (file.is_open())
    {
        cout << "������ ��������. ����� ������������� ��������." << endl;
        file << endl << login << " " << encryptPass(password) << endl; // ���������� ����� � ������
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
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
        cout << "������ �������� ����� ��� ������ ������." << endl;
    }
}

void displayAdminRequests()//������� �� ����� ������ ������
{
    vector<string> requests;
    readAdminRequests(requests);
    bool has = false;//���� �� ������
    for (const auto& request : requests)
    {
        if (request.find("�����: ") != string::npos)
        {
            has = true;
            break;
        }
    }
    if (!has)
    {
        cout << "+---------------------------+\n";
        cout << "|��� ������ ��� ������������|" << endl;
        cout << "+---------------------------+\n" << endl;
        return;
    }
    else
    {
        cout << "+-------------------------+\n";
        cout << "|      ������ ������      |\n";
        cout << "+-------------------------+\n";
        for (int i = 0; i < requests.size(); ++i)
        {
            if (requests[i].find("�����: ") != string::npos)
            {
                string login = requests[i].substr(7); // ����� ����� �����: 
                string name, surname;

                if (i + 2 < requests.size() && requests[i + 2].find("���: ") != string::npos)
                {
                    name = requests[i + 2].substr(5); // ��� ����� ���: 
                }
                if (i + 3 < requests.size() && requests[i + 3].find("�������: ") != string::npos)
                {
                    surname = requests[i + 3].substr(9); // ������� ����� �������: 
                }

                cout << "�����: " << login << "\n���: " << name << "\n�������: " << surname << endl;
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
            if (!skipNextLines && allRequests[i].find("�����: " + login) != string::npos)
            {
                // ������� ������, ��������������� ������
                skipNextLines = true;
                lines = 6; // ���������� 6 �����
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
        rename("admin_requests_tmp.txt", "admin_requests.txt");//������� ������ ���� �����
    }
    else
    {
        cout << "������ �������� ����� ��� ������ ������." << endl;
    }
}

void approveAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    // ��������� ������ �������������� � ���� � �������� ������� ���������������
    saveAdminCredentials(login, password);
    // ������� ������ �� ������ ������
    removeAdminRequest(login, password);
}

void addAdminRequest(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
{
    ofstream file("admin_requests.txt", ios::app);
    if (file.is_open())
    {
        file << "�����: " << login << "\n";
        file << "������: " << encryptPass(password) << "\n";
        file << "���: " << name << "\n";
        file << "�������: " << surname << "\n";
        file << "�������: " << phone << "\n";
        file << "���� ��������: " << birthday.day << " " << birthday.month << " " << birthday.year << "\n";
        file << "\n"; // ������ ������ ��� ���������� ������
        file.close();
    }
    else
    {
        cout << "������ ���������� ������." << endl;
    }
}
