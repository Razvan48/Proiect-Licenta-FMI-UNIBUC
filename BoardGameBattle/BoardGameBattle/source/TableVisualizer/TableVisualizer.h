#pragma once


class TableVisualizer
{
private:
	TableVisualizer();
	~TableVisualizer();
	TableVisualizer(const TableVisualizer& other) = delete;
	TableVisualizer& operator= (const TableVisualizer& other) = delete;
	TableVisualizer(const TableVisualizer&& other) = delete;
	TableVisualizer& operator= (const TableVisualizer&& other) = delete;

public:
	static TableVisualizer& get();

	void draw();
	void update();
};