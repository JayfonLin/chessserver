cc = g++
program = chess
objects = board.o chess_move.o chess_util.o constant.o define.o evaluation.o \
history_heuristic.o main.o move_generator.o negascout_tt_hh.o search_engine.o \
test_util.o transposition_table.o

$(program) : $(objects)
	$(cc) -o $(program) $(objects)

main.o : negascout_tt_hh.h board.cpp test_util.cpp
	$(cc) -c main.cpp

negascout_tt_hh.o : search_engine.h transposition_table.h history_heuristic.h negascout_tt_hh.h negascout_tt_hh.cpp
	$(cc) -c negascout_tt_hh.cpp

board.o : constant.h board.cpp
	$(cc) -c board.cpp

test_util.o : constant.h chess_move.h chess_util.h test_util.cpp
	$(cc) -c test_util.cpp

search_engine.o : constant.h chess_move.h move_generator.h evaluation.h chess_util.h search_engine.h search_engine.cpp
	$(cc) -c search_engine.cpp

transposition_table.o : constant.h chess_move.h define.h chess_util.h transposition_table.h transposition_table.cpp 
	$(cc) -c transposition_table.cpp

history_heuristic.o : chess_move.h history_heuristic.h history_heuristic.cpp
	$(cc) -c history_heuristic.cpp

constant.o : define.h constant.h
	$(cc) -c constant.h

chess_move.o : define.h chess_move.h
	$(cc) -c chess_move.h

chess_util.o : constant.h define.h chess_util.h
	$(cc) -c chess_util.h

move_generator.o : constant.h chess_util.h move_generator.h move_generator.cpp
	$(cc) -c move_generator.cpp

evaluation.o : constant.h chess_util.h move_generator.h evaluation.h evaluation.cpp
	$(cc) -c evaluation.cpp

define.o : define.h
	$(cc) -c define.h
.PHONY : clean
clean :
	-rm chess $(objects) *.h.gch


