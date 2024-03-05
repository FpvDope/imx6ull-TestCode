#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
 
/*
 * ./hello_drv_test -w abc
 * ./hello_drv_test -r
 */
int main(int argc, char *argv[]){
  int fd;
  int len;
  char buf[1024];

  /*1.判断参数*/
  if(argc < 2){
		printf("Usage: %s -w <string>\n", argv[0]);
		printf("       %s -r\n", argv[0]);
    return -1;
  }

  /*2.打开设备文件*/
  fd = open("/dev/hello", O_RDWR);
  if(fd == -1){
    perror("open");
    return -1;
  }
  /*3.读写操作*/
  if(0 == (strcmp(argv[1], "-w")) && (argc == 3)){
    len = strlen(argv[2]) + 1;
		len = len < 1024 ? len : 1024;
		write(fd, argv[2], len);
  }else{
    len = read(fd, buf, 1024);		
		buf[1023] = '\0';
		printf("APP read : %s\n", buf);
  }

  /*4.关闭设备文件*/
  close(fd);
  
  return 0;
}
