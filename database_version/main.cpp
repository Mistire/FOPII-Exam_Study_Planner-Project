#include <iostream>
#include <memory>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "user.h"

string url("tcp://127.0.0.1:3306");
string user("root");
string pass("mindahun@2119");
string database("exam_stude_planner");

using namespace std;


unique_ptr<sql::Connection> create_connection() {
    sql::Driver* driver = sql::mysql::get_driver_instance();

    std::unique_ptr<sql::Connection> con(driver->connect(url, user, pass));
    con->setSchema(database);

    return con;
}

template <typename T>
bool create(unique_ptr<sql::Connection>& con, const T& object) {
   try { 
        string query = "INSERT INTO " + object.getTableName() + " VALUES(";
        for (int i = 0; i < object.size(); ++i) {
                    if (i > 0) query += ", ";
                    query += "?";
                }
        query += ")";
        
        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        // prep_stmt->setInt(1, test_data[i].id);
        // prep_stmt->setString(2, test_data[i].label);
        /* executeUpdate() returns the number of affected = inserted rows */
        object.setParams(pstmt);
        int row_num = pstmt->executeUpdate();
        return row_num == 1;
    } catch (sql::SQLException& e) {
            cerr << e.what() << endl;
            return false;
    }
}

void find_user(unique_ptr<sql::Connection>& con, User user) {
   try { 
        string query = "SELECT username, password FROM users WHERE username=? && password=?";
        
        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, user.getUsername());
        pstmt->setString(2, user.getPassword());

        unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if(res->next()) {
            cout << "Username: " << res->getString("username");
        }
    } catch (sql::SQLException& e) {
            cerr << e.what() << endl;
    }
}
template <typename T>
bool update(unique_ptr<sql::Connection>& con, const T& object) {
   try { 
        string query = "UPDATE " + object.getTableName() + " SET " object.updateString() + " WHERE " + object._id() + "=?";
        
        unique_ptr<sql::PreparedStatement> pstmt = con->prepareStatement(query);
        object.setUpdateParams(pstmt);

        int row_num = pstmt->executeUpdate();
        return row_num == 1;
    } catch (SQLException& e) {
            cerr << e.what() << endl;
            return false;
    }
}
template <typename T>
bool del(unique_ptr<sql::Connection>& con, const T& object) {
   try { 
        string query = "DELETE FROM " + object.getTableName() + " WHERE " + object._id() + "=?";
        
        unique_ptr<sql::PreparedStatement> pstmt = con->prepareStatement(query);
        object.setId(pstmt);

        int row_num = pstmt->executeUpdate();
        return row_num == 1;
    } catch (SQLException& e) {
            cerr << e.what() << endl;
            return false;
    }
}

int main() {
    try {
        unique_ptr<sql::Connection> con = create_connection();
        User user("Abbe", "1q2w3");
        create(con, user);
        find_user(con, user);
        user = User("Kebbe", "1q2w3");
        update(con, user);
        del(con, user);
        find_user(con, user);


    } catch (sql::SQLException &e) {

    }
}