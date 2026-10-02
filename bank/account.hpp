#pragma once
#include <stdexcept>
#include <string>
#include <vector>

// Money is kept in integer cents so balances never pick up floating-point rounding errors.
using Cents = long long;

std::string formatMoney(Cents cents);  // 123456 -> "$1234.56"

class InsufficientFunds : public std::runtime_error {
public:
    InsufficientFunds(const std::string& accountId, Cents requested, Cents available);
};

struct Transaction {
    enum class Kind { Deposit, Withdrawal, TransferIn, TransferOut, Interest, Fee };
    Kind kind;
    Cents amount;        // always positive; the kind says which way it went
    Cents balanceAfter;
};

class Account {
public:
    Account(std::string id, std::string owner);
    virtual ~Account() = default;

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;

    const std::string& id() const { return id_; }
    const std::string& owner() const { return owner_; }
    Cents balance() const { return balance_; }
    const std::vector<Transaction>& history() const { return history_; }

    void deposit(Cents amount);
    void withdraw(Cents amount);

    // Month-end processing (interest, fees); each account type decides what happens.
    virtual void endOfMonth() = 0;
    virtual std::string type() const = 0;

protected:
    // How far below zero the balance may go. Plain accounts have no overdraft.
    virtual Cents overdraftLimit() const { return 0; }

    void requireFunds(Cents amount) const;
    void apply(Transaction::Kind kind, Cents signedAmount);

private:
    friend class Bank;  // transfers check funds and post both sides

    std::string id_;
    std::string owner_;
    Cents balance_ = 0;
    std::vector<Transaction> history_;
};

class SavingsAccount : public Account {
public:
    SavingsAccount(std::string id, std::string owner, double annualRate);

    void endOfMonth() override;  // pays one month of interest on a positive balance
    std::string type() const override { return "savings"; }

private:
    double annualRate_;
};

class CheckingAccount : public Account {
public:
    CheckingAccount(std::string id, std::string owner, Cents overdraft, Cents monthlyFee);

    void endOfMonth() override;  // charges the monthly fee
    std::string type() const override { return "checking"; }

protected:
    Cents overdraftLimit() const override { return overdraft_; }

private:
    Cents overdraft_;
    Cents monthlyFee_;
};
