#include "acejudge.h"

int str_read_int(char* a) {
	int s = 0;
	while (*a && !isdigit(*a))
		++ a;
	if (!*a)
		return -1;
	while (s = s * 10 + *a - 48, isdigit(*(++ a)));
	return s;
}

run_res watch(prob_cfg& pcfg, pid_t pid) {
	run_res ret;
	int status;
   
	/*
	int	tmem = 0;
	char stfln[max_path], tmp[max_path];
	sprintf(stfln, "/proc/%d/status", pid);
	while (1) {
		FILE* stfl = fopen(stfln, "r");
		if (!stfl)
			break;
		int cmem = -1;
		while (fgets(tmp, sizeof(tmp), stfl), !feof(stfl))
			if (strstr(tmp, "VmData"))
				cmem = str_read_int(tmp);
		fclose(stfl);
		if (cmem > tmem)
			tmem = cmem;
		else if (cmem == -1)
			break;
	}
	*/
	struct rusage ru;
	wait4(pid, &status, 0, &ru);

	//ret. time = ((te. tv_sec - tb. tv_sec)* 1000000 + (te. tv_usec - tb. tv_usec)) / 1000;
	ret. time = ru. ru_utime. tv_sec * 1000 + ru. ru_utime. tv_usec / 1000;
	//ret. mem = max((long)tmem, ru. ru_maxrss);
	ret. mem = ru. ru_maxrss;

	if (ret. mem > pcfg. mem_lmt * 1024)
		ret. res_num = -2;
	else if (ret. time > pcfg. time_lmt)
		ret. res_num = -1;
	else if (WTERMSIG(status))
		ret. res_num = -3;
	else
		ret. res_num = 0;
	return ret;
}

run_res run_case(prob_cfg& pcfg, int id) {
	char ifn[max_path], ofn[max_path];
	char od[max_path], arglg[max_path], argrp[max_path];
	sprintf(ifn, pcfg. ipt_fmt, id);
	sprintf(arglg, "runtime%d.log", id);
	sprintf(ofn, pcfg. opt_fmt, id);
	if (pcfg. fin) {
		sprintf(od, "cp %s .ajtest/%s", ifn, pcfg. prg_in);
		system(od);
	}
	else {
		sprintf(od, "cp %s .ajtest/testtmp.in", ifn);
		system(od);
	}

	struct rlimit lmtCPU, lmtMem, lmtN;
	char old_dir[max_path];
	getcwd(old_dir, sizeof(old_dir));
	sprintf(argrp, "%s/.ajtest/testtmp", old_dir);
	strcpy(od, argrp);
	chdir(".ajtest");
	getrlimit(RLIMIT_CPU, &lmtCPU);
	getrlimit(RLIMIT_AS, &lmtMem);
	run_res ret;


	pid_t pid = fork();

	if (!pid) {
		if (!pcfg. fin)
			freopen("testtmp.in", "r", stdin);
		if (!pcfg. fou)
			freopen("testtmp.out", "w", stdout);
		freopen(arglg, "w", stderr);
		lmtN. rlim_cur = pcfg. mem_lmt * 1024 * 1280;
		lmtN. rlim_max = pcfg. mem_lmt * 1024 * 1280;
		setrlimit(RLIMIT_AS, &lmtN);
		lmtN. rlim_cur = (pcfg. time_lmt + 100) / 1000 + 1;
		lmtN. rlim_max = (pcfg. time_lmt + 100) / 1000 + 1;
		setrlimit(RLIMIT_CPU, &lmtN);
		execl(od, argrp, (char*)0);
	}
	else {
		setcolor(36);
		printf("Test case %d	", id);
		ret = watch(pcfg, pid);
	}

	setrlimit(RLIMIT_CPU, &lmtCPU);
	setrlimit(RLIMIT_AS, &lmtMem);
	chdir(old_dir);
	return ret;
}

int judge_case(prob_cfg& pcfg, int id) {
	char ifn[max_path], ofn[max_path], od[max_path];
	sprintf(ifn, pcfg. ipt_fmt, id);
	sprintf(ofn, pcfg. opt_fmt, id);
	if (pcfg. fou) {
		//sprintf(od, "mv %s %s", prg_opt, prg_opt);
		//system(od);
		sprintf(od, "diff -s -w .ajtest/%s %s >.ajtest/diff%d.log", pcfg. prg_ou, ofn, id);
	}
	else {
		sprintf(od, "diff -s -w .ajtest/testtmp.out %s >.ajtest/diff%d.log", ofn, id);
	}
	int dret = system(od);
	return !dret;
}

void clean_test(prob_cfg& pcfg) {
	char od[max_path];
	system("rm .ajtest/testtmp");
	if (pcfg. fin)
		sprintf(od, "rm .ajtest/%s", pcfg. prg_in);
	else
		strcpy(od, "rm .ajtest/testtmp.in");
	system(od);
	if (pcfg. fin)
		sprintf(od, "rm .ajtest/%s", pcfg. prg_ou);
	else
		strcpy(od, "rm .ajtest/testtmp.out");
	system(od);
}

void judge(prob_cfg& pcfg, bool fastrun) {
	if (!fastrun) {
		setcolor(33);
		puts("Program name");
		get_str(pcfg. prg_name);
	}
	if (pcfg. empty) {
		setcolor(31);
		puts("No configure is loaded");
		return;
	}
	pcfg. make(0);
	setcolor(33);
	printf("Testing program: ");
	setcolor(35);
	puts(pcfg. prg_name);
	if (compile(pcfg)) {
		setcolor(31);
		puts("Compile error!");
	}
	else {
		setcolor(32);
		puts("Compile ok");
		int ca = 0, ct = pcfg. end_num - pcfg. beg_num + 1, tot_time = 0, max_mem = 0;
		for (int i = pcfg. beg_num; i <= pcfg. end_num; i ++) {
			run_res res = run_case(pcfg, i);
			if (res. res_num < 0) {
				setcolor(33);
				if (res. res_num == -1)
					puts("Time limit exceeded");
				else if (res. res_num == -2)
					puts("Memory limit exceeded");
				else
					puts("Run time error");
			}
			else {
				int dres = judge_case(pcfg, i);
				if (!dres) {
					setcolor(31);
					printf("Wrong Answer	");
				}
				else {
					setcolor(32);
					printf("Accepted	");
					tot_time += res. time;
					max_mem = max(max_mem, res. mem);
					ca ++;
				}
				printf("\33[33mtime: \33[35m%d\33[33m MS	memory: \33[35m%d\33[33m KB\n", res. time, res. mem);
			}
		}
		setcolor(34);
		printf("Score ratio: ");
		if (ca == ct)
			setcolor(32);
		else
			setcolor(31);
		printf("%.2lf%%\n", (double)ca/(double)ct * 100.0);
		setcolor(0);
		printf("Valid time used: \33[35m%d\33[0m ms	Max memory used: \33[35m%d\33[0m KB\n", tot_time, max_mem);
	}
	clean_test(pcfg);
	puts("\33[32mPress any key to continue\33[0m");
	get_key();
}
