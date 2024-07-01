#include "socket.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

using JsonObj = nlohmann::json;

typedef std::unordered_map<std::string, std::string> ParamMap;

enum HttpStatusCode{
    OK, //200
    CREATED, //201
    NOT_FOUND, //404
    METHOD_NOT_ALLOWED, //405 
    CONFLICT, //409,
    BAD_REQUEST //400
};

enum HttpRequestType{
    GET,
    POST,
    PUT,
    DELETE,
    UNKOWN
};

enum HttpContentType {
        json,
        html
};

struct HttpRequest {
    HttpRequestType type;
    std::string url;
    ParamMap urlParams;
    ParamMap headers;
    JsonObj jsonBody;
};

struct HttpResponse {
    
    HttpResponse(){}

    HttpResponse(HttpStatusCode statusCode, const std::string& body = "", HttpContentType contentType = html):
        statusCode(statusCode), body(body) {
            std::string typeStr;

            switch(contentType) {
                case json: {
                    typeStr = "application/json";
                    break;
                }
                case html: {
                    typeStr = "text/html";
                    break;
                }
            }

            headers.push_back("Content-Type: " + typeStr);
        }

    std::vector<std::string> headers;
    HttpStatusCode statusCode;
    std::string body;
};

class HttpConnection: public Socket 
{
public:
    HttpConnection(const int port);

    HttpRequest receive();

    void respond(HttpResponse response);

    void respond(HttpStatusCode statusCode, const std::string& body = "");

private:
    std::string getStatusCodeString(HttpStatusCode statusCode);

    HttpRequestType parseRequestType(const std::string& typeStr);

    ParamMap parseHeaders(const std::string& msg);

    JsonObj parseJsonBody(const std::string& msg);

    ParamMap parseURLEncodedBody(const std::string& msg);

    ParamMap parseParamStr(const std::string& paramStr);

    std::vector<std::string> splitStr(const std::string& str, const std::string& delimiter);
};