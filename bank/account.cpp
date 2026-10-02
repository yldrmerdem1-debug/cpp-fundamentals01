#include "bank/account.hpp"

#include <cmath>
#include <utility>

std::string formatMoney(Cents cents) {
    const bool negative = cents < 0;
    const unsigned long long abs =
        negative ? 0ULL - static_cast<unsigned long long>(cents) : static_cast<unsigned long long>(cents);
    std::string fraction = std::to_string(abs % 100);
    if (fraction.size() < 2) fraction.insert(0, "0");
    return (negative ? "-$" : "$") + std::to_string(abs / 100) + "." + fraction;
}

InsufficientFunds::InsufficientFunds(const std::string& accountId, Cents requested, Cents available)
    : std::runtime_error("account " + accountId + ": requested " + formatMoney(requested) + ", available " +
                         formatMoney(available)) {}

Account::Account(std::string id, std::string owner) : id_(std::move(id)), owner_(std::move(owner)) {
    if (id_.empty()) throw std::invalid_argument("account id must not be empty");
}

void Account::deposit(Cents amount) {
    if (amount <= 0) throw std::invalid_argument("deposit must be positive");
    apply(Transaction::Kind::Deposit, amount);
}

void Account::withdraw(Cents amount) {
    if (amount <= 0) throw std::invalid_argument("withdrawal must be positive");
    requireFunds(amount);
    apply(Transaction::Kind::Withdrawal, -amount);
}

void Account::requireFunds(Cents amount) const {
    const Cents available = balance_ + overdraftLimit();
    if (amount > available) throw InsufficientFunds(id_, amount, available);
}

void Account::apply(Transaction::Kind kind, Cents signedAmount) {
    balance_ += signedAmount;
    history_.push_back({kind, signedAmount < 0 ? -signedAmount : signedAmount, balance_});
}

SavingsAccount::SavingsAccount(std::string id, std::string owner, double annualRate)
    : Account(std::move(id), std::move(owner)), annualRate_(annualRate) {
    if (annualRate_ < 0.0 || annualRate_ >= 1.0) throw std::invalid_argument("annual rate must be in [0, 1)");
}

void SavingsAccount::endOfMonth() {
    if (balance() <= 0) return;
    const Cents interest = std::llround(static_cast<double>(balance()) * annualRate_ / 12.0);
    if (interest > 0) apply(Transaction::Kind::Interest, interest);
}

CheckingAccount::CheckingAccount(std::string id, std::string owner, Cents overdraft, Cents monthlyFee)
    : Account(std::move(id), std::move(owner)), overdraft_(overdraft), monthlyFee_(monthlyFee) {
    if (overdraft_ < 0 || monthlyFee_ < 0) throw std::invalid_argument("overdraft and fee cannot be negative");
}

void CheckingAccount::endOfMonth() {
    if (monthlyFee_ > 0) apply(Transaction::Kind::Fee, -monthlyFee_);
}
