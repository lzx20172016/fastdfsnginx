
CC=gcc
CPPFLAGS=-I ./include -I/usr/local/include/hiredis -I./redis_op
CFLAGS=-Wall
LIBS= -lfdfsclient -lpthread -lfcgi -lhiredis

main=./main
test_hiredis=./redis_op/test_hiredis
test_redis_op=./redis_op/test_redis_op
save_one_file=./redis_op/save_one_file
demo_cgi=./demo_cgi
echo_cgi=./echo_cgi
upload_cgi=./upload_cgi
target=$(main) $(demo_cgi) $(echo_cgi) $(upload_cgi) $(test_hiredis) $(test_redis_op) $(save_one_file)

ALL:$(target)

%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

$(demo_cgi):demo_cgi.o 
	$(CC) $^ -o $@ $(LIBS)
$(main):./main.o ./fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)
$(echo_cgi):echo_cgi.o 
	$(CC) $^ -o $@ $(LIBS)
$(upload_cgi):upload_cgi.o util.o ./redis_op/upload_file.o ./redis_op/redis_op.o ./redis_op/make_log.o ./redis_op/fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)
$(test_hiredis):./redis_op/test_hiredis_api.o
	$(CC) $^ -o $@ $(LIBS)
$(test_redis_op):./redis_op/test_redis_op.o ./redis_op/redis_op.o ./redis_op/make_log.o
	$(CC) $^ -o $@ $(LIBS)
$(save_one_file):./redis_op/save_one_file.o ./redis_op/redis_op.o ./redis_op/fdfs_api.o ./redis_op/make_log.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o
