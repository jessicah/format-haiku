# format-haiku Makefile

SRCS = AffectedRangeManager.cpp BreakableToken.cpp Comments.cpp \
	ContinuationIndenter.cpp FormatToken.cpp TokenAnnotator.cpp \
	FormatTokenLexer.cpp TokenAnalyzer.cpp UnwrappedLineFormatter.cpp \
	UnwrappedLineParser.cpp WhitespaceManager.cpp Format.cpp

LIBS = LLVMSupport clangBasic clangLex clangRewrite clangAST clangToolingCore

format-haiku: $(SRCS) FormatHaiku.cpp
	clang++ -o format-haiku $+ -I ~/include \
		-fPIC -std=c++11 -L ~/lib -pthreads -ldl -lncurses -fno-rtti \
		-lclangToolingCore -lclangAST -lclangRewrite -lclangLex -lclangBasic \
		-lLLVMCore -lLLVMSupport -lLLVMDemangle
