#pragma once
#include <cstddef>
#include <map>
#include <memory>
#include <string>

#include "bank/account.hpp"

// Owns the accounts and runs operations that touch more than one of them.
class Bank {
public:
    Account& open(std::unique_ptr<Account> account);  // throws on a duplicate id
    Account& find(const std::string& id);              // throws std::out_of_range

    // All-or-nothing: either both sides are posted or nothing changes.
    void transfer(const std::string& fromId, const std::string& toId, Cents amount);

    void endOfMonth();
    Cents totalHeld() const;
    std::size_t size() const { return accounts_.size(); }

private:
    std::map<std::string, std::unique_ptr<Account>> accounts_;
};
