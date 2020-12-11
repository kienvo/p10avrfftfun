#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

void println_reg(uint8_t byte, char *name, char *file, int line) {
	char bin[33];
	utoa((int)byte,bin,2);
	fprintf(stderr, "at %s:%d: \n\t%s:\t%s\n", file, line, name, bin);
}
void printvar(int var, char *name, char *file, int line) {
	fprintf(stderr, "at %s:%d: %s:\t%d\n", file, line, name, var);
}
void print_reg(uint8_t byte, char *name) {
	char bin[33];
	utoa((int)byte,bin,2);
	fprintf(stderr, "\t%s: %8s", name, bin);
}

void print_io_reg(char *file, int line) {
	fprintf(stderr, "IO at %s:%d:\n",file,line);
	print_reg(PINA,"PINA");
	print_reg(DDRA,"DDRA");
	print_reg(PORTA,"PORTA");
	fprintf(stderr, "\n");
	print_reg(PINB,"PINB");
	print_reg(DDRB,"DDRB");
	print_reg(PORTB,"PORTB");
	fprintf(stderr, "\n");
	print_reg(PINC,"PINC");
	print_reg(DDRC,"DDRC");
	print_reg(PORTC,"PORTC");
	fprintf(stderr, "\n");
	print_reg(PIND,"PIND");
	print_reg(DDRD,"DDRD");
	print_reg(PORTD,"PORTD");
	fprintf(stderr, "\n");
	print_reg(PINE,"PINE");
	print_reg(DDRE,"DDRE");
	print_reg(PORTE,"PORTE");
	fprintf(stderr, "\n");
	print_reg(PINF,"PINF");
	print_reg(DDRF,"DDRF");
	print_reg(PORTF,"PORTF");
	fprintf(stderr, "\n");
	print_reg(PING,"PING");
	print_reg(DDRG,"DDRG");
	print_reg(PORTG,"PORTG");
	fprintf(stderr, "\n");
}

void print_info() {

}