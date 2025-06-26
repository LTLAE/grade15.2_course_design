#include <iostream>
#include <string>
#include <curl/curl.h>

class City {
public:
    double lat = 0.0;
    double lon = 0.0;
    std::string city_name = "";
    std::string weather = "";
    double temperature = 0.0;
    std::string advice = "";
};

int getWeather(City &city);

int getLocation(City &city);
