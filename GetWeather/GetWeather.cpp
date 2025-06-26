#include "GetWeather.h"
#include "json.hpp"

using json = nlohmann::json;

std::string server_url = "192.168.2.1:5000";

int getWeather(City &city) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    std::string url = server_url + "/get_current_weather";
    json payload = {
            {"lat", city.lat},
            {"lon", city.lon}
    };
    std::string payload_str = payload.dump();
    std::string response;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload_str.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char *ptr, size_t size, size_t nmemb, void *userdata) -> size_t {
        std::string *resp = static_cast<std::string*>(userdata);
        resp->append(ptr, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cerr << "Get weather data success." << std::endl;
    }
    std::cerr << "Freeing headers and cleaning up CURL." << std::endl;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    std::cerr << "Transforming JSON response to separated values." << std::endl;
    // JSON -> Separated values
    json weather_data = json::parse(response);

    city.weather = weather_data[0][4];
    double temperature = weather_data[0][6];
    city.temperature = temperature - 273.15; // Convert from Kelvin to Celsius
    city.advice = weather_data[1][0];

    return 0;
}

int getLocation(City &city) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    std::string url = server_url + "/search_city";
    json payload = {
            {"city", city.city_name},
    };
    std::string payload_str = payload.dump();
    std::string response;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload_str.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char *ptr, size_t size, size_t nmemb, void *userdata) -> size_t {
        std::string *resp = static_cast<std::string*>(userdata);
        resp->append(ptr, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cerr << "Get location data success." << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // JSON -> Separated values
    json weather_data = json::parse(response);
    double lat = weather_data[0]["lat"];
    double lon = weather_data[0]["lon"];
    // keep 4 decimal places
    city.lat = std::round(lat * 10000.0) / 10000.0;
    city.lon = std::round(lon * 10000.0) / 10000.0;

    return 0;
}

