/*
 * Ace Judge
 * Source code
 * A simple judge software for OIers
 * Code by CDQZ_laekov
 * Version on linux
 * Last change on 12/31 2014
 */

#include "acejudge.h"

int main(int argc, char* args[]) {
	//puts("RUNNING");
	bool flag = 1, fastrun = 0;
	freopen(".ajruntime", "w", stderr);
	system("mkdir .ajtest");
	compile_init();
	prob_cfg ccfg;
	for (int i = 1; i < argc; ++ i)
		if (args[i][0] != '-')
			ccfg. load(args[i]);
	for (int i = 1; i < argc; ++ i)
		if (args[i][0] == '-') {
			if (!strcmp(args[i] + 1, "fast"))
				fastrun = 1;
			else if (!strcmp(args[i] + 1, "l")) {
				ccfg. show();
				return 0;
			}
			else if (!strcmp(args[i] + 1, "help")) {
				show_help();
				return 0;
			}
		}
	if (fastrun) {
		judge(ccfg, 1);
	}
	else {
		while (1) {
			if (flag)
				show_cmd(), flag = 0;
			switch (get_cmd()) {
				case 'C':
					config(ccfg);
					flag = 1;
					break;
				case 'R':
					judge(ccfg, 1);
					flag = 1;
					break;
				case 'Q':
					return 0;
				default:
					flag = 0;
					break;
			}
		}
	}
}
