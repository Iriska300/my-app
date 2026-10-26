#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    std::cout << "Server listening on port 5000..." << std::endl;
    
    const char* app_version = std::getenv("APP_VERSION");
    if (!app_version) app_version = "dev";

    std::string json_response = "{\"status\":\"ok\", \"version\":\"" + std::string(app_version) + "\", \"service\":\"server (C++)\"}";
    std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(json_response.length()) + "\r\n\r\n" + json_response;

    while (true) {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket < 0) continue;

        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::string req(buffer);
        
        if (req.find("GET /api/status") != std::string::npos) {
            send(new_socket, http_response.c_str(), http_response.length(), 0);
        } else if (req.find("GET /healthz") != std::string::npos) {
            std::string ok_resp = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok";
            send(new_socket, ok_resp.c_str(), ok_resp.length(), 0);
        } else {
            std::string not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(new_socket, not_found.c_str(), not_found.length(), 0);
        }
        close(new_socket);
    }
    return 0;
}
