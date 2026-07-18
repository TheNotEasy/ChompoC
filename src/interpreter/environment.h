#pragma once

#include "lexer/token.h"
#include "value.h"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

class Environment {
public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr, std::size_t expected_values = 8);

    void reset(std::shared_ptr<Environment> parent, std::size_t expected_values = 8);

    void define(std::string name, Value value);
    void define(const Token &name, Value value);

    Value get(const Token &name) const;
    void assign(const Token &name, Value value);

    std::shared_ptr<Environment> parent() const;

private:
    using Values = std::unordered_map<SymbolId, Value>;

    SymbolId token_symbol(const Token &name) const;
    std::size_t resolve_depth(const Token &name, SymbolId symbol) const;
    const Environment *ancestor(std::size_t depth) const;
    Environment *ancestor(std::size_t depth);

    Values values_;
    mutable std::unordered_map<SymbolId, std::size_t> depth_cache_;
    std::shared_ptr<Environment> parent_;
};
