MW_DIR=$(PWD)
EBIN=$(MW_DIR)/ebin
#INCLUDE = -I$(MW_DIR)/include
#INCLUDE+= -I.
#LIBPATH=-L $(MW_DIR)/lib     //如果有库文件,可以这样指定
LIB= -lpthread
CC=gcc
CFLAGS=-DHI_DEBUG -g -Wall

#加入内部的头文件
SAMDIR=$(MW_DIR)/src
#SAMINCH=$(shell find $(SAMDIR) -name '*.h')
#SAMINCDIR=$(dir $(SAMINCH))
#INCLUDE+= $(foreach temp,$(SAMINCDIR), -I$(temp)) 

#查找到需要编译的C文件
SRC=$(shell find $(SAMDIR) -name '*.c')#子目录的C文件
SRC+=$(wildcard *.c)#当前目录中的C文件

APP=$(SRC:%.c=%.o)#得到.o文件

all  :  $(APP)
	$(CC) $^ -o $(EBIN)/main $(INCLUDE) $(LIBPATH) $(LIB)

%.o: %.c#$(APP): 
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -fr $(APP)
.PHONY:clean