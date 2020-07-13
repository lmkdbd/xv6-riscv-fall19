#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define PATH_SIZE 512
char *fmtname(char *path)
{
		static char fmt_buffer[DIRSIZ + 1];
		memset(fmt_buffer, 0, DIRSIZ + 1);
		char *p = path + strlen(path);
		while (p >= path)
		{
				if ((*p) == '/')
				{
						p++;
						break;
				}
				p--;
		}
		if (strlen(p) > DIRSIZ)
		{
				fprintf(2, "Invalid file length\n");
				exit();
		}
		memmove(fmt_buffer, p, strlen(p));
		return fmt_buffer;
}

void checkdir(char *path, char *target)
{
		if (!strcmp(fmtname(path), ".") || !strcmp(fmtname(path), ".."))
		{
				return;
		}
		struct stat st;
		if (stat(path, &st) < 0)
		{
				fprintf(2, "can't get stat!\n");
				return;
		}
		if (st.type != T_DIR)
		{
				return;
		}
		int fd = open(path, 0);
		if (fd < 0)
		{
				fprintf(2, "can't open this dir\n");
				return;
		}
		struct dirent de;
		while (read(fd, &de, sizeof(de)) == sizeof(de))
		{
				if (de.inum == 0)
				{
						continue;
				}
				if (strlen(path) + 1 + DIRSIZ + 1 > PATH_SIZE)
				{
						fprintf(2, "file length is larger than buffer size\n");
						continue;
				}
				char *p = path + strlen(path);
				*p++ = '/';
				memmove(p, de.name, DIRSIZ);
				if (stat(path, &st) < 0)
				{
						fprintf(2, "can't open this dir\n");
						goto clear;
				}
				if (!strcmp(target, fmtname(path)))
				{
						fprintf(1, "%s\n", path);
				}
				if (st.type != T_DIR)
				{
						goto clear;
				}
				checkdir(path, target);
clear:
				p--;
				p[DIRSIZ + 1] = 0;
				memset(p, 0, DIRSIZ + 1);
		}
		close(fd);
}

int main(int argc, const char *argv[])
{
		if (argc != 3)
		{
				fprintf(2, "usage: find dir target\n");
				exit();
		}

		char p_buffer[PATH_SIZE] = {0};
		if (strlen(argv[1]) > PATH_SIZE - 1)
		{
				fprintf(2, "target length out of range\n");
				exit();
		}
		memmove(p_buffer, argv[1], strlen(argv[1]));

		char t_buffer[DIRSIZ + 1] = {0};
		if (strlen(argv[2]) > DIRSIZ)
		{
				fprintf(2, "target length out of range\n");
				exit();
		}
		memmove(t_buffer, argv[2], strlen(argv[2]));

		checkdir(p_buffer, t_buffer);

		exit();
}
