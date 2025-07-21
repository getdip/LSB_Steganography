lsb_steg: encode.o test_encode.o decode.o
	@gcc encode.o test_encode.o decode.o -o lsb_steg
	@echo "To Encode secret file execute 'make ENCODE INFILE=<filename_with_extension>'"
	@echo "Default command 'make ENCODE' will encode secret.txt"

encode.o: encode.c encode.h types.h common.h
	@gcc -c encode.c

test_encode.o: test_encode.c encode.h types.h decode.h
	@gcc -c test_encode.c

decode.o: decode.c decode.h types.h common.h
	@gcc -c decode.c

.PHONY = clean
clean:
	@ls -1 *.txt *.o *lsb_steg* *stego_image.bmp* | grep -v -x  -e "secret.txt" -e "Error_logs.txt" | xargs rm -rf


.PHONY = ENCODE
INFILE ?=
ENCODE: beautiful.bmp secret.txt
	@if [ -z "$(INFILE)" ]; then \
		./lsb_steg -e beautiful.bmp secret.txt; \
	else \
		./lsb_steg -e beautiful.bmp $(INFILE); \
	fi
	@echo "To Decode execute 'make DECODE OUTFILE=<output_filename_with_extension>'"
	@echo "Default command 'make DECODE' will write secret message to Output.txt"

.PHONY = DECODE
OUTFILE ?=
DECODE: stego_image.bmp
	@if [ -z "$(OUTFILE)" ]; then \
		./lsb_steg -d stego_image.bmp; \
	else \
		./lsb_steg -d stego_image.bmp $(OUTFILE); \
	fi
	
