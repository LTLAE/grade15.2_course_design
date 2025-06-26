#include "GetWeather.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <city_name>" << std::endl;
        return 1;
    }
    City city;
    city.city_name = argv[1];
    if (getLocation(city) != 0) {
        std::cerr << "Failed to get location for city: " << city.city_name << std::endl;
        return 1;
    }
    if (getWeather(city) != 0) {
        std::cerr << "Failed to get weather for city: " << city.city_name << std::endl;
        return 1;
    }

    std::cout << city.city_name << ","
              << city.lat << ","
              << city.lon << ","
              << city.weather << ","
              << city.temperature << ","
              << city.advice << std::endl;

}

/*
int main(){
    City city;
    city.city_name = "shanghai";
    getLocation(city);
    getWeather(city);

    std::cout << "City: " << city.city_name << std::endl;
    std::cout << "Latitude: " << city.lat << std::endl;
    std::cout << "Longitude: " << city.lon << std::endl;
    std::cout << "Weather: " << city.weather << std::endl;
    std::cout << "Temperature: " << city.temperature << " °C" << std::endl;
    std::cout << "Advice: " << city.advice << std::endl;

    return 0;
}
*/