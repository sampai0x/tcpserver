#include "request_handler.h"

void handle_client(int new_socket){
    char buffer[BUFFER_SIZE] = { 0 };

    read(new_socket, buffer, BUFFER_SIZE);

    if(strstr(buffer, "GET /") !=  NULL && strstr(buffer, "GET /favicon.ico") == NULL){
        printf("Connection accepted!");
        printf("Request: %s\n", buffer);
    }
    if(strstr(buffer, "GET /favicon.ico") != NULL){
        close(new_socket);
        return;
    }

    char* html_content = serve_html("../html/index.html");

    if(html_content){
        const char* keys[] = { "user", "is_logged_in"};
        const char* values[] {"Dexter", "1" };
        const char* loop_key = "item";
        const char* loop_values[] = { "Item 1", "Item 2", "Item 3", "Item 4" };

        char* processed_template = process_template(html_content, keys, values, 2, loop_key, loop_values, 4);

        write(new_socket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n", 48);

        write(new_socket, processed_template, strlen(processed_template));

        free(html_content);
        free(processed_template);
    }else{
        const char* not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
        write(new_socket, not_found_response, strlen(not_found_response));
    }

    close(new_socket);
}