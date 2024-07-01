#include "httpconnection.h"
#include <map>
class HttpServer
{
public:
    HttpServer(const int port);

    void addGetHandler(const std::string& url, std::function<HttpResponse(ParamMap)> handler);

    void addPostHandler(const std::string& url, std::function<HttpResponse(ParamMap, JsonObj)> handler);

    void addPutHandler(const std::string& url, std::function<HttpResponse(ParamMap, JsonObj)> handler);

    void addDeleteHandler(const std::string& url, std::function<HttpResponse(ParamMap)> handler);

    void start();

private:
    HttpConnection httpConnection;

    std::map<std::string, std::function<HttpResponse(ParamMap)>> getHandlers;
    std::map<std::string, std::function<HttpResponse(ParamMap, JsonObj)>> postHandlers;
    std::map<std::string, std::function<HttpResponse(ParamMap, JsonObj)>> putHandlers;
    std::map<std::string, std::function<HttpResponse(ParamMap)>> deleteHandlers;
  
};