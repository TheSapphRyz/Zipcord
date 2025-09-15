#include "BD.hpp"
#include "main.h"
#include <winsqlite/winsqlite3.h>
#include <algorithm>

sqlite3* BD::db;
char* BD::err;

void BD::initdb() {
    const char* SQL = R"(
        CREATE TABLE IF NOT EXISTS msgs (
            id INTEGER,
            text TEXT,
            sender TEXT,
            time TEXT,
            uid INTEGER,
            media64 TEXT,
            isImage BOOLEAN,
            isVideo BOOLEAN,
            isText BOOLEAN,
            doc BOOLEAN,
            doci TEXT,
            ras TEXT,
            w INTEGER,
            h INTEGER
        );
        CREATE TABLE IF NOT EXISTS avas (
            uid INTEGER,
            aid INTEGER,
            avatar TEXT
        );
        CREATE TABLE IF NOT EXISTS user (
            uid INTEGER,
            name TEXT,
            theme_gl TEXT,
            avatar TEXT
        );
        CREATE TABLE IF NOT EXISTS m_data (
            m_id INTEGER,
            em_id INTEGER,
            users TEXT,
            FOREIGN KEY (m_id) REFERENCES messages(id) ON DELETE CASCADE
        );
    )";
	if (sqlite3_open("zipcord.db", &db));
	sqlite3_exec(db, SQL, 0, 0, &err);
	sqlite3_free(err);
}
void BD::saveMSG(Message m) {
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &err);
    std::stringstream sql;
    sql << "INSERT INTO msgs (id, text, sender, time, uid, media64, isImage, isVideo, isText, doc, doci, ras, w, h) " << "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 0, m.id);
        sqlite3_bind_text(stmt, 1, m.text.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, m.sender.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, m.time.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, m.uid);
        sqlite3_bind_text(stmt, 5, m.media64.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 8, m.isText);
        sqlite3_bind_int(stmt, 9, m.doc);
        sqlite3_bind_int(stmt, 12, m.w);
        sqlite3_bind_int(stmt, 13, m.h);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    /*std::stringstream map_sql;
    map_sql << "INSERT INTO m_data (message_id, key, value) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, map_sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        for (const auto& [key, value] : m.m) {
            sqlite3_bind_int64(stmt, 1, m.id);
            sqlite3_bind_int(stmt, 2, key);
            sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
    }*/
    sqlite3_exec(db, "COMMIT", 0, 0, &err);
}
void BD::getMSGS(std::vector<Message>& msgs) {
    msgs.clear();
    const char* sql = "SELECT * FROM messages";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Message m;
            m.id = sqlite3_column_int(stmt, 0);
            m.text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            m.sender = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            m.time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            m.uid = sqlite3_column_int(stmt, 4);
            m.media64 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            m.isImage = sqlite3_column_int(stmt, 6);
            m.isText = sqlite3_column_int(stmt, 8);
            m.doc = sqlite3_column_int(stmt, 9);
            m.w = sqlite3_column_int(stmt, 12);
            m.h = sqlite3_column_int(stmt, 13);
            msgs.push_back(m);
        }
        sqlite3_finalize(stmt);
    }
}
std::string BD::getava(int uid) {

}
void BD::updateavas() {

}
void BD::stop() {

}




