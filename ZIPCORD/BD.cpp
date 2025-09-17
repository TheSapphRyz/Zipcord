#include "BD.hpp"
#include "main.h"
#include <winsqlite/winsqlite3.h>
#include <algorithm>

sqlite3* BD::db;
std::string db_path = "zipcord.db";
char* BD::err;
void BD::exec(const std::string& sql) {
    char* err_msg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string error = err_msg ? err_msg : "Unknown error";
        sqlite3_free(err_msg);
        throw std::runtime_error("SQL error: " + error);
    }
}

void BD::initdb() {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Создание таблиц
    const std::string create_tables = R"(
            CREATE TABLE IF NOT EXISTS Users (
                uid INTEGER PRIMARY KEY,
                name TEXT NOT NULL,
                theme_gl TEXT,
                mic INTEGER DEFAULT 0,
                invoice INTEGER DEFAULT 0,
                logined INTEGER DEFAULT 0,
                regged INTEGER DEFAULT 0
            );

            CREATE TABLE IF NOT EXISTS Chats (
                cid INTEGER PRIMARY KEY,
                name TEXT NOT NULL,
                last_msg_w_time TEXT,
                um INTEGER DEFAULT 0,
                par INTEGER DEFAULT 0,
                ls_or_group INTEGER DEFAULT 0
            );

            CREATE TABLE IF NOT EXISTS Messages (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                cid INTEGER NOT NULL,
                uid INTEGER NOT NULL,
                text TEXT,
                sender TEXT,
                time TEXT,
                media64 TEXT,
                isImage INTEGER DEFAULT 0,
                isText INTEGER DEFAULT 1,
                doc INTEGER DEFAULT 0,
                doci INTEGER,
                w INTEGER DEFAULT 0,
                h INTEGER DEFAULT 0,
                reply INTEGER,
                FOREIGN KEY (cid) REFERENCES Chats(cid),
                FOREIGN KEY (uid) REFERENCES Users(uid)
            );

            CREATE TABLE IF NOT EXISTS Avatars (
                uid INTEGER PRIMARY KEY,
                ava64 TEXT,
                FOREIGN KEY (uid) REFERENCES Users(uid)
            );

            CREATE TABLE IF NOT EXISTS Reactions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                message_id INTEGER NOT NULL,
                rid INTEGER NOT NULL,
                user_id INTEGER NOT NULL,
                reaction TEXT,
                FOREIGN KEY (message_id) REFERENCES Messages(id),
                FOREIGN KEY (user_id) REFERENCES Users(uid)
            );

            CREATE INDEX IF NOT EXISTS idx_messages_cid ON Messages(cid);
            CREATE INDEX IF NOT EXISTS idx_reactions_message_id ON Reactions(message_id);
        )";
    bd.exec(create_tables);
}
void BD::saveMSG(Message m) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Messages (cid, uid, text, sender, time, media64, isImage, isText, doc, doci, w, h, reply) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_int(stmt, 1, m.cid);
    sqlite3_bind_int(stmt, 2, m.uid);
    sqlite3_bind_text(stmt, 3, m.text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, m.sender.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, m.time.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, m.media64.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, m.isImage);
    sqlite3_bind_int(stmt, 8, m.isText);
    sqlite3_bind_int(stmt, 9, m.doc);
    sqlite3_bind_int(stmt, 10, m.doci);
    sqlite3_bind_int(stmt, 11, m.w);
    sqlite3_bind_int(stmt, 12, m.h);
    sqlite3_bind_int(stmt, 13, m.reply);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to insert message: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(stmt);

    /*if (m.m.size() != 0) {
        for (const auto& [rid, reaction] : m.m) {
            const char* reaction_sql = "INSERT INTO Reactions (message_id, rid, user_id, reaction) VALUES ((SELECT last_insert_rowid()), ?, ?, ?)";
            if (sqlite3_prepare_v2(db, reaction_sql, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error("Failed to prepare reaction statement: " + std::string(sqlite3_errmsg(db)));
            }
            sqlite3_bind_int(stmt, 1, rid);
            sqlite3_bind_int(stmt, 2, m.uid); // Предполагаем, что user_id берется из сообщения
            sqlite3_bind_text(stmt, 3, reaction.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw std::runtime_error("Failed to insert reaction: " + std::string(sqlite3_errmsg(db)));
            }
            sqlite3_finalize(stmt);
        }
    }*/
}
void BD::getMSGS(std::vector<Message>& msgs) {
    msgs.clear();
    const char* sql = "SELECT id, cid, uid, text, sender, time, media64, isImage, isText, doc, doci, w, h, reply "
        "FROM Messages ORDER BY time ASC";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Message msg;
        msg.id = sqlite3_column_int(stmt, 0);
        msg.cid = sqlite3_column_int(stmt, 1);
        msg.uid = sqlite3_column_int(stmt, 2);
        msg.text = sqlite3_column_text(stmt, 3) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        msg.sender = sqlite3_column_text(stmt, 4) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : "";
        msg.time = sqlite3_column_text(stmt, 5) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";
        msg.media64 = sqlite3_column_text(stmt, 6) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)) : "";
        msg.isImage = sqlite3_column_int(stmt, 7);
        msg.isText = sqlite3_column_int(stmt, 8);
        msg.doc = sqlite3_column_int(stmt, 9);
        msg.doci = sqlite3_column_int(stmt, 10);
        msg.w = sqlite3_column_int(stmt, 11);
        msg.h = sqlite3_column_int(stmt, 12);
        msg.reply = sqlite3_column_int(stmt, 13);
        msgs.push_back(msg);
    }
    sqlite3_finalize(stmt);
}
std::string BD::getava(int uid) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ava64 FROM Avatars WHERE uid = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare avatar select: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_bind_int(stmt, 1, uid);
    std::string ava64;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        ava64 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return ava64;
}
void BD::updateavas() {
    // тут запрос на сервер возвращает uid, ava64
    std::vector<std::pair<int, std::string>> avatars; // Заглушка

    for (const auto& [uid, ava64] : avatars) {
        sqlite3_stmt* stmt;
        const char* sql = "INSERT OR REPLACE INTO Avatars (uid, ava64) VALUES (?, ?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare avatar update: " + std::string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_int(stmt, 1, uid);
        sqlite3_bind_text(stmt, 2, ava64.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to update avatar: " + std::string(sqlite3_errmsg(db)));
        }
        sqlite3_finalize(stmt);
    }
}
void BD::stop() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}




