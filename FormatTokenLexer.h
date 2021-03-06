//===--- FormatTokenLexer.h - Format C++ code ----------------*- C++ ----*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains FormatTokenLexer, which tokenizes a source file
/// into a token stream suitable for ClangFormat.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_FORMAT_FORMATTOKENLEXER_H
#define LLVM_CLANG_LIB_FORMAT_FORMATTOKENLEXER_H

#include "Encoding.h"
#include "FormatToken.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Format/Format.h"
#include "llvm/Support/Regex.h"

#include <stack>

namespace clang {
namespace format {

enum LexerState {
  NORMAL,
  TEMPLATE_STRING,
  TOKEN_STASHED,
};

class FormatTokenLexer {
public:
  FormatTokenLexer(const SourceManager &SourceMgr, FileID ID,
                   const FormatStyle &Style, encoding::Encoding Encoding);

  ArrayRef<FormatToken *> lex();

  const AdditionalKeywords &getKeywords() { return Keywords; }

private:
  void tryMergePreviousTokens();

  bool tryMergeLessLess();

  bool tryMergeTokens(ArrayRef<tok::TokenKind> Kinds, TokenType NewType);

  bool tryMerge_TMacro();

  bool tryMergeConflictMarkers();

  FormatToken *getStashedToken();

  FormatToken *getNextToken();

  FormatToken *FormatTok;
  bool IsFirstToken;
  std::stack<LexerState> StateStack;
  unsigned Column;
  unsigned TrailingWhitespace;
  std::unique_ptr<Lexer> Lex;
  const SourceManager &SourceMgr;
  FileID ID;
  const FormatStyle &Style;
  IdentifierTable IdentTable;
  AdditionalKeywords Keywords;
  encoding::Encoding Encoding;
  llvm::SpecificBumpPtrAllocator<FormatToken> Allocator;
  // Index (in 'Tokens') of the last token that starts a new line.
  unsigned FirstInLineIndex;
  SmallVector<FormatToken *, 16> Tokens;
  SmallVector<IdentifierInfo *, 8> ForEachMacros;

  bool FormattingDisabled;

  llvm::Regex MacroBlockBeginRegex;
  llvm::Regex MacroBlockEndRegex;

  void readRawToken(FormatToken &Tok);

  void resetLexer(unsigned Offset);
};

} // namespace format
} // namespace clang

#endif
