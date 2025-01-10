lsb_steg: encode.o test_encode.o decode.o
	gcc encode.o test_encode.o decode.o -o lsb_steg

encode.o: encode.c encode.h types.h common.h
	gcc -c encode.c

test_encode.o: test_encode.c encode.h types.h decode.h
	gcc -c test_encode.c

decode.o: decode.c decode.h types.h common.h
	gcc -c decode.c

clean:
	rm *.o *.out lsb_steg Output.txt stego_image.bmp enc_completion_check.txt


ENCODE: beautiful.bmp secret.txt
	./lsb_steg -e beautiful.bmp secret.txt

DECODE: 
	./lsb_steg -d stego_image.bmp
