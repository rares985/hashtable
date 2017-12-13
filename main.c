#include "hashtable.h"


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 64

int tokenize(char line[], char **name, char **farg, char **sarg)
{
	char *token = NULL;

	token = strtok(line, " ");
	*name = token;
	token = strtok(NULL, " ");
	*farg = token;
	token = strtok(NULL, " ");
	*sarg = token;
	if (strtok(NULL, " ") != NULL)
		return -1;
	return 0;
}

void report_error(const char *message) {
	fprintf(stderr, "[ERROR]:%s\n", message);
}

int execute(struct hashtable *ht, char *name, char *farg, char *sarg)
{
	int fd = STDOUT_FILENO;

	if (!name) {
		report_error("Invalid command");
		return -2;
	}
	if (strcmp(name, "exit") == 0) {
		return -2;
	}
	else if (strcmp(name, "help") == 0) {
		printf("Permitted commands:\nadd <word> - Adds a word to the hashtable\n");
		printf("remove <word> - Removes a word from the hashtable\nfind <word> [file]- Finds a word in the hashtable\n");
		printf("print_bucket <index> [file]- Prints the corresponding bucket\n");
		printf("print [file]- Prints the whole hashtable\nresize <halve/double> - Resizes the hashtable and moves all entries\n");
		printf("<arg> means mandatory argument, [arg] means optional argument\n");
	} 
	else if (strcmp(name, "add") == 0) {
		if (sarg || !farg) {
			report_error("Invalid arguments");
			return -2;
		}
		add_value(ht, farg);
	} else if (strcmp(name, "remove") == 0) {
		if (sarg || !farg) {
			report_error("Invalid arguments");
			return -2;
		}
		remove_value(ht, farg);
	} else if (strcmp(name, "find") == 0) {
		if (!farg) {
			report_error("Invalid arguments");
			return -2;
		}
		if (sarg) {
			fd = open(sarg, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) {
				report_error("Invalid file descriptor");
				return -1;
			}
		}
		find_value(ht, farg, fd);
		if (fd != 1)
			close(fd);
	} else if (strcmp(name, "clear") == 0) {
		if (farg || sarg) {
			report_error("Invalid arguments");
			return -1;
		}
		clear_hashtable(ht);
	} else if (strcmp(name, "print_bucket") == 0) {
		uint32_t index = strtol(farg, NULL, 10);

		if (index == 0 || !farg) {
			report_error("Invalid arguments");
			return -1;
		}
		if (sarg) {
			fd = open(sarg, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) {
				report_error("Invalid file descriptor");
				return -1;
			}
		}
		print_bucket(ht, index, fd);
		if (fd != 1)
			close(fd);
	} else if (strcmp(name, "print") == 0) {
		if (sarg) {
			report_error("Invalid arguments");
			return -2;
		}
		if (farg) {
			fd = open(farg, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) {
				report_error("Invalid file descriptor");
				return -1;
			}
		}
		print_hashtable(ht, fd);
		if (fd != 1)
			close(fd);
	} else if (strcmp(name, "resize") == 0) {
		if (sarg || !farg) {
			report_error("Invalid arguments");
			return -2;
		}
		double modifier = strcmp(farg, "double") == 0 ? 2.0 :
				strcmp(farg, "halve") == 0 ? 0.5 : -1;
		if (modifier < 0) {
			report_error("Invalid arguments");
			return -2;
		}
		resize(ht, modifier);
	} else {
		report_error("Invalid command");
		return -2;
	}
	return 0;
}
int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <size> [in1] ...\n", argv[0]);
		return -1;
	}

	uint32_t size = strtol(argv[1], NULL, 10);
	struct hashtable *ht = create_hashtable(size);

	char line[BUFSIZE];
	char *name = NULL;
	char *farg = NULL;
	char *sarg = NULL;
	int ret_val;
	char *rc;

	int i;
	FILE *f = NULL;

	if (argc == 2) {
		while (1) {
			rc = fgets(line, BUFSIZE, stdin);
			if (!rc) {
				report_error("read");
				delete_hashtable(ht);
				return -1;
			}
			line[strcspn(line, "\n")] = 0;
			tokenize(line, &name, &farg, &sarg);
			ret_val = execute(ht, name, farg, sarg);
			if (ret_val == -2) {
				delete_hashtable(ht);
				return -2;
			}
		}
	} else {
		for (i = 2; i < argc; i++) {
			f = fopen(argv[i], "r");
			if (!f) {
				report_error("opening file");
				continue;
			}
			while (1) {
				rc = fgets(line, BUFSIZE, f);
				if (strcmp(line, "\n") == 0)
					continue;
				if (rc == NULL) {
					report_error("in read");
					fclose(f);
					break;
				}
				line[strcspn(line, "\n")] = 0;
				if (feof(f)) {
					fclose(f);
					break;
				}
				ret_val = tokenize(line, &name, &farg, &sarg);
				if (ret_val < 0) {
					report_error("Too many arguments");
					continue;
				}
				ret_val = execute(ht, name, farg, sarg);
				if (ret_val == -2)
					return -2;
				if (ret_val < 0)
					continue;
			}
		}
	}
	delete_hashtable(ht);

	return 0;
}
