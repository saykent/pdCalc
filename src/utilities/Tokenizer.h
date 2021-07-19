#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <istream>

namespace pdCalc {

class Tokenizer
{
public:
    using Token = std::string;
    using Tokens = std::vector<Token>;
    using const_iterator = Tokens::const_iterator;

    explicit Tokenizer(const std::string&);
    explicit Tokenizer(std::istream&);
    ~Tokenizer();

    size_t nTokens() const { return tokens_.size(); }

    const_iterator begin() const { return tokens_.begin(); }
    const_iterator end() const { return tokens_.end(); }

    const Token& operator[](size_t i) const { return tokens_[i]; }

    Tokenizer() = delete;
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer(Tokenizer&&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    Tokenizer& operator=(Tokenizer&&) = delete;

private:
    void tokenize(std::istream&);

    Tokens tokens_;
};

}
#endif //TOKENIZER_H
