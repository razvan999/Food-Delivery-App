#include <string>
#include <ctime>
#include <vector>

using namespace std;

struct audit_log
{
    string action;
    time_t timestamp;
};

class auditable {
private:
    vector<audit_log> logs;

    void log_action(string action) {
        audit_log log;
        log.action = action;
        log.timestamp = time(0);
        logs.push_back(log);
    }
public:
    auditable() {}
    ~auditable() {}
    
    vector<audit_log> get_logs() {
        return logs;
    }
};