main: main.o interpreter.o parser.o lexer.o
	c++ -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -stdlib=libc++ \
	-o main main.o interpreter.o parser.o lexer.o

parser.o: ./parser/parser.h ./parser/parser.cpp
	c++ -stdlib=libc++ -c -o parser.o ./parser/parser.cpp

lexer.o: ./lexer/lexer.h ./lexer/lexer.cpp
	c++ -stdlib=libc++ -c -o lexer.o ./lexer/lexer.cpp

clean:
	rm parser_test *.o