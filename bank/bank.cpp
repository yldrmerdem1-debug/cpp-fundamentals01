#include "bank/bank.hpp"

#include <stdexcept>
#include <utility>

Account& Bank::open(std::unique_ptr<Account> account) {
    if (!account) throw std::invalid_argument("cannot open a null account");
    const std::string id = account->id();
    auto [it, inserted] = accounts_.emplace(id, std::move(account));
    if (!inserted) throw std::invalid_argument("duplicate account id " + id);
    return *it->second;
}

Account& Bank::find(const std::string& id) {
    auto it = accounts_.find(id);
    if (it == accounts_.end()) throw std::out_of_range("no account " + id);
    return *it->second;
}

void Bank::transfer(const std::string& fromId, const std::string& toId, Cents amount) {
    if (fromId == toId) throw std::invalid_argument("cannot transfer to the same account");
    if (amount <= 0) throw std::invalid_argument("transfer must be positive");
    Account& from = find(fromId);
    Account& to = find(toId);
    from.requireFunds(amount);  // check before touching either balance
    from.apply(Transaction::Kind::TransferOut, -amount);
    to.apply(Transaction::Kind::TransferIn, amount);
}

void Bank::endOfMonth() {
    for (auto& [id, account] : accounts_) account->endOfMonth();
}

Cents Bank::totalHeld() const {
    Cents total = 0;
    for (const auto& [id, account] : accounts_) total += account->balance();
    return total;
}
