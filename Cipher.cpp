#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <utility>
#include <vector>

long long char_to_num(char symbol) {
    if (symbol >= 48 && symbol <= 57)
        return symbol - 48;
    if (symbol >= 65 && symbol <= 90)
        return symbol - 55;
    if (symbol >= 97 && symbol <= 122)
        return symbol - 61;
    if (symbol == 32)
        return 62;
    if (symbol == 46)
        return 63;
    return 64;
}

struct BigInt {
    std::vector<uint64_t> nums;
    uint64_t num_system = 10;

    void DeleteZeros() {
        while (!nums.empty() && nums.back() == 0) {
            nums.pop_back();
        }
    }

    BigInt() = default;

    explicit BigInt(std::vector<uint64_t>  x, uint64_t s = 10) : nums(std::move(x)), num_system(s) {
    }

    BigInt(const BigInt& other) : nums(other.nums), num_system(other.num_system) {
    }

    explicit BigInt(uint64_t n, uint64_t ns = 10) : num_system(ns) {
        while (n) {
            nums.push_back(n % num_system);
            n /= num_system;
        }
    }

    BigInt& operator=(const BigInt& other) {
        nums = other.nums;
        return *this;
    }

    uint64_t operator[](int i) {
        return nums[i];
    }

    uint64_t operator[](int i) const {
        return nums[i];
    }

    [[nodiscard]] size_t size() const {
        return nums.size();
    }

    BigInt& operator+=(const BigInt& rhs) {
        uint64_t carry = 0, s = std::max(size(), rhs.size()) + 1;
        nums.resize(s);
        for (int i = 0; i < s; ++i) {
            nums[i] += (i < rhs.size() ? rhs[i] : 0) + carry;
            carry = nums[i] / num_system;
            nums[i] %= num_system;
        }
        DeleteZeros();
        return *this;
    }

    BigInt& operator+=(uint64_t rhs) {
        *this += BigInt(rhs, num_system);
        return *this;
    }

    BigInt& operator*=(uint64_t rhs) {
        *this = *this * rhs;
        return *this;
    }

    BigInt operator*(uint64_t rhs) {
        uint64_t carry = 0, i = 0;
        std::vector<uint64_t> res;
        while (i < size() || carry) {
            uint64_t now = (i < size() ? nums[i] : 0) * rhs + carry;
            res.push_back(now % num_system);
            carry = now / num_system;
            ++i;
        }
        return BigInt(res, num_system);
    }

    void Print() {
        if (nums.empty()) {
            std::cout << 0 << "\n";
            return;
        }
        for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }

    friend BigInt operator+(const BigInt& lhs_, const BigInt& rhs_) {
        BigInt result(lhs_);
        result += rhs_;
        return result;
    }
};

class Cipher {
private:
    uint64_t p, g, k;

    uint64_t Pow(uint64_t num, uint64_t deg) {
        if (deg == 0) {
            return 1;
        }
        if (deg % 2 != 0) {
            return (num * Pow(num, deg - 1)) % p;
        }
        uint64_t base = Pow(num, deg / 2);
        return (base * base) % p;
    }

public:
    Cipher() {
        std::cin >> p >> g >> k;
    }

    void Encode(const std::string& message) {
        BigInt number({}, 64);
        number.nums.resize(message.size());
        std::transform(message.begin(), message.end(), number.nums.begin(), char_to_num);
        // number.Print();
        BigInt changed({}, p);
        for (auto it = number.nums.rbegin(); it != number.nums.rend(); ++it) {
            changed *= number.num_system;
            changed += *it;
        }
        changed.DeleteZeros();
        // changed.Print();
        for (auto n : changed.nums) {
            uint64_t b = 1 + rand() % (p - 1);
            // std::cout << b << "---------------------------------------\n";
            std::cout << Pow(g, b) << " " << (n * Pow(k, b)) % p << "\n";
        }
    }

    void Decode() {
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::mt19937 rand;

    Cipher cipher;
    std::string message;
    std::cin.ignore();
    std::getline(std::cin, message);
    std::cout << message << "\n";
    cipher.Encode('\r' + message);
    return 0;
}