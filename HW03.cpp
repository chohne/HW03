#include <curl/curl.h>
#include <json/json.h>

#include <string>
#include <iostream>
using namespace std;

// Controls how the data is written to the string buffer
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    //284dda4ebd3f4fb2b34182603251503

    // Get API key from the user
    string apiKey;
    cout << "Input a valid API key: ";
    cin >> apiKey;
    const string URL = "http://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=Dayton,TN";

    // Create curl object
    CURL* handle = curl_easy_init();
    string str_buffer;

    // Set options for curl object
    curl_easy_setopt(handle, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &str_buffer);
    //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);

    // Attempt to get data
    CURLcode result = curl_easy_perform(handle);
    if (result != CURLE_OK) {
        cerr << "curl_easy_perform() failed with error:" << curl_easy_strerror(result) << endl;
        return -1;
    }
    curl_easy_cleanup(handle);

    // Convert the data string into a JSON value
    Json::Reader reader;
    Json::Value root;

    bool success = reader.parse(str_buffer, root);
    if (!success) {
        cerr << "parse() failed with error(s): " << reader.getFormattedErrorMessages() << endl;
        return -1;
    }

    // Output the data in a readable format    
    Json::Value current = root["current"];

    cout << "Last Updated: " << current["last_updated"].asString() << endl;
    cout << "Temperature: " << current["temp_f"] << " degrees F" << endl;
    cout << "Feels Like: " << current["feelslike_f"] << " degrees F" << endl;
    cout << "Current Weather: " << current["condition"]["text"].asString() << endl;
    cout << "Rainfall: " << current["precip_in"] << " inches" << endl;
    cout << "Cloud Cover: " << current["cloud"] << "%" << endl;
    
    return 0;
}