#include "auditable.h"

struct courier_location
{
    int x, y;
};

enum class courier_status
{
    available,
    busy,
    offline
};

struct courier_zone
{
    float radius;
    courier_location center;
};

class timetable
{
private:
public:
    timetable() {}
};

class courier : public auditable
{
private:
    string first_name, last_name, phone_number, email;
    int id, max_capacity;
    courier_location location;
    courier_status status;
    courier_zone zone;
    timetable schedule;
    vector<node> route;

public:
    courier(string first_name, string last_name, string phone_number, string email, int id, int max_capacity, courier_location location, courier_zone zone)
    {
        this->first_name = first_name;
        this->last_name = last_name;
        this->phone_number = phone_number;
        this->email = email;
        this->id = id;
        this->max_capacity = max_capacity;
        this->location = location;
        this->status = courier_status::available;
        this->zone = zone;
        this->schedule = timetable();
    }

    bool is_available()
    {
        return status == courier_status::available;
    }

    int get_id()
    {
        return id;
    }

    courier_location get_location()
    {
        return location;
    }
};

vector<courier> initiate_vehicles(string vehicles_path)
    {
        vector<courier> vehicles_list;

        ifstream file(vehicles_path);
        if (!file.is_open())
        {
            cerr << "Failed to open file!" << endl;
            throw runtime_error("Failed to open file");
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        try
        {
            json data = json::parse(jsonContent);

            auto vehicles = data["vehicles"];
            for (const auto &vehicle : vehicles)
            {
                int id = vehicle["id"];
                string first_name = vehicle["first_name"];
                // string last_name = vehicle["last_name"];
                // string phone_number = vehicle["phone_number"];
                // string email = vehicle["email"];
                int max_capacity = vehicle["max_capacity"];
                courier_location location = {vehicle["location"]["x"], vehicle["location"]["y"]};
                // courier_zone zone = {vehicle["zone"]["radius"], {vehicle["zone"]["center"]["x"], vehicle["zone"]["center"]["y"]}};

                courier new_courier = {first_name, "last_name", "phone_number", "email", id, max_capacity, location, courier_zone()};
                vehicles_list.push_back(new_courier);
            }
        }
        catch (json::parse_error &e)
        {
            cerr << "Failed to parse JSON: " << e.what() << endl;
            throw runtime_error("Failed to parse JSON");
        }

        return  vehicles_list;
    }