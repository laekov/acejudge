#include "acejudge.h"

char compile_cpp_cmd[max_path];

void compile_init() {
	FILE *ipf = fopen("/etc/.ajcompilecpp", "r");
	if (ipf) {
		fgets(compile_cpp_cmd, max_path, ipf);
		fclose(ipf);
	}
	else
		strcpy(compile_cpp_cmd, default_compile_cpp_cmd);
}

void load_filename(char* fln, char* a) {
	while (*a && *a != '"')
		++ a;
	*fln = 0;
	if (!(*a))
		return;
	while (*(++ a) != '"')
		*(fln ++) = *a;
	*fln = 0;
}

int compile_cpp(prob_cfg& pcfg) {
	char od[max_path];
	char *gl = new char[max_line], *pt, *tmp;
	FILE *pf = fopen(pcfg. prg_name, "r");
	if (!pf)
		return 1;
	pcfg. fin = pcfg. fou = 0;
	while (!feof(pf)) {
		fgets(gl, max_line, pf);
		if (strstr(gl, "system(") || strstr(gl, "<con>") || strstr(gl, "fork") || strstr(gl, "unistd.h"))
			return 1;
		char *cp = strstr(gl, "//"), *bn;
		pt = gl;
		while ((pt = strstr(pt, "freopen(")) && (pt < cp || !cp)) {
			if (strstr(pt, "\"r\"")) {
				pcfg. fin = 1;
				load_filename(pcfg. prg_in, pt);
			}
			else if (strstr(pt, "\"w\"")) {
				pcfg. fou = 1;
				load_filename(pcfg. prg_ou, pt);
			}
			pt = strchr(pt, ')');
		}
	}
	fclose(pf);
	sprintf(od, compile_cpp_cmd, pcfg. prg_name);
	setcolor(0);
	return system(od);
}

int compile(prob_cfg& pcfg) {
	char od[max_path];
	char *p = strstr(pcfg. prg_name, ".");
	if (!p) {
		sprintf(od, "cp %s .ajtest/testtmp", pcfg. prg_name);
		return system(od);
	}
	else if (*(p + 1) == 'c')
		return compile_cpp(pcfg);
	else
		return -1;
}
