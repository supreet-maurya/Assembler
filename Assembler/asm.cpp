// SUPREET MAURYA
#include <bits/stdc++.h>
using namespace std;
#define S second
#define F first
#define pb push_back
#define sort_all(V) sort(V.begin(),V.end())

struct syml {
    int address;
    int type_of_symbol;
};
struct mn {
    string opcode ;
    int mnemonic_type ;
};

bool newFlag = 0;
map<string , int> mp;

struct lb_ins_op {
    string instrc , label , oprd;
    lb_ins_op(string &l, string &ins, string &oprd1) {
        label = l; instrc = ins; oprd = oprd1;
    }
};
map<string, syml> labels_declared; 
map<string, syml> labels_used;
map<string, mn> opcode_table;
set<string> data_set; 
vector<string> machine_code;
vector<string> list_code;
vector<pair<int, string>> warn;
vector<pair<int, lb_ins_op>> data_seg;
string file_list;
vector<pair<int , string >> v_error;
set<string> data_declared;
vector<pair<int, lb_ins_op>> instrc_cleaned; 
string file_log;
vector<pair<int, lb_ins_op>> instrc_ip; 
string mach_cd_file_name;
string inp_name;

int p_counter = 0;
bool fl_halt = 0 ;
bool q = 0;
bool is_num( char chr ) {
    return (chr <= '9' && chr >= '0') ;
}

void get_lb(string &lbl_name, string &instrc, string &oprd1, int &line_num, string ip_format , int &j1 ) {
    int i2 = j1;
    while ( (ip_format[j1] == '\t') || (ip_format[j1] == ' ' ) ) {
        if ( j1 == ip_format.size() ) break;
        j1++;
    }
    while (!(ip_format[j1] == ' ' || ip_format[j1] == '\t')) {
        if (j1 == ip_format.size()) break;
        instrc += ip_format[j1];
        j1++;
    }
    q = 0;
    while ((ip_format[j1] == ' ' || ip_format[j1] == '\t')) {
        if (j1 == ip_format.size()) break;
        j1++;
    }
    i2 = j1;
    while (!(ip_format[j1] == ' ' || ip_format[j1] == '\t')) {
        if (j1 == ip_format.size()) break;
        oprd1 += ip_format[j1];
        j1++;
    }
    while ((ip_format[j1] == ' ' || ip_format[j1] == '\t')) {
        if (j1 == ip_format.size()) break;
        j1++;
    }
}
void get_lb_ins_op ( string &instrc, string &lbl_name , string &oprd1, int &line_num, string ip_format) {
    int j1 = 0;
    while( j1 < ip_format.size() ) {
        if (ip_format[j1] == ':') {
            int i1 = 0 ;
            while( i1 < j1 ) {
                lbl_name = lbl_name + ip_format[i1];
                i1++;
            }
            j1++;
            break;
        }
        j1++;
    }
    if ( ip_format[j1 - 1] != ':' ) j1 = 0;

    get_lb( lbl_name , instrc , oprd1 , line_num , ip_format , j1 ) ;

    if  ( j1 != ip_format.size() ) {
        string err = "Extra oprd Present";
        v_error.pb({line_num, err });
    }
}

bool is_character(char chr ) {
    return ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z')) ;
}
void chtd(string &str , int &ans , int &j1 , int &mult ) {     //converts hexa to decimal
    while(j1 >= 2 ) {
        if ( str[j1] <= 'F' && str[j1] >= 'A')    {
            int x = (int(str[j1]) - 55);
            ans = ans + (x * mult);
            mult *=  16;
        }
        else if ( str[j1] <= '9' && str[j1] >= '0' )    {
            int x = (int(str[j1]) - 48) ;
            ans = ans + (x * mult);
            mult *=  16;   
        }
        j1--;
    }
}
bool is_hexa(string str)    {
    return ( str.size() > 2 && str.compare(0, 2, "0x") == 0 && str.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos );
}
int convert_hex_to_dec(string str) {
    int mult = 1;
    int y = str.size();
    int ans = 0;
    int j1 = y - 1;
    chtd(str , ans , j1 , mult ) ;
    return ans;
}
bool is_oct(string str) {
    return (str.size() > 1 && str.compare(0, 1, "0") == 0 && str.find_first_not_of("01234567", 1) == std::string::npos);
}
void cotd(string &str , int &mult , int &j1 , int &ans ) {   //converts octal to decimal
    while(j1 >= 1 ) {
        if (str[j1] >= '0' && str[j1] <= '7') {
            ans = ans + (int(str[j1]) - 48) * mult;
            mult = mult * 8;
        }
        j1--;
    }
}
int convert_oct_to_dec(string str) {
    int y = str.size();
    int mult = 1;
    int ans = 0;
    int j1 = y - 1;
    cotd( str , mult , j1 , ans ) ;
    return ans;
}
bool is_label_name(string &lbl_name) {
    if (lbl_name.empty())  return false;
    bool flk = (is_character(lbl_name[0]));
    int i = 1; 
    while(i < lbl_name.size() ) {
        bool flk1 = ((is_character(lbl_name[i])) | (is_num(lbl_name[i]))) ;
        flk1 = ( flk1 | ( lbl_name[i] == '_') );
        flk = ( flk & flk1 );
        i++;
    }
    return flk;
}
string oprd_process(string trs, int *wrs, int line_num) {
    if (is_label_name(trs)) { 
        labels_used[trs] = {line_num, 5};
        return trs;
    }
    int pos = 1 ;
    if ( trs[0] == '-' ) pos = -pos ;
    bool flk = false;
    if( trs[0] == '-'  ) flk = true;
    if( (trs[0] == '+') ) flk = true;

    if (flk) trs = trs.substr( 1 );
    if (is_oct(trs)) return to_string( pos * convert_oct_to_dec(trs) ) ;
    if (is_hexa(trs)) return to_string( pos * convert_hex_to_dec(trs) ) ;
    bool flk1 = 1 ;
    int j1 = 0 ;
    while( j1 < (int)trs.size() ) {
        flk1 = ( flk1 & is_num( trs[ j1 ] ) ) ;
        j1++;
    }
    if ( flk1 ) return (to_string(pos * stoi(trs)));
    *wrs = 1 ;
    return trs ;
}
string hexadeci_generation(int f_num, bool is_addr) {
    map< int , char > mp;
    char hr = 'A';
    char letter = '0';
    int i = 0 ;
    while( i <= 15 ) {
        if( i >= 10 ) mp[i] = hr++;
        else mp[i] = letter++;
        i++;
    }
    string final_result = "" ;
    if ( !f_num ) final_result = "0" ;

    if ( f_num <= 0 ) {
        unsigned int n = f_num;
        while ( n ) {
            final_result = mp[n % 16] + final_result;
            n /= 16;
        }
    }
    else {
        while ( f_num ) {
            final_result = mp[f_num % 16] + final_result;
            f_num /= 16;
        }
    }
    int tmpr = final_result.size();
    int j1 = 1 ;
    while( j1 <= (8 - tmpr) ) {
        final_result = '0' + final_result ;
        j1++;
    }
    if ( is_addr ) return final_result.substr(2);
    return final_result;
}
void ip_clean(int &i1 , string &str , string &new_processed_string) {
    while( i1 < str.size() ) {
        char trte = str[i1] ;
        if ( trte == ';' ) break ;
        bool flk = 1 ;
        bool flr2 = false;
        if( trte <= 'Z' && trte >= 'A' )flr2 = true ;
        flk = ( flk & flr2 ) ;
        if ( flk ) trte = tolower( trte ) ;
        new_processed_string += trte ;
        i1++;
    }
}
string input_cleaning(string str) {
    string new_processed_string;
    int j1 = 0;
    int i2 = j1;
    while ( str[j1] == ' ') {
        i2 = j1;
        if ( j1 == str.size() ) break;
        j1++;
    }
    int i1 = j1;
    i2 = j1 ;
    ip_clean(i1 , str , new_processed_string ) ;
    return new_processed_string;
}
void set_declare() {
    vector<pair<int, lb_ins_op>> instrc_tmp;
    for (auto &rlt : instrc_cleaned) {
        if ( (rlt.S.instrc == "set" ) ) {
            mp[rlt.S.label] = stoi(rlt.S.oprd);
            data_set.insert(rlt.S.label);   //data set ( set<string>  ) 

        }
    }
    for (auto &rlt : data_seg) {
        data_declared.insert(rlt.S.label);
    }
    for (auto &rlt : instrc_cleaned) {
        bool in_data = false;
        if ( (rlt.S.instrc == "set") ) {

            if (data_declared.count(rlt.S.label) == 0) {
                newFlag = 1;
                
                rlt.S.instrc = "data";
                data_declared.insert(rlt.S.label);   //data_declare (set<string> )
                in_data = true;
                data_seg.pb(rlt);
            }
        }
        if (!in_data) {
            instrc_tmp.pb(rlt);
        }
    }
    instrc_cleaned.clear();
    for (auto &rlt : instrc_tmp) {
        instrc_cleaned.pb( rlt );
    }
    for (auto &rlt : data_seg) {
        instrc_cleaned.pb( rlt );
    }
    instrc_tmp.clear();
    data_seg.clear();
}
bool instrc_set(string &lbl_name, string &val, int line_num, int to_add) {
    if ( lbl_name.empty() ) {
        string err = "No label name Given in Set Instruction ";
        v_error.pb( make_pair( line_num , err ) ) ;
        return false;
    }
    if ( val.empty() ) {
        string err = "No oprd Given in Set Instruction ";
        v_error.pb( make_pair( line_num, err ) );
        return false;
    }
    int chk = 0;
    string nop = oprd_process(val, &chk, line_num);
    bool fl = (is_character(val[0]) | (chk == 1)) ;
    if (fl) {
        string err = "Unexpected number Set Instruction ";
        v_error.pb({line_num, err});
        return false;
    }
    fl = ( data_declared.count(lbl_name) & labels_declared.count(lbl_name) ) ;
    string ins;

    if ( !fl ) {
        string label;
        string op;
        // increase Stack Size
        ins = "adj";
        op = "2";
        nop = oprd_process(op, &chk, line_num);
        int tmpo = 0;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        //PUSH A
        op = "-1";
        ins = "stl";        
        nop = oprd_process(op, &chk, line_num);
        tmpo = 1;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        // PUSH B
        ins = "stl";
        op = "0";
        nop = oprd_process(op, &chk, line_num);
        tmpo = 2;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;
        
        //load value to set
        ins = "ldc";
        op = val;
        nop = oprd_process(op, &chk, line_num);
        tmpo = 3;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        //load location Of pointer
        ins = "ldc";
        op = lbl_name;
        tmpo = 2;
        nop = oprd_process(op, &chk, line_num);
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        //set value in array
        ins = "adc";
        tmpo = 1;
        string str = to_string(to_add);
        op = str ;
        nop = oprd_process(op, &chk, line_num);
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        // Store value To Set 
        ins = "stnl";
        op = "0";
        tmpo = 1;
        nop = oprd_process(op, &chk, line_num);
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        //  Load Back B 
        ins = "ldl";
        op = "0";
        nop = oprd_process(op, &chk, line_num);
        tmpo = 5;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;

        //load back A
        ins = "ldl";
        op = "0";
        nop = oprd_process(op, &chk, line_num);
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;
         tmpo = 8;

        // Decrease Stack Size
        ins = "adj";
        op = "-2";
        nop = oprd_process(op, &chk, line_num);
        tmpo = 0 ;
        instrc_ip.pb(make_pair(p_counter, lb_ins_op(label, ins, nop)));
        p_counter++;
    }
    else return true;
    return false;   
}
void take_input_file(string &inp_name) {
    ifstream file;
    file.open(inp_name);

    if (file.fail()) {
        std :: cout << "Error Occured - Cannot Open Input File" << std :: endl ;
        exit(0);
    }

    int line_num = 0;
    string line_ip;
    while ( getline(file, line_ip) ) {
        line_num++;
        string ip_format = input_cleaning(line_ip);
        bool flrk1 = ip_format.empty();
        if (ip_format.empty()) continue;

        string lbl_name ; 
        string oprd1 ;
        string instrc1 ;
        get_lb_ins_op( instrc1 , lbl_name , oprd1 , line_num , ip_format ) ;

        flrk1 = ( !instrc1.empty() &  (instrc1 == "data") ) ;
    
        if ( !flrk1 ) { 
            instrc_cleaned.pb( { line_num, lb_ins_op{lbl_name, instrc1, oprd1 } } );
        }
        else {
            bool fl3 = instrc_cleaned.size() > 0;
            if (fl3) {
                auto last_instrc = instrc_cleaned.back();
                if ( (lbl_name.empty() & last_instrc.S.instrc.empty()) ) {
                    data_seg.pb(last_instrc);
                    instrc_cleaned.pop_back();
                }
            }
            data_seg.pb({line_num, lb_ins_op{lbl_name, instrc1, oprd1}});    
        }
    }
}
void first_pass_assembler() {
    take_input_file(inp_name);
    // q = 0;
    set_declare();

    string prev;   //label_name
    // q = 1;
    int ctr = 0;

    for (auto &rlt : instrc_cleaned) { 
        string instrc1 = rlt.S.instrc;
        int line_num = rlt.F;
        string lbl_name = rlt.S.label;
        string oprd = rlt.S.oprd;

        bool flg = 0 ;
        bool flrt1 = lbl_name.empty();
        if ( !lbl_name.empty() ) {
            bool ert = is_label_name(lbl_name);
            if ( ert ) {
                ctr = 0;
                flg = 1;
                prev = lbl_name;
            }
            else {
                string err = "Incorrect label Name ";
                v_error.pb( { line_num, err });
            }
        }
        else ctr++;

        bool oprd_need = 0 ;
        bool flrt = 0 ;
        int use{} ;
        int disp = 0 ;
        if ( !instrc1.empty() ) { 
            if (!(opcode_table.count(instrc1) > 0)) {
                string err = "Invalid Mnemonic ";
                v_error.pb( { line_num, err } );
            }
            else {
                oprd_need = (oprd_need | ( opcode_table[instrc1].mnemonic_type > 0 ) );
                if ( (instrc1 == "set") ) flrt = instrc_set( prev , oprd , line_num , ctr );
                if ( flrt ) instrc1 = "data";
                disp++;
                if ( flg && instrc1 == "data" ) use = 1;
            }
        }
        if ( !flrt && (instrc1 == "set") ) {
            continue;
        }
        if (flg) {
            if ( !(labels_declared.count(lbl_name) > 0)) {
                if( newFlag ) {
                    labels_declared[lbl_name] = { mp[lbl_name] , 0};
                }
                else labels_declared[lbl_name] = {p_counter, use};
            }
            else {
                string err = "Duplicate label Found - \"" + lbl_name + "\"";
                v_error.pb({line_num, err });
            }
        }
        string npr;
        if ( oprd.empty() ) {
            if ( oprd_need ) {
                string err = "oprd Missing ";
                v_error.pb ( { line_num, err } ) ;
            }
        }
        else {
            if (! oprd_need ) {
                string err = "Unexpected oprd Found  ";
                v_error.pb(make_pair(line_num, err ));
            }
            else {
                int tmpr = 0;
                npr = oprd_process(oprd, &tmpr , line_num);
                if (tmpr == 1) {
                    string err = "label Name not Found ";
                    v_error.pb(make_pair(line_num, err ));
                }
            }
        }
        instrc_ip.pb( make_pair( p_counter , lb_ins_op( lbl_name , instrc1 , npr ) ) ) ;
        p_counter = p_counter + disp ;
    }
    for (auto it : labels_used) {
        string label_name = it.F;
        int addr = it.S.address;
        if ((labels_declared.count(label_name) == 0)) {
            string error = "No  label as \"" + label_name + "\" has been Declared ";
            v_error.pb({addr, error});
        }
    }
    for (auto it : labels_declared) {
        string label_name = it.F;
        if ( (labels_used.count(label_name) == 0) ) {
            string err = "label with Name \"" + label_name + "\" not used but Declared ";
            warn.pb(make_pair(it.S.address, err ));
        }
    }
}
void second_pass_assembler() {
    for (auto &rl : instrc_ip) {
        int oprd_val;
        bool is_data_value_true = false;
        bool trp = 1;
        string instrc1 = rl.S.instrc;
        //cout << instrc1 << endl ;
        bool off_need = false;
        string instrc_value;
        int tmpr = 0;
        string oprd = rl.S.oprd;       
        string lbl_name = rl.S.label;
        int Location = rl.F;
   
        if (!instrc1.empty()) {
            instrc_value = opcode_table[instrc1].opcode;
            if (instrc1 == "halt") fl_halt = true;
            if ((opcode_table[instrc1].mnemonic_type == 2) )  off_need = 1;
            if ( (instrc1 == "data") ) is_data_value_true = true;
        }

        string f_final;
        if ( oprd.empty() ) {
            string tmp = "000000";
            f_final = tmp + instrc_value;
        }
        else {
            if ( !(labels_declared.count(oprd) == 0)) { oprd_val = labels_declared[oprd].address; }
            else  oprd_val = stoi(oprd);

            bool fl4 = ( off_need & ( labels_declared.count(oprd) != 0) ) ;
            if (fl4) oprd_val -= Location + 1;
            if ( !is_data_value_true ) {
                string tmp = hexadeci_generation(oprd_val, true);
                tmp += instrc_value;
                f_final = tmp;
            }
            else {
                string tmp = hexadeci_generation(oprd_val, false);
                f_final = tmp;
            }
        }
        string PC = hexadeci_generation(Location, false);
        string mac_code;
        string lis__code = PC;
        if (!instrc1.empty()) {
            mac_code += f_final;
            lis__code += " " + f_final;
        }
        else lis__code += "         ";

        if ( !lbl_name.empty() ) lis__code = lis__code + " " + lbl_name + ":";
        if ( !instrc1.empty() ) lis__code = lis__code + " " + instrc1;
        if ( !oprd.empty() ) lis__code = lis__code + " " + oprd;
        if ( !lis__code.empty() ) list_code.pb(lis__code);
        if ( !mac_code.empty() ) machine_code.pb(mac_code);        
    }
    if (!fl_halt) {
        string err  = "HALT Instruction Not Found ";
        warn.pb( {-1, err } );
    }
}

int main(int argc, char *argv[]) {
    opcode_table["adj"] = {"0A", 1};
    opcode_table["set"] = {"", 1};
    opcode_table["data"] = {"", 1};
    opcode_table["ldc"] = {"00", 1};
    opcode_table["adc"] = {"01", 1};
    opcode_table["ldl"] = {"02", 2};
    opcode_table["stl"] = {"03", 2};
    opcode_table["ldnl"] = {"04", 2};
    opcode_table["stnl"] = {"05", 2};
    opcode_table["call"] = {"0D", 2};
    opcode_table["brz"] = {"0F", 2};
    opcode_table["brlz"] = {"10", 2};
    opcode_table["br"] = {"11", 2};
    opcode_table["shr"] = {"09", 0};
    opcode_table["a2sp"] = {"0B", 0};
    opcode_table["sp2a"] = {"0C", 0};
    opcode_table["return"] = {"0E", 0};
    opcode_table["halt"] = {"12", 0};
    opcode_table["add"] = {"06", 0};
    opcode_table["sub"] = {"07", 0};
    opcode_table["shl"] = {"08", 0};

    if (argc != 2) {
        std :: cout << "ERROR: you can pass only .asm File ";
        return 0;
    }

    char *file_name = argv[1];

    if (strlen(file_name) < 4 or strcmp("asm", file_name + strlen(file_name) - 3)) {
        std :: cout << "ERROR: Incorrect file type " << std :: endl ;
        std :: cout << "ERROR: Enter file with .asm extension ";
        return 0;
    }

    string file_name_without_asm;
    int i1 = 0;
    for( ; ; )
    {
        if(argv[1][i1] == '.')break;
        file_name_without_asm += argv[1][i1];
        i1++;
    }
    inp_name = argv[1];

    mach_cd_file_name = file_name_without_asm + ".o";
    file_list = file_name_without_asm + ".l";
    file_log = file_name_without_asm + ".log";

    first_pass_assembler();

    if (v_error.size() > 0) {
        std :: cout << "Process Failed" << std :: endl ;
        std :: cout << "Errors Found" << std :: endl ;
        std :: sort(v_error.begin(), v_error.end());

        ofstream out_log(file_log);

        out_log << "Failed To Assemble" << std :: endl ;
        out_log << "ERRORS :- " << std :: endl ;
        std::vector<std::pair<int, std::string>>::iterator it;
        for (it = v_error.begin(); it != v_error.end(); ++it){
            std :: cout << "Error at Line " << it->F << " : " << it->S << std :: endl;
        }
        for (it = v_error.begin(); it != v_error.end(); ++it){
            out_log << "Error at Line " << it->F << " : " << it->S << std :: endl;
        }

        out_log.close();
        exit(0);
    }

    second_pass_assembler();
    if (warn.size() > 0) {
        sort(warn.begin(), warn.end());

        ofstream out_log(file_log);
        std :: cout << "Warnings" << std :: endl ;
        out_log << "Warnings" << std :: endl ;
        std::vector<std::pair<int, std::string>>::iterator i;
        for (i = warn.begin(); i != warn.end(); ++i){
            std :: cout << "Warning at Line " << i->F << " : " << i->S << std :: endl;
        }
        for (i = warn.begin(); i != warn.end(); ++i){
            out_log << "Warning at Line " << i->F << " : " << i->S << std :: endl;
        }
        out_log.close();
    }
    ofstream out_l(file_list);
    for(int i = 0 ; i < list_code.size() ; i++){
        out_l << list_code[i] << std :: endl;
    }
    out_l.close();

    ofstream outp_ob;

    outp_ob.open( mach_cd_file_name , ( ios::binary | ios::out ) );
    for (auto &j : machine_code) {
        unsigned int y;
        // cout << "j = " << j << endl; 
        stringstream ss;
        ss << hex << j;
        ss >> y;

        static_cast<int>(y);
        outp_ob.write((const char *)&y, sizeof(unsigned));
    }
    outp_ob.close();
    return 0;
}