typedef enum { C_CONST, C_PLUS, C_MINUS, C_MULT } cmdtype;

struct expr {
	int type;
	struct expr *left;
	struct expr *right;
	int value;
};

extern struct expr* parser (char*);
