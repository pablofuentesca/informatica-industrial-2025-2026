#ifndef __PIECE_H__
#define __PIECE_H__

class Object {
public:
	enum type_t { QUEEN_GREEN, QUEEN_BLACK, EMPTY_CELL };
	virtual void getCellNumber(int& x, int& y) = 0;
	virtual type_t getType() = 0;
	virtual ~Object() = default;
};

class Piece : public Object {
protected:
	int x;
	int y;
	type_t type;
public:
	Piece() : x(-1), y(-1), type(EMPTY_CELL) {}
	void setCell(int x, int y, type_t t) { this->x = x; this->y = y; this->type = t; }
	virtual void getCellNumber(int& x_out, int& y_out) override { x_out = this->x; y_out = this->y; }
	virtual type_t getType() override { return type; }
	virtual ~Piece() = default;
};

#endif