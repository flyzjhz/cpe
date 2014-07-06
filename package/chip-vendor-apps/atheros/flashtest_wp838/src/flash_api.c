#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>

struct mtd_info_user {
	u_char type;
	u_int32_t flags;
	u_int32_t size;
	u_int32_t erasesize;
	u_int32_t oobblock;
	u_int32_t oobsize;
	u_int32_t ecctype;
	u_int32_t eccsize;
};

struct erase_info_user {
	u_int32_t start;
	u_int32_t length;
};

#define MEMGETINFO	_IOR('M', 1, struct mtd_info_user)
#define MEMERASE	_IOW('M', 2, struct erase_info_user)

int mtd_open(const char *name, int flags)
{
	FILE *fp;
	char dev[80];
	int i, ret;

	if ((fp = fopen("/proc/mtd", "r"))) {
		while (fgets(dev, sizeof(dev), fp)) {
			if (sscanf(dev, "mtd%d:", &i) && strstr(dev, name)) {
				snprintf(dev, sizeof(dev), "/dev/mtd/%d", i);
				if ((ret = open(dev, flags)) < 0) {
					snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
					ret = open(dev, flags);
				}
				fclose(fp);
				return ret;
			}
		}
		fclose(fp);
	}
	return -1;
}

int flash_read_mac(char *buf)
{
	int fd, ret;

	if (!buf)
		return -1;
	fd = mtd_open("Factory", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device\n");
		return -1;
	}
#if ! defined (NO_WIFI_SOC)
	lseek(fd, 0x2E, SEEK_SET);
#else
	lseek(fd, 0xE006, SEEK_SET);
#endif
	ret = read(fd, buf, 6);
	close(fd);
	return ret;
}

int flash_read_NicConf(char *buf)
{
	int fd, ret;

	if (!buf)
		return -1;
	fd = mtd_open("Factory", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device\n");
		return -1;
	}
	lseek(fd, 0x34, SEEK_SET);
	ret = read(fd, buf, 6);
	close(fd);
	return ret;
}

int flash_read(char *buf, off_t from, size_t len)
{
	int fd, ret;
	struct mtd_info_user info;

	//fd = mtd_open("Config", O_RDONLY);
	fd = mtd_open("nvram", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device\n");
		return -1;
	}

	if (ioctl(fd, MEMGETINFO, &info)) {
		fprintf(stderr, "Could not get mtd device info\n");
		close(fd);
		return -1;
	}
	if (len > info.size) {
		fprintf(stderr, "Too many bytes - %d > %d bytes\n", len, info.erasesize);
		close(fd);
		return -1;
	}

	lseek(fd, from, SEEK_SET);
	ret = read(fd, buf, len);
	if (ret == -1) {
		fprintf(stderr, "Reading from mtd failed\n");
		close(fd);
		return -1;
	}

	close(fd);
	return ret;
}

#define min(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x < _y ? _x : _y; })

int flash_write(char *buf, off_t to, size_t len)
{
	int fd, ret = 0;
	char *bak = NULL;
	struct mtd_info_user info;
	struct erase_info_user ei;

	//fd = mtd_open("Config", O_RDWR | O_SYNC);
	fd = mtd_open("nvram", O_RDWR | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device\n");
		return -1;
	}

	if (ioctl(fd, MEMGETINFO, &info)) {
		fprintf(stderr, "Could not get mtd device info\n");
		close(fd);
		return -1;
	}
	if (len > info.size) {
		fprintf(stderr, "Too many bytes: %d > %d bytes\n", len, info.erasesize);
		close(fd);
		return -1;
	}

	while (len > 0) {
		if ((len & (info.erasesize-1)) || (len < info.erasesize)) {
			int piece_size;
			unsigned int piece, bakaddr;

			bak = (char *)malloc(info.erasesize);
			if (bak == NULL) {
				fprintf(stderr, "Not enough memory\n");
				close(fd);
				return -1;
			}

			bakaddr = to & ~(info.erasesize - 1);
			lseek(fd, bakaddr, SEEK_SET);

			ret = read(fd, bak, info.erasesize);
			if (ret == -1) {
				fprintf(stderr, "Reading from mtd failed\n");
				close(fd);
				free(bak);
				return -1;
			}

			piece = to & (info.erasesize - 1);
			piece_size = min(len, info.erasesize - piece);
			memcpy(bak + piece, buf, piece_size);

			ei.start = bakaddr;
			ei.length = info.erasesize;
			if (ioctl(fd, MEMERASE, &ei) < 0) {
				fprintf(stderr, "Erasing mtd failed\n");
				close(fd);
				free(bak);
				return -1;
			}

			lseek(fd, bakaddr, SEEK_SET);
			ret = write(fd, bak, info.erasesize);
			if (ret == -1) {
				fprintf(stderr, "Writing to mtd failed\n");
				close(fd);
				free(bak);
				return -1;
			}

			free(bak);
			buf += piece_size;
			to += piece_size;
			len -= piece_size;
		}
		else {
			ei.start = to;
			ei.length = info.erasesize;
			if (ioctl(fd, MEMERASE, &ei) < 0) {
				fprintf(stderr, "Erasing mtd failed\n");
				close(fd);
				return -1;
			}

			ret = write(fd, buf, info.erasesize);
			if (ret == -1) {
				fprintf(stderr, "Writing to mtd failed\n");
				close(fd);
				free(bak);
				return -1;
			}

			buf += info.erasesize;
			to += info.erasesize;
			len -= info.erasesize;
		}
	}

	close(fd);
	return ret;
}



int main(int argc, char* argv[]){
#if 0
   //arg1:tmp_file arg2:content number arg3:nvram name
   int test_mem=atoi(argv[2]), i=0;
   char content[test_mem];

   for(;i<test_mem; i++){

     content[i]='a'+i%26;
   }
   content[i]='\0';

   char shell[test_mem+100];
   memset(shell, 0, sizeof(shell));
   
   sprintf(shell, "echo %s > %s", content, argv[1]);
   system(shell);
   memset(shell, 0, sizeof(shell));
   sprintf(shell, "nvram fset %s=%s", argv[3], content);
   //printf("shell=%s\n", shell);
   system(shell);
   system("nvram commit");
#else 
	int i=0;
	int test_mem=65535;
	//int test_mem=atoi(argv[2]);
	int result;
	//char content[test_mem];
	//char buf[test_mem];
	//char shell[100];
	char * content=NULL;
	char * buf=NULL;
	char * shell=NULL;
	content = (char *)malloc(test_mem);
	buf=(char *)malloc(test_mem);
       shell=(char *)malloc(100);

	
	for(i=0;i<test_mem; i++){

    		 content[i]='a'+i%26;
   	}
   	//content[i]='\0';
	//printf("==========1=============\n");
	flash_write(content, 0x0, test_mem);
	//printf("==========2=============\n");
	//memset(buf,0,sizeof(buf));
	memset(buf,0,test_mem);
	//printf("==========3=============\n");
	flash_read(buf,0x0,test_mem);	
	//printf("==========4=============\n");
	result= memcmp(content,buf,test_mem);
//	memcmp(content,buf,test_mem);
	//printf("==========5 == %d===========\n",result);

	   if(result == 0){
      		sprintf(shell,"echo Test result: PASS > %s", argv[1]);
     
   		}else{
      		sprintf(shell,"echo Test result: FAIL > %s", argv[1]);
      
   	}
   	system(shell);
	
	free(content);
	content=NULL;
	
	free(buf);
	buf=NULL;
	
	free(shell);
	shell=NULL;
	
	return 0;

#endif

 
}

