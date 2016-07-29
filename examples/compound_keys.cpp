#include <array>
#include <iostream>
#include <art/radix_map.h>

// We want to use pointers to officers as keys for a radix_map.
// As key we will use the compound key of (rank, brigade, division)
// where want to store the elements in descending order of the rank
// and then ascending order of brigade and division.

// IMPORTANT: Path compression is not implemented yet, meaning that long
// compound keys will lead to a big space overhead and a drop in performance!!!
// At this point in time, this is more of a proof of concept type of example.

using std::string;
struct Officer {
    short rank;
    int brigade;
    long division;

    string first_name;
    string last_name;

    Officer() {}

    Officer(short rank, int brigade, long division, string first_name, string last_name)
            : rank(rank), brigade(brigade), division(division), first_name(first_name), last_name(last_name) {}
};

std::ostream &operator<<(std::ostream &os, const Officer &officer) {
    os << officer.first_name << " " << officer.last_name;
    return os;
}

// Custom key_definition that takes pointer to officers as input
namespace art
{
    // The return type of our key transformation will be a byte array. Its size is
    // equal to the sum of its components.
    typedef std::array<uint8_t, sizeof(short) + sizeof(int) + sizeof(long)> trans_key_type;

    template<>
    struct key_transform<Officer *> {
        trans_key_type operator()(const Officer *officer) const noexcept {
            trans_key_type transformed_key{};

            // Transform individual keys to make them binary comparable
            // We want to sort by rank in descending order, that's why its value is inverted
            auto p1 = art::key_transform<short>()(-officer->rank);
            auto p2 = art::key_transform<int>()(officer->brigade);
            auto p3 = art::key_transform<long>()(officer->division);

            // Move the individual parts into an array in the desired order of importance
            // rank | brigade | division
            std::move(static_cast<const char *>(static_cast<const void *>(&p1)),
                      static_cast<const char *>(static_cast<const void *>(&p1)) + sizeof(p1),
                      transformed_key.begin());

            size_t index = sizeof(p1);
            std::move(static_cast<const char *>(static_cast<const void *>(&p2)),
                      static_cast<const char *>(static_cast<const void *>(&p2)) + sizeof(p2),
                      transformed_key.begin() + index);

            index += sizeof(p2);
            std::move(static_cast<const char *>(static_cast<const void *>(&p3)),
                      static_cast<const char *>(static_cast<const void *>(&p3)) + sizeof(p3),
                      transformed_key.begin() + index);

            return transformed_key;
        }
    };
}

int main() {
    using std::cout;
    using std::endl;

    art::radix_map<Officer *, std::string> m;

    Officer *major = new Officer(8, 251, 2312311, "Major", "Will");
    Officer *colonel = new Officer(9, 251, 5435345, "Colonel", "John");
    Officer *general1 = new Officer(10, 57, 41, "Brigade General", "Lewis");
    Officer *general2 = new Officer(10, 251, 41, "Brigade General", "Tom");

    m.emplace(major, "Actually fighting");
    m.emplace(colonel, "Wants to be a general");
    m.emplace(general1, "Chilling in his office");
    m.emplace(general2, "Looking serious");

    for (auto &e : m)
        cout << *e.first << " -> " << e.second << endl;

    return 0;
}