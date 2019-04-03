sirvia: sirvia.o connect.o map.o fileImport.o stringUtil.o urls.o handler.o cJSON.o regx.o templateSplite.o
sirvia.o: sirvia.c
	gcc -c sirvia.c -o sirvia.o
connect.o: network_tool/connect.c
	gcc -c network_tool/connect.c -o connect.o
map.o: utils/map.c
	gcc -c utils/map.c -o map.o
fileImport.o: utils/fileImport.c
	gcc -c utils/fileImport.c -o fileImport.o
stringUtil.o: utils/stringUtil.c
	gcc -c utils/stringUtil.c -o stringUtil.o
urls.o: urls/urls.c
	gcc -c urls/urls.c -o urls.o
handler.o: handler/handler.c
	gcc -c handler/handler.c -o handler.o
cJSON.o: utils/cJSON.c
	gcc -c utils/cJSON.c -o cJSON.o -lm
regx.o: utils/regx.c
	gcc -c utils/regx.c -o regx.o
templateSplite.o: utils/templateSplite.c
	gcc -c utils/templateSplite.c -o templateSplite.o

clean:
	rm -rf *.o
