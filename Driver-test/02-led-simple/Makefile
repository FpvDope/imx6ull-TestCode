KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88

all:	
	make -C $(KERN_DIR) M=`pwd` modules 	
	$(CROSS_COMPILE)gcc -o led_app led_app.c
  
clean:	
	make -C $(KERN_DIR) M=`pwd` modules clean	
	rm -rf modules.order	
	rm -f led_app

obj-m	+= led_drv.o
