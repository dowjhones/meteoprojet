NAME	=	meteo_sort #binary name

CC	=	gcc #binary for the compilation

RM	=	rm -f #command to remove files

SRCS	=	 $(wildcard src/*.c) #spource file

OBJS	=	$(SRCS:.c=.o) #name of .o to save the compilation of the file

CFLAGS	=	-Wall -g -I include/ #flags for warning valgrind and folder of include headers

all:	$(NAME) #make 

$(NAME):	$(OBJS) #compile
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

clean: #make clean  remove all .o
	$(RM) $(OBJS)

fclean:	clean #make fclean remove all .o et and the binary
	$(RM) $(NAME)

re:	fclean all #make re remove all .o et and the binary and compile

.PHONY:	all clean fclean re #list of rules
