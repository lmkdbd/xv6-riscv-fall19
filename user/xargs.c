#include "kernel/types.h"
#include "user/user.h"

#define MAXARGS 10
#define BUFFER_SIZE 32
int main(int argc, const char* argv[]){
		char* pass[MAXARGS];
		char input_data[MAXARGS][BUFFER_SIZE];
		for (int i = 0; i < MAXARGS; i++){
			pass[i] = input_data[i];
		}
		char temp[BUFFER_SIZE];
		int pass_count = 0;
		for (int i = 1; i < argc; i++){
				strcpy(input_data[pass_count], argv[i]);
				pass_count += 1;
		}
		int n = 0;
		int pos = 0;
		while((n = read(0,temp,BUFFER_SIZE)) > 0){
				char* current = input_data[pass_count];
				for (int i = 0; i < n; i++){
						if (temp[i] == ' ' || temp[i] == '\n'){
								current[++pos] = '\0';
								pos = 0;
								if ((++pass_count) > MAXARGS){
										fprintf(2,"too many args!");
										exit();
								}
								current = input_data[pass_count];
						} else {
								current[pos++] = temp[i];
						}
				}
		}
		input_data[pass_count][pos] = '\0';
		pass[++pass_count] = 0;

		if (n < 0){
				fprintf(2,"read error!");
				exit();
		}
		int fd = fork();
		if (fd < 0){
				fprintf(2,"can't fork!");
		} else if (fd > 0){
				//parent
				wait();
		} else {
				exec(pass[0], pass);
		}
		exit();
}
