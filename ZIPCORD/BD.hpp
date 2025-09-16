#pragma once
#include "main.h"
#include <winsqlite/winsqlite3.h>
struct Message;
class BD {
public:
	void initdb();
	void saveMSG(Message m);
	void getMSGS(std::vector<Message>& msgs);
	std::string getava(int uid);
	void updateavas();
	void stop();
	void exec(const std::string& sql);
private:
	static sqlite3* db;
	static char* err;
};

