# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=g++
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS= `cat CFLAGS.txt`
BuildF = obj/
all: clean assembler processor disassembler


assembler :  $(BuildF)assembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o 
	$(CC) -g $(BuildF)assembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o -o assm.exe

$(BuildF)assembler.o :
	$(CC) -c $(CFLAGS) assembler.cpp -o $(BuildF)assembler.o



processor :  $(BuildF)processor.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)stack.o $(BuildF)functions.o
	$(CC) -g $(BuildF)processor.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)stack.o $(BuildF)functions.o -o proc.exe

$(BuildF)processor.o :
	$(CC) -c $(CFLAGS) processor.cpp -o $(BuildF)processor.o



disassembler : $(BuildF)disassembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o 
	$(CC) -g   $(BuildF)disassembler.o $(BuildF)prepeare_text.o $(BuildF)log_file.o $(BuildF)functions.o -o dis.exe

$(BuildF)disassembler.o:
	$(CC) -c $(CFLAGS) disassembler.cpp -o $(BuildF)disassembler.o


$(BuildF)prepeare_text.o : 
	$(CC) -c $(CFLAGS) prepeare_text.cpp -o $(BuildF)prepeare_text.o

$(BuildF)log_file.o :
	$(CC) -c $(CFLAGS) log_file.cpp -o $(BuildF)log_file.o

$(BuildF)stack.o :
	$(CC) -c $(CFLAGS) stack.cpp -o $(BuildF)stack.o

$(BuildF)functions.o :
	$(CC) -c $(CFLAGS) functions.cpp -o $(BuildF)functions.o

clean:
	rm -rf obj/*.o
