//
//  main.cpp
//  aplicatie_zboruri
//
//  Created by Alexa Iordache on 26.11.2021.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include <ctime>

using namespace std;

//clasa pentru zboruri: aici retinem de unde se pleaca, destinatia, data plecarii, cat costa in total (cu tot cu taxe) si daca este rezervat sau nu
// putem sa daugam zboruri
class Flight
{
public:
    string startingPoint;
    string destination;
    string date;
    float fare;
    int routeID;
    string booked;

public:
    void setFlight(int routeID, string startingPoint, string destination, string date, float fare, string booked);

    //getters
    int getRouteID();
    string getStartingPoint();
    string getDestination();
    string getDate();
    string getBooked();
    float getFare();

    //functie pentru a ne afisa zborurile
    void displayRoute();
};

void Flight::setFlight(int routeID, string startingPoint, string destination, string date, float fare, string booked)
{
    this->routeID = routeID;
    this->startingPoint = startingPoint;
    this->destination = destination;
    this->date = date;
    this->fare = fare;
    this->booked = booked;
}

int Flight::getRouteID()
{
    return this->routeID;
}

string Flight::getStartingPoint()
{
    return this->startingPoint;
}

string Flight::getDestination()
{
    return this->destination;
}

string Flight::getDate()
{
    return this->date;
}

float Flight::getFare()
{
    return this->fare;
}

string Flight::getBooked()
{
    return this->booked;
}

void Flight::displayRoute()
{
    std::cout << "Route[" << this->getRouteID() << "]: " << this->getStartingPoint() << " to " << this->getDestination() << endl;
    std::cout << "Date: " << this->getDate() << endl;
    std::cout << "Total fare: " << this->getFare() << endl;
}

//class for users
class User
{
public:
    string username;
    string password;
    string encrytedPassword;

public:
    void setUser(string username, string password)
    {
        this->username = username;
        this->password = password;
    }

    void setEncryptedPassword(string encrytedPassword)
    {
        this->encrytedPassword = encrytedPassword;
    }

    string getUsername()
    {
        return this->username;
    }

    string getPassword()
    {
        return this->password;
    }

    string getEncryptedPassword()
    {
        return this->encrytedPassword;
    }

    void dispayUserInfo()
    {
        std::cout << "Username: " << this->getUsername() << endl;
        std::cout << "Password: " << this->getPassword() << endl;
        std::cout << endl;
    }
};

//friend function - replaces the flights in the file with the updated info (if a flight is booked, this information
// have to appear in the file as well)
void replace (Flight flights[], int nrFlights)
{
    fstream dataIn;

    dataIn.open("dataIN.txt", ios::out);
    if (dataIn.is_open())
    {
        for (int i = 0; i < nrFlights; i++)
        {
            dataIn << flights[i].routeID << " " << flights[i].startingPoint << " " << flights[i].destination << " " << flights[i].date << " " << flights[i].fare << " " << flights[i].booked << endl;
        }
    }
    dataIn.close();
}


//function - used by the operator to mark a flight as booked (it means that that flight is not available anymore)
void deleteFlight (Flight flights[], int nrFlights) {
    int routeID;

    cout << " Give the route ID of the flight that you want not to be available anymore: " << endl;
    cin >> routeID;
    flights[routeID -1].booked = "yes";

    replace (flights, nrFlights);
}

// friend function - checks if there are some available flights or not
void checkFlightAvailibility(Flight flights[], int nrFlights, string departurePoint, string destinationPoint, string date, int maximFare)
{
    int found = 0;
    int var = 0;
    string answer;

    for (int i = 0; i < nrFlights; i++)
    {

        if (flights[i].getStartingPoint() == departurePoint && flights[i].getDestination() == destinationPoint && flights[i].getDate() == date && flights[i].getFare() <= maximFare && flights[i].booked == "no")
        {
            found = 1;
            var = i;
        }
    }

    try
    {
        if (found != 1)
        {
            throw "There are no available flights.";
        }
        cout << "There are some flights for you: " << endl;
        cout << endl;
        flights[var].displayRoute(); //display the available route
        cout << endl;
        cout << "Do you want to book this flight? (yes/no)" << endl;
        cin >> answer;
        flights[var].booked = "yes"; //if the user wants to book the displayed flight, then that flight will be booked
                                     // found = 1;
    }
    catch (const char *msg)
    {
        cout << msg << endl;
    }

    // we change the file if a flight was booked (to keep the file updated)
    replace(flights, nrFlights);
}

// algorithm that returns the greatest common factor of the numbers a and b
int cmmdc(int a, int b)
{
    int rest;
    while (1)
    {
        rest = a % b;
        if (rest == 0)
            return b;
        a = b;
        b = rest;
    }
}

// RSA algorithm for words
void RSA_forWords(char str[])
{
    int i;

    //criptare
    for (i = 0; (i < 100 && str[i] != '\0'); i++)
        str[i] = str[i] + 2;

    // cout << "Encrypted string: " << str << endl;

    //decriptare
    // for (i = 0; (i < 100 && str[i] != '\0'); i++)
    //     str[i] = str[i] - 2;

    // cout << "Decrypted string: " << str << endl;
}

//friend function - checks if the authentication was successful or not
int checkAuthenticity(string nameEntered, string passwordEntered)
{
    int wrong = 0;
    string name, password;
    fstream dataEncrypted;

    // we encrypt the password entered from the keyboard
    int nr_letters = passwordEntered.length();
    char encryptedPassword[nr_letters + 1];
    strcpy(encryptedPassword, passwordEntered.c_str());
    RSA_forWords(encryptedPassword);

    dataEncrypted.open("dataENCRYPTED.txt", ios::in);
    if (dataEncrypted.is_open())
    {
        //we read each password in the encrypted password file
        while (dataEncrypted >> name >> password)
        {
            // if the username entered on the keyboard matches the username in the file and the entered encrypted password
            // matches the encrypted password of that user, then that user already exists
            if (name == nameEntered && encryptedPassword == password)
            {
                wrong = 1;
                return wrong;
            }
            else
            {
                wrong = 0;
            }
        }
    }
    return wrong;
}

//friend function - checks if the operator tried to log in
int checkBeingOperator(string name, string password)
{
    int isOperator;
    if (name == "Operator" && password == "operator123")
    {
        isOperator = 1; //it is the operator
    }
    else
    {
        isOperator = 0; //it is not the operator
    }

    return isOperator;
}

// function that verifies if a city exists in the file of european cities or not
int verifyCity(string city)
{
    string line;
    fstream dataCities;
    int found = 0;

    dataCities.open("europeanCITIES.txt", ios::in);
    if (dataCities.is_open())
    {
        while (getline(dataCities, line))
        {
            if (city == line)
            {
                found = 1;
            }
        }
    }
    return found;
}

// function that transforms the date into an array of 3 integers
void dateToNumbers(string date, int arrayOfNumbers[])
{
    // we transform the date into a char
    string arrayOfWords[5];
    int nr_letters = date.length();
    char copyDate[nr_letters + 1];
    strcpy(copyDate, date.c_str());
    int i = 0;

    // we take each part of the date separately
    char *ptr;
    ptr = strtok(copyDate, "/");
    while (ptr != NULL)
    {
        arrayOfWords[i++] = ptr;
        ptr = strtok(NULL, "/");
    }

    for (int i = 0; i <= 3; i++)
    {
        stringstream geek(arrayOfWords[i]);
        geek >> arrayOfNumbers[i];
    }
}

// function that verifies if the date is valid or not
void verifyDate(string date)
{
    int arrayOfNumbers[5];
    dateToNumbers(date, arrayOfNumbers);

    time_t now = time(0);
    tm *dt = localtime(&now);

    try
    {

        // if the year is one of the past years
        if (arrayOfNumbers[2] < (1900 + (dt->tm_year)))
        {
            throw 1;
        }

        // if it's the current year, we check if the month has passed
        if (arrayOfNumbers[2] == (1900 + (dt->tm_year)))
        {
            if (arrayOfNumbers[1] < (dt->tm_mon + 1))
            {
                throw 1;
            }

            // if it is the current month, we check if the day has passed
            if (arrayOfNumbers[1] == (dt->tm_mon + 1))
            {
                if (arrayOfNumbers[0] < dt->tm_mday)
                {
                    throw 1;
                }
            }
        }

        // if the year is one of the next years, we check if the month and day are both valid
        if (arrayOfNumbers[1] <= 0 || arrayOfNumbers[1] > 12)
        {
            throw 1;
        }

        // if the month has 31 days 
        if (arrayOfNumbers[1] == 1 || arrayOfNumbers[1] == 3 || arrayOfNumbers[1] == 5 || arrayOfNumbers[1] == 7 || arrayOfNumbers[1] == 8 || arrayOfNumbers[1] == 10 || arrayOfNumbers[1] == 12)
        {
            if (arrayOfNumbers[0] <= 0 || arrayOfNumbers[0] >= 31)
            {
                throw 1;
            }
        }

        // if the month is february
        if (arrayOfNumbers[0] == 2)
        {
            if (arrayOfNumbers[0] <= 0 || arrayOfNumbers[0] > 29 )
            {
                throw 1;
            }
        }

        // if the month has 30 days
        if (arrayOfNumbers[0] <= 0 || arrayOfNumbers[0] >= 30)
            {
                throw 1;
            }

    }
    catch (int x)
    {
        cout << " Invalid date." << endl;
        exit(-1);
    }
}

//frined function - adds a new flight
void addNewFlight(Flight flights[], int nrFlights)
{
    int routeId, nr_letters, i = 0;
    float fare;
    string startingPoint, destination, date, isBooked;
    string arrayOfWords[5];
    int arrayOfNumbers[5];
    fstream dataIn;

    try
    {
        cout << "Add a new flight: " << endl;
        cin >> routeId >> startingPoint >> destination >> date >> fare >> isBooked;

        if (verifyCity(startingPoint) != 1)
        {
            throw 1;
        }

        if (verifyCity(destination) != 1)
        {
            throw 1;
        }

        dateToNumbers(date, arrayOfNumbers);
        verifyDate(date);

        flights[nrFlights++].setFlight(routeId, startingPoint, destination, date, fare, isBooked);

        dataIn.open("dataIN.txt", ios::app);
        if (dataIn.is_open())
        {
            dataIn << routeId << " " << startingPoint << " " << destination << " " << date << " " << fare << " " << isBooked << endl;
            dataIn.close();
        }
    }
    catch (...)
    {
        cout << "City does not exist." << endl;
        exit(1);
    }
}

//frined function - adds a new user
void createAccount(User users[], int nrUsers)
{
    string name, password, passwordRepeted;
    fstream dataUsers, dataEncrypted;

    cout << "You can now create you new account" << endl;
    cout << endl;

    try
    {
        cout << "Enter you name: ";
        cin >> name;
        cout << "Enter a password: ";
        cin >> password;

        // if it is a weak password
        if (password.length() <= 3)
        {
            throw 1;
        }

        // the user has to enter the password twice
        cout << "Enter yhe password again: ";
        cin >> passwordRepeted;

        // if the second password does not match the first
        if (passwordRepeted != password)
        {
            throw 'a';
        }
        users[nrUsers].setUser(name, password);

        // we add the user in the file with decrypted passwords
        dataUsers.open("dataUSERS.txt", ios::app);
        if (dataUsers.is_open())
        {
            dataUsers << name << " " << password << endl;
            dataUsers.close();
        }

        // we add the user in the file with the encrypted passwords
        dataEncrypted.open("dataENCRYPTED.txt", ios::app);
        if (dataEncrypted.is_open())
        {
            // we transform the string into char
            int nr_letters = password.length();
            char encryptedPassword[nr_letters + 1];
            strcpy(encryptedPassword, password.c_str());

            // we apply on the char the RSA algortihm
            RSA_forWords(encryptedPassword);
            users[nrUsers].setEncryptedPassword(encryptedPassword);
            dataEncrypted << name << " " << encryptedPassword << endl;
        }

        nrUsers++;
    }
    catch (int x)
    {
        try
        {
            cout << "Password too weak. Please try another password." << endl;
            cout << "Enter a password: ";
            cin >> password;

            cout << "Enter yhe password again: ";
            cin >> passwordRepeted;

            if (passwordRepeted != password)
            {
                throw 'a';
            }
            users[nrUsers].setUser(name, password);

            // we add the user in the file with decrypted passwords
            dataUsers.open("dataUSERS.txt", ios::app);
            if (dataUsers.is_open())
            {
                dataUsers << name << " " << password << endl;
                dataUsers.close();
            }

            // we add the user in the file with the encrypted passwords
            dataEncrypted.open("dataENCRYPTED.txt", ios::app);
            if (dataEncrypted.is_open())
            {
                // we transform the string into char
                int nr_letters = password.length();
                char encryptedPassword[nr_letters + 1];
                strcpy(encryptedPassword, password.c_str());

                // we apply on the char the RSA algortihm
                RSA_forWords(encryptedPassword);
                users[nrUsers].setEncryptedPassword(encryptedPassword);
                dataEncrypted << name << " " << encryptedPassword << endl;
            }
            nrUsers++;
        }
        catch (char y)
        {
            cout << "Please try again. " << endl;
            cout << "Enter the password again: ";
            cin >> passwordRepeted;

            if (password != passwordRepeted)
            {
                exit(1);
            }
            else
            {
                users[nrUsers].setUser(name, password);

                // we add the user in the file with decrypted passwords
                dataUsers.open("dataUSERS.txt", ios::app);
                if (dataUsers.is_open())
                {
                    dataUsers << name << " " << password << endl;
                    dataUsers.close();
                }

                // we add the user in the file with the encrypted passwords
                dataEncrypted.open("dataENCRYPTED.txt", ios::app);
                if (dataEncrypted.is_open())
                {
                    // we transform the string into char
                    int nr_letters = password.length();
                    char encryptedPassword[nr_letters + 1];
                    strcpy(encryptedPassword, password.c_str());

                    // we apply on the char the RSA algortihm
                    RSA_forWords(encryptedPassword);
                    users[nrUsers].setEncryptedPassword(encryptedPassword);
                    dataEncrypted << name << " " << encryptedPassword << endl;
                }

                nrUsers++;
            }
        }
    }
    catch (char x)
    {
        cout << "Please try again. " << endl;
        cout << "Enter the password again: ";
        cin >> passwordRepeted;

        if (password != passwordRepeted)
        {
            exit(1);
        }
        else
        {
            users[nrUsers].setUser(name, password);

            // we add the user in the file with decrypted passwords
            dataUsers.open("dataUSERS.txt", ios::app);
            if (dataUsers.is_open())
            {
                dataUsers << name << " " << password << endl;
                dataUsers.close();
            }

            // we add the user in the file with the encrypted passwords
            dataEncrypted.open("dataENCRYPTED.txt", ios::app);
            if (dataEncrypted.is_open())
            {
                // we transform the string into char
                int nr_letters = password.length();
                char encryptedPassword[nr_letters + 1];
                strcpy(encryptedPassword, password.c_str());

                // we apply on the char the RSA algortihm
                RSA_forWords(encryptedPassword);
                users[nrUsers].setEncryptedPassword(encryptedPassword);
                dataEncrypted << name << " " << encryptedPassword << endl;
            }

            nrUsers++;
        }
    }
}

//frined function - displays all the flights (available or not)
void displayAllFlights(Flight flights[], int nrFlights)
{
    cout << "All the flights are: " << endl;
    for (int i = 0; i < nrFlights; i++)
    {
        cout << flights[i].routeID << " " << flights[i].startingPoint << " " << flights[i].destination << " " << flights[i].date << " " << flights[i].fare << " " << flights[i].booked << endl;
    }
}

//frined function - diplays all the users with the encrypted passwords
void displayAllUsers(User users[], int nrUsers)
{
    cout << "All the users are: " << endl;
    for (int i = 0; i < nrUsers; i++)
    {
        cout << users[i].username << " " << users[i].encrytedPassword << endl;
    }
}

int main()
{

    int nrFlights = 0;
    int nrUsers = 0;
    int i, maximFare, routeId, fare;
    string startingPoint, destination, date;
    string destinationPoint, departurePoint;
    string name, password, username, isBooked;
    string answer;
    int existingAccount;
    int arrayOfNumbers[50];

    string arrayOfCities[50];
    int nrCities = 0;

    fstream dataIn, dataUsers, dataEncrypted;

    Flight flights[100];
    User users[100];

    // we read the info about flights
    dataIn.open("dataIN.txt", ios::in);
    if (!dataIn)
    {
        cout << "File 'dataIN.txt' could not be opened" << endl;
        exit(1);
    }
    else
    {
        while (dataIn >> routeId >> startingPoint >> destination >> date >> fare >> isBooked)
        {
            flights[nrFlights].setFlight(routeId, startingPoint, destination, date, fare, isBooked);
            nrFlights++;
        }
        dataIn.close();
    }

    // we read the info abou users
    dataUsers.open("dataUSERS.txt");
    if (!dataUsers)
    {
        cout << " File 'dataUSERS.txt' could not be opened" << endl;
        exit(1);
    }
    else
    {
        while (dataUsers >> name >> password)
        {
            users[nrUsers].setUser(name, password);
            nrUsers++;
        }
        dataUsers.close();
    }

    // we make the file with the encrypted password using the file with the information about users
    dataEncrypted.open("dataENCRYPTED.txt", ios::out);
    dataUsers.open("dataUSERS.txt", ios::in);

    if (!dataUsers || !dataEncrypted)
    {
        cout << " File 'dataUSERS.txt' or 'dataENCRYPTED.txt' could not be opened" << endl;
        exit(1);
    }
    else
    {
        while (dataUsers >> name >> password)
        {
            // we transform the string into char
            int nr_letters = password.length();
            char encryptedPassword[nr_letters + 1];
            strcpy(encryptedPassword, password.c_str());

            // we apply on the char the RSA algortihm
            RSA_forWords(encryptedPassword);
            dataEncrypted << name << " " << encryptedPassword << endl;
        }
        dataUsers.close();
        dataEncrypted.close();
    }

    // we add the encrypted passwords to the users atributes
    dataEncrypted.open("dataENCRYPTED.txt", ios::in);
    string encryptedPassword;
    i = 0;

    if (!dataEncrypted)
    {
        cout << " File 'dataENCRYPTED.txt' could not be opened" << endl;
        exit(1);
    }
    else
    {
        while (dataEncrypted >> name >> encryptedPassword)
        {
            users[i++].setEncryptedPassword(encryptedPassword);
        }
        dataEncrypted.close();
    }

    // ----------------------------------------------------------
    // log in part

    cout << "Do you want to log in as an operator or as an user? (operator/ user)" << endl;
    cin >> answer;
    cout << endl;

    // if it is the operator
    if (answer == "operator")
    {
        cout << "Please login" << endl;
        cout << "Enter username: ";
        cin >> name;
        cout << "Enter password: ";
        cin >> password;

        cout << endl;
        cout << "Checking authenticity ... " << endl;
        cout << endl;

        //if the operators logs in, the he can add a new flight
        try
        {
            existingAccount = checkBeingOperator(name, password);
            if (existingAccount != 1)
            {
                throw "Authentication failed!";
            }
            cout << "Successful operator authentication" << endl;
            cout << endl;

            cout << "Do you want to add a new flight? (yes/no)" << endl;
            string answer;
            cin >> answer;
            if (answer == "yes")
            {
                addNewFlight(flights, nrFlights);
            }

            cout << "do yoy want to delete a flight? (yes/no) " << endl;
            cout << endl;
            cin >> answer;
            if (answer == "yes")
            {
                deleteFlight (flights, nrFlights);
            }

            exit(1);
        }
        catch (const char *msg)
        {
            cout << msg << endl;
            exit(1);
        }
    }

    // if it is not the operator, then la algorithm checks if the user already exists in the database or nor
    else
    {
        cout << "Please login" << endl;
        cout << "Enter username: ";
        cin >> name;
        cout << "Enter password: ";
        cin >> password;

        cout << endl;
        cout << "Checking authenticity ... " << endl;
        cout << endl;

        try
        {
            existingAccount = checkAuthenticity(name, password);
            if (existingAccount != 1)
            {
                throw "Authentication failed!";
            }

            cout << "Successful authentication!" << endl;
            cout << endl;
        }
        catch (const char *msg)
        {
            cout << msg << endl;
            cout << "Do you want to create a new account? (yes/ no)";
            cin >> answer;
            if (answer == "yes")
            {
                createAccount(users, nrUsers);
            }
            else
            {
                cout << endl;
                exit(1);
            }
        }
    }

    // ----------------------------------------------------------
    // booking flights part

    cout << "Where do you want to fly from? ";
    cin >> departurePoint;

    // we make sure the departure point and destination are valid cities (using the file with the european cities)
    if (verifyCity(departurePoint) != 1)
    {
        cout << "City not found." << endl;
        exit(1);
    }

    cout << "Where do you want to go? ";
    cin >> destinationPoint;

    if (verifyCity(destinationPoint) != 1)
    {
        cout << "City not found." << endl;
        exit(1);
    }

    cout << "When do you want to leave? (day/mounth/year): ";
    cin >> date;

    // we ckeck if the date introduced by the user is a valid one or not
    verifyDate(date);

    cout << "Which is the maximum price you would like to pay? ";
    cin >> maximFare;
    cout << endl;

    //after the users gives the information about the desired flight, we check if there is a flight that matches his
    //preferences or not
    cout << "Lookin for flights ... " << endl;
    cout << endl;

    checkFlightAvailibility(flights, nrFlights, departurePoint, destinationPoint, date, maximFare);

    return 0;
}