#ifndef ACEJUDGE_H
#define ACEJUDGE_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>

#include <algorithm>

#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

using namespace std;

typedef long long qw;
typedef unsigned int uint;
typedef struct {
	int res_num, time, mem;
} run_res;
run_res mkres(int, int, int);

const int max_path = 256;
const int max_line = 1024;
const char default_compile_cpp_cmd[] = "g++ %s -g -o ajtest/testtmp";

struct prob_cfg {
	bool empty, nofile;
	char prg_name[max_path];
	char cfg_name[max_path];
	char ipt_fmt[max_path], opt_fmt[max_path];
	int beg_num, end_num, time_lmt, mem_lmt;
	char prg_in[max_path], prg_ou[max_path];
	bool fin, fou;
	prob_cfg();
	void load(char*);
	void make(char*);
	void show();
	void get();
	void config();
};

void setcolor(int);

void get_char_pre();
void get_char_undo();
int get_cmd();
void get_key();
void get_str(char*);
void get_int(int&);
void file_wrong();

void str_cut(char*);

void show_cmd();

void config(prob_cfg&);

void compile_init();
int compile(prob_cfg&);

void judge(prob_cfg&, bool);

void show_help();
#endif

