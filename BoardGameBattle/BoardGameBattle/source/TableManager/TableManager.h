#pragma once


class TableManager
{
private:
	TableManager();
	~TableManager();
	TableManager(const TableManager& other) = delete;
	TableManager& operator= (const TableManager& other) = delete;
	TableManager(const TableManager&& other) = delete;
	TableManager& operator= (const TableManager&& other) = delete;

public:
	static TableManager& get();

	void update();
};