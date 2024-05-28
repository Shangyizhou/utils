all: redis_test stream_test

redis_test: redis_test.cc RedisClient.cc
	g++ redis_test.cc RedisClient.cc -g -lhiredis -lpthread -o redis_test

stream_test: stream_test.cc RedisClient.cc
	g++ stream_test.cc RedisClient.cc -g -lhiredis -lpthread -o stream_test

clean:
	rm -r redis_test