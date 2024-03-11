#include <iostream>
#include <fstream>
#include <list>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "auditable.cpp"

using namespace std;
using json = nlohmann::json;

boost::uuids::uuid generate_uuid()
{
    return boost::uuids::random_generator()();
}

struct location
{
    string address;
};

struct courier_zone {
    float radius;
    location center;
};

class courier : public auditable
{
private:
    string firstname, lastname, email, phone, addres;
    boost::uuids::uuid uuid;
    int id;
    bool available;
    location current_location;
    delivery current_delivery;
    timetable timetable;

    void change_availability()
    {
        this->available = !this->available;
    }

    void change_location(location new_location)
    {
        this->current_location = new_location;
    }

    void set_zone(courier_zone zone)
    {
        this->current_location = zone.center;
    }

public:
    courier(string firstname, string lastname, string email, string phone, string addres)
    {
        this->id = 0;
        this->firstname = firstname;
        this->lastname = lastname;
        this->email = email;
        this->phone = phone;
        this->addres = addres;
        this->uuid = generate_uuid();
        this->available = true;
    }

    bool is_available()
    {
        return this->available;
    }

    location get_location()
    {
        return this->current_location;
    }
};

class timetable
{
private:

public:
    timetable()
    {
    }
};

class delivery{
private:
    // vector<node> path;

public:
    delivery()
    {
    }
};

int main()
{
    cout << "Hello, World!" << endl;
    return 0;
}