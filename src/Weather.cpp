#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>

using json = nlohmann::json;

// Function to handle the cURL response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int main() {
    // Step 1: Choose a Weather API Provider
    std::string api_key = "Cb4xlZWAYgewVwOlhEAbVmnCgOIDxMOP";
    std::string lat = "42.3478";
    std::string lon = "-71.0466";

    // Step 3: Integrate the API into Your Application
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "https://api.tomorrow.io/v4/weather/forecast?location=" + lat + "," + lon + "&apikey=" + api_key;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Step 4: Parse and Display the Weather Data
            json jsonData = json::parse(response_data);

            // Extract and print temperature and precipitation intensity
            double temperature = jsonData["data"]["timelines"][0]["intervals"][0]["values"]["temperature"].get<double>();
            double precipitation_intensity = jsonData["data"]["timelines"][0]["intervals"][0]["values"]["precipitationIntensity"].get<double>();

            std::cout << "Temperature: " << temperature << " C" << std::endl;
            std::cout << "Precipitation Intensity: " << precipitation_intensity << " mm/h" << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
