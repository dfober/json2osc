
OUT  = jsonlex.cpp jsonparse.cpp jsonparse.hpp
YACC = bison
LEX = flex
PREFIX := json

code : $(OUT)

jsonparse.cpp : json.y
	$(YACC) -d -o jsonparse.cpp  -p $(PREFIX) json.y

jsonparse.hpp : json.y
	$(YACC) -d -o jsonparse.cpp  -p $(PREFIX) json.y

jsonlex.cpp : json.l jsonparse.hpp
	$(LEX)  -ojsonlex.cpp -I -P$(PREFIX) json.l
	
clean:
	rm -f $(OUT)
