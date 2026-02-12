#pragma once
#include <string>

class ConfigState {
public:
    static ConfigState& instance() {
        static ConfigState inst;
        return inst;
    }

    void setIp(const std::string& ip) { ipAddress = ip; }
    const std::string& ip() const { return ipAddress; }

    void setSsid(const std::string& s) { connectedSsid = s; }
    const std::string& ssid() const { return connectedSsid; }

private:
    ConfigState() = default;

    std::string ipAddress;
    std::string connectedSsid;
};
