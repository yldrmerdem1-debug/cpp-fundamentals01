#include "lru_cache/lru_cache.hpp"

#include <string>

#include "check.hpp"

int main() {
    LruCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");
    CHECK(cache.get(1).value() == "one");  // 1 becomes the most recent entry
    cache.put(3, "three");                  // so 2 is evicted
    CHECK(!cache.contains(2));
    CHECK(cache.contains(1) && cache.contains(3));
    CHECK(!cache.get(2).has_value());

    cache.put(1, "uno");  // updating keeps the size and refreshes recency
    CHECK(cache.size() == 2);
    cache.put(4, "four");  // evicts 3
    CHECK(cache.get(1).value() == "uno");
    CHECK(!cache.contains(3));

    CHECK(cache.erase(4));
    CHECK(!cache.erase(4));
    CHECK(cache.size() == 1);
    CHECK(cache.hits() == 2 && cache.misses() == 1);

    CHECK_THROWS((LruCache<int, int>(0)), std::invalid_argument);  // extra parentheses: the comma is not a macro separator
    return check::report("lru_cache");
}
