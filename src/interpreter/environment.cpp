#include "environment.h"
#include "runtime_error.h"

#include <limits>
#include <stdexcept>
#include <utility>

Environment::Environment(std::shared_ptr<Environment> parent, std::size_t expected_values)
    : parent_(std::move(parent)) {
    values_.reserve(expected_values);
    depth_cache_.reserve(expected_values);
}

void Environment::reset(std::shared_ptr<Environment> parent, std::size_t expected_values) {
    parent_ = std::move(parent);
    values_.clear();
    depth_cache_.clear();
    if (values_.bucket_count() < expected_values)
        values_.reserve(expected_values);
    if (depth_cache_.bucket_count() < expected_values)
        depth_cache_.reserve(expected_values);
}

SymbolId Environment::token_symbol(const Token &name) const {
    return name.symbol != InvalidSymbol ? name.symbol : intern_symbol(name.lexeme);
}

void Environment::define(const Token &name, Value value) {
    const SymbolId symbol = token_symbol(name);
    const bool inserted = values_.try_emplace(symbol, std::move(value)).second;
    if (!inserted)
        throw RuntimeError(name, "variable '" + name.lexeme + "' is already declared in this scope");
    depth_cache_[symbol] = 0;
}

void Environment::define(std::string name, Value value) {
    const SymbolId symbol = intern_symbol(name);
    const bool inserted = values_.try_emplace(symbol, std::move(value)).second;
    if (!inserted)
        throw std::logic_error("global value '" + name + "' is already defined");
    depth_cache_[symbol] = 0;
}

std::size_t Environment::resolve_depth(const Token &name, SymbolId symbol) const {
    if (const auto cached = depth_cache_.find(symbol); cached != depth_cache_.end())
        return cached->second;

    std::size_t depth = 0;
    for (const Environment *environment = this; environment != nullptr;
         environment = environment->parent_.get(), ++depth) {
        if (environment->values_.contains(symbol)) {
            depth_cache_.emplace(symbol, depth);
            return depth;
        }
    }

    throw RuntimeError(name, "undefined variable '" + name.lexeme + "'");
}

const Environment *Environment::ancestor(std::size_t depth) const {
    const Environment *environment = this;
    while (depth-- > 0)
        environment = environment->parent_.get();
    return environment;
}

Environment *Environment::ancestor(std::size_t depth) {
    Environment *environment = this;
    while (depth-- > 0)
        environment = environment->parent_.get();
    return environment;
}

Value Environment::get(const Token &name) const {
    const SymbolId symbol = token_symbol(name);
    const Environment *environment = ancestor(resolve_depth(name, symbol));
    return environment->values_.find(symbol)->second;
}

void Environment::assign(const Token &name, Value value) {
    const SymbolId symbol = token_symbol(name);
    Environment *environment = ancestor(resolve_depth(name, symbol));
    environment->values_.find(symbol)->second = std::move(value);
}

std::shared_ptr<Environment> Environment::parent() const { return parent_; }
