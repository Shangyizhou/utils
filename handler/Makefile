all: handler_test

handler_test: handler_test.cc Handler.cc Looper.cc Message.cc MessageQueue.cc 
	g++ handler_test.cc Handler.cc Looper.cc Message.cc MessageQueue.cc  -g -lpthread -o handler_test

clean:
	rm -r hander_test