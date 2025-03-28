// SUPREET MAURYA
#include <bits/stdc++.h>
#define S second
#define F first
using namespace std;
#define pb push_back
#define sort_all(V) sort(V.begin(),V.end())
vector< int > mac_code;
map< std :: string , pair< std :: string , int> > op_table;
int mem[1 << 24];
int r1 = 0;
pair<int, int> read_write_pair;
int reg1, reg2, p_counter, stack_pointer, line_num = 0;

void adc(int val) { reg1 += val; }
void stl ( int r_offs ) {
	read_write_pair = {stack_pointer + r_offs, mem[stack_pointer + r_offs]};
	bool tmper = 0;
	mem[stack_pointer + r_offs] = reg1;
	reg1 = reg2;
}
void ldc(int val) { reg2 = reg1; reg1 = val; }
void ldl(int r_offs) {
	reg2 = reg1;
	reg1 = mem[stack_pointer + r_offs];
	read_write_pair = {stack_pointer + r_offs, 0};
}
void ldnl(int r_offs) {
	reg1 = mem[reg1 + r_offs];
	read_write_pair = { stack_pointer + r_offs , 0 } ;
}
void sub(int val) { reg1 = reg2 - reg1; }
void stnl(int r_offs) {
	read_write_pair = { stack_pointer + r_offs , mem[ stack_pointer + r_offs ] } ;
	r1 = 0 ;
	mem[reg1 + r_offs] = reg2;
}
void shr( int val ) { reg1 = reg2 >> reg1; }
// sp2a 
void sp2a ( int val ) {
	reg2 = reg1;
	reg1 = stack_pointer;
}
void adj( int val ) { stack_pointer += val; }
int reggg1 = 0 ;
void ret( int val ) {
	p_counter = reg1;
	reg1 = reg2;
}
void call( int r_offs ) {
	reg2 = reg1;
	reg1 = p_counter;
	p_counter += r_offs;
}
void brlz( int r_offs ) { if (reg1 < 0) { p_counter += r_offs; } }
void halt( int val ) { return; }
void a2sp( int val ) {
	stack_pointer = reg1;
	reg1 = reg2;
}
void brz( int r_offs ) { if ( reg1 == 0 ) {  p_counter += r_offs ; }  }
void add( int val ) { reg1 += reg2 ; }
void br( int r_offs ) {p_counter += r_offs ; }
int r2 = 2;
vector<std :: string > mns = { "ldc" , "adc" , "ldl" , "stl" , "ldnl" , "stnl" , "add" , "sub" , "shl" , "shr" , "adj" , "a2sp" , "sp2a" , "call" , "return" , "brz" , "brlz" , "br" , "HALT" } ;
int r3 = 0;
void shl(int val) { reg1 = reg2 << reg1 ; }
void ( *function1[] ) ( int val ) = {ldc , adc , ldl , stl , ldnl , stnl , add , sub , shl , shr , adj , a2sp , sp2a , call , ret , brz , brlz ,  br , halt } ;
// r3 = 1 ;
std :: string convert_decimal_to_hex(int n) {
	std :: string str =  "" ;
	int j1 = 0 ;
	while( j1  <  8 ) {
		int rem = ( n % 16 ) ;
		str += ( rem <= 9 ? char( rem + '0' ) : char( rem - 10 ) + 'A' ) ;
		j1++ ;
		n = ( n / 16 ) ;
	}
	string str1 = "" ;
	for(int i = (int)str.size() - 1 ; i >= 0 ; i-- ) {
		str1  = str1 + str[i] ;
	}
	str = str1 ;
	for(int i = 0; i < (int)str.size(); i++ ) {
		if( !(str[i] <= '9' && str[i] >= '0') && !(str[i] <= 'Z' && str[i] >= 'A' ) ) str[i] = '0';
	}
	return str ;
}
void printing() {
	int j1 = 0 ;
	while( j1 < (int)mac_code.size() ) { 
		std :: cout << convert_decimal_to_hex(j1) << " ";
		int i1 = j1;
		while( i1 < min(  { (int)mac_code.size(), j1 + 4 } ) ) {
			std :: cout << convert_decimal_to_hex(mem[i1]) << " " ;
			i1++;
		}
		std :: cout << std :: endl;
		j1 += 4;
	}
}
void print_memory_after_execution() {
	std :: cout << "after execution memory dump" << std :: endl;
	printing();
}
bool instrc_exe(int line_curr, int  flr) {
	int opcd = ( line_curr & 0xFF );
	int val = ( line_curr - opcd );
	val = ( val >> 8 ) ;
	line_num = line_num + 1;
	(function1[opcd])(val);
	if ( ( p_counter < 0 ) || ( line_num > ( 1 << 24 ) ) || (p_counter > (int)mac_code.size()) ) {
		std :: cout << "segmentation fault " << std :: endl;
		exit(0);
		return true;
	}
	if ( flr == 0) {
		printf("p_counter = %08X, stack_pointer = %08X, A = %08X, B = %08X ", p_counter, stack_pointer, reg1, reg2);
		// cout << endl; 
		std :: cout << mns[opcd] << " " ;
		if ( op_table[mns[opcd]].S > 0 ) { std :: cout << convert_decimal_to_hex(val) << std :: endl ; }
		else cout << endl;
	}
	else if ( flr == 1 and (opcd == 2 or opcd == 4)) { std :: cout << "Reading memory[" << convert_decimal_to_hex(read_write_pair.F) << "] finds " << convert_decimal_to_hex(reg1) << std :: endl ; }
	else if ( flr == 2 and (opcd == 3 or opcd == 5)) { std :: cout << "Writing memory[" <<convert_decimal_to_hex(read_write_pair.F) << "] was " << convert_decimal_to_hex(read_write_pair.S) << " now " << convert_decimal_to_hex(mem[read_write_pair.F]) << std :: endl ; }
	if (opcd >= 18) { return true; }
	else return false;
}
void run( int  flr ) {
	while (p_counter < (int)mac_code.size()) {
		int curr_line_num = mac_code[p_counter++];
		if ( instrc_exe(curr_line_num,  flr) ) break;
	}
}
void printing2() {
	int j1 = 0;
	while( j1 < (int)mac_code.size() ) {
		std :: cout << convert_decimal_to_hex(j1) << " ";
		int i1 = j1;
		while( i1 < min( { (int)mac_code.size(), j1 + 4 } ) ) {
			std :: cout << convert_decimal_to_hex(mac_code[i1]) << " ";
			i1++;
		}
		std :: cout << endl;
		j1 += 4;
	}
}
void print_memory_before_execution() {
	std :: cout << "before execution memory dump" << endl;
	printing2();
}
void print_instruction_set_architecture() {
	std :: cout<<"Opcode 				Mnemonic 				  Operand"<< endl ;
	std :: cout<<"0      					ldc      				value "<< endl ;
	std :: cout<<"1      					adc      				value "<< endl ;
	std :: cout<<"2      					ldl      				value "<< endl ;
	std :: cout<<"3      					stl      				value "<< endl ;
	std :: cout<<"4      					ldnl     				value "<< endl ;
	std :: cout<<"5      					stnl     				value "<< endl ;
	std :: cout<<"6      					add            				   "<< endl ;
	std :: cout<<"7      					sub            					"<< endl ;
	std :: cout<<"9      					shr           					"<< endl ;
	std :: cout<<"10     					adj      				value "<< endl ;
	std :: cout<<"11     					a2sp           				  "<< endl ;
	std :: cout<<"12     					sp2a                          "<< endl ;
	std :: cout<<"13     					call     				offset"<< endl ;
	std :: cout<<"14     					return                        "<< endl ;
	std :: cout<<"15     					brz      				offset"<< endl ;
	std :: cout<<"16     					brlz     				offset"<< endl ;
	std :: cout<<"17     					br       				offset"<< endl ;
	std :: cout<<"18     					HALT           				  "<< endl ;
	std :: cout<<"       					SET      				value "<< endl ;
}
void get_input(std :: string file_name) {
	ifstream file(file_name, ios::in | ios::binary);
	unsigned int current;
	int cntr = 0;
	while(file.read((char*)&current, sizeof(int))) { 
		mac_code.pb(current);
		mem[cntr++] = current;
	}
}
void exe_command(std :: string str) {
	if (str == "-trace") { run(0) ; std :: cout << "Program execution finished " << endl ; }
	// else if (str == "-write") { run(2) ; }
	// else if (str == "-read") { run(1) ; }
	else if (str == "-after") { run(3) ; print_memory_after_execution() ; }
	else if (str == "-before") { run(3) ; print_memory_before_execution() ; }
	else if (str == "-isa") { print_instruction_set_architecture() ; }
	// else if (str == "-wipe") { reg1 = reg2 = stack_pointer = p_counter = 0 ; }
	else { std :: cout << "Invalid command" << endl; exit(0) ; }
	std :: cout << line_num << " instructions executed" << endl ;
}
int main(int argc, char* argv[]) {
	if (argc < 3) {
		std :: cout << "Invalid Format" << std :: endl;
		std :: cout << "Expected format << ./emu [command] filename.o" << std :: endl;
		std :: cout << "Commands are " << std :: endl;
		std :: cout << "-trace  show instruction trace" << std :: endl;
		std :: cout << "-read   show memory reads" << std :: endl;
		std :: cout << "-write  show memory writes" << std :: endl;
		std :: cout << "-before show memory dump before execution" << std :: endl;
		std :: cout << "-after  show memory dump after execution" << std :: endl;
		std :: cout << "-wipe   wipe written flags before execution" << std :: endl;
		std :: cout << "-isa    display ISA" << std :: endl;
		return 0;
	}
	std :: string file_name = argv[2];
	op_table = {{"data", {"", 1}}, {"ldc", {"00", 1}}, {"adc", {"01", 1}}, {"ldl", {"02", 2}}, {"stl", {"03", 2}}, {"ldnl", {"04", 2}}, {"stnl", {"05", 2}}, {"add", {"06", 0}}, {"sub", {"07", 0}}, {"shl", {"08", 0}}, {"shr", {"09", 0}}, {"adj", {"0A", 1}}, {"a2sp", {"0B", 0}}, {"sp2a", {"0C", 0}}, {"call", {"0D", 2}}, {"return", {"0E", 0}}, {"brz", {"0F", 2}}, {"brlz", {"10", 2}}, {"br", {"11", 2}}, {"HALT", {"12", 0}}, {"SET", {"", 1}}};
	get_input(file_name);
	std :: string str = argv[1];
	exe_command(str);
	// cout << mem[999] << " " << mem[1000] << " " << mem[1001] << endl ;
	// for(int i = 0; i < ( 1ll << 24 ) ; i++ ) {
	// 	if( mem[i] != 0 ) {
	// 		cout << "YEP";
	// 		cout << i << " " << mem[i] << " "<< endl ;
	// 	}
	// }
	// cout << endl; 
	return 0;
}