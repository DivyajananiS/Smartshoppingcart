#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

string processOrder() {
    cout << "Order Received -> Processing Payment -> Confirmed -> Out for Delivery" << endl;
    return "✅ Order Confirmed! 🚚 Out for Delivery";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "🚀 Server running at http://localhost:8080" << endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        char buffer[30000] = {0};
        read(new_socket, buffer, 30000);

        string request(buffer);

        string response;

        if (request.find("POST /checkout") != string::npos) {
            response = processOrder();
        } else {
            response = "❌ Invalid Endpoint";
        }

        string http = 
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/plain\n"
            "Access-Control-Allow-Origin: *\n\n" 
            + response;

        send(new_socket, http.c_str(), http.size(), 0);
        close(new_socket);
    }
}
