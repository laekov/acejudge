#include "acejudge.h"

run_res mkres(int res, int time, int mem) {
	run_res r;
	r. res_num = res;
	r. time = time;
	r. mem = mem;
	return r;
}

/*color list:
 * 30 : gray
 * 31 : red
 * 32 : green
 * 33 : yellow
 * 34 : blue
 * 35 : purple
 * 36 : olive
*/
void setcolor(int x) {
	static int now_color = 0, prv_color;
	if (x == -1) {
		swap(now_color, prv_color);
	}
	else {
		prv_color = now_color;
		now_color = x;
	}
	printf("\33[%dm", now_color);
}
bool is_letter(int x) {
	return (x > 64 && x < 92) || (x > 96 && x < 124);
}
void get_char_pre() {
	struct termios new_s;
	struct termios old_s;
	tcgetattr(0, &old_s);
	new_s = old_s;
	new_s. c_lflag &= (~ICANON);
//	new_s. c_cc[VTIME] = 0;
//	new_s. c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_s);
}
void get_char_undo() {
	struct termios new_s;
	struct termios old_s;
	tcgetattr(0, &old_s);
	new_s = old_s;
	new_s. c_lflag |= ICANON;
//	new_s. c_cc[VTIME] = 0;
//	new_s. c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_s);
}
int get_cmd() {
	char o;
	get_char_pre();
	do
		o = getchar();
	while (!is_letter(o));
	get_char_undo();
	putchar(10);
	return (o > 96) ? (o - 32) : o;
}
void get_key() {
	get_char_pre();
	getchar();
	get_char_undo();
}
void get_str(char *str) {
	char buf[max_path], od[max_path];
	setcolor(35);
	while (1) {
		scanf("%s", buf);
		if (strcmp(buf, "old") == 0)
			break;
		else if (strcmp(buf, "ls") == 0) {
			scanf("%s", buf);
			setcolor(32);
			puts("File list");
			setcolor(37);
			sprintf(od, "ls %s", buf);
			system(od);
			setcolor(35);
		}
		else {
			strcpy(str, buf);
			break;
		}
	}
	setcolor(-1);
}
void get_int(int &num) {
	char buf[max_path];
	setcolor(35);
	scanf("%s", buf);
	if (strcmp(buf, "\\o"))
		sscanf(buf, "%d", &num);
	setcolor(-1);
}
void file_wrong() {
	//setcolor(31);
	puts("Wrong file");
}

inline void str_clr(char*) {
//	if (x)
//		delete x;
}
inline void str_new(char* &x) {
	str_clr(x);
	x = new char[max_path];
}
void str_cut(char *p) {
	while (*p)
		if (*p == '\"') {
			*p = 0;
			break;
		}
		else
			p ++;
}
