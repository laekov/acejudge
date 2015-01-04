#include "acejudge.h"

prob_cfg :: prob_cfg() {
	empty = 1;
	nofile = 1;
}

void prob_cfg :: load(char *gfn = 0) {
	if (!gfn) {
		puts("Cfg file name:");
		get_str(cfg_name);
	}
	else
		strcpy(cfg_name, gfn);
	for (char *g = cfg_name; *g; g ++)
		if (*g == 10)
			*g = 0;
	FILE *ipf = fopen(cfg_name, "r");
	if (!ipf) {
		ipf = fopen(cfg_name, "w");
		fclose(ipf);
		nofile = 0;
		return file_wrong();
	}
	else {
		fscanf(ipf, "%s", ipt_fmt);
		fscanf(ipf, "%s", opt_fmt);
		fscanf(ipf, "%d%d%d%d", &beg_num, &end_num, &time_lmt, &mem_lmt);
		fscanf(ipf, "%s", prg_name);
		fclose(ipf);
		empty = 0;
		nofile = 0;
	}
}

void prob_cfg :: make(char *gfn = 0) {
	if (gfn)
		strcpy(cfg_name, gfn);
	else if (nofile) {
		puts("Cfg file name:");
		get_str(cfg_name);
	}
	for (char *g = cfg_name; *g; g ++)
		if (*g == 10)
			*g = 0;
	FILE *opf = fopen(cfg_name, "w");
	if (opf) {
		fprintf(opf, "%s\n%s\n", ipt_fmt, opt_fmt);
		fprintf(opf, "%d %d %d %d\n", beg_num, end_num, time_lmt, mem_lmt);
		fprintf(opf, "%s\n", prg_name);
		fclose(opf);
		nofile = 0;
	}
	else
		file_wrong();
}

void prob_cfg :: show() {
	if (empty) {
		setcolor(31);
		puts("Not set");
	}
	else {
		setcolor(33);
		puts("Input data format");
		setcolor(32);
		puts(ipt_fmt);

		setcolor(33);
		puts("Output data format");
		setcolor(32);
		puts(opt_fmt);

		setcolor(33);
		puts("Begin - End");
		setcolor(32);
		printf("%d - %d\n", beg_num, end_num);

		setcolor(33);
		puts("Time limit");
		setcolor(32);
		printf("%d ms\n", time_lmt);

		setcolor(33);
		puts("Memory limit");
		setcolor(32);
		printf("%d MB\n", mem_lmt);

		setcolor(33);
		puts("Current testing file name");
		setcolor(32);
		puts(prg_name);

		setcolor(0);
	}
}

void prob_cfg :: get() {
	char fmt[max_path], suf[max_path];
	setcolor(33);
	fmt[0] = 0;
	puts("IO data format");
	get_str(fmt);
	if (fmt[0]) {
		puts("Input data sufix");
		get_str(suf);
		sprintf(ipt_fmt, "%s%s", fmt, suf);
		puts("Output data sufix");
		get_str(suf);
		sprintf(opt_fmt, "%s%s", fmt, suf);
	}
	puts("Begin - End");
	get_int(beg_num);
	get_int(end_num);
	puts("Time limit(ms)");
	get_int(time_lmt);
	puts("Memory limit(MB)");
	get_int(mem_lmt);
	empty = 0;
}

void config(prob_cfg& c) {
	puts("C - Config problem");
	puts("L - Show cfg now");
	puts("E - Load existing cfg file");
	puts("W - Save cfg file");
	switch (get_cmd()) {
		case 'C':
			c. get();
			break;
		case 'L':
			c. show();
			break;
		case 'E':
			c. load();
			break;
		case 'W':
			c. make();
			break;
	}
	if (!c. nofile)
		c. make();
}
