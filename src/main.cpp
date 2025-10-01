#include <cstdio>
#include <print>
#include <PicoSHA2/picosha2.h>
#include <sqlite3.h>

template <typename T>
using Vec = std::vector<T>;
template <typename T>
using Opt = std::optional<T>;
using String = std::string;

using byte = uint_least8_t;

// defer
#ifndef defer
struct defer_dummy {};
template <class F> struct deferrer { F f; ~deferrer() { f(); } };
template <class F> deferrer<F> operator*(defer_dummy, F f) { return {f}; }
#define DEFER_(LINE) zz_defer##LINE
#define DEFER(LINE) DEFER_(LINE)
#define defer auto DEFER(__LINE__) = defer_dummy{} *[&]()
#endif // defer

class Sqlite_handler {
    sqlite3 *ctx = nullptr;
public:
    Sqlite_handler(String filename) {
        if (sqlite3_open(filename.c_str(), &ctx) != SQLITE_OK) {
            std::println("Failed to create the sql database");
            std::abort();
        }
    }
    ~Sqlite_handler() {
        if (ctx) {
            if (sqlite3_close(ctx) != SQLITE_OK) {
                std::println("Failed to close the sql database");
                std::abort();
            } 
        }
    }
};

[[nodiscard]]
Vec<byte> load_file(String filename) {
    FILE *file = fopen(filename.c_str(),"rb");
    defer {
        fclose(file);
    };
    if (!file) {
        std::println("Failed to open the file!");
        return Vec<byte>(0);
    }
    if (fseek(file, 0, SEEK_END)) {
        std::println("Failed to load the file, seek end!");
        return Vec<byte>(0);
    }
    size_t fsize = ftell(file);
    if (fsize <= 0) {
        std::println("Failed to load the file, size count!");
        return Vec<byte>(0);
    }
    if (fseek(file, 0, SEEK_SET)) {
        std::println("Failed to load the file, seek start!");
        return Vec<byte>(0);
    }
    Vec<byte> buff(fsize);
    size_t n = fread(buff.data(), fsize, 1, file);
    if (n < 1) {
        std::println("Failed to load the file!");
        return Vec<byte>(0);
    }
    return std::move(buff);
};

int main () {
    auto sql = Sqlite_handler("o2db.db");
    auto file = load_file(
        "/home/gero/o2jam_duplicate_finder/files/30 Min harder/o2ma3626.ojn"
    );
    if (!file.size()) {
        std::abort();
    }
    // TODO: check, wiki says it's 0x52 for some reason?
    constexpr size_t TITLE_OFF = 0x66;
    constexpr size_t TITLE_SIZ = 58;

    constexpr size_t ARTIST_OFF = 0x8C;
    constexpr size_t ARTIST_SIZ = 64;

    constexpr size_t CREAT_OFF = 0xCC;
    constexpr size_t CREAT_SIZ = 32;

    constexpr size_t NOTE_OFF = 0x11C;

    Vec<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(file.begin() + NOTE_OFF, file.end(), hash.begin(), hash.end());
    String str_hash(hash.begin(), hash.end());

    std::println(
        "Artist: {} \nTitle: {} \nNotecharter: {} \nPattern hash: {}",
        String(file.begin() + ARTIST_OFF, file.begin() + ARTIST_OFF + ARTIST_SIZ),
        String(file.begin() + TITLE_OFF, file.begin() + TITLE_OFF + TITLE_SIZ),
        String(file.begin() + CREAT_OFF, file.begin() + CREAT_OFF + CREAT_SIZ),
        str_hash
    );

    file = load_file(
        "/home/gero/o2jam_duplicate_finder/files/MILK - RAY/o2ma99.ojn"
    );
    if (!file.size()) {
        std::abort();
    }

    picosha2::hash256(file.begin() + NOTE_OFF, file.end(), hash.begin(), hash.end());
    str_hash = String(hash.begin(), hash.end());

    std::println(
        "Artist: {} \nTitle: {} \nNotecharter: {} \nPattern hash: {}",
        String(file.begin() + ARTIST_OFF, file.begin() + ARTIST_OFF + ARTIST_SIZ),
        String(file.begin() + TITLE_OFF, file.begin() + TITLE_OFF + TITLE_SIZ),
        String(file.begin() + CREAT_OFF, file.begin() + CREAT_OFF + CREAT_SIZ),
        str_hash
    );
    return 0;
}
