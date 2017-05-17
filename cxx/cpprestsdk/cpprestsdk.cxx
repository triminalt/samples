//
// triminalt AT gmail DOT com
// 2017-05-15
// initial
//


#include <iostream>
#include <cpprest\http_client.h>
#include <cpprest\filestream.h>
#include "cpprestsdk.hxx"
#include <chrono>

// cpprestsdk pplx
using concurrency::streams::ostream;
using concurrency::streams::istream;
using concurrency::streams::fstream;

using utility::size64_t;

// cpprestsdk web
using web::http::http_headers;
using web::http::header_names;
using web::http::http_request;
using web::http::http_response;
using web::http::uri_builder;
using web::http::methods;
using web::http::client::http_client;
using web::http::details::mime_types;


namespace {
size64_t GetLength(istream const& const is) {
    is.seek(0, std::ios::end);
    auto const length = static_cast<size64_t>(is.tell());
    is.seek(0, 0);
    return length;
}

size64_t GetLength(ostream const& const os) {
    os.seek(0, std::ios::end);
    auto const length = static_cast<size64_t>(os.tell());
    os.seek(0, 0);
    return length;
}

static auto const PostServer = U("http://posttestserver.com/post.php");
static auto const Boundary = mime_types::boundary + U("=-=+#CPPRESTSDK#+=-");
}

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
            std::clog
                << "BingSearch code: "
                << response.status_code() << std::endl;
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
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - begin).count();
        std::clog
            << "BingSearch, request.wait end ..."
            << ", cost: " << cost << std::endl;
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
    auto request = fstream::open_ostream(U("quanmin.json"))
        .then([=] (ostream os) {
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
        std::clog
            << "GetQuanminLiveList code: "
            << response.status_code() << std::endl;
        return response.body().read_to_end(os_ptr->streambuf());
    })
    // Close the file stream.
   .then([=] (size_t) {
        return os_ptr->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try {
        std::clog
            << "GetQuanminLiveList, request.wait begin ..."
            << std::endl;
        auto begin = std::chrono::steady_clock::now();
        request.wait();
        auto end = std::chrono::steady_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - begin).count();
        std::clog << "GetQuanminLiveList, request.wait end ..."
            << ", cost: " << cost << std::endl;
    } catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::system("pause");
}

void DumpTrash(std::string const& const file) {
    // http://https://www.posttestserver.com/

    std::clog << "DumpTrash, entering ..." << std::endl;
    
    auto is_ptr = std::make_shared<istream>();

    // Open stream to output file.
    auto task_is = fstream::open_istream(U("E:\\git\\samples\\samples\\cxx\\cpprestsdk\\wangfei.qrc"));
    auto request = task_is.then([=] (istream is) {
            std::clog << "DumpTrash, request sending ..." << std::endl;
            *is_ptr = is;
            
            auto builder = uri_builder();
            //builder.append_path(U("cpprestsdk"));
            builder.append_query(U("cpprestsdk"), U("abc123"));

            auto request = http_request();
            auto headers = request.headers();
            headers.add(  header_names::content_type
                        , mime_types::multipart_form_data);
                        //+ U("; ")
                        //+ Boundary);
            /*
            headers.add(  header_names::content_disposition
                        , mime_types::form_data
                        + U("name=text"));
            */
            request.set_body(  *is_ptr
                             , GetLength(*is_ptr)
                             , mime_types::multipart_form_data);
            auto uploader = http_client(PostServer);
            return uploader.request(methods::POST);
    })
    // Handle response headers arriving.
    .then([=] (http_response response) {
        std::clog << "DumpTrash, response received ..." << std::endl;
        response.extract_utf8string();
        std::clog
            << "DumpTrash code: "
            << response.status_code() << std::endl;
            // task base c++
        auto task_os_response = fstream::open_ostream(U("response.html"));
        task_os_response.then([=](ostream os) {
            return response.body().read_to_end(os.streambuf());
        });
        task_os_response.wait();
        return response.extract_utf8string();
    })
    // Print the body of the respose
    .then([] (std::string const& txt) {
        std::clog << "DumpTrash txt: " << txt << std::endl;
    })
    // Close the file stream.
    .then([=] () {
        return is_ptr->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try {
        std::clog
            << "DumpTrash, request.wait begin ..."
            << std::endl;
        auto begin = std::chrono::steady_clock::now();
        request.wait();
        auto end = std::chrono::steady_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - begin).count();
        std::clog << "DumpTrash, request.wait end ..."
            << ", cost: " << cost << std::endl;
    } catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::system("pause");
}