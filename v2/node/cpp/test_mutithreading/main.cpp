#include<iostream>
#include<fstream>
#include<vector>
#include <random>
#include <algorithm>

#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex m;
bool flag = true;
vector<int> v; 

vector<int> read_file() {
    vector<int> v;
    ifstream file("file.txt");
    int x;
    while (file >> x) {
        v.push_back(x);
    }
    return v;
}

void print_vector(vector<int> &v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void randomize(std::vector<int> &v) {
    random_device rd;
    mt19937 gen(rd());

    unique_lock<mutex> lock(m);
    shuffle(v.begin(), v.end(), gen);
    lock.unlock();
}

vector<int> initialize() {
    vector<int> v;
    v.reserve(10);

    return read_file();
}

void run(vector<int> &v) {
    while (flag) {
        randomize(v);
        // print_vector(v);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void wait_for_input() {
    while (flag) {
        int n;
        cout << "scrie weeee ";
        cin >> n;
        cout << endl;

        if (n == 0) {
            flag = false;
            break;
        }

        unique_lock<mutex> lock(m);
        v.push_back(n);
        lock.unlock();
    }
}

int main() {
    v = initialize();

    thread t1(run, ref(v));
    thread t2(wait_for_input);

    t1.join();
    t2.join();

    cout << "GATTAAAAAA" << endl;
    print_vector(v);

    return 0;
}