// A month in the life of two accounts: deposits, a transfer, an overdraft, a declined
// withdrawal and month-end interest and fees, printed as statements.
#include <iomanip>
#include <iostream>
#include <memory>

#include "bank/bank.hpp"

namespace {

const char* kindName(Transaction::Kind kind) {
    switch (kind) {
        case Transaction::Kind::Deposit: return "deposit";
        case Transaction::Kind::Withdrawal: return "withdrawal";
        case Transaction::Kind::TransferIn: return "transfer in";
        case Transaction::Kind::TransferOut: return "transfer out";
        case Transaction::Kind::Interest: return "interest";
        case Transaction::Kind::Fee: return "fee";
    }
    return "?";
}

void printStatement(const Account& account) {
    std::cout << account.type() << " account " << account.id() << " (" << account.owner() << ")\n";
    for (const Transaction& t : account.history()) {
        std::cout << "  " << std::left << std::setw(14) << kindName(t.kind) << std::right << std::setw(12)
                  << formatMoney(t.amount) << "   balance " << formatMoney(t.balanceAfter) << '\n';
    }
}

}  // namespace

int main() {
    Bank bank;
    bank.open(std::make_unique<SavingsAccount>("S-100", "Ada Lovelace", 0.045));
    bank.open(std::make_unique<CheckingAccount>("C-200", "Ada Lovelace", 100'00, 4'50));

    bank.find("S-100").deposit(2500'00);
    bank.find("C-200").deposit(300'00);
    bank.transfer("S-100", "C-200", 150'00);
    bank.find("C-200").withdraw(520'00);  // dips into the overdraft

    try {
        bank.find("C-200").withdraw(100'00);
    } catch (const InsufficientFunds& e) {
        std::cout << "declined: " << e.what() << "\n\n";
    }

    bank.endOfMonth();
    printStatement(bank.find("S-100"));
    printStatement(bank.find("C-200"));
    std::cout << "\ntotal held: " << formatMoney(bank.totalHeld()) << '\n';
}
