all: alarmServer RFSniffer

alarmServer: libs/*.o alarmServer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
	
RFSniffer: libs/*.o RFSniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
clean:
	$(RM) *.o alarmServer RFSniffer