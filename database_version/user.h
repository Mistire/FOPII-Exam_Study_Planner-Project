#ifndef USER_H
#define USER_H

#include <string>
#include <memory>
#include <iostream>
#include <openssl/sha.h>
#include <cppconn/prepared_statement.h>

using std::string;

struct User {
    
    User(string username, string password) : _user_id(createUniqueId(username)), _username(username), _password(password) {}

    string getTableName() { return "users"; }
    int size() { return 3; }
    void setParams(std::unique_ptr<sql::PreparedStatement> pstmt) {
        pstmt->setString(1, this->_user_id);
        pstmt->setString(2, this->_username);
        pstmt->setString(3, this->_password);
    }

    void setUpdateParams(std::unique_ptr<sql::PreparedStatement> pstmt) {
        pstmt->setString(1, this->_username);
        pstmt->setString(2, this->_password);
        pstmt->setString(3, this->_user_id);
    }

    void setId(unique_ptr<sql::PreparedStatement> pstmt) {
        pstmt->setString(1, this->_user_id);
    }
    string _id() { return "user_id"; }

    string updateString () { return "username=?, password=?"; }

    

    string getUsername() {
        return _username;
    }
    string getPassword() {
        return _password;
    }
    string getUserId() {
        return _user_id;
    }

    private:
        string _user_id;
        string _username;
        string _password;    
};

std::string createUniqueId(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::string hashedString = "";
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char hex[3];
        sprintf(hex, "%02x", hash[i]);
        hashedString += hex;
    }
    
    std::string uniqueId = hashedString.substr(0, 8);
    return uniqueId;
}

#endif // USER_H
