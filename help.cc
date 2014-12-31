#include "acejudge.h"

const char help_text[] = "\
acejudge [config file] [-fast] [-l] [-help] \n\
config file: load config file automaticly \n\
-fast: judge without configing (need config file) \n\
-l: show what is in the config file (need config file) \n\
-help: show this page \
";

void show_help() {
	setcolor(0);
	puts(help_text);
}
