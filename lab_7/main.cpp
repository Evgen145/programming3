#include "test_runner.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
#include <optional>
#include <sstream>
#include <stdexcept>

using namespace std;

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302,
};

ostream& operator<<(ostream& output, HttpCode code) {
    switch (code) {
        case HttpCode::Ok:
            output << "200 OK";
            break;
        case HttpCode::Found:
            output << "302 Found";
            break;
        case HttpCode::NotFound:
            output << "404 Not found";
            break;
        default:
            throw invalid_argument("Unknown HTTP code");
    }
    return output;
}

struct HttpHeader {
    string name, value;
};

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

ostream& operator<<(ostream& output, const HttpHeader& header) {
    return output << header.name << ": " << header.value;
}

class HttpResponse {
public:
    explicit HttpResponse(HttpCode code) : code(code) {}

    HttpResponse& AddHeader(string name, string value) {
        headers.push_back({move(name), move(value)});
        return *this;
    }

    HttpResponse& SetContent(string content) {
        this->content = move(content);
        return *this;
    }

    HttpResponse& SetCode(HttpCode code) {
        this->code = code;
        return *this;
    }

    bool operator==(const HttpResponse& other) const {
        return code == other.code &&
               headers == other.headers &&
               content == other.content;
    }

    friend ostream& operator<<(ostream& output, const HttpResponse& resp) {
        output << "HTTP/1.1 " << resp.code << "\n";
        for (const auto& header : resp.headers) {
            output << header << "\n";
        }
        if (!resp.content.empty()) {
            output << "Content-Length: " << resp.content.size() << "\n";
        }
        return output << "\n" << resp.content;
    }

private:
    HttpCode code;
    vector<HttpHeader> headers;
    string content;
};

struct HttpRequest {
    string method, path, body;
    map<string, string> get_params;
};

pair<string, string> SplitBy(const string& what, const string& by) {
    size_t pos = what.find(by);
    if (by.size() < what.size() && pos < what.size() - by.size()) {
        return {what.substr(0, pos), what.substr(pos + by.size())};
    } else {
        return {what, {}};
    }
}

template<typename T>
T FromString(const string& s) {
    T x;
    istringstream is(s);
    is >> x;
    return x;
}

pair<size_t, string> ParseIdAndContent(const string& body) {
    auto [id_string, content] = SplitBy(body, " ");
    return {FromString<size_t>(id_string), content};
}

struct LastCommentInfo {
    size_t user_id, consecutive_count;
};

class CommentServer {
public:
    HttpResponse ServeRequest(const HttpRequest& req) {
        if (req.method == "POST") {
            if (req.path == "/add_user") {
                return ServeAddUser();
            } else if (req.path == "/add_comment") {
                return ServeAddComment(req);
            } else if (req.path == "/check_captcha") {
                return ServeCheckCaptcha(req);
            }
        } else if (req.method == "GET") {
            if (req.path == "/user_comments") {
                return ServeUserComments(req);
            } else if (req.path == "/captcha") {
                return ServeCaptcha();
            }
        }
        return HttpResponse(HttpCode::NotFound);
    }

private:
    HttpResponse ServeAddUser() {
        comments_.emplace_back();
        return HttpResponse(HttpCode::Ok).SetContent(to_string(comments_.size() - 1));
    }

    HttpResponse ServeAddComment(const HttpRequest& req) {
        auto [user_id, comment] = ParseIdAndContent(req.body);
        if (!last_comment || last_comment->user_id != user_id) {
            last_comment = {user_id, 1};
        } else if (++last_comment->consecutive_count > 3) {
            banned_users.insert(user_id);
        }

        if (banned_users.count(user_id) == 0) {
            comments_[user_id].push_back(comment);
            return HttpResponse(HttpCode::Ok);
        } else {
            return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
        }
    }

    HttpResponse ServeCheckCaptcha(const HttpRequest& req) {
        auto [id, response] = ParseIdAndContent(req.body);
        if (response == "42") {
            banned_users.erase(id);
            return HttpResponse(HttpCode::Ok);
        }
        return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
    }

    HttpResponse ServeUserComments(const HttpRequest& req) {
        auto user_id = FromString<size_t>(req.get_params.at("user_id"));
        string response;
        for (const auto& comment : comments_[user_id]) {
            response += comment + "\n";
        }
        return HttpResponse(HttpCode::Ok).SetContent(move(response));
    }

    HttpResponse ServeCaptcha() {
        return HttpResponse(HttpCode::Ok)
            .SetContent("What's the answer for The Ultimate Question of Life, the Universe, and Everything?");
    }

    vector<vector<string>> comments_;
    optional<LastCommentInfo> last_comment;
    unordered_set<size_t> banned_users;
};

// Тестування
void TestHttpResponse() {
    HttpResponse response(HttpCode::Ok);
    response.AddHeader("Header1", "Value1").SetContent("Body").SetCode(HttpCode::Found);

    ostringstream output;
    output << response;

    ASSERT_EQUAL(output.str(),
                 "HTTP/1.1 302 Found\n"
                 "Header1: Value1\n"
                 "Content-Length: 4\n\n"
                 "Body");
}

void TestCommentServer() {
    CommentServer server;

    HttpRequest add_user_request;
    add_user_request.method = "POST";
    add_user_request.path = "/add_user";
    auto response = server.ServeRequest(add_user_request);
    ASSERT_EQUAL(response, HttpResponse(HttpCode::Ok).SetContent("0"));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestHttpResponse);
    RUN_TEST(tr, TestCommentServer);
    return 0;
}