#include <cstdint>
#include <climits>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <tuple>


template <
    template <typename, typename> typename Container,
    typename Type,
    typename Allocator = std::allocator<Type>,
    typename = std::enable_if_t<
        std::is_same_v<Container<Type, Allocator>, std::vector<Type, Allocator>> ||
        std::is_same_v<Container<Type, Allocator>, std::list<Type, Allocator>>>>
std::ostream &operator<<(std::ostream &os, const Container<Type, Allocator> &container) {
    for (auto iter = std::begin(container); iter != std::end(container); ++iter) {
        if (iter != std::begin(container))
            std::cout << ".";
        os << *iter;
    }
    return os;
}

template <typename Type,
          std::enable_if_t<std::is_integral<Type>::value, bool> = true>
void print_ip(Type v)
{
    static_assert(CHAR_BIT == 8, "char = 8 bits is required");
    using uType = std::make_unsigned_t<Type>;
    std::vector<int> res(sizeof(Type), 0);
    uType new_v = v;
    int i = res.size() - 1;
    while (new_v) {
        res[i--] = new_v & ((1 << 8) - 1);
        new_v >>= 8;
    }
    std::cout << res << std::endl;
}

template <typename... Types>
void print_ip(const std::tuple<Types...>& container)
{
    size_t tsz = std::tuple_size_v<std::tuple<Types...>>;
    if (!tsz) return;
    using mtype = std::tuple_element_t<0, std::tuple<Types...>>;
    std::vector<mtype> res(tsz);
    std::apply([&res](Types const&... tuple_args)
        {
            size_t id{0};
            ((res[id++] = tuple_args), ...);
        }, container);
    std::cout << res << std::endl;
}

template <typename Container>
auto print_ip(const Container& container) -> decltype((void)container.size())
{
    std::cout << container << std::endl;
}

int main()
{
    print_ip(int8_t{-1}); // 255
    print_ip(int16_t{0}); // 0.0
    print_ip(int32_t{2130706433}); // 127.0.0.1
    print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"}); // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0
    return 0;
}
