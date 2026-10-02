#include <memory>
#include <stdexcept>

#include "bank/bank.hpp"
#include "check.hpp"

int main() {
    Bank bank;
    bank.open(std::make_unique<SavingsAccount>("S-1", "Ada", 0.06));
    bank.open(std::make_unique<CheckingAccount>("C-1", "Ada", 50'00, 5'00));

    bank.find("S-1").deposit(1000'00);
    bank.transfer("S-1", "C-1", 200'00);
    CHECK(bank.find("S-1").balance() == 800'00);
    CHECK(bank.find("C-1").balance() == 200'00);

    // Checking may go 50.00 below zero, not further.
    bank.find("C-1").withdraw(240'00);
    CHECK(bank.find("C-1").balance() == -40'00);
    CHECK_THROWS(bank.find("C-1").withdraw(20'00), InsufficientFunds);
    CHECK_THROWS(bank.find("S-1").withdraw(900'00), InsufficientFunds);
    CHECK(bank.find("S-1").balance() == 800'00);  // a declined withdrawal changes nothing

    bank.endOfMonth();
    CHECK(bank.find("S-1").balance() == 804'00);  // 6% a year = 0.5% a month on 800.00
    CHECK(bank.find("C-1").balance() == -45'00);  // 5.00 monthly fee
    CHECK(bank.totalHeld() == 759'00);
    CHECK(bank.find("S-1").history().size() == 3);  // deposit, transfer out, interest

    CHECK_THROWS(bank.find("X-9"), std::out_of_range);
    CHECK_THROWS(bank.transfer("S-1", "S-1", 1), std::invalid_argument);
    CHECK_THROWS(bank.transfer("S-1", "C-1", 0), std::invalid_argument);
    CHECK_THROWS(bank.open(std::make_unique<SavingsAccount>("S-1", "Bob", 0.01)), std::invalid_argument);
    CHECK_THROWS(SavingsAccount("S-2", "Bob", 1.5), std::invalid_argument);

    CHECK(formatMoney(-45'00) == "-$45.00");
    CHECK(formatMoney(5) == "$0.05");
    CHECK(formatMoney(123456) == "$1234.56");
    return check::report("bank");
}
