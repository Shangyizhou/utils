#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class WeightRobin {
public:
    WeightRobin() = default;
    void Init(std::vector<std::pair<std::string, int>>);
    std::string GetNext();
    

private:
    // typedef std::unordered_map<std::string, int> ServiceMap;
    int gcd(int a, int b);
    int GetGcd();
    int GetMaxWeight();

    int total;
    int current_pos;
    int current_weight;
    std::vector<std::pair<std::string, int>> services;
};

int WeightRobin::gcd(int a, int b) {
    int temp = 0;
    while (b) {
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

int WeightRobin::GetGcd() {
    auto it = services.begin();
    int res = it->second;

    for (const auto it : services) {
        res = gcd(res, it.second);
    }

    return res;
}

int WeightRobin::GetMaxWeight() {
    int max_weight = 0;

    for (const auto& it : services) {
        max_weight = std::max(max_weight, it.second);
    }

    return max_weight;
}


void WeightRobin::Init(std::vector<std::pair<std::string, int>> services) {
    current_pos = -1;
    this->services = services;
    total = services.size();
}

std::string WeightRobin::GetNext() {
    int index = current_pos;
    while (true) {
        index = (index + 1) % total;

        if (0 == index) {
            current_weight -= GetGcd();

            if (current_weight <= 0) {
                current_weight = GetMaxWeight();
            }
        }

        if (services[index].second >= current_weight) {
            current_pos = index;
            return services[index].first;
        }
    }
}

int main() {
    std::vector<std::pair<std::string, int>> services = {
        {"192.168.10.1:2202", 6},
        {"192.168.10.2:2202", 2},
        {"192.168.10.3:2202", 1},
        {"192.168.10.4:2202", 1}
    };

    WeightRobin weight_robin;
    weight_robin.Init(services);
    for (int i = 0; i < 20; i++) {
        std::string res = weight_robin.GetNext();
        std::cout << res << std::endl;
    }
    
    return 0;
}

// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.2:2202
// 192.168.10.1:2202
// 192.168.10.2:2202
// 192.168.10.3:2202
// 192.168.10.4:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.1:2202
// 192.168.10.2:2202
// 192.168.10.1:2202
// 192.168.10.2:2202
// 192.168.10.3:2202
// 192.168.10.4:2202