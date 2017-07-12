#include <iostream>
#include <regex>
#include <vector>
#include <string>

enum TYPE{
    ADDRESS,
    ADDRESS_2,
    PHONE,
    STATE,
    POSTAL,
    COUNTRY
};

struct Regex{
    std::regex reg_;
    TYPE type_;

    Regex(std::regex reg, TYPE type) : reg_{reg}, type_{type}{}
    Regex(const Regex &obj) :
        reg_{obj.reg_}, type_{obj.type_}{}
};

struct Status{
    bool business = false;
    bool address = false;
    bool street = false;
    bool city = false;
    bool state = false;
    bool postal = false;
    bool country = false;
    bool area_code = false;
    bool phone_number = false;
};

std::smatch m;
std::vector<Regex> regs = {
    Regex(std::regex("\\((\\d{3})\\) (.*)"), PHONE),// (212) 986-8080
    Regex(std::regex("\\+(\\d+) (.*)"), PHONE),   // +299 32 42 22
    Regex(std::regex("(\\d+-\\d+).(.*)"), ADDRESS), // 24-38 Tudor City Pl
    Regex(std::regex("(\\d+) (.*)"), ADDRESS),      // 633 3rd Ave
    Regex(std::regex("(.*) (\\d+)"), ADDRESS_2),    // Platz der Einheit 14
    Regex(std::regex("(.*), (.*)"), STATE),         // New York, NY
    Regex(std::regex("(\\d+)"), POSTAL),    // 10017 | USA
    Regex(std::regex("(\\w+)"), COUNTRY),    // 10017 | USA
};

bool match(const std::string &str, Status &stats){
    std::vector<std::string>::iterator it;
    for(auto x : regs){
        if(std::regex_search(str, m, x.reg_)){
            switch(x.type_){
                case ADDRESS:
                    if(stats.address || stats.street)
                        continue;
                    std::cout << "address       = " << m[1] << std::endl;
                    std::cout << "street        = " << m[2] << std::endl;
                    stats.address = stats.street = true;
                    break;
                case ADDRESS_2:
                    if(stats.address || stats.street)
                        continue;
                    std::cout << "address       = " << m[2] << std::endl;
                    std::cout << "street        = " << m[1] << std::endl;
                    stats.address = stats.street = true;
                    break;
                case PHONE:
                    if(stats.phone_number || stats.area_code)
                        continue;
                    std::cout << "area_code     = " << m[1] << std::endl;
                    std::cout << "phone_number  = " << m[2] << std::endl;
                    stats.phone_number = stats.area_code = true;
                    break;
                case STATE:
                    if(stats.state)
                        continue;
                    std::cout << "city          = " << m[1] << std::endl;
                    std::cout << "state         = " << m[2] << std::endl;
                    stats.city = stats.state = true;
                    break;
                case POSTAL:
                    if(stats.postal)
                        continue;
                    std::cout << "postal        = " << m[1] << std::endl;
                    stats.postal = true;
                    break;
                case COUNTRY:
                    if(!stats.business){
                        std::cout << "business      = " << str << std::endl;
                        stats.business = true;
                        return true;
                    }
                    if(!stats.address){
                        std::cout << "address       = " << str << std::endl;
                        stats.address = true;
                        return true;
                    }
                    if(!stats.street){
                        std::cout << "street        = " << str << std::endl;
                        stats.street = true;
                        return true;
                    }
                    if(stats.country)
                        continue;
                    std::cout << "country       = " << m[1] << std::endl;
                    stats.country = true;
                    break;
            }
            return true;
        }
    }
    return false;
}

int main(){
    std::string str;
    std::deque<std::string> vec;
    Status stats;
    while(std::getline(std::cin, str)){
        vec.push_back(str);
    }
    while(!vec.empty()){
        if(!match(vec.front(), stats)){
            std::cout << ">>> BAD : " << vec.front() << std::endl;
        }
        vec.pop_front();
    }
}
