#pragma once
#include <iostream>
#include <string>

using namespace std;

struct date
{
public:
    int day, month, year;
    date(int d = 0, int m = 0, int y = 0000) : day(d), month(m), year(y) {}
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
    bool isEmpty() const
    {
        return (day == 0 && month == 0 && year == 0);
    }
};

ostream& operator<<(ostream& os, const date& d)//перегрущка оператора << для даты
{
    os << d.day << " " << d.month << " " << d.year;
    return os;
}

bool operator>=(const date& lhs, const date& rhs) //перегрузка оператора 
{
    if (lhs.year != rhs.year) return lhs.year >= rhs.year;
    if (lhs.month != rhs.month) return lhs.month >= rhs.month;
    return lhs.day >= rhs.day;
}

bool operator==(const date& lhs, const date& rhs) {
    return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
}

bool operator<=(const date& lhs, const date& rhs) 
{
    return rhs >= lhs;
}

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
      bool operator==(const Time& other) const {
          return hour == other.hour && minute == other.minute;
      }
};

class Person
{
public:
    string name;
    string surname;
    string phone;
    Person() {};
    Person( const string& name, const string& surname, const string& phone) :
        name(name), surname(surname), phone(phone) {}

    void setName(const string& newName) { name = newName; }
    const string& getName() const { return name; }

    void setSurname(const string& newSurname) { surname = newSurname; }
    const string& getSurname() const { return surname; }

    void setPhone(const string& newPhone) { phone = newPhone; }
    const string& getPhone() const { return phone; }
};

class Admin : public Person
{ 
public:
    string login;
    string password;
    date birthday;
    Admin() {};
    Admin(const string& login, const string& name, const string& surname, const string& phone, const date& birthday)
        :login(login), Person(name, surname, phone), birthday(birthday) {}

    void setLogin(const string& newLogin) { login = newLogin; }
    const string& getlogin() const { return login; }

    void setPassword(const string& newPassword) { password = newPassword; }
    const string& getPassword() const { return password; }

    void setBirthday(const date& newBirthday) { birthday = newBirthday; }
    const date& getBirthday() const { return birthday; }
};

class Client : public Person
{  
public:
    date appointmentDate;  // дата записи
    Time appointmentTime;  // время записи
    string login;
    string password;
    date birthday;
    string review; //отзыв
    Client() {};

    Client(const string& login, const string& name, const string& surname, const string& phone, const date&birthday)
        :login(login), Person(name, surname, phone), birthday(birthday), password(""), appointmentDate(), appointmentTime(), review("") {}
    
    Client(const string& login,  const string& name, const string& surname, const string& phone,
        const date& birthday, const date& appointmentDate, const Time& appointmentTime) :
        login(login), Person(name, surname, phone),
        birthday(birthday), appointmentDate(appointmentDate), appointmentTime(appointmentTime){}

    void setLogin(const string& newLogin) { login = newLogin; }
    const string& getlogin() const { return login; }
   
    void setPassword(const string& newPassword) { password = newPassword; }
    const string& getPassword() const { return password; }

    void setBirthday(const date& newBirthday) { birthday = newBirthday; }
    const date& getBirthday() const { return birthday; }
   
    void setAppointmentDate(const date& newDate) { appointmentDate = newDate; }
    const date& getAppointmentDate() const { return appointmentDate; }

    void setAppointmentTime(const Time& newTime) { appointmentTime = newTime; }
    const Time& getAppointmentTime() const { return appointmentTime; }

    void setReview(const string& newReview) { review = newReview; }
    const string& getReview() const { return review; }

    void saveContactInfo(ofstream& file) const
    {
        file << "Логин: " << getlogin() << "\n";
        file << "Имя: " << getName() << "\n";
        file << "Фамилия: " << getSurname() << "\n";
        file << "Номер телефона: " << getPhone() << "\n";
    }
};

struct master
{
public:
    string name;
    string surname;
};

class service
{
private:
    
public:
    string name;
    string info;
    int price;
    int duration;
    master serviceMaster;
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
        cout << "+------------------------------+------------------------------+" << endl;
        cout << "| " << setw(28) << left << name << " |" << setw(29) << left << info << " |" << setw(20) << left << price << " |" << endl;
    }
};

class Request {
public:
    string login;
    string password;
    string name;
    string surname;
    string phone;
    date birthday;

    Request(const string& login, const string& password, const string& name, const string& surname, const string& phone, const date& birthday)
        : login(login), password(password), name(name), surname(surname), phone(phone), birthday(birthday) {}
    
    string toString() const 
    {
        // преобразование полей объекта в строку
        return "Логин: " + login + "\n"+ "Пароль: " + password + "\n"; 
    }
    
    void setLogin(const string& newLogin) { login = newLogin; }
    const string& getLogin() const { return login; }

    void setPassword(const string& newPassword) { password = newPassword; }
    const string& getPassword() const { return password; }

    void setBirthday(const date& newBirthday) { birthday = newBirthday; }
    const date& getBirthday() const { return birthday; }

    void setName(const string& newName) { name = newName; }
    const string& getName() const { return name; }

    void setSurname(const string& newSurname) { surname = newSurname; }
    const string& getSurname() const { return surname; }

    void setPhone(const string& newPhone) { phone = newPhone; }
    const string& getPhone() const { return phone; }
};

bool operator==(const Request& lhs, const Request& rhs)
{
    return lhs.login == rhs.login;
}

struct shedule
{
    date appointmentDate;
    Time appointmentTime;
    bool isBooked;
    master Master;
    service selectedService;
    shedule(date date, Time time, master Master, const service& selectedService)
        : appointmentDate(date), appointmentTime(time), isBooked(false), Master(Master), selectedService(selectedService) {}
};

struct ProcedureInfo {
    string name;
    int count;
};
