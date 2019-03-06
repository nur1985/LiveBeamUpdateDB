CXX       = g++
CXXFLAGS  = -g -Wall -fPIC -std=c++0x
OBJS      = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

INCLUDE  += -I$(EXTERNAL_release_area)/EXTERNAL/EXTERNAL_$(MINERVA_RELEASE)/NuConDB/src
LDFLAGS  += -shared -WL,-soname
LDLIBS   += -L$(EXTERNAL_release_area)/EXTERNAL/EXTERNAL_$(MINERVA_RELEASE)/NuConDB/$(CMTCONFIG) -lNuConDBLib

TARGET    = IFBeamData

print-%  : ; @echo $* = $($*)

$(TARGET): $(OBJS) 
	$(CXX) -o $@ $< $(LDLIBS)

$(OBJS): %.o : %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(wildcard *.o)
	rm -f $(TARGET)
