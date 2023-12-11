/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "translate.h"

#include <libintl.h>
#include <qdebug.h>

#include <mutex>
#include <string>
#include <unordered_set>

#define LOCALEDIR "/usr/share/locale"

class UniversalPiece
{
public:
    UniversalPiece(std::pair<const char *, std::size_t> p)
        : piece_(p.first)
        , size_(p.second)
    {
    }

    template<typename T, typename = std::enable_if_t<std::is_same<T, char>::value>>
    UniversalPiece(const T *p)
        : piece_(p)
        , size_(std::char_traits<T>::length(p))
    {
    }

    UniversalPiece(const std::string &str)
        : piece_(str.data())
        , size_(str.size())
    {
    }

    UniversalPiece(char c) = delete;
#define UNIVERSAL_PIECE_NUMERIC_CONVERSION(TYPE, FORMAT_STRING)                \
    UniversalPiece(TYPE i)                                                     \
        : piece_(buffer_)                                                      \
    {                                                                          \
        auto size = snprintf(buffer_, sizeof(buffer_), FORMAT_STRING, i);      \
        assert(size >= 0 && static_cast<size_t>(size) + 1 <= sizeof(buffer_)); \
        size_ = size;                                                          \
    }
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(int, "%d");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(unsigned int, "%u");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(long, "%ld");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(unsigned long, "%lu");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(long long, "%lld");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(unsigned long long, "%llu");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(float, "%f");
    UNIVERSAL_PIECE_NUMERIC_CONVERSION(double, "%lf");

    UniversalPiece(const UniversalPiece &) = delete;

    const char *piece() const { return piece_; }

    std::size_t size() const { return size_; }

    std::pair<const char *, std::size_t> toPair() const { return { piece_, size_ }; }

    std::pair<const char *, std::size_t> toPathPair(const bool removePrefixSlash = true) const
    {
        const auto *piece = piece_;
        auto size = size_;
        // Consume prefix and suffix slash.
        if (removePrefixSlash) {
            while (size && piece[0] == '/') {
                ++piece;
                --size;
            }
        }
        while (size && piece[size - 1] == '/') {
            --size;
        }
        // If first component is all slash, keep all of them.
        if (size_ && !removePrefixSlash && !size) {
            return { piece_, size_ };
        }

        assert(size > 0);
        return { piece, size };
    }

private:
    static constexpr int IntegerBufferSize = 30;
    const char *piece_;
    std::size_t size_;
    char buffer_[IntegerBufferSize];
};

std::string concatPieces(std::initializer_list<std::pair<const char *, std::size_t>> list)
{
    std::size_t size = 0;
    for (auto pair : list) {
        size += pair.second;
    }
    std::string result;
    result.reserve(size);
    for (auto pair : list) {
        result.append(pair.first, pair.first + pair.second);
    }
    assert(result.size() == size);
    return result;
}

template<typename T>
struct UniversalPieceHelper
{
    static const T &forward(const T &t) { return t; }
};

template<typename... Args>
std::string concat(const Args &...args)
{
    return concatPieces(
        { static_cast<const UniversalPiece &>(UniversalPieceHelper<Args>::forward(args))
              .toPair()... });
}

namespace utilFuncs {

class GettextManager
{
public:
    void addDomain(const char *domain, const char *dir = nullptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (domains_.count(domain)) {
            return;
        }
        const auto *localedir = LOCALEDIR;
        if (!dir) {
            dir = localedir;
        }
        bindtextdomain(domain, dir);
        bind_textdomain_codeset(domain, "UTF-8");
        domains_.insert(domain);
        qDebug() << "Add gettext domain " << domain << " at " << dir;
    }

private:
    std::mutex mutex_;
    std::unordered_set<std::string> domains_;
};

static GettextManager gettextManager;

std::string translate(const std::string &s)
{
    return translate(s.c_str());
}

const char *translate(const char *s)
{
    return ::gettext(s);
}

std::string translateCtx(const char *ctx, const std::string &s)
{
    return translateCtx(ctx, s.c_str());
}

const char *translateCtx(const char *ctx, const char *s)
{
    auto str = concat(ctx, "\004", s);
    const auto *p = str.c_str();
    const auto *result = ::gettext(str.c_str());
    if (p == result) {
        return s;
    }
    return result;
}

std::string translateDomain(const char *domain, const std::string &s)
{
    return translateDomain(domain, s.c_str());
}

const char *translateDomain(const char *domain, const char *s)
{
    gettextManager.addDomain(domain);
    return ::dgettext(domain, s);
}

std::string translateDomainCtx(const char *domain, const char *ctx, const std::string &s)
{
    return translateDomainCtx(domain, ctx, s.c_str());
}

const char *translateDomainCtx(const char *domain, const char *ctx, const char *s)
{
    gettextManager.addDomain(domain);
    auto str = concat(ctx, "\004", s);
    const auto *p = str.c_str();
    const auto *result = ::dgettext(domain, p);
    if (p == result) {
        return s;
    }
    return result;
}

void registerDomain(const char *domain, const char *dir)
{
    gettextManager.addDomain(domain, dir);
}
} // namespace utilFuncs
