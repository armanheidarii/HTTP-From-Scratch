#include <string>
#include <vector>
#include <fstream>
#include "httpserver.h"

JsonObj getSensors(const std::string& filename) {
    std::ifstream i(filename);
    JsonObj j;
    i >> j;
    i.close();

    return j;
}

void saveSensors(const std::string& filename, JsonObj j) {
    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}

int main() 
{
    const std::string filename = "sensors.json";

    const int port = 8080;
    HttpServer server(port);

    server.addGetHandler("/", [](ParamMap urlParams){
        std::string helloWorldHTML = "<h1>Hello World</h1>";

        return HttpResponse(HttpStatusCode::OK, helloWorldHTML, HttpContentType::html);
    });

    server.addGetHandler("/sensor", [filename](ParamMap urlParams){
        if(!urlParams.count("id"))
            return HttpResponse(HttpStatusCode::BAD_REQUEST, "missing id parameter");

        std::string id = urlParams["id"];

        JsonObj sensors = getSensors(filename);

        if(!sensors.contains(id))
            return HttpResponse(HttpStatusCode::NOT_FOUND, "Not Found");

        return HttpResponse(HttpStatusCode::OK, sensors[id].dump(), HttpContentType::json);
    });

    server.addPostHandler("/sensor", [filename](ParamMap urlParams, JsonObj jsonBody){
        if(!jsonBody.contains("id"))
            return HttpResponse(HttpStatusCode::BAD_REQUEST, "missing id parameter");

        JsonObj sensors = getSensors(filename);

        std::string id = std::to_string(static_cast<int>(jsonBody["id"]));

        if(sensors.contains(id))
             return HttpResponse(HttpStatusCode::CONFLICT, "Sensor already exists"); 

        sensors[id]["values"] = nullptr;

        saveSensors(filename, sensors);

        return HttpResponse(HttpStatusCode::CREATED, sensors.dump(), HttpContentType::json);
    });

    server.addPutHandler("/sensor", [filename](ParamMap urlParams, JsonObj jsonBody){
        if(!jsonBody.contains("id"))
            return HttpResponse(HttpStatusCode::BAD_REQUEST, "missing id parameter");

        JsonObj sensors = getSensors(filename);

        std::string id = std::to_string(static_cast<int>(jsonBody["id"]));

        if(!sensors.contains(id))
            return HttpResponse(HttpStatusCode::NOT_FOUND, "Could not find sensor with id '" + id + "'");

        jsonBody.erase("id");

        sensors[id]["values"] = jsonBody;
        
        saveSensors(filename, sensors);

        return HttpResponse(HttpStatusCode::OK, sensors.dump(), HttpContentType::json);
    });

    server.addDeleteHandler("/sensor", [filename](ParamMap urlParams){
        if(!urlParams.count("id"))
            return HttpResponse(HttpStatusCode::BAD_REQUEST, "missing id parameter");
        
        JsonObj sensors = getSensors(filename);

        std::cout << sensors << "\n";

        std::string id = urlParams["id"];

       if(!sensors.contains(id))
            return HttpResponse(HttpStatusCode::NOT_FOUND, "Could not find sensor with id '" + id + "'");

        sensors.erase(id);

        saveSensors(filename, sensors);

        return HttpResponse(HttpStatusCode::OK, sensors.dump(), HttpContentType::json);
    });

    server.start();

    return 0;
}