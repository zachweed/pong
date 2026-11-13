pong:
	cc -Wall pong.c board.c ball.c paddle.c court.c timer.c -o pong -lncurses

clean:
	rm -rf ./pong