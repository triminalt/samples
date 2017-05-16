//
// triminalt AT gmail DOT com
// 2017-05-15
// initial
//


#include <iostream>
#include <cpprest\http_client.h>
#include <cpprest\filestream.h>
#include <experimentalcppcorecheck\warnings.h>
#include <cppcorecheck\warnings.h>
#include "cpprestsdk.hxx"

using concurrency::streams::ostream;
using concurrency::streams::fstream;
using web::http::client::http_client;
using web::http::http_response;
using web::http::uri_builder;
using web::http::methods;

void BingSearch() {
    std::clog << "BingSearch, entering ..." << std::endl;
    auto os_ptr = std::make_shared<ostream>();

    // Open stream to output file.
    auto request = fstream::open_ostream(U("google.html"))
        // Send the GET request.
        .then([=] (ostream os) {
            std::clog << "BingSearch, request sending ..." << std::endl;
            *os_ptr = os;
            http_client client(U("http://www.bing.com"));
            uri_builder builder(U("/search"));
            builder.append_query(U("q"), U("cpprestsdk"));
            return client.request(methods::GET, builder.to_string());
        })
        // Handle response headers arriving.
        .then([=](http_response response) {
            std::clog << "BingSearch, response received ..." << std::endl;
            response.extract_utf8string();
            std::clog << "BingSearch code: " << response.status_code() << std::endl;
            return response.body().read_to_end(os_ptr->streambuf());
        })
        // Close the file stream.
        .then([=] (size_t) {
            return os_ptr->close();
        });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try {
        std::clog << "BingSearch, request.wait begin ..." << std::endl;
        auto begin = std::chrono::steady_clock::now();
        request.wait();
        auto end = std::chrono::steady_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::clog << "BingSearch, request.wait end ..." << ", cost: " << cost << std::endl;
    }
    catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::system("pause");
}

void GetQuanminLiveList() {
    // http://www.quanmin.tv/json/play/list.json?_t=201705170006

    std::clog << "GetQuanminLiveList, entering ..." << std::endl;
    auto os_ptr = std::make_shared<ostream>();

    // Open stream to output file.
    auto request = fstream::open_ostream(U("quanmin.json")).then(
        [=] (ostream os) {
        std::clog << "GetQuanminLiveList, request sending ..." << std::endl;
        *os_ptr = os;
        http_client client(U("http://www.quanmin.tv"));
        uri_builder builder(U("/json/play/list.json"));
        builder.append_query(U("t"), U("122333"));
        return client.request(methods::GET, builder.to_string());
    })
        // Handle response headers arriving.
        .then([=] (http_response response) {
        std::clog << "GetQuanminLiveList, response received ..." << std::endl;
        response.extract_utf8string();
        std::clog << "GetQuanminLiveList code: " << response.status_code() << std::endl;
        return response.body().read_to_end(os_ptr->streambuf());
    })
        // Close the file stream.
        .then([=] (size_t) {
        return os_ptr->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try {
        
        std::clog << "GetQuanminLiveList, request.wait begin ..." << std::endl;
        auto begin = std::chrono::steady_clock::now();
        request.scheduler();
        auto end = std::chrono::steady_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::clog << "GetQuanminLiveList, request.wait end ..." << ", cost: " << cost << std::endl;
    } catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::system("pause");
}

void DumpTrash() {

}