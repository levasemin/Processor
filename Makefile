# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=g++
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS= -g -std=c++17 -Wall -Wextra -Weffc++ -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Waggressive-loop-optimizations -Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline -Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new -Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca -Walloca-larger-than=8192 -Wcast-align=strict -Wdangling-else -Wduplicated-branches -Wformat-overflow=2 -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros -Wrestrict -Wshadow=global -Wsuggest-attribute=malloc -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow -fchkp-first-field-has-own-bounds -fchkp-narrow-to-innermost-array -flto-odr-type-merging -fno-omit-frame-pointer
BuildF = obj/
all: clean List


assembler : assembler.o prepeare_text.o log_file.o
	$(CC) -g $(BuildF)assembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o

assembler.o :
	$(CC) -c $(CFLAGS) assembler.cpp -o $(BuildF)assembler.o



processor : assembler.o prepeare_text.o log_file.o
	$(CC) -g $(BuildF)processor.o $(BuildF)prepeare_text.o $(BuildF)log_file.o

processor.o :
	$(CC) -c $(CFLAGS) processor.cpp -o $(BuildF)processor.o



disassembler : disassembler.o prepeare_text.o log_file.o functions.o
	$(CC) -g $(BuildF)assembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o

disassembler.o:
	$(CC) -c $(CFLAGS) disassembler.cpp -o $(BuildF)disassembler.o


prepeare_text.o : 
	$(CC) -c $(CFLAGS)prepeare_text.cpp -o $(BuildF)prepeare_text.o

log_file.o :
	$(CC) -c $(CFLAGS) log_file.cpp -o $(BuildF)log_file.o

functions.o :
	$(CC) -c $(CFLAGS) functions.cpp -o $(BuildF)functions.o

List: Linked-list.o Linker.o main.o tools.o
	$(CC) $(BuildF)Linked-list.o $(BuildF)Linker.o $(BuildF)main.o $(BuildF)tools.o


clean:
	rm -rf obj/*.o
