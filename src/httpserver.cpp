#include "httpserver.h"

HttpServer::HttpServer(const int port):
    httpConnection(port)
{

}

void HttpServer::addGetHandler(const std::string& url, std::function<HttpResponse(ParamMap)> handler)
{
    getHandlers.insert({url, handler});
}

void HttpServer::addPostHandler(const std::string& url, std::function<HttpResponse(ParamMap, JsonObj)> handler)
{
    postHandlers.insert({url, handler});
}

void HttpServer::addPutHandler(const std::string& url, std::function<HttpResponse(ParamMap, JsonObj)> handler)
{
    putHandlers.insert({url, handler});
}

void HttpServer::addDeleteHandler(const std::string& url, std::function<HttpResponse(ParamMap)> handler)
{
    deleteHandlers.insert({url, handler});
}

void HttpServer::start() 
{
    while(true){ 
        if(!httpConnection.connect()){
            std::cout << "failed to connect\n";
            return;
        }

        HttpRequest request = httpConnection.receive();

        HttpResponse response;

        switch(request.type) {
            case GET: {
                if(getHandlers.find(request.url) != getHandlers.end()){ 
                    auto handler = getHandlers[request.url];
                    response = handler(request.urlParams);
                }
                else response = HttpResponse(NOT_FOUND);
                break;
            }
            case POST: {
                if(postHandlers.find(request.url) != postHandlers.end()){
                    auto handler = postHandlers[request.url];

                    response = handler(request.urlParams, request.jsonBody);
                }
                else response = HttpResponse(NOT_FOUND);
                break;
            }
            case PUT: {
                if(putHandlers.find(request.url) != putHandlers.end()){
                    auto handler = putHandlers[request.url];
                    response = handler(request.urlParams, request.jsonBody);
                }
                else response = HttpResponse(NOT_FOUND);
                break;
            }
            case DELETE: {
                if(deleteHandlers.find(request.url) != deleteHandlers.end()){
                    auto handler = deleteHandlers[request.url];
                    response = handler(request.urlParams);
                }
                else response = HttpResponse(NOT_FOUND);
                break;
            }
            case UNKOWN:
            default: {
                response = HttpResponse(METHOD_NOT_ALLOWED);
                break;
            }
        }       

        httpConnection.respond(response);
        httpConnection.close();
    }
}